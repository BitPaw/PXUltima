#include "MTL.h"

#include <File/DataStream.h>
#include <Text/Text.h>
#include <Container/ClusterValue.h>
#include <Memory/PXMemory.h>
#include <Compiler/Compiler.h>

void MTLConstruct(MTL* const mtl)
{
	MemorySet(mtl, sizeof(MTL), 0);
}

void MTLDestruct(MTL* const mtl)
{
	// Loop through MaterialListSize

	MemoryRelease(mtl->MaterialList, mtl->MaterialListSize);

	mtl->MaterialListSize = 0;
	mtl->MaterialList = 0;
}

IlluminationMode MTLIlluminationModeFromID(const unsigned int illuminationModeID)
{
	switch (illuminationModeID)
	{
		case 0u: return IlluminationColorAndAmbientDisable;
		case 1u: return IlluminationColorAndAmbientEnable;
		case 2u: return IlluminationHighlightEnable;
		case 3u: return IlluminationReflectionOnRayTraceEnable;
		case 4u: return IlluminationReflectionOnRayTraceTransparency;
		case 5u: return IlluminationReflectionOnRayTraceFresnel;
		case 6u: return IlluminationReflectionOnRayTraceTransparencyFresnel;
		case 7u: return IlluminationReflectionOnRayTraceFullEnable;
		case 8u: return IlluminationReflectionEnable;
		case 9u: return IlluminationTransparencyEnable;
		case 10u: return IlluminationShadowsEnable;

		default:
			return IlluminationNone;
	}
}

MTLLineType MTLPeekLine(const char* const line, const size_t lineSize)
{
	const unsigned short tagID = MakeShort(line[0], line[1]);

	switch(tagID)
	{
		case MakeShort('m', 'a'): return MTLLineTexture; // map_Kd
		case MakeShort('n', 'e'): return MTLLineName; // newmtl
		case MakeShort('N', 's'): return MTLLineWeight;
		case MakeShort('N', 'i'): return MTLLineDensity;
		case MakeShort('K', 'a'): return MTLLineColorAmbient;
		case MakeShort('K', 'd'): return MTLLineColorDiffuse;
		case MakeShort('K', 's'): return MTLLineColorSpecular;
		case MakeShort('K', 'e'): return MTLLineColorEmission;
		case MakeShort('d', ' '): return MTLLineDissolved;
		case MakeShort('i', 'l'): return MTLLineIllumination; // illum
		default:
			return MTLLineInvalid;
	}
}

ActionResult MTLFileCompile(DataStream* const inputStream, DataStream* const outputStream)
{
	DataStream tokenSteam;
	DataStream headerStream;

	const size_t startoffset = outputStream->DataCursor;

	// temo
	unsigned int materialAmount = 0;
	unsigned int materialSizeDelta = 0;

	const size_t headerOffset = 1024;


	DataStreamFromExternal(&headerStream, (unsigned char*)outputStream->Data + startoffset, headerOffset);

	// Lexer - Level I 
	{	
		PXCompilerSettings compilerSettings;
		compilerSettings.KeepWhiteSpace = PXNo;
		compilerSettings.KeepWhiteSpaceIndentationLeft = PXNo;
		compilerSettings.TryAnalyseTypes = PXYes;
		compilerSettings.IntrepredNewLineAsWhiteSpace = PXNo;
		compilerSettings.KeepTabs = PXNo;
		compilerSettings.IntrepredTabsAsWhiteSpace = PXYes;
		compilerSettings.CommentsKeep = PXNo;
		compilerSettings.CommentSingleLineSize = 1u;
		compilerSettings.CommentSingleLine = "#";
		compilerSettings.CommentMultibleLineSize = 0;
		compilerSettings.CommentMultibleLine = 0;

		outputStream->DataCursor += headerOffset;

		PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

		DataStreamFromExternal(&tokenSteam, (unsigned char*)outputStream->Data + startoffset + headerOffset, outputStream->DataCursor - (startoffset + headerOffset));
				
		outputStream->DataCursor = startoffset;
	}

	MemorySet(headerStream.Data, headerOffset, 0);
	DataStreamCursorAdvance(&headerStream, sizeof(unsigned int)); // Keep the number of materials free	
	DataStreamCursorAdvance(outputStream, headerOffset); // Write 0'ed data to later jump back to to change.

	while (!DataStreamIsAtEnd(&tokenSteam))
	{
		PXCompilerSymbolEntry compilerSymbolEntry;

		PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

		switch (compilerSymbolEntry.ID)
		{
			case PXCompilerSymbolLexerComment:
			case PXCompilerSymbolLexerNewLine:
				continue;
		}

		const MTLLineType mtlLineType = MTLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

		DataStreamWriteCU(outputStream, mtlLineType); // Write typeID

		switch (mtlLineType)
		{
			case MTLLineName:
			{
				++materialAmount;

				// write how big the last material was, it will be 0 the first time.
				// At the end, convert the delte size to actual size.
				DataStreamWriteIU(&headerStream, materialSizeDelta, EndianLittle);

				// [No break] Fall through.
			}
			case MTLLineTexture:
			{
				char text[256];

				//char* dataPosition = DataStreamCursorPosition(outputStream);
				//const size_t dataSize = DataStreamRemainingSize(outputStream);
				size_t dataSizeWritten = 0;
				const PXBool isText = PXCompilerParseStringUntilNewLine(&tokenSteam, &compilerSymbolEntry, text, 256, &dataSizeWritten);

				if (!isText)
				{
					break; // Error
				}

				materialSizeDelta += DataStreamWriteCU(outputStream, dataSizeWritten);
				materialSizeDelta += DataStreamWriteA(outputStream, text, dataSizeWritten);

				break;
			}
			case MTLLineWeight:
			case MTLLineDissolved:
			case MTLLineDensity:
			{
				float value = 0;
				const PXBool isFloatParsed = PXCompilerParseFloatSingle(&tokenSteam, &compilerSymbolEntry, &value);

				if (!isFloatParsed)
				{
					// Error
					break;
				}

				materialSizeDelta += DataStreamWriteF(outputStream, value);

				break;
			}
			case MTLLineColorAmbient:
			case MTLLineColorDiffuse:
			case MTLLineColorSpecular:
			case MTLLineColorEmission:
			{
				size_t valuesDetected = 0;
				const size_t colorVectorSize = 3u;
				float colorVector[3] = { -1, -1, -1 };

				const PXBool listParsed = PXCompilerParseFloatList(&tokenSteam, &compilerSymbolEntry, colorVector, colorVectorSize, &valuesDetected);

				if (!listParsed)
				{
					// Error
					break;
				}

				materialSizeDelta += DataStreamWriteP(outputStream, colorVector, sizeof(float) * colorVectorSize);

				break;
			}
			case MTLLineIllumination:
			{
				unsigned int value = 0;

				const PXBool isInt = PXCompilerParseIntUnsignedSingle(&tokenSteam, &compilerSymbolEntry, &value);

				if (!isInt)
				{
					// Error
					break;
				}

				const IlluminationMode illuminationMode = MTLIlluminationModeFromID(value);

				materialSizeDelta += DataStreamWriteCU(outputStream, illuminationMode);

				break;
			}
			default:
				break;
		}
	}

	// Write header
	{
		headerStream.DataCursor = 0;

		DataStreamWriteIU(&headerStream, materialAmount, EndianLittle);


		unsigned int sizeA = 0;
		unsigned int sizeB = 0;

		for (size_t i = 0; i < materialAmount-1; ++i)
		{		
			DataStreamReadIU(&headerStream, &sizeA, EndianLittle);
			DataStreamReadIU(&headerStream, &sizeB, EndianLittle);

			headerStream.DataCursor -= sizeof(unsigned int) * 2u;

			DataStreamWriteIU(&headerStream, sizeB - sizeA, EndianLittle);
		}

		const size_t positionA = headerStream.DataCursor;

		DataStreamReadIU(&headerStream, &sizeA, EndianLittle);

        DataStreamWriteAtIU(&headerStream, materialSizeDelta- sizeA, EndianLittle, positionA);
	}

	//outputStream->DataCursor = startoffset + tokenSteam.DataCursor; // Only save the actuan size

	return ActionSuccessful;
}

size_t MTLFetchAmount(const void* const data, const size_t dataSize)
{
	DataStream mtlStream;

	if (!data || !dataSize)
	{
		return 0;
	}

	DataStreamFromExternal(&mtlStream, data, dataSize);

	unsigned int materialListSize = 0;

	DataStreamReadIU(&mtlStream, &materialListSize, EndianLittle);

	return materialListSize;
}

PXBool MTLFetchMaterial(const void* const data, const size_t dataSize, const size_t materialID, MTLMaterial* const mtlMaterial)
{
	const size_t amount = MTLFetchAmount(data, dataSize);

	MemorySet(mtlMaterial, sizeof(MTLMaterial), 0);

	if (materialID > amount) // Material ID wrong/Too high
	{
		return PXNo;
	}

	DataStream mtlDataStream;
	DataStream mtlHeaderStream;

	DataStreamFromExternal(&mtlDataStream, (unsigned char*)data + 1024, dataSize - sizeof(unsigned int)); // Skip first int, we already got it
	DataStreamFromExternal(&mtlHeaderStream, (unsigned char*)data + sizeof(unsigned int), dataSize- sizeof(unsigned int)); // Skip first int, we already got it

	for (size_t i = 0; i <= materialID; ++i)
	{
		const PXBool isTarget = materialID == i;
		unsigned int materialDataSize = 0;

		DataStreamReadIU(&mtlHeaderStream, &materialDataSize, EndianLittle);

		if (isTarget)
		{
			mtlDataStream.DataSize = materialDataSize;	// Set max size for now

			while (!DataStreamIsAtEnd(&mtlDataStream))
			{
				MTLLineType mtlLineType;

				{
					unsigned char lineTypeID = 0;

					DataStreamReadCU(&mtlDataStream, &lineTypeID);

					mtlLineType = lineTypeID;
				}

				switch (mtlLineType)
				{
					case MTLLineName:
					{
						DataStreamReadCU(&mtlDataStream, &mtlMaterial->NameSize);
						mtlMaterial->Name = (char*)DataStreamCursorPosition(&mtlDataStream);

						DataStreamCursorAdvance(&mtlDataStream, mtlMaterial->NameSize);
						break;
					}
					case MTLLineTexture:
					{
						DataStreamReadCU(&mtlDataStream, &mtlMaterial->DiffuseTexturePathSize);
						mtlMaterial->DiffuseTexturePath = (char*)DataStreamCursorPosition(&mtlDataStream);

						DataStreamCursorAdvance(&mtlDataStream, mtlMaterial->DiffuseTexturePathSize);
						break;
					}
					case MTLLineColorAmbient:
					{
						DataStreamReadP(&mtlDataStream, mtlMaterial->Ambient, sizeof(float) * 3u);
						break;
					}
					case MTLLineColorDiffuse:
					{
						DataStreamReadP(&mtlDataStream, mtlMaterial->Diffuse, sizeof(float) * 3u);
						break;
					}
					case MTLLineColorSpecular:
					{
						DataStreamReadP(&mtlDataStream, mtlMaterial->Specular, sizeof(float) * 3u);
						break;
					}
					case MTLLineColorEmission:
					{
						DataStreamReadP(&mtlDataStream, mtlMaterial->Emission, sizeof(float) * 3u);
						break;
					}
					case MTLLineWeight:
						DataStreamReadF(&mtlDataStream, &mtlMaterial->Weight);
						break;

					case MTLLineDissolved:
						DataStreamReadF(&mtlDataStream, &mtlMaterial->Dissolved);
						break;

					case MTLLineDensity:
						DataStreamReadF(&mtlDataStream, &mtlMaterial->Density);
						break;

					case MTLLineIllumination:
					{
						unsigned char illuminationID = 0;

						DataStreamReadCU(&mtlDataStream, &illuminationID);

						mtlMaterial->Illumination = illuminationID;
						break;
					}						

					default:
					{
#if 0 // Skip or fail if anything is wrong
						DataStreamCursorAdvance(&mtlDataStream, sizeof(unsigned char)); // Handle
						break;
#else
						MemorySet(mtlMaterial, sizeof(MTLMaterial), 0);
						return PXNo;
#endif						
					}
				}

			}
		}
		else 		// else, do nothing -> skip
		{
			(unsigned char*)mtlDataStream.Data += materialDataSize+10u; // accumulate Size, missing 10 Bytes??
		}
	}

	return PXYes;
}

ActionResult MTLParseToMaterial(DataStream* const inputStream, PXMaterialList* const pxMaterialList)
{
	/*
	DataStream

	MemorySet(pxMaterialList, sizeof(PXMaterialList), 0);

	unsigned int amountOfMaterials = 0;

	DataStreamRead(inputStream, &amountOfMaterials, EndianLittle);

	DataStreamWriteIU();


	while (DataStreamIsAtEnd(&inputStream))
	{
		MTLLineType mtlLineType;

		{
			unsigned char lineTypeID = 0;

			DataStreamReadCU(inputStream, &lineTypeID); // Line Type     

			mtlLineType = lineTypeID;
		}


	}*/
}



/*
ActionResult MTLParse(MTL* mtl, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;

	MTLConstruct(mtl);
	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);
	*dataRead = 0;

	// Count How many materials are needed
	{
		size_t materialCounter = 0;

		do
		{
			const unsigned char* currentLine = DataStreamCursorPosition(&dataStream);
			const unsigned char isNewMaterialUsed = *currentLine == 'n';

			if(isNewMaterialUsed)
			{
				++materialCounter;
			}
		}
		while(DataStreamSkipLine(&dataStream));

		mtl->MaterialListSize = materialCounter;
		mtl->MaterialList = MemoryAllocate(sizeof(MTLMaterial) * materialCounter);

		MemorySet(mtl->MaterialList, sizeof(MTLMaterial) * materialCounter, 0);

		DataStreamCursorToBeginning(&dataStream);
	}

	// Raw Parse
	MTLMaterial* material = 0; // current material, has to be here, its state dependend
	size_t materialIndex = 0;

	do
	{
		const char* currentLine = DataStreamCursorPosition(&dataStream);
		const MTLLineType lineType = MTLPeekLine(currentLine);
			
		DataStreamSkipBlock(&dataStream); // Skip first element

		const char* dataPoint = DataStreamCursorPosition(&dataStream);
		const size_t maxSize = DataStreamRemainingSize(&dataStream);
		const size_t lineSize = TextLengthUntilA(dataPoint, maxSize, '\n');

		switch(lineType)
		{
			default:
			case MTLLineInvalid:
				// Do nothing
				break;

			case MTLLineName:
			{
				material = &mtl->MaterialList[materialIndex++];

				const char CPrivateText[] = "<CPrivate>";
				const size_t CPrivateTextSize = sizeof(CPrivateText);

				TextCopyA(CPrivateText, CPrivateTextSize, material->TextureFilePath, MTLFilePath);

				TextParseA(dataPoint, lineSize, "s", material->Name);

				break;
			}
			case MTLLineTexture:
			{
				TextParseA(dataPoint, lineSize, "s", material->TextureFilePath);
				break;
			}
			case MTLLineWeight:
			{
				TextParseA(dataPoint, lineSize, "f", &material->Weight);
				break;
			}
			case MTLLineColorAmbient:
			case MTLLineColorDiffuse:
			case MTLLineColorSpecular:
			case MTLLineColorEmission:
			{
				float* colorVector = 0;

				switch(lineType)
				{
					case MTLLineColorAmbient:
						colorVector = material->Ambient;
						break;
					case MTLLineColorDiffuse:
						colorVector = material->Diffuse;
						break;
					case MTLLineColorSpecular:
						colorVector = material->Specular;
						break;
					case MTLLineColorEmission:
						colorVector = material->Emission;
						break;
				}

				TextParseA(dataPoint, lineSize, "fff", &colorVector[0], &colorVector[1], &colorVector[2]);

				break;
			}
			case MTLLineDissolved:
			{
				TextParseA(dataPoint, lineSize, "f", &material->Dissolved);
				break;
			}
			case MTLLineDensity:
			{
				TextParseA(dataPoint, lineSize, "f", &material->Density);
				break;
			}
			case MTLLineIllumination:
			{
				IlluminationMode mode = IlluminationNone;
				int number = -1;

				TextParseA(dataPoint, lineSize, "i", &number);

				switch(number)
				{
					case 0:
						mode = IlluminationColorAndAmbientDisable;
						break;

					case 1:
						mode = IlluminationColorAndAmbientEnable;
						break;

					case 2:
						mode = IlluminationHighlightEnable;
						break;

					case 3:
						mode = IlluminationReflectionOnRayTraceEnable;
						break;

					case 4:
						mode = IlluminationReflectionOnRayTraceTransparency;
						break;

					case 5:
						mode = IlluminationReflectionOnRayTraceFresnel;
						break;

					case 6:
						mode = IlluminationReflectionOnRayTraceTransparencyFresnel;
						break;

					case 7:
						mode = IlluminationReflectionOnRayTraceFullEnable;
						break;

					case 8:
						mode = IlluminationReflectionEnable;
						break;

					case 9:
						mode = IlluminationTransparencyEnable;
						break;

					case 10:
						mode = IlluminationShadowsEnable;
						break;
				}

				material->Illumination = mode;

				break;
			}
		}
	}
	while(DataStreamSkipLine(&dataStream));

	return ActionSuccessful;
}


/*


void BF::MTL::PrintContent()
{
	printf("===[Material]===\n");

	for (size_t i = 0; i < MaterialListSize; i++)
	{
		MTLMaterial& material = MaterialList[i];

		if (i > 0)
		{
			printf("+--------------------+\n");
		}

		printf
		(
			"| ID        : %u\n"
			"| Name      : %s\n"
			"| FilePath  : %s\n"
			"| Weight    : %f\n"
			"| Ambient   : <%f|%f|%f>\n"
			"| Diffuse   : <%f|%f|%f>\n"
			"| Specular  : <%f|%f|%f>\n"
			"| Emmission : <%f|%f|%f>\n"
			"| Dissolved : %f\n"
			"| Density   : %f\n",
			i,
			material.Name,
			material.TextureFilePath,
			material.Weight,
			material.Ambient[0],
			material.Ambient[1],
			material.Ambient[2],
			material.Diffuse[0],
			material.Diffuse[1],
			material.Diffuse[2],
			material.Specular[0],
			material.Specular[1],
			material.Specular[2],
			material.Emission[0],
			material.Emission[1],
			material.Emission[2],
			material.Dissolved,
			material.Density
		);
	}

	printf("================\n");
}
*/
#include "MTL.h"

#include <File/PXDataStream.h>
#include <Text/PXText.h>
#include <Container/ClusterValue.h>
#include <OS/Memory/PXMemory.h>
#include <Compiler/PXCompiler.h>

void MTLConstruct(MTL* const mtl)
{
	MemoryClear(mtl, sizeof(MTL));
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

MTLLineType MTLPeekLine(const char* const line, const PXSize lineSize)
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

PXActionResult MTLFileCompile(PXDataStream* const inputStream, PXDataStream* const outputStream)
{
	PXDataStream tokenSteam;
	PXDataStream headerStream;

	const PXSize startoffset = outputStream->DataCursor;

	// temo
	unsigned int materialAmount = 0;
	unsigned int materialSizeDelta = 0;

	const PXSize headerOffset = 1024;


	PXDataStreamFromExternal(&headerStream, (PXAdress)outputStream->Data + startoffset, headerOffset);

	// Lexer - Level I
	{
		PXCompilerSettings compilerSettings;

		PXCompilerSettingsConstruct(&compilerSettings);

		compilerSettings.TryAnalyseTypes = PXYes;
		compilerSettings.CommentSingleLineSize = 1u;
		compilerSettings.CommentSingleLine = "#";

		outputStream->DataCursor += headerOffset;

		PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

		PXDataStreamFromExternal(&tokenSteam, (PXAdress)outputStream->Data + startoffset + headerOffset, outputStream->DataCursor - (startoffset + headerOffset));

		outputStream->DataCursor = startoffset;
	}

	MemoryClear(headerStream.Data, headerOffset);
	PXDataStreamCursorAdvance(&headerStream, sizeof(unsigned int)); // Keep the number of materials free
	PXDataStreamCursorAdvance(outputStream, headerOffset); // Write 0'ed data to later jump back to to change.

	while (!PXDataStreamIsAtEnd(&tokenSteam))
	{
		PXCompilerSymbolEntry compilerSymbolEntry;

		PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

		const MTLLineType mtlLineType = MTLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

		PXDataStreamWriteI8U(outputStream, mtlLineType); // Write typeID

		switch (mtlLineType)
		{
			case MTLLineName:
			{
				++materialAmount;

				// write how big the last material was, it will be 0 the first time.
				// At the end, convert the delte size to actual size.
				PXDataStreamWriteI32U(&headerStream, materialSizeDelta);

				// [No break] Fall through.
			}
			case MTLLineTexture:
			{
				char text[256];

				//char* dataPosition = PXDataStreamCursorPosition(outputStream);
				//const PXSize dataSize = PXDataStreamRemainingSize(outputStream);
				PXSize dataSizeWritten = 0;
				const PXBool isText = PXCompilerParseStringUntilNewLine(&tokenSteam, text, 256, &dataSizeWritten);

				if (!isText)
				{
					break; // Error
				}

				materialSizeDelta += PXDataStreamWriteI8U(outputStream, dataSizeWritten);
				materialSizeDelta += PXDataStreamWriteA(outputStream, text, dataSizeWritten);

				break;
			}
			case MTLLineWeight:
			case MTLLineDissolved:
			case MTLLineDensity:
			{
				PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

				const PXBool isFloat = compilerSymbolEntry.ID == PXCompilerSymbolLexerFloat;

				if (!isFloat)
				{
					// Error
					break;
				}

				materialSizeDelta += PXDataStreamWriteF(outputStream, compilerSymbolEntry.DataF);

				break;
			}
			case MTLLineColorAmbient:
			case MTLLineColorDiffuse:
			case MTLLineColorSpecular:
			case MTLLineColorEmission:
			{
				PXSize valuesDetected = 0;
				const PXSize colorVectorSize = 3u;
				float colorVector[3] = { -1, -1, -1 };

				const PXBool listParsed = PXCompilerParseFloatList(&tokenSteam, colorVector, colorVectorSize, &valuesDetected);

				if (!listParsed)
				{
					// Error
					break;
				}

				materialSizeDelta += PXDataStreamWriteFV(outputStream, colorVector, colorVectorSize);

				break;
			}
			case MTLLineIllumination:
			{
				PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

				const PXBool isInt = compilerSymbolEntry.ID == PXCompilerSymbolLexerInteger;

				if (!isInt)
				{
					// Error
					break;
				}

				const IlluminationMode illuminationMode = MTLIlluminationModeFromID(compilerSymbolEntry.DataI);

				materialSizeDelta += PXDataStreamWriteI8U(outputStream, illuminationMode);

				break;
			}
			default:
				break;
		}
	}

	// Write header
	{
		headerStream.DataCursor = 0;

		PXDataStreamWriteI32U(&headerStream, materialAmount);


		unsigned int sizeA = 0;
		unsigned int sizeB = 0;

		for (PXSize i = 0; i < materialAmount-1; ++i)
		{
			PXDataStreamReadI32U(&headerStream, &sizeA);
			PXDataStreamReadI32U(&headerStream, &sizeB);

			headerStream.DataCursor -= sizeof(unsigned int) * 2u;

			PXDataStreamWriteI32U(&headerStream, sizeB - sizeA);
		}

		const PXSize positionA = headerStream.DataCursor;

		PXDataStreamReadI32U(&headerStream, &sizeA);

        PXDataStreamWriteAtI32U(&headerStream, materialSizeDelta- sizeA, positionA);
	}

	//outputStream->DataCursor = startoffset + tokenSteam.DataCursor; // Only save the actuan size

	return PXActionSuccessful;
}

PXSize MTLFetchAmount(const void* const data, const PXSize dataSize)
{
	PXDataStream mtlStream;

	if (!data || !dataSize)
	{
		return 0;
	}

	PXDataStreamFromExternal(&mtlStream, data, dataSize);

	unsigned int materialListSize = 0;

	PXDataStreamReadI32U(&mtlStream, &materialListSize);

	return materialListSize;
}

PXBool MTLFetchMaterial(const void* const data, const PXSize dataSize, const PXSize materialID, MTLMaterial* const mtlMaterial)
{
	const PXSize amount = MTLFetchAmount(data, dataSize);

	MemoryClear(mtlMaterial, sizeof(MTLMaterial));

	if (materialID > amount) // Material ID wrong/Too high
	{
		return PXNo;
	}

	PXDataStream mtlPXDataStream;
	PXDataStream mtlHeaderStream;

	PXDataStreamFromExternal(&mtlPXDataStream, (PXAdress)data + 1024, dataSize - sizeof(unsigned int)); // Skip first int, we already got it
	PXDataStreamFromExternal(&mtlHeaderStream, (PXAdress)data + sizeof(unsigned int), dataSize- sizeof(unsigned int)); // Skip first int, we already got it

	for (PXSize i = 0; i <= materialID; ++i)
	{
		const PXBool isTarget = materialID == i;
		unsigned int materialDataSize = 0;

		PXDataStreamReadI32U(&mtlHeaderStream, &materialDataSize);

		if (isTarget)
		{
			mtlPXDataStream.DataSize = materialDataSize;	// Set max size for now

			while (!PXDataStreamIsAtEnd(&mtlPXDataStream))
			{
				MTLLineType mtlLineType;

				{
					unsigned char lineTypeID = 0;

					PXDataStreamReadI8U(&mtlPXDataStream, &lineTypeID);

					mtlLineType = lineTypeID;
				}

				switch (mtlLineType)
				{
					case MTLLineName:
					{
						PXDataStreamReadI8U(&mtlPXDataStream, &mtlMaterial->NameSize);
						mtlMaterial->Name = (char*)PXDataStreamCursorPosition(&mtlPXDataStream);

						PXDataStreamCursorAdvance(&mtlPXDataStream, mtlMaterial->NameSize);
						break;
					}
					case MTLLineTexture:
					{
						PXDataStreamReadI8U(&mtlPXDataStream, &mtlMaterial->DiffuseTexturePathSize);
						mtlMaterial->DiffuseTexturePath = (char*)PXDataStreamCursorPosition(&mtlPXDataStream);

						PXDataStreamCursorAdvance(&mtlPXDataStream, mtlMaterial->DiffuseTexturePathSize);
						break;
					}
					case MTLLineColorAmbient:
					{
						PXDataStreamReadFV(&mtlPXDataStream, mtlMaterial->Ambient, 3u);
						break;
					}
					case MTLLineColorDiffuse:
					{
						PXDataStreamReadFV(&mtlPXDataStream, mtlMaterial->Diffuse, 3u);
						break;
					}
					case MTLLineColorSpecular:
					{
						PXDataStreamReadFV(&mtlPXDataStream, mtlMaterial->Specular, 3u);
						break;
					}
					case MTLLineColorEmission:
					{
						PXDataStreamReadFV(&mtlPXDataStream, mtlMaterial->Emission, 3u);
						break;
					}
					case MTLLineWeight:
						PXDataStreamReadF(&mtlPXDataStream, &mtlMaterial->Weight);
						break;

					case MTLLineDissolved:
						PXDataStreamReadF(&mtlPXDataStream, &mtlMaterial->Dissolved);
						break;

					case MTLLineDensity:
						PXDataStreamReadF(&mtlPXDataStream, &mtlMaterial->Density);
						break;

					case MTLLineIllumination:
					{
						unsigned char illuminationID = 0;

						PXDataStreamReadI8U(&mtlPXDataStream, &illuminationID);

						mtlMaterial->Illumination = illuminationID;
						break;
					}

					default:
					{
#if 0 // Skip or fail if anything is wrong
						PXDataStreamCursorAdvance(&mtlPXDataStream, sizeof(unsigned char)); // Handle
						break;
#else
						MemoryClear(mtlMaterial, sizeof(MTLMaterial));
						return PXNo;
#endif
					}
				}

			}
		}
		else 		// else, do nothing -> skip
		{
			mtlPXDataStream.Data = ((PXAdress)mtlPXDataStream.Data) + materialDataSize + 10u; // accumulate Size, missing 10 Bytes??
		}
	}

	return PXYes;
}

PXActionResult MTLParseToMaterial(PXDataStream* const inputStream, PXMaterialList* const pxMaterialList)
{
	/*
	PXDataStream

	MemorySet(pxMaterialList, sizeof(PXMaterialList), 0);

	unsigned int amountOfMaterials = 0;

	PXDataStreamRead(inputStream, &amountOfMaterials, EndianLittle);

	PXDataStreamWriteIU();


	while (PXDataStreamIsAtEnd(&inputStream))
	{
		MTLLineType mtlLineType;

		{
			unsigned char lineTypeID = 0;

			PXDataStreamReadI8U(inputStream, &lineTypeID); // Line Type

			mtlLineType = lineTypeID;
		}


	}*/
}



/*
PXActionResult MTLParse(MTL* mtl, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXDataStream dataStream;

	MTLConstruct(mtl);
	PXDataStreamConstruct(&dataStream);
	PXDataStreamFromExternal(&dataStream, data, dataSize);
	*dataRead = 0;

	// Count How many materials are needed
	{
		PXSize materialCounter = 0;

		do
		{
			const unsigned char* currentLine = PXDataStreamCursorPosition(&dataStream);
			const unsigned char isNewMaterialUsed = *currentLine == 'n';

			if(isNewMaterialUsed)
			{
				++materialCounter;
			}
		}
		while(PXDataStreamSkipLine(&dataStream));

		mtl->MaterialListSize = materialCounter;
		mtl->MaterialList = MemoryAllocate(sizeof(MTLMaterial) * materialCounter);

		MemorySet(mtl->MaterialList, sizeof(MTLMaterial) * materialCounter, 0);

		PXDataStreamCursorToBeginning(&dataStream);
	}

	// Raw Parse
	MTLMaterial* material = 0; // current material, has to be here, its state dependend
	PXSize materialIndex = 0;

	do
	{
		const char* currentLine = PXDataStreamCursorPosition(&dataStream);
		const MTLLineType lineType = MTLPeekLine(currentLine);

		PXDataStreamSkipBlock(&dataStream); // Skip first element

		const char* dataPoint = PXDataStreamCursorPosition(&dataStream);
		const PXSize maxSize = PXDataStreamRemainingSize(&dataStream);
		const PXSize lineSize = TextLengthUntilA(dataPoint, maxSize, '\n');

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
				const PXSize CPrivateTextSize = sizeof(CPrivateText);

				PXTextCopyA(CPrivateText, CPrivateTextSize, material->TextureFilePath, MTLFilePath);

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
	while(PXDataStreamSkipLine(&dataStream));

	return PXActionSuccessful;
}


/*


void BF::MTL::PrintContent()
{
	printf("===[Material]===\n");

	for (PXSize i = 0; i < MaterialListSize; i++)
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

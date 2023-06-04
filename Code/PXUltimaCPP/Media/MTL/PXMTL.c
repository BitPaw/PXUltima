#include "PXMTL.h"

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <Compiler/PXCompiler.h>

void PXMTLConstruct(PXMTL* const mtl)
{
	PXMemoryClear(mtl, sizeof(PXMTL));
}

void PXMTLDestruct(PXMTL* const mtl)
{
	// Loop through MaterialListSize

	PXMemoryRelease(mtl->MaterialList, mtl->MaterialListSize);

	mtl->MaterialListSize = 0;
	mtl->MaterialList = 0;
}

PXMaterialIlluminationMode PXMTLIlluminationModeFromID(const unsigned int illuminationModeID)
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

PXMTLLineType PXMTLPeekLine(const char* const line, const PXSize lineSize)
{
	const unsigned short tagID = PXInt16Make(line[0], line[1]);

	switch(tagID)
	{
		case PXInt16Make('m', 'a'): return MTLLineTexture; // map_Kd
		case PXInt16Make('n', 'e'): return MTLLineName; // newmtl
		case PXInt16Make('N', 's'): return MTLLineWeight;
		case PXInt16Make('N', 'i'): return MTLLineDensity;
		case PXInt16Make('K', 'a'): return MTLLineColorAmbient;
		case PXInt16Make('K', 'd'): return MTLLineColorDiffuse;
		case PXInt16Make('K', 's'): return MTLLineColorSpecular;
		case PXInt16Make('K', 'e'): return MTLLineColorEmission;
		case PXInt16Make('d', ' '): return MTLLineDissolved;
		case PXInt16Make('i', 'l'): return MTLLineIllumination; // illum
		default:
			return MTLLineInvalid;
	}
}

PXActionResult PXMTLFileCompile(PXFile* const inputStream, PXFile* const outputStream)
{
	PXFile tokenSteam;
	PXFile headerStream;

	const PXSize startoffset = outputStream->DataCursor;

	// temo
	unsigned int materialAmount = 0;
	unsigned int materialSizeDelta = 0;

	const PXSize headerOffset = 1024;


	PXFileBufferExternal(&headerStream, (PXAdress)outputStream->Data + startoffset, headerOffset);

	// Lexer - Level I
	{
		PXCompilerSettings compilerSettings;

		PXCompilerSettingsConstruct(&compilerSettings);

		compilerSettings.TryAnalyseTypes = PXYes;
		compilerSettings.CommentSingleLineSize = 1u;
		compilerSettings.CommentSingleLine = "#";

		outputStream->DataCursor += headerOffset;

		PXCompilerLexicalAnalysis(inputStream, outputStream, &compilerSettings); // Raw-File-Input -> Lexer tokens

		PXFileBufferExternal(&tokenSteam, (PXAdress)outputStream->Data + startoffset + headerOffset, outputStream->DataCursor - (startoffset + headerOffset));

		outputStream->DataCursor = startoffset;
	}

	PXMemoryClear(headerStream.Data, headerOffset);
	PXFileCursorAdvance(&headerStream, sizeof(unsigned int)); // Keep the number of materials free
	PXFileCursorAdvance(outputStream, headerOffset); // Write 0'ed data to later jump back to to change.

	while (!PXFileIsAtEnd(&tokenSteam))
	{
		PXCompilerSymbolEntry compilerSymbolEntry;

		PXCompilerSymbolEntryExtract(&tokenSteam, &compilerSymbolEntry);

		const PXMTLLineType mtlLineType = PXMTLPeekLine(compilerSymbolEntry.Source, compilerSymbolEntry.Size);

		PXFileWriteI8U(outputStream, mtlLineType); // Write typeID

		switch (mtlLineType)
		{
			case MTLLineName:
			{
				++materialAmount;

				// write how big the last material was, it will be 0 the first time.
				// At the end, convert the delte size to actual size.
				PXFileWriteI32U(&headerStream, materialSizeDelta);

				// [No break] Fall through.
			}
			case MTLLineTexture:
			{
				char text[256];

				//char* dataPosition = PXFileCursorPosition(outputStream);
				//const PXSize dataSize = PXFileRemainingSize(outputStream);
				PXSize dataSizeWritten = 0;
				const PXBool isText = PXCompilerParseStringUntilNewLine(&tokenSteam, text, 256, &dataSizeWritten);

				if (!isText)
				{
					break; // Error
				}

				materialSizeDelta += PXFileWriteI8U(outputStream, dataSizeWritten);
				materialSizeDelta += PXFileWriteA(outputStream, text, dataSizeWritten);

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

				materialSizeDelta += PXFileWriteF(outputStream, compilerSymbolEntry.DataF);

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

				materialSizeDelta += PXFileWriteFV(outputStream, colorVector, colorVectorSize);

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

				const PXMaterialIlluminationMode illuminationMode = PXMTLIlluminationModeFromID(compilerSymbolEntry.DataI);

				materialSizeDelta += PXFileWriteI8U(outputStream, illuminationMode);

				break;
			}
			default:
				break;
		}
	}

	// Write header
	{
		headerStream.DataCursor = 0;

		PXFileWriteI32U(&headerStream, materialAmount);


		unsigned int sizeA = 0;
		unsigned int sizeB = 0;

		for (PXSize i = 0; i < materialAmount-1; ++i)
		{
			PXFileReadI32U(&headerStream, &sizeA);
			PXFileReadI32U(&headerStream, &sizeB);

			headerStream.DataCursor -= sizeof(unsigned int) * 2u;

			PXFileWriteI32U(&headerStream, sizeB - sizeA);
		}

		const PXSize positionA = headerStream.DataCursor;

		PXFileReadI32U(&headerStream, &sizeA);

        PXFileWriteAtI32U(&headerStream, materialSizeDelta- sizeA, positionA);
	}

	//outputStream->DataCursor = startoffset + tokenSteam.DataCursor; // Only save the actuan size

	return PXActionSuccessful;
}

PXSize PXMTLFetchAmount(const void* const data, const PXSize dataSize)
{
	PXFile mtlStream;

	if (!data || !dataSize)
	{
		return 0;
	}

	PXFileBufferExternal(&mtlStream, data, dataSize);

	unsigned int materialListSize = 0;

	PXFileReadI32U(&mtlStream, &materialListSize);

	return materialListSize;
}

PXBool PXMTLFetchMaterial(const void* const data, const PXSize dataSize, const PXSize materialID, PXMTLMaterial* const mtlMaterial)
{
	const PXSize amount = PXMTLFetchAmount(data, dataSize);

	PXMemoryClear(mtlMaterial, sizeof(PXMTLMaterial));

	if (materialID > amount) // Material ID wrong/Too high
	{
		return PXNo;
	}

	PXFile mtlPXFile;
	PXFile mtlHeaderStream;

	PXFileBufferExternal(&mtlPXFile, (PXAdress)data + 1024, dataSize - sizeof(unsigned int)); // Skip first int, we already got it
	PXFileBufferExternal(&mtlHeaderStream, (PXAdress)data + sizeof(unsigned int), dataSize- sizeof(unsigned int)); // Skip first int, we already got it

	for (PXSize i = 0; i <= materialID; ++i)
	{
		const PXBool isTarget = materialID == i;
		unsigned int materialDataSize = 0;

		PXFileReadI32U(&mtlHeaderStream, &materialDataSize);

		if (isTarget)
		{
			mtlPXFile.DataSize = materialDataSize;	// Set max size for now

			while (!PXFileIsAtEnd(&mtlPXFile))
			{
				PXMTLLineType mtlLineType;

				{
					unsigned char lineTypeID = 0;

					PXFileReadI8U(&mtlPXFile, &lineTypeID);

					mtlLineType = lineTypeID;
				}

				switch (mtlLineType)
				{
					case MTLLineName:
					{
						PXFileReadI8U(&mtlPXFile, &mtlMaterial->NameSize);
						mtlMaterial->Name = (char*)PXFileCursorPosition(&mtlPXFile);

						PXFileCursorAdvance(&mtlPXFile, mtlMaterial->NameSize);
						break;
					}
					case MTLLineTexture:
					{
						PXFileReadI8U(&mtlPXFile, &mtlMaterial->DiffuseTexturePathSize);
						mtlMaterial->DiffuseTexturePath = (char*)PXFileCursorPosition(&mtlPXFile);

						PXFileCursorAdvance(&mtlPXFile, mtlMaterial->DiffuseTexturePathSize);
						break;
					}
					case MTLLineColorAmbient:
					{
						PXFileReadFV(&mtlPXFile, mtlMaterial->Ambient, 3u);
						break;
					}
					case MTLLineColorDiffuse:
					{
						PXFileReadFV(&mtlPXFile, mtlMaterial->Diffuse, 3u);
						break;
					}
					case MTLLineColorSpecular:
					{
						PXFileReadFV(&mtlPXFile, mtlMaterial->Specular, 3u);
						break;
					}
					case MTLLineColorEmission:
					{
						PXFileReadFV(&mtlPXFile, mtlMaterial->Emission, 3u);
						break;
					}
					case MTLLineWeight:
						PXFileReadF(&mtlPXFile, &mtlMaterial->Weight);
						break;

					case MTLLineDissolved:
						PXFileReadF(&mtlPXFile, &mtlMaterial->Dissolved);
						break;

					case MTLLineDensity:
						PXFileReadF(&mtlPXFile, &mtlMaterial->Density);
						break;

					case MTLLineIllumination:
					{
						unsigned char illuminationID = 0;

						PXFileReadI8U(&mtlPXFile, &illuminationID);

						mtlMaterial->Illumination = illuminationID;
						break;
					}

					default:
					{
#if 0 // Skip or fail if anything is wrong
						PXFileCursorAdvance(&mtlPXFile, sizeof(unsigned char)); // Handle
						break;
#else
						PXMemoryClear(mtlMaterial, sizeof(PXMTLMaterial));
						return PXNo;
#endif
					}
				}

			}
		}
		else 		// else, do nothing -> skip
		{
			mtlPXFile.Data = ((PXAdress)mtlPXFile.Data) + materialDataSize + 10u; // accumulate Size, missing 10 Bytes??
		}
	}

	return PXYes;
}

PXActionResult PXMTLParseToMaterial(PXFile* const inputStream, PXMaterialList* const pxMaterialList)
{
	/*
	PXFile

	MemorySet(pxMaterialList, sizeof(PXMaterialList), 0);

	unsigned int amountOfMaterials = 0;

	PXFileRead(inputStream, &amountOfMaterials, PXEndianLittle);

	PXFileWriteIU();


	while (PXFileIsAtEnd(&inputStream))
	{
		MTLLineType mtlLineType;

		{
			unsigned char lineTypeID = 0;

			PXFileReadI8U(inputStream, &lineTypeID); // Line Type

			mtlLineType = lineTypeID;
		}


	}*/
}



/*
PXActionResult MTLParse(MTL* mtl, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXFile dataStream;

	MTLConstruct(mtl);
	PXFileConstruct(&dataStream);
	PXFileBufferExternal(&dataStream, data, dataSize);
	*dataRead = 0;

	// Count How many materials are needed
	{
		PXSize materialCounter = 0;

		do
		{
			const unsigned char* currentLine = PXFileCursorPosition(&dataStream);
			const unsigned char isNewMaterialUsed = *currentLine == 'n';

			if(isNewMaterialUsed)
			{
				++materialCounter;
			}
		}
		while(PXFileSkipLine(&dataStream));

		mtl->MaterialListSize = materialCounter;
		mtl->MaterialList = MemoryAllocate(sizeof(MTLMaterial) * materialCounter);

		MemorySet(mtl->MaterialList, sizeof(MTLMaterial) * materialCounter, 0);

		PXFileCursorToBeginning(&dataStream);
	}

	// Raw Parse
	MTLMaterial* material = 0; // current material, has to be here, its state dependend
	PXSize materialIndex = 0;

	do
	{
		const char* currentLine = PXFileCursorPosition(&dataStream);
		const MTLLineType lineType = MTLPeekLine(currentLine);

		PXFileSkipBlock(&dataStream); // Skip first element

		const char* dataPoint = PXFileCursorPosition(&dataStream);
		const PXSize maxSize = PXFileRemainingSize(&dataStream);
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
	while(PXFileSkipLine(&dataStream));

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
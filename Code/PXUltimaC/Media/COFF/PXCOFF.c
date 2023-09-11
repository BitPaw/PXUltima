#include "PXCOFF.h"

#include <Math/PXMath.h>

#include <assert.h>

#define PXCOFFDebug 1

#define STYP_REG 0x0000;
#define STYP_DSECT 0x0001
#define STYP_NOLOAD 0x0002
#define STYP_GROUP 0x0004
#define STYP_PAD 0x0008
#define STYP_COPY 0x010
#define STYP_TEXT 0x0020
#define STYP_DATA 0x0040
#define STYP_BSS 0x0080
#define STYP_BLOCK 0x1000
#define STYP_PASS 0x2000
#define STYP_CLINK 0x4000
#define STYP_VECTOR 0x8000
#define STYP_PADDED 0x00010000

PXCOFFMachineType PXCOFFMachineFromID(const PXInt16U valueID)
{
	switch (valueID)
	{
		case 0x000: return PXCOFFMachineTypeUnkown;
		case 0x184: return PXCOFFMachineTypeAlpha;
		case 0x1c0: return PXCOFFMachineTypeARM;
		case 0x284: return PXCOFFMachineTypeALPHA64;
		case 0x14c: return PXCOFFMachineTypeI386;
		case 0x200: return PXCOFFMachineTypeIA64;
		case 0x268: return PXCOFFMachineTypeM68K;
		case 0x266: return PXCOFFMachineTypeMIPS16;
		case 0x366: return PXCOFFMachineTypeMIPSFPU;
		case 0x466: return PXCOFFMachineTypeMIPSFPU16;
		case 0x1f0: return PXCOFFMachineTypePOWERPC;
		case 0x162: return PXCOFFMachineTypeR3000;
		case 0x166: return PXCOFFMachineTypeR4000;
		case 0x168: return PXCOFFMachineTypeR10000;
		case 0x1a2: return PXCOFFMachineTypeSH3;
		case 0x1a6: return PXCOFFMachineTypeSH4;
		case 0x1c2: return PXCOFFMachineTypeTHUMB;

		default:
			return PXCOFFMachineTypeInvalid;
	}
}

PXCOFFFormat PXCOFFFormatFromID(const PXInt16U valueID)
{
	switch (valueID)
	{
		case 0x010b: return PXCOFFFormatPE32;
		case 0x020b: return PXCOFFFormatPE32Plus;

		default:
			return PXCOFFFormatInvalid;
	}
}

PXSectionType PXSectionTypeFromID(const PXInt64U valueID)
{
	switch (valueID)
	{
		case PXInt40Make('.', 'a', 'r', 'c', 'h'): return PXSectionTypeAlphaArchitectureInformation;
		case PXInt32Make('.', 'b', 's', 's'): return PXSectionTypeUninitializedData;
		case PXInt40Make('.', 'd', 'a', 't', 'a'): return PXSectionTypeInitializedData;
		case PXInt48Make('.', 'e', 'd', 'a', 't', 'a'): return PXSectionTypeExportTables;
		case PXInt48Make('.', 'i', 'd', 'a', 't', 'a'): return PXSectionTypeImportTables;
		case PXInt48Make('.', 'p', 'd', 'a', 't', 'a'): return PXSectionTypeExceptionInformationA;
		case PXInt48Make('.', 'r', 'd', 'a', 't', 'a'): return PXSectionTypeReadOnlyInitializedData;
		case PXInt48Make('.', 'r', 'e', 'l', 'o', 'c'): return PXSectionTypeImageRelocations;
		case PXInt40Make('.', 'r', 's', 'r', 'c'): return PXSectionTypeResourceDirectory;
		case PXInt40Make('.', 't', 'e', 'x', 't'): return PXSectionTypeExecutableCode;
		case PXInt32Make('.', 't', 'l', 's'): return PXSectionTypeThreadLocalstorage;
		case PXInt48Make('.', 'x', 'd', 'a', 't', 'a'): return PXSectionTypeExceptionInformationB;
		case PXInt48Make('.', 'd', 'e', 'b', 'u', 'g'): return PXSectionTypeDebug;
		case PXInt64Make('.', 'd', 'r', 'e', 'c', 't', 'v', 'e'): return PXSectionTypeDirective;

		default:
			return PXSectionTypeInvalid;
	}
}

PXActionResult PXCOFFLoadFromFile(PXCOFF* const pxCOFF, PXFile* const pxFile)
{
	PXClear(PXCOFF, pxCOFF);



	// COFF File Header
	{
		PXInt16U machineTypeID = 0;

		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{&machineTypeID, PXDataTypeInt16ULE},
			{&pxCOFF->NumberOfSections,PXDataTypeInt16ULE},
			{&pxCOFF->TimeDateStamp,PXDataTypeInt32ULE},
			{&pxCOFF->PointerToSymbolTable,PXDataTypeInt32ULE},
			{&pxCOFF->NumberOfSymbols,PXDataTypeInt32ULE},
			{&pxCOFF->SizeOfOptionalHeader, PXDataTypeInt16ULE},
			{&pxCOFF->CharacteristicsFlagList, PXDataTypeInt16ULE}
		};

		PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

		pxCOFF->MachineType = PXCOFFMachineFromID(machineTypeID);
	}

	// Optional Header Standard Fields
	{
		const PXBool hasOptionalHeader = pxCOFF->SizeOfOptionalHeader > 0;
		PXSize remainingOptionalHeaderOffset = pxCOFF->SizeOfOptionalHeader;
		const PXSize syncPosition = pxFile->DataCursor + pxCOFF->SizeOfOptionalHeader;

		if (hasOptionalHeader)
		{
			{
				PXInt16U magicNumber = 0;

				remainingOptionalHeaderOffset -= PXFileReadI16UE(pxFile, &magicNumber, PXEndianLittle);

				pxCOFF->Format = PXCOFFFormatFromID(magicNumber);

				switch (pxCOFF->Format)
				{
					case PXCOFFFormatPE32:
						pxFile->BitFormatOfData = PXBitFormat32;
						break;

					case PXCOFFFormatPE32Plus:
						pxFile->BitFormatOfData = PXBitFormat64;
						break;
				}
			}

		
			{
				PXInt16U magicNumber = 0;

				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{&magicNumber, PXDataTypeInt16ULE},
					{&pxCOFF->MajorLinkerVersion, PXDataTypeInt08U},
					{&pxCOFF->MinorLinkerVersion, PXDataTypeInt08U},
					{&pxCOFF->SizeOfCode, PXDataTypeInt32ULE},
					{&pxCOFF->SizeOfInitializedData, PXDataTypeInt32ULE},
					{&pxCOFF->SizeOfUninitializedData, PXDataTypeInt32ULE},
					{&pxCOFF->AddressOfEntryPoint, PXDataTypeInt32ULE},
					{&pxCOFF->BaseOfCode, PXDataTypeInt32ULE},
					{&pxCOFF->BaseOfData, PXDataTypeInt32ULEOnlyIf32B}
				};

				const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

				remainingOptionalHeaderOffset -= batchSize;

				switch (pxCOFF->Format)
				{
					case PXCOFFFormatPE32:
						assert(batchSize == 28);
						break;

					case PXCOFFFormatPE32Plus:
						assert(batchSize == 24);
						break;
				}
			}

			// Optional Header Windows NT - Specific Fields
			{
				// B
				{
					const PXFileDataElementType pxDataStreamElementList[] =
					{
						{&pxCOFF->ImageBase, PXDataTypeAdressFlex},
						{&pxCOFF->SectionAlignment,PXDataTypeInt32ULE},
						{&pxCOFF->FileAlignment,PXDataTypeInt32ULE}, 
						{&pxCOFF->MajorOperatingSystemVersion,PXDataTypeInt16ULE},
						{&pxCOFF->MinorOperatingSystemVersion,PXDataTypeInt16ULE},
						{&pxCOFF->MajorImageVersion,PXDataTypeInt16ULE},
						{&pxCOFF->MinorImageVersion,PXDataTypeInt16ULE},
						{&pxCOFF->MajorSubsystemVersion,PXDataTypeInt16ULE},
						{&pxCOFF->MinorSubsystemVersion,PXDataTypeInt16ULE},
						{0, PXDataTypePadding(4)},
						{&pxCOFF->SizeOfImage,PXDataTypeInt32ULE},
						{&pxCOFF->SizeOfHeaders,PXDataTypeInt32ULE},
						{&pxCOFF->CheckSum,PXDataTypeInt32ULE},
						{&pxCOFF->Subsystem,PXDataTypeInt16ULE},
						{&pxCOFF->DLLCharacteristics,PXDataTypeInt16ULE},
						{&pxCOFF->SizeOfStackReserve,PXDataTypeAdressFlex},
						{&pxCOFF->SizeOfStackCommit,PXDataTypeAdressFlex},
						{&pxCOFF->SizeOfHeapReserve,PXDataTypeAdressFlex},
						{&pxCOFF->SizeOfHeapCommit,PXDataTypeAdressFlex},
						{&pxCOFF->LoaderFlags,PXDataTypeInt32ULE},
						{&pxCOFF->NumberOfRvaAndSizes,PXDataTypeInt32ULE}
					};

					const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
				
					remainingOptionalHeaderOffset -= batchSize;

					switch (pxCOFF->Format)
					{
						case PXCOFFFormatPE32:
							assert(batchSize == 68u);
							break;

						case PXCOFFFormatPE32Plus:
							assert(batchSize == 88u);
							break;
					}
				}

				// D
				{
					PXInt32U virtualAddress = 0;
					PXInt32U size = 0;

					/*
					for (PXInt32U i = 0; i < pxCOFF->NumberOfRvaAndSizes; ++i)
					{
						PXFileReadI32U(pxFile, &virtualAddress);
						PXFileReadI32U(pxFile, &size);
					}*/
				}
			}

			// Optional Header Data Directories
			{
				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{ &pxCOFF->ExportTableAdress, PXDataTypeAdress32},
					{ &pxCOFF->ExportTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->ImportTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->ImportTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->ResourceTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->ResourceTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->ExceptionTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->ExceptionTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->CertificateTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->CertificateTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->BaseRelocationTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->BaseRelocationTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->DebugAdress, PXDataTypeAdress32 },
					{ &pxCOFF->DebugSize,PXDataTypeInt32ULE },

					{ &pxCOFF->ArchitectureAdress, PXDataTypeAdress32 },
					{ &pxCOFF->ArchitectureSize,PXDataTypeInt32ULE },

					{ &pxCOFF->GlobalPtrAdress, PXDataTypeAdress32 },
					{ &pxCOFF->GlobalPtrSize,PXDataTypeInt32ULE },

					{ &pxCOFF->ThreadLocalStorageTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->ThreadLocalStorageTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->LoadConfigTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->LoadConfigTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->BoundImportAdress, PXDataTypeAdress32 },
					{ &pxCOFF->BoundImportSize,PXDataTypeInt32ULE },

					{ &pxCOFF->ImportAddressTableAdress, PXDataTypeAdress32 },
					{ &pxCOFF->ImportAddressTableSize,PXDataTypeInt32ULE },

					{ &pxCOFF->DelayImportDescriptorAdress, PXDataTypeAdress32 },
					{ &pxCOFF->DelayImportDescriptorSize,PXDataTypeInt32ULE },

					{ &pxCOFF->COMPlusRuntimeHeaderAdress, PXDataTypeAdress32 },
					{ &pxCOFF->COMPlusRuntimeHeaderSize,PXDataTypeInt32ULE },

					{ 0, PXDataTypePadding(8)}
				};

				const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

				remainingOptionalHeaderOffset -= batchSize;

				assert(batchSize == 128u);
			}

			pxFile->DataCursor = syncPosition;
		}
	}

	// Parse SectionTable
	

	pxCOFF->SectionTableList = PXNewList(PXSectionTable, pxCOFF->NumberOfSections);

	for (PXInt16U sectionID = 0; sectionID < pxCOFF->NumberOfSections; ++sectionID)
	{
		PXSectionTable* const pxSectionTableCurrent = &pxCOFF->SectionTableList[sectionID];

		{
			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{pxSectionTableCurrent->Name.Data, PXDataTypeDatax8},
				{&pxSectionTableCurrent->PhysicalAddress,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->VirtualAddress,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->SectionSizeInBytes,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->PointerToRawData,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->PointerToRelocations,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->PointerToLinenumbers,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->NumberOfRelocations,PXDataTypeInt16ULE},
				{&pxSectionTableCurrent->NumberOfLinenumbers,PXDataTypeInt16ULE},
				{&pxSectionTableCurrent->CharacteristicFlags, PXDataTypeInt32ULE},
			};

			const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

			assert(readBytes == 40u);


			pxSectionTableCurrent->Type = PXSectionTypeFromID(pxSectionTableCurrent->Name.Value);

			printf("[COFF][Section %2i/%2i - %-8s] %6i Bytes\n", sectionID+1, pxCOFF->NumberOfSections, pxSectionTableCurrent->Name.Data, pxSectionTableCurrent->SectionSizeInBytes);
		}

		const PXSize oldPosition = pxFile->DataCursor;

		if (pxSectionTableCurrent->SectionSizeInBytes > 0)
		{
			PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->PointerToRawData);

			char* adressD = PXFileCursorPosition(pxFile);

			for (size_t i = 0; i < pxSectionTableCurrent->SectionSizeInBytes; i++)
			{
				const ww = (i + 1) % 64 == 0;

				const char xxx = MakePrintable(adressD[i]);

				printf("%c", xxx);


				if (ww)
				{
					printf("\n");
				}

			}

	
		}

		// Reallocations
		if (pxSectionTableCurrent->NumberOfRelocations > 0)
		{
			PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->PointerToRelocations);

			for (PXInt16U i = 0; i < pxSectionTableCurrent->NumberOfRelocations; ++i)
			{
				PXRelocationInformation pxRelocationInformation;

				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{&pxRelocationInformation.VirtualAddressOfReference, PXDataTypeInt32ULE},
					{&pxRelocationInformation.SymbolTableIndex,PXDataTypeInt32ULE},
					{&pxRelocationInformation.RelocationType,PXDataTypeInt16ULE}
				};

				PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

#if PXCOFFDebug
				printf
				(
					"[COFF][RelocationInformation] %6i,%6i,%6i\n",
					pxRelocationInformation.VirtualAddressOfReference,
					pxRelocationInformation.SymbolTableIndex,
					pxRelocationInformation.RelocationType
				);
#endif
			}
		}

		// Lines
		if (pxSectionTableCurrent->NumberOfLinenumbers > 0)
		{
			PXLineNumberEntry pxLineNumberEntryXX;

			PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->PointerToLinenumbers);

			for (PXInt16U i = 0; i < pxSectionTableCurrent->NumberOfLinenumbers; ++i)
			{
				PXLineNumberEntry* pxLineNumberEntry = &pxLineNumberEntryXX;

				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{&pxLineNumberEntry->SymbolIndex, PXDataTypeInt32ULE},
					{&pxLineNumberEntry->LineNumber,PXDataTypeInt16ULE}
				};

				PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

#if PXCOFFDebug
				printf("[COFF][Section][LineEntry] %3i/%-3i - Line:%i Index:%i\n", i + 1, pxSectionTableCurrent->NumberOfLinenumbers, pxLineNumberEntry->LineNumber, pxLineNumberEntry->SymbolIndex);
#endif
			}
		}

		PXFileCursorMoveTo(pxFile, oldPosition); // Restore position to further parse the section table		
	}
	 
	// Parse actual symbols
	{
		if (pxCOFF->NumberOfSymbols > 0)
		{
			PXFileCursorMoveTo(pxFile, pxCOFF->PointerToSymbolTable);

			PXCOFFSymbolTableEntry pxCOFFSymbolTableEntryXX;

#if PXCOFFDebug
			printf("[COFF] Symbols deteced <%i>\n", pxCOFF->NumberOfSymbols);
#endif

			for (PXInt32U i = 0; i < pxCOFF->NumberOfSymbols; i++)
			{
				PXCOFFSymbolTableEntry* pxCOFFSymbolTableEntry = &pxCOFFSymbolTableEntryXX;

				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{pxCOFFSymbolTableEntry->Name, PXDataTypeDatax8},
					{&pxCOFFSymbolTableEntry->ValueOfSymbol,PXDataTypeInt32ULE},
					{&pxCOFFSymbolTableEntry->SectionNumber,PXDataTypeInt16ULE},
					{&pxCOFFSymbolTableEntry->TypeAndDerived,PXDataTypeInt16ULE},
					{&pxCOFFSymbolTableEntry->StorageClass,PXDataTypeInt08U},
					{&pxCOFFSymbolTableEntry->NumberOfAuxiliaryEntries,PXDataTypeInt08U}
				};

				const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

				assert(readBytes == 18u);


				if (pxCOFFSymbolTableEntry->DoesNotUseExternalString && pxCOFFSymbolTableEntry->DoesNotUseExternalString != 0xCCCCCCCC)
				{
					printf("[COFF][Symbol] %8.8s\n", pxCOFFSymbolTableEntry->Name);
				}
				else
				{
					PXSize oolPos = pxFile->DataCursor;

					PXFileCursorMoveTo(pxFile, pxCOFFSymbolTableEntry->NameReferenceOffset);

					char* xxx = (char*)PXFileCursorPosition(pxFile);

					printf("[COFF][Symbol] -------- -> %s\n", xxx);

					PXFileCursorMoveTo(pxFile, oolPos);
				}
			}
		}
#if PXCOFFDebug
		else
		{
			printf("[COFF] No Symbols\n");
		}
#endif
		
	}

	return PXActionSuccessful;
}

PXActionResult PXCOFFSaveToFile(const PXCOFF* const pxCOFF, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}
#include "PXCOFF.h"


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
			{PXDataTypeLEInt16U, &machineTypeID},
			{PXDataTypeLEInt16U, &pxCOFF->NumberOfSections},
			{PXDataTypeLEInt32U, &pxCOFF->TimeDateStamp},
			{PXDataTypeLEInt32U, &pxCOFF->PointerToSymbolTable},
			{PXDataTypeLEInt32U, &pxCOFF->NumberOfSymbols},
			{PXDataTypeLEInt16U, &pxCOFF->SizeOfOptionalHeader},
			{PXDataTypeLEInt16U, &pxCOFF->CharacteristicsFlagList}
		};
		const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

		PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

		pxCOFF->MachineType = PXCOFFMachineFromID(machineTypeID);
	}

	// Optional Header Standard Fields
	{
		const PXBool hasOptionalHeader = pxCOFF->SizeOfOptionalHeader > 0;

		if (hasOptionalHeader)
		{
			{
				PXInt16U magicNumber = 0;

				PXFileReadI16UE(pxFile, &magicNumber, PXEndianLittle);

				pxCOFF->Format = PXCOFFFormatFromID(magicNumber);
			}

		
			{
				PXInt16U magicNumber = 0;

				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{PXDataTypeLEInt16U, &magicNumber},
					{PXDataTypeInt8U, &pxCOFF->MajorLinkerVersion},
					{PXDataTypeInt8U, &pxCOFF->MinorLinkerVersion},
					{PXDataTypeLEInt32U, &pxCOFF->SizeOfCode},
					{PXDataTypeLEInt32U, &pxCOFF->SizeOfInitializedData},
					{PXDataTypeLEInt32U, &pxCOFF->SizeOfUninitializedData},
					{PXDataTypeLEInt32U, &pxCOFF->AddressOfEntryPoint},
					{PXDataTypeLEInt32U, &pxCOFF->BaseOfCode},
					{PXDataTypeLEInt32U, &pxCOFF->BaseOfData}
				};
				const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

				if (PXCOFFFormatPE32 == pxCOFF->Format)
				{
					PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);
				}
				else
				{
					PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize - 1u);
				}
			}

			// Optional Header Windows NT - Specific Fields
			{
				// A
				{
					switch (pxCOFF->Format)
					{
						case PXCOFFFormatPE32:
						{
							PXInt32U value = 0;

							PXFileReadI32U(pxFile, &value);

							pxCOFF->ImageBase = value;

							break;
						}
						case PXCOFFFormatPE32Plus:
						{
							PXFileReadI64U(pxFile, &pxCOFF->ImageBase);
							break;
						}
					}
				}		

				// B
				{
					const PXFileDataElementType pxDataStreamElementList[] =
					{
						{PXDataTypeLEInt32U, &pxCOFF->SectionAlignment},
						{PXDataTypeLEInt32U, &pxCOFF->FileAlignment},
						{PXDataTypeLEInt16U, &pxCOFF->MajorOperatingSystemVersion},
						{PXDataTypeLEInt16U, &pxCOFF->MinorOperatingSystemVersion},
						{PXDataTypeLEInt16U, &pxCOFF->MajorImageVersion},
						{PXDataTypeLEInt16U, &pxCOFF->MinorImageVersion},
						{PXDataTypeLEInt16U, &pxCOFF->MajorSubsystemVersion},
						{PXDataTypeLEInt16U, &pxCOFF->MinorSubsystemVersion},
						{PXDataTypeTextx4, pxCOFF->ReservedA},
						{PXDataTypeLEInt32U, &pxCOFF->SizeOfImage},
						{PXDataTypeLEInt32U, &pxCOFF->SizeOfHeaders},
						{PXDataTypeLEInt32U, &pxCOFF->CheckSum},
						{PXDataTypeLEInt16U, &pxCOFF->Subsystem},
						{PXDataTypeLEInt16U, &pxCOFF->DLLCharacteristics}
					};
					const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

					PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);
				}

				// C
				{
					switch (pxCOFF->Format)
					{
						case PXCOFFFormatPE32:
						{
							const PXFileDataElementType pxDataStreamElementList[] =
							{
								{PXDataTypeAdress32Bit, &pxCOFF->SizeOfStackReserve},
								{PXDataTypeAdress32Bit, &pxCOFF->SizeOfStackCommit},
								{PXDataTypeAdress32Bit, &pxCOFF->SizeOfHeapReserve},
								{PXDataTypeAdress32Bit, &pxCOFF->SizeOfHeapCommit}
							};
							const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

							PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

							break;
						}
						case PXCOFFFormatPE32Plus:
						{
							const PXFileDataElementType pxDataStreamElementList[] =
							{
								{PXDataTypeAdress64Bit, &pxCOFF->SizeOfStackReserve},
								{PXDataTypeAdress64Bit, &pxCOFF->SizeOfStackCommit},
								{PXDataTypeAdress64Bit, &pxCOFF->SizeOfHeapReserve},
								{PXDataTypeAdress64Bit, &pxCOFF->SizeOfHeapCommit}
							};
							const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

							PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

							break;
						}
					}
				}			

				// D
				{
					const PXFileDataElementType pxDataStreamElementList[] =
					{
						{PXDataTypeLEInt32U, &pxCOFF->LoaderFlags},
						{PXDataTypeLEInt32U, &pxCOFF->NumberOfRvaAndSizes}
					};
					const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

					PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);
					
					PXInt32U virtualAddress = 0;
					PXInt32U size = 0;

					for (PXInt32U i = 0; i < pxCOFF->NumberOfRvaAndSizes; ++i)
					{
						PXFileReadI32U(pxFile, &virtualAddress);
						PXFileReadI32U(pxFile, &size);
					}
				}


			}

			// Optional Header Data Directories
			{
				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{PXDataTypeAdress32Bit, &pxCOFF->ExportTableAdress},
					{PXDataTypeLEInt32U, &pxCOFF->ExportTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->ImportTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->ImportTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->ResourceTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->ResourceTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->ExceptionTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->ExceptionTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->CertificateTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->CertificateTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->BaseRelocationTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->BaseRelocationTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->DebugAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->DebugSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->ArchitectureAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->ArchitectureSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->GlobalPtrAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->GlobalPtrSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->ThreadLocalStorageTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->ThreadLocalStorageTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->LoadConfigTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->LoadConfigTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->BoundImportAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->BoundImportSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->ImportAddressTableAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->ImportAddressTableSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->DelayImportDescriptorAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->DelayImportDescriptorSize },

					{ PXDataTypeAdress32Bit, &pxCOFF->COMPlusRuntimeHeaderAdress },
					{ PXDataTypeLEInt32U, &pxCOFF->COMPlusRuntimeHeaderSize },

					{ PXDataTypeTextx8, pxCOFF->ReservedB }
				};
				const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

				PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

			}
		}



	}

	PXFileCursorRewind(pxFile, 2u);



	// Parse SectionTable

	pxCOFF->SectionTableList = PXNewList(PXSectionTable, pxCOFF->NumberOfSections);

	for (PXInt16U sectionID = 0; sectionID < pxCOFF->NumberOfSections; ++sectionID)
	{
		PXSectionTable* const pxSectionTableCurrent = &pxCOFF->SectionTableList[sectionID];

		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{PXDataTypeTextx8, pxSectionTableCurrent->Name.Data},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->VirtualSize},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->VirtualAddress},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->SizeOfRawData},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->PointerToRawData},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->PointerToRelocations},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->PointerToLinenumbers},
			{PXDataTypeLEInt16U, &pxSectionTableCurrent->NumberOfRelocations},
			{PXDataTypeLEInt16U, &pxSectionTableCurrent->NumberOfLinenumbers},
			{PXDataTypeLEInt32U, &pxSectionTableCurrent->Characteristics},
		};
		const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

		PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

		pxSectionTableCurrent->Type = PXSectionTypeFromID(pxSectionTableCurrent->Name.Value);

		printf("[Section %2i - %-8s] %6i Bytes\n", sectionID, pxSectionTableCurrent->Name.Data, pxSectionTableCurrent->VirtualSize);
	}

	return PXActionSuccessful;
}

PXActionResult PXCOFFSaveToFile(const PXCOFF* const pxCOFF, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}
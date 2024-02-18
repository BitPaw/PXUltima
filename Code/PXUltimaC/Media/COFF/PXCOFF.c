#include "PXCOFF.h"

#include <Math/PXMath.h>
#include <OS/Console/PXConsole.h>

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

PXCOFFMachineType PXAPI PXCOFFMachineFromID(const PXInt16U valueID)
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

PXCOFFFormat PXAPI PXCOFFFormatFromID(const PXInt16U valueID)
{
	switch (valueID)
	{
		case 0x010b: return PXCOFFFormatPE32;
		case 0x020b: return PXCOFFFormatPE32Plus;

		default:
			return PXCOFFFormatInvalid;
	}
}

PXSectionType PXAPI PXSectionTypeFromID(const PXInt64U valueID)
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

PXActionResult PXAPI PXCOFFLoadFromFile(PXCOFF* const pxCOFF, PXFile* const pxFile)
{
	PXClear(PXCOFF, pxCOFF);

#if PXCOFFDebug
	PXLogPrint
	(
		PXLoggingInfo,
		"COFF",
		"Parsing",
		"Start"
	);
#endif

	// COFF File Header
	{
		PXInt16U machineTypeID = 0;

		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{&machineTypeID, PXDataTypeInt16ULE},
			{&pxCOFF->Header.NumberOfSections,PXDataTypeInt16ULE},
			{&pxCOFF->Header.TimeDateStamp,PXDataTypeInt32ULE},
			{&pxCOFF->Header.PointerToSymbolTable,PXDataTypeInt32ULE},
			{&pxCOFF->Header.NumberOfSymbols,PXDataTypeInt32ULE},
			{&pxCOFF->Header.SizeOfOptionalHeader, PXDataTypeInt16ULE},
			{&pxCOFF->Header.CharacteristicsFlagList, PXDataTypeInt16ULE}
		};

		PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

		pxCOFF->Header.MachineType = PXCOFFMachineFromID(machineTypeID);
	}

	// Optional Header Standard Fields
	{
		const PXBool hasOptionalHeader = pxCOFF->Header.SizeOfOptionalHeader > 0;
		PXSize remainingOptionalHeaderOffset = pxCOFF->Header.SizeOfOptionalHeader;
		const PXSize syncPosition = pxFile->DataCursor + pxCOFF->Header.SizeOfOptionalHeader;

		if (hasOptionalHeader)
		{
			{
				PXInt16U magicNumber = 0;

				remainingOptionalHeaderOffset -= PXFileReadI16UE(pxFile, &magicNumber, PXEndianLittle);

				pxCOFF->OptionalHeader.Format = PXCOFFFormatFromID(magicNumber);

				switch (pxCOFF->OptionalHeader.Format)
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
					{&pxCOFF->OptionalHeader.StandardFields.MajorLinkerVersion, PXDataTypeInt08U},
					{&pxCOFF->OptionalHeader.StandardFields.MinorLinkerVersion, PXDataTypeInt08U},
					{&pxCOFF->OptionalHeader.StandardFields.SizeOfCode, PXDataTypeInt32ULE},
					{&pxCOFF->OptionalHeader.StandardFields.SizeOfInitializedData, PXDataTypeInt32ULE},
					{&pxCOFF->OptionalHeader.StandardFields.SizeOfUninitializedData, PXDataTypeInt32ULE},
					{&pxCOFF->OptionalHeader.StandardFields.AddressOfEntryPoint, PXDataTypeInt32ULE},
					{&pxCOFF->OptionalHeader.StandardFields.BaseOfCode, PXDataTypeInt32ULE},
					{&pxCOFF->OptionalHeader.StandardFields.BaseOfData, PXDataTypeInt32ULEOnlyIf32B}
				};

				const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

				remainingOptionalHeaderOffset -= batchSize;

				switch (pxCOFF->OptionalHeader.Format)
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
						{&pxCOFF->OptionalHeader.WindowsNT.ImageBase, PXDataTypeAdressFlex},
						{&pxCOFF->OptionalHeader.WindowsNT.SectionAlignment,PXDataTypeInt32ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.FileAlignment,PXDataTypeInt32ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.MajorOperatingSystemVersion,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.MinorOperatingSystemVersion,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.MajorImageVersion,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.MinorImageVersion,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.MajorSubsystemVersion,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.MinorSubsystemVersion,PXDataTypeInt16ULE},
						{0, PXDataTypePadding(4)},
						{&pxCOFF->OptionalHeader.WindowsNT.SizeOfImage,PXDataTypeInt32ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeaders,PXDataTypeInt32ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.CheckSum,PXDataTypeInt32ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.Subsystem,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.DLLCharacteristics,PXDataTypeInt16ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.SizeOfStackReserve,PXDataTypeAdressFlex},
						{&pxCOFF->OptionalHeader.WindowsNT.SizeOfStackCommit,PXDataTypeAdressFlex},
						{&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeapReserve,PXDataTypeAdressFlex},
						{&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeapCommit,PXDataTypeAdressFlex},
						{&pxCOFF->OptionalHeader.WindowsNT.LoaderFlags,PXDataTypeInt32ULE},
						{&pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes,PXDataTypeInt32ULE}
					};

					const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
				
					remainingOptionalHeaderOffset -= batchSize;

					switch (pxCOFF->OptionalHeader.Format)
					{
						case PXCOFFFormatPE32:
							assert(batchSize == 68u);
							break;

						case PXCOFFFormatPE32Plus:
							assert(batchSize == 88u);
							break;
					}

					// D
					{
			

						if (pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes > 0 && 0)
						{
							PXInt32U virtualAddress = 0;
							PXInt32U size = 0;

							//assert(pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes * 2*sizeof(PXInt32U) < pxFile->DataSize);

							printf("RVA\n");

							for (PXInt32U i = 0; i < pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes; ++i)
							{
								PXFileReadI32U(pxFile, &virtualAddress);
								PXFileReadI32U(pxFile, &size);

								printf("[PXCOFF][RVA] 0x%p %i\n", virtualAddress, size);
							}
						}						
					}
				}			
			}

			// Optional Header Data Directories
			{
				PXCOFFOptionalHeaderDataDirectories* const pxCOFFOptionalHeaderDataDirectories = &pxCOFF->OptionalHeader.DataDirectories;

				const PXFileDataElementType pxDataStreamElementList[] =
				{
					{ &pxCOFFOptionalHeaderDataDirectories->ExportTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->ExportTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->ImportTable.RelativeVirtualAddress, PXDataTypeInt32ULE},
					{ &pxCOFFOptionalHeaderDataDirectories->ImportTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->ResourceTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->ResourceTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->ExceptionTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->ExceptionTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->CertificateTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->CertificateTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->BaseRelocationTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->BaseRelocationTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->Debug.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->Debug.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->Architecture.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->Architecture.Size, PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->GlobalPtr.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->GlobalPtr.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->ThreadLocalStorageTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->ThreadLocalStorageTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->LoadConfigTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->LoadConfigTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->BoundImport.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->BoundImport.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->ImportAddressTable.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->ImportAddressTable.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->DelayImportDescriptor.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->DelayImportDescriptor.Size,PXDataTypeInt32ULE },

					{ &pxCOFFOptionalHeaderDataDirectories->COMPlusRuntimeHeader.RelativeVirtualAddress, PXDataTypeInt32ULE },
					{ &pxCOFFOptionalHeaderDataDirectories->COMPlusRuntimeHeader.Size,PXDataTypeInt32ULE },

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
	

	pxCOFF->SectionTableList = PXNewList(PXSectionTable, pxCOFF->Header.NumberOfSections);

	for (PXInt16U sectionID = 0; sectionID < pxCOFF->Header.NumberOfSections; ++sectionID)
	{
		PXSectionTable* const pxSectionTableCurrent = &pxCOFF->SectionTableList[sectionID];

		{
			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{pxSectionTableCurrent->Name.Data, PXDataTypeDatax8},
				{&pxSectionTableCurrent->VirtualSize,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->VirtualAddress,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->SectionRawDataSize,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->SectionRawDataAdress,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->PointerToRelocations,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->PointerToLinenumbers,PXDataTypeInt32ULE},
				{&pxSectionTableCurrent->NumberOfRelocations,PXDataTypeInt16ULE},
				{&pxSectionTableCurrent->NumberOfLinenumbers,PXDataTypeInt16ULE},
				{&pxSectionTableCurrent->CharacteristicFlags, PXDataTypeInt32ULE},
			};

			const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

			assert(readBytes == 40u);


			pxSectionTableCurrent->Type = PXSectionTypeFromID(pxSectionTableCurrent->Name.Value);

#if PXCOFFDebug
			PXLogPrint
			(
				PXLoggingInfo,
				"COFF",
				"Parsing",
				"Section deteced %2i/%-2i - %-8s %6i Bytes",
				sectionID + 1, 
				pxCOFF->Header.NumberOfSections,
				pxSectionTableCurrent->Name.Data,
				pxSectionTableCurrent->SectionRawDataSize
			);
		}
#endif

		const PXSize oldPosition = pxFile->DataCursor;

		if (pxSectionTableCurrent->SectionRawDataSize > 0)
		{
			PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->SectionRawDataAdress);

			switch (pxSectionTableCurrent->Type)
			{
				case PXSectionTypeImportTables:
				{
					while (1)
					{
						const PXFileDataElementType pxDataStreamElementList[] =
						{
							{&pxCOFF->ImportDirectoryTable.ImportLookupTableOffset, PXDataTypeInt32ULE},
							{&pxCOFF->ImportDirectoryTable.TimeDateStamp,PXDataTypeInt32ULE},
							{&pxCOFF->ImportDirectoryTable.FowarderChainOffset,PXDataTypeInt32ULE},
							{&pxCOFF->ImportDirectoryTable.NameOffset,PXDataTypeInt32ULE},
							{&pxCOFF->ImportDirectoryTable.ImportAddressTableOffset,PXDataTypeInt32ULE}
						};

						const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

						assert(readBytes == 20u);

						const PXBool isLastEntry = 
							!
							(
							pxCOFF->ImportDirectoryTable.ImportLookupTableOffset ||
							pxCOFF->ImportDirectoryTable.TimeDateStamp ||
							pxCOFF->ImportDirectoryTable.FowarderChainOffset ||
							pxCOFF->ImportDirectoryTable.NameOffset ||
							pxCOFF->ImportDirectoryTable.ImportAddressTableOffset
							);

						if (isLastEntry)
						{
							PXLogPrint
							(
								PXLoggingInfo,
								"COFF",
								"Parsing",
								"[.idata][ImportDirectoryTable] Last entry deteced."
							);

							break;
						}

						PXSize namePosition = ((PXInt32S)pxCOFF->ImportDirectoryTable.NameOffset - (PXInt32S)pxSectionTableCurrent->SectionRawDataAdress);

						//PXFileCursorRewind(pxFile, namePosition);

						PXLogPrint
						(
							PXLoggingInfo,
							"COFF",
							"Parsing",
							"[.idata][ImportDirectoryTable] entry deteced."
						);

#if 0



						PXCOFFImportLookupTable pxCOFFImportLookupTable;
						PXSize importLookupTableOffsetReal = pxCOFF->ImportDirectoryTable.ImportLookupTableOffset;

						PXFileCursorMoveTo(pxFile, importLookupTableOffsetReal);

						for (PXBool isDone = PXFalse; isDone; )
						{
							switch (pxCOFF->Format)
							{
								case PXCOFFFormatPE32:
								{
									PXFileReadI32UE(pxFile, &pxCOFFImportLookupTable.OrdinalNumber32, PXEndianLittle);

									isDone = pxCOFFImportLookupTable.OrdinalNumber32 == 0;

									pxCOFFImportLookupTable.IsOrdinal = pxCOFFImportLookupTable.OrdinalNumber64 & 0x80000000;
									pxCOFFImportLookupTable.OrdinalNumber64 &= ~0x80000000;

									if (pxCOFFImportLookupTable.IsOrdinal)
									{
										printf("Ordinal\n");
									}
									else
									{

										printf("Name\n");
									}

									break;
								}
								case PXCOFFFormatPE32Plus:
								{
									PXFileReadI64UE(pxFile, &pxCOFFImportLookupTable.OrdinalNumber64, PXEndianLittle);

									isDone = pxCOFFImportLookupTable.OrdinalNumber64 == 0;

									pxCOFFImportLookupTable.IsOrdinal = pxCOFFImportLookupTable.OrdinalNumber64 & 0x8000000000000000;
									pxCOFFImportLookupTable.OrdinalNumber64 &= ~0x8000000000000000;

									if (pxCOFFImportLookupTable.IsOrdinal)
									{
										printf("Ordinal\n");
									}
									else
									{

										printf("Name\n");
									}

									break;
								}
							}
						}
#endif // 0

					}		


					break;
				}
				case PXSectionTypeExportTables:
				{
					// Export Directory Table
					{
						const PXFileDataElementType pxDataStreamElementList[] =
						{
							{&pxCOFF->ExportDirectoryTable.ExportFlags, PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.TimeDateStamp,PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.MajorVersion,PXDataTypeInt16ULE},
							{&pxCOFF->ExportDirectoryTable.MinorVersion,PXDataTypeInt16ULE},
							{&pxCOFF->ExportDirectoryTable.NameRVA,PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.OrdinalBase,PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.AddressTableEntries,PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.NumberOfNamePointers,PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.ExportAddressTableRVA,PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.NamePointerRVA, PXDataTypeInt32ULE},
							{&pxCOFF->ExportDirectoryTable.OrdinalTableRVA, PXDataTypeInt32ULE}
						};

						const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
					}

					// Parse Export Address Table
					{
						PXCOFFExportAddressTableEntry pxCOFFExportAddressTableEntryxxx;
						PXCOFFExportAddressTableEntry* pxCOFFExportAddressTableEntry = &pxCOFFExportAddressTableEntryxxx;

						const PXSize totalOffset = 0;// pxCOFF->OptionalHeader.StandardFields.ImageBase + pxCOFF->ExportDirectoryTable.ExportAddressTableRVA;

						PXFileCursorMoveTo(pxFile, totalOffset);

						const PXFileDataElementType pxDataStreamElementList[] =
						{
							{&pxCOFFExportAddressTableEntry->ExportSymbolOffset, PXDataTypeInt32ULE}
						};

						const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
					}


					break;
				}


				default:
					break;
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
				PXLogPrint
				(
					PXLoggingInfo,
					"COFF",
					"Parse",
					"[RelocationInformation] %6i,%6i,%6i",
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
		if (pxCOFF->Header.NumberOfSymbols > 0)
		{
			PXFileCursorMoveTo(pxFile, pxCOFF->Header.PointerToSymbolTable);

			PXCOFFSymbolTableEntry pxCOFFSymbolTableEntryXX;

#if PXCOFFDebug
			PXLogPrint
			(
				PXLoggingInfo,
				"COFF",
				"Parse",
				"Symbols deteced <%i>",
				pxCOFF->Header.NumberOfSymbols
			);
#endif

			for (PXInt32U i = 0; i < pxCOFF->Header.NumberOfSymbols; i++)
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
			PXLogPrint
			(
				PXLoggingInfo,
				"COFF",
				"Parsing",
				"*** No Symbols ***"
			);
		}
#endif
		
	}

	PXLogPrint
	(
		PXLoggingInfo,
		"COFF",
		"Parsing",
		"--- Finished Parsing ---"
	);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXCOFFSaveToFile(const PXCOFF* const pxCOFF, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}
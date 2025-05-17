#include "PXCOFF.h"

#include <PX/Math/PXMath.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Media/X86/PXX86.h>
#include <PX/OS/PXOS.h>

//#include <assert.h>


// Every segction starts with a "."
const char PXCOFFSectionTypeAlphaArchitectureInformation[4] = "arch";
const char PXCOFFSectionTypeUninitializedData[3] = "bss";
const char PXCOFFSectionTypeInitializedData[4] = "data";
const char PXCOFFSectionTypeExportTables[5] = "edata";
const char PXCOFFSectionTypeImportTables[5] = "idata";
const char PXCOFFSectionTypeExceptionInformationA[5] = "pdata";
const char PXCOFFInt4PXSectionTypeReadOnlyInitializedData8Make[5] = "rdata";
const char PXCOFFSectionTypeImageRelocations[5] = "reloc";
const char PXCOFFSectionTypeResourceDirectory[4] = "rsrc";
const char PXCOFFSectionTypeExecutableCode[4] = "text";
const char PXCOFFSectionTypeThreadLocalstorage[3] = "tls";
const char PXCOFFSectionTypeExceptionInformationB[5] = "xdata";
const char PXCOFFSectionTypeDebug[5] = "debug";
const char PXCOFFSectionTypeRec[7] = "drectve";

const char* PXCOFFSectionList[] =
{
    PXCOFFSectionTypeAlphaArchitectureInformation,
    PXCOFFSectionTypeUninitializedData,
    PXCOFFSectionTypeInitializedData,
    PXCOFFSectionTypeExportTables,
    PXCOFFSectionTypeImportTables,
    PXCOFFSectionTypeExceptionInformationA,
    PXCOFFInt4PXSectionTypeReadOnlyInitializedData8Make,
    PXCOFFSectionTypeImageRelocations,
    PXCOFFSectionTypeResourceDirectory,
    PXCOFFSectionTypeExecutableCode,
    PXCOFFSectionTypeThreadLocalstorage,
    PXCOFFSectionTypeExceptionInformationB,
    PXCOFFSectionTypeDebug,
    PXCOFFSectionTypeRec
};

const PXInt8U PXCOFFSectionListSize[] = 
{
    sizeof(PXCOFFSectionTypeAlphaArchitectureInformation),
    sizeof(PXCOFFSectionTypeUninitializedData),
    sizeof(PXCOFFSectionTypeInitializedData),
    sizeof(PXCOFFSectionTypeExportTables),
    sizeof(PXCOFFSectionTypeImportTables),
    sizeof(PXCOFFSectionTypeExceptionInformationA),
    sizeof(PXCOFFInt4PXSectionTypeReadOnlyInitializedData8Make),
    sizeof(PXCOFFSectionTypeImageRelocations),
    sizeof(PXCOFFSectionTypeResourceDirectory),
    sizeof(PXCOFFSectionTypeExecutableCode),
    sizeof(PXCOFFSectionTypeThreadLocalstorage),
    sizeof(PXCOFFSectionTypeExceptionInformationB),
    sizeof(PXCOFFSectionTypeDebug),
    sizeof(PXCOFFSectionTypeRec)
};

const PXInt8U PXCOFFSectionListAmount = sizeof(PXCOFFSectionListSize) / sizeof(PXInt8U);



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
    switch(valueID)
    {
        case 0x000:
            return PXCOFFMachineTypeUnkown;
        case 0x184:
            return PXCOFFMachineTypeAlpha;
        case 0x1c0:
            return PXCOFFMachineTypeARM;
        case 0x284:
            return PXCOFFMachineTypeALPHA64;
        case 0x14c:
            return PXCOFFMachineTypeI386;
        case 0x200:
            return PXCOFFMachineTypeIA64;
        case 0x268:
            return PXCOFFMachineTypeM68K;
        case 0x266:
            return PXCOFFMachineTypeMIPS16;
        case 0x366:
            return PXCOFFMachineTypeMIPSFPU;
        case 0x466:
            return PXCOFFMachineTypeMIPSFPU16;
        case 0x1f0:
            return PXCOFFMachineTypePOWERPC;
        case 0x162:
            return PXCOFFMachineTypeR3000;
        case 0x166:
            return PXCOFFMachineTypeR4000;
        case 0x168:
            return PXCOFFMachineTypeR10000;
        case 0x1a2:
            return PXCOFFMachineTypeSH3;
        case 0x1a6:
            return PXCOFFMachineTypeSH4;
        case 0x1c2:
            return PXCOFFMachineTypeTHUMB;

        default:
            return PXCOFFMachineTypeInvalid;
    }
}

PXCOFFFormat PXAPI PXCOFFFormatFromID(const PXInt16U valueID)
{
    switch(valueID)
    {
        case 0x010b:
            return PXCOFFFormatPE32;
        case 0x020b:
            return PXCOFFFormatPE32Plus;

        default:
            return PXCOFFFormatInvalid;
    }
}

PXActionResult PXAPI PXCOFFLoadFromFile(PXCOFF* const pxCOFF, PXFile* const pxFile)
{
    PXClear(PXCOFF, pxCOFF);

#if PXLogEnable && PXCOFFDebug
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

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&machineTypeID, PXTypeInt16ULE},
            {&pxCOFF->Header.NumberOfSections,PXTypeInt16ULE},
            {&pxCOFF->Header.TimeDateStamp,PXTypeInt32ULE},
            {&pxCOFF->Header.PointerToSymbolTable,PXTypeInt32ULE},
            {&pxCOFF->Header.NumberOfSymbols,PXTypeInt32ULE},
            {&pxCOFF->Header.SizeOfOptionalHeader, PXTypeInt16ULE},
            {&pxCOFF->Header.CharacteristicsFlagList, PXTypeInt16ULE}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        pxCOFF->Header.MachineType = PXCOFFMachineFromID(machineTypeID);
    }

    // Optional Header Standard Fields
    {
        const PXBool hasOptionalHeader = pxCOFF->Header.SizeOfOptionalHeader > 0;
        PXSize remainingOptionalHeaderOffset = pxCOFF->Header.SizeOfOptionalHeader;
        const PXSize syncPosition = pxFile->DataCursor + pxCOFF->Header.SizeOfOptionalHeader;

        if(hasOptionalHeader)
        {
            {
                PXInt16U magicNumber = 0;

                remainingOptionalHeaderOffset -= PXFileReadI16UE(pxFile, &magicNumber, PXEndianLittle);

                pxCOFF->OptionalHeader.Format = PXCOFFFormatFromID(magicNumber);

                switch(pxCOFF->OptionalHeader.Format)
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

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&magicNumber, PXTypeInt16ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.MajorLinkerVersion, PXTypeInt08U},
                    {&pxCOFF->OptionalHeader.StandardFields.MinorLinkerVersion, PXTypeInt08U},
                    {&pxCOFF->OptionalHeader.StandardFields.SizeOfCode, PXTypeInt32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.SizeOfInitializedData, PXTypeInt32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.SizeOfUninitializedData, PXTypeInt32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.AddressOfEntryPoint, PXTypeInt32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.BaseOfCode, PXTypeInt32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.BaseOfData, PXTypeInt32ULEOnlyIf32B}
                };

                const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                remainingOptionalHeaderOffset -= batchSize;

                switch(pxCOFF->OptionalHeader.Format)
                {
                    case PXCOFFFormatPE32:
                        //assert(batchSize == 28);
                        break;

                    case PXCOFFFormatPE32Plus:
                        //assert(batchSize == 24);
                        break;
                }
            }

            // Optional Header Windows NT - Specific Fields
            {
                // B
                {
                    const PXTypeEntry pxDataStreamElementList[] =
                    {
                        {&pxCOFF->OptionalHeader.WindowsNT.ImageBase, PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SectionAlignment,PXTypeInt32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.FileAlignment,PXTypeInt32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MajorOperatingSystemVersion,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MinorOperatingSystemVersion,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MajorImageVersion,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MinorImageVersion,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MajorSubsystemVersion,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MinorSubsystemVersion,PXTypeInt16ULE},
                        {0, PXTypePadding(4)},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfImage,PXTypeInt32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeaders,PXTypeInt32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.CheckSum,PXTypeInt32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.Subsystem,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.DLLCharacteristics,PXTypeInt16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfStackReserve,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfStackCommit,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeapReserve,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeapCommit,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.LoaderFlags,PXTypeInt32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes,PXTypeInt32ULE}
                    };

                    const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                    remainingOptionalHeaderOffset -= batchSize;

                    switch(pxCOFF->OptionalHeader.Format)
                    {
                        case PXCOFFFormatPE32:
                            //assert(batchSize == 68u);
                            break;

                        case PXCOFFFormatPE32Plus:
                            //assert(batchSize == 88u);
                            break;
                    }

                    // Parse version
                    {
                        switch(pxCOFF->OptionalHeader.WindowsNT.MinorImageVersion)
                        {
                            case 4:
                            {
                                pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersionNT;
                                break;
                            }
                            case 5:
                            {
                                switch(pxCOFF->OptionalHeader.WindowsNT.MajorSubsystemVersion)
                                {
                                    case 0:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersion2000;
                                        break;
                                    case 1:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersionXP;
                                        break;
                                    case 2:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersionServer2003;
                                        break;
                                }

                                break;
                            }
                            case 6:
                            {
                                switch(pxCOFF->OptionalHeader.WindowsNT.MajorSubsystemVersion)
                                {
                                    case 0:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersionVista;
                                        break;
                                    case 1:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersion7;
                                        break;
                                    case 2:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersion8;
                                        break;
                                    case 3:
                                        pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersion8x1;
                                        break;
                                }

                                break;
                            }
                            case 0xA:
                                pxCOFF->OptionalHeader.WindowsNT.WindowsVersion = PXWindowsVersion10;
                                break;
                        }
                    }

                    // D
                    {


                        if(pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes > 0 && 0)
                        {
                            PXInt32U virtualAddress = 0;
                            PXInt32U size = 0;

                            //assert(pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes * 2*sizeof(PXInt32U) < pxFile->DataSize);

#if PXLogEnable && PXCOFFDebug
                            PXLogPrint
                            (
                                PXLoggingInfo,
                                "COFF",
                                "Parsing",
                                "RVA"
                            );
#endif

                            for(PXInt32U i = 0; i < pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes; ++i)
                            {
                                PXFileReadI32U(pxFile, &virtualAddress);
                                PXFileReadI32U(pxFile, &size);

#if PXLogEnable && PXCOFFDebug
                                PXLogPrint
                                (
                                    PXLoggingInfo,
                                    "COFF",
                                    "Parsing-RVA",
                                    "0x%p %i",
                                    virtualAddress,
                                    size
                                );
#endif
                            }
                        }
                    }
                }
            }

            // Optional Header Data Directories
            {
                PXCOFFOptionalHeaderDataDirectories* const pxCOFFOptionalHeaderDataDirectories = &pxCOFF->OptionalHeader.DataDirectories;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    { &pxCOFFOptionalHeaderDataDirectories->ExportTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ExportTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ImportTable.RelativeVirtualAddress, PXTypeInt32ULE},
                    { &pxCOFFOptionalHeaderDataDirectories->ImportTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ResourceTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ResourceTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ExceptionTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ExceptionTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->CertificateTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->CertificateTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->BaseRelocationTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->BaseRelocationTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->Debug.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->Debug.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->Architecture.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->Architecture.Size, PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->GlobalPtr.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->GlobalPtr.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ThreadLocalStorageTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ThreadLocalStorageTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->LoadConfigTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->LoadConfigTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->BoundImport.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->BoundImport.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ImportAddressTable.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ImportAddressTable.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->DelayImportDescriptor.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->DelayImportDescriptor.Size,PXTypeInt32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->COMPlusRuntimeHeader.RelativeVirtualAddress, PXTypeInt32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->COMPlusRuntimeHeader.Size,PXTypeInt32ULE },

                    { 0, PXTypePadding(8)}
                };

                const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                remainingOptionalHeaderOffset -= batchSize;

                //assert(batchSize == 128u);
            }

            pxFile->DataCursor = syncPosition;
        }
    }

    // Parse SectionTable


    pxCOFF->SectionTableList = PXMemoryHeapCallocT(PXSectionTable, pxCOFF->Header.NumberOfSections);
    pxCOFF->SectionTableListSize = pxCOFF->Header.NumberOfSections;

    for(PXInt16U sectionID = 0; sectionID < pxCOFF->Header.NumberOfSections; ++sectionID)
    {
        PXSectionTable* const pxSectionTableCurrent = &pxCOFF->SectionTableList[sectionID];

        {
            const PXTypeEntry pxDataStreamElementList[] =
            {
                {pxSectionTableCurrent->Name.Data, PXTypeDatax8},
                {&pxSectionTableCurrent->VirtualSize,PXTypeInt32ULE},
                {&pxSectionTableCurrent->VirtualAddress,PXTypeInt32ULE},
                {&pxSectionTableCurrent->SectionRawDataSize,PXTypeInt32ULE},
                {&pxSectionTableCurrent->SectionRawDataAdress,PXTypeInt32ULE},
                {&pxSectionTableCurrent->PointerToRelocations,PXTypeInt32ULE},
                {&pxSectionTableCurrent->PointerToLinenumbers,PXTypeInt32ULE},
                {&pxSectionTableCurrent->NumberOfRelocations,PXTypeInt16ULE},
                {&pxSectionTableCurrent->NumberOfLinenumbers,PXTypeInt16ULE},
                {&pxSectionTableCurrent->CharacteristicFlags, PXTypeInt32ULE},
            };

            const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

            //assert(readBytes == 40u);

            if(pxSectionTableCurrent->Name.Data[0] != '.')
            {
                // Illegal?
            }

            PXMemoryMove(&pxSectionTableCurrent->Name.Data[1], 7, &pxSectionTableCurrent->Name.Data[0], 8);

            pxSectionTableCurrent->Type = PXMemoryCompareSVI8(&pxSectionTableCurrent->Name.Data, PXCOFFSectionList, PXCOFFSectionListSize, PXCOFFSectionListAmount);


           // pxSectionTableCurrent->Type = PXSectionTypeFromID(pxSectionTableCurrent->Name.Value);

#if PXLogEnable && PXCOFFDebug
            PXLogPrint
            (
                PXLoggingInfo,
                "COFF",
                "Parsing",
                "Section deteced <%-8s> (%2i/%-2i)\n"
                "%25s : %i\n"
                "%25s : 0x%8.8X\n"
                "%25s : %i\n"
                "%25s : 0x%8.8X\n"
                "%25s : 0x%8.8X\n"
                "%25s : 0x%8.8X\n"
                "%25s : %i\n"
                "%25s : %i\n"
                "%25s : %i\n",

                pxSectionTableCurrent->Name.Data,
                sectionID + 1,
                pxCOFF->Header.NumberOfSections,

                "VirtualSize", pxSectionTableCurrent->VirtualSize,
                "VirtualAddress", pxSectionTableCurrent->VirtualAddress,
                "SectionRawDataSize", pxSectionTableCurrent->SectionRawDataSize,
                "SectionRawDataAdress", pxSectionTableCurrent->SectionRawDataAdress,
                "PointerToRelocations", pxSectionTableCurrent->PointerToRelocations,
                "PointerToLinenumbers", pxSectionTableCurrent->PointerToLinenumbers,
                "NumberOfRelocations", pxSectionTableCurrent->NumberOfRelocations,
                "NumberOfLinenumbers", pxSectionTableCurrent->NumberOfLinenumbers,
                "CharacteristicFlags", pxSectionTableCurrent->CharacteristicFlags
            );
#endif
        }

        const PXSize oldPosition = pxFile->DataCursor;

        if(pxSectionTableCurrent->SectionRawDataSize > 0)
        {
            PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->SectionRawDataAdress);

            switch(pxSectionTableCurrent->Type)
            {
                case PXSectionTypeExecutableCode:
                {
                    // Move to actual data
                  //  PXSize oldPosition;
                  //  PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->SectionRawDataAdress);


                    PXX86InstructionWalk(pxFile, pxSectionTableCurrent);


                    break;
                }

                case PXSectionTypeImportTables:
                {
                    while(1)
                    {
                        const PXTypeEntry pxDataStreamElementList[] =
                        {
                            {&pxCOFF->ImportDirectoryTable.ImportLookupTableOffset, PXTypeInt32ULE},
                            {&pxCOFF->ImportDirectoryTable.TimeDateStamp,PXTypeInt32ULE},
                            {&pxCOFF->ImportDirectoryTable.FowarderChainOffset,PXTypeInt32ULE},
                            {&pxCOFF->ImportDirectoryTable.NameOffset,PXTypeInt32ULE},
                            {&pxCOFF->ImportDirectoryTable.ImportAddressTableOffset,PXTypeInt32ULE}
                        };

                        const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                        //assert(readBytes == 20u);

                        const PXBool isLastEntry =
                            !
                            (
                            pxCOFF->ImportDirectoryTable.ImportLookupTableOffset ||
                            pxCOFF->ImportDirectoryTable.TimeDateStamp ||
                            pxCOFF->ImportDirectoryTable.FowarderChainOffset ||
                            pxCOFF->ImportDirectoryTable.NameOffset ||
                            pxCOFF->ImportDirectoryTable.ImportAddressTableOffset
                            );

                        if(isLastEntry)
                        {
#if PXLogEnable
                            PXLogPrint
                            (
                                PXLoggingInfo,
                                "COFF",
                                "Parsing",
                                "[.idata][ImportDirectoryTable] Last entry deteced."
                            );
#endif

                            break;
                        }

                        PXSize namePosition = ((PXInt32S)pxCOFF->ImportDirectoryTable.NameOffset - (PXInt32S)pxSectionTableCurrent->SectionRawDataAdress);

                        //PXFileCursorRewind(pxFile, namePosition);

#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "COFF",
                            "Parsing",
                            "[.idata][ImportDirectoryTable] entry deteced."
                        );
#endif


#if 0



                        PXCOFFImportLookupTable pxCOFFImportLookupTable;
                        PXSize importLookupTableOffsetReal = pxCOFF->ImportDirectoryTable.ImportLookupTableOffset;

                        PXFileCursorMoveTo(pxFile, importLookupTableOffsetReal);

                        for(PXBool isDone = PXFalse; isDone; )
                        {
                            switch(pxCOFF->Format)
                            {
                                case PXCOFFFormatPE32:
                                {
                                    PXFileReadI32UE(pxFile, &pxCOFFImportLookupTable.OrdinalNumber32, PXEndianLittle);

                                    isDone = pxCOFFImportLookupTable.OrdinalNumber32 == 0;

                                    pxCOFFImportLookupTable.IsOrdinal = pxCOFFImportLookupTable.OrdinalNumber64 & 0x80000000;
                                    pxCOFFImportLookupTable.OrdinalNumber64 &= ~0x80000000;

                                    if(pxCOFFImportLookupTable.IsOrdinal)
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

                                    if(pxCOFFImportLookupTable.IsOrdinal)
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
                        const PXTypeEntry pxDataStreamElementList[] =
                        {
                            {&pxCOFF->ExportDirectoryTable.ExportFlags, PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.TimeDateStamp,PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.MajorVersion,PXTypeInt16ULE},
                            {&pxCOFF->ExportDirectoryTable.MinorVersion,PXTypeInt16ULE},
                            {&pxCOFF->ExportDirectoryTable.NameRVA,PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.OrdinalBase,PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.AddressTableEntries,PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.NumberOfNamePointers,PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.ExportAddressTableRVA,PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.NamePointerRVA, PXTypeInt32ULE},
                            {&pxCOFF->ExportDirectoryTable.OrdinalTableRVA, PXTypeInt32ULE}
                        };

                        const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
                    }

                    // Parse Export Address Table
                    {
                        PXCOFFExportAddressTableEntry pxCOFFExportAddressTableEntryxxx;
                        PXCOFFExportAddressTableEntry* pxCOFFExportAddressTableEntry = &pxCOFFExportAddressTableEntryxxx;

                        const PXSize totalOffset = 0;// pxCOFF->OptionalHeader.StandardFields.ImageBase + pxCOFF->ExportDirectoryTable.ExportAddressTableRVA;

                        PXFileCursorMoveTo(pxFile, totalOffset);

                        const PXTypeEntry pxDataStreamElementList[] =
                        {
                            {&pxCOFFExportAddressTableEntry->ExportSymbolOffset, PXTypeInt32ULE}
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
        if(pxSectionTableCurrent->NumberOfRelocations > 0)
        {
            PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->PointerToRelocations);

            for(PXInt16U i = 0; i < pxSectionTableCurrent->NumberOfRelocations; ++i)
            {
                PXRelocationInformation pxRelocationInformation;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&pxRelocationInformation.VirtualAddressOfReference, PXTypeInt32ULE},
                    {&pxRelocationInformation.SymbolTableIndex,PXTypeInt32ULE},
                    {&pxRelocationInformation.RelocationType,PXTypeInt16ULE}
                };

                PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

#if PXLogEnable && PXCOFFDebug
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
        if(pxSectionTableCurrent->NumberOfLinenumbers > 0)
        {
            PXLineNumberEntry pxLineNumberEntryXX;

            PXFileCursorMoveTo(pxFile, pxSectionTableCurrent->PointerToLinenumbers);

            for(PXInt16U i = 0; i < pxSectionTableCurrent->NumberOfLinenumbers; ++i)
            {
                PXLineNumberEntry* pxLineNumberEntry = &pxLineNumberEntryXX;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&pxLineNumberEntry->SymbolIndex, PXTypeInt32ULE},
                    {&pxLineNumberEntry->LineNumber,PXTypeInt16ULE}
                };

                PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));


#if PXLogEnable && PXCOFFDebug
                PXLogPrint
                (
                    PXLoggingInfo,
                    "COFF",
                    "Parsing-Section",
                    "[LineEntry] %3i/%-3i - Line:%i Index:%i",
                    i + 1,
                    pxSectionTableCurrent->NumberOfLinenumbers,
                    pxLineNumberEntry->LineNumber,
                    pxLineNumberEntry->SymbolIndex
                );
#endif
            }
        }

        PXFileCursorMoveTo(pxFile, oldPosition); // Restore position to further parse the section table
    }

    // Parse actual symbols
    {
        if(pxCOFF->Header.NumberOfSymbols > 0)
        {
            PXFileCursorMoveTo(pxFile, pxCOFF->Header.PointerToSymbolTable);

            PXCOFFSymbolTableEntry pxCOFFSymbolTableEntryXX;

#if PXLogEnable && PXCOFFDebug
            PXLogPrint
            (
                PXLoggingInfo,
                "COFF",
                "Parse-Symbols",
                "Deteced <%i>",
                pxCOFF->Header.NumberOfSymbols
            );
#endif

            for(PXInt32U i = 0; i < pxCOFF->Header.NumberOfSymbols; i++)
            {
                PXCOFFSymbolTableEntry* pxCOFFSymbolTableEntry = &pxCOFFSymbolTableEntryXX;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {pxCOFFSymbolTableEntry->Name, PXTypeDatax8},
                    {&pxCOFFSymbolTableEntry->ValueOfSymbol,PXTypeInt32ULE},
                    {&pxCOFFSymbolTableEntry->SectionNumber,PXTypeInt16ULE},
                    {&pxCOFFSymbolTableEntry->TypeAndDerived,PXTypeInt16ULE},
                    {&pxCOFFSymbolTableEntry->StorageClass,PXTypeInt08U},
                    {&pxCOFFSymbolTableEntry->NumberOfAuxiliaryEntries,PXTypeInt08U}
                };

                const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                //assert(readBytes == 18u);


                if(pxCOFFSymbolTableEntry->DoesNotUseExternalString && pxCOFFSymbolTableEntry->DoesNotUseExternalString != 0xCCCCCCCC)
                {
#if PXLogEnable && PXCOFFDebug
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "COFF",
                        "Parse-Symbols",
                        "%8.8s",
                        pxCOFFSymbolTableEntry->Name
                    );
#endif
                }
                else
                {
                    PXSize oolPos = pxFile->DataCursor;

                    PXFileCursorMoveTo(pxFile, pxCOFFSymbolTableEntry->NameReferenceOffset);

                    char* xxx = (char*)PXFileCursorPosition(pxFile);


#if PXLogEnable && PXCOFFDebug
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "COFF",
                        "Parse-Symbols",
                        " -------- -> %s",
                        xxx
                    );
#endif

                    PXFileCursorMoveTo(pxFile, oolPos);
                }
            }
        }
#if PXLogEnable && PXCOFFDebug
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

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "COFF",
        "Parsing",
        "--- Finished Parsing ---"
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXCOFFSaveToFile(const PXCOFF* const pxCOFF, PXFile* const pxFile)
{
    return PXActionRefusedNotImplemented;
}
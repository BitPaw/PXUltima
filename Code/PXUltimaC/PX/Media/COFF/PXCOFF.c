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

const PXI8U PXCOFFSectionListSize[] = 
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

const PXI8U PXCOFFSectionListAmount = sizeof(PXCOFFSectionListSize) / sizeof(PXI8U);

const char PXCOFFText[] = "COFF";


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

PXCOFFMachineType PXAPI PXCOFFMachineFromID(const PXI16U valueID)
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

PXCOFFFormat PXAPI PXCOFFFormatFromID(const PXI16U valueID)
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

PXResult PXAPI PXCOFFLoadFromFile(PXCOFF PXREF pxCOFF, PXFile PXREF pxFile)
{
    PXClear(PXCOFF, pxCOFF);

#if PXLogEnable && PXCOFFDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXCOFFText,
        "Parsing",
        "Start"
    );
#endif

    // COFF File Header
    {
        PXI16U machineTypeID = 0;

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&machineTypeID, PXTypeI16ULE},
            {&pxCOFF->Header.NumberOfSections,PXTypeI16ULE},
            {&pxCOFF->Header.TimeDateStamp,PXTypeI32ULE},
            {&pxCOFF->Header.PointerToSymbolTable,PXTypeI32ULE},
            {&pxCOFF->Header.NumberOfSymbols,PXTypeI32ULE},
            {&pxCOFF->Header.SizeOfOptionalHeader, PXTypeI16ULE},
            {&pxCOFF->Header.CharacteristicsFlagList, PXTypeI16ULE}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        pxCOFF->Header.MachineType = PXCOFFMachineFromID(machineTypeID);
    }

    // Optional Header Standard Fields
    {
        const PXBool hasOptionalHeader = pxCOFF->Header.SizeOfOptionalHeader > 0;
        PXSize remainingOptionalHeaderOffset = pxCOFF->Header.SizeOfOptionalHeader;
        const PXSize syncPosition = PXFileDataPosition(pxFile) + pxCOFF->Header.SizeOfOptionalHeader;

        if(hasOptionalHeader)
        {
            {
                PXI16U magicNumber = 0;

                remainingOptionalHeaderOffset -= PXFileReadI16UE(pxFile, &magicNumber, PXEndianLittle);

                pxCOFF->OptionalHeader.Format = PXCOFFFormatFromID(magicNumber);

                switch(pxCOFF->OptionalHeader.Format)
                {
                    case PXCOFFFormatPE32:
                        PXFileBitFormatOfDataSet(pxFile, PXBitFormat32);
                        break;

                    case PXCOFFFormatPE32Plus:
                        PXFileBitFormatOfDataSet(pxFile, PXBitFormat64);
                        break;
                }
            }


            {
                PXI16U magicNumber = 0;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&magicNumber, PXTypeI16ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.MajorLinkerVersion, PXTypeI08U},
                    {&pxCOFF->OptionalHeader.StandardFields.MinorLinkerVersion, PXTypeI08U},
                    {&pxCOFF->OptionalHeader.StandardFields.SizeOfCode, PXTypeI32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.SizeOfInitializedData, PXTypeI32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.SizeOfUninitializedData, PXTypeI32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.AddressOfEntryPoint, PXTypeI32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.BaseOfCode, PXTypeI32ULE},
                    {&pxCOFF->OptionalHeader.StandardFields.BaseOfData, PXTypeI32ULEOnlyIf32B}
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
                        {&pxCOFF->OptionalHeader.WindowsNT.SectionAlignment,PXTypeI32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.FileAlignment,PXTypeI32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MajorOperatingSystemVersion,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MinorOperatingSystemVersion,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MajorImageVersion,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MinorImageVersion,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MajorSubsystemVersion,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.MinorSubsystemVersion,PXTypeI16ULE},
                        {0, PXTypePadding(4)},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfImage,PXTypeI32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeaders,PXTypeI32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.CheckSum,PXTypeI32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.Subsystem,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.DLLCharacteristics,PXTypeI16ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfStackReserve,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfStackCommit,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeapReserve,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.SizeOfHeapCommit,PXTypeAdressFlex},
                        {&pxCOFF->OptionalHeader.WindowsNT.LoaderFlags,PXTypeI32ULE},
                        {&pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes,PXTypeI32ULE}
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
                            PXI32U virtualAddress = 0;
                            PXI32U size = 0;

                            //assert(pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes * 2*sizeof(PXI32U) < pxFile->Buffer.DataSize);

#if PXLogEnable && PXCOFFDebug
                            PXLogPrint
                            (
                                PXLoggingInfo,
                                PXCOFFText,
                                "Parsing",
                                "RVA"
                            );
#endif

                            for(PXI32U i = 0; i < pxCOFF->OptionalHeader.WindowsNT.NumberOfRvaAndSizes; ++i)
                            {
                                PXFileReadI32U(pxFile, &virtualAddress);
                                PXFileReadI32U(pxFile, &size);

#if PXLogEnable && PXCOFFDebug
                                PXLogPrint
                                (
                                    PXLoggingInfo,
                                    PXCOFFText,
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
                PXCOFFOptionalHeaderDataDirectories PXREF pxCOFFOptionalHeaderDataDirectories = &pxCOFF->OptionalHeader.DataDirectories;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    { &pxCOFFOptionalHeaderDataDirectories->ExportTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ExportTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ImportTable.RelativeVirtualAddress, PXTypeI32ULE},
                    { &pxCOFFOptionalHeaderDataDirectories->ImportTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ResourceTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ResourceTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ExceptionTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ExceptionTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->CertificateTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->CertificateTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->BaseRelocationTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->BaseRelocationTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->Debug.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->Debug.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->Architecture.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->Architecture.Size, PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->GlobalPtr.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->GlobalPtr.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ThreadLocalStorageTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ThreadLocalStorageTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->LoadConfigTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->LoadConfigTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->BoundImport.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->BoundImport.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->ImportAddressTable.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->ImportAddressTable.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->DelayImportDescriptor.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->DelayImportDescriptor.Size,PXTypeI32ULE },

                    { &pxCOFFOptionalHeaderDataDirectories->COMPlusRuntimeHeader.RelativeVirtualAddress, PXTypeI32ULE },
                    { &pxCOFFOptionalHeaderDataDirectories->COMPlusRuntimeHeader.Size,PXTypeI32ULE },

                    { 0, PXTypePadding(8)}
                };

                const PXSize batchSize = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

                remainingOptionalHeaderOffset -= batchSize;

                //assert(batchSize == 128u);
            }

            PXFileCursorMoveTo(pxFile, syncPosition);
        }
    }

    // Parse SectionTable


    pxCOFF->SectionTableList = PXMemoryHeapCallocT(PXSectionTable, pxCOFF->Header.NumberOfSections);
    pxCOFF->SectionTableListSize = pxCOFF->Header.NumberOfSections;

    for(PXI16U sectionID = 0; sectionID < pxCOFF->Header.NumberOfSections; ++sectionID)
    {
        PXSectionTable PXREF pxSectionTableCurrent = &pxCOFF->SectionTableList[sectionID];

        {
            const PXTypeEntry pxDataStreamElementList[] =
            {
                {pxSectionTableCurrent->Name.Data, PXTypeDatax8},
                {&pxSectionTableCurrent->VirtualSize,PXTypeI32ULE},
                {&pxSectionTableCurrent->VirtualAddress,PXTypeI32ULE},
                {&pxSectionTableCurrent->SectionRawDataSize,PXTypeI32ULE},
                {&pxSectionTableCurrent->SectionRawDataAdress,PXTypeI32ULE},
                {&pxSectionTableCurrent->PointerToRelocations,PXTypeI32ULE},
                {&pxSectionTableCurrent->PointerToLinenumbers,PXTypeI32ULE},
                {&pxSectionTableCurrent->NumberOfRelocations,PXTypeI16ULE},
                {&pxSectionTableCurrent->NumberOfLinenumbers,PXTypeI16ULE},
                {&pxSectionTableCurrent->CharacteristicFlags, PXTypeI32ULE},
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
                PXCOFFText,
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

        const PXSize oldPosition = PXFileDataPosition(pxFile);

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
                            {&pxCOFF->ImportDirectoryTable.ImportLookupTableOffset, PXTypeI32ULE},
                            {&pxCOFF->ImportDirectoryTable.TimeDateStamp,PXTypeI32ULE},
                            {&pxCOFF->ImportDirectoryTable.FowarderChainOffset,PXTypeI32ULE},
                            {&pxCOFF->ImportDirectoryTable.NameOffset,PXTypeI32ULE},
                            {&pxCOFF->ImportDirectoryTable.ImportAddressTableOffset,PXTypeI32ULE}
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
                                PXCOFFText,
                                "Parsing",
                                "[.idata][ImportDirectoryTable] Last entry deteced."
                            );
#endif

                            break;
                        }

                        PXSize namePosition = ((PXI32S)pxCOFF->ImportDirectoryTable.NameOffset - (PXI32S)pxSectionTableCurrent->SectionRawDataAdress);

                        //PXFileCursorRewind(pxFile, namePosition);

#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingInfo,
                            PXCOFFText,
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
                            {&pxCOFF->ExportDirectoryTable.ExportFlags, PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.TimeDateStamp,PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.MajorVersion,PXTypeI16ULE},
                            {&pxCOFF->ExportDirectoryTable.MinorVersion,PXTypeI16ULE},
                            {&pxCOFF->ExportDirectoryTable.NameRVA,PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.OrdinalBase,PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.AddressTableEntries,PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.NumberOfNamePointers,PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.ExportAddressTableRVA,PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.NamePointerRVA, PXTypeI32ULE},
                            {&pxCOFF->ExportDirectoryTable.OrdinalTableRVA, PXTypeI32ULE}
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
                            {&pxCOFFExportAddressTableEntry->ExportSymbolOffset, PXTypeI32ULE}
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

            for(PXI16U i = 0; i < pxSectionTableCurrent->NumberOfRelocations; ++i)
            {
                PXRelocationInformation pxRelocationInformation;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&pxRelocationInformation.VirtualAddressOfReference, PXTypeI32ULE},
                    {&pxRelocationInformation.SymbolTableIndex,PXTypeI32ULE},
                    {&pxRelocationInformation.RelocationType,PXTypeI16ULE}
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

            for(PXI16U i = 0; i < pxSectionTableCurrent->NumberOfLinenumbers; ++i)
            {
                PXLineNumberEntry* pxLineNumberEntry = &pxLineNumberEntryXX;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {&pxLineNumberEntry->SymbolIndex, PXTypeI32ULE},
                    {&pxLineNumberEntry->LineNumber,PXTypeI16ULE}
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

            for(PXI32U i = 0; i < pxCOFF->Header.NumberOfSymbols; i++)
            {
                PXCOFFSymbolTableEntry* pxCOFFSymbolTableEntry = &pxCOFFSymbolTableEntryXX;

                const PXTypeEntry pxDataStreamElementList[] =
                {
                    {pxCOFFSymbolTableEntry->Name, PXTypeDatax8},
                    {&pxCOFFSymbolTableEntry->ValueOfSymbol,PXTypeI32ULE},
                    {&pxCOFFSymbolTableEntry->SectionNumber,PXTypeI16ULE},
                    {&pxCOFFSymbolTableEntry->TypeAndDerived,PXTypeI16ULE},
                    {&pxCOFFSymbolTableEntry->StorageClass,PXTypeI08U},
                    {&pxCOFFSymbolTableEntry->NumberOfAuxiliaryEntries,PXTypeI08U}
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
                    PXSize oolPos = PXFileDataPosition(pxFile);

                    PXFileCursorMoveTo(pxFile, pxCOFFSymbolTableEntry->NameReferenceOffset);

                    char* xxx = (char*)PXFileDataAtCursor(pxFile);


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

    return PXResultOK;
}

PXResult PXAPI PXCOFFSaveToFile(const PXCOFF PXREF pxCOFF, PXFile PXREF pxFile)
{
    return PXActionRefusedNotImplemented;
}
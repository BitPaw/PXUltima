#pragma once

#ifndef PXCOFFIncluded
#define PXCOFFIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXCOFFMachineType_
{
    PXCOFFMachineTypeInvalid,
    PXCOFFMachineTypeUnkown, // 0x0 Contents assumed to be applicable to any    machine type.
    PXCOFFMachineTypeAlpha, // 0x184 Alpha AXP™.
    PXCOFFMachineTypeARM, //  0x1c0
    PXCOFFMachineTypeALPHA64, //  0x284 Alpha AXP™ 64 - bit.
    PXCOFFMachineTypeI386, //  0x14c Intel 386 or later, and compatible processors.
    PXCOFFMachineTypeIA64, //  0x200 Intel IA64™
    PXCOFFMachineTypeM68K, //  0x268 Motorola 68000 series.
    PXCOFFMachineTypeMIPS16, //  0x266
    PXCOFFMachineTypeMIPSFPU, //  0x366 MIPS with FPU
    PXCOFFMachineTypeMIPSFPU16, //  0x466 MIPS16 with FPU
    PXCOFFMachineTypePOWERPC, //  0x1f0 Power PC, little endian.
    PXCOFFMachineTypeR3000, //  0x162
    PXCOFFMachineTypeR4000, //  0x166 MIPS® little endian.
    PXCOFFMachineTypeR10000, //  0x168
    PXCOFFMachineTypeSH3, //  0x1a2 Hitachi SH3
    PXCOFFMachineTypeSH4, //  0x1a6 Hitachi SH4
    PXCOFFMachineTypeTHUMB //  0x1c2
}
PXCOFFMachineType;

typedef enum PXCOFFFormat_
{
    PXCOFFFormatInvalid,
    PXCOFFFormatPE32,
    PXCOFFFormatPE32Plus
}
PXCOFFFormat;


#define PXSectionTypeInvalid    0xFF
#define PXSectionTypeAlphaArchitectureInformation   1
#define PXSectionTypeUninitializedData      2
#define PXSectionTypeInitializedData        3
#define PXSectionTypeExportTables           4
#define PXSectionTypeImportTables           5
#define PXSectionTypeExceptionInformationA  6
#define PXSectionTypeReadOnlyInitializedData 7
#define PXSectionTypeImageRelocations       8
#define PXSectionTypeResourceDirectory      9
#define PXSectionTypeExecutableCode         10
#define PXSectionTypeThreadLocalstorage     11
#define PXSectionTypeExceptionInformationB  12
#define PXSectionTypeDebug                  13
#define PXSectionTypeDirective              14



// 40 Bytes in file
typedef struct PXSectionTable_
{
    PXI64UCluster Name;

    union
    {
        PXI32U PhysicalAddress;
        PXI32U VirtualSize;
    };

    PXI32U VirtualAddress;
    PXI32U SectionRawDataSize;
    PXI32U SectionRawDataAdress;
    PXI32U PointerToRelocations;
    PXI32U PointerToLinenumbers;
    PXI16U NumberOfRelocations;
    PXI16U NumberOfLinenumbers;
    PXI32U CharacteristicFlags;

    PXI8U Type;
}
PXSectionTable;

typedef struct PXCOFFSymbolTableEntry_
{
    union
    {
        char Name[8];

        struct
        {
            PXI32U DoesNotUseExternalString;
            PXI32U NameReferenceOffset;
        };

        // If the symbol name is greater than eight characters, this field is treated as two integers. The entire
        //symbol name is stored in the string table.Bytes 0 - 3 contain 0, and bytes 4 - 7 are an offset into the
        //    string table.
    };

    PXI32U ValueOfSymbol;
    PXI16S SectionNumber;
    PXI16U TypeAndDerived;
    PXI8U StorageClass;
    PXI8U NumberOfAuxiliaryEntries;
}
PXCOFFSymbolTableEntry;

typedef struct PXRelocationInformation_
{
    PXI32U VirtualAddressOfReference;
    PXI32U SymbolTableIndex;
    PXI16U RelocationType;
}
PXRelocationInformation;

typedef struct PXLineNumberEntry_
{
    union
    {
        PXI32U SymbolIndex;
        PXI32U PhysicalAddress;
    };
    PXI16U LineNumber;
}
PXLineNumberEntry;

typedef struct PXCOFFExportDirectoryTable_
{
    PXI32U ExportFlags;
    PXI32U TimeDateStamp;
    PXI16U MajorVersion;
    PXI16U MinorVersion;
    PXI32U NameRVA;
    PXI32U OrdinalBase;
    PXI32U AddressTableEntries;
    PXI32U NumberOfNamePointers;
    PXI32U ExportAddressTableRVA;
    PXI32U NamePointerRVA;
    PXI32U OrdinalTableRVA;
}
PXCOFFExportDirectoryTable;

typedef struct PXCOFFExportAddressTableEntry_
{
    union
    {
        PXI32U ExportSymbolOffset;
        PXI32U ForwarderOffset;
    };
}
PXCOFFExportAddressTableEntry;


typedef struct PXCOFFImportDirectoryTable_
{
    PXI32U ImportLookupTableOffset;
    PXI32U TimeDateStamp;
    PXI32U FowarderChainOffset;
    PXI32U NameOffset;
    PXI32U ImportAddressTableOffset;
}
PXCOFFImportDirectoryTable;

typedef struct PXCOFFImportLookupTable_
{
    union
    {
        union
        {
            PXI64U OrdinalNumber64;
            PXI64U Name64;
        };
        union
        {
            PXI32U OrdinalNumber32;
            PXI32U Name32;
        };
    };

    PXBool IsOrdinal;
}
PXCOFFImportLookupTable;

typedef struct PXCOFFHeader_
{
    PXI32U TimeDateStamp;
    PXI32U PointerToSymbolTable;
    PXI32U NumberOfSymbols;
    PXI16U NumberOfSections;
    PXI16U SizeOfOptionalHeader;
    PXI16U CharacteristicsFlagList;
    PXCOFFMachineType MachineType;
}
PXCOFFHeader;


typedef struct PXCOFFOptionalHeaderStandardFields_
{
    int StateType;

    PXI8U MajorLinkerVersion;
    PXI8U MinorLinkerVersion;

    PXI32U SizeOfCode;
    PXI32U SizeOfInitializedData;
    PXI32U SizeOfUninitializedData;
    PXI32U AddressOfEntryPoint;
    PXI32U BaseOfCode;
    PXI32U BaseOfData; // Only in PE32, not used in PE32+
}
PXCOFFOptionalHeaderStandardFields;

typedef struct PXCOFFOptionalHeaderWindowsNT_
{
    PXI64U ImageBase;
    PXI32U SectionAlignment;
    PXI32U FileAlignment;
    PXI16U MajorOperatingSystemVersion;
    PXI16U MinorOperatingSystemVersion;
    PXI16U MajorImageVersion;
    PXI16U MinorImageVersion;
    PXI16U MajorSubsystemVersion;
    PXI16U MinorSubsystemVersion;
    PXI32U SizeOfImage;
    PXI32U SizeOfHeaders;
    PXI32U CheckSum;
    PXI16U Subsystem;
    PXI16U DLLCharacteristics;
    PXI64U SizeOfStackReserve;
    PXI64U SizeOfStackCommit;
    PXI64U SizeOfHeapReserve;
    PXI64U SizeOfHeapCommit;
    PXI32U LoaderFlags;
    PXI32U NumberOfRvaAndSizes;

    // Extra
    PXWindowsVersion WindowsVersion;
}
PXCOFFOptionalHeaderWindowsNT;

typedef struct PXCOFFOptionalHeaderDataDirectoriesEntry_
{
    PXI32U RelativeVirtualAddress;
    PXI32U Size;
}
PXCOFFOptionalHeaderDataDirectoriesEntry;

typedef struct PXCOFFOptionalHeaderDataDirectories_
{
    PXCOFFOptionalHeaderDataDirectoriesEntry ExportTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry ImportTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry ResourceTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry ExceptionTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry CertificateTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry BaseRelocationTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry Debug;
    PXCOFFOptionalHeaderDataDirectoriesEntry Architecture;
    PXCOFFOptionalHeaderDataDirectoriesEntry GlobalPtr;
    PXCOFFOptionalHeaderDataDirectoriesEntry ThreadLocalStorageTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry LoadConfigTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry BoundImport;
    PXCOFFOptionalHeaderDataDirectoriesEntry ImportAddressTable;
    PXCOFFOptionalHeaderDataDirectoriesEntry DelayImportDescriptor;
    PXCOFFOptionalHeaderDataDirectoriesEntry COMPlusRuntimeHeader;
}
PXCOFFOptionalHeaderDataDirectories;

typedef struct PXCOFFOptionalHeader_
{
    PXCOFFFormat Format;
    PXCOFFOptionalHeaderStandardFields StandardFields;
    PXCOFFOptionalHeaderWindowsNT WindowsNT;
    PXCOFFOptionalHeaderDataDirectories DataDirectories;
}
PXCOFFOptionalHeader;

typedef struct PXCOFF_
{
    PXCOFFHeader Header;
    PXCOFFOptionalHeader OptionalHeader;

    PXCOFFExportDirectoryTable ExportDirectoryTable;
    PXCOFFImportDirectoryTable ImportDirectoryTable;

    PXSectionTable* SectionTableList;
    PXSize SectionTableListSize;
}
PXCOFF;

PXPrivate inline PXCOFFMachineType PXAPI PXCOFFMachineFromID(const PXI16U valueID);
PXPrivate inline PXCOFFFormat PXAPI PXCOFFFormatFromID(const PXI16U valueID);

PXPublic PXResult PXAPI PXCOFFLoadFromFile(PXCOFF PXREF pxCOFF, PXFile PXREF pxFile);
PXPublic PXResult PXAPI PXCOFFSaveToFile(const PXCOFF PXREF pxCOFF, PXFile PXREF pxFile);

#endif

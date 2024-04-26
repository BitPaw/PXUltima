#ifndef PXCOFFInclude
#define PXCOFFInclude

#include "../PXResource.h"

typedef enum PXCOFFMachineType_
{
	PXCOFFMachineTypeInvalid,
	PXCOFFMachineTypeUnkown, // 0x0 Contents assumed to be applicable to any	machine type.
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

typedef enum PXSectionType_
{
	PXSectionTypeInvalid,
	PXSectionTypeAlphaArchitectureInformation,
	PXSectionTypeUninitializedData,
	PXSectionTypeInitializedData,
	PXSectionTypeExportTables,
	PXSectionTypeImportTables,
	PXSectionTypeExceptionInformationA,
	PXSectionTypeReadOnlyInitializedData,
	PXSectionTypeImageRelocations,
	PXSectionTypeResourceDirectory,
	PXSectionTypeExecutableCode,
	PXSectionTypeThreadLocalstorage,
	PXSectionTypeExceptionInformationB,
	PXSectionTypeDebug,
	PXSectionTypeDirective
}
PXSectionType;


// 40 Bytes in file
typedef struct PXSectionTable_
{
	PXInt64UCluster Name;

	union
	{
		PXInt32U PhysicalAddress;
		PXInt32U VirtualSize;
	};

	PXInt32U VirtualAddress;
	PXInt32U SectionRawDataSize;
	PXInt32U SectionRawDataAdress;
	PXInt32U PointerToRelocations;
	PXInt32U PointerToLinenumbers;
	PXInt16U NumberOfRelocations;
	PXInt16U NumberOfLinenumbers;
	PXInt32U CharacteristicFlags;

	PXSectionType Type;
}
PXSectionTable;

typedef struct PXCOFFSymbolTableEntry_
{
	union
	{
		char Name[8];

		struct
		{
			PXInt32U DoesNotUseExternalString;
			PXInt32U NameReferenceOffset;
		};

		// If the symbol name is greater than eight characters, this field is treated as two integers. The entire
		//symbol name is stored in the string table.Bytes 0 - 3 contain 0, and bytes 4 - 7 are an offset into the
		//	string table.
	};

	PXInt32U ValueOfSymbol;
	PXInt16S SectionNumber;
	PXInt16U TypeAndDerived;
	PXInt8U StorageClass;
	PXInt8U NumberOfAuxiliaryEntries;
}
PXCOFFSymbolTableEntry;

typedef struct PXRelocationInformation_
{
	PXInt32U VirtualAddressOfReference;
	PXInt32U SymbolTableIndex;
	PXInt16U RelocationType;
}
PXRelocationInformation;

typedef struct PXLineNumberEntry_
{
	union
	{
		PXInt32U SymbolIndex;
		PXInt32U PhysicalAddress;
	};
	PXInt16U LineNumber;
}
PXLineNumberEntry;

typedef struct PXCOFFExportDirectoryTable_
{
	PXInt32U ExportFlags;
	PXInt32U TimeDateStamp;
	PXInt16U MajorVersion;
	PXInt16U MinorVersion;
	PXInt32U NameRVA;
	PXInt32U OrdinalBase;
	PXInt32U AddressTableEntries;
	PXInt32U NumberOfNamePointers;
	PXInt32U ExportAddressTableRVA;
	PXInt32U NamePointerRVA;
	PXInt32U OrdinalTableRVA;
}
PXCOFFExportDirectoryTable;

typedef struct PXCOFFExportAddressTableEntry_
{
	union
	{
		PXInt32U ExportSymbolOffset;
		PXInt32U ForwarderOffset;
	};
}
PXCOFFExportAddressTableEntry;


typedef struct PXCOFFImportDirectoryTable_
{
	PXInt32U ImportLookupTableOffset;
	PXInt32U TimeDateStamp;
	PXInt32U FowarderChainOffset;
	PXInt32U NameOffset;
	PXInt32U ImportAddressTableOffset;
}
PXCOFFImportDirectoryTable;

typedef struct PXCOFFImportLookupTable_
{
	union
	{
		union
		{
			PXInt64U OrdinalNumber64;
			PXInt64U Name64;
		};
		union
		{
			PXInt32U OrdinalNumber32;
			PXInt32U Name32;
		};
	};

	PXBool IsOrdinal;
}
PXCOFFImportLookupTable;

typedef struct PXCOFFHeader_
{
	PXInt32U TimeDateStamp;
	PXInt32U PointerToSymbolTable;
	PXInt32U NumberOfSymbols;
	PXInt16U NumberOfSections;
	PXInt16U SizeOfOptionalHeader;
	PXInt16U CharacteristicsFlagList;
	PXCOFFMachineType MachineType;
}
PXCOFFHeader;


typedef struct PXCOFFOptionalHeaderStandardFields_
{
	int StateType;

	PXInt8U MajorLinkerVersion;
	PXInt8U MinorLinkerVersion;

	PXInt32U SizeOfCode;
	PXInt32U SizeOfInitializedData;
	PXInt32U SizeOfUninitializedData;
	PXInt32U AddressOfEntryPoint;
	PXInt32U BaseOfCode;
	PXInt32U BaseOfData; // Only in PE32, not used in PE32+
}
PXCOFFOptionalHeaderStandardFields;

typedef struct PXCOFFOptionalHeaderWindowsNT_
{
	PXInt64U ImageBase;
	PXInt32U SectionAlignment;
	PXInt32U FileAlignment;
	PXInt16U MajorOperatingSystemVersion;
	PXInt16U MinorOperatingSystemVersion;
	PXInt16U MajorImageVersion;
	PXInt16U MinorImageVersion;
	PXInt16U MajorSubsystemVersion;
	PXInt16U MinorSubsystemVersion;
	PXInt32U SizeOfImage;
	PXInt32U SizeOfHeaders;
	PXInt32U CheckSum;
	PXInt16U Subsystem;
	PXInt16U DLLCharacteristics;
	PXInt64U SizeOfStackReserve;
	PXInt64U SizeOfStackCommit;
	PXInt64U SizeOfHeapReserve;
	PXInt64U SizeOfHeapCommit;
	PXInt32U LoaderFlags;
	PXInt32U NumberOfRvaAndSizes;

	// Extra
	PXWindowsVersion WindowsVersion;
}
PXCOFFOptionalHeaderWindowsNT;

typedef struct PXCOFFOptionalHeaderDataDirectoriesEntry_
{
	PXInt32U RelativeVirtualAddress;
	PXInt32U Size;
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

PXPrivate inline PXCOFFMachineType PXAPI PXCOFFMachineFromID(const PXInt16U valueID);
PXPrivate inline PXCOFFFormat PXAPI PXCOFFFormatFromID(const PXInt16U valueID);
PXPrivate inline PXSectionType PXAPI PXSectionTypeFromID(const PXInt64U valueID);

PXPublic PXActionResult PXAPI PXCOFFLoadFromFile(PXCOFF* const pxCOFF, PXFile* const pxFile);
PXPublic PXActionResult PXAPI PXCOFFSaveToFile(const PXCOFF* const pxCOFF, PXFile* const pxFile);

#endif
#ifndef PXCOFFInclude
#define PXCOFFInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

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
		PXInt32U VirtualSize;
		PXInt32U VirtualAddress;
		PXInt32U SizeOfRawData;
		PXInt32U PointerToRawData;
		PXInt32U PointerToRelocations;
		PXInt32U PointerToLinenumbers;
		PXInt16U NumberOfRelocations;
		PXInt16U NumberOfLinenumbers;
		PXInt32U Characteristics;

		PXSectionType Type;
	}
	PXSectionTable;

	// 20 Byte size
	typedef struct PXCOFF_
	{
		PXInt32U TimeDateStamp;
		PXInt32U PointerToSymbolTable;
		PXInt32U NumberOfSymbols;	
		PXInt16U NumberOfSections;
		PXInt16U SizeOfOptionalHeader;
		PXInt16U CharacteristicsFlagList;
		PXCOFFMachineType MachineType;

		// Optional Header 

		PXCOFFFormat Format;

		int StateType;

		PXInt8U MajorLinkerVersion;
		PXInt8U MinorLinkerVersion;

		PXInt32U SizeOfCode;
		PXInt32U SizeOfInitializedData;
		PXInt32U SizeOfUninitializedData;
		PXInt32U AddressOfEntryPoint;
		PXInt32U BaseOfCode;
		PXInt32U BaseOfData; // Only in PE32, not used in PE32+


		// Windows spesific
		PXInt64U ImageBase;
		PXInt32U SectionAlignment;
		PXInt32U FileAlignment;
		PXInt16U MajorOperatingSystemVersion;
		PXInt16U MinorOperatingSystemVersion;
		PXInt16U MajorImageVersion;
		PXInt16U MinorImageVersion;
		PXInt16U MajorSubsystemVersion;
		PXInt16U MinorSubsystemVersion;
		char ReservedA[4];
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


		PXInt32U ExportTableAdress;
		PXInt32U ExportTableSize;

		PXInt32U ImportTableAdress;
		PXInt32U ImportTableSize;

		PXInt32U ResourceTableAdress;
		PXInt32U ResourceTableSize;

		PXInt32U ExceptionTableAdress;
		PXInt32U ExceptionTableSize;

		PXInt32U CertificateTableAdress;
		PXInt32U CertificateTableSize;
		
		PXInt32U BaseRelocationTableAdress;
		PXInt32U BaseRelocationTableSize;

		PXInt32U DebugAdress;
		PXInt32U DebugSize;

		PXInt32U ArchitectureAdress;
		PXInt32U ArchitectureSize;

		PXInt32U GlobalPtrAdress;
		PXInt32U GlobalPtrSize;

		PXInt32U ThreadLocalStorageTableAdress;
		PXInt32U ThreadLocalStorageTableSize;

		PXInt32U LoadConfigTableAdress;
		PXInt32U LoadConfigTableSize;

		PXInt32U BoundImportAdress;
		PXInt32U BoundImportSize;

		PXInt32U ImportAddressTableAdress;
		PXInt32U ImportAddressTableSize;

		PXInt32U DelayImportDescriptorAdress;
		PXInt32U DelayImportDescriptorSize;

		PXInt32U COMPlusRuntimeHeaderAdress;
		PXInt32U COMPlusRuntimeHeaderSize;

		PXByte ReservedB[8];


		PXSectionTable* SectionTableList;
	}
	PXCOFF;

	PXPrivate inline PXCOFFMachineType PXCOFFMachineFromID(const PXInt16U valueID);
	PXPrivate inline PXCOFFFormat PXCOFFFormatFromID(const PXInt16U valueID);
	PXPrivate inline PXSectionType PXSectionTypeFromID(const PXInt64U valueID);

	PXPublic PXActionResult PXCOFFLoadFromFile(PXCOFF* const pxCOFF, PXFile* const pxFile);
	PXPublic PXActionResult PXCOFFSaveToFile(const PXCOFF* const pxCOFF, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
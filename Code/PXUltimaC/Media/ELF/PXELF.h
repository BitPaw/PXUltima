#ifndef PXELFINCLUDE
#define PXELFINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXELFTargetOSAPI_
	{
		PXELFTargetOSAPIInvalid,

		PXELFTargetOSAPISystemV,
		PXELFTargetOSAPIHPUX,
		PXELFTargetOSAPINetBSD,
		PXELFTargetOSAPILinux,
		PXELFTargetOSAPIGNUHurd,
		PXELFTargetOSAPISolaris,
		PXELFTargetOSAPIAIXMonterey,
		PXELFTargetOSAPIIRIX,
		PXELFTargetOSAPIFreeBSD,
		PXELFTargetOSAPITru64,
		PXELFTargetOSAPINovellModesto,
		PXELFTargetOSAPIOpenBSD,
		PXELFTargetOSAPIOpenVMS,
		PXELFTargetOSAPINonStopKernel,
		PXELFTargetOSAPIAROS,
		PXELFTargetOSAPIFenixOS,
		PXELFTargetOSAPINuxiCloudABI,
		PXELFTargetOSAPIStratusTechnologiesOpenVOS
	}
	PXELFTargetOSAPI;

	typedef enum PXELFType_
	{
		PXELFTypeInvalid,
		PXELFTypeUnknown,
		PXELFTypeRelocatableFile,
		PXELFTypeExecutableFile,
		PXELFTypeDynamicSharedObject,
		PXELFTypeCoreFile,
		PXELFTypeET_LOOS,
		PXELFTypeET_HIOS,
		PXELFTypeET_LOPROC
	}
	PXELFType;

	typedef enum PXELFMachine_
	{
		PXELFMachineInvalid,
		PXELFMachineNoSpecificInstructionSet,
		PXELFMachineATandTWE32100,
		PXELFMachineSPARC,
		PXELFMachinex86,
		PXELFMachineMotorola68000M68k,
		PXELFMachineMotorola88000M88k,
		PXELFMachineIntelMCU,
		PXELFMachineIntel80860,
		PXELFMachineMIPS,
		PXELFMachineIBMSystem370,
		PXELFMachineMIPSRS3000LittleEndian,
		PXELFMachineReservedForFutureUse,
		PXELFMachineHewlettPackardPARISC,
		PXELFMachineIntel80960,
		PXELFMachinePowerPC,
		PXELFMachinePowerPC64Bit,
		PXELFMachineS390IncludingS390x,
		PXELFMachineIBMSPUSPC,
		PXELFMachineNECV800,
		PXELFMachineFujitsuFR20,
		PXELFMachineTRWRH32,
		PXELFMachineMotorolaRCE,
		PXELFMachineArmUpToArmv7AArch32,
		PXELFMachineDigitalAlpha,
		PXELFMachineSuperH,
		PXELFMachineSPARCVersion9,
		PXELFMachineSiemensTriCoreEmbeddedProcessor,
		PXELFMachineArgonautRISCCore,
		PXELFMachineHitachiH8300,
		PXELFMachineHitachiH8300H,
		PXELFMachineHitachiH8S,
		PXELFMachineHitachiH8500,
		PXELFMachineIA64,
		PXELFMachineStanfordMIPSX,
		PXELFMachineMotorolaColdFire,
		PXELFMachineMotorolaM68HC12,
		PXELFMachineFujitsuMMAMultimediaAccelerator,
		PXELFMachineSiemensPCP,
		PXELFMachineSonyNCPUEmbeddedRISCProcessor,
		PXELFMachineDensoNDR1Microprocessor,
		PXELFMachineMotorolaStarCoreProcessor,
		PXELFMachineToyotaME16Processor,
		PXELFMachineSTMicroelectronicsST100Processor,
		PXELFMachineAdvancedLogicCorpTinyJEmbeddedProcessorFamily,
		PXELFMachineAMDx86and64,
		PXELFMachineSonyDSPProcessor,
		PXELFMachineDigitalEquipmentCorpPDP10,
		PXELFMachineDigitalEquipmentCorpPDP11,
		PXELFMachineSiemensFX66Microcontroller,
		PXELFMachineSTMicroelectronicsST98and16BitMicrocontroller,
		PXELFMachineSTMicroelectronicsST78BitMicrocontroller,
		PXELFMachineMotorolaMC68HC16Microcontroller,
		PXELFMachineMotorolaMC68HC11Microcontroller,
		PXELFMachineMotorolaMC68HC08Microcontroller,
		PXELFMachineMotorolaMC68HC05Microcontroller,
		PXELFMachineSiliconGraphicsSVx,
		PXELFMachineSTMicroelectronicsST198BitMicrocontroller,
		PXELFMachineDigitalVAX,
		PXELFMachineAxisCommunications32BitEmbeddedProcessor,
		PXELFMachineInfineonTechnologies32BitEmbeddedProcessor,
		PXELFMachineElement1464BitDSPProcessor,
		PXELFMachineLSILogic16BitDSPProcessor,
		PXELFMachineTMS320C6000Family,
		PXELFMachineMCSTElbrusE2k,
		PXELFMachineArm64BitsArmv8AArch64,
		PXELFMachineZilogZ80,
		PXELFMachineRISCV,
		PXELFMachineBerkeleyPacketFilter,
		PXELFMachineWDC65C816,
	}
	PXELFMachine;

	typedef enum PXELFSegmentType_
	{
		PXELFSegmentTypeInvalid,

		PXELFSegmentTypeUnused,
		PXELFSegmentTypeLoadable,
		PXELFSegmentTypeDynamicLinkingInformation,
		PXELFSegmentTypeInterpreterInformation,
		PXELFSegmentTypeAuxiliaryInformation,
		PXELFSegmentTypePT_SHLIB,
		PXELFSegmentTypeProgramHeaderTable,
		PXELFSegmentTypeThreadLocalStorage,
		PXELFSegmentTypePT_LOOS 	,
		PXELFSegmentTypePT_HIOS,
		PXELFSegmentTypePT_LOPROC ,	
		PXELFSegmentTypePT_HIPROC
	}
	PXELFSegmentType;

	typedef struct PXELFHeader_
	{
		PXBitFormat BitFormat;
		PXEndian Endian;

		PXELFMachine Machine;
		PXELFType Type;

		PXELFTargetOSAPI TargetOSAPI;
		PXInt8U TargetOSAPIVersion;


		// Offset
		PXAdress64 EntryPointOfCode;

		PXAdress64 ProgrammHeaderOffset;
		PXInt16U ProgrammHeaderSize;
		PXInt16U ProgrammHeaderAmount;

		PXAdress64 SectionHeaderOffset;
		PXInt16U SectionHeaderSize;
		PXInt16U SectionHeaderAmount;

	}
	PXELFHeader;

	typedef struct PXSectionHeader_
	{
		PXInt32U sh_nameOffset;
		PXInt32U sh_type;
		PXInt64U sh_flags;
		PXInt64U sh_addr;
		PXInt64U sh_offset;
		PXInt64U sh_size;
		PXInt32U sh_link;
		PXInt32U sh_info;
		PXInt64U sh_addralign;
		PXInt64U sh_entsize;
	}
	PXSectionHeader;

	typedef struct PXELF
	{
		PXELFHeader Header;
	}
	PXELF;

	PXPrivate inline PXEndian PXELFEndianessFromID(const PXInt8U value);
	PXPrivate inline PXBitFormat PXELFBitFormatFromID(const PXInt8U value);
	PXPrivate inline PXELFTargetOSAPI PXPXELFTargetOSAPIFromID(const PXInt8U value);
	PXPrivate inline PXELFMachine PXELFMachineFromID(const PXInt8U value);
	PXPrivate inline PXELFType PXELFTypeFromID(const PXInt8U value);

	PXPublic PXActionResult PXELFLoadFromFile(PXELF* const pxELF, PXFile* const pxFile);
	PXPublic PXActionResult PXELFSaveToFile(PXELF* const pxELF, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
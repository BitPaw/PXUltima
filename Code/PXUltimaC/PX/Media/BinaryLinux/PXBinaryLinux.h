#pragma once

#ifndef PXELFIncluded
#define PXELFIncluded

#include <PX/Engine/PXResource.h>

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
    PXELFSegmentTypePT_LOOS,
    PXELFSegmentTypePT_HIOS,
    PXELFSegmentTypePT_LOPROC,
    PXELFSegmentTypePT_HIPROC
}
PXELFSegmentType;

typedef struct PXELFProgramHeader_
{
    PXAdress64 AdressOffsetRead;
    PXAdress64 AdressTargetVirtual;
    PXAdress64 AdressTargetPhysical;
    PXAdress64 SizeOnFile;
    PXAdress64 SizeInMemory;
    PXAdress64 p_align;

    PXBool IsSegmentExecutable;
    PXBool IsSegmentWriteable;
    PXBool IsSegmentReadable;

    PXELFSegmentType Type;
}
PXELFProgramHeader;

typedef struct PXELFHeader_
{
    PXBitFormat BitFormat;
    PXEndian Endian;

    PXELFMachine Machine;
    PXELFType Type;

    PXELFTargetOSAPI TargetOSAPI;
    PXI8U TargetOSAPIVersion;


    // Offset
    PXAdress64 EntryPointOfCode;

    PXAdress64 ProgrammHeaderOffset;
    PXI16U ProgrammHeaderSize;
    PXI16U ProgrammHeaderAmount;

    PXAdress64 SectionHeaderOffset;
    PXI16U SectionHeaderSize;
    PXI16U SectionHeaderAmount;

}
PXELFHeader;

typedef struct PXSectionHeader_
{
    PXI32U sh_nameOffset;
    PXI32U sh_type;
    PXI64U sh_flags;
    PXI64U sh_addr;
    PXI64U FileImageOffset;
    PXI64U FileImageSize;
    PXI32U sh_link;
    PXI32U sh_info;
    PXI64U sh_addralign;
    PXI64U sh_entsize;
}
PXSectionHeader;

typedef struct PXBinaryLinux_
{
    PXELFHeader Header;
}
PXBinaryLinux;

PXPrivate inline PXEndian PXAPI PXELFEndianessFromID(const PXI8U value);
PXPrivate inline PXBitFormat PXAPI PXELFBitFormatFromID(const PXI8U value);
PXPrivate inline PXELFTargetOSAPI PXAPI PXPXELFTargetOSAPIFromID(const PXI8U value);
PXPrivate inline PXELFMachine PXAPI PXELFMachineFromID(const PXI8U value);
PXPrivate inline PXELFType PXAPI PXELFTypeFromID(const PXI8U value);
PXPrivate inline PXELFSegmentType PXAPI PXELFSegmentTypeFromID(const PXI32U value);

PXPublic PXResult PXAPI PXBinaryLinuxLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXBinaryLinuxSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

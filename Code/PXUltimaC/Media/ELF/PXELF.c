#include "PXELF.h"

const char PXELFSignature[4] = { 0x7F, 'E', 'L' ,'F' };

#define PXELFMachineInvalidID		 -1
#define PXELFMachineNoSpecificInstructionSetID		 0x00
#define PXELFMachineATandTWE32100ID		 0x01
#define PXELFMachineSPARCID		 0x02
#define PXELFMachinex86ID		 0x03
#define PXELFMachineMotorola68000M68kID		 0x04
#define PXELFMachineMotorola88000M88kID		 0x05
#define PXELFMachineIntelMCUID		 0x06
#define PXELFMachineIntel80860ID		 0x07
#define PXELFMachineMIPSID		 0x08
#define PXELFMachineIBMSystem370ID		 0x09
#define PXELFMachineMIPSRS3000LittleEndianID		 0x0A

#define PXELFMachineHewlettPackardPARISCID		 0x0F
#define PXELFMachineIntel80960ID		 0x13
#define PXELFMachinePowerPCID		 0x14
#define PXELFMachinePowerPC64BitID		 0x15
#define PXELFMachineS390IncludingS390xID		 0x16
#define PXELFMachineIBMSPUSPCID		 0x17
#define PXELFMachineNECV800ID		 0x24
#define PXELFMachineFujitsuFR20ID		 0x25
#define PXELFMachineTRWRH32ID		 0x26
#define PXELFMachineMotorolaRCEID		 0x27
#define PXELFMachineArmUpToArmv7AArch32ID		 0x28
#define PXELFMachineDigitalAlphaID		 0x29
#define PXELFMachineSuperHID		 0x2A
#define PXELFMachineSPARCVersion9ID		 0x2B
#define PXELFMachineSiemensTriCoreEmbeddedProcessorID		 0x2C
#define PXELFMachineArgonautRISCCoreID		 0x2D
#define PXELFMachineHitachiH8300ID		 0x2E
#define PXELFMachineHitachiH8300HID		 0x2F
#define PXELFMachineHitachiH8SID		 0x30
#define PXELFMachineHitachiH8500ID		 0x31
#define PXELFMachineIA64ID		 0x32
#define PXELFMachineStanfordMIPSXID		 0x33
#define PXELFMachineMotorolaColdFireID		 0x34
#define PXELFMachineMotorolaM68HC12ID		 0x35
#define PXELFMachineFujitsuMMAMultimediaAcceleratorID		 0x36
#define PXELFMachineSiemensPCPID		 0x37
#define PXELFMachineSonyNCPUEmbeddedRISCProcessorID		 0x38
#define PXELFMachineDensoNDR1MicroprocessorID		 0x39
#define PXELFMachineMotorolaStarCoreProcessorID		 0x3A
#define PXELFMachineToyotaME16ProcessorID		 0x3B
#define PXELFMachineSTMicroelectronicsST100ProcessorID		 0x3C
#define PXELFMachineAdvancedLogicCorpTinyJEmbeddedProcessorFamilyID		 0x3D
#define PXELFMachineAMDx86and64ID		 0x3E
#define PXELFMachineSonyDSPProcessorID		 0x3F
#define PXELFMachineDigitalEquipmentCorpPDP10ID		 0x40
#define PXELFMachineDigitalEquipmentCorpPDP11ID		 0x41
#define PXELFMachineSiemensFX66MicrocontrollerID		 0x42
#define PXELFMachineSTMicroelectronicsST98and16BitMicrocontrollerID		 0x43
#define PXELFMachineSTMicroelectronicsST78BitMicrocontrollerID		 0x44
#define PXELFMachineMotorolaMC68HC16MicrocontrollerID		 0x45
#define PXELFMachineMotorolaMC68HC11MicrocontrollerID		 0x46
#define PXELFMachineMotorolaMC68HC08MicrocontrollerID		 0x47
#define PXELFMachineMotorolaMC68HC05MicrocontrollerID		 0x48
#define PXELFMachineSiliconGraphicsSVxID		 0x49
#define PXELFMachineSTMicroelectronicsST198BitMicrocontrollerID		 0x4A
#define PXELFMachineDigitalVAXID		 0x4B
#define PXELFMachineAxisCommunications32BitEmbeddedProcessorID		 0x4C
#define PXELFMachineInfineonTechnologies32BitEmbeddedProcessorID	0x4D
#define PXELFMachineElement1464BitDSPProcessorID					0x4E
#define PXELFMachineLSILogic16BitDSPProcessorID					0x4F
#define PXELFMachineTMS320C6000FamilyID							0x8C
#define PXELFMachineMCSTElbrusE2kID								0xAF
#define PXELFMachineArm64BitsArmv8AArch64ID				0xB7
#define PXELFMachineZilogZ80ID							0xDC
#define PXELFMachineRISCVID								0xF3
#define PXELFMachineBerkeleyPacketFilterID				0xF7
#define PXELFMachineWDC65C816ID					0x101

PXEndian PXELFEndianessFromID(const PXInt8U value)
{
	switch (value)
	{
		case 1u: return PXEndianLittle;
		case 2u: return PXEndianBig;

		default:
			return PXEndianInvalid;
	}
}

PXBitFormat PXELFBitFormatFromID(const PXInt8U value)
{
	switch (value)
	{
		case 1u: return PXBitFormat32;
		case 2u: return PXBitFormat64;

		default:
			return PXBitFormatInvalid;
	}
}

PXELFTargetOSAPI PXPXELFTargetOSAPIFromID(const PXInt8U value)
{
	switch (value)
	{
		case 0x00: return PXELFTargetOSAPISystemV;
		case 0x01: return PXELFTargetOSAPIHPUX;
		case 0x02: return PXELFTargetOSAPINetBSD;
		case 0x03: return PXELFTargetOSAPILinux;
		case 0x04: return PXELFTargetOSAPIGNUHurd;
		case 0x06: return PXELFTargetOSAPISolaris;
		case 0x07: return PXELFTargetOSAPIAIXMonterey;
		case 0x08: return PXELFTargetOSAPIIRIX;
		case 0x09: return PXELFTargetOSAPIFreeBSD;
		case 0x0A: return PXELFTargetOSAPITru64;
		case 0x0B: return PXELFTargetOSAPINovellModesto;
		case 0x0C: return PXELFTargetOSAPIOpenBSD;
		case 0x0D: return PXELFTargetOSAPIOpenVMS;
		case 0x0E: return PXELFTargetOSAPINonStopKernel;
		case 0x0F: return PXELFTargetOSAPIAROS;
		case 0x10: return PXELFTargetOSAPIFenixOS;
		case 0x11: return PXELFTargetOSAPINuxiCloudABI;
		case 0x12: return PXELFTargetOSAPIStratusTechnologiesOpenVOS;

		default:
			return PXELFTargetOSAPIInvalid;
	}
}

PXELFMachine PXELFMachineFromID(const PXInt8U value)
{
	switch (value)
	{
		case 0x0B:
		case 0x0C:
		case 0x0D:
		case 0x0E:

		case 0x18:
		case 0x19:
		case 0x1A:
		case 0x1B:
		case 0x1C:
		case 0x1D:
		case 0x1E:
		case 0x1F:
		case 0x20:
		case 0x21:
		case 0x22:
		case 0x23:

			return PXELFMachineReservedForFutureUse;


		case PXELFMachineNoSpecificInstructionSetID: return PXELFMachineNoSpecificInstructionSet;
		case PXELFMachineATandTWE32100ID: return PXELFMachineATandTWE32100;
		case PXELFMachineSPARCID: return PXELFMachineSPARC;
		case PXELFMachinex86ID: return PXELFMachinex86;
		case PXELFMachineMotorola68000M68kID: return PXELFMachineMotorola68000M68k;
		case PXELFMachineMotorola88000M88kID: return PXELFMachineMotorola88000M88k;
		case PXELFMachineIntelMCUID: return PXELFMachineIntelMCU;
		case PXELFMachineIntel80860ID: return PXELFMachineIntel80860;
		case PXELFMachineMIPSID: return PXELFMachineMIPS;
		case PXELFMachineIBMSystem370ID: return PXELFMachineIBMSystem370;
		case PXELFMachineMIPSRS3000LittleEndianID: return PXELFMachineMIPSRS3000LittleEndian;
		case PXELFMachineHewlettPackardPARISCID: return PXELFMachineHewlettPackardPARISC;
		case PXELFMachineIntel80960ID: return PXELFMachineIntel80960;
		case PXELFMachinePowerPCID: return PXELFMachinePowerPC;
		case PXELFMachinePowerPC64BitID: return PXELFMachinePowerPC64Bit;
		case PXELFMachineS390IncludingS390xID: return PXELFMachineS390IncludingS390x;
		case PXELFMachineIBMSPUSPCID: return PXELFMachineIBMSPUSPC;
		case PXELFMachineNECV800ID: return PXELFMachineNECV800;
		case PXELFMachineFujitsuFR20ID: return PXELFMachineFujitsuFR20;
		case PXELFMachineTRWRH32ID: return PXELFMachineTRWRH32;
		case PXELFMachineMotorolaRCEID: return PXELFMachineMotorolaRCE;
		case PXELFMachineArmUpToArmv7AArch32ID: return PXELFMachineArmUpToArmv7AArch32;
		case PXELFMachineDigitalAlphaID: return PXELFMachineDigitalAlpha;
		case PXELFMachineSuperHID: return PXELFMachineSuperH;
		case PXELFMachineSPARCVersion9ID: return PXELFMachineSPARCVersion9;
		case PXELFMachineSiemensTriCoreEmbeddedProcessorID: return PXELFMachineSiemensTriCoreEmbeddedProcessor;
		case PXELFMachineArgonautRISCCoreID: return PXELFMachineArgonautRISCCore;
		case PXELFMachineHitachiH8300ID: return PXELFMachineHitachiH8300;
		case PXELFMachineHitachiH8300HID: return PXELFMachineHitachiH8300H;
		case PXELFMachineHitachiH8SID: return PXELFMachineHitachiH8S;
		case PXELFMachineHitachiH8500ID: return PXELFMachineHitachiH8500;
		case PXELFMachineIA64ID: return PXELFMachineIA64;
		case PXELFMachineStanfordMIPSXID: return PXELFMachineStanfordMIPSX;
		case PXELFMachineMotorolaColdFireID: return PXELFMachineMotorolaColdFire;
		case PXELFMachineMotorolaM68HC12ID: return PXELFMachineMotorolaM68HC12;
		case PXELFMachineFujitsuMMAMultimediaAcceleratorID: return PXELFMachineFujitsuMMAMultimediaAccelerator;
		case PXELFMachineSiemensPCPID: return PXELFMachineSiemensPCP;
		case PXELFMachineSonyNCPUEmbeddedRISCProcessorID: return PXELFMachineSonyNCPUEmbeddedRISCProcessor;
		case PXELFMachineDensoNDR1MicroprocessorID: return PXELFMachineDensoNDR1Microprocessor;
		case PXELFMachineMotorolaStarCoreProcessorID: return PXELFMachineMotorolaStarCoreProcessor;
		case PXELFMachineToyotaME16ProcessorID: return PXELFMachineToyotaME16Processor;
		case PXELFMachineSTMicroelectronicsST100ProcessorID: return PXELFMachineSTMicroelectronicsST100Processor;
		case PXELFMachineAdvancedLogicCorpTinyJEmbeddedProcessorFamilyID: return PXELFMachineAdvancedLogicCorpTinyJEmbeddedProcessorFamily;
		case PXELFMachineAMDx86and64ID: return PXELFMachineAMDx86and64;
		case PXELFMachineSonyDSPProcessorID: return PXELFMachineSonyDSPProcessor;
		case PXELFMachineDigitalEquipmentCorpPDP10ID: return PXELFMachineDigitalEquipmentCorpPDP10;
		case PXELFMachineDigitalEquipmentCorpPDP11ID: return PXELFMachineDigitalEquipmentCorpPDP11;
		case PXELFMachineSiemensFX66MicrocontrollerID: return PXELFMachineSiemensFX66Microcontroller;
		case PXELFMachineSTMicroelectronicsST98and16BitMicrocontrollerID: return PXELFMachineSTMicroelectronicsST98and16BitMicrocontroller;
		case PXELFMachineSTMicroelectronicsST78BitMicrocontrollerID: return PXELFMachineSTMicroelectronicsST78BitMicrocontroller;
		case PXELFMachineMotorolaMC68HC16MicrocontrollerID: return PXELFMachineMotorolaMC68HC16Microcontroller;
		case PXELFMachineMotorolaMC68HC11MicrocontrollerID: return PXELFMachineMotorolaMC68HC11Microcontroller;
		case PXELFMachineMotorolaMC68HC08MicrocontrollerID: return PXELFMachineMotorolaMC68HC08Microcontroller;
		case PXELFMachineMotorolaMC68HC05MicrocontrollerID: return PXELFMachineMotorolaMC68HC05Microcontroller;
		case PXELFMachineSiliconGraphicsSVxID: return PXELFMachineSiliconGraphicsSVx;
		case PXELFMachineSTMicroelectronicsST198BitMicrocontrollerID: return PXELFMachineSTMicroelectronicsST198BitMicrocontroller;
		case PXELFMachineDigitalVAXID: return PXELFMachineDigitalVAX;
		case PXELFMachineAxisCommunications32BitEmbeddedProcessorID: return PXELFMachineAxisCommunications32BitEmbeddedProcessor;
		case PXELFMachineInfineonTechnologies32BitEmbeddedProcessorID: return PXELFMachineInfineonTechnologies32BitEmbeddedProcessor;
		case PXELFMachineElement1464BitDSPProcessorID: return PXELFMachineElement1464BitDSPProcessor;
		case PXELFMachineLSILogic16BitDSPProcessorID: return PXELFMachineLSILogic16BitDSPProcessor;
		case PXELFMachineTMS320C6000FamilyID: return PXELFMachineTMS320C6000Family;
		case PXELFMachineMCSTElbrusE2kID: return PXELFMachineMCSTElbrusE2k;
		case PXELFMachineArm64BitsArmv8AArch64ID: return PXELFMachineArm64BitsArmv8AArch64;
		case PXELFMachineZilogZ80ID: return PXELFMachineZilogZ80;
		case PXELFMachineRISCVID: return PXELFMachineRISCV;
		case PXELFMachineBerkeleyPacketFilterID: return PXELFMachineBerkeleyPacketFilter;
		case PXELFMachineWDC65C816ID: return PXELFMachineWDC65C816;

		default:
			return PXELFMachineInvalid;
	}
}

PXELFType PXELFTypeFromID(const PXInt8U value)
{
	return PXELFTypeInvalid;
}

PXActionResult PXELFLoadFromFile(PXELF* const pxELF, PXFile* const pxFile)
{
	PXClear(PXELF, pxELF);

    {
        const PXBool isValidSignature = PXFileReadAndCompare(pxFile, PXELFSignature, sizeof(PXELFSignature));

        if (!isValidSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

	// Read Header
	{
		// A
		{
			PXInt8U classID;
			PXInt8U dataID;
			PXInt8U versionID; // Expect to be = 1
			PXInt8U osAPIID;		

			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{PXDataTypeInt8U, &classID},
				{PXDataTypeInt8U, &dataID},
				{PXDataTypeInt8U, &versionID},
				{PXDataTypeInt8U, &osAPIID},
				{PXDataTypeInt8U, &pxELF->Header.TargetOSAPIVersion},
				{PXDataTypePadding, (void*)7u}
			};
			const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

			PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

			pxELF->Header.BitFormat = PXELFBitFormatFromID(classID);
			pxELF->Header.Endian = PXELFEndianessFromID(dataID);
			pxELF->Header.TargetOSAPI = PXPXELFTargetOSAPIFromID(osAPIID);

			pxFile->BitFormatOfData = pxELF->Header.BitFormat;
			pxFile->EndiannessOfData = pxELF->Header.Endian;
		}

		// B
		{
			PXInt16U typeID;
			PXInt16U machineID;
			PXInt32U versionID; // Extecped to be = 1
			PXInt32U e_flags;
			PXInt16U sizeOfThisHeader;
			PXInt16U shstrndx;

			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{PXDataTypeInt16Flex, &typeID},
				{PXDataTypeInt16Flex, &machineID},
				{PXDataTypeInt32Flex, &versionID},
				{PXDataTypeAdressFlex, &pxELF->Header.EntryPointOfCode},
				{PXDataTypeAdressFlex, &pxELF->Header.ProgrammHeaderOffset},
				{PXDataTypeAdressFlex, &pxELF->Header.SectionHeaderOffset},
				{PXDataTypeInt32Flex, &e_flags},
				{PXDataTypeInt16Flex, &sizeOfThisHeader},
				{PXDataTypeInt16Flex, &pxELF->Header.ProgrammHeaderSize},
				{PXDataTypeInt16Flex, &pxELF->Header.ProgrammHeaderAmount},
				{PXDataTypeInt16Flex, &pxELF->Header.SectionHeaderSize},
				{PXDataTypeInt16Flex, &pxELF->Header.SectionHeaderAmount},
				{PXDataTypeInt16Flex, &shstrndx}
			};
			const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

			PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

			pxELF->Header.Machine = PXELFMachineFromID(machineID);
			pxELF->Header.Type = PXELFTypeFromID(typeID);
		}
	}


	//--------------------------------------------------------
	// Program header - Read
	//--------------------------------------------------------
	{
		PXFileCursorMoveTo(pxFile, pxELF->Header.ProgrammHeaderOffset);

		for (PXInt16U programHeaderID = 0; programHeaderID < pxELF->Header.ProgrammHeaderAmount; programHeaderID++)
		{

			// 4

			// 4 64Only

			// 5-sysInt

			// 4


			// 1xSysint
		}
	}

	//--------------------------------------------------------
	// Section header
	//--------------------------------------------------------
	{
		PXFileCursorMoveTo(pxFile, pxELF->Header.SectionHeaderOffset);

		for (PXInt16U i = 0; i < pxELF->Header.SectionHeaderAmount; i++)
		{
			PXSectionHeader pxSectionHeader;

			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{PXDataTypeInt32Flex, &pxSectionHeader.sh_nameOffset},
				{PXDataTypeInt32Flex, &pxSectionHeader.sh_type},
				{PXDataTypeAdressFlex, &pxSectionHeader.sh_flags},
				{PXDataTypeAdressFlex, &pxSectionHeader.sh_addr},
				{PXDataTypeAdressFlex, &pxSectionHeader.sh_offset},
				{PXDataTypeAdressFlex, &pxSectionHeader.sh_size},
				{PXDataTypeInt32Flex, &pxSectionHeader.sh_link},
				{PXDataTypeInt32Flex, &pxSectionHeader.sh_info},
				{PXDataTypeAdressFlex, &pxSectionHeader.sh_addralign},
				{PXDataTypeAdressFlex, &pxSectionHeader.sh_entsize}
			};
			const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

			PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);
		}	
	}


    return PXActionRefusedNotImplemented;
}

PXActionResult PXELFSaveToFile(PXELF* const pxELF, PXFile* const pxFile)
{
	PXFileWriteB(pxFile, PXELFSignature, sizeof(PXELFSignature));

    return PXActionRefusedNotImplemented;
}
#include "PXBinaryLinux.h"

#define PXEFIDebug 0

const char PXELFSignature[4] = { 0x7F, 'E', 'L' ,'F' };

#define PT_NULL 0x00000000
#define PT_LOAD 0x00000001
#define PT_DYNAMIC 0x00000002
#define PT_INTERP 0x00000003
#define PT_NOTE 0x00000004
#define PT_SHLIB 0x00000005
#define PT_PHDR 0x00000006
#define PT_TLS 0x00000007
#define PT_LOOS 0x60000000
#define PT_HIOS 0x6FFFFFFF
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7FFFFFFF

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

PXEndian PXAPI PXELFEndianessFromID(const PXInt8U value)
{
	switch (value)
	{
		case 1u: return PXEndianLittle;
		case 2u: return PXEndianBig;

		default:
			return PXEndianInvalid;
	}
}

PXBitFormat PXAPI PXELFBitFormatFromID(const PXInt8U value)
{
	switch (value)
	{
		case 1u: return PXBitFormat32;
		case 2u: return PXBitFormat64;

		default:
			return PXBitFormatInvalid;
	}
}

PXELFTargetOSAPI PXAPI PXPXELFTargetOSAPIFromID(const PXInt8U value)
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

PXELFMachine PXAPI PXELFMachineFromID(const PXInt8U value)
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

PXELFType PXAPI PXELFTypeFromID(const PXInt8U value)
{
	return PXELFTypeInvalid;
}

PXELFSegmentType PXAPI PXELFSegmentTypeFromID(const PXInt32U value)
{
	switch (value)
	{
		case PT_NULL: return PXELFSegmentTypeUnused;
		case PT_LOAD: return PXELFSegmentTypeLoadable;
		case PT_DYNAMIC: return PXELFSegmentTypeDynamicLinkingInformation;
		case PT_INTERP: return PXELFSegmentTypeInterpreterInformation;
		case PT_NOTE: return PXELFSegmentTypeAuxiliaryInformation;
		case PT_SHLIB: return PXELFSegmentTypePT_SHLIB;
		case PT_PHDR: return PXELFSegmentTypeProgramHeaderTable;
		case PT_TLS: return PXELFSegmentTypeThreadLocalStorage;
		case PT_LOOS: return PXELFSegmentTypePT_LOOS;
		case PT_HIOS: return PXELFSegmentTypePT_HIOS;
		case PT_LOPROC: return PXELFSegmentTypePT_LOPROC;
		case PT_HIPROC: return PXELFSegmentTypePT_HIPROC;

		default:
			return PXELFSegmentTypeInvalid;
	}
}

PXActionResult PXAPI PXBinaryLinuxLoadFromFile(PXBinaryLinux* const pxBinaryLinux, PXFile* const pxFile)
{
	PXClear(PXBinaryLinux, pxBinaryLinux);

	// Read Header
	{
		// A
		{
			PXInt32UCluster signature;
			PXInt8U classID;
			PXInt8U dataID;
			PXInt8U versionID; // Expect to be = 1
			PXInt8U osAPIID;		

			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{signature.Data, PXDataTypeDatax4},
				{&classID,PXDataTypeInt08U},
				{&dataID,PXDataTypeInt08U},
				{&versionID,PXDataTypeInt08U},
				{&osAPIID,PXDataTypeInt08U},
				{&pxBinaryLinux->Header.TargetOSAPIVersion,PXDataTypeInt08U},
				{PXNull, PXDataTypePadding(7u})
			};

			PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

			{
				const PXBool isValidSignature = PXMemoryCompare(signature.Data, 4u, PXELFSignature, sizeof(PXELFSignature));

				if (!isValidSignature)
				{
					return PXActionRefusedInvalidHeaderSignature;
				}
			}		

			pxBinaryLinux->Header.BitFormat = PXELFBitFormatFromID(classID);
			pxBinaryLinux->Header.Endian = PXELFEndianessFromID(dataID);
			pxBinaryLinux->Header.TargetOSAPI = PXPXELFTargetOSAPIFromID(osAPIID);

			pxFile->BitFormatOfData = pxBinaryLinux->Header.BitFormat;
			pxFile->EndiannessOfData = pxBinaryLinux->Header.Endian;
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
				{&typeID, PXDataTypeInt16UXE},
				{&machineID, PXDataTypeInt16UXE},
				{&versionID,PXDataTypeInt32UXE},
				{&pxBinaryLinux->Header.EntryPointOfCode,PXDataTypeAdressFlex},
				{&pxBinaryLinux->Header.ProgrammHeaderOffset,PXDataTypeAdressFlex},
				{&pxBinaryLinux->Header.SectionHeaderOffset,PXDataTypeAdressFlex},
				{&e_flags,PXDataTypeInt32UXE},
				{&sizeOfThisHeader, PXDataTypeInt16UXE},
				{&pxBinaryLinux->Header.ProgrammHeaderSize, PXDataTypeInt16UXE},
				{&pxBinaryLinux->Header.ProgrammHeaderAmount, PXDataTypeInt16UXE},
				{&pxBinaryLinux->Header.SectionHeaderSize, PXDataTypeInt16UXE},
				{&pxBinaryLinux->Header.SectionHeaderAmount, PXDataTypeInt16UXE},
				{&shstrndx, PXDataTypeInt16UXE}
			};

			PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

			pxBinaryLinux->Header.Machine = PXELFMachineFromID(machineID);
			pxBinaryLinux->Header.Type = PXELFTypeFromID(typeID);
		}
	}


	//--------------------------------------------------------
	// Program header - Read
	//--------------------------------------------------------
	{
		PXFileCursorMoveTo(pxFile, pxBinaryLinux->Header.ProgrammHeaderOffset);

		for (PXInt16U programHeaderID = 0; programHeaderID < pxBinaryLinux->Header.ProgrammHeaderAmount; ++programHeaderID)
		{
			PXELFProgramHeader pxELFProgramHeader;
			PXInt32U typeID;
			PXInt32U flagsID;

			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{&typeID,PXDataTypeInt32UXE},
				{&flagsID, PXDataTypeInt32ULEOnlyIf64B},
				{&pxELFProgramHeader.AdressOffsetRead,PXDataTypeAdressFlex},
				{&pxELFProgramHeader.AdressTargetVirtual,PXDataTypeAdressFlex},
				{&pxELFProgramHeader.AdressTargetPhysical,PXDataTypeAdressFlex},
				{&pxELFProgramHeader.SizeOnFile,PXDataTypeAdressFlex},
				{&pxELFProgramHeader.SizeInMemory,PXDataTypeAdressFlex},
				{&flagsID, PXDataTypeInt32ULEOnlyIf32B},
				{&pxELFProgramHeader.p_align,PXDataTypeAdressFlex}
			};

			PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

			pxELFProgramHeader.IsSegmentExecutable = 0x01 & flagsID;
			pxELFProgramHeader.IsSegmentWriteable = (0x02 & flagsID) >> 1;
			pxELFProgramHeader.IsSegmentReadable = (0x04 & flagsID) >> 2;
			pxELFProgramHeader.Type = PXELFSegmentTypeFromID(typeID);

#if PXEFIDebug
			printf
			(
				"| Program Header <%i/%i> | R:%c W:%c E:%c | Size: %15iB |\n",
				programHeaderID+1,
				pxELF->Header.ProgrammHeaderAmount,
				pxELFProgramHeader.IsSegmentReadable ? 'x' : '_',
				pxELFProgramHeader.IsSegmentWriteable ? 'x' : '_',
				pxELFProgramHeader.IsSegmentExecutable ? 'x' : '_',
				pxELFProgramHeader.SizeInMemory
			);
#endif
		}
	}

	//--------------------------------------------------------
	// Section header
	//--------------------------------------------------------
	{
		PXFileCursorMoveTo(pxFile, pxBinaryLinux->Header.SectionHeaderOffset);

		for (PXInt16U i = 0; i < pxBinaryLinux->Header.SectionHeaderAmount; ++i)
		{
			PXSectionHeader pxSectionHeader;

			const PXFileDataElementType pxDataStreamElementList[] =
			{
				{&pxSectionHeader.sh_nameOffset,PXDataTypeInt32UXE},
				{&pxSectionHeader.sh_type,PXDataTypeInt32UXE},
				{&pxSectionHeader.sh_flags,PXDataTypeAdressFlex},
				{&pxSectionHeader.sh_addr,PXDataTypeAdressFlex},
				{&pxSectionHeader.FileImageOffset,PXDataTypeAdressFlex},
				{&pxSectionHeader.FileImageSize,PXDataTypeAdressFlex},
				{&pxSectionHeader.sh_link,PXDataTypeInt32UXE},
				{&pxSectionHeader.sh_info,PXDataTypeInt32UXE},
				{&pxSectionHeader.sh_addralign,PXDataTypeAdressFlex},
				{&pxSectionHeader.sh_entsize,PXDataTypeAdressFlex}
			};

			PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));
		}	
	}

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXBinaryLinuxSaveToFile(PXBinaryLinux* const pxBinaryLinux, PXFile* const pxFile)
{
	PXFileWriteB(pxFile, PXELFSignature, sizeof(PXELFSignature));

    return PXActionRefusedNotImplemented;
}
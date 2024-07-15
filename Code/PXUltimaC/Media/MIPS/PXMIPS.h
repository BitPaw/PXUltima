#ifndef PXMIPSINCLUDE
#define PXMIPSINCLUDE

#include "../PXResource.h"

typedef enum PXMIPSInstructionType_
{
	PXMIPSInstructionTypeInvalid = 99999,

	PXMIPSInstructionTypeSpecial = 0, // MIPS has instructions with an ID of 0, they are overloaded
	PXMIPSInstructionTypeJumpRegister = 0b001000 << 8,


	PXMIPSInstructionTypeAddImmediate = 8, // ADDI
	PXMIPSInstructionTypeAddImmediateUnsigned = 9, // ADDIU
	PXMIPSInstructionTypeLoadUpperImmediate = 0b001111, // LUI
	PXMIPSInstructionTypeORImmediate = 13, // ORI

	PXMIPSInstructionTypeStoreWord = 0b101011, // SW
	PXMIPSInstructionTypeStoreWordLeft = 0b101010, // SWL
	PXMIPSInstructionTypeBranchOnNotEqual = 0b000101, // BNE
	PXMIPSInstructionTypeBranchOnNotEqualLikely = 0b010101, // BNEL

	//PXMIPSInstructionType = 00000000000,
	//PXMIPSInstructionType = 00000000000,
	//PXMIPSInstructionType = 00000000000,
	//PXMIPSInstructionType = 00000000000,
	//PXMIPSInstructionType = 00000000000,
}
PXMIPSInstructionType;

typedef struct PXMIPSTInstruction_
{
	PXInt8U* Adress;
	PXInt8U* AdressVirtual;
	PXInt16U Immediate;
	PXMIPSInstructionType Type;
	PXInt8U RS;
	PXInt8U RT;
}
PXMIPSTInstruction;

typedef struct PXMIPSProcessor_
{
	void* ROMOffset;

	PXByte Stack[32000];

	PXSize Register[64]; // general purpose registers
}
PXMIPSProcessor;

typedef struct PXCodeSegment_
{
	void* Start;
	void* Stop;
	void* SourceAdress;
	PXSize SourceSize;
	char* NameShort;
	char* NameLong;
	char MemoryMode;
}
PXCodeSegment;

PXPublic void PXAPI PXMIPSInitializize(PXMIPSProcessor* const pxMIPSProcessor, PXCodeSegment* const pxCodeSegment);

PXPublic const char* PXAPI PXMIPSInstructionTypeToString(const PXMIPSInstructionType pxMIPSInstruction);

PXPublic PXActionResult PXAPI PXMIPSTranslate(PXMIPSProcessor* const pxMIPSProcessor, const PXByte* const data, const PXSize length);

#endif
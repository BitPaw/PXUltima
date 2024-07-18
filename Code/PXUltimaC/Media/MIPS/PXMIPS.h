#ifndef PXMIPSINCLUDE
#define PXMIPSINCLUDE

#include "../PXResource.h"

#define PXMIPSOPCodeNormal    0b00000000 
#define PXMIPSOPCodeSpecial 0b01000000 
#define PXMIPSOPCodeREGIMM    0b10000000 
#define PXMIPSOPCodeCOPz    0b11000000 

typedef enum PXMIPSInstructionType_
{
    PXMIPSInstructionTypeInvalid = 99999,

    // MIPS has instructions with an ID of 0, they are overloaded
    PXMIPSInstructionTypeOPCodeSpecial                      = 0b000000,
    PXMIPSInstructionTypeOPCodeREGIMM                       = 0b000001,
    PXMIPSInstructionTypeJump                               = 0b000010, // J
    PXMIPSInstructionTypeJumpAndLink                        = 0b000011, // JAL
    PXMIPSInstructionTypeBranchOnEqual                      = 0b000100, // BEQ
    PXMIPSInstructionTypeBranchOnNotEqual                   = 0b000101, // BNE
    PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero      = 0b000110, // BLEZ
    PXMIPSInstructionTypeBranchOnGreaterThanZero            = 0b000111, // BGTZ
    PXMIPSInstructionTypeAddImmediate                       = 0b001000, // ADDI
    PXMIPSInstructionTypeAddImmediateUnsigned               = 0b001001, // ADDIU
    PXMIPSInstructionTypeSetOnLessThanImmediate             = 0b001010, // SLTI
    PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned     = 0b001011, // SLTIU 
    PXMIPSInstructionTypeAndImmediate                       = 0b001100, // ANDI
    PXMIPSInstructionTypeORImmediate                        = 0b001101, // ORI
    PXMIPSInstructionTypeExclusiveOrImmediate               = 0b001110, // XORI
    PXMIPSInstructionTypeLoadUpperImmediate                 = 0b001111, // LUI
    PXMIPSInstructionTypeCoprocessorZOperation0             = 0b010000, // COP0
    PXMIPSInstructionTypeCoprocessorZOperation1             = 0b010001, // COP1
    PXMIPSInstructionTypeCoprocessorZOperation2             = 0b010010, // COP2
    // Reserved = 0b010011, // ??? 
    PXMIPSInstructionTypeBranchOnEqualLikely                = 0b010100, // BEQL
    PXMIPSInstructionTypeBranchOnNotEqualLikely             = 0b010101, // BNEL
    PXMIPSInstructionTypeBranchOnLessThan                   = 0b010110, // BLEZL
    PXMIPSInstructionTypeBranchOnGreaterThanZeroLikely      = 0b010111, // BGTZL
    PXMIPSInstructionTypeDoublewordADDImmediate             = 0b011000, // DADDI
    PXMIPSInstructionTypeDoublewordADDImmediateUnsigned     = 0b011001, // DADDIU
    PXMIPSInstructionTypeLoadDoublewordLeft                 = 0b011010, // LDL
    PXMIPSInstructionTypeLoadDoublewordRight                = 0b011011, // LDR
    // Reserved = 0b011100, // ??? 
    // Reserved = 0b011101, // ???
    // Reserved = 0b011110, // ???
    // Reserved = 0b011111, // ???
    PXMIPSInstructionTypeLoadByte                           = 0b100000, // LB
    PXMIPSInstructionTypeLoadHalfword                       = 0b100001, // LH
    PXMIPSInstructionTypeLoadWordLeft                       = 0b100010, // LWL
    PXMIPSInstructionTypeLoadWord                           = 0b100011, // LW
    PXMIPSInstructionTypeLoadByteUnsigned                   = 0b100100, // LBU
    PXMIPSInstructionTypeLoadHalfwordUnsigned               = 0b100101, // LHU
    PXMIPSInstructionTypeLoadWordRight                      = 0b100110, // LWR
    PXMIPSInstructionTypeLoadWordUnsigned                   = 0b100111, // LWU
    PXMIPSInstructionTypeStoreByte                          = 0b101000, // SB
    PXMIPSInstructionTypeStoreHalfword                      = 0b101001, // SH
    PXMIPSInstructionTypeStoreWordLeft                      = 0b101010, // SWL
    PXMIPSInstructionTypeStoreWord                          = 0b101011, // SW
    PXMIPSInstructionTypeStoreDoublewordLeft                = 0b101100, // SDL
    PXMIPSInstructionTypeStoreDoublewordRight               = 0b101101, // SDR
    PXMIPSInstructionTypeStoreWordRight                     = 0b101110, // SWR
    PXMIPSInstructionTypeCacheOperation                     = 0b101111, // CACHE
    PXMIPSInstructionTypeLoadLinked                         = 0b110000, // LL
    PXMIPSInstructionLoadWordToFPUCoprocessor1              = 0b110001, // LWC1
    PXMIPSInstructionLoadWordToFPUCoprocessor2              = 0b110010, // LWC2
    // Reserved = 0b110011, // ???
    PXMIPSInstructionTypeLoadLinkedDoubleword               = 0b110100, // LLD
    PXMIPSInstructionTypeLoadDoublewordToFPUCoprocessor1    = 0b110101, // LDC1
    PXMIPSInstructionTypeLoadDoublewordToFPUCoprocessor2    = 0b110110, // LDC2
    PXMIPSInstructionTypeLoadDoubleword                     = 0b110111, // LD
    PXMIPSInstructionTypeStoreConditional                   = 0b111000, // SC
    PXMIPSInstructionTypeStoreWordFromFPUCoprocessor1       = 0b111001, // SWC1
    PXMIPSInstructionTypeStoreWordFromFPUCoprocessor2       = 0b111010, // SEC2
    // Reserved = 0b111011, // ???
    PXMIPSInstructionTypeStoreConditionalDoubleword         = 0b111100, // SCD
    PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor1 = 0b111101, // SCD1
    PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor2 = 0b111110, // SCD2
    PXMIPSInstructionTypeStoreDoubleword                    = 0b111111, // SD
    //--------------------------------------------------------



    //--------------------------------------------------------
    // To solve these 'special' instuctions, we shift the command by 1 Byte
    //--------------------------------------------------------
    PXMIPSInstructionTypeShiftLeftLogical                       = 0b000000 | PXMIPSOPCodeSpecial, // SLL
    // Reserved = 0b000001, // ???
    PXMIPSInstructionTypeShiftRightLogical                      = 0b000010 | PXMIPSOPCodeSpecial, // SRL
    PXMIPSInstructionTypeShiftRightArithmetic                   = 0b000011 | PXMIPSOPCodeSpecial, // SRA
    PXMIPSInstructionTypeShiftLeftLogicalVariable               = 0b000100 | PXMIPSOPCodeSpecial, // SLLV
    // Reserved = 0b000101, // ???
    PXMIPSInstructionTypeShiftRightLogicalVariable              = 0b000110 | PXMIPSOPCodeSpecial, // SRLV
    PXMIPSInstructionTypeShiftRightArithmeticVariable           = 0b000111 | PXMIPSOPCodeSpecial, // SRAV
    PXMIPSInstructionTypeJumpRegister                           = 0b001000 | PXMIPSOPCodeSpecial, // JR
    PXMIPSInstructionTypeJumpAndLinkRegister                    = 0b001001 | PXMIPSOPCodeSpecial, // JALR
    // Reserved = 0b001010, // ???
    // Reserved = 0b001011, // ???
    PXMIPSInstructionTypeSystemCall                             = 0b001100 | PXMIPSOPCodeSpecial, // SYSCALL
    PXMIPSInstructionTypeBreak                                  = 0b001101 | PXMIPSOPCodeSpecial, // BREAK
    // Reserved = 0b001110, // ???
    PXMIPSInstructionTypeSynchronize                            = 0b001111 | PXMIPSOPCodeSpecial, // SYNC
    PXMIPSInstructionTypeMoveFromHI                             = 0b010000 | PXMIPSOPCodeSpecial, // MFHI
    PXMIPSInstructionTypeMoveToHI                               = 0b010001 | PXMIPSOPCodeSpecial, // MTHI
    PXMIPSInstructionTypeMoveFromLO                             = 0b010010 | PXMIPSOPCodeSpecial, // MFLO
    PXMIPSInstructionTypeMoveToLO                               = 0b010011 | PXMIPSOPCodeSpecial, // MTLO
    PXMIPSInstructionTypeDoublewordShiftLeftLogicalVariable     = 0b010100 | PXMIPSOPCodeSpecial, // DSLLV
    // Reserved = 0b010101, // ???
    PXMIPSInstructionTypeDoublewordShiftRightLogicalVariable    = 0b010110 | PXMIPSOPCodeSpecial, // DSRLV
    PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable = 0b010111 | PXMIPSOPCodeSpecial,// DSRAV
    PXMIPSInstructionTypeMultiply                               = 0b011000 | PXMIPSOPCodeSpecial, // MULT
    PXMIPSInstructionTypeMultiplyUnsigned                       = 0b011001 | PXMIPSOPCodeSpecial, // MULTU
    PXMIPSInstructionTypeDivide                                 = 0b011010 | PXMIPSOPCodeSpecial, // DIV
    PXMIPSInstructionTypeDivideUnsigned                         = 0b011011 | PXMIPSOPCodeSpecial, // DIVU
    PXMIPSInstructionTypeDoublewordMultiply                     = 0b011100 | PXMIPSOPCodeSpecial, // DMULT
    PXMIPSInstructionTypeDoublewordMultiplyUnsigned             = 0b011101 | PXMIPSOPCodeSpecial, // DMULTU
    PXMIPSInstructionTypeDoublewordDivide                       = 0b011110 | PXMIPSOPCodeSpecial, // DDIV
    PXMIPSInstructionTypeDoublewordDivideUnsigned               = 0b011111 | PXMIPSOPCodeSpecial, // DDIVU
    PXMIPSInstructionTypeAdd                                    = 0b100000 | PXMIPSOPCodeSpecial, // ADD
    PXMIPSInstructionTypeAddUnsigned                            = 0b100001 | PXMIPSOPCodeSpecial, // ADDU
    PXMIPSInstructionTypeSubtract                               = 0b100010 | PXMIPSOPCodeSpecial, // SUB
    PXMIPSInstructionTypeSubtractUnsigned                       = 0b100011 | PXMIPSOPCodeSpecial, // SUBU 
    PXMIPSInstructionTypeAND                                    = 0b100100 | PXMIPSOPCodeSpecial, // AND
    PXMIPSInstructionTypeOR                                     = 0b100101 | PXMIPSOPCodeSpecial, // OR
    PXMIPSInstructionTypeExclusiveOR                            = 0b100110 | PXMIPSOPCodeSpecial, // XOR
    PXMIPSInstructionTypeNOR                                    = 0b100111 | PXMIPSOPCodeSpecial, // NOR 
    // Reserved = 0b101000, // ???
    // Reserved = 0b111001, // ???
    PXMIPSInstructionTypeSetOnLessThan                          = 0b101010 | PXMIPSOPCodeSpecial, // SLT
    PXMIPSInstructionTypeSetOnLessThanUnsigned                  = 0b101011 | PXMIPSOPCodeSpecial, // SLTU
    PXMIPSInstructionTypeDoublewordADD                          = 0b101100 | PXMIPSOPCodeSpecial, // DADD
    PXMIPSInstructionTypeDoublewordADDUnsigned                  = 0b101101 | PXMIPSOPCodeSpecial, // DADDU
    PXMIPSInstructionTypeDoublewordSubtract                     = 0b101110 | PXMIPSOPCodeSpecial, // DSUB
    PXMIPSInstructionTypeDoublewordSubtractUnsigned             = 0b101111 | PXMIPSOPCodeSpecial, // DSUBU
    PXMIPSInstructionTypeTrapIfGreaterThanOrEqual               = 0b110000 | PXMIPSOPCodeSpecial, // TGE
    PXMIPSInstructionTypeTrapIfGreaterThanOrEqualUnsigned       = 0b110001 | PXMIPSOPCodeSpecial, // TGEU
    PXMIPSInstructionTypeTrapIfLessThan                         = 0b110010 | PXMIPSOPCodeSpecial, // TLT
    PXMIPSInstructionTypeTrapIfLessThanUnsigned                 = 0b110011 | PXMIPSOPCodeSpecial, // TLTU
    PXMIPSInstructionTypeTrapIfEqual                            = 0b110100 | PXMIPSOPCodeSpecial, // TEQ
    // Reserved = 0b110101, // ???
    PXMIPSInstructionTypeTrapIfNotEqual                         = 0b110110 | PXMIPSOPCodeSpecial, // TNE
    // Reserved = 0b110111, // ???
    PXMIPSInstructionTypeDoublewordShiftLeftLogical             = 0b111000 | PXMIPSOPCodeSpecial, // DSLL
    // Reserved = 0b111001, // ???
    PXMIPSInstructionTypeDoublewordShiftRightLogical            = 0b111010 | PXMIPSOPCodeSpecial, // DSRL
    PXMIPSInstructionTypeDoublewordShiftRightArithmetic         = 0b111011 | PXMIPSOPCodeSpecial, // DSRA
    PXMIPSInstructionTypeDoublewordShiftLeftLogicalPlus32       = 0b111100 | PXMIPSOPCodeSpecial, // DSLL32
    // Reserved = 0b111101, // ???
    PXMIPSInstructionTypeDoublewordShiftRight                   = 0b111110 | PXMIPSOPCodeSpecial, // DSRL32
    PXMIPSInstructionTypeDoublewordShiftRightArithmeticPlus32   = 0b111111 | PXMIPSOPCodeSpecial, // DSRA32

    //----------------------------------------------------------------------------




    //----------------------------------------------------------------------------
    // REGIMM
    //----------------------------------------------------------------------------
    // BLTZ
    // BGEZ
    // BLTZL
    // BGEZL
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // TGEI
    // TGEIU
    // TLTI
    // TLTIU
    // TEQI
    // Reserved = xxxxx, // ???
    // TNEI
    // Reserved = xxxxx, // ???
    // BLTZAL
    // BGEZAL
    // BLTZALL
    // BGEZALL
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    //----------------------------------------------------------------------------



    //----------------------------------------------------------------------------
    // COPz rs
    //----------------------------------------------------------------------------
    // MF
    // DMF
    // CF
    // ----
    // MT
    // DMT
    // CT
    // ----
    // BC
    //----------------------------------------------------------------------------



    //----------------------------------------------------------------------------
    // COPz rt
    //----------------------------------------------------------------------------

    //----------------------------------------------------------------------------



    //----------------------------------------------------------------------------
    // CP0 Function
    //----------------------------------------------------------------------------

    //----------------------------------------------------------------------------
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

    PXMIPSTInstructionFunction* GeneralInstructionList;
    PXMIPSTInstructionFunction* SpecialInstructionList;
    PXBool IncrmentCounter;
}
PXMIPSTInstruction;

typedef void (PXAPI* PXMIPSTInstructionFunction)(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);





#define PXMIPSProcessorLinkAddress 31

typedef struct PXMIPSProcessor_
{
    void* ROMOffset;

    PXSize ProgramCounter;

    float RegisterFloat[32];
    
    PXSize Register[255]; // GPRs general purpose registers (should be 32)
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

PXPublic const char* PXAPI PXMIPSInstructionTypeToStringShort(const PXMIPSInstructionType pxMIPSInstruction);
PXPublic const char* PXAPI PXMIPSInstructionTypeToStringLong(const PXMIPSInstructionType pxMIPSInstruction);

PXPublic PXActionResult PXAPI PXMIPSTranslate(PXMIPSProcessor* const pxMIPSProcessor, const PXByte* const data, const PXSize length);



PXPublic void PXAPI PXMIPSInstructionReserved(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);

// General instructions
PXPublic void PXAPI PXMIPSInstructionSpecial(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionREGIMM(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionJump(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionJumpAndLink(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnNotEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnLessThanOrEqualToZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnGreaterThanZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionAddImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionAddImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSetOnLessThanImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSetOnLessThanImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionAndImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionORImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionExclusiveOrImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadUpperImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionCoprocessorZOperation0(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionCoprocessorZOperation1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionCoprocessorZOperation2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnEqualLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnNotEqualLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnGreaterThanZeroLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordADDImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordADDImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadDoublewordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadDoublewordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadByte(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadHalfword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadWordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadWord(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadByteUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadHalfwordUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadWordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadWordUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreByte(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreHalfword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreWordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreWord(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreDoublewordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreDoublewordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreWordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionCacheOperation(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadLinked(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadWordToFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadWordToFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadLinkedDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadDoublewordToFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadDoublewordToFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionLoadDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreConditional(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreWordFromFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreWordFromFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreConditionalDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreDoublewordFromFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreDoublewordFromFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionStoreDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);



// Special instructions
PXPublic void PXAPI PXMIPSInstructionTypeShiftLeftLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeShiftRightLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeShiftRightArithmetic(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeShiftLeftLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeShiftRightLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeShiftRightArithmeticVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeJumpRegister(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeJumpAndLinkRegister(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeSystemCall(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeBreak(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeSynchronize(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeMoveFromHI(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeMoveToHI(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeMoveFromLO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeMoveToLO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftLeftLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftRightLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeMultiply(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeMultiplyUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDivide(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDivideUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordMultiply(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordMultiplyUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordDivide(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordDivideUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeAdd(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeAddUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeSubtract(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeSubtractUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeAND(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeExclusiveOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeNOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeSetOnLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeSetOnLessThanUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordADD(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordADDUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordSubtract(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordSubtractUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeTrapIfGreaterThanOrEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeTrapIfGreaterThanOrEqualUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeTrapIfLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeTrapIfLessThanUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeTrapIfEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeTrapIfNotEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftLeftLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftRightLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftRightArithmetic(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftLeftLogicalPlus32(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTypeDoublewordShiftRightArithmeticPlus32(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);




















#endif
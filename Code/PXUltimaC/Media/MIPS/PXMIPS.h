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
    // COP0
    // COP1
    // COP2
    // MISSING
    // BEQL
    PXMIPSInstructionTypeBranchOnNotEqualLikely             = 0b010101, // BNEL
    // BLEZL
    // BGTZL
    PXMIPSInstructionTypeDoublewordADDImmediate             = 0b011000, // DADDI
    PXMIPSInstructionTypeDoublewordADDImmediateUnsigned     = 0b011001, // DADDIU
    PXMIPSInstructionTypeLoadDoublewordLeft                 = 0b011010, // LDL
    PXMIPSInstructionTypeLoadDoublewordRight                = 0b011011, // LDR
    // Missing 
    // Missing 
    // Missing 
    // Missing 
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
    // LWC1
    // LWC2
    // MISSING
    PXMIPSInstructionTypeLoadLinkedDoubleword               = 0b110100, // LLD
    // LDC1
    // LDC2
    PXMIPSInstructionTypeLoadDoubleword                     = 0b110111, // LD
    PXMIPSInstructionTypeStoreConditional                   = 0b111000, // SC
    // SWC1
    // SEC2
    // Missing
    PXMIPSInstructionTypeStoreConditionalDoubleword         = 0b111100, // SCD
    // SCD1
    // SCD2
    PXMIPSInstructionTypeStoreDoubleword                    = 0b111111, // SD
    //--------------------------------------------------------



    //--------------------------------------------------------
    // To solve these 'special' instuctions, we shift the command by 1 Byte
    //--------------------------------------------------------
    PXMIPSInstructionTypeShiftLeftLogical                   = 0b000000 | PXMIPSOPCodeSpecial, // SLL
    // missing instruction 0x01
    PXMIPSInstructionTypeShiftRightLogical                  = 0b000010 | PXMIPSOPCodeSpecial, // SRL
    PXMIPSInstructionTypeShiftRightArithmetic               = 0b000011 | PXMIPSOPCodeSpecial, // SRA
    PXMIPSInstructionTypeShiftLeftLogicalVariable           = 0b000100 | PXMIPSOPCodeSpecial, // SLLV
    // missing instruction 0x0101
    PXMIPSInstructionTypeShiftRightLogicalVariable          = 0b000110 | PXMIPSOPCodeSpecial, // SRLV
    PXMIPSInstructionTypeShiftRightArithmeticVariable       = 0b000111 | PXMIPSOPCodeSpecial, // SRAV


    PXMIPSInstructionTypeJumpRegister                       = 0b001000 | PXMIPSOPCodeSpecial, // JR
    PXMIPSInstructionTypeJumpAndLinkRegister                = 0b001001 | PXMIPSOPCodeSpecial, // JALR
    // MISSING
    // MISSING
    PXMIPSInstructionTypeSystemCall                         = 0b001100 | PXMIPSOPCodeSpecial, // SYSCALL
    PXMIPSInstructionTypeBreak                              = 0b001101 | PXMIPSOPCodeSpecial, // BREAK
    // MISSING
    PXMIPSInstructionTypeSynchronize                        = 0b001111 | PXMIPSOPCodeSpecial, //  SYNC


    PXMIPSInstructionTypeMoveFromHI                         = 0b010000 | PXMIPSOPCodeSpecial, // MFHI
    PXMIPSInstructionTypeMoveToHI                           = 0b010001 | PXMIPSOPCodeSpecial, // MTHI
    PXMIPSInstructionTypeMoveFromLO                         = 0b010010 | PXMIPSOPCodeSpecial, // MFLO
    PXMIPSInstructionTypeMoveToLO                           = 0b010011 | PXMIPSOPCodeSpecial, // MTLO
    PXMIPSInstructionTypeDoublewordShiftLeftLogicalVariable = 0b010100 | PXMIPSOPCodeSpecial, // DSLLV
    // MISSING
    // 000





    PXMIPSInstructionTypeMultiply                           = 0b011000 | PXMIPSOPCodeSpecial, // MULT
    PXMIPSInstructionTypeMultiplyUnsigned                   = 0b011001 | PXMIPSOPCodeSpecial, // MULTU
    PXMIPSInstructionTypeDivide                             = 0b011010 | PXMIPSOPCodeSpecial, // DIV
    PXMIPSInstructionTypeDivideUnsigned                     = 0b011011 | PXMIPSOPCodeSpecial, // DIVU
    PXMIPSInstructionTypeDoublewordMultiply                 = 0b011100 | PXMIPSOPCodeSpecial, // DMULT
    PXMIPSInstructionTypeDoublewordMultiplyUnsigned         = 0b011101 | PXMIPSOPCodeSpecial, // DMULTU
    PXMIPSInstructionTypeDoublewordDivide                   = 0b011110 | PXMIPSOPCodeSpecial, // DDIV
    PXMIPSInstructionTypeDoublewordDivideUnsigned           = 0b011111 | PXMIPSOPCodeSpecial, // DDIVU
    PXMIPSInstructionTypeAdd                                = 0b100000 | PXMIPSOPCodeSpecial, // ADD
    PXMIPSInstructionTypeAddUnsigned                        = 0b100001 | PXMIPSOPCodeSpecial, // ADDU
    PXMIPSInstructionTypeSubtract                           = 0b100010 | PXMIPSOPCodeSpecial, // SUB
    PXMIPSInstructionTypeSubtractUnsigned                   = 0b100011 | PXMIPSOPCodeSpecial, // SUBU 
    PXMIPSInstructionTypeAND                                = 0b100100 | PXMIPSOPCodeSpecial, // AND
    PXMIPSInstructionTypeOR                                 = 0b100101 | PXMIPSOPCodeSpecial, // OR
    PXMIPSInstructionTypeExclusiveOR                        = 0b100110 | PXMIPSOPCodeSpecial, // XOR
    PXMIPSInstructionTypeNOR                                = 0b100111 | PXMIPSOPCodeSpecial, // NOR 
    PXMIPSInstructionTypeUnkownA                            = 0b101000 | PXMIPSOPCodeSpecial, // ?????????????????
    PXMIPSInstructionTypeUnkownB                            = 0b101001 | PXMIPSOPCodeSpecial, // ????????????????? 
    PXMIPSInstructionTypeSetOnLessThan                      = 0b101010 | PXMIPSOPCodeSpecial, // SLT
    PXMIPSInstructionTypeSetOnLessThanUnsigned              = 0b101011 | PXMIPSOPCodeSpecial, // SLTU
    PXMIPSInstructionTypeDoublewordADD                      = 0b101100 | PXMIPSOPCodeSpecial, // DADD
    PXMIPSInstructionTypeDoublewordADDUnsigned              = 0b101101 | PXMIPSOPCodeSpecial, // DADDU
    PXMIPSInstructionTypeDoublewordSubtract                 = 0b101110 | PXMIPSOPCodeSpecial, // DSUB
    PXMIPSInstructionTypeDoublewordSubtractUnsigned         = 0b101111 | PXMIPSOPCodeSpecial, // DSUBU
    PXMIPSInstructionTypeTrapIfGreaterThanOrEqual           = 0b110000 | PXMIPSOPCodeSpecial, // TGE
    PXMIPSInstructionTypeTrapIfGreaterThanOrEqualUnsigned   = 0b110001 | PXMIPSOPCodeSpecial, // TGEU
    PXMIPSInstructionTypeTrapIfLessThan                     = 0b110010 | PXMIPSOPCodeSpecial, // TLT
    PXMIPSInstructionTypeTrapIfLessThanUnsigned             = 0b110011 | PXMIPSOPCodeSpecial, // TLTU
    PXMIPSInstructionTypeTrapIfEqual                        = 0b110100 | PXMIPSOPCodeSpecial, // TEQ
    PXMIPSInstructionTypeUnkownC                            = 0b110101 | PXMIPSOPCodeSpecial, // ?????????????????
    PXMIPSInstructionTypeTrapIfNotEqual                     = 0b110110 | PXMIPSOPCodeSpecial, // TNE
    PXMIPSInstructionTypeUnkownD                            = 0b110111 | PXMIPSOPCodeSpecial, // ?????????????????
    PXMIPSInstructionTypeDoublewordShiftLeftLogical         = 0b111000 | PXMIPSOPCodeSpecial, // DSLL
    PXMIPSInstructionTypeUnkownF                            = 0b111001 | PXMIPSOPCodeSpecial, // ?????????????????
    // DSRL
    PXMIPSInstructionTypeDoublewordShiftRightArithmetic     = 0b111011 | PXMIPSOPCodeSpecial, // DSRA
    PXMIPSInstructionTypeDoublewordShiftLeftLogicalPlus32   = 0b111100 | PXMIPSOPCodeSpecial, // DSLL32
    // MISSING
    // DSRL32
    // DSRA32

    //----------------------------------------------------------------------------








    PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable = 0b010111 | 0x60, // DSRAV
    // DSRLV





    PXMIPSInstructionTypeMoveToSystemControlCoprocessor        = 0b010000, // MTC0
    // MTC0
    //MTCz


    TrapIfEqualImmediate, // TEQI
    TrapIfGreaterThanOrEqualImmediate, // TGEI
    // TGEIU
    // TLBP
    // TLBR
    // TLBWI
    // TLBWR
    // TLTI
    // TLTIU
    // TNEI







    //--------------------------------------------------------



    PXMIPSInstructionTypeMoveFromSystemControlCoprocessor = 0b010000, // MFC0
    // MFCz






    PXMIPSInstructionTypeBranchOnCoprocessorZFalse, // BCzF
    PXMIPSInstructionTypeBranchOnCoprocessorZFalseLikely, // BCzFL

    PXMIPSInstructionTypeBranchOnCoprocessorZTrue, // BCzT
    PXMIPSInstructionTypeBranchOnCoprocessorZTrueLikely, // BCzTL
    //PXMIPSInstructionTypeBranchOnEqual = 0b000100,
    //PXMIPSInstructionTypeBranchOnEqualLikely = 0b010100,
    PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZero, // BGEZ



    //REGIMM
    PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLink, // BGEZAL
    //BGEZALL
    // BGEZL
    //BGTZ
    //BGTZL
    // BLEZ
    // BLEZL
    //BLTZ
    // BLTZAL
    //BLTZALL
    // BLTZL


    


    // CFCz
    // COPz
    // CTCz



    // COP0
    PXMIPSInstructionTypeDoublewordMoveFromSystemControlCoprocessor, // DMFC0
    // DMTC0
    ReturnFromException, // ERET
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

#endif
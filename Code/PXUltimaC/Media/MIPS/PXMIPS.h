#ifndef PXMIPSINCLUDE
#define PXMIPSINCLUDE

#include "../PXResource.h"

#define PXMIPSOPCodeNormal  0b00000000 
#define PXMIPSOPCodeSpecial 0b01000000 
#define PXMIPSOPCodeREGIMM  0b10000000 
#define PXMIPSOPCodeCOPz    0b11000000 

typedef enum PXMIPSInstructionType_
{
    PXMIPSInstructionTypeInvalid = 99999,

    //----------------------------------------------------------------------------
    // General instructions
    //----------------------------------------------------------------------------
    PXMIPSInstructionTypeOPCodeSpecial                      = 0b000000,
    PXMIPSInstructionTypeOPCodeREGIMM                       = 0b000001,
    PXMIPSInstructionTypeJump                               = 0b000010, // J
    PXMIPSInstructionTypeJumpAndLink                        = 0b000011, // JAL
    PXMIPSInstructionTypeBranchOnEqual                      = 0b000100, // BEQ
    PXMIPSInstructionTypeBranchOnNotEqual                   = 0b000101, // BNE
    PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero      = 0b000110, // BLEZ
    PXMIPSInstructionTypeBranchOnGreaterThanZero            = 0b000111, // BGTZ
    PXMIPSInstructionTypeADDImmediate                       = 0b001000, // ADDI
    PXMIPSInstructionTypeADDImmediateUnsigned               = 0b001001, // ADDIU
    PXMIPSInstructionTypeSetOnLessThanImmediate             = 0b001010, // SLTI
    PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned     = 0b001011, // SLTIU 
    PXMIPSInstructionTypeANDImmediate                       = 0b001100, // ANDI
    PXMIPSInstructionTypeORImmediate                        = 0b001101, // ORI
    PXMIPSInstructionTypeExclusiveORImmediate               = 0b001110, // XORI
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
    PXMIPSInstructionTypeLoadWordToFPUCoprocessor1          = 0b110001, // LWC1
    PXMIPSInstructionTypeLoadWordToFPUCoprocessor2          = 0b110010, // LWC2
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
    PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable = 0b010111 | PXMIPSOPCodeSpecial, // DSRAV
    PXMIPSInstructionTypeMultiply                               = 0b011000 | PXMIPSOPCodeSpecial, // MULT
    PXMIPSInstructionTypeMultiplyUnsigned                       = 0b011001 | PXMIPSOPCodeSpecial, // MULTU
    PXMIPSInstructionTypeDivide                                 = 0b011010 | PXMIPSOPCodeSpecial, // DIV
    PXMIPSInstructionTypeDivideUnsigned                         = 0b011011 | PXMIPSOPCodeSpecial, // DIVU
    PXMIPSInstructionTypeDoublewordMultiply                     = 0b011100 | PXMIPSOPCodeSpecial, // DMULT
    PXMIPSInstructionTypeDoublewordMultiplyUnsigned             = 0b011101 | PXMIPSOPCodeSpecial, // DMULTU
    PXMIPSInstructionTypeDoublewordDivide                       = 0b011110 | PXMIPSOPCodeSpecial, // DDIV
    PXMIPSInstructionTypeDoublewordDivideUnsigned               = 0b011111 | PXMIPSOPCodeSpecial, // DDIVU
    PXMIPSInstructionTypeADD                                    = 0b100000 | PXMIPSOPCodeSpecial, // ADD
    PXMIPSInstructionTypeADDUnsigned                            = 0b100001 | PXMIPSOPCodeSpecial, // ADDU
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
    PXMIPSInstructionTypeBranchOnLessThanZero                           = 0b00000 | PXMIPSOPCodeREGIMM, // BLTZ
    PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZero               = 0b00001 | PXMIPSOPCodeREGIMM, // BGEZ
    PXMIPSInstructionTypeBranchOnLessThanZeroLikely                     = 0b00010 | PXMIPSOPCodeREGIMM, // BLTZL
    PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroLikely         = 0b00011 | PXMIPSOPCodeREGIMM, // BGEZL
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    // Reserved = xxxxx, // ???
    PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediate               = 0b01000 | PXMIPSOPCodeREGIMM, // TGEI
    PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediateUnsigned       = 0b01001 | PXMIPSOPCodeREGIMM, // TGEIU
    PXMIPSInstructionTypeTrapIfLessThanImmediate                         = 0b01010 | PXMIPSOPCodeREGIMM, // TLTI
    PXMIPSInstructionTypeTrapIfLessThanImmediateUnsigned                 = 0b01011 | PXMIPSOPCodeREGIMM, // TLTIU
    PXMIPSInstructionTypeTrapIfEqualImmediate                            = 0b01100 | PXMIPSOPCodeREGIMM, // TEQI
    // Reserved = xxxxx, // ???
    PXMIPSInstructionTypeTrapIfNotEqualImmediate                         = 0b01110 | PXMIPSOPCodeREGIMM, // TNEI
    // Reserved = xxxxx, // ???
    PXMIPSInstructionTypeBranchOnLessThanZeroAndLink                     = 0b10000 | PXMIPSOPCodeREGIMM, // BLTZAL
    PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLink         = 0b10001 | PXMIPSOPCodeREGIMM, // BGEZAL
    PXMIPSInstructionTypeBranchOnLessThanZeroAndLinkLikely               = 0b10010 | PXMIPSOPCodeREGIMM, // BLTZALL
    PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLinkLikely   = 0b10011 | PXMIPSOPCodeREGIMM, // BGEZALL
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
    PXMIPSInstructionTypeMoveWordFromFPUCoprocessor1                = 0b00000 | PXMIPSOPCodeCOPz, // MF
    PXMIPSInstructionTypeDoublewordMoveFromSystemControlCoprocessor = 0b00001 | PXMIPSOPCodeCOPz, // DMFC0
    PXMIPSInstructionTypeMoveControlWordFromCoprocessorZ            = 0b00010 | PXMIPSOPCodeCOPz, // CFCz - FPU
    // Reserved = xxxxx, // ???
    PXMIPSInstructionTypeMoveToSystemControlCoprocessor             = 0b00100 | PXMIPSOPCodeCOPz, // MT / MTC0
    PXMIPSInstructionTypeDoublewordMoveToSystemControlCoprocessor   = 0b00101 | PXMIPSOPCodeCOPz, // DMTC0
    PXMIPSInstructionTypeMoveControlToCoprocessorZ                  = 0b00110 | PXMIPSOPCodeCOPz, // CTCz
    // = 0b00111 // Reserved = xxxxx, // ???
    // = 0b01000 // BC
    //----------------------------------------------------------------------------



    //----------------------------------------------------------------------------
    // BC - COPz rt - Only for CoProcessor 1
    //----------------------------------------------------------------------------
    BranchOnFPUFalse        = 0b00000, // BCF
    BranchOnFPUFalseLikely  = 0b00010, // BCT
    BranchOnFPUTrue         = 0b00001, // BCFL
    BranchOnFPUTrueLikely   = 0b00011, // BCTL
    //----------------------------------------------------------------------------



    //----------------------------------------------------------------------------
    // CP0 Function
    //----------------------------------------------------------------------------
    // ---
    // TLBR
    // TLBWI
    // ----
    // ----
    // ----
    // TLBWR
    // ----
    // TLBP
    //----------------------------------------------------------------------------
}
PXMIPSInstructionType;


#define PXMIPSProcessorLinkAddress 31

#define PXMIPSRegisterStatusFR 0b0001 // ???

#define PXMIPSRegisterFT 2


typedef struct PXMIPSRegister_
{
    PXSize Index; // 0
    PXSize Random; // 1
    PXSize EntryLo0; // 2
    PXSize EntryLo1; // 3
    PXSize Context; // 4
    PXSize PageMask; // 5
    PXSize Wired; // 6
    PXSize UnusedA; // 7 - unused
    PXSize BadVAddr; // 8
    PXSize Count; // 9
    PXSize EntryHi; // 10
    PXSize Compare; // 11
    PXSize Status; // 12
    PXSize Cause; // 13
    PXSize EPC; // 14
    PXSize PRId; // 15
    PXSize Config; // 16
    PXSize LLAddr; // 17
    PXSize WatchLo; // 18
    PXSize WatchHi; // 19
    PXSize XContext; // 20
    PXSize UnusedB; // 21
    PXSize UnusedC; // 22
    PXSize UnusedD; // 23
    PXSize UnusedE; // 24
    PXSize UnusedF; // 25
    PXSize ParityError; // 26
    PXSize CacheError; // 27
    PXSize TagLo; // 28
    PXSize TagHi; // 29
    PXSize ErrorEPC; // 30
    PXSize UnusedG; // 31
}
PXMIPSRegister;

typedef void (PXAPI* PXMIPSTInstructionFunction)(struct PXMIPSProcessor_* const pxMIPSProcessor, struct PXMIPSTInstruction_* const pxMIPSTInstruction);


typedef struct PXMIPSCoProcessor_
{

    PXSize RegisterList[32];
    PXBool Enabled;
}
PXMIPSCoProcessor;

typedef struct PXMIPSProcessor_
{
    PXMIPSTInstructionFunction* GeneralInstructionList;
    PXMIPSTInstructionFunction* SpecialInstructionList;
    PXMIPSTInstructionFunction* REGIMMInstructionList;
    PXMIPSTInstructionFunction* CorpocessorList;

    void* ROMOffsetVirtual;
    void* ROMOffsetActual;
    PXSize ROMOffsetCurrent;
    PXSize ROMOffsetMaximal;

    void* RAMAdressVirtual;
    void* RAMAdress;
    PXSize RAMSize;

    PXSize ProgramCounter;
    PXSize RegisterStatus;

    float RegisterFloat[32];

    union
    {
        PXSize RegisterList[32]; // GPRs general purpose registers (should be 32)
        PXMIPSRegister Register;
    };

    PXMIPSCoProcessor CoProcessor[4];
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



typedef struct PXMIPSTInstruction_
{
    PXInt8U* Adress;
    PXInt8U* AdressVirtual;
    PXInt16U Immediate;

    PXInt32U OperationCode;

    PXMIPSInstructionType Type;

    PXInt8U RegisterSourceID;
    PXInt8U RegisterTargetID; // target register ID
    PXInt8U RegisterDestinationID; // destination? register ID
    PXInt8U ShiftAmount;

    PXInt8U FS; // Floatingpoint register ID



    PXBool IncrmentCounter;
    PXInt8U CoProcessorID;
}
PXMIPSTInstruction;



//---------------------------------------------------------
// UTility function
//---------------------------------------------------------

#define PXMIPSMemoryIOStore 0x01
#define PXMIPSMemoryIOLoad  0x02

void PXAPI PXMIPSMemoryIO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction, const PXInt32U datatype, PXInt8U mode);


#define PXMIPSBranchEqual 0x01
#define PXMIPSBranchNotEqual  0x02
#define PXMIPSBranchLessThanZero 0x03
#define PXMIPSBranchGreaterThanZero 0x04
#define PXMIPSBranchLessThanOrEqualZero 0x05
#define PXMIPSBranchGreaterThanOrEqualZero 0x06

// We could use these flags at some point but with allignment rules, this would add no gain at all
#define PXMIPSBranchCondition           0b0000000000001111
#define PXMIPSBranchLikely              0b0000000000010000
#define PXMIPSBranchCompareToRegiste    0b0000000000100000
#define PXMIPSBranchCommitToJump        0b0000000001000000


typedef struct PXMIPSBranch_
{
    void* Address;
    PXInt8U Mode;
    PXBool Likely;
    PXBool CompareToRegister;
    PXBool CommitJump;
}
PXMIPSBranch;


void PXAPI PXMIPSBranchCalc(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction, PXMIPSBranch* const pxMIPSBranch);



typedef struct PXMIPSJump_
{
    void* AddressVirual;
    void* AddressPhysical;
    PXBool PreserveReturnPoint;
}
PXMIPSJump;

void PXAPI PXMIPSJumpCalc(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction, PXMIPSJump* const pxMIPSJump);




typedef struct PXMIPSInstructionCoProcessor_
{
    PXInt8U ProcessorID;
}
PXMIPSInstructionCoProcessor;


void PXAPI PXMIPSInstructionCoProcessorCalc(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);




void PXAPI PXMIPSInstructionExecuteDeleay(PXMIPSProcessor* const pxMIPSProcessor);

//---------------------------------------------------------


PXPublic void PXAPI PXMIPSInitializize(PXMIPSProcessor* const pxMIPSProcessor, PXCodeSegment* const pxCodeSegment);

PXPublic const char* PXAPI PXMIPSInstructionTypeToStringShort(const PXMIPSInstructionType pxMIPSInstruction);
PXPublic const char* PXAPI PXMIPSInstructionTypeToStringLong(const PXMIPSInstructionType pxMIPSInstruction);

PXPublic void PXAPI PXMIPSInstructionExecute(PXMIPSProcessor* const pxMIPSProcessor);

PXPublic PXActionResult PXAPI PXMIPSTranslate(PXMIPSProcessor* const pxMIPSProcessor, const PXByte* const data, const PXSize length);



typedef enum PXMIPSMemoryRegion_
{
    PXMIPSMemoryRegionInvalid,
    PXMIPSMemoryRegionUnkown,
    PXMIPSMemoryRegionRDRAMRegistersHandler,
    PXMIPSMemoryRegionSPRegistersHandler,
    PXMIPSMemoryRegionDPCommandRegistersHandler,
    PXMIPSMemoryRegionMIPSInterfaceHandler,
    PXMIPSMemoryRegionVideoInterfaceHandler,
    PXMIPSMemoryRegionAudioInterfaceHandler,
    PXMIPSMemoryRegionPeripheralInterfaceHandler,
    PXMIPSMemoryRegionRDRAMInterfaceHandler,
    PXMIPSMemoryRegionSerialInterfaceHandler,
    PXMIPSMemoryRegionCartridgeDomain2Address1Handler,
    PXMIPSMemoryRegionCartridgeDomain1Address1Handler,
    PXMIPSMemoryRegionCartridgeDomain2Address2Handler,
    PXMIPSMemoryRegionISViewerHandler,
    PXMIPSMemoryRegionPIFRamHandler,
    PXMIPSMemoryRegionCartridgeDomain1Address3Handler
}
PXMIPSMemoryRegion;

PXPublic void* PXAPI PXMIPSTranslateVirtualAdress(PXMIPSProcessor* const pxMIPSProcessor, const PXSize virtualAdress);



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
PXPublic void PXAPI PXMIPSInstructionADDImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionADDImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSetOnLessThanImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSetOnLessThanImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionANDImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
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
PXPublic void PXAPI PXMIPSInstructionShiftLeftLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionShiftRightLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionShiftRightArithmetic(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionShiftLeftLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionShiftRightLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionShiftRightArithmeticVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionJumpRegister(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionJumpAndLinkRegister(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSystemCall(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBreak(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSynchronize(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveFromHI(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveToHI(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveFromLO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveToLO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftLeftLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftRightLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftRightArithmeticVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMultiply(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMultiplyUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDivide(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDivideUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordMultiply(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordMultiplyUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordDivide(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordDivideUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionADD(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionADDUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSubtract(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSubtractUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionAND(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionExclusiveOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionNOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSetOnLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionSetOnLessThanUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordADD(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordADDUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordSubtract(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordSubtractUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfLessThanUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfNotEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftLeftLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftRightLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftRightArithmetic(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftLeftLogicalPlus32(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordShiftRightArithmeticPlus32(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);



PXPublic void PXAPI PXMIPSInstructionBranchOnLessThanZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnLessThanZeroLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfLessThanImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfLessThanImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfEqualImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionTrapIfNotEqualImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnLessThanZeroAndLink(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLink(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnLessThanZeroAndLinkLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLinkLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);




PXPublic void PXAPI PXMIPSInstructionMoveWordFromFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordMoveFromSystemControlCoprocessor(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveControlWordFromCoprocessorZ(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveToSystemControlCoprocessor(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionDoublewordMoveToSystemControlCoprocessor(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
PXPublic void PXAPI PXMIPSInstructionMoveControlToCoprocessorZ(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction);
// BC





#endif
#include "PXMIPS.h"

#include <OS/Console/PXConsole.h>

void PXAPI PXMIPSInitializize(PXMIPSProcessor* const pxMIPSProcessor, PXCodeSegment* const pxCodeSegment)
{
    
}

const char* PXAPI PXMIPSInstructionTypeToStringShort(const PXMIPSInstructionType pxMIPSInstruction)
{
    switch(pxMIPSInstruction)
    { 
        case PXMIPSInstructionTypeOPCodeSpecial: return "SPECIAL";
        case PXMIPSInstructionTypeOPCodeREGIMM: return "REGIMM";
        case PXMIPSInstructionTypeJump: return "J";
        case PXMIPSInstructionTypeJumpAndLink: return "JAL";
        case PXMIPSInstructionTypeBranchOnEqual: return "BEQ";
        case PXMIPSInstructionTypeBranchOnNotEqual: return "BNE";
        case PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero: return "BLEZ";
        case PXMIPSInstructionTypeBranchOnGreaterThanZero: return "BGTZ";
        case PXMIPSInstructionTypeAddImmediate: return "ADDI";
        case PXMIPSInstructionTypeAddImmediateUnsigned: return "ADDIU";
        case PXMIPSInstructionTypeSetOnLessThanImmediate: return "SLTI";
        case PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned: return "SLTIU";
        case PXMIPSInstructionTypeAndImmediate: return "ANDI";
        case PXMIPSInstructionTypeORImmediate: return "ORI";
        case PXMIPSInstructionTypeExclusiveOrImmediate: return "XORI";
        case PXMIPSInstructionTypeLoadUpperImmediate: return "LUI";
        case PXMIPSInstructionTypeCoprocessorZOperation0: return "COP0";
        case PXMIPSInstructionTypeCoprocessorZOperation1: return "COP1";
        case PXMIPSInstructionTypeCoprocessorZOperation2: return "COP2";
        case PXMIPSInstructionTypeBranchOnEqualLikely: return "BEQL";
        case PXMIPSInstructionTypeBranchOnNotEqualLikely: return "BNEL";
        case PXMIPSInstructionTypeBranchOnLessThan: return "BLEZL";
        case PXMIPSInstructionTypeBranchOnGreaterThanZeroLikely: return "BGTZL";
        case PXMIPSInstructionTypeDoublewordADDImmediate: return "DADDI";
        case PXMIPSInstructionTypeDoublewordADDImmediateUnsigned: return "DADDIU";
        case PXMIPSInstructionTypeLoadDoublewordLeft: return "LDL";
        case PXMIPSInstructionTypeLoadDoublewordRight: return "LDR";
        case PXMIPSInstructionTypeLoadByte: return "LB";
        case PXMIPSInstructionTypeLoadHalfword: return "LH";
        case PXMIPSInstructionTypeLoadWordLeft: return "LWL";
        case PXMIPSInstructionTypeLoadWord: return "LW";
        case PXMIPSInstructionTypeLoadByteUnsigned: return "LBU";
        case PXMIPSInstructionTypeLoadHalfwordUnsigned: return "LHU";
        case PXMIPSInstructionTypeLoadWordRight: return "LWR";
        case PXMIPSInstructionTypeLoadWordUnsigned: return "LWU";
        case PXMIPSInstructionTypeStoreByte: return "SB";
        case PXMIPSInstructionTypeStoreHalfword: return "SH";
        case PXMIPSInstructionTypeStoreWordLeft: return "SWL";
        case PXMIPSInstructionTypeStoreWord: return "SW";
        case PXMIPSInstructionTypeStoreDoublewordLeft: return "SDL";
        case PXMIPSInstructionTypeStoreDoublewordRight: return "SDR";
        case PXMIPSInstructionTypeStoreWordRight: return "SWR";
        case PXMIPSInstructionTypeCacheOperation: return "CACHE";
        case PXMIPSInstructionTypeLoadLinked: return "LL";
        case PXMIPSInstructionTypeLoadWordToFPUCoprocessor1: return "LWC1";
        case PXMIPSInstructionTypeLoadWordToFPUCoprocessor2: return "LWC2";
        case PXMIPSInstructionTypeLoadLinkedDoubleword: return "LLD";
        case PXMIPSInstructionTypeLoadDoublewordToFPUCoprocessor1: return "LDC1";
        case PXMIPSInstructionTypeLoadDoublewordToFPUCoprocessor2: return "LDC2";
        case PXMIPSInstructionTypeLoadDoubleword: return "LD";
        case PXMIPSInstructionTypeStoreConditional: return "SC";
        case PXMIPSInstructionTypeStoreWordFromFPUCoprocessor1: return "SWC1";
        case PXMIPSInstructionTypeStoreWordFromFPUCoprocessor2: return "SEC2";
        case PXMIPSInstructionTypeStoreConditionalDoubleword: return "SCD";
        case PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor1: return "SCD1";
        case  PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor2: return "SCD2";
        case   PXMIPSInstructionTypeStoreDoubleword: return "SD";
        case  PXMIPSInstructionTypeShiftLeftLogical: return "SLL";
        case PXMIPSInstructionTypeShiftRightLogical: return "SRL";
        case  PXMIPSInstructionTypeShiftRightArithmetic: return "SRA";
        case    PXMIPSInstructionTypeShiftLeftLogicalVariable: return "SLLV";
        case   PXMIPSInstructionTypeShiftRightLogicalVariable: return "SRLV";
        case   PXMIPSInstructionTypeShiftRightArithmeticVariable: return "SRAV";
        case   PXMIPSInstructionTypeJumpRegister: return "JR";
        case   PXMIPSInstructionTypeJumpAndLinkRegister: return "JALR";
        case  PXMIPSInstructionTypeSystemCall: return "SYSCALL";
        case    PXMIPSInstructionTypeBreak: return "BREAK";
        case   PXMIPSInstructionTypeSynchronize: return "SYNC";
        case   PXMIPSInstructionTypeMoveFromHI: return "MFHI";
        case   PXMIPSInstructionTypeMoveToHI: return "MTHI";
        case   PXMIPSInstructionTypeMoveFromLO: return "MFLO";
        case   PXMIPSInstructionTypeMoveToLO: return "MTLO";
        case   PXMIPSInstructionTypeDoublewordShiftLeftLogicalVariable: return "DSLLV";
        case   PXMIPSInstructionTypeDoublewordShiftRightLogicalVariable: return "DSRLV";
        case   PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable: return "DSRAV";
        case   PXMIPSInstructionTypeMultiply: return "MULT";
        case   PXMIPSInstructionTypeMultiplyUnsigned: return "MULTU";
        case   PXMIPSInstructionTypeDivide: return "DIV";
        case    PXMIPSInstructionTypeDivideUnsigned: return "DIVU";
        case  PXMIPSInstructionTypeDoublewordMultiply: return "DMULT";
        case   PXMIPSInstructionTypeDoublewordMultiplyUnsigned: return "DMULTU";
        case   PXMIPSInstructionTypeDoublewordDivide: return "DDIV";
        case   PXMIPSInstructionTypeDoublewordDivideUnsigned: return "DDIVU";
        case   PXMIPSInstructionTypeAdd: return "ADD";
        case  PXMIPSInstructionTypeAddUnsigned: return "ADDU";
        case    PXMIPSInstructionTypeSubtract: return "SUB";
        case   PXMIPSInstructionTypeSubtractUnsigned: return "SUBU";
        case   PXMIPSInstructionTypeAND: return "AND";
        case   PXMIPSInstructionTypeOR: return "OR";
        case   PXMIPSInstructionTypeExclusiveOR: return "XOR";
        case   PXMIPSInstructionTypeNOR: return "NOR";
        case   PXMIPSInstructionTypeSetOnLessThan: return "SLT";
        case   PXMIPSInstructionTypeSetOnLessThanUnsigned: return "SLTU";
        case   PXMIPSInstructionTypeDoublewordADD: return "DADD";
        case   PXMIPSInstructionTypeDoublewordADDUnsigned: return "DADDU";
        case   PXMIPSInstructionTypeDoublewordSubtract: return "DSUB";
        case   PXMIPSInstructionTypeDoublewordSubtractUnsigned: return "DSUBU";
        case   PXMIPSInstructionTypeTrapIfGreaterThanOrEqual: return "TGE";
        case   PXMIPSInstructionTypeTrapIfGreaterThanOrEqualUnsigned: return "TGEU";
        case   PXMIPSInstructionTypeTrapIfLessThan: return "TLT";
        case   PXMIPSInstructionTypeTrapIfLessThanUnsigned: return "TLTU";
        case    PXMIPSInstructionTypeTrapIfEqual: return "TEQ";
        case   PXMIPSInstructionTypeTrapIfNotEqual: return "TNE";
        case   PXMIPSInstructionTypeDoublewordShiftLeftLogical: return "DSLL";
        case   PXMIPSInstructionTypeDoublewordShiftRightLogical: return "DSRL";
        case   PXMIPSInstructionTypeDoublewordShiftRightArithmetic: return "DSRA";
        case   PXMIPSInstructionTypeDoublewordShiftLeftLogicalPlus32: return "DSLL32";
        case   PXMIPSInstructionTypeDoublewordShiftRight: return "DSRL32";
        case    PXMIPSInstructionTypeDoublewordShiftRightArithmeticPlus32: return "DSRA32";
        case   PXMIPSInstructionTypeBranchOnLessThanZero: return "BLTZ";
        case   PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZero: return "BGEZ";
        case    PXMIPSInstructionTypeBranchOnLessThanZeroLikely: return "BLTZL";
        case   PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroLikely: return "BGEZL";
        case   PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediate: return "TGEI";
        case   PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediateUnsigned: return "TGEIU";
        case   PXMIPSInstructionTypeTrapIfLessThanImmediate: return "TLTI";
        case  PXMIPSInstructionTypeTrapIfLessThanImmediateUnsigned: return "TLTIU";
        case   PXMIPSInstructionTypeTrapIfEqualImmediate: return "TEQI";
        case   PXMIPSInstructionTypeTrapIfNotEqualImmediate: return "TNEI";
        case   PXMIPSInstructionTypeBranchOnLessThanZeroAndLink: return "BLTZAL";
        case   PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLink: return "BGEZAL";
        case   PXMIPSInstructionTypeBranchOnLessThanZeroAndLinkLikely: return "BLTZALL";
        case   PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLinkLikely: return "BGEZALL";



        case PXMIPSInstructionTypeMoveWordFromFPUCoprocessor1: return "MFC0";
        case PXMIPSInstructionTypeDoublewordMoveFromSystemControlCoprocessor: return "DMFC0";
            // CF
            // ----
        case PXMIPSInstructionTypeMoveToSystemControlCoprocessor: return "MTC0";
        case PXMIPSInstructionTypeDoublewordMoveToSystemControlCoprocessor: return "DMTC0";
            // CT
            // ----
            // BC

        default:
            return PXNull;
    }
}

const char* PXAPI PXMIPSInstructionTypeToStringLong(const PXMIPSInstructionType pxMIPSInstruction)
{
    switch(pxMIPSInstruction)
    {
        case PXMIPSInstructionTypeJump: return "Jump";
        case PXMIPSInstructionTypeJumpAndLink: return "Jump And Link";
        case PXMIPSInstructionTypeBranchOnEqual: return "Branch On Equal";
        case PXMIPSInstructionTypeBranchOnNotEqual: return "Branch On Not Equal";
        case PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero: return "Branch On Less Than Or Equal To Zero";
        case PXMIPSInstructionTypeBranchOnGreaterThanZero: return "Branch On Greater Than Zero";
        case PXMIPSInstructionTypeAddImmediate: return "Add Immediate";
        case PXMIPSInstructionTypeAddImmediateUnsigned: return "Add Immediate Unsigned";
        case PXMIPSInstructionTypeSetOnLessThanImmediate: return "Set On Less Than Immediate";
        case PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned: return "Set On Less Than Immediate Unsigned";
        case PXMIPSInstructionTypeAndImmediate: return "And Immediate";
        case PXMIPSInstructionTypeORImmediate: return "OR Immediate";
        case PXMIPSInstructionTypeExclusiveOrImmediate: return "Exclusive Or Immediate";
        case PXMIPSInstructionTypeLoadUpperImmediate: return "Load Upper Immediate";


        case PXMIPSInstructionTypeStoreWord: return "Store Word";
        case PXMIPSInstructionTypeStoreWordLeft: return "Store Word Left";    
        case PXMIPSInstructionTypeBranchOnNotEqualLikely: return "Branch On Not Equal Likely";
        case PXMIPSInstructionTypeJumpRegister: return "Jump Register";

        default:
            return PXNull;
    }
}


void PXMIPSInstructionPrint(PXMIPSTInstruction* const pxMIPSTInstruction)
{
#if PXLogEnable
    const char* instuctionNameShort = PXMIPSInstructionTypeToStringShort(pxMIPSTInstruction->Type);
    const char* instuctionNameLong = PXMIPSInstructionTypeToStringLong(pxMIPSTInstruction->Type);

    PXLogPrint
    (
        PXLoggingAllocation,
        "MIPS",
        "Instruction",
        "%8.8X   %2.2X %2.2X %2.2X %2.2X   RS:%2.2X RT:%2.2X IMM:%4.4X   %2.2X:%-7s %s",
        (int)pxMIPSTInstruction->AdressVirtual,    
        pxMIPSTInstruction->Adress[0],
        pxMIPSTInstruction->Adress[1],
        pxMIPSTInstruction->Adress[2],
        pxMIPSTInstruction->Adress[3],
        pxMIPSTInstruction->RS,
        pxMIPSTInstruction->RT,
        pxMIPSTInstruction->Immediate,
        pxMIPSTInstruction->Type,
        instuctionNameShort,
        instuctionNameLong

    );
#endif
}



PXActionResult PXAPI PXMIPSTranslate(PXMIPSProcessor* const pxMIPSProcessor, const PXByte* const data, const PXSize length)
{
    //PXClear(PXMIPSProcessor, pxMIPSProcessor);
    DWORD protectIDOld = 0;
    DWORD protectIDNew = PAGE_READONLY;
    const PXBool succ = VirtualProtect(data, 0x0007dac0, protectIDNew, &protectIDOld);

    void* programmCounter = data;

    PXBool jumped = 0;

    pxMIPSProcessor->ProgramCounter = 0;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Execute",
        "--- Start ---"
    );
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Execute",
        "Begin at 0x%p, Size %i",
        data,
        length
    );
#endif


    const PXMIPSTInstructionFunction instructionGeneralLookup[64] =
    {
        PXMIPSInstructionSpecial,
        PXMIPSInstructionREGIMM,
        PXMIPSInstructionJump,
        PXMIPSInstructionJumpAndLink,
        PXMIPSInstructionBranchOnEqual,
        PXMIPSInstructionBranchOnNotEqual,
        PXMIPSInstructionBranchOnLessThanOrEqualToZero,
        PXMIPSInstructionBranchOnGreaterThanZero,
        PXMIPSInstructionAddImmediate,
        PXMIPSInstructionAddImmediateUnsigned,
        PXMIPSInstructionSetOnLessThanImmediate,
        PXMIPSInstructionSetOnLessThanImmediateUnsigned,
        PXMIPSInstructionAndImmediate,
        PXMIPSInstructionORImmediate,
        PXMIPSInstructionExclusiveOrImmediate,
        PXMIPSInstructionLoadUpperImmediate,
        PXMIPSInstructionCoprocessorZOperation0,
        PXMIPSInstructionCoprocessorZOperation1,
        PXMIPSInstructionCoprocessorZOperation2,
        PXMIPSInstructionReserved,
        PXMIPSInstructionBranchOnEqualLikely,
        PXMIPSInstructionBranchOnNotEqualLikely,
        PXMIPSInstructionBranchOnLessThan,
        PXMIPSInstructionBranchOnGreaterThanZeroLikely,
        PXMIPSInstructionDoublewordADDImmediate,
        PXMIPSInstructionDoublewordADDImmediateUnsigned,
        PXMIPSInstructionLoadDoublewordLeft,
        PXMIPSInstructionLoadDoublewordRight,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionLoadByte,
        PXMIPSInstructionLoadHalfword,
        PXMIPSInstructionLoadWordLeft,
        PXMIPSInstructionLoadWord,
        PXMIPSInstructionLoadByteUnsigned,
        PXMIPSInstructionLoadHalfwordUnsigned,
        PXMIPSInstructionLoadWordRight,
        PXMIPSInstructionLoadWordUnsigned,
        PXMIPSInstructionStoreByte,
        PXMIPSInstructionStoreHalfword,
        PXMIPSInstructionStoreWordLeft,
        PXMIPSInstructionStoreWord,
        PXMIPSInstructionStoreDoublewordLeft,
        PXMIPSInstructionStoreDoublewordRight,
        PXMIPSInstructionStoreWordRight,
        PXMIPSInstructionCacheOperation,
        PXMIPSInstructionLoadLinked,
        PXMIPSInstructionLoadWordToFPUCoprocessor1,
        PXMIPSInstructionLoadWordToFPUCoprocessor2,
        PXMIPSInstructionReserved,
        PXMIPSInstructionLoadLinkedDoubleword,
        PXMIPSInstructionLoadDoublewordToFPUCoprocessor1,
        PXMIPSInstructionLoadDoublewordToFPUCoprocessor2,
        PXMIPSInstructionLoadDoubleword,
        PXMIPSInstructionStoreConditional,
        PXMIPSInstructionStoreWordFromFPUCoprocessor1,
        PXMIPSInstructionStoreWordFromFPUCoprocessor2,
        PXMIPSInstructionReserved,
        PXMIPSInstructionStoreConditionalDoubleword,
        PXMIPSInstructionStoreDoublewordFromFPUCoprocessor1,
        PXMIPSInstructionStoreDoublewordFromFPUCoprocessor2,
        PXMIPSInstructionStoreDoubleword,
    };

    const PXMIPSTInstructionFunction instructionSpecialLookup[64] =
    {
        PXMIPSInstructionShiftLeftLogical,
        PXMIPSInstructionReserved,
        PXMIPSInstructionShiftRightLogical,
        PXMIPSInstructionShiftRightArithmetic,
        PXMIPSInstructionShiftLeftLogicalVariable,
        PXMIPSInstructionReserved,
        PXMIPSInstructionShiftRightLogicalVariable,
        PXMIPSInstructionShiftRightArithmeticVariable,
        PXMIPSInstructionJumpRegister,
        PXMIPSInstructionJumpAndLinkRegister,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionSystemCall,
        PXMIPSInstructionBreak,
        PXMIPSInstructionReserved,
        PXMIPSInstructionSynchronize,
        PXMIPSInstructionMoveFromHI,
        PXMIPSInstructionMoveToHI,
        PXMIPSInstructionMoveFromLO,
        PXMIPSInstructionMoveToLO,
        PXMIPSInstructionDoublewordShiftLeftLogicalVariable,
        PXMIPSInstructionReserved,
        PXMIPSInstructionDoublewordShiftRightLogicalVariable,
        PXMIPSInstructionDoublewordShiftRightArithmeticVariable,
        PXMIPSInstructionMultiply,
        PXMIPSInstructionMultiplyUnsigned,
        PXMIPSInstructionDivide,
        PXMIPSInstructionDivideUnsigned,
        PXMIPSInstructionDoublewordMultiply,
        PXMIPSInstructionDoublewordMultiplyUnsigned,
        PXMIPSInstructionDoublewordDivide,
        PXMIPSInstructionDoublewordDivideUnsigned,
        PXMIPSInstructionAdd,
        PXMIPSInstructionAddUnsigned,
        PXMIPSInstructionSubtract,
        PXMIPSInstructionSubtractUnsigned,
        PXMIPSInstructionAND,
        PXMIPSInstructionOR,
        PXMIPSInstructionExclusiveOR,
        PXMIPSInstructionNOR,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionSetOnLessThan,
        PXMIPSInstructionSetOnLessThanUnsigned,
        PXMIPSInstructionDoublewordADD,
        PXMIPSInstructionDoublewordADDUnsigned,
        PXMIPSInstructionDoublewordSubtract,
        PXMIPSInstructionDoublewordSubtractUnsigned,
        PXMIPSInstructionTrapIfGreaterThanOrEqual,
        PXMIPSInstructionTrapIfGreaterThanOrEqualUnsigned,
        PXMIPSInstructionTrapIfLessThan,
        PXMIPSInstructionTrapIfLessThanUnsigned,
        PXMIPSInstructionTrapIfEqual,
        PXMIPSInstructionReserved,
        PXMIPSInstructionTrapIfNotEqual,
        PXMIPSInstructionReserved,
        PXMIPSInstructionDoublewordShiftLeftLogical,
        PXMIPSInstructionReserved,
        PXMIPSInstructionDoublewordShiftRightLogical,
        PXMIPSInstructionDoublewordShiftRightArithmetic,
        PXMIPSInstructionDoublewordShiftLeftLogicalPlus32,
        PXMIPSInstructionReserved,
        PXMIPSInstructionDoublewordShiftRight,
        PXMIPSInstructionDoublewordShiftRightArithmeticPlus32
    };

    const PXMIPSTInstructionFunction instructionRegimmLookup[32] =
    {
        PXMIPSInstructionBranchOnLessThanZero ,
        PXMIPSInstructionBranchOnGreaterThanOrEqualToZero ,
    PXMIPSInstructionBranchOnLessThanZeroLikely ,
    PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroLikely,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionTrapIfGreaterThanOrEqualImmediate ,
    PXMIPSInstructionTrapIfGreaterThanOrEqualImmediateUnsigned ,
    PXMIPSInstructionTrapIfLessThanImmediate,
    PXMIPSInstructionTrapIfLessThanImmediateUnsigned ,
    PXMIPSInstructionTrapIfEqualImmediate,
    PXMIPSInstructionReserved,
    PXMIPSInstructionTrapIfNotEqualImmediate ,
    PXMIPSInstructionReserved,
    PXMIPSInstructionBranchOnLessThanZeroAndLink ,
    PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLink ,
    PXMIPSInstructionBranchOnLessThanZeroAndLinkLikely ,
    PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLinkLikely ,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved,
    PXMIPSInstructionReserved
    };

    const PXMIPSTInstructionFunction instructioncoprocLookup[] = 
    {
        PXMIPSInstructionMoveWordFromFPUCoprocessor1,
        PXMIPSInstructionDoublewordMoveFromSystemControlCoprocessor,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionMoveToSystemControlCoprocessor ,
        PXMIPSInstructionDoublewordMoveToSystemControlCoprocessor ,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved
    };

    PXMIPSTInstruction pxMIPSTInstruction;
    pxMIPSTInstruction.GeneralInstructionList = instructionGeneralLookup;
    pxMIPSTInstruction.SpecialInstructionList = instructionSpecialLookup;
    pxMIPSTInstruction.REGIMMInstructionList = instructionRegimmLookup;
    pxMIPSTInstruction.CorpocessorList = instructioncoprocLookup;

    for(;;)
    {        
        pxMIPSTInstruction.IncrmentCounter = PXTrue;
        pxMIPSTInstruction.Adress = (PXInt8U*)programmCounter + pxMIPSProcessor->ProgramCounter;
        pxMIPSTInstruction.AdressVirtual = (PXSize)pxMIPSProcessor->ROMOffset + pxMIPSProcessor->ProgramCounter;

        // instruction is big endian, to use our bit extraction, we need to swap it

        pxMIPSTInstruction.Value = PXInt32Make(pxMIPSTInstruction.Adress[3], pxMIPSTInstruction.Adress[2], pxMIPSTInstruction.Adress[1], pxMIPSTInstruction.Adress[0]);

        if(pxMIPSTInstruction.Value == 0)
        {
            break;
        }

        pxMIPSTInstruction.Type         = (pxMIPSTInstruction.Value & 0b11111100000000000000000000000000) >> 26;
        pxMIPSTInstruction.RS           = (pxMIPSTInstruction.Value & 0b00000011111000000000000000000000) >> 21;
        pxMIPSTInstruction.RT           = (pxMIPSTInstruction.Value & 0b00000000000111110000000000000000) >> 16;
        pxMIPSTInstruction.Immediate    = (pxMIPSTInstruction.Value & 0b00000000000000001111111111111111) >> 0;

        const PXMIPSTInstructionFunction instructionFunction = instructionGeneralLookup[pxMIPSTInstruction.Type]; // Direct ID lookup

#if PXLogEnable

        PXBool noprint =
            instructionFunction == PXMIPSInstructionAddImmediate ||
            instructionFunction == PXMIPSInstructionStoreWord ||
            instructionFunction == PXMIPSInstructionBranchOnNotEqual;

        if(!noprint)
        {
            PXMIPSInstructionPrint(&pxMIPSTInstruction);
        }

#endif

        instructionFunction(pxMIPSProcessor, &pxMIPSTInstruction); // Call function

        if(!pxMIPSTInstruction.IncrmentCounter)
        {
            jumped = 1;
        }


        // Next command
        // MIPS always has 4-Byte commands
        pxMIPSProcessor->ProgramCounter += (pxMIPSTInstruction.IncrmentCounter * 4);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Execute",
        "--- Done ---"
    );
#endif

    return PXActionSuccessful;
}

void PXAPI PXMIPSInstructionReserved(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        "MIPS",
        "Instruction",
        "Illegal or not implemented!"
    );
#endif
    DebugBreak();
}

void PXAPI PXMIPSInstructionSpecial(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    const PXInt8U instructionID = pxMIPSTInstruction->Immediate & 0b111111;

    pxMIPSTInstruction->Type = instructionID | PXMIPSOPCodeSpecial;

    PXMIPSInstructionPrint(pxMIPSTInstruction);

    const PXMIPSTInstructionFunction specialinstructionFunction = pxMIPSTInstruction->SpecialInstructionList[instructionID]; // Direct ID lookup

    specialinstructionFunction(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionREGIMM(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    const PXInt8U instructionID = pxMIPSTInstruction->RT;

    pxMIPSTInstruction->Type = instructionID | PXMIPSOPCodeREGIMM;

    const PXMIPSTInstructionFunction specialinstructionFunction = pxMIPSTInstruction->REGIMMInstructionList[instructionID]; // Direct ID lookup

    specialinstructionFunction(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionJump(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
   
}

void PXAPI PXMIPSInstructionJumpAndLink(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    /*
                The 26-bit target is shifted left two bits and combined with the high-order four bits
                of the address of the delay slot to calculate the address. The program
                unconditionally jumps to this calculated address with a delay of one instruction.
                The address of the instruction after the delay slot is placed in the link register, r31.
                */
    const PXSize vAdress = pxMIPSProcessor->ProgramCounter + (PXSize)pxMIPSProcessor->ROMOffset;

    PXSize functionAdress = pxMIPSTInstruction->Value & 0b00000011111111111111111111111111;
    functionAdress <<= 2; // Shift by 2
    functionAdress |= vAdress & 0b11110000000000000000000000000000; // Highest 4 bits

    PXSize offset = functionAdress - (PXSize)pxMIPSProcessor->ROMOffset;


    pxMIPSProcessor->Register[PXMIPSProcessorLinkAddress] = vAdress + 4; // Next instruction


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Jump And Link",
        "Jump to %8.8X (offset:%i)",
        functionAdress,
        offset
    );
#endif


    // Jump
    pxMIPSProcessor->ProgramCounter = offset;

    pxMIPSTInstruction->IncrmentCounter = PXFalse; // Skip the +4 offset
}

void PXAPI PXMIPSInstructionBranchOnEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBranchOnNotEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    /*
                A branch address is calculated from the sum of the address of the instruction in the
                delay slot and the 16-bit offset, shifted two bits left and sign-extended. The
                contents of general purpose register rs and the contents of general purpose register
                rt are compared. If the two registers are not equal, then the program branches to
                the branch address, with a delay of one instruction.
                */

    PXInt64S offset = ((PXInt16S)pxMIPSTInstruction->Immediate) << 2;
    PXBool notEqualRSRT = pxMIPSProcessor->Register[pxMIPSTInstruction->RS] != pxMIPSProcessor->Register[pxMIPSTInstruction->RT];

    if(notEqualRSRT)
    {
        // delay of oone instruction?

#if PXLogEnable && 0
        PXLogPrint
        (
            PXLoggingInfo,
            "MIPS",
            "Jump NEQ",
            "%8.8X vs %4.4X - Jump to: %i",
            pxMIPSProcessor->Register[pxMIPSTInstruction->RS],
            pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
            offset
        );
#endif

        pxMIPSProcessor->ProgramCounter += offset;

        //continue;
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "MIPS",
            "Jump NEQ",
            "No jump. Value equal %8.8X",
            pxMIPSProcessor->Register[pxMIPSTInstruction->RS]
        );
#endif
    }
}

void PXAPI PXMIPSInstructionBranchOnLessThanOrEqualToZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBranchOnGreaterThanZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionAddImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    /*
                The 16-bit immediate is sign-extended and added to the contents of general
                purpose register rs to store the result in general purpose register rt. In 64-bit
                mode, the operand must be sign-extended, 32-bit values.
                An integer overflow exception occurs if carries out of bits 30 and 31 differ (2’s
                complement overflow). The contents of destination register rt is not modified
                when an integer overflow exception occurs
                */

    const PXInt64S value = (PXInt16S)pxMIPSTInstruction->Immediate;


#if PXLogEnable
    const PXInt64S rtBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RT];
    const PXInt64S rsBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RS];
#endif

    const PXInt64S resukt =
        (PXInt64U)pxMIPSProcessor->Register[pxMIPSTInstruction->RS] +
        (PXInt16S)pxMIPSTInstruction->Immediate;

    pxMIPSProcessor->Register[pxMIPSTInstruction->RS] = resukt;
    pxMIPSProcessor->Register[pxMIPSTInstruction->RT] = resukt;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Add Signed",
        "\n"
        "%16.8X = %16.8X + %16.8X Before:%16.8X\n"
        "%16i = %16i + %16i Before:%16i",
        resukt,
        rsBefore,
        value,
        rtBefore,
        resukt,
        rsBefore,
        value,
        rtBefore
    );
#endif
}

void PXAPI PXMIPSInstructionAddImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    /*
The 16-bit immediate is sign-extended and added to the contents of general
purpose register rs to store the result in general purpose register rt. No integer
overflow exception occurs under any circumstance. In 64-bit mode, the operand
must be sign-extended, 32-bit values.
The only difference between this instruction and the ADDI instruction is that
ADDIU instruction never causes an integer overflow exception.
*/

    const PXInt64U value = (PXInt16U)pxMIPSTInstruction->Immediate;

#if PXLogEnable
    const PXSize rsBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RS];
    const PXSize rtBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RT];
#endif

    //PXSize result = pxMIPSProcessor->Register[pxMIPSTInstruction->RT] + value; // Calc and store into rs
    //*target += pxMIPSProcessor->Register[pxMIPSTInstruction.RS]; // copy rs to rt


    pxMIPSProcessor->Register[pxMIPSTInstruction->RS] += value; // Add Immediate to RS
    pxMIPSProcessor->Register[pxMIPSTInstruction->RT] = pxMIPSProcessor->Register[pxMIPSTInstruction->RS]; // Store RS in RT

#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Add Unsigned",
        "\n"
        "%16.8X = %16.8X + %16.8X Before:%16.8X\n"
        "%16i = %16i + %16i Before:%16i",
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        rsBefore,
        value,
        rtBefore,
        (PXInt64S)pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        (PXInt64S)rsBefore,
        value,
        rtBefore
    );
#endif
}

void PXAPI PXMIPSInstructionSetOnLessThanImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSetOnLessThanImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionAndImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionORImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    /*
A logical OR operation applied between 16-bit zero-extended immediate and the
contents of general purpose register rs is executed in bit units. The result is stored
in general purpose register rt
*/

#if PXLogEnable
    const PXSize rtBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RT];
    const PXSize rsBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RS];
#endif

    pxMIPSProcessor->Register[pxMIPSTInstruction->RS] = pxMIPSProcessor->Register[pxMIPSTInstruction->RT];
    pxMIPSProcessor->Register[pxMIPSTInstruction->RS] |= (PXInt64U)pxMIPSTInstruction->Immediate;
    pxMIPSProcessor->Register[pxMIPSTInstruction->RT] = pxMIPSProcessor->Register[pxMIPSTInstruction->RS];

#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "OR",
        "\n"
        "%16.8X = %16.8X | %16.8X Before:%16.8X\n"
        "%16i = %16i | %16i Before:%16i",
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        rsBefore,
        (PXInt64U)pxMIPSTInstruction->Immediate,
        rtBefore,
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        rsBefore,
        (PXInt64U)pxMIPSTInstruction->Immediate,
        rtBefore
    );
#endif
}

void PXAPI PXMIPSInstructionExclusiveOrImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadUpperImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    PXBool rsValid = 0 == pxMIPSTInstruction->RS;

    /*
    The 16-bit immediate is shifted left 16 bits and combined to 16 bits of zeros. The
    result is placed into general purpose register rt.
    In 64-bit mode, the loaded word    is sign-extended to 64 bits.
    */

    PXInt64U value = (PXInt16U)pxMIPSTInstruction->Immediate;

    value <<= 16;


#if PXLogEnable
    PXSize valueBefore = pxMIPSProcessor->Register[pxMIPSTInstruction->RT];
#endif

    pxMIPSProcessor->Register[pxMIPSTInstruction->RT] |= value;

#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Load",
        "\n"
        "%16.8X = %16.8X + (%8.4X << 16) Before:%8.8X\n"
        "%16i = %16i + (%8i << 16) Before:%4i",
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        pxMIPSProcessor->Register[pxMIPSTInstruction->RS],
        pxMIPSTInstruction->Immediate,
        valueBefore,
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        pxMIPSProcessor->Register[pxMIPSTInstruction->RS],
        pxMIPSTInstruction->Immediate,
        valueBefore
    );
#endif
}

void PXAPI PXMIPSInstructionCoprocessorZOperation0(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    const PXBool instructionID = pxMIPSTInstruction->RS;
    const PXMIPSTInstructionFunction instuction = pxMIPSTInstruction->CorpocessorList[instructionID];

    pxMIPSTInstruction->Type = instructionID | PXMIPSOPCodeCOPz; 
    pxMIPSTInstruction->FS = (pxMIPSTInstruction->Immediate & 0b1111100000000000) >> 11;

    PXMIPSInstructionPrint(pxMIPSTInstruction);

    instuction(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionCoprocessorZOperation1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionCoprocessorZOperation2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBranchOnEqualLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBranchOnNotEqualLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBranchOnLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBranchOnGreaterThanZeroLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordADDImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordADDImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadDoublewordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadDoublewordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadByte(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadHalfword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadWordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadWord(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadByteUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadHalfwordUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadWordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadWordUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreByte(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreHalfword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreWordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreWord(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    /*
                The 16-bit offset is sign-extended and added to the contents of general purpose
                register base to form a virtual address. The contents of general purpose register rt
                are stored in the memory location specified by the address. If either of the low-
                order two bits of the address are not zero, an address error exception occurs.
                */
    const PXInt64U base = pxMIPSProcessor->Register[pxMIPSTInstruction->RS];
    const PXInt64S offset = (PXInt16S)pxMIPSTInstruction->Immediate;
    const PXSize virtualAdress = base + offset;

    PXSize realAdress = virtualAdress - (PXSize)pxMIPSProcessor->ROMOffset;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "StoreWord",
        "Value:%8.8X -> %8.8X - Base:%8.8X + Offset:%i",
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        virtualAdress,
        base,
        offset
    );
#endif

    pxMIPSProcessor->Register[pxMIPSTInstruction->RS] += 0x02;


    //    pxMIPSProcessor->Stack[realAdress] = pxMIPSProcessor->Register[pxMIPSTInstruction.RT];

    //void* adress = (PXInt8U*)programmCounter + realAdress;

   // *((PXInt32U*)adress) = pxMIPSProcessor->Register[pxMIPSTInstruction->RT];
}

void PXAPI PXMIPSInstructionStoreDoublewordLeft(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreDoublewordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreWordRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionCacheOperation(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadLinked(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadWordToFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadWordToFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadLinkedDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadDoublewordToFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadDoublewordToFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionLoadDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreConditional(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreWordFromFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreWordFromFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreConditionalDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreDoublewordFromFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreDoublewordFromFPUCoprocessor2(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionStoreDoubleword(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionShiftLeftLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionShiftRightLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionShiftRightArithmetic(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionShiftLeftLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionShiftRightLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionShiftRightArithmeticVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionJumpRegister(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    const PXSize vadress = pxMIPSProcessor->Register[pxMIPSTInstruction->RS] - 0x10000; // Why ??
    
    const PXSize validAdress = 0 == (0b11 & vadress); // Must be word alligned!
    
    if(!validAdress)
    {
        DebugBreak();
    }

    PXSize offset = vadress - (PXSize)pxMIPSProcessor->ROMOffset;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "MIPS",
        "Jump Register",
        "Jump to %8.8X (offset:%i)",
        vadress,
        offset
    );
#endif
    pxMIPSProcessor->ProgramCounter = offset;

    // jumped = 1;

    pxMIPSTInstruction->IncrmentCounter = PXFalse; // Skip the +4 offset
}

void PXAPI PXMIPSInstructionJumpAndLinkRegister(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSystemCall(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionBreak(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSynchronize(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionMoveFromHI(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionMoveToHI(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionMoveFromLO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionMoveToLO(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftLeftLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftRightLogicalVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftRightArithmeticVariable(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionMultiply(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionMultiplyUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDivide(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDivideUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordMultiply(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordMultiplyUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordDivide(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordDivideUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionAdd(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionAddUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSubtract(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSubtractUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionAND(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionExclusiveOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionNOR(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSetOnLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionSetOnLessThanUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordADD(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordADDUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordSubtract(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordSubtractUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionTrapIfLessThan(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionTrapIfLessThanUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionTrapIfEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionTrapIfNotEqual(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftLeftLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftRightLogical(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftRightArithmetic(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftLeftLogicalPlus32(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftRight(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

void PXAPI PXMIPSInstructionDoublewordShiftRightArithmeticPlus32(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{

}

 void PXAPI PXMIPSInstructionBranchOnLessThanZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZero(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZeroLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionTrapIfLessThanImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionTrapIfLessThanImmediateUnsigned(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionTrapIfEqualImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionTrapIfNotEqualImmediate(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZeroAndLink(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLink(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZeroAndLinkLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLinkLikely(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
{
    
}

 void PXAPI PXMIPSInstructionMoveWordFromFPUCoprocessor1(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
 {
  

    /*
    The contents of floating-point general purpose register fs are stored to the general purpose register rt of the CPU register rt.
    The contents of general purpose register rt are undefined while the instruction
    immediately following this instruction is being executed.
    If the FR bit of the Status register is 0 and if the least-significant bit in the ft field
    is 0, the low-order 32 bits of floating-point register ft are stored to the general
    purpose register rt. If the least-significant bit in the ft area is 1, the high-order 32
    bits of floating-point register ft-1 are stored to the general purpose register rt.
    If the FR bit is 1, all the 64-bit floating-point registers can be accessed; therefore,
    the low-order 32 bits of floating-point register ft are stored to the general purpose register rt.
    */

     // Check Coprocessor and throw "unusable" as an exception it it is not.


     float* source = &pxMIPSProcessor->RegisterFloat[pxMIPSTInstruction->FS];
     float* target = &pxMIPSProcessor->Register[pxMIPSTInstruction->RT];

     // As we store a float inside a normal int space, the value is garbage until read as an float.
     // As a float is 4 byte, the rest 4 byte of the 64-bit register is undefined.
     *target = *source; 




     const PXBool a = 0 == (PXMIPSRegisterStatusFR & pxMIPSProcessor->RegisterStatus);
     const PXBool b = 0 == (0b01 == pxMIPSProcessor->RegisterFloat[PXMIPSRegisterFT]);


#if PXLogEnable && 1
     PXLogPrint
     (
         PXLoggingInfo,
         "MIPS",
         "Coprocessor-1",
         "--"
     );
#endif

 }

 void PXAPI PXMIPSInstructionDoublewordMoveFromSystemControlCoprocessor(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
 {
    
 }

 void PXAPI PXMIPSInstructionMoveToSystemControlCoprocessor(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
 {
   
 }

 void PXAPI PXMIPSInstructionDoublewordMoveToSystemControlCoprocessor(PXMIPSProcessor* const pxMIPSProcessor, PXMIPSTInstruction* const pxMIPSTInstruction)
 {
    
 }
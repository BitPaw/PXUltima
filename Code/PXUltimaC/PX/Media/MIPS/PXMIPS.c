#include "PXMIPS.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/PXOS.h>

const char PXMIPSProcessorName[] = "MIPS-VR43xx";

void PXAPI PXMIPSBranchCalc(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction, PXMIPSBranch PXREF pxMIPSBranch)
{
    PXSize res = (PXI64S)((PXI16S)pxMIPSTInstruction->Immediate) << 2;

    pxMIPSBranch->Address = (void*)res;


#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "BranchCalc",
        "%8.8X vs %4.4X - Jump to: %i",
        pxMIPSProcessor->Register[pxMIPSTInstruction->RS],
        pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
        offset
    );
#endif



    // Check condition
    {
        switch(pxMIPSBranch->Mode)
        {
            case PXMIPSBranchEqual:
            {
                pxMIPSBranch->CommitJump = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] == pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
                break;
            }
            case PXMIPSBranchNotEqual:
            {
                pxMIPSBranch->CommitJump = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] != pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
                break;
            }
            case PXMIPSBranchLessThanZero:
            {
                pxMIPSBranch->CommitJump = 0 > pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
                break;
            }
            case PXMIPSBranchGreaterThanZero:
            {
                pxMIPSBranch->CommitJump = 0 < pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
                break;
            }
            case PXMIPSBranchLessThanOrEqualZero:
            {
                pxMIPSBranch->CommitJump = 0 >= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
                break;
            }
            case PXMIPSBranchGreaterThanOrEqualZero:
            {
                pxMIPSBranch->CommitJump = 0 <= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
                break;
            }
        }
    }



    if(pxMIPSBranch->CommitJump)
    {
#if PXLogEnable && 0
        PXLogPrint
        (
            PXLoggingInfo,
            PXMIPSProcessorName,
            "Jump EQ",
            "%8.8X vs %4.4X - Jump to: %i",
            pxMIPSProcessor->Register[pxMIPSTInstruction->RS],
            pxMIPSProcessor->Register[pxMIPSTInstruction->RT],
            offset
        );
#endif
        (PXSize)pxMIPSProcessor->ProgramCounter += (PXSize)pxMIPSBranch->Address;
    }
    else // Dont jump
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXMIPSProcessorName,
            "Jump EQ",
            "No jump. Value equal %8.8X",
            pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID]
        );
#endif
    }
}

void PXAPI PXMIPSJumpCalc(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction, PXMIPSJump PXREF pxMIPSJump)
{
    const PXSize actualAdress = (PXSize)pxMIPSProcessor->ProgramCounter + 4;
    const PXSize delaySlotAdress = (PXSize)pxMIPSProcessor->ROMOffsetVirtual + (PXSize)actualAdress;

    const PXSize res =
        ((0b00000011111111111111111111111111u & pxMIPSTInstruction->OperationCode) << 2) |
        0b11110000000000000000000000000000u & delaySlotAdress;

    // 26-Bit target, shifted left because the two lowest bits are always 0 anyway (allignment).
    // add the additional 4-bits of the delay slot
    pxMIPSJump->AddressVirual = (void*)res;       

    pxMIPSJump->AddressPhysical = (void*)((PXSize)pxMIPSJump->AddressVirual - (PXSize)pxMIPSProcessor->ROMOffsetVirtual);

    if(pxMIPSJump->PreserveReturnPoint)
    {
        // We are two instructions further.
        // 4+ because of the next instcution that is executed before this jump (Delayinstuciton)
        // another 4+ to point to the actual next one.
        pxMIPSProcessor->RegisterList[31] = delaySlotAdress + 4;
    }



    // Delay this instruction by calling another
    PXMIPSInstructionExecuteDeleay(pxMIPSProcessor);



    pxMIPSProcessor->ProgramCounter = pxMIPSJump->AddressPhysical; // Now, jump to actual target

    pxMIPSTInstruction->IncrmentCounter = PXFalse; // Skip the +4 offset


#if PXLogEnable
    PXSize returnAdress = 0;

    if(pxMIPSJump->PreserveReturnPoint)
    {
        returnAdress = pxMIPSProcessor->RegisterList[31];
    }

    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "JumpCalc",
        "Jump to 0x%8.8X, return:0x%8.8X",
        pxMIPSJump->AddressVirual,
        returnAdress
    );
#endif
}

void PXAPI PXMIPSInitializize(PXMIPSProcessor PXREF pxMIPSProcessor, PXCodeSegment PXREF pxCodeSegment)
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
        case PXMIPSInstructionTypeADDImmediate: return "ADDI";
        case PXMIPSInstructionTypeADDImmediateUnsigned: return "ADDIU";
        case PXMIPSInstructionTypeSetOnLessThanImmediate: return "SLTI";
        case PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned: return "SLTIU";
        case PXMIPSInstructionTypeANDImmediate: return "ANDI";
        case PXMIPSInstructionTypeORImmediate: return "ORI";
        case PXMIPSInstructionTypeExclusiveORImmediate: return "XORI";
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
        case PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor2: return "SCD2";
        case PXMIPSInstructionTypeStoreDoubleword: return "SD";
        case PXMIPSInstructionTypeShiftLeftLogical: return "SLL";
        case PXMIPSInstructionTypeShiftRightLogical: return "SRL";
        case PXMIPSInstructionTypeShiftRightArithmetic: return "SRA";
        case PXMIPSInstructionTypeShiftLeftLogicalVariable: return "SLLV";
        case PXMIPSInstructionTypeShiftRightLogicalVariable: return "SRLV";
        case PXMIPSInstructionTypeShiftRightArithmeticVariable: return "SRAV";
        case PXMIPSInstructionTypeJumpRegister: return "JR";
        case PXMIPSInstructionTypeJumpAndLinkRegister: return "JALR";
        case PXMIPSInstructionTypeSystemCall: return "SYSCALL";
        case PXMIPSInstructionTypeBreak: return "BREAK";
        case PXMIPSInstructionTypeSynchronize: return "SYNC";
        case PXMIPSInstructionTypeMoveFromHI: return "MFHI";
        case PXMIPSInstructionTypeMoveToHI: return "MTHI";
        case PXMIPSInstructionTypeMoveFromLO: return "MFLO";
        case PXMIPSInstructionTypeMoveToLO: return "MTLO";
        case PXMIPSInstructionTypeDoublewordShiftLeftLogicalVariable: return "DSLLV";
        case PXMIPSInstructionTypeDoublewordShiftRightLogicalVariable: return "DSRLV";
        case PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable: return "DSRAV";
        case PXMIPSInstructionTypeMultiply: return "MULT";
        case PXMIPSInstructionTypeMultiplyUnsigned: return "MULTU";
        case PXMIPSInstructionTypeDivide: return "DIV";
        case PXMIPSInstructionTypeDivideUnsigned: return "DIVU";
        case PXMIPSInstructionTypeDoublewordMultiply: return "DMULT";
        case PXMIPSInstructionTypeDoublewordMultiplyUnsigned: return "DMULTU";
        case PXMIPSInstructionTypeDoublewordDivide: return "DDIV";
        case PXMIPSInstructionTypeDoublewordDivideUnsigned: return "DDIVU";
        case PXMIPSInstructionTypeADD: return "ADD";
        case PXMIPSInstructionTypeADDUnsigned: return "ADDU";
        case PXMIPSInstructionTypeSubtract: return "SUB";
        case PXMIPSInstructionTypeSubtractUnsigned: return "SUBU";
        case PXMIPSInstructionTypeAND: return "AND";
        case PXMIPSInstructionTypeOR: return "OR";
        case PXMIPSInstructionTypeExclusiveOR: return "XOR";
        case PXMIPSInstructionTypeNOR: return "NOR";
        case PXMIPSInstructionTypeSetOnLessThan: return "SLT";
        case PXMIPSInstructionTypeSetOnLessThanUnsigned: return "SLTU";
        case PXMIPSInstructionTypeDoublewordADD: return "DADD";
        case PXMIPSInstructionTypeDoublewordADDUnsigned: return "DADDU";
        case PXMIPSInstructionTypeDoublewordSubtract: return "DSUB";
        case PXMIPSInstructionTypeDoublewordSubtractUnsigned: return "DSUBU";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqual: return "TGE";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqualUnsigned: return "TGEU";
        case PXMIPSInstructionTypeTrapIfLessThan: return "TLT";
        case PXMIPSInstructionTypeTrapIfLessThanUnsigned: return "TLTU";
        case PXMIPSInstructionTypeTrapIfEqual: return "TEQ";
        case PXMIPSInstructionTypeTrapIfNotEqual: return "TNE";
        case PXMIPSInstructionTypeDoublewordShiftLeftLogical: return "DSLL";
        case PXMIPSInstructionTypeDoublewordShiftRightLogical: return "DSRL";
        case PXMIPSInstructionTypeDoublewordShiftRightArithmetic: return "DSRA";
        case PXMIPSInstructionTypeDoublewordShiftLeftLogicalPlus32: return "DSLL32";
        case PXMIPSInstructionTypeDoublewordShiftRight: return "DSRL32";
        case PXMIPSInstructionTypeDoublewordShiftRightArithmeticPlus32: return "DSRA32";
        case PXMIPSInstructionTypeBranchOnLessThanZero: return "BLTZ";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZero: return "BGEZ";
        case PXMIPSInstructionTypeBranchOnLessThanZeroLikely: return "BLTZL";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroLikely: return "BGEZL";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediate: return "TGEI";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediateUnsigned: return "TGEIU";
        case PXMIPSInstructionTypeTrapIfLessThanImmediate: return "TLTI";
        case PXMIPSInstructionTypeTrapIfLessThanImmediateUnsigned: return "TLTIU";
        case PXMIPSInstructionTypeTrapIfEqualImmediate: return "TEQI";
        case PXMIPSInstructionTypeTrapIfNotEqualImmediate: return "TNEI";
        case PXMIPSInstructionTypeBranchOnLessThanZeroAndLink: return "BLTZAL";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLink: return "BGEZAL";
        case PXMIPSInstructionTypeBranchOnLessThanZeroAndLinkLikely: return "BLTZALL";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLinkLikely: return "BGEZALL";

        case PXMIPSInstructionTypeMoveWordFromFPUCoprocessor1: return "MFC0";
        case PXMIPSInstructionTypeDoublewordMoveFromSystemControlCoprocessor: return "DMFC0";
        case PXMIPSInstructionTypeMoveControlWordFromCoprocessorZ: return "CFCz";
        case PXMIPSInstructionTypeMoveToSystemControlCoprocessor: return "MTC0";
        case PXMIPSInstructionTypeDoublewordMoveToSystemControlCoprocessor: return "DMTC0";
        case PXMIPSInstructionTypeMoveControlToCoprocessorZ: return "CTCz";
            // BC

        default:
            return PXNull;
    }
}

const char* PXAPI PXMIPSInstructionTypeToStringLong(const PXMIPSInstructionType pxMIPSInstruction)
{
    switch(pxMIPSInstruction)
    {
        case PXMIPSInstructionTypeOPCodeSpecial: return "-----";
        case PXMIPSInstructionTypeOPCodeREGIMM: return "-----";
        case PXMIPSInstructionTypeJump: return "Jump";
        case PXMIPSInstructionTypeJumpAndLink: return "Jump And Link";
        case PXMIPSInstructionTypeBranchOnEqual: return "Branch On Equal";
        case PXMIPSInstructionTypeBranchOnNotEqual: return "Branch On Not Equal";
        case PXMIPSInstructionTypeBranchOnLessThanOrEqualToZero: return "Branch On Less Than Or Equal To Zero";
        case PXMIPSInstructionTypeBranchOnGreaterThanZero: return "Branch On Greater Than Zero";
        case PXMIPSInstructionTypeADDImmediate: return "ADD Immediate";
        case PXMIPSInstructionTypeADDImmediateUnsigned: return "ADD Immediate Unsigned";
        case PXMIPSInstructionTypeSetOnLessThanImmediate: return "Set On Less Than Immediate";
        case PXMIPSInstructionTypeSetOnLessThanImmediateUnsigned: return "Set On Less Than Immediate Unsigned";
        case PXMIPSInstructionTypeANDImmediate: return "AND Immediate";
        case PXMIPSInstructionTypeORImmediate: return "OR Immediate";
        case PXMIPSInstructionTypeExclusiveORImmediate: return "Exclusive OR Immediate";
        case PXMIPSInstructionTypeLoadUpperImmediate: return "Load Upper Immediate";
        case PXMIPSInstructionTypeCoprocessorZOperation0: return "Coprocessor Z Operation0";
        case PXMIPSInstructionTypeCoprocessorZOperation1: return "Coprocessor Z Operation1";
        case PXMIPSInstructionTypeCoprocessorZOperation2: return "Coprocessor Z Operation2";
        case PXMIPSInstructionTypeBranchOnEqualLikely: return "Branch On Equal Likely";
        case PXMIPSInstructionTypeBranchOnNotEqualLikely: return "Branch On Not Equal Likely";
        case PXMIPSInstructionTypeBranchOnLessThan: return "Branch On Less Than";
        case PXMIPSInstructionTypeBranchOnGreaterThanZeroLikely: return "Branch On Greater Than Zero Likely";
        case PXMIPSInstructionTypeDoublewordADDImmediate: return "Doubleword ADD Immediate";
        case PXMIPSInstructionTypeDoublewordADDImmediateUnsigned: return "Doubleword ADD Immediate Unsigned";
        case PXMIPSInstructionTypeLoadDoublewordLeft: return "Load Doubleword Left";
        case PXMIPSInstructionTypeLoadDoublewordRight: return "Load Doubleword Right";
        case PXMIPSInstructionTypeLoadByte: return "Load Byte";
        case PXMIPSInstructionTypeLoadHalfword: return "Load Halfword";
        case PXMIPSInstructionTypeLoadWordLeft: return "Load Word Left";
        case PXMIPSInstructionTypeLoadWord: return "Load Word";
        case PXMIPSInstructionTypeLoadByteUnsigned: return "Load Byte Unsigned";
        case PXMIPSInstructionTypeLoadHalfwordUnsigned: return "Load Halfword Unsigned";
        case PXMIPSInstructionTypeLoadWordRight: return "Load Word Right";
        case PXMIPSInstructionTypeLoadWordUnsigned: return "Load Word Unsigned";
        case PXMIPSInstructionTypeStoreByte: return "Store Byte";
        case PXMIPSInstructionTypeStoreHalfword: return "Store Halfword";
        case PXMIPSInstructionTypeStoreWordLeft: return "Store Word Left";
        case PXMIPSInstructionTypeStoreWord: return "Store Word";
        case PXMIPSInstructionTypeStoreDoublewordLeft: return "Store Doubleword Left";
        case PXMIPSInstructionTypeStoreDoublewordRight: return "Store Doubleword Right";
        case PXMIPSInstructionTypeStoreWordRight: return "Store Word Right";
        case PXMIPSInstructionTypeCacheOperation: return "Cache Operation";
        case PXMIPSInstructionTypeLoadLinked: return "Load Linked";
        case PXMIPSInstructionTypeLoadWordToFPUCoprocessor1: return "LoadWord To FPU Coprocessor 1";
        case PXMIPSInstructionTypeLoadWordToFPUCoprocessor2: return "LoadWord To FPU Coprocessor 2";
        case PXMIPSInstructionTypeLoadLinkedDoubleword: return "Load Linked Doubleword";
        case PXMIPSInstructionTypeLoadDoublewordToFPUCoprocessor1: return "Load Doubleword To FPU Coprocessor 1";
        case PXMIPSInstructionTypeLoadDoublewordToFPUCoprocessor2: return "Load Doubleword To FPU Coprocessor 2";
        case PXMIPSInstructionTypeLoadDoubleword: return "Load Doubleword";
        case PXMIPSInstructionTypeStoreConditional: return "Store Conditional";
        case PXMIPSInstructionTypeStoreWordFromFPUCoprocessor1: return "StoreWord From FPU Coprocessor 1";
        case PXMIPSInstructionTypeStoreWordFromFPUCoprocessor2: return "StoreWord From FPU Coprocessor 2";
        case PXMIPSInstructionTypeStoreConditionalDoubleword: return "Store Conditional Doubleword";
        case PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor1: return "Store Doubleword From FPU Coprocessor 1";
        case PXMIPSInstructionTypeStoreDoublewordFromFPUCoprocessor2: return "Store Doubleword From FPU Coprocessor 2";
        case PXMIPSInstructionTypeStoreDoubleword: return "Store Doubleword";
        case PXMIPSInstructionTypeShiftLeftLogical: return "Shift Left Logical";
        case PXMIPSInstructionTypeShiftRightLogical: return "Shift Right Logical";
        case PXMIPSInstructionTypeShiftRightArithmetic: return "Shift Right Arithmetic";
        case PXMIPSInstructionTypeShiftLeftLogicalVariable: return "Shift Left Logical Variable";
        case PXMIPSInstructionTypeShiftRightLogicalVariable: return "Shift Right Logical Variable";
        case PXMIPSInstructionTypeShiftRightArithmeticVariable: return "Shift Right Arithmetic Variable";
        case PXMIPSInstructionTypeJumpRegister: return "Jump Register";
        case PXMIPSInstructionTypeJumpAndLinkRegister: return "Jump And Link Register";
        case PXMIPSInstructionTypeSystemCall: return "System Call";
        case PXMIPSInstructionTypeBreak: return "Break";
        case PXMIPSInstructionTypeSynchronize: return "Synchronize";
        case PXMIPSInstructionTypeMoveFromHI: return "Move From HI";
        case PXMIPSInstructionTypeMoveToHI: return "Move To HI";
        case PXMIPSInstructionTypeMoveFromLO: return "Move From low";
        case PXMIPSInstructionTypeMoveToLO: return "Move To low";
        case PXMIPSInstructionTypeDoublewordShiftLeftLogicalVariable: return "Doubleword Shift Left Logical Variable";
        case PXMIPSInstructionTypeDoublewordShiftRightLogicalVariable: return "Doubleword Shift Right Logical Variable";
        case PXMIPSInstructionTypeDoublewordShiftRightArithmeticVariable: return "Doubleword Shift Right Arithmetic Variable";
        case PXMIPSInstructionTypeMultiply: return "Multiply";
        case PXMIPSInstructionTypeMultiplyUnsigned: return "Multiply Unsigned";
        case PXMIPSInstructionTypeDivide: return "Divide";
        case PXMIPSInstructionTypeDivideUnsigned: return "Divide Unsigned";
        case PXMIPSInstructionTypeDoublewordMultiply: return "Doubleword Multiply";
        case PXMIPSInstructionTypeDoublewordMultiplyUnsigned: return "Doubleword Multiply Unsigned";
        case PXMIPSInstructionTypeDoublewordDivide: return "Doubleword Divide";
        case PXMIPSInstructionTypeDoublewordDivideUnsigned: return "Doubleword Divide Unsigned";
        case PXMIPSInstructionTypeADD: return "Addition";
        case PXMIPSInstructionTypeADDUnsigned: return "Addition Unsigned";
        case PXMIPSInstructionTypeSubtract: return "Subtract";
        case PXMIPSInstructionTypeSubtractUnsigned: return "Subtract Unsigned";
        case PXMIPSInstructionTypeAND: return "AND";
        case PXMIPSInstructionTypeOR: return "OR";
        case PXMIPSInstructionTypeExclusiveOR: return "Exclusive OR";
        case PXMIPSInstructionTypeNOR: return "NOR";
        case PXMIPSInstructionTypeSetOnLessThan: return "Set On Less Than";
        case PXMIPSInstructionTypeSetOnLessThanUnsigned: return "Set On Less Than Unsigned";
        case PXMIPSInstructionTypeDoublewordADD: return "Doubleword ADD";
        case PXMIPSInstructionTypeDoublewordADDUnsigned: return "Doubleword ADD Unsigned";
        case PXMIPSInstructionTypeDoublewordSubtract: return "Doubleword Subtract";
        case PXMIPSInstructionTypeDoublewordSubtractUnsigned: return "Doubleword Subtract Unsigned";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqual: return "Trap If Greater Than Or Equal";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqualUnsigned: return "Trap If Greater Than Or Equal Unsigned";
        case PXMIPSInstructionTypeTrapIfLessThan: return "Trap If Less Than";
        case PXMIPSInstructionTypeTrapIfLessThanUnsigned: return "Trap If Less Than Unsigned";
        case PXMIPSInstructionTypeTrapIfEqual: return "Trap If Equal";
        case PXMIPSInstructionTypeTrapIfNotEqual: return "Trap If Not Equal";
        case PXMIPSInstructionTypeDoublewordShiftLeftLogical: return "Doubleword Shift Left Logical";
        case PXMIPSInstructionTypeDoublewordShiftRightLogical: return "Doubleword Shift Right Logical";
        case PXMIPSInstructionTypeDoublewordShiftRightArithmetic: return "Doubleword Shift Right Arithmetic";
        case PXMIPSInstructionTypeDoublewordShiftLeftLogicalPlus32: return "Doubleword Shift Left Logical +32";
        case PXMIPSInstructionTypeDoublewordShiftRight: return "Doubleword Shift Right";
        case PXMIPSInstructionTypeDoublewordShiftRightArithmeticPlus32: return "Doubleword Shift Right Arithmetic +32";
        case PXMIPSInstructionTypeBranchOnLessThanZero: return "Branch On Less Than Zero";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZero: return "Branch On Greater Than Or Equal To Zero";
        case PXMIPSInstructionTypeBranchOnLessThanZeroLikely: return "Branch On Less Than Zero Likely";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroLikely: return "Branch On Greater Than Or Equal To Zero Likely";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediate: return "Trap If Greater Than Or Equal Immediate";
        case PXMIPSInstructionTypeTrapIfGreaterThanOrEqualImmediateUnsigned: return "Trap If Greater Than Or Equal Immediate Unsigned";
        case PXMIPSInstructionTypeTrapIfLessThanImmediate: return "Trap If Less Than Immediate";
        case PXMIPSInstructionTypeTrapIfLessThanImmediateUnsigned: return "Trap If Less Than Immediate Unsigned";
        case PXMIPSInstructionTypeTrapIfEqualImmediate: return "Trap If Equal Immediate";
        case PXMIPSInstructionTypeTrapIfNotEqualImmediate: return "Trap If Not Equal Immediate";
        case PXMIPSInstructionTypeBranchOnLessThanZeroAndLink: return "Branch On Less Than Zero And Link";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLink: return "Branch On Greater Than Or Equal To Zero And Link";
        case PXMIPSInstructionTypeBranchOnLessThanZeroAndLinkLikely: return "Branch On Less Than Zero And Link Likely";
        case PXMIPSInstructionTypeBranchOnGreaterThanOrEqualToZeroAndLinkLikely: return "Branch On Greater Than Or Equal To Zero And Link Likely";



        case PXMIPSInstructionTypeMoveWordFromFPUCoprocessor1: return "MoveWord From FPU Coprocessor 1";
        case PXMIPSInstructionTypeDoublewordMoveFromSystemControlCoprocessor: return "Doubleword Move From System Control Coprocessor";
        case PXMIPSInstructionTypeMoveControlWordFromCoprocessorZ: return "Move Control Word From Coprocessor Z";
        case PXMIPSInstructionTypeMoveToSystemControlCoprocessor: return "Move To System Control Coprocessor";
        case PXMIPSInstructionTypeDoublewordMoveToSystemControlCoprocessor: return "Doubleword Move To System Control Coprocessor";
        case PXMIPSInstructionTypeMoveControlToCoprocessorZ: return "Move Control To Coprocessor Z";
            // BC

        default:
            return PXNull;
    }
}

void PXMIPSInstructionPrint(PXMIPSTInstruction PXREF pxMIPSTInstruction);

void PXAPI PXMIPSInstructionExecute(PXMIPSProcessor PXREF pxMIPSProcessor)
{
    const PXSize instructionWidth = 4;

    PXMIPSTInstruction pxMIPSTInstruction;
    pxMIPSTInstruction.IncrmentCounter = PXTrue;
    pxMIPSTInstruction.Adress = (void*)((PXSize)pxMIPSProcessor->ROMOffsetActual + (PXSize)pxMIPSProcessor->ProgramCounter);
    pxMIPSTInstruction.AdressVirtual = (PXI8U*)((PXSize)pxMIPSProcessor->ROMOffsetVirtual + (PXSize)pxMIPSProcessor->ProgramCounter);

    // instruction is big endian, to use our bit extraction, we need to swap it

    pxMIPSTInstruction.OperationCode = PXI32Make(pxMIPSTInstruction.Adress[3], pxMIPSTInstruction.Adress[2], pxMIPSTInstruction.Adress[1], pxMIPSTInstruction.Adress[0]);

    if(pxMIPSTInstruction.OperationCode == 0)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXMIPSProcessorName,
            "NOOP",
            "0x%8.8X   --- No Operation ---",
            pxMIPSTInstruction.AdressVirtual
        );
#endif
        (PXSize)pxMIPSProcessor->ProgramCounter += instructionWidth;

        return;
    }

    pxMIPSTInstruction.Type             = (pxMIPSTInstruction.OperationCode & 0b11111100000000000000000000000000) >> 26;
    pxMIPSTInstruction.RegisterSourceID = (pxMIPSTInstruction.OperationCode & 0b00000011111000000000000000000000) >> 21;
    pxMIPSTInstruction.RegisterTargetID = (pxMIPSTInstruction.OperationCode & 0b00000000000111110000000000000000) >> 16;
    pxMIPSTInstruction.Immediate        = (pxMIPSTInstruction.OperationCode & 0b00000000000000001111111111111111) >> 0;

    const PXMIPSTInstructionFunction instructionFunction = pxMIPSProcessor->GeneralInstructionList[pxMIPSTInstruction.Type]; // Direct ID lookup
    
#if PXLogEnable

    PXBool noprint =
        instructionFunction == PXMIPSInstructionADDImmediate ||
        instructionFunction == PXMIPSInstructionStoreWord ||
        instructionFunction == PXMIPSInstructionBranchOnNotEqual;

    if(!noprint)
    {
        PXMIPSInstructionPrint(&pxMIPSTInstruction);
    }

#endif

    instructionFunction(pxMIPSProcessor, &pxMIPSTInstruction); // Call function

    // Next command
    // MIPS always has 4-Byte commands
    (PXSize)pxMIPSProcessor->ProgramCounter += (pxMIPSTInstruction.IncrmentCounter * instructionWidth);
}


void PXMIPSInstructionPrint(PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
#if PXLogEnable
    const char* instuctionNameShort = PXMIPSInstructionTypeToStringShort(pxMIPSTInstruction->Type);
    const char* instuctionNameLong = PXMIPSInstructionTypeToStringLong(pxMIPSTInstruction->Type);

    PXLogPrint
    (
        PXLoggingAllocation,
        PXMIPSProcessorName,
        "Instruction",
        "0x%8.8X   %2.2X %2.2X %2.2X %2.2X   RS:%2.2X RT:%2.2X IMM:%4.4X   %2.2X:%-7s %s",
        (int)pxMIPSTInstruction->AdressVirtual,
        pxMIPSTInstruction->Adress[0],
        pxMIPSTInstruction->Adress[1],
        pxMIPSTInstruction->Adress[2],
        pxMIPSTInstruction->Adress[3],
        pxMIPSTInstruction->RegisterSourceID,
        pxMIPSTInstruction->RegisterTargetID,
        pxMIPSTInstruction->Immediate,
        pxMIPSTInstruction->Type,
        instuctionNameShort,
        instuctionNameLong

    );
#endif
}

#define PXMIPSMemoryIOStore 0x01
#define PXMIPSMemoryIOLoad  0x02



void PXAPI PXMIPSMemoryIO(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction, const PXI32U datatype, PXI8U mode)
{
    const PXSize base = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
    const PXSize offset = (PXI16S)pxMIPSTInstruction->Immediate; // Sign extend
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] += offset; // add to register

    const PXSize virtualAdress = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];

    void* realAdressOffset = PXMIPSTranslateVirtualAdress(pxMIPSProcessor, virtualAdress); // Virtual adress to actual pointer
    void* value = &pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID]; // Adress of value to store

    const PXSize typeSize = datatype & PXTypeSizeMask;

    if(mode)
    {
#if PXLogEnable && 0
        PXLogPrint
        (
            PXLoggingInfo,
            PXMIPSProcessorName,
            "Memory-Store",
            "Value:%8.8X -> %8.8X - Base:%8.8X + Offset:%i",
            pxMIPSProcessor->Register[pxMIPSTInstruction->RegisterTargetID],
            virtualAdress,
            base,
            offset
        );
#endif

        PXMemoryCopy(value, realAdressOffset, typeSize);
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXMIPSProcessorName,
            "Memory-Load",
            "Value:%8.8X -> %8.8X - Base:%8.8X + Offset:%i",
            pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
            virtualAdress,
            base,
            offset
        );
#endif

        PXMemoryCopy(realAdressOffset, value, typeSize);
    }




    // pxMIPSProcessor->Register[pxMIPSTInstruction->RegisterSourceID] = pxMIPSProcessor->Register[pxMIPSTInstruction->RegisterTargetID];
    //    pxMIPSProcessor->Register[pxMIPSTInstruction->RegisterSourceID] += 0x02;

}

PXResult PXAPI PXMIPSTranslate(PXMIPSProcessor PXREF pxMIPSProcessor, const PXByte PXREF data, const PXSize length)
{
    //PXClear(PXMIPSProcessor, pxMIPSProcessor);
    //DWORD protectIDOld = 0;
   // DWORD protectIDNew = PAGE_READONLY;
    //const PXBool succ = VirtualProtect(data, 0x0007dac0, protectIDNew, &protectIDOld);


    pxMIPSProcessor->ROMOffsetActual = data;
    pxMIPSProcessor->ROMOffsetMaximal = length;
    pxMIPSProcessor->ProgramCounter = 0;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "Execute",
        "--- Start ---"
    );
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
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
        PXMIPSInstructionADDImmediate,
        PXMIPSInstructionADDImmediateUnsigned,
        PXMIPSInstructionSetOnLessThanImmediate,
        PXMIPSInstructionSetOnLessThanImmediateUnsigned,
        PXMIPSInstructionANDImmediate,
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
        PXMIPSInstructionADD,
        PXMIPSInstructionADDUnsigned,
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
        PXMIPSInstructionMoveControlWordFromCoprocessorZ,
        PXMIPSInstructionReserved,
        PXMIPSInstructionMoveToSystemControlCoprocessor ,
        PXMIPSInstructionDoublewordMoveToSystemControlCoprocessor ,
        PXMIPSInstructionMoveControlToCoprocessorZ,
        PXMIPSInstructionReserved,
        PXMIPSInstructionReserved
    };


    pxMIPSProcessor->GeneralInstructionList = instructionGeneralLookup;
    pxMIPSProcessor->SpecialInstructionList = instructionSpecialLookup;
    pxMIPSProcessor->REGIMMInstructionList = instructionRegimmLookup;
    pxMIPSProcessor->CorpocessorList = instructioncoprocLookup;

    for(;;)
    {
        PXMIPSInstructionExecute(pxMIPSProcessor);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "Execute",
        "--- Done ---"
    );
#endif

    return PXResultOK;
}

void PXAPI PXMIPSInstructionCoProcessorCalc(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
   // PXMIPSInstructionCoProcessor pxMIPSInstructionCoProcessor;

    const PXBool instructionID = pxMIPSTInstruction->RegisterSourceID;
    const PXMIPSTInstructionFunction instuction = pxMIPSProcessor->CorpocessorList[instructionID];

   // pxMIPSInstructionCoProcessor.ProcessorID = pxMIPSTInstruction->Type & 0b11;
    pxMIPSTInstruction->CoProcessorID = pxMIPSTInstruction->Type & 0b11;

    pxMIPSTInstruction->Type = instructionID | PXMIPSOPCodeCOPz;
    pxMIPSTInstruction->RegisterDestinationID = (pxMIPSTInstruction->Immediate & 0b1111100000000000) >> 11; // Can also be FS? for PXF32s

    PXMIPSInstructionPrint(pxMIPSTInstruction);

    instuction(pxMIPSProcessor, pxMIPSTInstruction);

}

void PXAPI PXMIPSInstructionExecuteDeleay(PXMIPSProcessor PXREF pxMIPSProcessor)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "Delay",
        "Execute delay instruction"
    );
#endif

    (PXSize)pxMIPSProcessor->ProgramCounter += 4; // go to next instuction

    PXMIPSInstructionExecute(pxMIPSProcessor); // Call next instruction to re-execute it before current
}

PXMIPSMemoryRegion PXMIPSMemoryRegionDetect(const void* adress)
{
    const PXSize adressVal = (PXSize)adress;

    PXSize adressSpace = (adressVal >> 4) & 0xFFF00000;

    switch(adressSpace)
    {
        case 0x03F00000: return PXMIPSMemoryRegionRDRAMRegistersHandler;
        case 0x04000000: return PXMIPSMemoryRegionSPRegistersHandler;
        case 0x04100000: return PXMIPSMemoryRegionDPCommandRegistersHandler;
        case 0x04300000: return PXMIPSMemoryRegionMIPSInterfaceHandler;
        case 0x04400000: return PXMIPSMemoryRegionVideoInterfaceHandler;
        case 0x04500000: return PXMIPSMemoryRegionAudioInterfaceHandler;
        case 0x04600000: return PXMIPSMemoryRegionPeripheralInterfaceHandler;
        case 0x04700000: return PXMIPSMemoryRegionRDRAMInterfaceHandler;
        case 0x04800000: return PXMIPSMemoryRegionSerialInterfaceHandler;
        case 0x05000000: return PXMIPSMemoryRegionCartridgeDomain2Address1Handler;
        case 0x06000000: return PXMIPSMemoryRegionCartridgeDomain1Address1Handler;
        case 0x08000000: return PXMIPSMemoryRegionCartridgeDomain2Address2Handler;
        case 0x13F00000: return PXMIPSMemoryRegionISViewerHandler;
        case 0x1FC00000: return PXMIPSMemoryRegionPIFRamHandler;
        case 0x1FF00000: return PXMIPSMemoryRegionCartridgeDomain1Address3Handler;

        default:
            return PXMIPSMemoryRegionUnkown;
    }
}


void* PXAPI PXMIPSTranslateVirtualAdress(PXMIPSProcessor PXREF pxMIPSProcessor, const void* virtualAdress)
{
    const PXBool virtualAdressValid = 0 == (0b11 & (PXSize)virtualAdress);
    PXSize translatedAdress = (PXSize)virtualAdress;

    if(!virtualAdressValid)
    {
        PXDebugBreak();
    }

    const PXMIPSMemoryRegion region = PXMIPSMemoryRegionDetect(virtualAdress);

    switch(region)
    {
        case PXMIPSMemoryRegionRDRAMRegistersHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionSPRegistersHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionDPCommandRegistersHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionMIPSInterfaceHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionVideoInterfaceHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionAudioInterfaceHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionPeripheralInterfaceHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionRDRAMInterfaceHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionSerialInterfaceHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionCartridgeDomain2Address1Handler:
        {
            break;
        }
        case PXMIPSMemoryRegionCartridgeDomain1Address1Handler:
        {
            break;
        }
        case PXMIPSMemoryRegionCartridgeDomain2Address2Handler:
        {
            translatedAdress -= (PXSize)pxMIPSProcessor->ROMOffsetVirtual;
            translatedAdress += (PXSize)pxMIPSProcessor->ROMOffsetActual;
            break;
        }
        case PXMIPSMemoryRegionISViewerHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionPIFRamHandler:
        {
            break;
        }
        case PXMIPSMemoryRegionCartridgeDomain1Address3Handler:
        {
            break;
        }
        default:
        {
            translatedAdress += (PXSize)pxMIPSProcessor->RAMAdress;
            break;
        }
    }


#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "TranslateAdress",
        "V:0x%8.8X -> P:0x%p",
        virtualAdress,
        translatedAdress

    );
#endif


    // Check in what range this adress is and then get the proper poition
    //     void* target = (PXI8U*)pxMIPSProcessor->RAMAdress + realAdressOffset;


    return (void*)translatedAdress;
}

void PXAPI PXMIPSInstructionReserved(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingFailure,
        PXMIPSProcessorName,
        "Instruction",
        "Illegal or not implemented!"
    );
#endif
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionSpecial(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    const PXI8U instructionID                 =  pxMIPSTInstruction->Immediate & 0b0000000000111111;
    pxMIPSTInstruction->ShiftAmount             = (pxMIPSTInstruction->Immediate & 0b0000011111000000) >> 6;
    pxMIPSTInstruction->RegisterDestinationID   = (pxMIPSTInstruction->Immediate & 0b1111100000000000) >> 11;
    pxMIPSTInstruction->Type                    = instructionID | PXMIPSOPCodeSpecial;

    PXMIPSInstructionPrint(pxMIPSTInstruction);

    const PXMIPSTInstructionFunction specialinstructionFunction = pxMIPSProcessor->SpecialInstructionList[instructionID]; // Direct ID lookup

    specialinstructionFunction(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionREGIMM(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    const PXI8U instructionID = pxMIPSTInstruction->RegisterTargetID;

    pxMIPSTInstruction->Type = instructionID | PXMIPSOPCodeREGIMM;

    const PXMIPSTInstructionFunction specialinstructionFunction = pxMIPSProcessor->REGIMMInstructionList[instructionID]; // Direct ID lookup

    specialinstructionFunction(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionJump(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSJump pxMIPSJump;
    pxMIPSJump.PreserveReturnPoint = PXFalse;

    PXMIPSJumpCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSJump);
}

void PXAPI PXMIPSInstructionJumpAndLink(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSJump pxMIPSJump;
    pxMIPSJump.PreserveReturnPoint = PXTrue;

    PXMIPSJumpCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSJump);
}

void PXAPI PXMIPSInstructionBranchOnEqual(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchEqual;
    pxMIPSBranch.Likely = PXFalse;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionBranchOnNotEqual(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchNotEqual;
    pxMIPSBranch.Likely = PXFalse;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionBranchOnLessThanOrEqualToZero(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchLessThanOrEqualZero;
    pxMIPSBranch.Likely = PXFalse;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionBranchOnGreaterThanZero(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchGreaterThanOrEqualZero;
    pxMIPSBranch.Likely = PXFalse;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionADDImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
                The 16-bit immediate is sign-extended and added to the contents of general
                purpose register rs to store the result in general purpose register rt. In 64-bit
                mode, the operand must be sign-extended, 32-bit values.
                An integer overflow exception occurs if carries out of bits 30 and 31 differ (2’s
                complement overflow). The contents of destination register rt is not modified
                when an integer overflow exception occurs
                */

    const PXI64S value = (PXI16S)pxMIPSTInstruction->Immediate;


#if PXLogEnable
    const PXI64S rtBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    const PXI64S rsBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
#endif

    const PXI64S resukt =
        (PXI64U)pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] +
        (PXI16S)pxMIPSTInstruction->Immediate;

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] = resukt;
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] = resukt;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
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

void PXAPI PXMIPSInstructionADDImmediateUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
The 16-bit immediate is sign-extended and added to the contents of general
purpose register rs to store the result in general purpose register rt. No integer
overflow exception occurs under any circumstance. In 64-bit mode, the operand
must be sign-extended, 32-bit values.
The only difference between this instruction and the ADDI instruction is that
ADDIU instruction never causes an integer overflow exception.
*/

    const PXI64U value = (PXI16S)pxMIPSTInstruction->Immediate;

#if PXLogEnable
    const PXSize rsBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
    const PXSize rtBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
#endif

    //PXSize result = pxMIPSProcessor->Register[pxMIPSTInstruction->RT] + value; // Calc and store into rs
    //*target += pxMIPSProcessor->Register[pxMIPSTInstruction.RS]; // copy rs to rt


    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] += value; // Add Immediate to RS
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID]; // Store RS in RT

#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "Add Unsigned",
        "\n"
        "%16.8X = %16.8X + %16.8X Before:%16.8X\n"
        "%16i = %16i + %16i Before:%16i",
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
        rsBefore,
        value,
        rtBefore,
        (PXI64U)pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
        (PXI64U)rsBefore,
        value,
        rtBefore
    );
#endif
}

void PXAPI PXMIPSInstructionSetOnLessThanImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionSetOnLessThanImmediateUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionANDImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] &= pxMIPSTInstruction->Immediate;
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionORImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
A logical OR operation applied between 16-bit zero-extended immediate and the
contents of general purpose register rs is executed in bit units. The result is stored
in general purpose register rt
*/

#if PXLogEnable
    const PXSize rtBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    const PXSize rsBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
#endif

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] |= (PXI64U)pxMIPSTInstruction->Immediate;
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];

#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "OR",
        "\n"
        "%16.8X = %16.8X | %16.8X Before:%16.8X\n"
        "%16i = %16i | %16i Before:%16i",
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
        rsBefore,
        (PXI64U)pxMIPSTInstruction->Immediate,
        rtBefore,
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
        rsBefore,
        (PXI64U)pxMIPSTInstruction->Immediate,
        rtBefore
    );
#endif
}

void PXAPI PXMIPSInstructionExclusiveOrImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadUpperImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXBool rsValid = 0 == pxMIPSTInstruction->RegisterSourceID;

    /*
    The 16-bit immediate is shifted left 16 bits and combined to 16 bits of zeros. The
    result is placed into general purpose register rt.
    In 64-bit mode, the loaded word    is sign-extended to 64 bits.
    */

    PXI64U value = (PXI16U)pxMIPSTInstruction->Immediate;

    value <<= 16;


#if PXLogEnable
    PXSize valueBefore = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
#endif

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] |= value;

#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "Load",
        "\n"
        "%16.8X = %16.8X + (%8.4X << 16) Before:%8.8X\n"
        "%16i = %16i + (%8i << 16) Before:%4i",
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID],
        pxMIPSTInstruction->Immediate,
        valueBefore,
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID],
        pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID],
        pxMIPSTInstruction->Immediate,
        valueBefore
    );
#endif
}

void PXAPI PXMIPSInstructionCoprocessorZOperation0(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSInstructionCoProcessorCalc(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionCoprocessorZOperation1(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSInstructionCoProcessorCalc(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionCoprocessorZOperation2(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSInstructionCoProcessorCalc(pxMIPSProcessor, pxMIPSTInstruction);
}

void PXAPI PXMIPSInstructionBranchOnEqualLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchEqual;
    pxMIPSBranch.Likely = PXTrue;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionBranchOnNotEqualLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchNotEqual;
    pxMIPSBranch.Likely = PXTrue;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionBranchOnLessThan(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchLessThanOrEqualZero;
    pxMIPSBranch.Likely = PXFalse;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionBranchOnGreaterThanZeroLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSBranch pxMIPSBranch;
    pxMIPSBranch.Mode = PXMIPSBranchGreaterThanOrEqualZero;
    pxMIPSBranch.Likely = PXTrue;

    PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

void PXAPI PXMIPSInstructionDoublewordADDImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordADDImmediateUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadDoublewordLeft(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadDoublewordRight(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadByte(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt08U, PXMIPSMemoryIOLoad);
}

void PXAPI PXMIPSInstructionLoadHalfword(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt16U, PXMIPSMemoryIOLoad);
}

void PXAPI PXMIPSInstructionLoadWordLeft(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadWord(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt32U, PXMIPSMemoryIOLoad);
}

void PXAPI PXMIPSInstructionLoadByteUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadHalfwordUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadWordRight(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadWordUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreByte(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt08U, PXMIPSMemoryIOStore);
}

void PXAPI PXMIPSInstructionStoreHalfword(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt16U, PXMIPSMemoryIOStore);
}

void PXAPI PXMIPSInstructionStoreWordLeft(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}



void PXAPI PXMIPSInstructionStoreWord(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
    The 16-bit offset is sign-extended and added to the contents of general purpose
    register base to form a virtual address. The contents of general purpose register rt
    are stored in the memory location specified by the address. If either of the low-
    order two bits of the address are not zero, an address error exception occurs.
    */

    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt32U, PXMIPSMemoryIOStore);
}

void PXAPI PXMIPSInstructionStoreDoublewordLeft(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreDoublewordRight(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreWordRight(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionCacheOperation(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadLinked(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadWordToFPUCoprocessor1(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadWordToFPUCoprocessor2(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadLinkedDoubleword(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadDoublewordToFPUCoprocessor1(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadDoublewordToFPUCoprocessor2(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionLoadDoubleword(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreConditional(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreWordFromFPUCoprocessor1(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreWordFromFPUCoprocessor2(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreConditionalDoubleword(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreDoublewordFromFPUCoprocessor1(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreDoublewordFromFPUCoprocessor2(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionStoreDoubleword(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXMIPSMemoryIO(pxMIPSProcessor, pxMIPSTInstruction, PXTypeInt64U, PXMIPSMemoryIOStore);
}

void PXAPI PXMIPSInstructionShiftLeftLogical(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] <<= pxMIPSTInstruction->ShiftAmount;
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionShiftRightLogical(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] >>= pxMIPSTInstruction->ShiftAmount;
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionShiftRightArithmetic(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionShiftLeftLogicalVariable(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] <<= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionShiftRightLogicalVariable(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID] >>= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionShiftRightArithmeticVariable(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}



void PXAPI PXMIPSInstructionJumpRegister(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    const PXSize adressVirtual = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
    const PXSize adressPhysical = adressVirtual - (PXSize)pxMIPSProcessor->ROMOffsetVirtual;

    const PXSize validAdress = 0 == (0b11 & adressVirtual); // Must be word alligned!

    if(!validAdress)
    {
        PXDebugBreak();
    }

    PXMIPSInstructionExecuteDeleay(pxMIPSProcessor);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXMIPSProcessorName,
        "Jump Register",
        "Jump to 0x%8.8X (offset:%i)",
        adressVirtual,
        adressPhysical
    );
#endif
    pxMIPSProcessor->ProgramCounter = (void*)adressPhysical;

    pxMIPSTInstruction->IncrmentCounter = PXFalse; // Skip the +4 offset
}

void PXAPI PXMIPSInstructionJumpAndLinkRegister(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionSystemCall(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionBreak(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionSynchronize(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionMoveFromHI(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionMoveToHI(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionMoveFromLO(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionMoveToLO(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftLeftLogicalVariable(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftRightLogicalVariable(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftRightArithmeticVariable(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionMultiply(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] *= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionMultiplyUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] *= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionDivide(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] /= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    //pxMIPSProcessor->Register[pxMIPSTInstruction->RegisterDestination] = pxMIPSProcessor->Register[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionDivideUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] /= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
}

void PXAPI PXMIPSInstructionDoublewordMultiply(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordMultiplyUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordDivide(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordDivideUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionADD(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
    The contents of general purpose register rs and the contents of general purpose
    register rt are added to store the result in general purpose register rd. In 64-bit
    mode, the operands must be sign-extended, 32-bit values.
    An integer overflow exception occurs if the carries out of bits 30 and 31 differ (2’s
    complement overflow). The contents of destination register rd is not modified when an integer overflow exception occurs.
    */

    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] += pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];

}

void PXAPI PXMIPSInstructionADDUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] += pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionSubtract(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] -= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionSubtractUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] -= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionAND(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
    The contents of general purpose register rs are combined with the contents of general purpose register rt in a bit-wise logical AND operation.
    The result is stored in general purpose register rd.
    */

    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] &= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionOR(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    /*
    A logical OR operation applied between the contents of general purpose registers rs and rt is executed in bit unites.
    The result is stored in general purpose register rd.
    */

    if(pxMIPSTInstruction->ShiftAmount) // Expected to be 0
    {
        PXDebugBreak();
    }

    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID] ^= pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
    pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterSourceID];
}

void PXAPI PXMIPSInstructionExclusiveOR(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionNOR(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionSetOnLessThan(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionSetOnLessThanUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordADD(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordADDUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordSubtract(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordSubtractUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqual(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionTrapIfLessThan(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionTrapIfLessThanUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionTrapIfEqual(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionTrapIfNotEqual(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftLeftLogical(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftRightLogical(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftRightArithmetic(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftLeftLogicalPlus32(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftRight(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

void PXAPI PXMIPSInstructionDoublewordShiftRightArithmeticPlus32(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
    PXDebugBreak();
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZero(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchLessThanZero;
     pxMIPSBranch.Likely = PXFalse;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZero(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchGreaterThanOrEqualZero;
     pxMIPSBranch.Likely = PXFalse;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZeroLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchLessThanZero;
     pxMIPSBranch.Likely = PXTrue;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchGreaterThanOrEqualZero;
     pxMIPSBranch.Likely = PXTrue;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXDebugBreak();
}

 void PXAPI PXMIPSInstructionTrapIfGreaterThanOrEqualImmediateUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXDebugBreak();
}

 void PXAPI PXMIPSInstructionTrapIfLessThanImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXDebugBreak();
}

 void PXAPI PXMIPSInstructionTrapIfLessThanImmediateUnsigned(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXDebugBreak();
}

 void PXAPI PXMIPSInstructionTrapIfEqualImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXDebugBreak();
}

 void PXAPI PXMIPSInstructionTrapIfNotEqualImmediate(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXDebugBreak();
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZeroAndLink(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchLessThanZero;
     pxMIPSBranch.Likely = PXFalse;
     pxMIPSBranch.CompareToRegister = PXTrue;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLink(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchLessThanOrEqualZero;
     pxMIPSBranch.Likely = PXFalse;
     pxMIPSBranch.CompareToRegister = PXTrue;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionBranchOnLessThanZeroAndLinkLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchLessThanZero;
     pxMIPSBranch.Likely = PXTrue;
     pxMIPSBranch.CompareToRegister = PXTrue;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionBranchOnGreaterThanOrEqualToZeroAndLinkLikely(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
{
     PXMIPSBranch pxMIPSBranch;
     pxMIPSBranch.Mode = PXMIPSBranchLessThanOrEqualZero;
     pxMIPSBranch.Likely = PXTrue;
     pxMIPSBranch.CompareToRegister = PXTrue;

     PXMIPSBranchCalc(pxMIPSProcessor, pxMIPSTInstruction, &pxMIPSBranch);
}

 void PXAPI PXMIPSInstructionMoveWordFromFPUCoprocessor1(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
 {


    /*
    The contents of PXF32ing-point general purpose register fs are stored to the general purpose register rt of the CPU register rt.
    The contents of general purpose register rt are undefined while the instruction
    immediately following this instruction is being executed.
    If the FR bit of the Status register is 0 and if the least-significant bit in the ft field
    is 0, the low-order 32 bits of PXF32ing-point register ft are stored to the general
    purpose register rt. If the least-significant bit in the ft area is 1, the high-order 32
    bits of PXF32ing-point register ft-1 are stored to the general purpose register rt.
    If the FR bit is 1, all the 64-bit PXF32ing-point registers can be accessed; therefore,
    the low-order 32 bits of PXF32ing-point register ft are stored to the general purpose register rt.
    */

     // Check Coprocessor and throw "unusable" as an exception it it is not.


     PXF32* source = &pxMIPSProcessor->RegisterPXF32[pxMIPSTInstruction->FS];
     PXF32* target = &pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];

     // As we store a PXF32 inside a normal int space, the value is garbage until read as an PXF32.
     // As a PXF32 is 4 byte, the rest 4 byte of the 64-bit register is undefined.
     *target = *source;




     const PXBool a = 0 == (PXMIPSRegisterStatusFR & pxMIPSProcessor->RegisterStatus);
     const PXBool b = 0 == (0b01 == pxMIPSProcessor->RegisterPXF32[PXMIPSRegisterFT]);


#if PXLogEnable
     PXLogPrint
     (
         PXLoggingInfo,
         PXMIPSProcessorName,
         "Coprocessor-1",
         "--"
     );
#endif

 }

 void PXAPI PXMIPSInstructionDoublewordMoveFromSystemControlCoprocessor(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
 {
     PXDebugBreak();
 }

 void PXAPI PXMIPSInstructionMoveControlWordFromCoprocessorZ(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
 {
     PXMIPSCoProcessor PXREF pxMIPSCoProcessor = &pxMIPSProcessor->CoProcessor[pxMIPSTInstruction->CoProcessorID];

     if(pxMIPSTInstruction->CoProcessorID == 0) // This instruction is not valid for CP0
     {
         PXDebugBreak();
     }

     pxMIPSCoProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
 }

 void PXAPI PXMIPSInstructionMoveToSystemControlCoprocessor(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
 {
     PXMIPSCoProcessor PXREF pxMIPSCoProcessor = &pxMIPSProcessor->CoProcessor[pxMIPSTInstruction->CoProcessorID];

     if(!pxMIPSCoProcessor->Enabled)
     {
         PXDebugBreak();
     }

     // The contents of general purpose register rt are loaded into general purpose register rd of CP0.
     pxMIPSCoProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
 }

 void PXAPI PXMIPSInstructionDoublewordMoveToSystemControlCoprocessor(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
 {
     PXDebugBreak();
 }

 void PXAPI PXMIPSInstructionMoveControlToCoprocessorZ(PXMIPSProcessor PXREF pxMIPSProcessor, PXMIPSTInstruction PXREF pxMIPSTInstruction)
 {
     PXMIPSCoProcessor PXREF pxMIPSCoProcessor = &pxMIPSProcessor->CoProcessor[pxMIPSTInstruction->CoProcessorID];

     if(pxMIPSTInstruction->CoProcessorID == 0) // This instruction is not valid for CP0
     {
         PXDebugBreak();
     }

     if(!pxMIPSCoProcessor->Enabled)
     {
         PXDebugBreak();
     }

     pxMIPSCoProcessor->RegisterList[pxMIPSTInstruction->RegisterDestinationID] = pxMIPSProcessor->RegisterList[pxMIPSTInstruction->RegisterTargetID];
 }

#include "PXX86.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Media/COFF/PXCOFF.h>


const char* RegisterNameList[] =
{
    "RAX",
    "RCX",
    "RDX",
    "RBX",
    "RSP",
    "RBP",
    "RSI",
    "RDI"
};

const char PXX86Name[] = "X86-X64";
const char PXX86Iitile[] = "Disassemble";


void PXAPI PXX86InstructionSUBImidate(PXX86Iterator PXREF pxX86Iterator)
{
    PXX86ModRM pxX86ModRM;

    PXI32U amount = 0; 

    PXX86InstructionFunctionMODRMRead(pxX86Iterator, &pxX86ModRM);

    PXFileReadI32U(pxX86Iterator->Data, &amount);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXX86Name,
        PXX86Iitile,
        "SUB-Imidate : HEX:<%8.8X>, DEZ:<%i>",
        amount,
        amount
    );
#endif
}

void PXAPI PXX86InstructionInvoke(PXX86Iterator PXREF pxX86Iterator)
{
    const PXSize size = PXTypeSizeMask & pxX86Iterator->InstructionCurrent->Type;

    if(pxX86Iterator->InstructionCurrent->HandlerFunction)
    {
        pxX86Iterator->InstructionCurrent->HandlerFunction(pxX86Iterator);
    }
    else
    {
        PXFileCursorAdvance(pxX86Iterator->Data, size);
    }
}

void PXAPI PXX86InstructionFunctionMODRMRead(PXX86Iterator PXREF pxX86Iterator, PXX86ModRM PXREF pxX86ModRM)
{
    PXI8U modRM = 0;

    PXFileReadI8U(pxX86Iterator->Data, &modRM);

    pxX86ModRM->Mod = (0b11000000 & modRM) >> 5;
    pxX86ModRM->RegisterIndex = (0b00111000 & modRM) >> 3;
    pxX86ModRM->RM = (0b00000111 & modRM) >> 0;

    pxX86ModRM->Name = RegisterNameList[pxX86ModRM->RegisterIndex];
}

void PXAPI PXX86InstructionFunctionMOVx3(PXX86Iterator PXREF pxX86Iterator)
{
    PXX86ModRM pxX86ModRM;

    PXI8U SIB = 0;
    PXI8U offset = 0;


    PXX86InstructionFunctionMODRMRead(pxX86Iterator, &pxX86ModRM);

    // Read 3 bytes
    PXFileReadI8U(pxX86Iterator->Data, &SIB);
    PXFileReadI8U(pxX86Iterator->Data, &offset);


    PXI8U scale = (0b11000000 & SIB) >> 5;
    PXI8U index = (0b00111000 & SIB) >> 3;
    PXI8U base  = (0b00000111 & SIB) >> 0;



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXX86Name,
        PXX86Iitile,
        "MOV : %s -> %s",
        pxX86ModRM.Name,
        "***"
    );
#endif

    /*
        54: Specifies the source and destination registers.

        The ModR / M byte can be broken down into fields : MOD, REG, and RM.

        MOD = 01b(memory operand with 8 - bit displacement)

        REG = 010b(source register, in this case, RDX)

        RM = 100b(base register with SIB)

        24: SIB byte indicating[RSP] as the base register and no index register.

        10 : 8 - bit displacement(16 bytes).
    */

}

void PXAPI PXX86InstructionFunctionREXMOV(PXX86Iterator PXREF pxX86Iterator)
{
    // HANDLE??

    PXX86InstructionNext(pxX86Iterator);

    PXX86InstructionInvoke(pxX86Iterator);
}

void PXAPI PXX86InstructionRET(PXX86Iterator PXREF pxX86Iterator)
{
    PXSize returnAdress = 0;

    // Reduce stack by amount and discard 
    PXListExtractAndReduce(&pxX86Iterator->Stack, PXNull, pxX86Iterator->Immediate.I32S);

    // Fetch the adress of the stack
    PXListExtractAndReduce(&pxX86Iterator->Stack, &returnAdress, sizeof(PXSize));


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXX86Name,
        PXX86Iitile,
        "Reduce stack by <%i> Bytes and return to <%p>",
        pxX86Iterator->Immediate.I32S,
        returnAdress
    );
#endif

    PXFileCursorMoveTo(pxX86Iterator->Data, returnAdress);
}

void PXAPI PXX86InstructionCall(PXX86Iterator PXREF pxX86Iterator)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXX86Name,
        PXX86Iitile,
        "CALL offset : HEX:<%8.8X> DEZ:<%i>",
        pxX86Iterator->Immediate.I32S,
        pxX86Iterator->Immediate.I32S
    );
#endif

    PXSize data = PXFileDataPosition(pxX86Iterator->Data);

    // Store the call to be able to return to 
    PXListAppend
    (
        &pxX86Iterator->Stack, 
        &data,
        sizeof(data)
    );

    PXFileCursorOffset(pxX86Iterator->Data, pxX86Iterator->Immediate.I32S);
}

void PXAPI PXX86InstructionJMPNEAR(PXX86Iterator PXREF pxX86Iterator)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXX86Name,
        PXX86Iitile,
        "Jump offset : HEX:<%8.8X> DEZ:<%i>",
        pxX86Iterator->Immediate.I32S,
        pxX86Iterator->Immediate.I32S
    );
#endif

    PXFileCursorOffset(pxX86Iterator->Data, pxX86Iterator->Immediate.I32S);
}

// Read first Bits/byte
// One-byte Opcode Map: (00H — F7H) 
// LookupTable[256];
// One-byte Opcode Map: (08H — FFH)
// LookupTable[256];

// This is the root of all instructions, the first byte.
// Instead of a switch, we can just use this as a fast lookup table.
// As 256 byte are a better deal then alot of compares and jumps-
const PXX86Instruction PXX86InstructionListRoot[] = // 0xFF
{
  // 0x0?
{PXNull, "ADD", PXNull, 0}, // 0x00
{PXNull, "ADD", PXNull, 0}, // 0x01
{PXNull, "ADD", PXNull, 0}, // 0x02
{PXNull, "ADD", PXNull, 0}, // 0x03
{PXNull, "ADD", PXNull, 0}, // 0x04
{PXNull, "ADD", PXNull, 0}, // 0x05
{PXNull, "PUSH", PXNull, 0}, // 0x06
{PXNull, "POP", PXNull, 0}, // 0x07
{PXNull, "OR", PXNull, 0}, // 0x08
{PXNull, "OR", PXNull, 0}, // 0x09
{PXNull, "OR", PXNull, 0}, // 0x0A
{PXNull, "OR", PXNull, 0}, // 0x0B
{PXNull, "OR", PXNull, 0}, // 0x0C
{PXNull, "OR", PXNull, 0}, // 0x0D
{PXNull, "PUSH", PXNull, 0}, // 0x0E
{PXNull, "ESC", PXNull, 0}, // 0x0F

// 0x1?
{PXNull, "ADC", PXNull, 0}, // 0x10
{PXNull, "ADC", PXNull, 0}, // 0x11
{PXNull, "ADC", PXNull, 0}, // 0x12
{PXNull, "ADC", PXNull, 0}, // 0x13
{PXNull, "ADC", PXNull, 0}, // 0x14
{PXNull, "ADC", PXNull, 0}, // 0x15
{PXNull, "PUSH", PXNull, 0}, // 0x16
{PXNull, "POP", PXNull, 0}, // 0x17
{PXNull, "SBB", PXNull, 0}, // 0x18
{PXNull, "SBB", PXNull, 0}, // 0x19
{PXNull, "SBB", PXNull, 0}, // 0x1A
{PXNull, "SBB", PXNull, 0}, // 0x1B
{PXNull, "SBB", PXNull, 0}, // 0x1C
{PXNull, "SBB", PXNull, 0}, // 0x1D
{PXNull, "PUSH", PXNull, 0}, // 0x1E
{PXNull, "***", "Prefix", 0}, // 0x1F

// 0x2?
{PXNull, "AND", PXNull, 0}, // 0x20
{PXNull, "AND", PXNull, 0}, // 0x21
{PXNull, "AND", PXNull, 0}, // 0x22
{PXNull, "AND", PXNull, 0}, // 0x23
{PXNull, "AND", PXNull, 0}, // 0x24
{PXNull, "AND", PXNull, 0}, // 0x25
{PXNull, "Prefix", PXNull, 0}, // 0x26
{PXNull, "DAA", PXNull, 0}, // 0x27
{PXNull, "SUB", PXNull, 0}, // 0x28
{PXNull, "SUB", PXNull, 0}, // 0x29
{PXNull, "SUB", PXNull, 0}, // 0x2A
{PXNull, "SUB", PXNull, 0}, // 0x2B
{PXNull, "SUB", PXNull, 0}, // 0x2C
{PXNull, "SUB", PXNull, 0}, // 0x2D
{PXNull, "SEG-CS", PXNull, 0}, // 0x2E
{PXNull, "DAS", PXNull, 0}, // 0x2F
// 0x3?
{PXNull, "XOR", PXNull, 0}, // 0x30
{PXNull, "XOR", PXNull, 0}, // 0x31
{PXNull, "XOR", PXNull, 0}, // 0x32
{PXNull, "XOR", PXNull, 0}, // 0x33
{PXNull, "XOR", PXNull, 0}, // 0x34
{PXNull, "XOR", PXNull, 0}, // 0x35
{PXNull, "Prefix", PXNull, 0}, // 0x36
{PXNull, "AAA", PXNull, 0}, // 0x37
{PXNull, "CMP", PXNull, 0}, // 0x38
{PXNull, "CMP", PXNull, 0}, // 0x39
{PXNull, "CMP", PXNull, 0}, // 0x3A
{PXNull, "CMP", PXNull, 0}, // 0x3B
{PXNull, "CMP", PXNull, 0}, // 0x3C
{PXNull, "CMP", PXNull, 0}, // 0x3D
{PXNull, "SEG-DS", PXNull, 0}, // 0x3E
{PXNull, "AAS", PXNull, 0}, // 0x3F
// 0x3?
{PXNull, "INC", PXNull, 0}, // 0x40
{PXNull, "INC", PXNull, 0}, // 0x41
{PXNull, "INC", PXNull, 0}, // 0x42
{PXNull, "INC", PXNull, 0}, // 0x43
{PXNull, "INC", PXNull, 0}, // 0x44
{PXNull, "INC", PXNull, 0}, // 0x45
{PXNull, "INC", PXNull, 0}, // 0x46
{PXNull, "INC", PXNull, 0}, // 0x47
{PXX86InstructionFunctionREXMOV, "MOV", "REX.W eAX", 1}, // 0x48
{PXX86InstructionFunctionREXMOV, "MOV", "REX.WB eCX", 1}, // 0x49
{PXX86InstructionFunctionREXMOV, "MOV", "REX.WX eDX", 1}, // 0x4A
{PXX86InstructionFunctionREXMOV, "MOV", "REX.WXB eBX", 1}, // 0x4B
{PXX86InstructionFunctionREXMOV, "MOV", "REX.WR eSP", 1}, // 0x4C
{PXX86InstructionFunctionREXMOV, "MOV", "REX.WRB eBP", 1}, // 0x4D
{PXX86InstructionFunctionREXMOV, "DEC", "REX.WRX eSI", 1}, // 0x4E
{PXX86InstructionFunctionREXMOV, "DEC", "REX.RXB eDI", 1}, // 0x4F

// 0x0?
{PXNull, "PUSH", "rAX", 0}, // 0x50
{PXNull, "PUSH", "rCX", 0}, // 0x51
{PXNull, "PUSH", "rDX", 0}, // 0x52
{PXNull, "PUSH", "rBX", 0}, // 0x53
{PXNull, "PUSH", "rSP", 0}, // 0x54
{PXNull, "PUSH", "rBP", 0}, // 0x55
{PXNull, "PUSH", "rSI", 0}, // 0x56
{PXNull, "PUSH", "rDI", 0}, // 0x57
{PXNull, "POP", PXNull, 0}, // 0x58
{PXNull, "POP", PXNull, 0}, // 0x59
{PXNull, "POP", PXNull, 0}, // 0x5A
{PXNull, "POP", PXNull, 0}, // 0x5B
{PXNull, "POP", PXNull, 0}, // 0x5C
{PXNull, "POP", PXNull, 0}, // 0x5D
{PXNull, "POP", PXNull, 0}, // 0x5E
{PXNull, "POP", PXNull, 0}, // 0x5F
// 0x6?
{PXNull, "PUSHA", PXNull, 0}, // 0x60
{PXNull, "POPA", PXNull, 0}, // 0x61
{PXNull, "BOUND", PXNull, 0}, // 0x62
{PXNull, "ARPL", PXNull, 0}, // 0x63
{PXNull, "SEG-FS", PXNull, 0}, // 0x64
{PXNull, "SEG-GS", PXNull, 0}, // 0x65
{PXNull, "Prefix-OperadSize", PXNull, 0}, // 0x66
{PXNull, "Prefix-AdressSize", PXNull, 0}, // 0x67
{PXNull, "PUSH", PXNull, 0}, // 0x68
{PXNull, "IMUL", PXNull, 0}, // 0x69
{PXNull, "PUSH", PXNull, 0}, // 0x6A
{PXNull, "IMUL", PXNull, 0}, // 0x6B
{PXNull, "INS", PXNull, 0}, // 0x6C
{PXNull, "INS", PXNull, 0}, // 0x6D
{PXNull, "OUTS", PXNull, 0}, // 0x6E
{PXNull, "OUTS", PXNull, 0}, // 0x6F
// 0x0?
{PXNull, "JMP-O", PXNull, 0}, // 0x70
{PXNull, "JMP-NO", PXNull, 0}, // 0x71
{PXNull, "JMP", PXNull, 0}, // 0x72
{PXNull, "JMP", PXNull, 0}, // 0x73
{PXNull, "JMP", PXNull, 0}, // 0x74
{PXNull, "JMP", PXNull, 0}, // 0x75
{PXNull, "JMP", PXNull, 0}, // 0x76
{PXNull, "JMP", PXNull, 0}, // 0x77
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x78
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x79
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x7A
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x7B
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x7C
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x7D
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x7E
{PXNull, "JMP-OnCondition", PXNull, 0}, // 0x7F
// 0x0?
{ PXX86InstructionSUBImidate, "SUB", "Immeditate", 4 }, // 0x80
{ PXX86InstructionSUBImidate, "SUB", "Immeditate", 4}, // 0x81
{ PXX86InstructionSUBImidate, "SUB", "Immeditate", 4}, // 0x82
{ PXX86InstructionSUBImidate, "SUB", "Immeditate", 4}, // 0x83
{PXNull, "TEST", PXNull, 0}, // 0x84
{PXNull, "TEST", PXNull, 0}, // 0x85
{PXNull, "XCHG", PXNull, 0}, // 0x86
{PXNull, "XCHG", PXNull, 0}, // 0x87
{ PXX86InstructionFunctionMOVx3, "MOV", PXNull, 3}, // 0x88
{ PXX86InstructionFunctionMOVx3, "MOV", PXNull, 3}, // 0x89
{ PXX86InstructionFunctionMOVx3, "MOV", PXNull, 3}, // 0x8A
{ PXX86InstructionFunctionMOVx3, "MOV", PXNull, 3}, // 0x8B
{PXNull, "MOV", PXNull, 0}, // 0x8C
{PXNull, "LEA", PXNull, 0}, // 0x8D
{PXNull, "MOV", PXNull, 0}, // 0x8E
{PXNull, "POP", PXNull, 0}, // 0x8F
// 0x0?
{PXNull, "NOP", PXNull, 0}, // 0x90
{PXNull, "XCHG", PXNull, 0}, // 0x91
{PXNull, "XCHG", PXNull, 0}, // 0x92
{PXNull, "XCHG", PXNull, 0}, // 0x93
{PXNull, "XCHG", PXNull, 0}, // 0x94
{PXNull, "XCHG", PXNull, 0}, // 0x95
{PXNull, "XCHG", PXNull, 0}, // 0x96
{PXNull, "XCHG", PXNull, 0}, // 0x97
{PXNull, "CWD", PXNull, 0}, // 0x98
{PXNull, "CWD", PXNull, 0}, // 0x99
{PXNull, "CALL-FAR", PXNull, 0}, // 0x9A
{PXNull, "FWAIT", PXNull, 0}, // 0x9B
{PXNull, "PUSHF", PXNull, 0}, // 0x9C
{PXNull, "POPF", PXNull, 0 }, // 0x9D
{PXNull, "SAHF", PXNull, 0 }, // 0x9E
{PXNull, "LAHF", PXNull, 0 }, // 0x9F
// 0x0?
{PXNull, "MOV", PXNull, 0 }, // 0xA0
{PXNull, "MOV", PXNull, 0 }, // 0xA1
{PXNull, "MOV", PXNull, 0 }, // 0xA2
{PXNull, "MOV", PXNull, 0 }, // 0xA3
{PXNull, "MOVS", PXNull, 0 }, // 0xA4
{PXNull, "MOVS", PXNull, 0 }, // 0xA5
{PXNull, "CMPS", PXNull, 0 }, // 0xA6
{PXNull, "CMPS", PXNull, 0 }, // 0xA7
{PXNull, "TEST", PXNull, 0 }, // 0xA8
{PXNull, "TEST", PXNull, 0 }, // 0xA9
{PXNull, "STOS", PXNull, 0 }, // 0xAA
{PXNull, "STOS", PXNull, 0 }, // 0xAB
{PXNull, "LODS", PXNull, 0 }, // 0xAC
{PXNull, "LODS", PXNull, 0 }, // 0xAD
{PXNull, "SCAS", "Scan String"}, // 0xAE
{PXNull, "SCAS", "Scan String"}, // 0xAF
// 0x0?
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB0
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB1
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB2
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB3
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB4
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB5
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB6
{PXNull, "MOV", "immediate byte into byte register"}, // 0xB7
{PXNull, "MOV", PXNull, 0 }, // 0xB8
{PXNull, "MOV", PXNull, 0 }, // 0xB9
{PXNull, "MOV", PXNull, 0 }, // 0xBA
{PXNull, "MOV", PXNull, 0 }, // 0xBB
{PXNull, "MOV", PXNull, 0 }, // 0xBC
{PXNull, "MOV", PXNull, 0 }, // 0xBD
{PXNull, "MOV", PXNull, 0 }, // 0xBE
{PXNull, "MOV", PXNull, 0 }, // 0xBF
// 0x0?
{PXNull, "SHIFT", PXNull, 0 }, // 0xC0
{PXNull, "SHIFT", PXNull, 0 }, // 0xC1
{ PXX86InstructionRET, "RET-NEAR", PXNull, PXX86InstructionImmediate4 }, // 0xC2
{PXNull, "RET-NEAR", PXNull, 0 }, // 0xC3
{PXNull, "LES", PXNull, 0 }, // 0xC4
{PXNull, "LDS", PXNull, 0 }, // 0xC5
{PXNull, "MOV", PXNull, 0 }, // 0xC6
{PXNull, "MOV", PXNull, 0 }, // 0xC7
{PXNull, "ENTER", PXNull, 0 }, // 0xC8
{PXNull, "LEAVE", PXNull, 0 }, // 0xC9
{PXNull, "RET-FAR", PXNull, 0 }, // 0xCA
{PXNull, "RET-FAR", PXNull, 0 }, // 0xCB
{PXNull, "INT3", PXNull, PXTypePadding(4)}, // 0xCC
{PXNull, "INT", PXNull, 0 }, // 0xCD
{PXNull, "INTO", PXNull, 0 }, // 0xCE
{PXNull, "IRET", PXNull, 0 }, // 0xCF
// 0x0?
{PXNull, "SHIFT", PXNull, 0 }, // 0xD0
{PXNull, "SHIFT", PXNull, 0 }, // 0xD1
{PXNull, "SHIFT", PXNull, 0 }, // 0xD2
{PXNull, "SHIFT", PXNull, 0 }, // 0xD3
{PXNull, "AAM", PXNull, 0 }, // 0xD4
{PXNull, "AAD", PXNull, 0 }, // 0xD5
{PXNull, PXNull, PXNull, 0 }, // 0xD6
{PXNull, "XLAT", PXNull, 0 }, // 0xD7
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xD8
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xD9
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xDA
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xDB
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xDC
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xDD
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xDE
{PXNull, "COPROCESSOR-Prefix", 0 }, // 0xDF
// 0x0?
{PXNull, "LOOPNE", PXNull, 0 }, // 0xE0
{PXNull, "LOOPPE", PXNull, 0 }, // 0xE1
{PXNull, "LOOP", PXNull, 0 }, // 0xE2
{PXNull, "CXZ", PXNull, 0 }, // 0xE3
{PXNull, "IN", PXNull, 0 }, // 0xE4
{PXNull, "IN", PXNull, 0 }, // 0xE5
{PXNull, "OUT", PXNull, 0 }, // 0xE6
{PXNull, "OUT", PXNull, 0 }, // 0xE7
{ PXX86InstructionCall, "CALL-NEAR", "", PXTypeI32SLE }, // 0xE8, Data: 32-Bit offset, IP_next = IP_now + opsize + offset   
{ PXX86InstructionJMPNEAR, "JMP-NEAR", PXNull, PXTypeI32SLE }, // 0xE9
{PXNull, "JMP-FAR", PXNull, 0 }, // 0xEA
{PXNull, "JMP-SHORT", PXNull, 0 }, // 0xEB
{PXNull, "IN", PXNull, 0 }, // 0xEC
{PXNull, "IN", PXNull, 0 }, // 0xED
{PXNull, "OUT", PXNull, 0 }, // 0xEE
{PXNull, "OUT", PXNull, 0 }, // 0xEF
// 0xF?
{PXNull, "LOCK", PXNull, 0 }, // 0xF0
{PXNull, "INT1", PXNull, 0 }, // 0xF1
{PXNull, "Prefix", PXNull, 0 }, // 0xF2
{PXNull, "Prefix", PXNull, 0 }, // 0xF3
{PXNull, "HLT", PXNull, 0 }, // 0xF4
{PXNull, "CCMC", PXNull, 0 }, // 0xF5
{PXNull, "Prefix", PXNull, 0 }, // 0xF6
{PXNull, "Prefix", PXNull, 0 }, // 0xF7
{PXNull, "CLC", PXNull, 0 }, // 0xF8
{PXNull, "STC", PXNull, 0 }, // 0xF9
{PXNull, "CLI", PXNull, 0 }, // 0xFA
{PXNull, "STI", PXNull, 0 }, // 0xFB
{PXNull, "CLS", PXNull, 0 }, // 0xFC
{PXNull, "STD", PXNull, 0 }, // 0xFD
{PXNull, "INC/DEC", PXNull, 0 }, // 0xFE
{PXNull, "INC/DEC", PXNull, 0 } // 0xFF
};

/*
// Simple arichmetic
const char PXX86InstructionListD8Low[] = 
{
  FADDSingleReal   = 0b000,
  FMUL_single-real= 0b001,
  FCOM_single-real= 0b010,
  FCOMP_single-real= 0b011,
  FSUB_single-real= 0b100,
  FSUBR_single-real= 0b101,
  FDIV_single-real= 0b110,
  FDIVR_single-rea= 0b111,
}

// Store data
const char PXX86InstructionListD8High[] = 
{
  FADDSingleReal   = 0b000,
  FMUL_single-real= 0b001,
  FCOM_single-real= 0b010,
  FCOMP_single-real= 0b011,
  FSUB_single-real= 0b100,
  FSUBR_single-real= 0b101,
  FDIV_single-real= 0b110,
  FDIVR_single-rea= 0b111,
}
*/

PXResult PXAPI PXX86InstructionNext(PXX86Iterator PXREF pxX86Iterator)
{
    PXFileReadI8U(pxX86Iterator->Data, &pxX86Iterator->OperationCode);

    pxX86Iterator->InstructionCurrent = &PXX86InstructionListRoot[pxX86Iterator->OperationCode];
    
    // if RAX, parse
    if(PXX86InstructionREX & pxX86Iterator->InstructionCurrent->Type)
    {
        // Do sup call

    }

    switch(PXX86InstructionDisplacementMask & pxX86Iterator->InstructionCurrent->Type)
    {
        case PXX86InstructionDisplacement1:
            PXFileReadI8S(pxX86Iterator->Data, &pxX86Iterator->Displacement.I8S);
            break;
        case PXX86InstructionDisplacement2:
            PXFileReadI16SE(pxX86Iterator->Data, &pxX86Iterator->Displacement.I16S, PXEndianLittle);
            break;
        case PXX86InstructionDisplacement4:
            PXFileReadI32SE(pxX86Iterator->Data, &pxX86Iterator->Displacement.I32S, PXEndianLittle);
            break;
    }

    switch(PXX86InstructionImmediateMask & pxX86Iterator->InstructionCurrent->Type)
    {
        case PXX86InstructionImmediate1:      
            PXFileReadI8S(pxX86Iterator->Data, &pxX86Iterator->Immediate.I8S);
            break;
        case PXX86InstructionImmediate2:
            PXFileReadI16SE(pxX86Iterator->Data, &pxX86Iterator->Immediate.I16S, PXEndianLittle);
            break;
        case PXX86InstructionImmediate4:
            PXFileReadI32SE(pxX86Iterator->Data, &pxX86Iterator->Immediate.I32S, PXEndianLittle);
            break;
    }
}

PXResult PXAPI PXX86InstructionDisassemble(PXX86Iterator PXREF pxX86Iterator)
{
    PXX86InstructionNext(pxX86Iterator);

    const PXSize size = PXTypeSizeGet(pxX86Iterator->InstructionCurrent->Type);

#if PXLogEnable

    char buffer[64];
    PXSize offset = 0;
    PXI8U dataByte = 0;

    offset += PXTextPrintA(buffer, 64, "%2.2X", pxX86Iterator->OperationCode);

    for(size_t i = 0; i < size; ++i)
    {
        PXFileReadI8U(pxX86Iterator->Data, &dataByte);

        offset += PXTextPrintA(buffer + offset, 64, " %2.2X", dataByte);
    }

    PXFileCursorRewind(pxX86Iterator->Data, size);

    PXLogPrint
    (
        PXLoggingInfo,
        "X86",
        "Disassemble",
        "%p - %-20s - %-10s - %s",
        pxX86Iterator->VirtualAdress,
        buffer,
        pxX86Iterator->InstructionCurrent->Name,
        pxX86Iterator->InstructionCurrent->Description
    );
#endif

    PXX86InstructionInvoke(pxX86Iterator);








     // How to parse:
     // Read the first byte to get the correct table
     // If applyable, select correct element by seperating with value 0xBF
     // When smaler: Read the lowest 3 bits as a command, the rest is the data
     // When bigger: Use the "left" nible as the row index, the "right" nible as the coloum index

     // Valid first bytes are in the range of :
     // HEX: 0xD8 to 0xDF
     // DEZ: 216 to 223 (7 Elements)

     /*

     switch (data[0])
     {
       case 0xD8:    // DEZ:216
           // When 2nd byte is smaler as 0xBF
           PXX86InstructionListD8Low
           // When bigger
           PXX86InstructionListD8High
         break;

       case 0xD9:  // DEZ:217
         {

            // When 2nd byte is smaler as 0xBF
             FLD_SR = 0b000
           UNDEFINED = 0b001
           FST_SR= 0b010
   FSTP_SR= 0b011
   FLDENV_14_28B= 0b100
   FLDCW_2B= 0b101
   FSTENV_14_28B= 0b110
   FSTCW_2B= 0b111

                   // When bigger


           break;
         }

       case 0xDA:  // DEZ:218

       case 0xDB: //


     }

     */



    return PXResultOK;
}


PXResult PXAPI PXX86InstructionWalk(PXFile PXREF pxFile, PXSectionTable PXREF pxSectionTable)
{
    PXSize old = PXFileDataPosition(pxFile);
    PXX86Iterator pxX86Iterator;
    PXClear(PXX86Iterator, &pxX86Iterator);
    pxX86Iterator.InstructionCurrent = 0;
    pxX86Iterator.Data = pxFile;

    PXListInitialize(&pxX86Iterator.Stack, sizeof(PXByte), 1024);

    for(PXSize i = 0; i < pxSectionTable->SectionRawDataSize; ++i)
    { 
        PXSize pos = PXFileDataPosition(pxFile);
        pxX86Iterator.VirtualAdress = pxSectionTable->VirtualAddress + (pos - old);

        PXX86InstructionDisassemble(&pxX86Iterator);

    }



    return PXResultOK;
}
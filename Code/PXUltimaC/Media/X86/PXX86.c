// Read first Bits/byte
// One-byte Opcode Map: (00H — F7H) 
// LookupTable[256];
// One-byte Opcode Map: (08H — FFH)
// LookupTable[256];

// This is the root of all instructions, the first byte.
// Instead of a switch, we can just use this as a fast lookup table.
// As 256 byte are a better deal then alot of compares and jumps-
const PXInt8U PXX86InstructionListRoot[0xFF] = 
{
  // 0x0?
{PXNULL, "ADD"}, // 0x00
{PXNULL, "ADD"}, // 0x01
{PXNULL, "ADD"}, // 0x02
{PXNULL, "ADD"}, // 0x03
{PXNULL, "ADD"}, // 0x04
{PXNULL, "ADD"}, // 0x05
{PXNULL, "PUSH"}, // 0x06
{PXNULL, "POP"}, // 0x07
{PXNULL, "OR"}, // 0x08
{PXNULL, "OR"}, // 0x09
{PXNULL, "OR"}, // 0x0A
{PXNULL, "OR"}, // 0x0B
{PXNULL, "OR"}, // 0x0C
{PXNULL, "OR"}, // 0x0D
{PXNULL, "PUSH"}, // 0x0E
{PXNULL, "ESC"}, // 0x0F

// 0x1?
{PXNULL, "ADC"}, // 0x10
{PXNULL, "ADC"}, // 0x11
{PXNULL, "ADC"}, // 0x12
{PXNULL, "ADC"}, // 0x13
{PXNULL, "ADC"}, // 0x14
{PXNULL, "ADC"}, // 0x15
{PXNULL, "PUSH"}, // 0x16
{PXNULL, "POP"}, // 0x17
{PXNULL, "SBB"}, // 0x18
{PXNULL, "SBB"}, // 0x19
{PXNULL, "SBB"}, // 0x1A
{PXNULL, "SBB"}, // 0x1B
{PXNULL, "SBB"}, // 0x1C
{PXNULL, "SBB"}, // 0x1D
{PXNULL, "PUSH"}, // 0x1E
{PXNULL, "***", "Prefix"}, // 0x1F

// 0x2?
{PXNULL, "AND"}, // 0x20
{PXNULL, "AND"}, // 0x21
{PXNULL, "AND"}, // 0x22
{PXNULL, "AND"}, // 0x23
{PXNULL, "AND"}, // 0x24
{PXNULL, "AND"}, // 0x25
{PXNULL, "Prefix"}, // 0x26
{PXNULL, "DAA"}, // 0x27
{PXNULL, "SUB"}, // 0x28
{PXNULL, "SUB"}, // 0x29
{PXNULL, "SUB"}, // 0x2A
{PXNULL, "SUB"}, // 0x2B
{PXNULL, "SUB"}, // 0x2C
{PXNULL, "SUB"}, // 0x2D
{PXNULL, "SEG-CS"}, // 0x2E
{PXNULL, "DAS"}, // 0x2F
// 0x3?
{PXNULL, "XOR"}, // 0x30
{PXNULL, "XOR"}, // 0x31
{PXNULL, "XOR"}, // 0x32
{PXNULL, "XOR"}, // 0x33
{PXNULL, "XOR"}, // 0x34
{PXNULL, "XOR"}, // 0x35
{PXNULL, "Prefix"}, // 0x36
{PXNULL, "AAA"}, // 0x37
{PXNULL, "CMP"}, // 0x38
{PXNULL, "CMP"}, // 0x39
{PXNULL, "CMP"}, // 0x3A
{PXNULL, "CMP"}, // 0x3B
{PXNULL, "CMP"}, // 0x3C
{PXNULL, "CMP"}, // 0x3D
{PXNULL, "SEG-DS"}, // 0x3E
{PXNULL, "AAS"}, // 0x3F
// 0x3?
{PXNULL, "INC"}, // 0x40
{PXNULL, "INC"}, // 0x41
{PXNULL, "INC"}, // 0x42
{PXNULL, "INC"}, // 0x43
{PXNULL, "INC"}, // 0x44
{PXNULL, "INC"}, // 0x45
{PXNULL, "INC"}, // 0x46
{PXNULL, "INC"}, // 0x47
{PXNULL, "DEC"}, // 0x48
{PXNULL, "DEC"}, // 0x49
{PXNULL, "DEC"}, // 0x4A
{PXNULL, "DEC"}, // 0x4B
{PXNULL, "DEC"}, // 0x4C
{PXNULL, "DEC"}, // 0x4D
{PXNULL, "DEC"}, // 0x4E
{PXNULL, "DEC"}, // 0x4F

// 0x0?
{PXNULL, "PUSH"}, // 0x50
{PXNULL, "PUSH"}, // 0x51
{PXNULL, "PUSH"}, // 0x52
{PXNULL, "PUSH"}, // 0x53
{PXNULL, "PUSH"}, // 0x54
{PXNULL, "PUSH"}, // 0x55
{PXNULL, "PUSH"}, // 0x56
{PXNULL, "PUSH"}, // 0x57
{PXNULL, "POP"}, // 0x58
{PXNULL, "POP"}, // 0x59
{PXNULL, "POP"}, // 0x5A
{PXNULL, "POP"}, // 0x5B
{PXNULL, "POP"}, // 0x5C
{PXNULL, "POP"}, // 0x5D
{PXNULL, "POP"}, // 0x5E
{PXNULL, "POP"}, // 0x5F
// 0x6?
{PXNULL, "PUSHA"}, // 0x60
{PXNULL, "POPA"}, // 0x61
{PXNULL, "BOUND"}, // 0x62
{PXNULL, "ARPL"}, // 0x63
{PXNULL, "SEG-FS"}, // 0x64
{PXNULL, "SEG-GS"}, // 0x65
{PXNULL, "Prefix-OperadSize"}, // 0x66
{PXNULL, "Prefix-AdressSize"}, // 0x67
{PXNULL, "PUSH"}, // 0x68
{PXNULL, "IMUL"}, // 0x69
{PXNULL, "PUSH"}, // 0x6A
{PXNULL, "IMUL"}, // 0x6B
{PXNULL, "INS"}, // 0x6C
{PXNULL, "INS"}, // 0x6D
{PXNULL, "OUTS"}, // 0x6E
{PXNULL, "OUTS"}, // 0x6F
// 0x0?
{PXNULL, "JMP-O"}, // 0x70
{PXNULL, "JMP-NO"}, // 0x71
{PXNULL, "JMP"}, // 0x72
{PXNULL, "JMP"}, // 0x73
{PXNULL, "JMP"}, // 0x74
{PXNULL, "JMP"}, // 0x75
{PXNULL, "JMP"}, // 0x76
{PXNULL, "JMP"}, // 0x77
{PXNULL, "JMP-OnCondition"}, // 0x78
{PXNULL, "JMP-OnCondition"}, // 0x79
{PXNULL, "JMP-OnCondition"}, // 0x7A
{PXNULL, "JMP-OnCondition"}, // 0x7B
{PXNULL, "JMP-OnCondition"}, // 0x7C
{PXNULL, "JMP-OnCondition"}, // 0x7D
{PXNULL, "JMP-OnCondition"}, // 0x7E
{PXNULL, "JMP-OnCondition"}, // 0x7F
// 0x0?
{PXNULL, "Immeditate"}, // 0x80
{PXNULL, "Immeditate"}, // 0x81
{PXNULL, "Immeditate"}, // 0x82
{PXNULL, "Immeditate"}, // 0x83
{PXNULL, "TEST"}, // 0x84
{PXNULL, "TEST"}, // 0x85
{PXNULL, "XCHG"}, // 0x86
{PXNULL, "XCHG"}, // 0x87
{PXNULL, "MOV"}, // 0x88
{PXNULL, "MOV"}, // 0x89
{PXNULL, "MOV"}, // 0x8A
{PXNULL, "MOV"}, // 0x8B
{PXNULL, "MOV"}, // 0x8C
{PXNULL, "LEA"}, // 0x8D
{PXNULL, "MOV"}, // 0x8E
{PXNULL, "POP"}, // 0x8F
// 0x0?
{PXNULL, "NOP"}, // 0x90
{PXNULL, "XCHG"}, // 0x91
{PXNULL, "XCHG"}, // 0x92
{PXNULL, "XCHG"}, // 0x93
{PXNULL, "XCHG"}, // 0x94
{PXNULL, "XCHG"}, // 0x95
{PXNULL, "XCHG"}, // 0x96
{PXNULL, "XCHG"}, // 0x97
{PXNULL, "CWD"}, // 0x98
{PXNULL, "CWD"}, // 0x99
{PXNULL, "CALL-FAR"}, // 0x9A
{PXNULL, "FWAIT"}, // 0x9B
{PXNULL, "PUSHF"}, // 0x9C
{PXNULL, "POPF"}, // 0x9D
{PXNULL, "SAHF"}, // 0x9E
{PXNULL, "LAHF"}, // 0x9F
// 0x0?
{PXNULL, "MOV"}, // 0xA0
{PXNULL, "MOV"}, // 0xA1
{PXNULL, "MOV"}, // 0xA2
{PXNULL, "MOV"}, // 0xA3
{PXNULL, "MOVS"}, // 0xA4
{PXNULL, "MOVS"}, // 0xA5
{PXNULL, "CMPS"}, // 0xA6
{PXNULL, "CMPS"}, // 0xA7
{PXNULL, "TEST"}, // 0xA8
{PXNULL, "TEST"}, // 0xA9
{PXNULL, "STOS"}, // 0xAA
{PXNULL, "STOS"}, // 0xAB
{PXNULL, "LODS"}, // 0xAC
{PXNULL, "LODS"}, // 0xAD
{PXNULL, "SCAS", "Scan String"}, // 0xAE
{PXNULL, "SCAS", "Scan String"}, // 0xAF
// 0x0?
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB0
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB1
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB2
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB3
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB4
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB5
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB6
{PXNULL, "MOV", "immediate byte into byte register"}, // 0xB7
{PXNULL, "MOV"}, // 0xB8
{PXNULL, "MOV"}, // 0xB9
{PXNULL, "MOV"}, // 0xBA
{PXNULL, "MOV"}, // 0xBB
{PXNULL, "MOV"}, // 0xBC
{PXNULL, "MOV"}, // 0xBD
{PXNULL, "MOV"}, // 0xBE
{PXNULL, "MOV"}, // 0xBF
// 0x0?
{PXNULL, "SHIFT"}, // 0xC0
{PXNULL, "SHIFT"}, // 0xC1
{PXNULL, "RET-NEAR"}, // 0xC2
{PXNULL, "RET-NEAR"}, // 0xC3
{PXNULL, "LES"}, // 0xC4
{PXNULL, "LDS"}, // 0xC5
{PXNULL, "MOV"}, // 0xC6
{PXNULL, "MOV"}, // 0xC7
{PXNULL, "ENTER"}, // 0xC8
{PXNULL, "LEAVE"}, // 0xC9
{PXNULL, "RET-FAR"}, // 0xCA
{PXNULL, "RET-FAR"}, // 0xCB
{PXNULL, "INT3"}, // 0xCC
{PXNULL, "INT"}, // 0xCD
{PXNULL, "INTO"}, // 0xCE
{PXNULL, "IRET"}, // 0xCF
// 0x0?
{PXNULL, "SHIFT"}, // 0xD0
{PXNULL, "SHIFT"}, // 0xD1
{PXNULL, "SHIFT"}, // 0xD2
{PXNULL, "SHIFT"}, // 0xD3
{PXNULL, "AAM"}, // 0xD4
{PXNULL, "AAD"}, // 0xD5
{PXNULL, PXNULL}, // 0xD6
{PXNULL, "XLAT"}, // 0xD7
{PXNULL, "COPROCESSOR-Prefix"}, // 0xD8
{PXNULL, "COPROCESSOR-Prefix"}, // 0xD9
{PXNULL, "COPROCESSOR-Prefix"}, // 0xDA
{PXNULL, "COPROCESSOR-Prefix"}, // 0xDB
{PXNULL, "COPROCESSOR-Prefix"}, // 0xDC
{PXNULL, "COPROCESSOR-Prefix"}, // 0xDD
{PXNULL, "COPROCESSOR-Prefix"}, // 0xDE
{PXNULL, "COPROCESSOR-Prefix"}, // 0xDF
// 0x0?
{PXNULL, "LOOPNE"}, // 0xE0
{PXNULL, "LOOPPE"}, // 0xE1
{PXNULL, "LOOP"}, // 0xE2
{PXNULL, "CXZ"}, // 0xE3
{PXNULL, "IN"}, // 0xE4
{PXNULL, "IN"}, // 0xE5
{PXNULL, "OUT"}, // 0xE6
{PXNULL, "OUT"}, // 0xE7
{PXNULL, "CALL-NEAR"}, // 0xE8
{PXNULL, "JMP-NEAR"}, // 0xE9
{PXNULL, "JMP-FAR"}, // 0xEA
{PXNULL, "JMP-SHORT"}, // 0xEB
{PXNULL, "IN"}, // 0xEC
{PXNULL, "IN"}, // 0xED
{PXNULL, "OUT"}, // 0xEE
{PXNULL, "OUT"}, // 0xEF
// 0xF?
{PXNULL, "LOCK"}, // 0xF0
{PXNULL, "INT1"}, // 0xF1
{PXNULL, "Prefix"}, // 0xF2
{PXNULL, "Prefix"}, // 0xF3
{PXNULL, "HLT"}, // 0xF4
{PXNULL, "CCMC"}, // 0xF5
{PXNULL, "Prefix"}, // 0xF6
{PXNULL, "Prefix"}, // 0xF7
{PXNULL, "CLC"}, // 0xF8
{PXNULL, "STC"}, // 0xF9
{PXNULL, "CLI"}, // 0xFA
{PXNULL, "STI"}, // 0xFB
{PXNULL, "CLS"}, // 0xFC
{PXNULL, "STD"}, // 0xFD
{PXNULL, "INC/DEC"}, // 0xFE
{PXNULL, "INC/DEC"}, // 0xFF
};

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

void PXAPI PXX86InstructionParse(void* data)
{
  // Read first byte
  PXInt8U opcode = data[0];
  // Extract lower and higher nibble for table lookup 
  PXint8U indexRow = (0b11110000 & opcode) >> 4 ;
  PXint8U indexColoum = (0b00001111 & opcode);

  PXint8U index = indexColoum + (0x0F * indexColoum);
  
  PXX86InstructionListRoot[index];

  
  // How to parse:
  // Read the first byte to get the correct table
  // If applyable, select correct element by seperating with value 0xBF
  // When smaler: Read the lowest 3 bits as a command, the rest is the data
  // When bigger: Use the "left" nible as the row index, the "right" nible as the coloum index

  // Valid first bytes are in the range of :
  // HEX: 0xD8 to 0xDF
  // DEZ: 216 to 223 (7 Elements)
  
  
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
  
}


# PowerPC
> Performance Optimization With Enhanced RISC – Performance Computing

## Info
Instruction set used alternativly to x86-64 on supported processors.

## Structure
As from a RISC language expected, all instructions are 4-Byte long.
The first 6-Bits are defined to be the primary opcode. The rest of the instruction depends on the opcode.

> I-Form (32-Bit)

|OPCode|Immediate|Absolute Address|Link Register|
|:-:|:-:|:-:|:-:|
|6-Bit|24-Bit|1-Bit|1-Bit|

> D-Form (32-Bit)

|OPCode|RT|RA|Immediate|
|:-:|:-:|:-:|:-:|
|6-Bit|5-Bit|5-Bit|16-Bit|

## Instructions
|OpCode|Format|Mnemonic|Desciption|
|:-:|:-:|:-:|:-|
|0x00|-|---|Illegal, Reserved|
|0x01|-|---|---|
|0x02|D|tdi|Trap Doubleword Immediate
|0x03|D|twi|Trap Word Immediate
|0x04|-|---|---|
|0x05|-|---|---|
|0x06|-|---|---|
|0x07|D|mulli|Multiply Low Immediate|
|0x08|D|subfic|Subtract From Immediate Carrying|
|0x09|D|dozi|Difference or Zero Immediate|
|0x0A|D|cmpli|Compare Logical Immediate|
|0x0B|D|cmpi|Compare Immediate|
|0x0C|D|addic|Add Immediate Carrying|
|0x0D|D|addic|Add Immediate Carrying and Record|
|0x0E|D|addi|Add Immediate|
|0x0F|D|addis|Add Immediate Shifted|
|0x10|D|bc|Branch Conditiona|
|0x11|-|sc|System Call|
|0x12|-|b|Branch|
|0x13|-|***| *CR OPCodes|
|0x14|-|rlwimi|Rotate Left Word Imm. then Mask Insert|
|0x15|-|rlwinm|Rotate Left Word Imm. then AND with Mask| 
|0x16|-|rlmi| Rotate Left then Mask Insert|
|0x17|-|rlwnm|Rotate Left Word then AND with Mask|
|0x18|D|ori|OR Immediate|
|0x19|D|oris|OR Immediate Shifted|
|0x1A|D|xori|XOR Immediate|
|0x1B|D|xoris|XOR Immediate Shifted|
|0x1C|D|andi|AND Immediate|
|0x1D|D|andis|AND Immediate Shifted|
|0x1E|-|***|*FX OPCodes|
|0x1F|-|***|*FX OPCodes|
|0x20|D|lwz|Load Word and Zero|
|0x21|D|lwzu|Load Word and Zero with Update|
|0x22|D|lbz|Load Byte and Zero|
|0x23|D|lbzu|Load Byte and Zero with Update|
|0x24|D|stw|Store Word|
|0x25|D|stwu|Store Word with Update|
|0x26|D|stb|Store Byte|
|0x27|D|stbu|Store Byte with Update|
|0x28|D|lhz|Load Half and Zero|
|0x29|D|lhzu|Load Half and Zero with Update|
|0x2A|D|lha|Load Half Algebraic|
|0x2B|D|lhau|Load Half Algebraic with Update|
|0x2C|D|sth|Store Half|
|0x2D|D|sthu|Store Half with Update|
|0x2E|D|lmw|Load Multiple Word|
|0x2F|D|stmw|Store Multiple Word|
|0x30|D|lfs|Load Floating-Point Single|
|0x31|D|lfsu|Load Floating-Point Single with Update|
|0x32|D|lfd|Load Floating-Point Double|
|0x33|D|lfdu|Load Floating-Point Double|
|0x34|D|stfs|Store Floating-Point Single|
|0x35|D|stfsu|Store Floating-Point Single with Update|
|0x36|D|stfd|Store Floating-Point Double|
|0x37|D|stfdu|Store Floating-Point Double with Update|
|0x38|-|---|---|
|0x39|-|---|---|
|0x3A|-|***|*FX DS-Loads OPCodes|
|0x3B|-|***|*FP Single OPCodes|
|0x3C|-|---|---|
|0x3D|-|---|---|
|0x3E|-|***|*FX DS-Stores OPCodes|
|0x3F|-|***|*FP Double OPCodes|

## Links
- [Wikipedia](https://en.wikipedia.org/wiki/PowerPC)
- [IBM](https://web.archive.org/web/20201129235141/https://www.ibm.com/developerworks/systems/library/es-archguide-v2.html)

# X86 / X64 - Assembly

## Info
Assembly language mainly used in desktop conputers.

## Structure
|Prefix|REX|OPCode|ModR/M|SIB|Displacement|Immediate|
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
|optional|optional|1-3 Bytes|1 Byte<br>(if required)|1 Byte<br>(if required)|1,2 or 4 Bytes<br>(if required) | 0,1,2 or 4 Bytes<br>(if required) |

<p>
Instructions have a varriable length.<br>
Read the first Byte and then use a lookuptable to decode the correct logic to execute.<br>
If a prefix code is read, we can read the next Byte to decode with another Table.<br>
This allows us to have an unlimited instuction set that can be extended at any point in time.<br>
Ofcourse, Only a limited set is defined to this point.
</p>

### REX-Prefix
This 1 Byte field adds extra bits to the following ModR/M and SIB for additional values. 

### ModR/M
> ...

### SIB
> ...

## Instructions / OPCodes
### First byte

|OPCode|Length|Short Name| Long Name |
|:-:|-:|:-:|:-|
|0x00|000 B|XXXX|XXXX|
|0x01|000 B|XXXX|XXXX|
|0x02|000 B|XXXX|XXXX|
|0x03|000 B|XXXX|XXXX|
|0x04|000 B|XXXX|XXXX|
|0x05|000 B|XXXX|XXXX|
|0x06|000 B|XXXX|XXXX|
|0x07|000 B|XXXX|XXXX|
|0x08|000 B|XXXX|XXXX|
|0x09|000 B|XXXX|XXXX|
|0x0A|000 B|XXXX|XXXX|
|0x0B|000 B|XXXX|XXXX|
|0x0C|000 B|XXXX|XXXX|
|0x0D|000 B|XXXX|XXXX|
|0x0E|000 B|XXXX|XXXX|
|0x0F|000 B|***|Prefix|
|0x10|000 B|XXXX|XXXX|
|0x11|000 B|XXXX|XXXX|
|0x12|000 B|XXXX|XXXX|
|0x13|000 B|XXXX|XXXX|
|0x14|000 B|XXXX|XXXX|
|0x15|000 B|XXXX|XXXX|
|0x16|000 B|XXXX|XXXX|
|0x17|000 B|XXXX|XXXX|
|0x18|000 B|XXXX|XXXX|
|0x19|000 B|XXXX|XXXX|
|0x1A|000 B|XXXX|XXXX|
|0x1B|000 B|XXXX|XXXX|
|0x1C|000 B|XXXX|XXXX|
|0x1D|000 B|XXXX|XXXX|
|0x1E|000 B|XXXX|XXXX|
|0x1F|000 B|XXXX|XXXX|
|0x20|000 B|XXXX|XXXX|
|0x21|000 B|XXXX|XXXX|
|0x22|000 B|XXXX|XXXX|
|0x23|000 B|XXXX|XXXX|
|0x24|000 B|XXXX|XXXX|
|0x25|000 B|XXXX|XXXX|
|0x26|000 B|XXXX|XXXX|
|0x27|000 B|XXXX|XXXX|
|0x28|000 B|XXXX|XXXX|
|0x29|000 B|XXXX|XXXX|
|0x2A|000 B|XXXX|XXXX|
|0x2B|000 B|XXXX|XXXX|
|0x2C|000 B|XXXX|XXXX|
|0x2D|000 B|XXXX|XXXX|
|0x2E|000 B|XXXX|XXXX|
|0x2F|000 B|XXXX|XXXX|
|0x30|000 B|XXXX|XXXX|
|0x31|000 B|XXXX|XXXX|
|0x32|000 B|XXXX|XXXX|
|0x33|000 B|XXXX|XXXX|
|0x34|000 B|XXXX|XXXX|
|0x35|000 B|XXXX|XXXX|
|0x36|000 B|XXXX|XXXX|
|0x37|000 B|XXXX|XXXX|
|0x38|000 B|XXXX|XXXX|
|0x39|000 B|XXXX|XXXX|
|0x3A|000 B|XXXX|XXXX|
|0x3B|000 B|XXXX|XXXX|
|0x3C|000 B|XXXX|XXXX|
|0x3D|000 B|XXXX|XXXX|
|0x3E|000 B|XXXX|XXXX|
|0x3F|000 B|XXXX|XXXX|
|0x40|000 B|XXXX|XXXX|
|0x41|000 B|XXXX|XXXX|
|0x42|000 B|XXXX|XXXX|
|0x43|000 B|XXXX|XXXX|
|0x44|000 B|XXXX|XXXX|
|0x45|000 B|XXXX|XXXX|
|0x46|000 B|XXXX|XXXX|
|0x47|000 B|XXXX|XXXX|
|0x48|8 B|***|REX-Prefix|
|0x49|000 B|XXXX|XXXX|
|0x4A|000 B|XXXX|XXXX|
|0x4B|000 B|XXXX|XXXX|
|0x4C|000 B|XXXX|XXXX|
|0x4D|000 B|XXXX|XXXX|
|0x4E|000 B|XXXX|XXXX|
|0x4F|000 B|XXXX|XXXX|
|0x50|000 B|XXXX|XXXX|
|0x51|000 B|XXXX|XXXX|
|0x52|000 B|XXXX|XXXX|
|0x53|000 B|XXXX|XXXX|
|0x54|000 B|XXXX|XXXX|
|0x55|000 B|XXXX|XXXX|
|0x56|000 B|XXXX|XXXX|
|0x57|000 B|XXXX|XXXX|
|0x58|000 B|XXXX|XXXX|
|0x59|000 B|XXXX|XXXX|
|0x5A|000 B|XXXX|XXXX|
|0x5B|000 B|XXXX|XXXX|
|0x5C|000 B|XXXX|XXXX|
|0x5D|000 B|XXXX|XXXX|
|0x5E|000 B|XXXX|XXXX|
|0x5F|000 B|XXXX|XXXX|
|0x60|000 B|XXXX|XXXX|
|0x61|000 B|XXXX|XXXX|
|0x62|000 B|XXXX|XXXX|
|0x63|000 B|XXXX|XXXX|
|0x64|000 B|XXXX|XXXX|
|0x65|000 B|XXXX|XXXX|
|0x66|000 B|XXXX|XXXX|
|0x67|000 B|XXXX|XXXX|
|0x68|000 B|XXXX|XXXX|
|0x69|000 B|XXXX|XXXX|
|0x6A|000 B|XXXX|XXXX|
|0x6B|000 B|XXXX|XXXX|
|0x6C|000 B|XXXX|XXXX|
|0x6D|000 B|XXXX|XXXX|
|0x6E|000 B|XXXX|XXXX|
|0x6F|000 B|XXXX|XXXX|
|0x70|000 B|XXXX|XXXX|
|0x71|000 B|XXXX|XXXX|
|0x72|000 B|XXXX|XXXX|
|0x73|000 B|XXXX|XXXX|
|0x74|2 B|JE|Jump if Equal<br>OPCode+offset|
|0x75|2 B|JE|Jump if not Equal<br>OPCode+offset|
|0x76|000 B|XXXX|XXXX|
|0x77|000 B|XXXX|XXXX|
|0x78|000 B|XXXX|XXXX|
|0x79|000 B|XXXX|XXXX|
|0x7A|000 B|XXXX|XXXX|
|0x7B|000 B|XXXX|XXXX|
|0x7C|000 B|XXXX|XXXX|
|0x7D|000 B|XXXX|XXXX|
|0x7E|000 B|XXXX|XXXX|
|0x7F|000 B|XXXX|XXXX|
|0x80|000 B|XXXX|XXXX|
|0x81|000 B|XXXX|XXXX|
|0x82|000 B|XXXX|XXXX|
|0x83|8 B|CMP 8I|Opcode+ModR/M+SIB+Displacment+Immediate|
|0x84|000 B|XXXX|XXXX|
|0x85|2 B|TEST|Logical AND REG vs REG<br>OPCode+ModR/M|
|0x86|000 B|XXXX|XXXX|
|0x87|000 B|XXXX|XXXX|
|0x88|7 B|MOV|MEM->REG<br>OPCode+ModR/M+SIB+Displacement|
|0x89|7 B|MOV|REG->MEM<br>OPCode+ModR/M+SIB+Displacement|
|0x8A|7 B|MOV|MEM->REG<br>OPCode+ModR/M+SIB+Displacement|
|0x8B|3 B|MOV|OPCode+ModR/M+Displacement|
|0x8C|000 B|XXXX|XXXX|
|0x8D|000 B|XXXX|XXXX|
|0x8E|000 B|XXXX|XXXX|
|0x8F|000 B|XXXX|XXXX|
|0x90|000 B|XXXX|XXXX|
|0x91|000 B|XXXX|XXXX|
|0x92|000 B|XXXX|XXXX|
|0x93|000 B|XXXX|XXXX|
|0x94|000 B|XXXX|XXXX|
|0x95|000 B|XXXX|XXXX|
|0x96|000 B|XXXX|XXXX|
|0x97|000 B|XXXX|XXXX|
|0x98|000 B|XXXX|XXXX|
|0x99|000 B|XXXX|XXXX|
|0x9A|000 B|XXXX|XXXX|
|0x9B|000 B|XXXX|XXXX|
|0x9C|000 B|XXXX|XXXX|
|0x9D|000 B|XXXX|XXXX|
|0x9E|000 B|XXXX|XXXX|
|0x9F|000 B|XXXX|XXXX|
|0xA0|000 B|XXXX|XXXX|
|0xA1|000 B|XXXX|XXXX|
|0xA2|000 B|XXXX|XXXX|
|0xA3|000 B|XXXX|XXXX|
|0xA4|000 B|XXXX|XXXX|
|0xA5|000 B|XXXX|XXXX|
|0xA6|000 B|XXXX|XXXX|
|0xA7|000 B|XXXX|XXXX|
|0xA8|000 B|XXXX|XXXX|
|0xA9|000 B|XXXX|XXXX|
|0xAA|000 B|XXXX|XXXX|
|0xAB|000 B|XXXX|XXXX|
|0xAC|000 B|XXXX|XXXX|
|0xAD|000 B|XXXX|XXXX|
|0xAE|000 B|XXXX|XXXX|
|0xAF|000 B|XXXX|XXXX|
|0xB0|000 B|XXXX|XXXX|
|0xB1|000 B|XXXX|XXXX|
|0xB2|000 B|XXXX|XXXX|
|0xB3|000 B|XXXX|XXXX|
|0xB4|000 B|XXXX|XXXX|
|0xB5|000 B|XXXX|XXXX|
|0xB6|000 B|XXXX|XXXX|
|0xB7|000 B|XXXX|XXXX|
|0xB8|000 B|XXXX|XXXX|
|0xB9|000 B|XXXX|XXXX|
|0xBA|000 B|XXXX|XXXX|
|0xBB|000 B|XXXX|XXXX|
|0xBC|000 B|XXXX|XXXX|
|0xBD|000 B|XXXX|XXXX|
|0xBE|000 B|XXXX|XXXX|
|0xBF|000 B|XXXX|XXXX|
|0xC0|000 B|XXXX|XXXX|
|0xC1|000 B|XXXX|XXXX|
|0xC2|000 B|XXXX|XXXX|
|0xC3|000 B|XXXX|XXXX|
|0xC4|000 B|XXXX|XXXX|
|0xC5|000 B|XXXX|XXXX|
|0xC6|000 B|XXXX|XXXX|
|0xC7|11 B|MOV|Move 32-Bit Immediate<br>OPCode+ModM/R+SIB+Displacement+Immediate|
|0xC8|000 B|XXXX|XXXX|
|0xC9|000 B|XXXX|XXXX|
|0xCA|000 B|XXXX|XXXX|
|0xCB|000 B|XXXX|XXXX|
|0xCC|000 B|XXXX|XXXX|
|0xCD|000 B|XXXX|XXXX|
|0xCE|000 B|XXXX|XXXX|
|0xCF|000 B|XXXX|XXXX|
|0xD0|000 B|XXXX|XXXX|
|0xD1|000 B|XXXX|XXXX|
|0xD2|000 B|XXXX|XXXX|
|0xD3|000 B|XXXX|XXXX|
|0xD4|000 B|XXXX|XXXX|
|0xD5|000 B|XXXX|XXXX|
|0xD6|000 B|XXXX|XXXX|
|0xD7|000 B|XXXX|XXXX|
|0xD8|000 B|XXXX|XXXX|
|0xD9|000 B|XXXX|XXXX|
|0xDA|000 B|XXXX|XXXX|
|0xDB|000 B|XXXX|XXXX|
|0xDC|000 B|XXXX|XXXX|
|0xDD|000 B|XXXX|XXXX|
|0xDE|000 B|XXXX|XXXX|
|0xDF|000 B|XXXX|XXXX|
|0xE0|000 B|XXXX|XXXX|
|0xE1|000 B|XXXX|XXXX|
|0xE2|000 B|XXXX|XXXX|
|0xE3|000 B|XXXX|XXXX|
|0xE4|000 B|XXXX|XXXX|
|0xE5|000 B|XXXX|XXXX|
|0xE6|000 B|XXXX|XXXX|
|0xE7|000 B|XXXX|XXXX|
|0xE8|4 B|CALL|Calls an adress with the offset specified |
|0xE9|000 B|XXXX|XXXX|
|0xEA|000 B|XXXX|XXXX|
|0xEB|2 B|JMP|Short Jump to offset<br>OPCode+offset |
|0xEC|000 B|XXXX|XXXX|
|0xED|000 B|XXXX|XXXX|
|0xEE|000 B|XXXX|XXXX|
|0xEF|000 B|XXXX|XXXX|
|0xF0|000 B|XXXX|XXXX|
|0xF1|000 B|XXXX|XXXX|
|0xF2|000 B|XXXX|XXXX|
|0xF3|000 B|XXXX|XXXX|
|0xF4|000 B|XXXX|XXXX|
|0xF5|000 B|XXXX|XXXX|
|0xF6|000 B|XXXX|XXXX|
|0xF7|000 B|XXXX|XXXX|
|0xF8|000 B|XXXX|XXXX|
|0xF9|000 B|XXXX|XXXX|
|0xFA|000 B|XXXX|XXXX|
|0xFB|000 B|XXXX|XXXX|
|0xFC|000 B|XXXX|XXXX|
|0xFD|000 B|XXXX|XXXX|
|0xFE|000 B|XXXX|XXXX|
|0xFF|000 B|XXXX|XXXX|

### Extended table if first byte is 0x0F.
|OPCode|Length|Short Name| Long Name |
|:-:|-:|:-:|:-|
|0x00|000 B|XXXX|XXXX|
|0x01|000 B|XXXX|XXXX|
|0x02|000 B|LAR|Load access rights byte from the specified segment descriptor|
|0x03|000 B|LSL|Load segment limit from the specified segment descriptor|
|0x04|- B|-|* undefined *|
|0x05|0 B|SYSCALL|Fast System call|
|0x06|0 B|CLTS|Clear task-switched flag in the MSW|
|0x07|0 B|SYSRET|Fast Return from System Call|
|0x08|1 B|INVD|Invalidate Internal Caches|
|0x09|1 B|WBINVD|Write Back and Invalidate Cache|
|0x0A|- B|-|XXXX|
|0x0B|000 B|UD2|2-Byte illegal opcodes|
|0x0C|- B|-|XXXX|
|0x0D|3 B|prefetchw|Prefetch cache line<br>0F 0D 00 - Dont Intent write<br>0F 0D 01 - Intent write|
|0x0E|- B|-|XXXX|
|0x0F|- B|-|XXXX|
|0x10|000 B|vmovups|Vector Move Unaligned Packed Single-Precision Floating-Point Values|
|0x11|000 B|vmovups|XXXX|
|0x12|000 B|vmovlps|Vector Move Low Packed Single-Precision Floating-Point Values|
|0x13|000 B|vunpcklps|XXXX|
|0x14|000 B|vunpckhps|Vector Unpack Low Packed Single-Precision Floating-Point Values|
|0x15|000 B|vmovhps|XXXX|
|0x16|000 B|vmovhps|Vector Move High Packed Single-Precision Floating-Point Values|
|0x17|000 B|XXXX|XXXX|
|0x18|000 B|XXXX|XXXX|
|0x19|000 B|NOP|No Operation|
|0x1A|000 B|bndldx| Load Extended Bounds Using Address Translation|
|0x1B|000 B|bndstx| Store Extended Bounds Using Address Translation|
|0x1C|000 B|NOP|No Operation|
|0x1D|000 B|NOP|No Operation|
|0x1E|000 B|NOP|No Operation|
|0x1F|000 B|NOP|No Operation|
|0x20|000 B|MOV|XXXX|
|0x21|000 B|MOV|XXXX|
|0x22|000 B|MOV|XXXX|
|0x23|000 B|MOV|XXXX|
|0x24|- B|-|-|
|0x25|- B|-|-|
|0x26|- B|-|-|
|0x27|- B|-|-|
|0x28|000 B|vmovaps|Vector Move Aligned Packed Single-Precision Floating-Point Values|
|0x29|000 B|vmovaps|XXXX|
|0x2A|000 B|cvtpi2ps|Convert Packed Dword Integers to Packed Single-Precision Floating-Point Values|
|0x2B|000 B|vmovntps|XXXX|
|0x2C|000 B|cvttps2pi|XXXX|
|0x2D|000 B|cvtps2pi|XXXX|
|0x2E|000 B|vucomiss|XXXX|
|0x2F|000 B|vcomiss|XXXX|
|0x30|000 B|WRMSR|XXXX|
|0x31|000 B|RDTSC|XXXX|
|0x32|000 B|RDMSR|XXXX|
|0x33|000 B|RDPMC|XXXX|
|0x34|000 B|SYSENTER|XXXX|
|0x35|000 B|SYSEXIT|XXXX|
|0x36|000 B|-|XXXX|
|0x37|000 B|GETSEC|XXXX|
|0x38|000 B|***|Prefix|
|0x39|000 B|-|-|
|0x3A|000 B|***|Prefix|
|0x3B|000 B|-|-|
|0x3C|000 B|-|-|
|0x3D|000 B|-|-|
|0x3E|000 B|-|-|
|0x3F|000 B|-|-|
|0x40|000 B|CMOV|XXXX|
|0x41|000 B|CMOV|XXXX|
|0x42|000 B|CMOV|XXXX|
|0x43|000 B|CMOV|XXXX|
|0x44|000 B|CMOV|XXXX|
|0x45|000 B|CMOV|XXXX|
|0x46|000 B|CMOV|XXXX|
|0x47|000 B|CMOV|XXXX|
|0x48|000 B|CMOV|XXXX|
|0x49|000 B|CMOV|XXXX|
|0x4A|000 B|CMOV|XXXX|
|0x4B|000 B|CMOV|XXXX|
|0x4C|000 B|CMOV|XXXX|
|0x4D|000 B|CMOV|XXXX|
|0x4E|000 B|CMOV|XXXX|
|0x4F|000 B|CMOV|XXXX|
|0x50|000 B|vmovmskps|XXXX|
|0x51|000 B|vsqrtps|XXXX|
|0x52|000 B|vrsqrtps|XXXX|
|0x53|000 B|vrcpps|XXXX|
|0x54|000 B|vandps|XXXX|
|0x55|000 B|vandnps|XXXX|
|0x56|000 B|vorps|XXXX|
|0x57|000 B|vxorps|XXXX|
|0x58|000 B|vaddps|XXXX|
|0x59|000 B|vmulps|XXXX|
|0x5A|000 B|vcvtps2pd|XXXX|
|0x5B|000 B|vcvtdq2ps|XXXX|
|0x5C|000 B|vsubps|XXXX|
|0x5D|000 B|vminps|XXXX|
|0x5E|000 B|vdivps|XXXX|
|0x5F|000 B|vmaxps|XXXX|
|0x60|000 B|punpcklbw|XXXX|
|0x61|000 B|punpcklwd|XXXX|
|0x62|000 B|punpckldq|XXXX|
|0x63|000 B|packsswb|XXXX|
|0x64|000 B|pcmpgtb|XXXX|
|0x65|000 B|pcmpgtw|XXXX|
|0x66|000 B|pcmpgtd|XXXX|
|0x67|000 B|packuswb|XXXX|
|0x68|000 B|punpckhbw|XXXX|
|0x69|000 B|punpckhwd|XXXX|
|0x6A|000 B|punpckhdq|XXXX|
|0x6B|000 B|packssdw|XXXX|
|0x6C|000 B|-|XXXX|
|0x6D|000 B|-|XXXX|
|0x6E|000 B|movdq|XXXX|
|0x6F|000 B|movdq|XXXX|
|0x70|000 B|pshufw|XXXX|
|0x71|000 B|XXXX|XXXX|
|0x72|000 B|XXXX|XXXX|
|0x73|000 B|XXXX|XXXX|
|0x74|000 B|pcmpeqb|XXXX|
|0x75|000 B|pcmpeqw|XXXX|
|0x76|000 B|pcmpeqd|XXXX|
|0x77|000 B|emms|XXXX|
|0x78|000 B|VMREAD|XXXX|
|0x79|000 B|VMWRITE|XXXX|
|0x7A|000 B|-|-|
|0x7B|000 B|-|-|
|0x7C|000 B|vhaddpd|-|
|0x7D|000 B|vhsubpd|-|
|0x7E|000 B|mov/q|XXXX|
|0x7F|000 B|movq|XXXX|
|0x80|000 B|JMP|Jump long|
|0x81|000 B|JMP|Jump long|
|0x82|000 B|JMP|Jump long|
|0x83|000 B|JMP|Jump long|
|0x84|000 B|JMP|Jump long|
|0x85|000 B|JMP|Jump long|
|0x86|000 B|JMP|Jump long|
|0x87|000 B|JMP|Jump long|
|0x88|000 B|JMP|XXXX|
|0x89|000 B|JMP|XXXX|
|0x8A|000 B|JMP|XXXX|
|0x8B|000 B|JMP|XXXX|
|0x8C|000 B|JMP|XXXX|
|0x8D|000 B|JMP|XXXX|
|0x8E|000 B|JMP|XXXX|
|0x8F|000 B|JMP|XXXX|
|0x90|000 B|SET|Set on condition|
|0x91|000 B|SET|Set on condition|
|0x92|000 B|SET|Set on condition|
|0x93|000 B|SET|Set on condition|
|0x94|000 B|SET|Set on condition|
|0x95|000 B|SET|Set on condition|
|0x96|000 B|SET|Set on condition|
|0x97|000 B|SET|Set on condition|
|0x98|000 B|SET|XXXX|
|0x99|000 B|SET|XXXX|
|0x9A|000 B|SET|XXXX|
|0x9B|000 B|SET|XXXX|
|0x9C|000 B|SET|XXXX|
|0x9D|000 B|SET|XXXX|
|0x9E|000 B|SET|XXXX|
|0x9F|000 B|SET|XXXX|
|0xA0|000 B|PUSH|XXXX|
|0xA1|000 B|POP|XXXX|
|0xA2|000 B|CPUID|CPU information|
|0xA3|000 B|BT|XXXX|
|0xA4|000 B|SHLD|XXXX|
|0xA5|000 B|SHLD|XXXX|
|0xA6|000 B|-|-|
|0xA7|000 B|-|-|
|0xA8|000 B|PUSH|XXXX|
|0xA9|000 B|POP|XXXX|
|0xAA|000 B|RSM|XXXX|
|0xAB|000 B|BTS|XXXX|
|0xAC|000 B|SHRD|XXXX|
|0xAD|000 B|SHRD|XXXX|
|0xAE|000 B|XXXX|XXXX|
|0xAF|000 B|IMUL|XXXX|
|0xB0|000 B|CMPXCHG|XXXX|
|0xB1|000 B|CMPXCHG|XXXX|
|0xB2|000 B|LSS|XXXX|
|0xB3|000 B|BTR|XXXX|
|0xB4|000 B|LFS|XXXX|
|0xB5|000 B|LGS|XXXX|
|0xB6|000 B|MOVZX|XXXX|
|0xB7|000 B|MOVZX|XXXX|
|0xB8|000 B|POPCNT|XXXX|
|0xB9|000 B|xxxx|XXXX|
|0xBA|000 B|xxxx|XXXX|
|0xBB|000 B|BTC|XXXX|
|0xBC|000 B|BSF|XXXX|
|0xBD|000 B|BSR|XXXX|
|0xBE|000 B|MOVSX|XXXX|
|0xBF|000 B|MOVSX|XXXX|
|0xC0|000 B|XADD|XXXX|
|0xC1|000 B|XADD|XXXX|
|0xC2|000 B|vcmpps|XXXX|
|0xC3|000 B|movnti|XXXX|
|0xC4|000 B|pinsrw|XXXX|
|0xC5|000 B|pextrw|XXXX|
|0xC6|000 B|vshufps|XXXX|
|0xC7|000 B|XXXX|XXXX|
|0xC8|000 B|BSWAP|XXXX|
|0xC9|000 B|BSWAP|XXXX|
|0xCA|000 B|BSWAP|XXXX|
|0xCB|000 B|BSWAP|XXXX|
|0xCC|000 B|BSWAP|XXXX|
|0xCD|000 B|BSWAP|XXXX|
|0xCE|000 B|BSWAP|XXXX|
|0xCF|000 B|BSWAP|XXXX|
|0xD0|000 B|vaddsubpd|XXXX|
|0xD1|000 B|psrlw|XXXX|
|0xD2|000 B|psrld|XXXX|
|0xD3|000 B|psrlq|XXXX|
|0xD4|000 B|paddq|XXXX|
|0xD5|000 B|pmullw|XXXX|
|0xD6|000 B|vmovq|XXXX|
|0xD7|000 B|pmovmskb|XXXX|
|0xD8|000 B|psubusb|XXXX|
|0xD9|000 B|psubusw|XXXX|
|0xDA|000 B|pminub|XXXX|
|0xDB|000 B|pand|XXXX|
|0xDC|000 B|paddusb|XXXX|
|0xDD|000 B|paddusw|XXXX|
|0xDE|000 B|pmaxub|XXXX|
|0xDF|000 B|pandn|XXXX|
|0xE0|000 B|pavgb|XXXX|
|0xE1|000 B|psraw|XXXX|
|0xE2|000 B|psrad|XXXX|
|0xE3|000 B|pavgw|XXXX|
|0xE4|000 B|pmulhuw|XXXX|
|0xE5|000 B|pmulhw|XXXX|
|0xE6|000 B|vcvttpd2dq|XXXX|
|0xE7|000 B|movntq|XXXX|
|0xE8|000 B|psubsb|XXXX|
|0xE9|000 B|psubsw|XXXX|
|0xEA|000 B|pminsw|XXXX|
|0xEB|000 B|por|XXXX|
|0xEC|000 B|paddsb|XXXX|
|0xED|000 B|paddsw|XXXX|
|0xEE|000 B|pmaxsw|XXXX|
|0xEF|000 B|pxor|XXXX|
|0xF0|000 B|vlddqu|XXXX|
|0xF1|000 B|psllw|XXXX|
|0xF2|000 B|pslld|XXXX|
|0xF3|000 B|psllq|XXXX|
|0xF4|000 B|pmuludq|XXXX|
|0xF5|000 B|pmaddwd|XXXX|
|0xF6|000 B|psadbw|XXXX|
|0xF7|000 B|maskmovq|XXXX|
|0xF8|000 B|psubb|XXXX|
|0xF9|000 B|psubw|XXXX|
|0xFA|000 B|psubd|XXXX|
|0xFB|000 B|psubq|XXXX|
|0xFC|000 B|paddb|XXXX|
|0xFD|000 B|paddw|XXXX|
|0xFE|000 B|paddd|XXXX|
|0xFF|000 B|UD0|Illegal instuction|

### Extended table if first two byte are 0x0F38.
|OPCode|Length|Short Name| Long Name |
|:-:|-:|:-:|:-|

### Extended table if first two byte are 0x0F3A.
|OPCode|Length|Short Name| Long Name |
|:-:|-:|:-:|:-|

## Links
- [Intel® 64 and IA-32 Architectures Software Developer’s Manual](https://cdrdv2.intel.com/v1/dl/getContent/671200)
- [AMD64 Architecture Programmer’s Manual](https://www.amd.com/content/dam/amd/en/documents/processor-tech-docs/programmer-references/24592.pdf)

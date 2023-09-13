# Common Object File Format (COFF)

## Info
Storage for binary data for execution or shared objects.<br>
Commonly used by Windows binarys.<br>
Format is generally regarded as 32-Bit Little Endian but has limted 64-Bit capability.

## Structure
- [Header](###File-Header)
- [Optional Header](###File-Header)
- [Section Header](###Section-Header)
- [Section Raw Data](###Section-Raw-Data)
- [Symbol Table](###Symbol-Table)
- [String Table](###String-Table)

### File Header
|Type| Name | Expect |
|:-:|:-|:-:|
| Int16LE | Machine | |
| Int16LE | NumberOfSections | |
| Int32LE | TimeDateStamp | |
| Int32LE | PointerToSymbolTable | |
| Int32LE | NumberOfSymbols | |
| Int16LE | SizeOfOptionalHeader | |
| Int16LE | Characteristics | |

### Optional Header
#### Format (2 Byte)

|Type| Name | Expect |
|:-:|:-|:-|
|Int16LE<br>(enum)|FormatID|0x10b = PE32<br>0x20b = PE32+|

#### Standard Fields (24/28 Byte)

|Type| Name | Expect |
|:-:|:-|:-|
|Int16LE<br>(enum)|StateID|0x10B = Executable<br>0x107 = ROM|
|Int8LE|MajorLinkerVersion||
|Int8LE|MinorLinkerVersion||
|Int32LE|SizeOfCode||
|Int32LE|SizeOfInitializedData||
|Int32LE|SizeOfUninitializedData||
|Int32LE|AddressOfEntryPoint||
|Int32LE|BaseOfCode||
|Int32LE<br>(Only 32Bit)|BaseOfData||

#### Windows NT-Specific Fields (68/88 Byte)
|Type| Name | Expect |
|:-:|:-|:-|
| PE32: Int32LE<br>PE32+: Int64LE | ImageBase | |
| Int32LE | SectionAlignment | |
| Int32LE | FileAlignment | |
| Int16LE | MajorOperatingSystemVersion | |
| Int16LE | MinorOperatingSystemVersion | |
| Int16LE | MajorImageVersion | |
| Int16LE | MinorImageVersion | |
| Int16LE | MajorSubsystemVersion | |
| Int16LE | MinorSubsystemVersion | |
| Byte[4] | Reserved | |
| Int32LE | SizeOfImage | |
| Int32LE | SizeOfHeaders | |
| Int32LE | CheckSum | |
| Int16LE | Subsystem | |
| Int16LE | DLL Characteristics | |
| PE32: Int32LE<br>PE32+: Int64LE | SizeOfStackReserve | |
| PE32: Int32LE<br>PE32+: Int64LE | SizeOfStackCommit | |
| PE32: Int32LE<br>PE32+: Int64LE | SizeOfHeapReserve | |
| PE32: Int32LE<br>PE32+: Int64LE | SizeOfHeapCommit | |
| Int32LE | LoaderFlags | |
| Int32LE | NumberOfRvaAndSizes | |

#### Data Directories (128 Byte)
|Type| Name | Expect |
|:-:|:-|:-|
...

### Section Header (40 Bytes)
|Type| Name | Expect |
|:-:|:-|:-|
| char[8] | Name | |
| Int32LE | VirtualSize | |
| Int32LE | VirtualAddress | |
| Int32LE | SizeOfRawData | |
| Int32LE | PointerToRawData | |
| Int32LE | PointerToRelocations | |
| Int32LE | PointerToLinenumbers | |
| Int16LE | NumberOfRelocations | |
| Int16LE | NumberOfLinenumbers | |
| Int32LE | Characteristics | |

### Section Raw Data
...

### Symbol Table (18 Byte)
|Type| Name | Expect |
|:-:|:-|:-|
| char[8] | Name  | |
| Int32LE | Value | |
| Int16LE | SectionNumber | |
| Int16LE | Type | |
| Int8LE | StorageClass | |
| Int8LE | NumberOfAuxSymbols | |

### String Table
...

## Links
- [Wikipedia](https://en.wikipedia.org/wiki/COFF) 

> Additional help can be found over the toppic "Portable Execution Format" from Windows.
# JFS
## Info

|||
|:-|:-|
|Type| Model, Mesh and Texture|
|Format| Binary - 32-Bit|
|Endianness| Little Endian|
|Magic/Signature| - |
|Legal|proprietary, closed source; abandonware|
|Popularity|Custom; only in few programs |



## Structure
### Header
|Type|Name|
|:-|:-|
|I32U|FileEntryAmount|
|FileEntry[]|FileEntryList|

### FileEntry
|Type|Name|Values|
|:-|:-|:-|
|I32U| NameHash| -  |
|I32U| Offset| -  |
|I32U| CompressedSize| -  |
|I32U| UncompressedSize| -  |
|I32U| CompressionLevel| -  |
|I32U| CompressionFlag| 0 - not compressed<br>1 - compressed (ZLIB)>  |

## Links
- [wiki.xentax - JFS](https://wiki.xentax.spektr.name/index.php/Juiced_2:_Hot_Import_Nights_JFS)

# Universal Scene Description (USD)

## Info
Text based list of resources sorted in a hirachy, created by Pixar.<br>
Additionally provides a binary format to store data more efficently.

|Name|Info|
|-:|:-|
|Type|Text & Binary|
|Use|Container for scenes. Models, Materials, Lighting|
|Difficulty|Medium|
|Documentation|Text: insufficient<br>Binary: Specification missing|
|Dependecy|In binary: [LZ4](https://github.com/lz4/lz4)|

## Structure/Layout
### usda - (Text)
|Name|Expect|Size|
|:-:|:-:|:-:|
|Signature|#usda 1.0|9 Bytes|

### usdc - (Binary)
> Header

|Name|Expect|Size|
|:-|:-:|:-:|
|Signature|PXR-USDC|8 Bytes|
|Version|0.1.2<br>Does not contain dots<br>Only 3 bytes used?|8 Bytes|
|TOC Offset|Int 64U|8 Bytes|

> TOC-Chunk

|Name|Expect|Size|
|:-|:-:|:-:|
|TOCSectionsAmount|Int 64U|8 Bytes|
|TOCSectionList|List\<TOCSection\>|n Bytes|


> TOC-Section

|Name|Expect|Size|
|:-|:-|:-:|
|Name|char[]|16 Bytes|
|Start|Int 64U|8 Bytes|
|Size|Int 64U|8 Bytes|

> Section: ("TOKENS" == Name )

|Name|Expect|Size|Hint|
|:-|:-|:-:|:-:|
|NumberOfTokens|Int64U|8 Bytes|-|
|SizeUncompressed|Int 64U|8 Bytes|Must be bigger than 3 and (3+NumberOfTokens)|
|SizeCompressed|Int 64U|8 Bytes|Compressed with [LZ4](https://github.com/lz4/lz4)|
|TokenListCompressed|char[]|SizeCompressed|List of NULL terminated strings|

> Section: ("STRINGS" == Name )

|Name|Expect|Size|Hint|
|:-|:-|:-:|:-:|
|AmountOFElemets|Int 64U|8 Bytes|-|
|Data|List\<Int32U\> ?|8 Bytes|actual size: AmountOFElemets * 4, why 4?<br> Data is uncompressed|

> Section: ("FIELDS" == Name )

|Name|Expect|Size|Hint|
|:-|:-|:-:|:-:|
|AmountOFFields|Int 64U|8 Bytes|Can be zero|
|IndexList|List\<???\>|n Bytes|List of compressed intergers|
|CompressedSize|Int64U|8 Bytes|-|
|CompressedData|???| CompressedSize |LZ4 compressed|

> Section: ("FIELDSETS" == Name )

|Name|Expect|Size|Hint|
|:-|:-|:-:|:-:|
|AmountOFFieldsSets|Int 64U|8 Bytes|Can be zero|
|fsets_Size|???|? Bytes|???|
|Compressed ints?|???|? Bytes|USD compressed ints|

> Section: ("SPECS" == Name )

|Name|Expect|Size|Hint|
|:-|:-|:-:|:-:|
|AmountOFFieldsSpecs|Int 64U|8 Bytes|Can be zero|
|Data|-|-|Compressed|
|path_indexes_size|Int 64U|8 Bytes|-|
|path_indexes_Data|-|-|USD Compressed data|
|fset_indexes_size|Int 64U|8 Bytes|-|
|fset_indexes_Data|-|-|USD Compressed data|
|spectype_size|Int 64U|8 Bytes|-|
|spectype_Data|-|-|USD Compressed data|

> Section: ("PATHS" == Name )

|Name|Expect|Size|Hint|
|:-|:-|:-:|:-:|
|AmountOfPaths|Int 64U|8 Bytes|Can be zero|
|AmountOfEncodedPaths|Int 64U|8 Bytes|-|
|compPathIndexesSize|Int 64U|8 Bytes|-|
|compPathIndexesData|-|-|USD Compressed data|
|compElementTokenIndexesSize|Int 64U|8 Bytes|-|
|compElementTokenIndexesData|-|-|USD Compressed data|
|compJumpsSize|Int 64U|8 Bytes|-|
|compJumpsData|-|-|USD Compressed data|

### usdz - (ZIP)

## Link
- [Wikipedia](https://en.wikipedia.org/wiki/Universal_Scene_Description)
- [loc.gov](https://loc.gov/preservation/digital/formats/fdd/fdd000561.shtml)
- [OpenUSD](https://openusd.org/dev/spec_usdz.html)
- [GitHub - OpenUSD](https://github.com/PixarAnimationStudios/OpenUSD)
- [GitHub - assimp](https://github.com/assimp/assimp)
- [GitHub - tinyusdz](https://github.com/lighttransport/tinyusdz)

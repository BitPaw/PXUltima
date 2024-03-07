# Direct Draw Surface Texture (DDS)

## Info
This file format is used by DirectX.<br>
It's use is to load an image with embeeded texture info so that we dont have to store them seperatly.

## Structure
|Name| Size
|:-|-:|
| Signature | 4 Bytes |
| Header          | 100 Bytes|
| Header Extended<br>(optional)  | 20 Bytes |
| Raw Data 1| ? |
| Raw Data 2<br>(optional)| ? |
|...<br>(optional)| ? |

### Signature (4 Bytes)
|Name|Length|Type| Description | Expect
|:-:|:-:|:-:|:-:|:-:|
|Signature|4 Bytes| char[4] | Magic number to detect filetype| "DDS "|
> Note the empty space in the string

### Header (100 Bytes)
|Name|Length|Type|
|-:|:-:|:-:|
|Size|4 Bytes| Int32U LE |
|Flags|4 Bytes| Int32U LE |
|Height|4 Bytes| Int32U LE |
|Width|4 Bytes| Int32U LE |
|pitchOrLinearSize|4 Bytes| Int32U LE |
|Depth|4 Bytes| Int32U LE |
|MipMapCount|4 Bytes| Int32U LE |
|Reserved|44 Bytes| --- |
|ddspf|4 Bytes| Int32U LE |
|caps|4 Bytes| Int32U LE |
|caps2|4 Bytes| Int32U LE |
|caps3|4 Bytes| Int32U LE |
|caps4|4 Bytes| Int32U LE |
|Reserved|8 Bytes| --- |

### Header Extended (20 Bytes)
|Name|Length|Type|
|-:|:-:|:-:|
|dxgiFormat|4 Bytes| enum<br>(Int32U LE) |
|resourceDimension|4 Bytes| enum<br>(Int32U LE) |
|miscFlag|4 Bytes| Int32U LE |
|arraySize|4 Bytes| Int32U LE |
|miscFlags2|4 Bytes| Int32U LE |

## Links
- [Microsoft Guide](https://learn.microsoft.com/en-us/windows/win32/direct3ddds/dx-graphics-dds-pguide#dds-file-layout)
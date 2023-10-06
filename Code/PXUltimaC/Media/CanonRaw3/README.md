# Canon RAW format version 3 (CR3)<br>[ISO/IEC 14496-12]

## Dependency
- TIFF

## Structure
### Signature
> No Signature, file starts directly with chunks.

### Chunk
|Type|Name| Description
|:-:|:-:|:-|
| UInt32 BigEndian | Size | Size of whole chunk inclusing this |
| UInt32 BigEndian | Type | Type of following data |

Depending on the size, spesific conditions can apply.

|Condition| Result|
|:-:|:-|
| Size = 0 | The chunk extends for the remaining file size|
| Size = 1 | Size is extended with a UInt64 BigEndian following directly after the type|
| Any other| Procceed normally |


## Links
- [exiftool.org](https://exiftool.org/TagNames/Canon.html)
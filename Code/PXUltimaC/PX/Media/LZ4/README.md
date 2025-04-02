# LZ4 Compression

## Info
||||||
|:-:|:-:|:-:|:-:|:-:|
Signature|0x04|0x22|0x4D|0x18|

## Structure
### Linear small-integer code (LISC)
We read one byte, if the byte is 255 (0xFF) then we read the next byte and add this to a sum.<br>
When we read something smaler, we are done and continue.

### Block/Segment

|Size|Name| Description|
|:-:|:-:|:-|
|4-Bit|Token|LitteralLength|
|4-Bit|Token|matchLength|
|n Byte|LISC-A|List of Int8U<br>(Only if LitteralLength=15)|
|(LitteralLength + LISC-A) Bytes|Literal|List of Int8U|
|2 Byte|Offset|Int16U Little Endian|
|n Byte|LISC-B|List of Int8U<br>(Only if matchLength=15)|

Then, use the offset value to move the cusor of the current output and move (matchLength + LISC-B + 4) Bytes to itself.<br>
Be aware that we move the data and not copy it! As the data is not inizialized, we copy byte per byte. Otherwise that data could corrupt itself.

## Links
- [GitHub - LZ4](https://github.com/lz4/lz4)
- [Wikipedia](https://en.wikipedia.org/wiki/LZ4_(compression_algorithm))
- [how LZ4 works](https://ticki.github.io/blog/how-lz4-works/)

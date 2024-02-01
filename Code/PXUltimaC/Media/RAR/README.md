# Eugene Roshal Archive (RAR)

## Info
### Legal problems
Due to the patent rules, you may decompress RAR files but are 
not allowed to compress a file without paying a fee. This explains 
why programs like 7Zip can only open RAR files but never creating 
them. The only program alowed to create these files is WinRAR itself.

## Dependency
- CRC

## Structure
### Signature

> for (RAR 1.5 to 4.0)

||
|:-:|:-:|:-:|:-:|:-:|
|R|a|r|!| 0x1A | 0x07 | 0x00 |

> for (RAR 5.0+)

||
|:-:|:-:|:-:|:-:|:-:|
|R|a|r|!| 0x1A | 0x07 | 0x01 | 0x00 |


## Links
- [rarlab](https://www.rarlab.com/technote.htm)
- [Wikipedia](https://en.wikipedia.org/wiki/RAR_(file_format))
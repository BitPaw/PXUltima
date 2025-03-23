# BASE64
## Info
Algorithm to convert a stream of 8-Bit data into a stream of 6-bit data.<br>
Although increases the size by a minimum of 33%, the gain is the ability to have printable 
characters as they now fit in the range of the ASCII characters. 
This allows data to be embeeded into plain text files without them beeing corrupted by characters like spaces or new lines.<be>
For every 3 input Bytes this algorithm yields 4 output Bytes. There is no size limit.  

## Structure
The input size to translate the bytes for Base64, like the name suggests, is 64. We will need an 8-Bit data array of 64 symbols.
RFC 4648 defines a standard translation table but you can also provide one yourself if spesific characters should be avoided while other used.


## Links
- [Wikipedia Base64](https://en.wikipedia.org/wiki/Base64)

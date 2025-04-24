# Where's all data? (WAD)

## Info
|||
|:-:|:-:|
|Format|Binary|

---

## Structure
### Header-Signature
|Type|Size|Description| Expect|
|:-:|:-:|:-:|:-:|
|char[2]|2 Bytes| Signature|RW|
|uInt8|1 Bytes| Major version|1,2,3
|uInt8|1 Bytes| Minor version|-

### Header (Major version = 1)
|Type|Size|Description|
|:-:|:-:|:-|
|u16|2 Bytes| Entry header offset|
|u16|2 Bytes| Entry header size|
|u32|4 Bytes| Entry count|

### Header (Major version = 2)
|Type|Size|Description|
|:-:|:-:|:-|
|u8|1 Byte| ECDSA signature length|
|...|83 Bytes| ECDSA signature of entry headers, padded with 0|
|...|8 Bytes| XXH64 checksum|
|u16|2 Bytes| Entry header offset|
|u16|2 Bytes| Entry header size|
|u32|4 Bytes| Entry count|


### Header (Major version = 3)
|Type|Size|Description|
|:-:|:-:|:-|
|...|256 Byte| ECDSA signature|
|...|8 Byte| XXH64 checksum|
|u32|4 Byte| Entry count|


### Entry header
> WAD version 1 ends at the data type field, followed by 3 padding bytes (for alignement).<br>
WAD versions 2 and 3 include all fields.

|Type|Size|Description|
|:-:|:-:|:-|
|u64|8 Byte| Path hash|
|u32|4 Byte| Data offset in the WAD archive|
|u32|4 Byte| Compressed size|
|u32|4 Byte| Uncompressed size|
|u4<br>(enum)|1 Nibble| Data type<br>0 = uncompressed data<br>1 = gzip<br>2 = file redirection<br>3 = Zstandard<br>4 = Zstandard with subchunks|
|u4|1 Nibble| Count of subbchunks|
|bool|1 Byte| Set for duplicate entries|
|u16|2 Byte| Index of first subbchunk|
|...|8 Byte| Entry checksum|


---

## Links
- [wikipedia-DE](https://de.wikipedia.org/wiki/WAD_(Dateiformat))
- [gamers.org](https://www.gamers.org/dhs/helpdocs/dmsp1666.html)
- [communitydragon](https://www.communitydragon.org/documentation/wad)
- ---
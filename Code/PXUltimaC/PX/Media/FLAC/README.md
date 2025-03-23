# Free Lossless Audio Codec (FLAC)

---

## Info
|||
|:-:|:-:|
|Type| Audio |
|Documentation| :heavy_check_mark: Yes |
|Format|Binary|

### Dependencys
- CRC
### Advantages
- Higher compression as DEFLATE due to improved use of the audio structure

### Drawbacks
- \-

---

## Structure
### Header (4 Bytes)
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-:|
|Signature|4 Byte|char[4]|fLaC|

### SteamInfo (34 Bytes)
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|
|SampleSizeMinimum|2 Bytes| uint16 | Valid: 16 to 65535<br>Invalid: 0-15 Invalid |
|SampleSizeMaximum|2 Bytes| uint16 | Valid: 16 to 65535<br>Invalid: 0-15 Invalid |
|FrameSizeMinimum|3 Bytes| uint24 | May be 0 to imply the value is not known|
|FrameSizeMaximum|3 Bytes| uint24 | May be 0 to imply the value is not known.|
|SampleRate|20 Bits| uint20 | in Hz. Though 20 bits are available, the maximum sample rate is limited by the structure of frame headers to 655350Hz. Also, a value of 0 is invalid.|
|NumberOfChannels|3 Bits | uint3 | (number of channels)-1. FLAC supports from 1 to 8 channels
|BitsPerSample|5 Bits| uint5 | (bits per sample)-1. FLAC supports from 4 to 32 bits per sample.|
|TotalSamples|36 Bits| uint36 | 'Samples' means inter-channel sample, i.e. one second of 44.1Khz audio will have 44100 samples regardless of the number of channels. A value of zero here means the number of total samples is unknown.|
|MD5Signature|16 Bytes/128 Bits| char[16] | MD5 signature of the unencoded audio data. This allows the decoder to determine if an error exists in the audio data even when the error does not result in an invalid bitstream.|	

### Block Header (4 Bytes)
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|
|IsLastMetaDataBlock|1 Bit|bool|Last-metadata-block flag: '1' if this block is the last metadata block before the audio blocks, '0' otherwise. |
|BlockType|7 Bits|enum| 0 : STREAMINFO<br>1 : PADDING<br>    2 : APPLICATION<br>    3 : SEEKTABLE<br>    4 : VORBIS_COMMENT<br>    5 : CUESHEET<br>    6 : PICTURE<br>    7-126 : reserved<br>    127 : invalid, to avoid confusion with a frame sync code
|BlockLength|3 Bytes|uint24|Length (in bytes) of metadata to follow (does not include the size of the METADATA_BLOCK_HEADER) |

### BlockType: PADDING
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|
|Padding| Full chunk size | - | Filler data |

### BlockType: APPLICATION#
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|
|Registered Application ID|4 Bytes | uint32 | - |
|Application data|x|char[x]|String must be a size of a multiple of 8|

### BlockType: SEEKTABLE
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|

### METADATA_BLOCK_VORBIS_COMMENT
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|

### METADATA_BLOCK_CUESHEET
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|

### METADATA_BLOCK_PICTURE
|Name|Size|Type|Expect|
|:-:|:-:|:-:|:-|

---


## Links
- [Wikipedia](https://en.wikipedia.org/wiki/FLAC)

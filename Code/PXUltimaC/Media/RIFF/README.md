# Resource Interchange File Format (RIFF)

# Info
Smal header to define type and length for any format that uses this.<br>
Used in WAVE and AVI files.<br>
The advantage of this format is not quite clear to me.

# Structure (12 Bytes)
|Type|Name|Expected|
|:-:|:-:|:-|
|char[4]|Endian Type ID| "RIFF" : Little Endian<br>"RIFX" : Big Endian|
|uInt32<br>Little Endian|Chunk Size|-|
|char[4]|Format ID|"ANI "<br>"AVI "<br>"AVIX"<br>"RDIB"<br>"PAL "<br>"RMMP"<br>"RMID"<br> "WAVE"<br> "WEBP"<br>|
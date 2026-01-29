# D8 format family (`.d8g`/`.d8w`/`.d8t`/`.d8m`)

## Info

|||
|:-|:-|
|Type| Model, Mesh and Texture|
|Format| Binary|
|Endianness| Little Endian|
|Legal|proprietary, closed source; abandonware|
|Popularity|Custom; only in few programs|

## .d8t - Texture
Headerless raw data.<br>
Format is defined in a `.d8w`.<br>
Expected to be a `.DDS` without the header.

## .d8w - Texture Table
### .d8w - Texture Table Header
|Type|Name|Values|
|:-|:-|:-|
|I32U|AmountOfTextures| -  |
|char[20]|Unknown| -   |
|.d8w - Entry|TextureEntryList| Length=AmountOfTextures |

### .d8w - Texture Table Entry
|Type|Name|Values|
|:-|:-|:-|
|I32U|RawDataSize| size of texture data in .d8t for this entry  |
|char[4]|PixelFormat|e.g. `DXT1`, `DXT5`, etc  |
|I32U|TextureWidth| - |
|I32U|TextureHeight| - |
|I32U|Unknown_A| - |
|I32U|Unknown_B| - |
|I32U|Unknown_C| - |
|I32U|Unknown_D| - |
|I32U|Unknown_E| - |
|I32U|Unknown_F| - |
|I32U|Unknown_G| - |
|I32U|Unknown_H| - |

## .d8g - Geometry

### Header (12 Bytes)
|Type|Name|Values|
|:-|:-|:-|
|I32U|VertexType| 1 = vertices sprites<br>2 = normal  |
|I32U|FaceType|  1 = triangle list<br>2 = triangle strips    |
|I32U|MeshAmount| - |
|.d8g Mesh|MeshList| Amount is from MeshAmount |

### .d8g - Geometry Mesh

|Type|Name|Values|
|:-|:-|:-|
|I32U|TextureID| -  |
|F32[4]|Position| X,Y,Z,W  |
|F32|RadiusBoundingSphere| - |
|F32|DistanceLODMin| - |
|F32|DistanceLODMax| - |
|I32U|UnknownA| Only counts if this is 0 |
|I32U|UnknownB| only exists if UnknownA == 0 |
|I32U|UnknownC| only exists if UnknownA == 0 |
|I32U|UnknownD| only exists if UnknownA == 0 |
|.d8g - Geometry Vertex Block|vertices| -  |
|I32U|face_count| - |
|I16U[face_count]|faces| - |
|I32U|node_count| - |
|Matrix4x4[node_count]|nodes| - |


#### .d8g - Geometry Vertex Block
|Type|Name|Values|
|:-|:-|:-|
| .d8g - Geometry Vertex FVF|vertices| If > 0, read exactly 'definition_count' FVF items<br>If == 0, read FVF items until flag==0xFF  |
|I32U|definition_count|-  |
|.d8g - Geometry Vertex FVF[definition_count]|definitions| - |
|I32U|BytesBetweenVertices (stride)| - |
|I32U|NumberOfVertices| - |

#### .d8g - Geometry Vertex FVF (8 Byte)

|Type|Name|Values|
|:-|:-|:-|
|I16U|Flag| Often 0<br>0xFF signals end of definitions if found  |
|I16U|Offset| Offset within the vertex stride block       |
|I16U|Format| 1=Float2<br>2=Float3<br>4=byte[3..4]? (color?)<br>etc  |
|I8U|VertexType|  0x00=Pos<br>0x03=Normal<br>0x05=TexCoord<br>0x0A=Color<br>...   |
|I8U|UVChannelIndex| 0 or 1, etc. |


## Links
- [reshax.com - d8g-d8m-boh](https://reshax.com/topic/764-juiced-d8g-d8m-boh/)
- [reshax.com - d8t-d8w](https://reshax.com/topic/1631-juiced-pc-d8t-d8w/)

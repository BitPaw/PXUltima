# D8 format family (.d8g/.d8w/.d8t/.d8m)

## .d8t - Texture
No header, only raw pixel data.<br>
Format is defined in a ".d8w".

## .d8w - ???
### .d8w - Header
|Type|Name|Values|
|:-|:-|:-|
|I32U|AmountOfTextures| -  |
|char[20]|Unknown| -   |
|.d8w - Entry|TexturEntryList| Length=AmountOfTextures |

### .d8w - Entry
|Type|Name|Values|
|:-|:-|:-|
|I32U|RawDataSize| size of texture data in .d8t for this entry  |
|char[4]|PixelFormat|e.g. "DXT1", "DXT5", etc  |
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

## .d8g - Geometric

### Header (12 Bytes)
|Type|Name|Values|
|:-|:-|:-|
|I32U|VertexType| 1 = vertices sprites<br>2 = normal  |
|I32U|FaceType|  1 = triangle list<br>2 = triangle strips    |
|I32U|MeshAmount| - |
|.d8g Mesh|MeshList| Amount is from MeshAmount |

### .d8g - Geometric Mesh (32 Bytes)

|Type|Name|Values|
|:-|:-|:-|
|I32U|TextureID| -  |
|F32[4]|Position| X,Y,Z,W  |
|F32|RadiusBoundingSphere| - |
|F32|DistanceLODMin| - |
|F32|DistanceLODMax| - |

Conditional block:
- Read 4 Bytes
- If > 0, seek backward 4 bytes and do not read the rest of the 12 bytes.
- Otherwise, reads the next three uint32_t (total of 16 bytes).

|Type|Name|Values|
|:-|:-|:-|
|.d8g - Geometric Vertex Block|vertices| -  |
|PXI32U|face_count| X,Y,Z,W  |
|PXI16U[face_count]|faces| - |
|PXI32U|node_count| - |
|Matrix4x4[node_count]|nodes| - |


#### .d8g - Geometric Vertex Block
|Type|Name|Values|
|:-|:-|:-|
| .d8g - Geometric Vertex FVF|vertices| If > 0, read exactly 'definition_count' FVF items<br>If == 0, read FVF items until flag==0xFF  |
|I32U|definition_count| X,Y,Z,W  |
|.d8g - Geometric Vertex FVF[definition_count]|definitions| - |
|I32U|BytesBetweenVertices (stride)| - |
|I32U|NumberOfVertices| - |

#### .d8g - Geometric Vertex FVF (8 Byte)

|Type|Name|Values|
|:-|:-|:-|
|I16U|flag| Often 0<br>0xFF signals end of definitions if found  |
|I16U|pos| Offset within the vertex stride block       |
|I16U|data| 1=Float2<br>2=Float3<br>4=byte[3..4]? (color?)<br>etc  |
|I8U|type|  0x00=Pos<br>0x03=Normal<br>0x05=TexCoord<br>0x0A=Color<br>...   |
|I8U|map| For UV channel index (0 or 1, etc.)  |


## Links
- [reshax.com - d8g-d8m-boh](https://reshax.com/topic/764-juiced-d8g-d8m-boh/)
- [reshax.com - d8t-d8w](https://reshax.com/topic/1631-juiced-pc-d8t-d8w/)

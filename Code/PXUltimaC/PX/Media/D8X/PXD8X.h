#pragma once

#ifndef PXD8XIncluded
#define PXD8XIncluded

#include <PX/Engine/PXResource.h>




typedef struct PXD8XArchiveEntry_
{
    PXI32U Hash;
    PXI32U Offset;
    PXI32S ChunksCount;
    PXI32S CompressedSize;
    PXI32S DecompressedSize; // Max chunk size 0x8000 (32768)

    PXFile* FileData;
}
PXD8XArchiveEntry;

typedef struct PXD8XDat_
{
    PXI32U TableEntryAmount;
    PXI32U TableOffset;

    PXD8XArchiveEntry* PXD8XDatEntry;
}
PXD8XDat;



typedef struct PXD8TextureTableEntry_
{
    PXI32U RawDataSize;
    char PixelFormat[4];
    PXI32U TextureWidth;
    PXI32U TextureHeight;
    PXI32U MinmapCount;
    PXI32U UnknownB;
    PXI32U UnknownC;
    PXI32U UnknownD;
    PXI32U UnknownE;
    PXI32U UnknownF;
    PXI32U UnknownG;
    PXI32U UnknownH;

    PXI32U Offset;
    PXFile* TextureFile;
}
PXD8TextureTableEntry;


typedef struct PXD8TextureSet_
{
    PXI32U TextureSetDataSize;
    PXI32U TextureTotalSize;
    PXI32U TextureEntryAmount;
}
PXD8TextureSet;

typedef struct PXD8TextureTable_
{ 
    PXI32U TextureAmount; // Total amount of textures in this file
    PXI32U TextureSetAmount; // Textures are grouped in sets
    PXI32U TotalSizeRequested; // All data?

    PXD8TextureSet* SetList;

   // char Unkown[12]; // ????

    PXD8TextureTableEntry* EntryList;
}
PXD8TextureTable;




#define PXD8GeometryVertexTypeInvalid 0
#define PXD8GeometryVertexTypeVerticesSprites 1
#define PXD8GeometryVertexTypeNormal 2

typedef struct PXD8GeometryHeader_
{
    PXI32U VertexType;
    PXI32U FaceType;
    PXI32U MeshAmount;
}
PXD8GeometryHeader;


typedef struct PXD8GeometryVertexFVF_
{
    PXI16U Flag;
    PXI16U Offset;
    PXI16U Format;
    PXI8U VertexType;
    PXI8U UVChannelIndex;
}
PXD8GeometryVertexFVF;

typedef struct PXD8GeometryVertexBlock_
{
    PXD8GeometryVertexFVF vertices;
    PXI32U definition_count;
    PXD8GeometryVertexFVF* definitions;
    PXI32U BytesBetweenVertices; // stride
    PXI32U NumberOfVertices;
}
PXD8GeometryVertexBlock;

typedef struct PXD8GeometryMesh_
{
    PXI32U TextureID;
    PXI32U Position;
    PXI32U RadiusBoundingSphere;
    PXI32U DistanceLODMin;
    PXI32U DistanceLODMax;
    PXI32U UnknownA;
    PXI32U UnknownB;
    PXI32U UnknownC;
    PXI32U UnknownD;
    PXD8GeometryVertexBlock VertexBlock;
    PXI32U face_count;
    PXI16U* IndexList;
    PXI32U node_count;
    PXMatrix4x4F* nodes;

    void* NodeListAdress;
    PXSize NodeListSize;

    void* IndexListArray;
    PXSize IndexListSize;
}
PXD8GeometryMesh;

typedef struct PXD8Geometry_
{
    PXD8GeometryHeader Header;
    PXD8GeometryMesh* MeshList;
}
PXD8Geometry;



typedef struct PXD8X_
{
    int x;   
}
PXD8X;



PXPublic PXResult PXAPI PXD8XLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);

PXPublic PXResult PXAPI PXD8DatLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);

// D8G
PXPrivate void PXAPI PXD8GGeometryVertexBlock(PXD8GeometryVertexBlock PXREF pxD8GeometryVertexBlock, PXFile PXREF pxFile);
PXPrivate void PXAPI PXD8GGeometryVertexFVFParse(PXD8GeometryVertexFVF PXREF pxD8GeometryVertexFVF, PXFile PXREF pxFile);
PXPublic PXResult PXAPI PXD8GLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXD8WLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo, PXFile* pxTexturePool);
PXPublic PXResult PXAPI PXD8TLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXD8MLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);

#endif

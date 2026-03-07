#pragma once


#ifndef PXD8GIncluded
#define PXD8GIncluded

#include <PX/Media/PXType.h>
#include <PX/Math/PXMatrix.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Entity/Model/PXModel.h>

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
    PXVector4F32 Position;
    PXI32U RadiusBoundingSphere;
    PXI32U DistanceLevelOfDetailMin;
    PXI32U DistanceLevelOfDetailMax;
    PXI32U NumberOfUnknownStructures;
    PXI32U UnknownA;
    PXI32U UnknownB;
    PXI32U UnknownC;
    PXI32U UnknownD;
    PXD8GeometryVertexBlock VertexBlock;
    PXI32U face_count;
    PXI16U* IndexList;

    PXMatrix4x4F* MatrixListAdress;
    PXI32U MatrixListSize;

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

PXPrivate void PXAPI PXD8GGeometryVertexFVFParse(PXD8GeometryVertexFVF PXREF pxD8GeometryVertexFVF, PXFile PXREF pxFile);

// D8G
PXPublic PXResult PXAPI PXD8GLoadFromFile(PXModel** pxModelREF, PXECSCreateInfo PXREF pxResourceLoadInfo);

#endif

#pragma once

#ifndef PXMeshGeometryIncluded
#define PXMeshGeometryIncluded
#include <PX/Engine/ECS/PXECS.h>
#include "PXIndexBuffer.h"
#include "PXVertexBuffer.h"

typedef struct PXMeshGeometry_
{
    PXECSInfo Info;

    PXIndexBuffer* IndexBuffer; // Contains IBO

    // Can either store interleaved data or seperate ones
    PXSize VertexBufferListAmount; // Realistically, is not more than 1,2,3,4,..

    union
    {
        PXVertexBuffer* VertexBufferPrime[PXEmbeddedArraySize]; // Can store position, normal, texturepos and one additional parameter 
        PXVertexBuffer** VertexBufferList; // Used as the same above, but can store many more.
    };
}
PXMeshGeometry;

typedef struct PXMeshGeometryCreateInfo_
{
    PXECSCreateInfo Info;

    // Vertex
    PXSize VertexBufferLayoutAmount;
    PXBufferLayoutEntry* VertexBufferLayoutList;

    // Index
    PXSize IndexBufferLayoutAmount;
    PXBufferLayoutEntry* IndexBufferLayoutList;
    PXSize PrimitveAmount;
    PXSize SegmentAmount;
}
PXMeshGeometryCreateInfo;

PXPublic PXResult PXAPI PXMeshGeometryRegisterToECS(void);
PXPublic PXResult PXAPI PXMeshGeometryCreate(PXMeshGeometry** pxMeshGeometryREF, PXMeshGeometryCreateInfo PXREF pxMeshGeometryCreateInfo);
PXPublic PXResult PXAPI PXMeshGeometryRelease(PXMeshGeometry PXREF pxMeshGeometry);

PXPublic PXResult PXAPI PXMeshGeometryVertexBufferLayoutPrint(const PXMeshGeometry PXREF pxMeshGeometry);

PXPublic PXVertexBuffer** PXAPI PXMeshGeometryVertexBufferListGET(const PXMeshGeometry PXREF pxMeshGeometry);
PXPublic PXVertexBuffer** PXAPI PXMeshGeometryVertexBufferListSET(PXMeshGeometry PXREF pxMeshGeometry, const PXSize amount);

#endif
#pragma once

#ifndef PXVertexBufferIncluded
#define PXVertexBufferIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Mesh/PXBufferLayout.h>

// Buffer to contain veretx data like position, normals, texturepositions, ...
// A mesh can contain multible vertexBuffers or just one thats interleaved
typedef struct PXVertexBuffer_
{
    PXECSInfo Info;

    union
    {
        PXI32U VBO; // OpenGL VertexBufferObject

        struct IDirect3DVertexBuffer9* Buffer_9;
        struct ID3D11Buffer* Buffer_11;
    };

    PXBuffer VertexData;

    PXBufferLayout Layout;
}
PXVertexBuffer;

typedef struct PXVertexBufferCreateInfo_
{
    PXECSCreateInfo Info;
}
PXVertexBufferCreateInfo;


typedef struct PXVertexBufferFormatInfo_
{
    PXI8U VertexAttributesAmount;

    PXI8U Format;
    PXI8U Position;
    PXI8U TexturePosition;
    PXI8U Normal;
    PXI8U Color;

    PXI8U Stride; // Byte per vertex

    // Bytes per data.
    // Can only be 1, 2, 3, 4. Nothing else!
    // char = 1
    // short = 2
    // float = 4
    PXI8U VertexElementSize;

    PXI8U VertexTotalSize; // XYZ = 3

    char AsText[32];
}
PXVertexBufferFormatInfo;


PXPublic PXResult PXAPI PXVertexBufferRegisterToECS(void);
PXPublic PXResult PXAPI PXVertexBufferCreate(PXVertexBuffer** pxVertexBufferREF, PXVertexBufferCreateInfo PXREF pxVertexBufferCreateInfo);
PXPublic PXResult PXAPI PXVertexBufferRelease(PXVertexBuffer PXREF pxVertexBuffer);

PXPublic PXResult PXAPI PXVertexBufferLayoutPrint(const PXVertexBuffer PXREF pxVertexBuffer);

#endif
#pragma once

#ifndef PXVertexBufferIncluded
#define PXVertexBufferIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Mesh/PXBufferLayout.h>

// Buffer to contain veretx data like position, normals, texturepositions, ...
// A mesh can contain multible vertexBuffers or just one thats interleaved
typedef struct PXVertexBuffer_
{
    PXECSInfo Info; // VertexArrayObject (VAO)

    union
    {
        PXI32U VBO;

        struct ID3D11Buffer* BufferD11;

        struct IDirect3DVertexBuffer9* Buffer_9;
    };

    PXBuffer VertexData;

    PXSize LayoutAmount;

    union
    {
        PXBufferLayout LayoutPrime[PXEmbeddedArraySize]; // Store up zo 4, saving allocations
        PXBufferLayout* LayoutList;
    };
}
PXVertexBuffer;

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


PXPublic PXBufferLayout* PXAPI PXVertexBufferLayoutGET(PXVertexBuffer PXREF pxVertexBuffer);

#endif
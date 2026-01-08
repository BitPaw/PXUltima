#pragma once

#ifndef PXIndexBufferIncluded
#define PXIndexBufferIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Component/Material/PXMaterial.h>
#include "PXBufferLayout.h"


typedef struct PXIndexSegment_
{
    PXMaterial* Material;
    PXSize DataRange;
}
PXIndexSegment;


// Index buffer, used to store the vertex render order.
// Additionally contains info about how to actually render, like modes.
typedef struct PXIndexBuffer_
{
    PXECSInfo Info;

    union
    {
        PXI32U IBO; // OpenGL

        struct IDirect3DIndexBuffer9* Buffer_9;
    };

    PXBuffer Data;

    PXI32U DataType; // Data type for buffer

    PXI32U DrawModeID; // How to draw, modes like triangle or lines

    PXSize LayoutListAmount;
    PXSize SegmentListAmount; // 0=Not allowed. 1=Mesh is single, n=Segmented

    union
    {
        PXIndexSegment SegmentPrime; // Only used if Segment is only one
        PXIndexSegment* SegmentList;
    };

    union
    {
        PXBufferLayout LayoutPrime;
        PXBufferLayout* LayoutList;
    };
}
PXIndexBuffer;


// Allocate an indexbuffer in a way to minimize size for given amount of vertex points
PXPublic void PXAPI PXIndexBufferPrepare(PXIndexBuffer PXREF pxIndexBuffer, const PXSize amountVertex, const PXSize amountMaterials);
PXPublic PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer PXREF pxIndexBuffer);

PXPublic PXIndexSegment* PXAPI PXIndexBufferSegmentListGET(const PXIndexBuffer PXREF pxIndexBuffer);
PXPublic PXBufferLayout* PXAPI PXIndexLayoutListGET(PXIndexBuffer PXREF pxIndexBuffer);

PXPublic PXSize PXAPI PXIndexIndexGET(const PXIndexBuffer PXREF pxIndexBuffer, const PXI8U type);




#endif
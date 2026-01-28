#pragma once

#ifndef PXIndexBufferIncluded
#define PXIndexBufferIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Component/Material/PXMaterial.h>
#include "PXBufferLayout.h"


typedef struct PXIndexSegmentEntry_
{
    PXMaterial* Material;
    PXSize DataRange;
}
PXIndexSegmentEntry;

typedef struct PXIndexSegment_
{
    PXSize SegmentListAmount; // 0=Not allowed. 1=Mesh is single, n=Segmented

    union
    {
        PXIndexSegmentEntry SegmentPrime; // Only used if Segment is only one
        PXIndexSegmentEntry* SegmentList;
    };
}
PXIndexSegment;

PXPublic void PXAPI PXIndexSegmentSET(PXIndexSegment PXREF pxIndexSegment, const PXSize amount);
PXPublic PXIndexSegmentEntry* PXAPI PXIndexSegmentGET(PXIndexSegment PXREF pxIndexSegment);



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

    PXIndexSegment Segment;
    PXBufferLayout Layout;
}
PXIndexBuffer;


typedef struct PXIndexBufferCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize AmountVertex;
    PXSize AmountSegments; // aka amount of materials
}
PXIndexBufferCreateInfo;

PXPublic PXResult PXAPI PXIndexBufferRegisterToECS(void);
PXPublic PXResult PXAPI PXIndexBufferCreate(PXIndexBuffer** pxIndexBuffer, PXIndexBufferCreateInfo PXREF pxIndexBufferCreateInfo);
PXPublic PXResult PXAPI PXIndexBufferRelease(PXIndexBuffer PXREF pxIndexBuffer);
PXPublic PXSize PXAPI PXIndexBufferAmount(const PXIndexBuffer PXREF pxIndexBuffer);


// Allocate an indexbuffer in a way to minimize size for given amount of vertex points
PXPublic PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer PXREF pxIndexBuffer);

PXPublic PXSize PXAPI PXIndexIndexGET(const PXIndexBuffer PXREF pxIndexBuffer, const PXI8U type);

typedef void (PXAPI* PXIndexBufferIndexSET)(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData);

PXPublic PXIndexBufferIndexSET PXAPI PXIndexBufferIndexFunc(const PXIndexBuffer PXREF pxIndexBuffer);

PXPublic void PXAPI PXIndexBufferIndex08SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData);
PXPublic void PXAPI PXIndexBufferIndex16SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData);
PXPublic void PXAPI PXIndexBufferIndex32SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData);
PXPublic void PXAPI PXIndexBufferIndex64SET(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize amount, const PXI32U PXREF indexData);

#endif
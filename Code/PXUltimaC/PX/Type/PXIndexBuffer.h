#pragma once

#ifndef PXIndexBufferIncluded
#define PXIndexBufferIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXMaterial.h>
#include <PX/Container/Buffer/PXBuffer.h>
#include <PX/Type/PXBufferLayout.h>

// Range of index data that is one and the same material.
// Or just different things.
typedef struct PXIndexSegment_
{
    // We cant put material here, because this will block mesh data from being reused.
    // When we reuse a cube mesh, we can't put different materials on it.
    // It needs to be stored in the model.
    //     PXMaterial* Material; 

    PXSize MaterialID;
    PXSize Range;
    //void* Data;

    // DrawMode?
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

        struct IDirect3DIndexBuffer9* DX_9;
    };

    PXBuffer Data; // Actual data

    PXI8U BitPerIndex;

    //PXI32U DataType; // Data type for buffer

    PXI32U DrawModeID; // How to draw, modes like triangle or lines


    PXSize SegmentAmount; // 0=Not allowed. 1=Mesh is single, n=Segmented
    union
    {
        PXIndexSegment SegmentPrime; // Only used if Segment is only one
        PXIndexSegment* SegmentList;
    };

    PXBufferLayout Layout;
}
PXIndexBuffer;


typedef struct PXIndexBufferCreateInfo_
{
    PXECSCreateInfo Info;

    PXBool IsConstData;

    union
    {
        // Byte data for indexs 
        void* IndexDataAddress;
        const void* IndexDataAddressConst;
    };

    PXSize IndexAmount; // Not the size of this data but only the amount!

    PXI8U Dimensions; // 3=Triangle, 4=Quads
    PXI8U BitPerIndex; // 8,16,32,64
    
    PXSize AmountSegments; // aka amount of materials
}
PXIndexBufferCreateInfo;

PXPublic void PXAPI PXIndexBufferRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXIndexBufferCreate(PXIndexBuffer** pxIndexBuffer, PXIndexBufferCreateInfo PXREF pxIndexBufferCreateInfo);
PXPublic PXResult PXAPI PXIndexBufferRelease(PXIndexBuffer PXREF pxIndexBuffer);
PXPublic PXSize PXAPI PXIndexBufferAmount(const PXIndexBuffer PXREF pxIndexBuffer);

PXPublic PXResult PXAPI PXIndexSegmentPrepare(PXIndexBuffer PXREF pxIndexBuffer, const PXSize amount);
PXPublic PXIndexSegment* PXAPI PXIndexSegmentGetViaIndex(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize index);
PXPublic PXResult PXAPI PXIndexSegmentSetViaIndex(PXIndexBuffer PXREF pxIndexBuffer, const PXSize index, const PXSize range);
PXPublic PXSize PXAPI PXIndexSegmentAmount(const PXIndexBuffer PXREF pxIndexBuffer);


PXPublic PXIndexSegment* PXAPI PXIndexSegmentGetViaMaterial(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize materialID);

PXPublic void* PXAPI PXIndexSegmentDataGetViaMaterial(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize materialID);
PXPublic void* PXAPI PXIndexSegmentDataGet(const PXIndexBuffer PXREF pxIndexBuffer, const PXSize segmentID);


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
#pragma once

#ifndef PXBufferLayoutIncluded
#define PXBufferLayoutIncluded

#include <PX/Type/PXType.h>

typedef enum PXVertexBufferLayoutType_
{
    PXVertexBufferLayoutTypeInvalid,
    PXVertexBufferLayoutTypePosition,
    PXVertexBufferLayoutTypeNormal,
    PXVertexBufferLayoutTypeTexturePos,
    PXVertexBufferLayoutTypeColor,
    PXVertexBufferLayoutTypeCustom,
}
PXVertexBufferLayoutType;

// Describes how the data is stuctured
typedef struct PXBufferLayoutEntry_
{
    PXI32U TypeData; // PXTypeF32
    PXVertexBufferLayoutType TypeUse;
    PXI8U AmountOfElements; // 2=2D, 3=3D, 4=RGBA, ...
    PXBool UpdatesPerPrimitive; // 1=Instancing
    PXSize RelativeOffsetInBytes;
}
PXBufferLayoutEntry;

typedef struct PXBufferLayout_
{
    PXSize LayoutAmount;

    union
    {
        PXBufferLayoutEntry LayoutPrime[PXEmbeddedArraySize]; // Store up zo 4, saving allocations
        PXBufferLayoutEntry* LayoutList;
    };
}
PXBufferLayout;

PXPublic PXResult PXAPI PXBufferLayoutPrepare(PXBufferLayout PXREF pxBufferLayout, const PXSize amount);
PXPublic PXBufferLayoutEntry* PXAPI PXBufferLayoutGetViaIndex(PXBufferLayout PXREF pxBufferLayout, const PXSize index);
PXPublic PXSize PXAPI PXBufferLayoutAmount(const PXBufferLayout PXREF pxBufferLayout);

#endif
#pragma once

#ifndef PXBufferLayoutIncluded
#define PXBufferLayoutIncluded

#include <PX/Media/PXType.h>

// Describes how the data is stuctured
typedef struct PXBufferLayoutEntry_
{
    PXI32U Format;
    PXI8U AmountOfElements; // 2=2D, 3=3D, 4=RGBA, ...
    PXI8U Type;
    PXBool UpdatePerPrimitive; // 1=Instancing
    PXSize Entrys;
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

typedef struct PXBufferLayoutInfo_
{
    PXSize Amount;
}
PXBufferLayoutInfo;


PXPublic void PXAPI PXBufferLayoutSET
(
    PXBufferLayout PXREF pxBufferLayout,
    PXBufferLayoutInfo PXREF pxBufferLayoutInfo
);
PXPublic PXBufferLayoutEntry* PXAPI PXBufferLayoutGET(PXBufferLayout PXREF pxBufferLayout);


#endif
#pragma once

#ifndef PXBufferLayoutIncluded
#define PXBufferLayoutIncluded

#include <PX/Media/PXType.h>

// Describes how the data is stuctured
typedef struct PXBufferLayout_
{
    PXI32U Format;
    PXI8U AmountOfElements; // 2=2D, 3=3D, 4=RGBA, ...
    PXI8U Type;
    PXBool UpdatePerPrimitive; // 1=Instancing
}
PXBufferLayout;

#endif
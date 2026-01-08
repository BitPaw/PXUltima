#pragma once

#ifndef PXRectangleIncluded
#define PXRectangleIncluded
#include <PX/Media/PXType.h>
#include <PX/Math/PXVector.h>

typedef struct PXRectangleXYWHI32_
{
    PXI32S X;
    PXI32S Y;
    PXI32U Width;
    PXI32U Height;
}
PXRectangleXYWHI32;

typedef struct PXRectangleLTRBI32_
{
    PXI32S Left;
    PXI32S Top;
    PXI32S Right;
    PXI32S Bottom;
}
PXRectangleLTRBI32;

// PXPadding;: Offset the space as the child container (Make object take more space)
// PXMargin;  Offset the whole child-Container (Make empty space)
typedef struct PXRectangleLTRBF32_
{
    PXF32 Left;
    PXF32 Top;
    PXF32 Right;
    PXF32 Bottom;
}
PXRectangleLTRBF32;


typedef struct PXRectangleVertexF32_
{
    union
    {
        struct
        {
            PXF32 AX;
            PXF32 AY;
        };

        PXF32 A[2];
    };

    union
    {
        struct
        {
            PXF32 BX;
            PXF32 BY;
        };

        PXF32 B[2];
    };
}
PXRectangleVertexF32;

PXPublic void PXAPI PXRectangleLTRBI32ToXYWHI32(const PXRectangleLTRBI32 PXREF pxRectangleLTRBI32, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32);
PXPublic void PXAPI PXRectangleXYWHI32ToLTRBI32(const PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, PXRectangleLTRBI32 PXREF pxRectangleLTRBI32);
PXPublic void PXAPI PXRectangleXYWHI32ToVertex(const PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const PXVector2F32 PXREF screenSize, PXRectangleVertexF32 PXREF pxRectangleVertexF32);

#endif
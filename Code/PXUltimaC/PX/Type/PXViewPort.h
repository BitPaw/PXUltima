#pragma once

#ifndef PXViewPortIncluded
#define PXViewPortIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>

// The rectangle of the view you can render in.
typedef struct PXViewPort_
{
    PXI32S X;
    PXI32S Y;
    PXI32S Width;
    PXI32S Height;
    PXF32 ClippingMinimum;
    PXF32 ClippingMaximum;
}
PXViewPort;


PXPublic PXResult PXAPI PXViewPortSetWH
(
    PXViewPort PXREF pxViewPort,
    const PXI32S width,
    const PXI32S height
);
PXPublic PXResult PXAPI PXViewPortSetXYWH
(
    PXViewPort PXREF pxViewPort,
    const PXI32S x,
    const PXI32S y,
    const PXI32S width,
    const PXI32S height
);

#endif
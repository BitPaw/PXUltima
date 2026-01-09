#include "PXRenderingSystem.h"

PXResult PXAPI PXViewPortSetWH(PXViewPort PXREF pxViewPort, const PXI32S width, const PXI32S height)
{
    return PXViewPortSetXYWH(pxViewPort, 0, 0, width, height);
}

PXResult PXAPI PXViewPortSetXYWH(PXViewPort PXREF pxViewPort, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
    pxViewPort->X = x;
    pxViewPort->Y = y;
    pxViewPort->Width = width;
    pxViewPort->Height = height;
    pxViewPort->ClippingMinimum = 0;
    pxViewPort->ClippingMaximum = 1;

    return PXResultOK;
}
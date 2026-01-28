#include "PXRenderingSystem.h"


const char PXRenderingSystemText[] = "RenderingSystem";
const PXI8U PXRenderingSystemTextLength = sizeof(PXRenderingSystemText);
const PXECSRegisterInfoStatic PXRenderingSystemRegisterInfoStatic =
{
    {sizeof(PXRenderingSystemText), sizeof(PXRenderingSystemText), PXRenderingSystemText, TextFormatASCII},
    sizeof(PXRenderingSystem),
    __alignof(PXRenderingSystem),
    PXECSTypeSystem,
    PXNull,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXRenderingSystemRegisterInfoDynamic;


PXResult PXAPI PXRenderingSystemRegisterToECS()
{
    PXECSRegister(&PXRenderingSystemRegisterInfoStatic, &PXRenderingSystemRegisterInfoDynamic);

    return PXResultOK;
}

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
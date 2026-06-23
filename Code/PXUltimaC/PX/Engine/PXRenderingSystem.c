#include "PXRenderingSystem.h"

#include <PX/OS/Console/PXConsole.h>

const char PXRenderingSystemText[] = "Renderer-PX";
const PXI8U PXRenderingSystemTextLength = sizeof(PXRenderingSystemText);
const PXECSRegisterInfoStatic PXRenderingSystemRegisterInfoStatic =
{
    {sizeof(PXRenderingSystemText), sizeof(PXRenderingSystemText), PXRenderingSystemText, TextFormatASCII},
    sizeof(PXRenderer),
    __alignof(PXRenderer),
    PXECSTypeSystem,
    PXRendererCreate,
    PXRendererRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXRenderingSystemRegisterInfoDynamic;


void PXAPI PXRenderingSystemRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXRenderingSystemRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXRenderingSystemRegisterInfoDynamic;
}

PXResult PXAPI PXRendererCreate(PXRenderer** pxRendererREF, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXRenderer* pxRenderer = PXNull;

    if(!(pxRendererREF && pxECSCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxECSCreateInfo->Static = &PXRenderingSystemRegisterInfoStatic;
    pxECSCreateInfo->Dynamic = &PXRenderingSystemRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxRendererREF, pxECSCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxRenderer = *pxRendererREF;

    // Create default failback camera
    PXCameraCreateInfo pxCameraCreateInfo;
    PXClear(PXCameraCreateInfo, &pxCameraCreateInfo);
    pxCameraCreateInfo.Perspective = PXCameraPerspective3D;

    PXCameraCreate(&pxRenderer->CameraCurrent, &pxCameraCreateInfo);

    return PXResultOK;
}

PXResult PXAPI PXRendererRelease(PXRenderer PXREF pxRenderer)
{
    return PXResultOK;
}

PXResult PXAPI PXRendererSceneDraw(PXRenderer PXREF pxRenderer, PXScene PXREF pxScene)
{
    return PXResultOK;
}

PXCamera* PXAPI PXRendererCameraCurrent(const PXRenderer PXREF pxRenderer)
{
    if(pxRenderer->CameraCurrent)
    {
        return pxRenderer->CameraCurrent;
    }

    return pxRenderer->CameraDefault;
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
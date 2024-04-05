#include "PXDirectX10x1.h"

#include <OS/Console/PXConsole.h>

#if PXDX10x1Enable
#pragma comment(lib, "D3d10_1.lib")
#endif

PXActionResult PXAPI PXDirectX10x1Initialize(PXDirectX10x1* const pxDirectX10x1)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX10.1",
        "Initialize",
        "Start..."
    );
#endif

    return PXActionRefusedNotImplemented;

#if 0
    D3D10_FEATURE_LEVEL1 featureLevel = 0;
    D3D10_DRIVER_TYPE dxDriverType = 0;

    switch (pxDirectXVersion)
    {
    case PXDirectXVersion10x1Simulate9x1:   featureLevel = D3D10_FEATURE_LEVEL_9_1;   break;
    case PXDirectXVersion10x1Simulate9x2:   featureLevel = D3D10_FEATURE_LEVEL_9_2;   break;
    case PXDirectXVersion10x1Simulate9x3:   featureLevel = D3D10_FEATURE_LEVEL_9_3;   break;
    case PXDirectXVersion10x1Simulate10x0:  featureLevel = D3D10_FEATURE_LEVEL_10_0;   break;
    case PXDirectXVersion10x1:              featureLevel = D3D10_FEATURE_LEVEL_10_1;   break;
    }

    switch (pxDirectXDriverType)
    {
    case PXDirectXDriverTypeHardwareDevice: dxDriverType = D3D10_DRIVER_TYPE_HARDWARE; break;
    case PXDirectXDriverTypeReferencDevice:dxDriverType = D3D10_DRIVER_TYPE_REFERENCE; break;
    case PXDirectXDriverTypeReferencDeviceWithoutRender:dxDriverType = D3D10_DRIVER_TYPE_NULL; break;
    case PXDirectXDriverTypeSoftware:dxDriverType = D3D10_DRIVER_TYPE_WARP; break;

    default:
        return PXActionRefuedInputInvalid;
    }

    const HRESULT result = D3D10CreateDevice1
    (
        &pxDirectX->VideoAdapter,
        dxDriverType,
        dxSoftware,
        Flags,
        featureLevel,
        D3D10_SDK_VERSION,
        &pxDirectX->DX10X1
    );
#endif
}

PXActionResult PXAPI PXDirectX10x1Release(PXDirectX10x1* const pxDirectX10x1)
{
    return PXActionRefusedNotImplemented;
}
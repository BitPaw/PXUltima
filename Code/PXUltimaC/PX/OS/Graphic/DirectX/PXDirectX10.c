#include "PXDirectX10.h"

#include <PX/OS/Console/PXConsole.h>

#if PXDX10Enable
#include <d3d10.h>
#endif

#if PXDX10Enable
//#include <d3d10shader.h>
#pragma comment(lib, "D3d10.lib")
#endif

PXResult PXAPI PXDirectX10Initialize(PXDirectX10 PXREF pxDirectX)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX10",
        "Initialize",
        "Start..."
    );
#endif

#if 0
    D3D10_DRIVER_TYPE dxDriverType = 0;

    switch (pxDirectXDriverType)
    {
    case PXDirectXDriverTypeHardwareDevice:
        dxDriverType = D3D10_DRIVER_TYPE_HARDWARE;
        break;
    case PXDirectXDriverTypeReferencDevice:
        dxDriverType = D3D10_DRIVER_TYPE_REFERENCE;
        break;
    case PXDirectXDriverTypeReferencDeviceWithoutRender:
        dxDriverType = D3D10_DRIVER_TYPE_NULL;
        break;
    case PXDirectXDriverTypeSoftware:
        dxDriverType = D3D10_DRIVER_TYPE_WARP;
        break;

    default:
        return PXActionRefuedInputInvalid;
    }

    const HRESULT result = D3D10CreateDevice
                           (
                               &pxDirectX->VideoAdapter,
                               dxDriverType,
                               dxSoftware,
                               Flags,
                               D3D10_SDK_VERSION,
                               &pxDirectX->DX10
                           );

    pxDirectX->DirectXVersion = pxDirectXDriverType;

#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXDirectX10Release(PXDirectX10 PXREF pxDirectX)
{
    return PXActionRefusedNotImplemented;
}

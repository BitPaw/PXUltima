#include "PXDirectX10.h"


#if PXDX10Enable
#include <d3d10.h>
#endif

#if PXDX10Enable
//#include <d3d10shader.h>
#pragma comment(lib, "D3d10.lib")
#endif

PXActionResult PXAPI PXDirectX10Initialize(PXDirectX10* const pxDirectX)
{
#if 0
    D3D10_DRIVER_TYPE dxDriverType = 0;

    switch (pxDirectXDriverType)
    {
    case PXDirectXDriverTypeHardwareDevice: dxDriverType = D3D10_DRIVER_TYPE_HARDWARE; break;
    case PXDirectXDriverTypeReferencDevice:dxDriverType = D3D10_DRIVER_TYPE_REFERENCE; break;
    case PXDirectXDriverTypeReferencDeviceWithoutRender:dxDriverType = D3D10_DRIVER_TYPE_NULL; break;
    case PXDirectXDriverTypeSoftware:dxDriverType = D3D10_DRIVER_TYPE_WARP; break;

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

PXActionResult PXAPI PXDirectX10Release(PXDirectX10* const pxDirectX)
{
    return PXActionRefusedNotImplemented;
}
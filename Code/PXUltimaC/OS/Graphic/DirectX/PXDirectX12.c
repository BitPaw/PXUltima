#include "PXDirectX12.h"

#if PXDX12Enable
#include <d3d12.h>
#endif


#if PXDX12Enable
#pragma comment(lib, "D3d12.lib")
#endif


PXActionResult PXAPI PXDirectX12Initialize(PXDirectX12* const pxDirectX12)
{
#if 0
    D3D_FEATURE_LEVEL featureLevel;

    switch (pxDirectXVersion)
    {
    case PXDirectXVersion12Emulate1x0Core: featureLevel = D3D_FEATURE_LEVEL_1_0_CORE; break;
    case PXDirectXVersion12Emulate9x1:featureLevel = D3D_FEATURE_LEVEL_9_1; break;
    case PXDirectXVersion12Emulate9x2:featureLevel = D3D_FEATURE_LEVEL_9_2; break;
    case PXDirectXVersion12Emulate9x3:featureLevel = D3D_FEATURE_LEVEL_9_3; break;
    case PXDirectXVersion12Emulate10x0:featureLevel = D3D_FEATURE_LEVEL_10_0; break;
    case PXDirectXVersion12Emulate10x1:featureLevel = D3D_FEATURE_LEVEL_10_1; break;
    case PXDirectXVersion12Emulate11x0:featureLevel = D3D_FEATURE_LEVEL_11_0; break;
    case PXDirectXVersion12Emulate11x1:featureLevel = D3D_FEATURE_LEVEL_11_1; break;
    case PXDirectXVersion12Emulate12x0:featureLevel = D3D_FEATURE_LEVEL_12_0; break;
    case PXDirectXVersion12Emulate12x1:featureLevel = D3D_FEATURE_LEVEL_12_1; break;
    case PXDirectXVersion12Emulate12x2:featureLevel = D3D_FEATURE_LEVEL_12_2; break;
    }

    void* adpater = 0;

    const HRESULT result = D3D12CreateDevice
    (
        adpater,
        featureLevel,
        0,
        0
    );
#endif
}

PXActionResult PXAPI PXDirectX12Release(PXDirectX12* const pxDirectX12)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX12DevicePhysicalListAmountFunction(PXDirectX12* const pxDirectX12, PXInt32U* const amountOfAdapters)
{
#if 0
    D3D_FEATURE_LEVEL featureLevel;

    switch (pxDirectXVersion)
    {
    case PXDirectXVersion12Emulate1x0Core: featureLevel = D3D_FEATURE_LEVEL_1_0_CORE; break;
    case PXDirectXVersion12Emulate9x1:featureLevel = D3D_FEATURE_LEVEL_9_1; break;
    case PXDirectXVersion12Emulate9x2:featureLevel = D3D_FEATURE_LEVEL_9_2; break;
    case PXDirectXVersion12Emulate9x3:featureLevel = D3D_FEATURE_LEVEL_9_3; break;
    case PXDirectXVersion12Emulate10x0:featureLevel = D3D_FEATURE_LEVEL_10_0; break;
    case PXDirectXVersion12Emulate10x1:featureLevel = D3D_FEATURE_LEVEL_10_1; break;
    case PXDirectXVersion12Emulate11x0:featureLevel = D3D_FEATURE_LEVEL_11_0; break;
    case PXDirectXVersion12Emulate11x1:featureLevel = D3D_FEATURE_LEVEL_11_1; break;
    case PXDirectXVersion12Emulate12x0:featureLevel = D3D_FEATURE_LEVEL_12_0; break;
    case PXDirectXVersion12Emulate12x1:featureLevel = D3D_FEATURE_LEVEL_12_1; break;
    case PXDirectXVersion12Emulate12x2:featureLevel = D3D_FEATURE_LEVEL_12_2; break;
    }

    void* adpater = 0;

    const HRESULT result = D3D12CreateDevice
    (
        adpater,
        featureLevel,
        0,
        0
    );
#endif

    return PXActionSuccessful;
}
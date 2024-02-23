#include "PXDirectX11.h"

#if PXDX11Enable
#include <d3d11.h>
#endif

#if PXDX11Enable
#pragma comment(lib, "D3d11.lib")
#endif

PXActionResult PXAPI PXDirectX11Initialize(PXDirectX11* const pxDirectX)
{
#if 0
    UINT amount = 1;
    D3D_FEATURE_LEVEL pFeatureLevels = 0;
    D3D_FEATURE_LEVEL pFeatureLevelResult = 0;
    D3D_DRIVER_TYPE dxDriverType = 0;

    switch (pxDirectXVersion)
    {
    case PXDirectXVersion11Emulate1x0Core: pFeatureLevels = D3D_FEATURE_LEVEL_1_0_CORE; break;
    case PXDirectXVersion11Emulate9x1:pFeatureLevels = D3D_FEATURE_LEVEL_9_1; break;
    case PXDirectXVersion11Emulate9x2:pFeatureLevels = D3D_FEATURE_LEVEL_9_2; break;
    case PXDirectXVersion11Emulate9x3:pFeatureLevels = D3D_FEATURE_LEVEL_9_3; break;
    case PXDirectXVersion11Emulate10x0:pFeatureLevels = D3D_FEATURE_LEVEL_10_0; break;
    case PXDirectXVersion11Emulate10x1:pFeatureLevels = D3D_FEATURE_LEVEL_10_1; break;
    case PXDirectXVersion11Emulate11x0:pFeatureLevels = D3D_FEATURE_LEVEL_11_0; break;
    case PXDirectXVersion11Emulate11x1:pFeatureLevels = D3D_FEATURE_LEVEL_11_1; break;
    }

    switch (pxDirectXDriverType)
    {
    case PXDirectXDriverTypeHardwareDevice: dxDriverType = D3D_DRIVER_TYPE_HARDWARE; break;
    case PXDirectXDriverTypeReferencDevice:dxDriverType = D3D_DRIVER_TYPE_REFERENCE; break;
    case PXDirectXDriverTypeReferencDeviceWithoutRender:dxDriverType = D3D_DRIVER_TYPE_NULL; break;
    case PXDirectXDriverTypeSoftware:dxDriverType = D3D_DRIVER_TYPE_WARP; break;

    default:
        return PXActionRefuedInputInvalid;
    }


    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
    const UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    const HRESULT result = D3D11CreateDevice
    (
        PXNull, // &pxDirectX->VideoAdapter,
        dxDriverType,
        PXNull,
        Flags,
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &pxDirectX->DX11,
        PXNull,
        &pxDirectX->DX11Context
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX11Release(PXDirectX11* const pxDirectX)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX11Clear(PXDirectX11* const pxDirectX11, const PXColorRGBAF* const pxColorRGBAF)
{
    const FLOAT rgba[4] = { pxColorRGBAF->Red, pxColorRGBAF->Green, pxColorRGBAF->Blue, pxColorRGBAF->Alpha };

    pxDirectX11->Context->lpVtbl->ClearRenderTargetView
    (
        pxDirectX11->Context,
        PXNull,
        rgba
    );

    return PXActionNotSupportedByLibrary;
}

PXActionResult PXAPI PXDirectX11VertexBufferCreate(PXDirectX11* const pxDirectX11, PXVertexBuffer* const pxVertexBuffer)
{
    ID3D11Buffer** g_pVertexBuffer = &(ID3D11Buffer*)pxVertexBuffer->ResourceID.DirectXInterface;

    // Fill in a buffer description.
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = pxVertexBuffer->VertexDataSize;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pxVertexBuffer->VertexData;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    // Create the vertex buffer.
    const HRESULT bufferCreateResult = pxDirectX11->Device->lpVtbl->CreateBuffer(pxDirectX11->Device, &bufferDesc, &InitData, g_pVertexBuffer);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11* const pxDirectX, PXInt32U* const value)
{
    //*value = pxDirectX->DX11->lpVtbl->GetAvailableTextureMem(pxDirectX->DX9);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX11Texture2DCreate(PXDirectX11* const pxDirectX, PXTexture2D* const pxTexture2D)
{
#if 0
    D3D11_TEXTURE2D_DESC textureDescription;
    textureDescription.Width = pxTexture->Image.Width,
    textureDescription.Height = pxTexture->Image.Height,
    textureDescription.MipLevels = 0;
    textureDescription.ArraySize = pxTexture->Image.PixelDataSize;
    textureDescription.Format = PXDirectXColorFormatFromID(pxTexture->Image.Format);
    textureDescription.Usage = 0;
    textureDescription.BindFlags = 0;
    textureDescription.CPUAccessFlags = 0;
    textureDescription.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA pInitialData;
    pInitialData.pSysMem = 0;
    pInitialData.SysMemPitch = 0;
    pInitialData.SysMemSlicePitch = 0;

    const HRESULT result = pxDirectX->DX11->lpVtbl->CreateTexture2D
    (
        pxDirectX->DX11,
        &textureDescription,
        &pInitialData,
        &(ID3D11Texture2D*)pxTexture->ResourceID.DirectXInterface
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX11Texture3DCreate(PXDirectX11* const pxDirectX, PXTexture3D* const pxTexture3D)
{
    D3D11_TEXTURE3D_DESC textureDescription;
    textureDescription.Width = pxTexture3D->Width;
    textureDescription.Height = pxTexture3D->Height;
    textureDescription.Depth = pxTexture3D->Depth;
    textureDescription.MipLevels = 0;
    textureDescription.Format = PXDirectXColorFormatFromID(pxTexture3D->Format);
    textureDescription.Usage = 0;
    textureDescription.BindFlags = 0;
    textureDescription.CPUAccessFlags = 0;
    textureDescription.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA pInitialData;
    pInitialData.pSysMem = 0;
    pInitialData.SysMemPitch = 0;
    pInitialData.SysMemSlicePitch = 0;

    const HRESULT result = pxDirectX->Device->lpVtbl->CreateTexture3D
    (
        pxDirectX->Device,
        &textureDescription,
        &pInitialData,
        &(ID3D11Texture3D*)pxTexture3D->ResourceID.DirectXInterface
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX11TextureCubeCreate(PXDirectX11* const pxDirectX, PXTextureCube* const pxTextureCube)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11* const pxDirectX, PXInt32U* const amountOfAdapters)
{
#if 0
    UINT amount = 1;
    D3D_FEATURE_LEVEL pFeatureLevels = 0;
    D3D_FEATURE_LEVEL pFeatureLevelResult = 0;
    D3D_DRIVER_TYPE dxDriverType = 0;

    switch (pxDirectXVersion)
    {
    case PXDirectXVersion11Emulate1x0Core: pFeatureLevels = D3D_FEATURE_LEVEL_1_0_CORE; break;
    case PXDirectXVersion11Emulate9x1:pFeatureLevels = D3D_FEATURE_LEVEL_9_1; break;
    case PXDirectXVersion11Emulate9x2:pFeatureLevels = D3D_FEATURE_LEVEL_9_2; break;
    case PXDirectXVersion11Emulate9x3:pFeatureLevels = D3D_FEATURE_LEVEL_9_3; break;
    case PXDirectXVersion11Emulate10x0:pFeatureLevels = D3D_FEATURE_LEVEL_10_0; break;
    case PXDirectXVersion11Emulate10x1:pFeatureLevels = D3D_FEATURE_LEVEL_10_1; break;
    case PXDirectXVersion11Emulate11x0:pFeatureLevels = D3D_FEATURE_LEVEL_11_0; break;
    case PXDirectXVersion11Emulate11x1:pFeatureLevels = D3D_FEATURE_LEVEL_11_1; break;
    }

    switch (pxDirectXDriverType)
    {
    case PXDirectXDriverTypeHardwareDevice: dxDriverType = D3D_DRIVER_TYPE_HARDWARE; break;
    case PXDirectXDriverTypeReferencDevice:dxDriverType = D3D_DRIVER_TYPE_REFERENCE; break;
    case PXDirectXDriverTypeReferencDeviceWithoutRender:dxDriverType = D3D_DRIVER_TYPE_NULL; break;
    case PXDirectXDriverTypeSoftware:dxDriverType = D3D_DRIVER_TYPE_WARP; break;

    default:
        return PXActionRefuedInputInvalid;
    }


    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };
    const UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    const HRESULT result = D3D11CreateDevice
    (
        PXNull, // &pxDirectX->VideoAdapter,
        dxDriverType,
        PXNull,
        Flags,
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &pxDirectX->DX11,
        PXNull,
        &pxDirectX->DX11Context
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11* const pxDirectX, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList)
{
    return PXActionRefusedNotImplemented;
}
#include "PXDirectX11.h"

#if PXDX11Enable
#include <d3d11.h>
#endif

#if PXDX11Enable
#pragma comment(lib, "D3d11.lib")
#endif

#include <OS/Graphic/PXGraphic.h>
#include <OS/GUI/PXGUI.h>
#include <OS/Console/PXConsole.h>
#include <OS/Memory/PXMemory.h>


typedef HRESULT (WINAPI* PXD3D11CreateDeviceAndSwapChain)
(
    __in_opt IDXGIAdapter* pAdapter,
    D3D_DRIVER_TYPE DriverType,
    HMODULE Software,
    UINT Flags,
    __in_ecount_opt(FeatureLevels) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
    UINT FeatureLevels,
    UINT SDKVersion,
    __in_opt CONST DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
    __out_opt IDXGISwapChain** ppSwapChain,
    __out_opt ID3D11Device** ppDevice,
    __out_opt D3D_FEATURE_LEVEL* pFeatureLevel,
    __out_opt ID3D11DeviceContext** ppImmediateContext
);


PXActionResult PXAPI PXDirectX11Initialize(PXDirectX11* const pxDirectX11, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
    PXD3D11CreateDeviceAndSwapChain createDeviceAndSwapChain = PXNull;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX11",
        "Initialize",
        "---Start---"
    );
#endif

    // OpenLib
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDirectX11->DirectX11Library, "D3D11.DLL");

        if(PXActionSuccessful != pxActionResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "DirectX11",
                "Initialize",
                "Library load failed!"
            );
#endif

            return PXActionRefusedNotSupported;
        }


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "DirectX11",
            "Initialize",
            "Library linked successfuly"
        );
#endif

        // Link functions
        {
            PXLibraryGetSymbolA(&pxDirectX11->DirectX11Library, &createDeviceAndSwapChain, "D3D11CreateDeviceAndSwapChain");
        }
    }


    // Fetch functions
    {
        PXGraphic* pxGraphic = pxGraphicInitializeInfo->Graphic;
        pxGraphic->TextureAction = PXDirectX11TextureAction;
        pxGraphic->ShaderVariableSet = PXNull;
        pxGraphic->ScreenBufferRead = PXNull;
        pxGraphic->DrawModeSet = PXNull;
        pxGraphic->DrawColorRGBAF = PXNull;
        pxGraphic->RectangleDraw = PXNull;
        pxGraphic->RectangleDrawTx = PXNull;
        pxGraphic->SwapIntervalSet = PXNull;
        pxGraphic->SwapIntervalGet = PXNull;
        pxGraphic->DevicePhysicalListAmount = PXNull;
        pxGraphic->DevicePhysicalListFetch = PXNull;
        pxGraphic->Release = PXNull;
        pxGraphic->Select = PXNull;
        pxGraphic->Deselect = PXNull;
        pxGraphic->Clear = PXNull;
        pxGraphic->SceneDeploy = PXNull;
        pxGraphic->ViewPortGet = PXNull;
        pxGraphic->ViewPortSet = PXNull;
        pxGraphic->ShaderProgramCreate = PXNull;
        pxGraphic->ShaderProgramSelect = PXNull;
        pxGraphic->ShaderProgramDelete = PXNull;
        pxGraphic->SceneBegin = PXNull;
        pxGraphic->SceneEnd = PXNull;
        pxGraphic->DrawScriptCreate = PXNull;
        pxGraphic->DrawScriptBegin = PXNull;
        pxGraphic->DrawScriptEnd = PXNull;
        pxGraphic->DrawScriptDelete = PXNull;
        pxGraphic->DrawScriptExecute = PXNull;
        pxGraphic->ModelRegister = PXNull;
        pxGraphic->ModelDraw = PXNull;
        pxGraphic->ModelSelect = PXNull;
        pxGraphic->ModelRelease = PXNull;
        pxGraphic->LightSet = PXNull;
        pxGraphic->LightGet = PXNull;
        pxGraphic->LightEnableSet = PXNull;
        pxGraphic->LightEnableGet = PXNull;
    }


    // Fetch GOU
    {

    }

    // Create Device
    {
        D3D_DRIVER_TYPE dxDriverType = 0;


        switch(pxGraphicInitializeInfo->DirectXDriverType)
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

        UINT amount = 1;
        D3D_FEATURE_LEVEL featureLevelResult = 0;

        UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined( DEBUG ) || defined( _DEBUG )
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  

        DXGI_SWAP_CHAIN_DESC dxGISwapChainDescription;
        PXClear(DXGI_SWAP_CHAIN_DESC, &dxGISwapChainDescription);
        dxGISwapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
        dxGISwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
        dxGISwapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        dxGISwapChainDescription.SampleDesc.Count = 1;
        dxGISwapChainDescription.SampleDesc.Quality = 0;
        dxGISwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxGISwapChainDescription.BufferCount = 1;
        dxGISwapChainDescription.OutputWindow = pxGraphicInitializeInfo->WindowReference->ID;
        dxGISwapChainDescription.Windowed = 1u;

        const HRESULT resultID = createDeviceAndSwapChain // D3D11CreateDevice()
        (
            PXNull, // &pxDirectX->VideoAdapter,
            dxDriverType,
            PXNull,
            flags,
            featureLevels,
            numFeatureLevels,
            D3D11_SDK_VERSION,
            &dxGISwapChainDescription,
            &pxDirectX11->SwapChain,
            &pxDirectX11->Device,
            &featureLevelResult,
            &pxDirectX11->Context
        );
        const PXActionResult result = PXWindowsHandleErrorFromID(resultID);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX11",
            "Initialize",
            "Device and Swap Chain"
        );
#endif
    }


    // xxxx
    {
        const HRESULT getBufferResult = pxDirectX11->SwapChain->lpVtbl->GetBuffer
        (
            pxDirectX11->SwapChain,
            0,
            &IID_ID3D11Texture2D,
            &pxDirectX11->FrameBuffer
        );

        const HRESULT createRenderResultID = pxDirectX11->Device->lpVtbl->CreateRenderTargetView
        (
            pxDirectX11->Device,
            pxDirectX11->FrameBuffer,
            0,
            &pxDirectX11->RenderTargetView
        );
 
        //pxDirectX11->FrameBuffer->lpVtbl->Release();
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX11",
        "Initialize",
        "Done..."
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX11Release(PXDirectX11* const pxDirectX)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX11TextureAction(PXDirectX11* const pxDirectX11, struct PXGraphicTexturInfo_* const pxGraphicTexturInfo)
{
    switch(pxGraphicTexturInfo->Action)
    {
        case PXResourceActionCreate:
        {
            switch(pxGraphicTexturInfo->Type)
            {
                case PXGraphicTextureType1D:
                {
                    PXTexture1D* pxTexture1D = (PXTexture1D*)pxGraphicTexturInfo->TextureReference;
                    ID3D11Texture1D** dx11Texture1D = &((ID3D11Texture1D*)pxTexture1D->ResourceID.DirectXInterface);

                    D3D11_TEXTURE1D_DESC desc;
                    desc.Width = pxTexture1D->Image->Width;
                    desc.MipLevels = 0;// static_cast<UINT>(mipCount);
                    desc.ArraySize = 0;//static_cast<UINT>(arraySize);
                    desc.Format = 0;
                    desc.Usage = D3D11_USAGE_DEFAULT;
                    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                    desc.CPUAccessFlags = 0;
                    desc.MiscFlags = 0;

                    D3D11_SUBRESOURCE_DATA initialData;
                    initialData.pSysMem = 0;
                    initialData.SysMemPitch = 0;
                    initialData.SysMemSlicePitch = 0;

                    const HRESULT hr = pxDirectX11->Device->lpVtbl->CreateTexture1D
                    (
                        pxDirectX11->Device,
                        &desc, 
                        &initialData,
                        dx11Texture1D
                    );
                    const PXBool success = SUCCEEDED(hr);

                    if(!success)
                    {
                        return PXActionFailedResourceRegister;
                    }

#if 0
                    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
                    PXClear(D3D11_SHADER_RESOURCE_VIEW_DESC, &SRVDesc);
                    SRVDesc.Format = format;

                    if(arraySize > 1)
                    {
                        SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE1DARRAY;
                        SRVDesc.Texture1DArray.MipLevels = desc.MipLevels;
                        SRVDesc.Texture1DArray.ArraySize = static_cast<UINT>(arraySize);
                    }
                    else
                    {
                        SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE1D;
                        SRVDesc.Texture1D.MipLevels = desc.MipLevels;
                    }

                    hr = d3dDevice->CreateShaderResourceView(tex, &SRVDesc, textureView);

                    if(FAILED(hr))
                    {
                        tex->Release();
                        return hr;
                    }
#endif

                    break;
                }
                case PXGraphicTextureType2D:
                {
                    PXTexture2D* const pxTexture2D = (PXTexture2D*)pxGraphicTexturInfo->TextureReference;
                    ID3D11Texture2D** texture2D = &(ID3D11Texture2D*)pxTexture2D->ResourceID.DirectXInterface;

                    D3D11_TEXTURE2D_DESC textureDescription;
                    textureDescription.Width = pxTexture2D->Image->Width,
                    textureDescription.Height = pxTexture2D->Image->Height,
                    textureDescription.MipLevels = 0;
                    textureDescription.ArraySize = 0;
                   // textureDescription.Format = format;
                    textureDescription.SampleDesc.Count = 1;
                    textureDescription.SampleDesc.Quality = 0;
                    textureDescription.Usage = D3D11_USAGE_DEFAULT;
                    textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                    textureDescription.CPUAccessFlags = 0;
                   // textureDescription.MiscFlags = (isCubeMap) ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;


                    D3D11_SUBRESOURCE_DATA pInitialData;
                    pInitialData.pSysMem = 0;
                    pInitialData.SysMemPitch = 0;
                    pInitialData.SysMemSlicePitch = 0;

                    const HRESULT result = pxDirectX11->Device->lpVtbl->CreateTexture2D
                    (
                        pxDirectX11->Device,
                        &textureDescription,
                        &pInitialData,
                        texture2D
                    );


#if 0
                    if(textureView != 0)
                    {
                        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
                        memset(&SRVDesc, 0, sizeof(SRVDesc));
                        SRVDesc.Format = format;

                        if(isCubeMap)
                        {
                            if(arraySize > 6)
                            {
                                SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBEARRAY;
                                SRVDesc.TextureCubeArray.MipLevels = desc.MipLevels;

                                // Earlier, we set arraySize to (NumCubes * 6).
                                SRVDesc.TextureCubeArray.NumCubes = static_cast<UINT>(arraySize / 6);
                            }
                            else
                            {
                                SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
                                SRVDesc.TextureCube.MipLevels = desc.MipLevels;
                            }
                        }
                        else if(arraySize > 1)
                        {
                            SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2DARRAY;
                            SRVDesc.Texture2DArray.MipLevels = desc.MipLevels;
                            SRVDesc.Texture2DArray.ArraySize = static_cast<UINT>(arraySize);
                        }
                        else
                        {
                            SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
                            SRVDesc.Texture2D.MipLevels = desc.MipLevels;
                        }

                        hr = d3dDevice->CreateShaderResourceView(tex, &SRVDesc, textureView);

                        if(FAILED(hr))
                        {
                            tex->Release();
                            return hr;
                        }
                    }

                    if(texture != 0)
                    {
                        *texture = tex;
                    }
                    else
                    {
                        tex->Release();
                    }
#endif



                    break;
                }
                case PXGraphicTextureType3D:
                {
                    PXTexture3D* const pxTexture3D = pxGraphicTexturInfo->TextureReference;
                    ID3D11Texture3D** dxTexture3D = &(ID3D11Texture3D*)pxTexture3D->ResourceID.DirectXInterface;

                    D3D11_TEXTURE3D_DESC textureDescription;
                    textureDescription.Width = pxTexture3D->Image->Width,
                    textureDescription.Height = pxTexture3D->Image->Height,
                    textureDescription.Depth = pxTexture3D->Image->Depth,
                    //textureDescription.MipLevels = static_cast<UINT>(mipCount);
                    //textureDescription.Format = format;
                    textureDescription.Usage = D3D11_USAGE_DEFAULT;
                    textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                    textureDescription.CPUAccessFlags = 0;
                    textureDescription.MiscFlags = 0;

                    //textureDescription.ArraySize = pxTexture3D->Image->PixelDataSize;
                    //textureDescription.Format = PXDirectXColorFormatFromID(pxTexture->Image.Format);
                    textureDescription.Usage = 0;
                    textureDescription.BindFlags = 0;
                    textureDescription.CPUAccessFlags = 0;
                    textureDescription.MiscFlags = 0;

                    D3D11_SUBRESOURCE_DATA pInitialData;
                    pInitialData.pSysMem = 0;
                    pInitialData.SysMemPitch = 0;
                    pInitialData.SysMemSlicePitch = 0;

                    const HRESULT result = pxDirectX11->Device->lpVtbl->CreateTexture3D
                    (
                        pxDirectX11->Device,
                        &textureDescription,
                        &pInitialData,
                        dxTexture3D
                    );
                    const PXBool successful = SUCCEEDED(result);

#if 0
                    if(SUCCEEDED(hr) && tex != 0)
                    {
                        if(textureView != 0)
                        {
                            D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
                            memset(&SRVDesc, 0, sizeof(SRVDesc));
                            SRVDesc.Format = format;
                            SRVDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE3D;
                            SRVDesc.Texture3D.MipLevels = desc.MipLevels;

                            hr = d3dDevice->CreateShaderResourceView(tex, &SRVDesc, textureView);

                            if(FAILED(hr))
                            {
                                tex->Release();
                                return hr;
                            }
                        }

                        if(texture != 0)
                        {
                            *texture = tex;
                        }
                        else
                        {
                            tex->Release();
                        }

#endif



                    break;
                }
                case PXGraphicTextureTypeCubeContainer:
                {
                    PXTextureCube* const pxTextureCube = pxGraphicTexturInfo->TextureReference;

                    // ???

                    break;
                }

                default:
                    break;
            }

            break;
        }
        case PXResourceActionDelete:
        {
            break;
        }
        case PXResourceActionUpdate:
        {
            break;
        }
        case PXResourceActionSelect:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX11Clear(PXDirectX11* const pxDirectX11, const PXColorRGBAF* const pxColorRGBAF)
{
    const FLOAT rgba[4] = { pxColorRGBAF->Red, pxColorRGBAF->Green, pxColorRGBAF->Blue, pxColorRGBAF->Alpha };

    pxDirectX11->Context->lpVtbl->ClearRenderTargetView
    (
        pxDirectX11->Context,
        pxDirectX11->RenderTargetView,
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
    const HRESULT bufferCreateResult = pxDirectX11->Device->lpVtbl->CreateBuffer
    (
        pxDirectX11->Device, 
        &bufferDesc,
        &InitData, 
        g_pVertexBuffer
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX11ShaderProgramCreate(PXDirectX11* const pxDirectX11, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXShader* const pxShader = &shaderList[i];
        ID3DBlob* const shaderCode = (ID3DBlob*)pxShader->Content;
        const void* const shaderBytecode = shaderCode->lpVtbl->GetBufferPointer(shaderCode);
        const SIZE_T bytecodeLength = shaderCode->lpVtbl->GetBufferSize(shaderCode);

        switch(pxShader->Type)
        {
            case PXShaderTypeVertex:
            {
                const HRESULT result = pxDirectX11->Device->lpVtbl->CreateVertexShader
                (
                    pxDirectX11->Device,
                    shaderBytecode,
                    bytecodeLength,
                    PXNull,
                    &(ID3D11VertexShader*)pxShader->ResourceID.DirectXInterface
                );

                break;
            }
            case PXShaderTypePixel:
            {
                const HRESULT result = pxDirectX11->Device->lpVtbl->CreatePixelShader
                (
                    pxDirectX11->Device,
                    shaderBytecode,
                    bytecodeLength,
                    PXNull,
                    &(ID3D11PixelShader*)pxShader->ResourceID.DirectXInterface
                );

                break;
            }
            default:
                return PXActionRefusedFormatNotSupported;
        }
    }
}

PXActionResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11* const pxDirectX, PXInt32U* const value)
{
    //*value = pxDirectX->DX11->lpVtbl->GetAvailableTextureMem(pxDirectX->DX9);

    return PXActionSuccessful;
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
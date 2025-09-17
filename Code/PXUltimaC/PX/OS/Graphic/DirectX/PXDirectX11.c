#include "PXDirectX11.h"

#if PXDX11Enable
#include <d3d11.h>
#pragma comment(lib, "D3d11.lib")
#endif

#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>

#if OSUnix

#elif OSWindows

const char PXDirectX11Name[] = "DirectX11";
const char PXDirectX11DLL[] = "D3D11.DLL";

typedef HRESULT(WINAPI* PXD3D11CreateDeviceAndSwapChain)
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
#endif

PXResult PXAPI  PXDirectX11Initialize(PXDirectX11* const pxDirectX11, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    PXD3D11CreateDeviceAndSwapChain createDeviceAndSwapChain = PXNull;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectX11Name,
        "Initialize",
        "---Start---"
    );
#endif

    // OpenLib
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDirectX11->DirectX11Library, PXDirectX11DLL);

        if(PXActionSuccessful != pxActionResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXDirectX11Name,
                "Initialize",
                "Library load failed!"
            );
#endif

            return PXActionRefusedNotSupportedByOperatingSystem;
        }


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXDirectX11Name,
            "Initialize",
            "Library linked successfuly"
        );
#endif

        // Link functions
        {
            PXLibraryGetSymbolA(&pxDirectX11->DirectX11Library, &createDeviceAndSwapChain, "D3D11CreateDeviceAndSwapChain", PXTrue);
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
            case PXDirectXDriverTypeHardwareDevice:
                dxDriverType = D3D_DRIVER_TYPE_HARDWARE;
                break;
            case PXDirectXDriverTypeReferencDevice:
                dxDriverType = D3D_DRIVER_TYPE_REFERENCE;
                break;
            case PXDirectXDriverTypeReferencDeviceWithoutRender:
                dxDriverType = D3D_DRIVER_TYPE_NULL;
                break;
            case PXDirectXDriverTypeSoftware:
                dxDriverType = D3D_DRIVER_TYPE_WARP;
                break;

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
        dxGISwapChainDescription.OutputWindow = pxGraphicInitializeInfo->WindowReference->Info.Handle.WindowHandle;
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
        const PXActionResult result = PXErrorFromHRESULT(resultID);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXDirectX11Name,
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
        PXDirectX11Name,
        "Initialize",
        "Done..."
    );
#endif
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXDirectX11Release(PXDirectX11* const pxDirectX)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXDirectX11TextureAction(PXDirectX11* const pxDirectX11, PXTexturInfo* const pxGraphicTexturInfo)
{
#if OSUnix
    return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows

    switch(pxGraphicTexturInfo->Action)
    {
        case PXResourceActionCreate:
        {
            switch(pxGraphicTexturInfo->Type)
            {
                case PXTextureType1D:
                {
                    PXTexture* pxTexture1D = (PXTexture*)pxGraphicTexturInfo->TextureReference;
                    ID3D11Texture1D** dx11Texture1D = &((ID3D11Texture1D*)pxTexture1D->Info.Handle.DirectXInterface);

                    D3D11_TEXTURE1D_DESC desc;
                    desc.Width = pxTexture1D->Width;
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

                    const HRESULT createTextureResult = pxDirectX11->Device->lpVtbl->CreateTexture1D
                    (
                        pxDirectX11->Device,
                        &desc,
                        &initialData,
                        dx11Texture1D
                    );
                    const PXBool success = SUCCEEDED(createTextureResult);

                    if(!success)
                    {
                        return PXActionFailedRegister;
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
                case PXTextureType2D:
                {
                    PXTexture* const pxTexture = (PXTexture*)pxGraphicTexturInfo->TextureReference;
                    ID3D11Texture2D** texture2D = &(ID3D11Texture2D*)pxTexture->Info.Handle.DirectXInterface;

                    D3D11_TEXTURE2D_DESC textureDescription;
                    textureDescription.Width = pxTexture->Width;
                    textureDescription.Height = pxTexture->Height;
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

                    const HRESULT resultID = pxDirectX11->Device->lpVtbl->CreateTexture2D
                    (
                        pxDirectX11->Device,
                        &textureDescription,
                        &pInitialData,
                        texture2D
                    );
                    const PXResult pxResult = PXErrorFromHRESULT(resultID);


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
                case PXTextureType3D:
                {
                    PXTexture* const pxTexture = pxGraphicTexturInfo->TextureReference;
                    ID3D11Texture3D** dxTexture3D = &(ID3D11Texture3D*)pxTexture->Info.Handle.DirectXInterface;

                    D3D11_TEXTURE3D_DESC textureDescription;
                    textureDescription.Width = pxTexture->Width;
                    textureDescription.Height = pxTexture->Height;
                    textureDescription.Depth = pxTexture->Depth;
                    //textureDescription.MipLevels = static_cast<UINT>(mipCount);
                    //textureDescription.Format = format;
                    textureDescription.Usage = D3D11_USAGE_DEFAULT;
                    textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
                    textureDescription.CPUAccessFlags = 0;
                    textureDescription.MiscFlags = 0;

                    //textureDescription.ArraySize = pxTexture->Image->PixelDataSize;
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
                case PXTextureTypeCubeContainer:
                {
                    PXTexture* const pxTexture = pxGraphicTexturInfo->TextureReference;

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

#endif

        return PXActionSuccessful;
    }

    PXResult PXAPI  PXDirectX11Clear(PXDirectX11* const pxDirectX11, const PXColorRGBAF* const pxColorRGBAF)
    {
#if OSUnix
        return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
        const PXF32 rgba[4] = { pxColorRGBAF->Red, pxColorRGBAF->Green, pxColorRGBAF->Blue, pxColorRGBAF->Alpha };

        pxDirectX11->Context->lpVtbl->ClearRenderTargetView
        (
            pxDirectX11->Context,
            pxDirectX11->RenderTargetView,
            rgba
        );
        return PXActionSuccessful;

#endif
    }

    PXResult PXAPI  PXDirectX11VertexBufferCreate(PXDirectX11* const pxDirectX11, PXVertexBuffer* const pxVertexBuffer)
    {
#if OSUnix
        return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
        ID3D11Buffer** g_pVertexBuffer = &(ID3D11Buffer*)pxVertexBuffer->Info.Handle.DirectXInterface;

        // Fill in a buffer description.
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = pxVertexBuffer->VertexData.Size;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        // Fill in the subresource data.
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = pxVertexBuffer->VertexData.Data;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;

        // Create the vertex buffer.
        const HRESULT resultID = pxDirectX11->Device->lpVtbl->CreateBuffer
        (
            pxDirectX11->Device,
            &bufferDesc,
            &InitData,
            g_pVertexBuffer
        );
        const PXResult pxResult = PXErrorFromHRESULT(resultID);

#endif

        return PXActionSuccessful;
    }

    PXResult PXAPI  PXDirectX11ShaderProgramCreate(PXDirectX11* const pxDirectX11, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount)
    {
#if OSUnix
        return PXActionRefusedNotSupportedByOperatingSystem;

#elif OSWindows
        for(PXSize i = 0; i < amount; ++i)
        {
            PXShader* const pxShader = &shaderList[i];
            ID3DBlob* const shaderCode = (ID3DBlob*)pxShader->ShaderFile->Data;
            const void* const shaderBytecode = shaderCode->lpVtbl->GetBufferPointer(shaderCode);
            const SIZE_T bytecodeLength = shaderCode->lpVtbl->GetBufferSize(shaderCode);

            switch(pxShader->Type)
            {
                case PXShaderTypeVertex:
                {
                    const HRESULT resultID = pxDirectX11->Device->lpVtbl->CreateVertexShader
                    (
                        pxDirectX11->Device,
                        shaderBytecode,
                        bytecodeLength,
                        PXNull,
                        &(ID3D11VertexShader*)pxShader->Info.Handle.DirectXInterface
                    );
                    const PXResult pxResult = PXErrorFromHRESULT(resultID);

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
                        &(ID3D11PixelShader*)pxShader->Info.Handle.DirectXInterface
                    );

                    break;
                }
                default:
                    return PXActionRefusedFormatNotSupported;
            }
        }
#endif
    }

    PXResult PXAPI  PXDirectX11TextureMemoryAvailable(PXDirectX11* const pxDirectX, PXI32U* const value)
    {
        //*value = pxDirectX->DX11->lpVtbl->GetAvailableTextureMem(pxDirectX->DX9);

        return PXActionSuccessful;
    }

    PXResult PXAPI  PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11* const pxDirectX, PXI32U* const amountOfAdapters)
    {
#if 0
        UINT amount = 1;
        D3D_FEATURE_LEVEL pFeatureLevels = 0;
        D3D_FEATURE_LEVEL pFeatureLevelResult = 0;
        D3D_DRIVER_TYPE dxDriverType = 0;

        switch(pxDirectXVersion)
        {
            case PXDirectXVersion11Emulate1x0Core:
                pFeatureLevels = D3D_FEATURE_LEVEL_1_0_CORE;
                break;
            case PXDirectXVersion11Emulate9x1:
                pFeatureLevels = D3D_FEATURE_LEVEL_9_1;
                break;
            case PXDirectXVersion11Emulate9x2:
                pFeatureLevels = D3D_FEATURE_LEVEL_9_2;
                break;
            case PXDirectXVersion11Emulate9x3:
                pFeatureLevels = D3D_FEATURE_LEVEL_9_3;
                break;
            case PXDirectXVersion11Emulate10x0:
                pFeatureLevels = D3D_FEATURE_LEVEL_10_0;
                break;
            case PXDirectXVersion11Emulate10x1:
                pFeatureLevels = D3D_FEATURE_LEVEL_10_1;
                break;
            case PXDirectXVersion11Emulate11x0:
                pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
                break;
            case PXDirectXVersion11Emulate11x1:
                pFeatureLevels = D3D_FEATURE_LEVEL_11_1;
                break;
        }

        switch(pxDirectXDriverType)
        {
            case PXDirectXDriverTypeHardwareDevice:
                dxDriverType = D3D_DRIVER_TYPE_HARDWARE;
                break;
            case PXDirectXDriverTypeReferencDevice:
                dxDriverType = D3D_DRIVER_TYPE_REFERENCE;
                break;
            case PXDirectXDriverTypeReferencDeviceWithoutRender:
                dxDriverType = D3D_DRIVER_TYPE_NULL;
                break;
            case PXDirectXDriverTypeSoftware:
                dxDriverType = D3D_DRIVER_TYPE_WARP;
                break;

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

    PXResult PXAPI  PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11* const pxDirectX, const PXI32U pxGraphicDevicePhysicalListSize, PXGPUPhysical* const pxGraphicDevicePhysicalList)
    {
        return PXActionRefusedNotImplemented;
    }

#include "PXDirectX11.h"


#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/File/PXFile.h>
#include <PX/Engine/PXEngineRender.h>
#include <PX/Type/PXTexture1D.h>
#include <PX/Type/PXTexture2D.h>
#include <PX/Type/PXTexture3D.h>
#include <PX/Type/PXTextureCube.h>


#if PXDX11Enable
#include <d3d11.h>
#pragma comment(lib, "D3d11.lib")
#endif
#include <PX/Type/PXWindow.h>

#if OSUnix

#elif OSWindows

const char PXDirectX11Name[] = "DirectX11";
const char PXDirectX11DLL[] = "D3D11.DLL";
const PXI8U PXDirectX11DLLLength = sizeof(PXDirectX11DLL);

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

PXResult PXAPI PXDirectX11Initialize(PXDirectX11 PXREF pxDirectX11, PXEngineRenderCreateInfo PXREF pxEngineRenderCreateInfo)
{
    PXResult pxResult = PXResultInvalid;

#if OSUnix
    return PXResultRefusedNotSupportedByOperatingSystem;

#elif OSWindows
    PXD3D11CreateDeviceAndSwapChain createDeviceAndSwapChain = PXNull;
    HRESULT resultID = 0;

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
        PXText pxText;
        PXTextFromAddressA(&pxText, PXDirectX11DLL, PXDirectX11DLLLength, PXDirectX11DLLLength);

        const PXResult pxResult = PXLibraryOpen(&pxDirectX11->DirectX11Library, &pxText);

        if(PXResultOK != pxResult)
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

            return PXResultRefusedNotSupportedByOperatingSystem;
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
        PXEngineRender PXREF pxEngineRender = pxEngineRenderCreateInfo->RenderSystem;
        pxEngineRender->Texture1DRegister = PXDirectX11Texture1DRegister;
        pxEngineRender->Texture2DRegister = PXDirectX11Texture2DRegister;
        pxEngineRender->Texture3DRegister = PXDirectX11Texture3DRegister;
        pxEngineRender->TextureCubeRegister = PXDirectX11TextureCubeRegister;
        pxEngineRender->ShaderVariableSet = PXNull;
        pxEngineRender->ScreenBufferRead = PXNull;
        pxEngineRender->DrawModeSet = PXNull;
        pxEngineRender->DrawColorRGBAF = PXNull;
        pxEngineRender->RectangleDraw = PXNull;
        pxEngineRender->RectangleDrawTx = PXNull;
        pxEngineRender->SwapIntervalSet = PXNull;
        pxEngineRender->SwapIntervalGet = PXNull;
        pxEngineRender->DevicePhysicalListAmount = PXNull;
        pxEngineRender->DevicePhysicalListFetch = PXNull;
        pxEngineRender->Release = PXNull;
        pxEngineRender->Select = PXNull;
        pxEngineRender->Deselect = PXNull;
        pxEngineRender->Clear = PXNull;
        pxEngineRender->SceneDeploy = PXNull;
        pxEngineRender->ViewPortGet = PXNull;
        pxEngineRender->ViewPortSet = PXNull;
        pxEngineRender->ShaderProgramCreate = PXNull;
        pxEngineRender->ShaderProgramSelect = PXNull;
        pxEngineRender->ShaderProgramDelete = PXNull;
        pxEngineRender->SceneBegin = PXNull;
        pxEngineRender->SceneEnd = PXNull;
        pxEngineRender->DrawScriptCreate = PXNull;
        pxEngineRender->DrawScriptBegin = PXNull;
        pxEngineRender->DrawScriptEnd = PXNull;
        pxEngineRender->DrawScriptDelete = PXNull;
        pxEngineRender->DrawScriptExecute = PXNull;
        pxEngineRender->ModelRegister = PXNull;
        pxEngineRender->ModelDraw = PXNull;
        pxEngineRender->ModelSelect = PXNull;
        pxEngineRender->ModelRelease = PXNull;
        pxEngineRender->LightSet = PXNull;
        pxEngineRender->LightGet = PXNull;
        pxEngineRender->LightEnableSet = PXNull;
        pxEngineRender->LightEnableGet = PXNull;
    }


    // Fetch GOU
    {

    }

    // Create Device
    {
        D3D_DRIVER_TYPE dxDriverType = D3D_DRIVER_TYPE_UNKNOWN;

        switch(pxEngineRenderCreateInfo->DirectXDriverType)
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
                return PXResultRefuedInputInvalid;
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

        PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxEngineRenderCreateInfo->WindowReference);

        DXGI_SWAP_CHAIN_DESC dxGISwapChainDescription;
        PXClear(DXGI_SWAP_CHAIN_DESC, &dxGISwapChainDescription);
        dxGISwapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
        dxGISwapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
        dxGISwapChainDescription.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        dxGISwapChainDescription.SampleDesc.Count = 1;
        dxGISwapChainDescription.SampleDesc.Quality = 0;
        dxGISwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxGISwapChainDescription.BufferCount = 1;
        dxGISwapChainDescription.OutputWindow = pxWindowHandle.Data;
        dxGISwapChainDescription.Windowed = 1u;

        resultID = createDeviceAndSwapChain // D3D11CreateDevice()
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
        pxResult = PXErrorFromHRESULT(resultID);

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
#if PXLanguageCPP
        resultID = pxDirectX11->SwapChain->GetBuffer
        (
            0,
            IID_ID3D11Texture2D,
            &pxDirectX11->FrameBuffer
        );
#else
        resultID = pxDirectX11->SwapChain->lpVtbl->GetBuffer
        (
            pxDirectX11->SwapChain,
            0,
            &IID_ID3D11Texture2D,
            &pxDirectX11->FrameBuffer
        );
#endif



#if PXLanguageCPP
        resultID = pxDirectX11->Device->CreateRenderTargetView
        (
            pxDirectX11->FrameBuffer,
            0,
            &pxDirectX11->RenderTargetView
        );
#else
        resultID = pxDirectX11->Device->lpVtbl->CreateRenderTargetView
        (
            pxDirectX11->Device,
            pxDirectX11->FrameBuffer,
            0,
            &pxDirectX11->RenderTargetView
        );
#endif
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

    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX11Release(PXDirectX11 PXREF pxDirectX)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXDirectX11Texture1DRegister(PXDirectX11 PXREF pxDirectX11, PXTexture1D PXREF pxTexture1D)
{
    PXResult pxResult = 0;
    HRESULT resultID = 0;

    D3D11_TEXTURE1D_DESC desc;
    desc.Width = pxTexture1D->Width;
    desc.MipLevels = 0;// static_cast<UINT>(mipCount);
    desc.ArraySize = 0;//static_cast<UINT>(arraySize);
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initialData;
    initialData.pSysMem = 0;
    initialData.SysMemPitch = 0;
    initialData.SysMemSlicePitch = 0;

#if PXLanguageCPP
    resultID = pxDirectX11->Device->CreateTexture1D
    (
        &desc,
        &initialData,
        &pxTexture1D->DX_11
    );
#else
    resultID = pxDirectX11->Device->lpVtbl->CreateTexture1D
    (
        pxDirectX11->Device,
        &desc,
        &initialData,
        &pxTexture1D->DX_11
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
    {
        return pxResult;
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

    return PXResultOK;
}

PXResult PXAPI PXDirectX11Texture2DRegister(PXDirectX11 PXREF pxDirectX11, PXTexture2D PXREF pxTexture2D)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = 0;


    D3D11_TEXTURE2D_DESC textureDescription;
    textureDescription.Width = pxTexture2D->Width;
    textureDescription.Height = pxTexture2D->Height;
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

#if PXLanguageCPP
    resultID = pxDirectX11->Device->CreateTexture2D
    (
        &textureDescription,
        &pInitialData,
        &pxTexture->DX_11
    );
#else
    resultID = pxDirectX11->Device->lpVtbl->CreateTexture2D
    (
        pxDirectX11->Device,
        &textureDescription,
        &pInitialData,
        &pxTexture2D->DX_11
    );
#endif
    pxResult = PXErrorFromHRESULT(resultID);


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


    return PXResultOK;
}

PXResult PXAPI PXDirectX11Texture3DRegister(PXDirectX11 PXREF pxDirectX11, PXTexture3D PXREF pxTexture3D)
{
    D3D11_TEXTURE3D_DESC textureDescription;
    textureDescription.Width = pxTexture3D->Width;
    textureDescription.Height = pxTexture3D->Height;
    textureDescription.Depth = pxTexture3D->Depth;
    //textureDescription.MipLevels = static_cast<UINT>(mipCount);
    //textureDescription.Format = format;
    textureDescription.Usage = D3D11_USAGE_DEFAULT;
    textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDescription.CPUAccessFlags = 0;
    textureDescription.MiscFlags = 0;

    //textureDescription.ArraySize = pxTexture->Image->PixelDataSize;
    //textureDescription.Format = PXDirectXColorFormatFromID(pxTexture->Image.Format);
    textureDescription.Usage = D3D11_USAGE_DEFAULT;
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
        &pxTexture3D->DX_11
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



    return PXResultOK;
}

PXResult PXAPI PXDirectX11TextureCubeRegister(PXDirectX11 PXREF pxDirectX11, PXTextureCube PXREF pxTextureCube)
{
    return PXResultOK;
}
























    PXResult PXAPI PXDirectX11Clear(PXDirectX11 PXREF pxDirectX11, const PXColorRGBA_F32 PXREF PXColorRGBA_F32)
    {
#if OSUnix
        return PXResultRefusedNotSupportedByOperatingSystem;

#elif OSWindows
        const PXF32 rgba[4] = { PXColorRGBA_F32->Red, PXColorRGBA_F32->Green, PXColorRGBA_F32->Blue, PXColorRGBA_F32->Alpha };

#if PXLanguageCPP
        pxDirectX11->Context->ClearRenderTargetView
        (
            pxDirectX11->RenderTargetView,
            rgba
        );
#else
        pxDirectX11->Context->lpVtbl->ClearRenderTargetView
        (
            pxDirectX11->Context,
            pxDirectX11->RenderTargetView,
            rgba
        );
#endif


        return PXResultOK;

#endif
    }
    PXResult PXAPI PXDirectX11VertexBufferCreate(PXDirectX11 PXREF pxDirectX11, PXVertexBuffer PXREF pxVertexBuffer)
    {
#if OSUnix
        return PXResultRefusedNotSupportedByOperatingSystem;

#elif OSWindows
        // Fill in a buffer description.
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = 0;// pxVertexBuffer->VertexData.SizeAllocated;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        // Fill in the subresource data.
        D3D11_SUBRESOURCE_DATA InitData;
        InitData.pSysMem = 0;//pxVertexBuffer->VertexData.Data;
        InitData.SysMemPitch = 0;
        InitData.SysMemSlicePitch = 0;

        // Create the vertex buffer.

#if PXLanguageCPP
        const HRESULT resultID = pxDirectX11->Device->CreateBuffer
        (
            &bufferDesc,
            &InitData,
            &pxVertexBuffer->Buffer_11
        );
#else
        const HRESULT resultID = pxDirectX11->Device->lpVtbl->CreateBuffer
        (
            pxDirectX11->Device,
            &bufferDesc,
            &InitData,
            &pxVertexBuffer->DX_11
        );
#endif


        const PXResult pxResult = PXErrorFromHRESULT(resultID);

#endif

        return PXResultOK;
    }
    PXResult PXAPI PXDirectX11ShaderProgramCreate(PXDirectX11 PXREF pxDirectX11, PXShaderProgram PXREF pxShaderProgram, PXShader PXREF shaderList, const PXSize amount)
    {

#if OSUnix
        return PXResultRefusedNotSupportedByOperatingSystem;

#elif OSWindows && 0
        PXResult pxResult = PXResultInvalid;
        HRESULT resultID = 0;

        for(PXSize i = 0; i < amount; ++i)
        {
            PXShader PXREF pxShader = &shaderList[i];
            PXBuffer PXREF pxBuffer = 0;// PXFileBufferGET(pxShader->ShaderFile);
            ID3DBlob PXREF shaderCode = (ID3DBlob*)pxBuffer->Data;

            const void PXREF shaderBytecode =
#if PXLanguageCPP
                shaderCode->GetBufferPointer();
#else
                shaderCode->lpVtbl->GetBufferPointer(shaderCode);
#endif

            const SIZE_T bytecodeLength =
#if PXLanguageCPP
                shaderCode->GetBufferSize();
#else
                shaderCode->lpVtbl->GetBufferSize(shaderCode);
#endif

            switch(0) // pxShader->Type
            {
                case PXShaderTypeVertex:
                {
#if PXLanguageCPP
                    resultID = pxDirectX11->Device->CreateVertexShader
                    (
                        shaderBytecode,
                        bytecodeLength,
                        PXNull,
                        &pxShader->VertexShader_11
                    );
#else
                    resultID = pxDirectX11->Device->lpVtbl->CreateVertexShader
                    (
                        pxDirectX11->Device,
                        shaderBytecode,
                        bytecodeLength,
                        PXNull,
                        &pxShader->VertexShader_11
                    );
#endif

                    pxResult = PXErrorFromHRESULT(pxResult);

                    break;
                }
                case PXShaderTypePixel:
                {
#if PXLanguageCPP
                    resultID = pxDirectX11->Device->CreatePixelShader
                    (
                        shaderBytecode,
                        bytecodeLength,
                        PXNull,
                        &pxShader->PixelShader_11
                    );
#else
                    resultID = pxDirectX11->Device->lpVtbl->CreatePixelShader
                    (
                        pxDirectX11->Device,
                        shaderBytecode,
                        bytecodeLength,
                        PXNull,
                        &pxShader->PixelShader_11
                    );
#endif
                    pxResult = PXErrorFromHRESULT(pxResult);

                    break;
                }
                default:
                    return PXResultRefusedFormatNotSupported;
            }
        }
#endif
    }
    PXResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11 PXREF pxDirectX, PXI32U PXREF value)
    {
        //*value = pxDirectX->DX11->lpVtbl->GetAvailableTextureMem(pxDirectX->DX9);

        return PXResultOK;
    }
    PXResult PXAPI PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11 PXREF pxDirectX, PXI32U PXREF amountOfAdapters)
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
                return PXResultRefuedInputInvalid;
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

        return PXResultRefusedNotImplemented;
    }
    PXResult PXAPI PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11 PXREF pxDirectX, const PXI32U pxGraphicDevicePhysicalListSize, PXGPU PXREF pxGPU)
    {
        return PXResultRefusedNotImplemented;
    }



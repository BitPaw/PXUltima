#include "PXDirectX.h"

#if PXDirectXUSE

#if OSUnix
#elif OSWindows
#include <windows.h>
//#include <dxdiag.h>
//#include <dsetup.h> // unsupported legacy lib, not updated
//#include <d3d12.h>

#if WindowsAtleastVista
//#include <d3d12.h>
#include <D3D9.h>
#else
#include <d3d9.h>
#endif

#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "D3d10.lib")
#pragma comment(lib, "D3d10_1.lib")
#pragma comment(lib, "D3d11.lib")
#pragma comment(lib, "D3d12.lib")
#endif

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

void PXDirectXContextConstruct(PXDirectX* const pxDirectX)
{
	PXMemoryClear(pxDirectX, sizeof(PXDirectX));
}

void PXDirectXContextDestruct(PXDirectX* const pxDirectX)
{
#if OSUnix
#elif OSWindows
    if (pxDirectX->DX9)
    {
        pxDirectX->DX9->lpVtbl->Release(pxDirectX->DX9);
    }

    if (pxDirectX->DX9Context)
    {
        pxDirectX->DX9Context->lpVtbl->Release(pxDirectX->DX9Context);
    }  
#endif
}

D3DFORMAT PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat)
{
    switch (pxColorFormat)
    {
        case PXColorFormatRGBI8: return D3DFMT_R8G8B8;
        case PXColorFormatARGBI8: return D3DFMT_A8R8G8B8;

        default:
            return D3DFMT_UNKNOWN;
    }
}

void PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial)
{
    pxMaterial->Diffuse[0] = d3dMaterial->Diffuse.r;
    pxMaterial->Diffuse[1] = d3dMaterial->Diffuse.g;
    pxMaterial->Diffuse[2] = d3dMaterial->Diffuse.b;
    pxMaterial->Diffuse[3] = d3dMaterial->Diffuse.a;

    pxMaterial->Ambient[0] = d3dMaterial->Ambient.r;
    pxMaterial->Ambient[1] = d3dMaterial->Ambient.g;
    pxMaterial->Ambient[2] = d3dMaterial->Ambient.b;
    pxMaterial->Ambient[3] = d3dMaterial->Ambient.a;

    pxMaterial->Specular[0] = d3dMaterial->Specular.r;
    pxMaterial->Specular[1] = d3dMaterial->Specular.g;
    pxMaterial->Specular[2] = d3dMaterial->Specular.b;
    pxMaterial->Specular[3] = d3dMaterial->Specular.a;

    pxMaterial->Emissive[0] = d3dMaterial->Emissive.r;
    pxMaterial->Emissive[1] = d3dMaterial->Emissive.g;
    pxMaterial->Emissive[2] = d3dMaterial->Emissive.b;
    pxMaterial->Emissive[3] = d3dMaterial->Emissive.a;

    pxMaterial->Power = d3dMaterial->Power;
}


PXInt32U PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch (pxVertexBufferFormat)
    {
        case PXVertexBufferFormatXYZ: return D3DFVF_XYZ;
        case PXVertexBufferFormatXYZUXC: return D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

        default:
            return 0;
    }
}

void PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial)
{
    d3dMaterial->Diffuse.r = pxMaterial->Diffuse[0];
    d3dMaterial->Diffuse.g = pxMaterial->Diffuse[1];
    d3dMaterial->Diffuse.b = pxMaterial->Diffuse[2];
    d3dMaterial->Diffuse.a = pxMaterial->Diffuse[3];

    d3dMaterial->Ambient.r = pxMaterial->Ambient[0];
    d3dMaterial->Ambient.g = pxMaterial->Ambient[1];
    d3dMaterial->Ambient.b = pxMaterial->Ambient[2];
    d3dMaterial->Ambient.a = pxMaterial->Ambient[3];

    d3dMaterial->Specular.r = pxMaterial->Specular[0];
    d3dMaterial->Specular.g = pxMaterial->Specular[1];
    d3dMaterial->Specular.b = pxMaterial->Specular[2];
    d3dMaterial->Specular.a = pxMaterial->Specular[3];

    d3dMaterial->Emissive.r = pxMaterial->Emissive[0];
    d3dMaterial->Emissive.g = pxMaterial->Emissive[1];
    d3dMaterial->Emissive.b = pxMaterial->Emissive[2];
    d3dMaterial->Emissive.a = pxMaterial->Emissive[3];

    d3dMaterial->Power = pxMaterial->Power;
}
















PXColorFormat PXDirectXColorFormatToID(const D3DFORMAT format)
{
	switch (format)
	{
        case D3DFMT_R8G8B8: return PXColorFormatRGBI8;
        case D3DFMT_A8R8G8B8: return PXColorFormatARGBI8;
            // case D3DFMT_X8R8G8B8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R5G6B5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X1R5G5B5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A1R5G5B5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A4R4G4B4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R3G3B2: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_A8: return PXColorFormatA8;
            // case D3DFMT_A8R3G3B2: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X4R4G4B4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2B10G10R10: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A8B8G8R8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X8B8G8R8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G16R16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2R10G10B10: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A16B16G16R16: return xxxxxxxxxxxxxxxxxx;
            // case  D3DFMT_A8P8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_P8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_L8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A8L8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A4L4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_V8U8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_L6V5U5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_X8L8V8U8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_Q8W8V8U8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_V16U16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2W10V10U10: return xxxxxxxxxxxxxxxxxx;
            //         case D3DFMT_UYVY: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R8G8_B8G8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_YUY2: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G8R8_G8B8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT1: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT2: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT3: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_DXT5: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D16_LOCKABLE: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D32: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D15S1: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24S8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24X8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24X4S4: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D32F_LOCKABLE: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_D24FS8: return xxxxxxxxxxxxxxxxxx;
#if !defined(D3D_DISABLE_9EX)
            // case D3DFMT_D32_LOCKABLE: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_S8_LOCKABLE: return xxxxxxxxxxxxxxxxxx;

#endif
            // case D3DFMT_L16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_VERTEXDATA: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_INDEX16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_INDEX32: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_Q16W16V16U16: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_MULTI2_ARGB8: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_R16F: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G16R16F: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_A16B16G16R16F: return PXColorFormatARGBF16;
            // case D3DFMT_R32F: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_G32R32F: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_A32B32G32R32F: return PXColorFormatARGBF32;

            // case D3DFMT_CxV8U8: return xxxxxxxxxxxxxxxxxx;
#if !defined(D3D_DISABLE_9EX)
            // case D3DFMT_A1: return xxxxxxxxxxxxxxxxxx;
            // case D3DFMT_A2B10G10R10_XR_BIAS: return xxxxxxxxxxxxxxxxxx;     
            // case D3DFMT_BINARYBUFFER: return xxxxxxxxxxxxxxxxxx;
#endif
        // case D3DFMT_FORCE_DWORD: return xxxxxxxxxxxxxxxxxx;
        case D3DFMT_UNKNOWN:
		default:
            return PXColorFormatInvalid;
	}
}

PXActionResult PXDirectXIndexBufferCreate(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer)
{
    //     STDMETHOD(CreateIndexBuffer)(THIS_ UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) PURE;

#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop

    D3DFORMAT dataFormat = D3DFMT_UNKNOWN;

    // fetch format
    {
        switch (pxIndexBuffer->DataType)
        {
            case PXDataTypeLEInt16S:
            case PXDataTypeLEInt16U:
            case PXDataTypeBEInt16S:
            case PXDataTypeBEInt16U:
            {
                dataFormat = D3DFMT_INDEX16;
                break;
            }
            case PXDataTypeLEInt32S:
            case PXDataTypeLEInt32U:
            case PXDataTypeBEInt32S:
            case PXDataTypeBEInt32U:
            {
                dataFormat = D3DFMT_INDEX32;
                break;
            }
            default:
            {
                dataFormat = D3DFMT_UNKNOWN;
                break;
            }
        }
    }

    const HRESULT bufferCreateResult = pxDirectX->DX9->lpVtbl->CreateIndexBuffer
    (
        pxDirectX->DX9,
        pxIndexBuffer->IndexDataSize,
        0,
        dataFormat,
        0,
        &pxIndexBuffer->DirectXInterface,
        PXNull
    );


    // Fill data



    return PXActionSuccessful;

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXRenderTargetCreate(PXDirectX* const pxDirectX, PXRenderTarget* const pxRenderTarget)
{
    //     STDMETHOD(CreateRenderTarget)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;

#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	

    return PXActionSuccessful;

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXDepthStencilSurfaceCreate(PXDirectX* const pxDirectX, PXDepthStencilSurface* const pxDepthStencilSurface)
{
    //    STDMETHOD(CreateDepthStencilSurface)(THIS_ UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) PURE;
   

#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	

    return PXActionSuccessful;

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXContextCreate(PXDirectX* const pxDirectX, const HWND pxWindowID, const PXDirectXVersion pxDirectXVersion, const PXDirectXDriverType pxDirectXDriverType)
{
	PXDirectXContextConstruct(pxDirectX);

#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	
  

    HMODULE           dxSoftware = GetModuleHandle(PXNull); // Reserved
    UINT              Flags = 0;



    
    switch (pxDirectXVersion)
    {
#if 0
        case PXDirectXVersionNewest:
        case PXDirectXVersion12Emulate1x0Core:
        case PXDirectXVersion12Emulate9x1:
        case PXDirectXVersion12Emulate9x2:
        case PXDirectXVersion12Emulate9x3:
        case PXDirectXVersion12Emulate10x0:
        case PXDirectXVersion12Emulate10x1:
        case PXDirectXVersion12Emulate11x0:
        case PXDirectXVersion12Emulate11x1:
        case PXDirectXVersion12Emulate12x0:
        case PXDirectXVersion12Emulate12x1:
        case PXDirectXVersion12Emulate12x2:
        {
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

            break;
        }
#endif       
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
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

            break;
        }
        case PXDirectXVersion10x1Simulate10x0:
        case PXDirectXVersion10x1Simulate9x1:
        case PXDirectXVersion10x1Simulate9x2:
        case PXDirectXVersion10x1Simulate9x3:
        case PXDirectXVersion10x1:
        {
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

            break;
        }
        case PXDirectXVersion10x0:
        {
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

            break;
        }   
        case PXDirectXVersion9:
        {

            // Get context
            //Direct3DCreate9Ex();

            pxDirectX->DX9Context = Direct3DCreate9(D3D_SDK_VERSION); // Create DirectX context, alternative Direct3DCreate9Ex()

            {
                const PXBool success = pxDirectX->DX9Context;

                if (!success)
                {
                    return PXActionFailedInitialization;
                }
            }
     
            const PXInt32U amountOfAdapters = pxDirectX->DX9Context->lpVtbl->GetAdapterCount(pxDirectX->DX9Context);

            for (PXInt32U i = 0; i < amountOfAdapters; ++i)
            {
                D3DADAPTER_IDENTIFIER9 adapterIdentifier;

                PXMemoryClear(&adapterIdentifier, sizeof(D3DADAPTER_IDENTIFIER9));

                pxDirectX->DX9Context->lpVtbl->GetAdapterIdentifier(pxDirectX->DX9Context, i, 0, &adapterIdentifier);

                PXTextCopyA(adapterIdentifier.Driver, MAX_DEVICE_IDENTIFIER_STRING, pxDirectX->Driver, MAX_DEVICE_IDENTIFIER_STRING);
                PXTextCopyA(adapterIdentifier.Description, MAX_DEVICE_IDENTIFIER_STRING, pxDirectX->Description, MAX_DEVICE_IDENTIFIER_STRING);
                PXTextCopyA(adapterIdentifier.DeviceName, 32, pxDirectX->DeviceName, 32);
            }

            D3DPRESENT_PARAMETERS presentParameters;
            PXObjectClear(D3DPRESENT_PARAMETERS, &presentParameters);
            presentParameters.Windowed = TRUE;
            presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
            presentParameters.SwapEffect = D3DSWAPEFFECT_COPY; // D3DSWAPEFFECT_DISCARD

            const HRESULT deviceCapaResult = pxDirectX->DX9Context->lpVtbl->GetDeviceCaps(pxDirectX->DX9Context, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &pxDirectX->DeviceCapabilitiesCurrent);

            const HRESULT createResult = pxDirectX->DX9Context->lpVtbl->CreateDevice(pxDirectX->DX9Context, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pxWindowID, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParameters, &pxDirectX->DX9);

            break;
        }

        default:
            return PXActionRefusedFormatNotSupported;
    }

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

#endif

PXActionResult PXDirectXVertexBufferCreate(PXDirectX* const pxDirectX, PXVertexBuffer* const pxVertexBuffer)
{
    //     STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) PURE;


#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	


    if (pxDirectX->DX11 != PXNull)
    {
        ID3D11Buffer** g_pVertexBuffer = &(ID3D11Buffer*)pxVertexBuffer->DirectXInterface;
     
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
        const HRESULT bufferCreateResult = pxDirectX->DX11->lpVtbl->CreateBuffer(pxDirectX->DX11, &bufferDesc, &InitData, &g_pVertexBuffer);     

        return PXActionSuccessful;
    }


    if (pxDirectX->DX9 != PXNull)
    {
        const HRESULT bufferCreateResult = pxDirectX->DX9->lpVtbl->CreateVertexBuffer
        (
            pxDirectX->DX9,
            pxVertexBuffer->VertexDataSize,
            0,
            D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
            D3DPOOL_DEFAULT,
            &pxVertexBuffer->DirectXInterface,
            PXNull
        );
        const PXBool bufferCreateSuccess = bufferCreateResult > 0;


        const PXBool hasData = pxVertexBuffer->VertexData && pxVertexBuffer->VertexDataSize;

        if (!hasData)
        {
            return PXActionInvalid;
        }

        IDirect3DVertexBuffer9* const vertexBuffer = (IDirect3DVertexBuffer9*)pxVertexBuffer->DirectXInterface;
        void* targetAdress = PXNull;

        const HRESULT lockResult = vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, pxVertexBuffer->VertexDataSize, &targetAdress, 0);

        PXMemoryCopy(pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize, targetAdress, MemorySizeUnkown);

        const HRESULT unlockResult = vertexBuffer->lpVtbl->Unlock(vertexBuffer);

        return PXActionSuccessful;
    }


    return PXActionInvalid;
   
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTexture2DCreate(PXDirectX* const pxDirectX, PXTexture2D* const pxTexture)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	
    const UINT levels = 0;
    const DWORD usage = 0; // D3DUSAGE
	const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture->Image.Format);
	const D3DPOOL pool = D3DPOOL_DEFAULT;
    HANDLE sharedHandle = PXNull;

	const HRESULT result = pxDirectX->DX9->lpVtbl->CreateTexture
	(
		pxDirectX->DX9,
		pxTexture->Image.Width,
		pxTexture->Image.Height,
		levels,
		usage,
		format,
		pool,
		&pxTexture->DirectXID,
		&sharedHandle
	);

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTexture3DCreate(PXDirectX* const pxDirectX, PXTexture3D* const pxTexture3D)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	
    const UINT levels = 0;
    const DWORD usage = 0; // D3DUSAGE
    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture3D->Format);
    const D3DPOOL pool = D3DPOOL_DEFAULT;
    HANDLE sharedHandle = PXNull;

    const HRESULT result = pxDirectX->DX9->lpVtbl->CreateVolumeTexture
    (
        pxDirectX->DX9,
        pxTexture3D->Width,
        pxTexture3D->Height,
        pxTexture3D->Depth,
        levels,
        usage,
        format,
        pool,
        &pxTexture3D->DirectXInterface,
        &sharedHandle
    );
    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTextureCubeCreate(PXDirectX* const pxDirectX, PXTextureCube* const pxTextureCube)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop	
    const PXInt32U edgeLength = 1; 
    const UINT levels = 0;
    const DWORD usage = 0; // D3DUSAGE
    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTextureCube->Format);
    const D3DPOOL pool = D3DPOOL_DEFAULT;
    HANDLE sharedHandle = PXNull;

    const HRESULT result = pxDirectX->DX9->lpVtbl->CreateCubeTexture
    (
        pxDirectX->DX9,
        edgeLength,
        levels,
        usage,
        format,
        pool,
        &pxTextureCube->DirectXInterface,
        &sharedHandle
    );

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}



PXActionResult PXDirectXMaterialSet(PXDirectX* const pxDirectX, const PXMaterial* const pxMaterial)
{
    D3DMATERIAL9 d3dMaterial;
    PXDirectXMaterialFromPXMaterial(&d3dMaterial, pxMaterial);

    const HRESULT result = pxDirectX->DX9->lpVtbl->SetMaterial
    (
        pxDirectX->DX9,
        &d3dMaterial
    );

    return PXActionSuccessful;
}

PXActionResult PXDirectXMaterialGet(PXDirectX* const pxDirectX, PXMaterial* const pxMaterial)
{
    D3DMATERIAL9 d3dMaterial;

    const HRESULT result = pxDirectX->DX9->lpVtbl->GetMaterial
    (
        pxDirectX->DX9,
        &d3dMaterial
    );

    PXDirectXMaterialToPXMaterial(pxMaterial, &d3dMaterial);

    return PXActionSuccessful;
}

PXActionResult PXDirectXViewportSet(PXDirectX* const pxDirectX, const PXViewPort* const pxViewPort)
{
    D3DVIEWPORT9 viewPort;

    // Convert
    {
        viewPort.X = pxViewPort->X;
        viewPort.Y = pxViewPort->Y;
        viewPort.Width = pxViewPort->Width;
        viewPort.Height = pxViewPort->Height;
        viewPort.MinZ = pxViewPort->ClippingMinimum;
        viewPort.MaxZ = pxViewPort->ClippingMaximum;
    }

    const HRESULT result = pxDirectX->DX9->lpVtbl->SetViewport
    (
        pxDirectX->DX9,
        &viewPort
    );

    return PXActionSuccessful;    
}

PXActionResult PXDirectXViewportGet(PXDirectX* const pxDirectX, PXViewPort* const pxViewPort)
{
    D3DVIEWPORT9 viewPort;

    const HRESULT result = pxDirectX->DX9->lpVtbl->GetViewport
    (
        pxDirectX->DX9,
        &viewPort
    );

    // Convert
    {
        pxViewPort->X = viewPort.X;
        pxViewPort->Y = viewPort.Y;
        pxViewPort->Width = viewPort.Width;
        pxViewPort->Height = viewPort.Height;
        pxViewPort->ClippingMinimum = viewPort.MinZ;
        pxViewPort->ClippingMaximum = viewPort.MaxZ;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXPrimitiveDraw(PXDirectX* const pxDirectX, const PXGraphicRenderMode pxGraphicRenderMode, const PXInt32U startVertex, const PXInt32U primitiveCount)
{
    const D3DPRIMITIVETYPE primitiveType = PXDirectXDrawTypeFromPX(pxGraphicRenderMode);

    const HRESULT result = pxDirectX->DX9->lpVtbl->DrawPrimitive
    (
        pxDirectX->DX9,
        primitiveType,
        startVertex,
        primitiveCount
    );

    return PXActionSuccessful;
}

D3DPRIMITIVETYPE PXDirectXDrawTypeFromPX(const PXGraphicRenderMode pxGraphicRenderMode)
{
    switch (pxGraphicRenderMode)
    {
        case PXGraphicRenderModePoint: return D3DPT_POINTLIST;
        case PXGraphicRenderModeLineLoop: return D3DPT_LINELIST;
        case PXGraphicRenderModeLineStrip: return D3DPT_LINESTRIP;
        case PXGraphicRenderModeTriangle: return D3DPT_TRIANGLELIST;
        case PXGraphicRenderModeTriangleStrip: return D3DPT_TRIANGLESTRIP;
        case PXGraphicRenderModeTriangleFAN: return D3DPT_TRIANGLEFAN;

        default:
            return 0; // Invalid mode does not exist!
    }
}

PXActionResult PXDirectXSceneBegin(PXDirectX* const pxDirectX)
{
    const HRESULT result = pxDirectX->DX9->lpVtbl->BeginScene(pxDirectX->DX9 );

    return PXActionSuccessful;
}

PXActionResult PXDirectXSceneEnd(PXDirectX* const pxDirectX)
{
    const HRESULT result = pxDirectX->DX9->lpVtbl->EndScene(pxDirectX->DX9 );

    return PXActionSuccessful;
}

PXActionResult PXDirectXClear(PXDirectX* const pxDirectX, const PXInt32U Count, const D3DRECT* pRects, const PXInt32U Flags, const PXInt32U colorRGBAID, const float Z, const PXInt32U Stencil)
{   
    const HRESULT result = pxDirectX->DX9->lpVtbl->Clear
    (
        pxDirectX->DX9,
        Count,
        pRects,
        Flags,
        colorRGBAID,
        Z,
        Stencil    
    );

    return PXActionSuccessful;
}

PXActionResult PXDirectXVertexFixedFunctionSet(PXDirectX* const pxDirectX, const PXVertexBufferFormat pxVertexBufferFormat)
{
    const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBufferFormat);

    const HRESULT result = pxDirectX->DX9->lpVtbl->SetFVF
    (
        pxDirectX->DX9,
        flagID
    );

    return PXActionSuccessful;
}

PXActionResult PXDirectXVertexFixedFunctionGet(PXDirectX* const pxDirectX, PXVertexBufferFormat* const pxVertexBufferFormat)
{
    DWORD flagID = 0;

    const HRESULT result = pxDirectX->DX9->lpVtbl->GetFVF
    (
        pxDirectX->DX9,
        &flagID
    );

    return PXActionSuccessful;
}

PXActionResult PXDirectXStreamSourceSet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride)
{
    const HRESULT result = pxDirectX->DX9->lpVtbl->SetStreamSource
    (
        pxDirectX->DX9,
        StreamNumber,
        pxVertexBuffer->DirectXInterface,
        OffsetInBytes,
        Stride
    );

    return PXActionSuccessful;
}

PXActionResult PXDirectXStreamSourceGet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride)
{
    return PXActionSuccessful;
}

PXActionResult PXDirectXPresent(PXDirectX* const pxDirectX, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
    const HRESULT result = pxDirectX->DX9->lpVtbl->Present
    (
        pxDirectX->DX9,
        pSourceRect,
        pSourceRect,
        pDestRect,
        hDestWindowOverride,
        pDirtyRegion
    );

    return PXActionSuccessful;
}
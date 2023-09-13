#include "PXDirectX.h"

#if OSUnix
#elif OSWindows
#include <windows.h>

#if PXDX9Enable
//#include <dxdiag.h>
//#include <dsetup.h> // unsupported legacy lib, not updated
// #include <d3d9.h> // Already included
#include <d3dcompiler.h>
#include <d3d9caps.h>
#include <d3dx9shader.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "D3d9.lib")
#pragma comment(lib, "D3dx9.lib")

#endif

#if PXDX10Enable
#include <d3d10shader.h>
#pragma comment(lib, "D3d10.lib")
#endif

#if PXDX10x1Enable
#pragma comment(lib, "D3d10_1.lib")
#endif

#if PXDX11Enable
#pragma comment(lib, "D3d11.lib")
#endif

#if PXDX12Enable
#pragma comment(lib, "D3d12.lib")
#endif

#endif

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#if OSWindows // TODO: Temp fix

void PXDirectXContextConstruct(PXDirectX* const pxDirectX)
{
	PXMemoryClear(pxDirectX, sizeof(PXDirectX));
}

void PXDirectXContextDestruct(PXDirectX* const pxDirectX)
{
#if OSUnix
#elif OSWindows
#if PXDX9Enable

    if (pxDirectX->DX9)
    {
        pxDirectX->DX9->lpVtbl->Release(pxDirectX->DX9);
    }

    if (pxDirectX->DX9Context)
    {
        pxDirectX->DX9Context->lpVtbl->Release(pxDirectX->DX9Context);
    }
#endif
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
        case PXVertexBufferFormatXYZC: return D3DFVF_XYZ | D3DFVF_DIFFUSE;
        case PXVertexBufferFormatXYZHWC: return D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

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
        &(IDirect3DIndexBuffer9*)pxIndexBuffer->ResourceID.DirectXInterface,
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

PXActionResult PXDirectXInitialize(PXDirectX* const pxDirectX, const HWND pxWindowID, const PXDirectXVersion pxDirectXVersion, const PXDirectXDriverType pxDirectXDriverType)
{
	PXDirectXContextConstruct(pxDirectX);

#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop


    HMODULE           dxSoftware = GetModuleHandle(PXNull); // Reserved
    UINT              Flags = 0;




    switch (pxDirectXVersion)
    {
#if PXDX12Enable
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

#if PXDX11Enable
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
#endif

#if PXDX10x1Enable

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
#endif

#if PXDX10Enable

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

            pxDirectX->DirectXVersion = pxDirectXDriverType;

            break;
        }
#endif

#if PXDX9Enable
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

            pxDirectX->DirectXVersion = PXDirectXVersion9;

            break;
        }
#endif

        default:
            return PXActionRefusedFormatNotSupported;
    }
#endif

    return PXActionNotSupportedByOperatingSystem;
}

PXActionResult PXDirectXRelease(PXDirectX* const pxDirectX)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXShaderVariableIDFetch(PXDirectX* const pxDirectX, const PXShader* pxShader, PXInt32U* const shaderVariableID, const char* const name)
{
#if WindowsAtleast10 // if the "D3DX9_43.DLL" is missing

    IDirect3DVertexShader9* pShader = NULL;
    ID3DXConstantTable* pConstantTable = NULL;
    DWORD* pData = NULL;

    pxDirectX->DX9->lpVtbl->GetVertexShader(pxDirectX->DX9, &pShader);


    UINT pSizeOfData;

    pShader->lpVtbl->GetFunction(pShader, NULL, &pSizeOfData);
    //findWeirdMirrorsEdgeShader(pSizeOfData);
    pData = malloc(pSizeOfData);
    pShader->lpVtbl->GetFunction(pShader, pData, &pSizeOfData);

   // bool shaderSeen = hasSeenShader(pSizeOfData);

    D3DXCONSTANT_DESC pConstantDesc[32];
    UINT pConstantNum = 32;

    const HRESULT err = D3DXGetShaderConstantTable(pData, &pConstantTable);



    D3DXCONSTANTTABLE_DESC pDesc;
    pConstantTable->lpVtbl->GetDesc(pConstantTable, &pDesc);
    for (UINT i = 0; i < pDesc.Constants; ++i)
    {
        D3DXHANDLE Handle = pConstantTable->lpVtbl->GetConstant(pConstantTable, NULL, i);
        if (Handle == NULL) continue;
        pConstantTable->lpVtbl->GetConstantDesc(pConstantTable, Handle, pConstantDesc, &pConstantNum);


        printf("\n");

#if 0
        for (UINT j = 0; j < pConstantNum; j++)
        {
            removeExistingMatrices(pConstantDesc[j]);
            parse4by4Matrices(pConstantDesc[j]);
            parseIndividualFloats(pConstantDesc[j]);
        }
#endif
    }

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupported;
#endif


    /*












    ID3DXConstantTable* pConstantTable = PXNull;

    DWORD bufferData[1024];
    D3DXCONSTANT_DESC pConstantDesc[32];
    UINT pConstantNum = 32;

    const HRESULT HRESULT = D3DXGetShaderConstantTable(bufferData, &pConstantTable);

    //if (pConstantTable == NULL) goto grexit;

    D3DXCONSTANTTABLE_DESC pDesc;


    pConstantTable->lpVtbl->GetDesc(pConstantTable, &pDesc);


    for (UINT i = 0; i < pDesc.Constants; i++)
    {
        D3DXHANDLE Handle = pConstantTable->GetConstant(NULL, i);
        if (Handle == NULL) continue;
        pConstantTable->GetConstantDesc(Handle, pConstantDesc, &pConstantNum);
        for (UINT j = 0; j < pConstantNum; j++)
        {
            removeExistingMatrices(pConstantDesc[j]);
            parse4by4Matrices(pConstantDesc[j]);
            parseIndividualFloats(pConstantDesc[j]);
        }
    }

    pxDirectX->DX10->lpVtbl.tab;


    ID3DXConstantTable id3DXConstantTable;

    id3DXConstantTable.lpVtbl->

    return NotSupported;*/
}

PXActionResult PXDirectXVertexBufferCreate(PXDirectX* const pxDirectX, PXVertexBuffer* const pxVertexBuffer)
{
    //     STDMETHOD(CreateVertexBuffer)(THIS_ UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) PURE;


#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop

    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
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
            const HRESULT bufferCreateResult = pxDirectX->DX11->lpVtbl->CreateBuffer(pxDirectX->DX11, &bufferDesc, &InitData, g_pVertexBuffer);

            return PXActionSuccessful;
        }
#endif

#if PXDX9Enable

        case PXDirectXVersion9:
        {
            const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBuffer->Format);

            const HRESULT bufferCreateResult = pxDirectX->DX9->lpVtbl->CreateVertexBuffer
            (
                pxDirectX->DX9,
                pxVertexBuffer->VertexDataSize,
                0,
                flagID,
                D3DPOOL_DEFAULT,
                &(IDirect3DVertexBuffer9*)pxVertexBuffer->ResourceID.DirectXInterface,
                PXNull
            );
            const PXBool bufferCreateSuccess = bufferCreateResult > 0;


            const PXBool hasData = pxVertexBuffer->VertexData && pxVertexBuffer->VertexDataSize;

            if (!hasData)
            {
                return PXActionInvalid;
            }

            IDirect3DVertexBuffer9* const vertexBuffer = (IDirect3DVertexBuffer9*)pxVertexBuffer->ResourceID.DirectXInterface;
            void* targetAdress = PXNull;

            const HRESULT lockResult = vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, pxVertexBuffer->VertexDataSize, &targetAdress, 0);

            PXMemoryCopy(pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize, targetAdress, MemorySizeUnkown);

            const HRESULT unlockResult = vertexBuffer->lpVtbl->Unlock(vertexBuffer);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTextureMemoryAvailable(PXDirectX* const pxDirectX, PXInt32U* const value)
{

#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop

    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            *value = pxDirectX->DX11->lpVtbl->GetAvailableTextureMem(pxDirectX->DX9);

            return PXActionSuccessful;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            *value = pxDirectX->DX9->lpVtbl->GetAvailableTextureMem(pxDirectX->DX9);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTexture2DCreate(PXDirectX* const pxDirectX, PXTexture2D* const pxTexture)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop

    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
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

            return PXActionSuccessful;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
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
                &(IDirect3DTexture9*)pxTexture->ResourceID.DirectXInterface,
                &sharedHandle
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTexture3DCreate(PXDirectX* const pxDirectX, PXTexture3D* const pxTexture3D)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop

    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
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

            const HRESULT result = pxDirectX->DX11->lpVtbl->CreateTexture3D
            (
                pxDirectX->DX11,
                &textureDescription,
                &pInitialData,
                &(ID3D11Texture3D*)pxTexture3D->ResourceID.DirectXInterface
            );

            return PXActionSuccessful;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
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
                &(IDirect3DVolumeTexture9*)pxTexture3D->ResourceID.DirectXInterface,
                &sharedHandle
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXTextureCubeCreate(PXDirectX* const pxDirectX, PXTextureCube* const pxTextureCube)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;

#elif PXOSWindowsDestop


    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionSuccessful;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {

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
                &(IDirect3DCubeTexture9*)pxTextureCube->ResourceID.DirectXInterface,
                &sharedHandle
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXDirectXVertexStructureRegister(PXDirectX* const pxDirectX, PXVertexStructure* const pxVertexStructure)
{
    PXDirectXVertexBufferCreate(pxDirectX, &pxVertexStructure->VertexBuffer);
    PXDirectXIndexBufferCreate(pxDirectX, &pxVertexStructure->IndexBuffer);

    return PXActionSuccessful;
}

PXActionResult PXDirectXMaterialSet(PXDirectX* const pxDirectX, const PXMaterial* const pxMaterial)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {


            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
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
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXMaterialGet(PXDirectX* const pxDirectX, PXMaterial* const pxMaterial)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
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
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }


    return PXActionSuccessful;
}

PXActionResult PXDirectXLightSet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index) // D3DLIGHT9
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            D3DLIGHT9 d3dLight9;
            d3dLight9.Diffuse.r = pxLight->Diffuse[0];
            d3dLight9.Diffuse.g = pxLight->Diffuse[1];
            d3dLight9.Diffuse.b = pxLight->Diffuse[2];
            d3dLight9.Diffuse.a = pxLight->Diffuse[3];
            d3dLight9.Specular.r = pxLight->Specular[0];
            d3dLight9.Specular.g = pxLight->Specular[1];
            d3dLight9.Specular.b = pxLight->Specular[2];
            d3dLight9.Specular.a = pxLight->Specular[3];
            d3dLight9.Ambient.r = pxLight->Ambient[0];
            d3dLight9.Ambient.g = pxLight->Ambient[1];
            d3dLight9.Ambient.b = pxLight->Ambient[2];
            d3dLight9.Ambient.a = pxLight->Ambient[3];
            d3dLight9.Position.x = pxLight->Position[0];
            d3dLight9.Position.y = pxLight->Position[1];
            d3dLight9.Position.z = pxLight->Position[2];
            d3dLight9.Direction.x = pxLight->Direction[0];
            d3dLight9.Direction.y = pxLight->Direction[1];
            d3dLight9.Direction.z = pxLight->Direction[2];
            d3dLight9.Range = pxLight->CutoffRange;
            d3dLight9.Falloff = pxLight->Falloff;
            d3dLight9.Attenuation0 = pxLight->AttenuationConstant;
            d3dLight9.Attenuation1 = pxLight->AttenuationLinear;
            d3dLight9.Attenuation2 = pxLight->AttenuationQuadratic;
            d3dLight9.Theta = pxLight->Theta;
            d3dLight9.Phi = pxLight->Phi;

            switch (pxLight->Type)
            {
                case PXLightTypePoint:
                    d3dLight9.Type = D3DLIGHT_POINT;
                    break;

                case PXLightTypeSpot:
                    d3dLight9.Type = D3DLIGHT_SPOT;
                    break;

                case PXLightTypeDirectional:
                    d3dLight9.Type = D3DLIGHT_DIRECTIONAL;
                    break;

                default:
                    d3dLight9.Type = 0;
                    break;
            }

            const HRESULT result = pxDirectX->DX9->lpVtbl->SetLight(pxDirectX->DX9, index, &d3dLight9);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXLightGet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            D3DLIGHT9 d3dLight9;

            const HRESULT result = pxDirectX->DX9->lpVtbl->GetLight(pxDirectX->DX9, index, &d3dLight9);

            pxLight->Diffuse[0] = d3dLight9.Diffuse.r;
            pxLight->Diffuse[1] = d3dLight9.Diffuse.g;
            pxLight->Diffuse[2] = d3dLight9.Diffuse.b;
            pxLight->Diffuse[3] = d3dLight9.Diffuse.a;
            pxLight->Specular[0] = d3dLight9.Specular.r;
            pxLight->Specular[1] = d3dLight9.Specular.g;
            pxLight->Specular[2] = d3dLight9.Specular.b;
            pxLight->Specular[3] = d3dLight9.Specular.a;
            pxLight->Ambient[0] = d3dLight9.Ambient.r;
            pxLight->Ambient[1] = d3dLight9.Ambient.g;
            pxLight->Ambient[2] = d3dLight9.Ambient.b;
            pxLight->Ambient[3] = d3dLight9.Ambient.a;
            pxLight->Position[0] = d3dLight9.Position.x;
            pxLight->Position[1] = d3dLight9.Position.y;
            pxLight->Position[2] = d3dLight9.Position.z;
            pxLight->Direction[0] = d3dLight9.Direction.x;
            pxLight->Direction[1] = d3dLight9.Direction.y;
            pxLight->Direction[2] = d3dLight9.Direction.z;
            pxLight->CutoffRange = d3dLight9.Range;
            pxLight->Falloff = d3dLight9.Falloff;
            pxLight->AttenuationConstant = d3dLight9.Attenuation0;
            pxLight->AttenuationLinear = d3dLight9.Attenuation1;
            pxLight->AttenuationQuadratic = d3dLight9.Attenuation2;
            pxLight->Theta = d3dLight9.Theta;
            pxLight->Phi = d3dLight9.Phi;

            switch (d3dLight9.Type)
            {
                case D3DLIGHT_POINT:
                    pxLight->Type = PXLightTypePoint;
                    break;

                case D3DLIGHT_SPOT:
                    pxLight->Type = PXLightTypeSpot;
                    break;

                case D3DLIGHT_DIRECTIONAL:
                    pxLight->Type = PXLightTypeDirectional;
                    break;

                default:
                    pxLight->Type = PXLightTypeInvalid;
                    break;
            }

            return PXActionSuccessful;
        }
#endif


        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXDirectXLightEnableSet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index, const PXBool enable)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->LightEnable(pxDirectX->DX9, index, enable);

            pxLight->Enabled = enable;

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXDirectXLightEnableGet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index, PXBool* const enable)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            BOOL enableBool = 0;

            const HRESULT result = pxDirectX->DX9->lpVtbl->GetLightEnable(pxDirectX->DX9, index, &enableBool);

            pxLight->Enabled = enableBool;
            *enable = enableBool;

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXDirectXViewportSet(PXDirectX* const pxDirectX, const PXViewPort* const pxViewPort)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
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
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }


    return PXActionSuccessful;
}

PXActionResult PXDirectXViewportGet(PXDirectX* const pxDirectX, PXViewPort* const pxViewPort)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {

           // pxDirectX->DX11->lpVtbl->CreateRenderTargetView


            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
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
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXPrimitiveDraw(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U startVertex, const PXInt32U primitiveCount)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {


            return PXActionSuccessful;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {

            const D3DPRIMITIVETYPE primitiveType = PXDirectXDrawTypeFromPX(PXGraphicDrawMode);

            const HRESULT result = pxDirectX->DX9->lpVtbl->DrawPrimitive
            (
                pxDirectX->DX9,
                primitiveType,
                startVertex,
                primitiveCount
            );


            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

D3DPRIMITIVETYPE PXDirectXDrawTypeFromPX(const PXGraphicDrawMode PXGraphicDrawMode)
{
    switch (PXGraphicDrawMode)
    {
        case PXGraphicDrawModePoint: return D3DPT_POINTLIST;
        case PXGraphicDrawModeLineLoop: return D3DPT_LINELIST;
        case PXGraphicDrawModeLineStrip: return D3DPT_LINESTRIP;
        case PXGraphicDrawModeTriangle: return D3DPT_TRIANGLELIST;
        case PXGraphicDrawModeTriangleStrip: return D3DPT_TRIANGLESTRIP;
        case PXGraphicDrawModeTriangleFAN: return D3DPT_TRIANGLEFAN;

        default:
            return 0; // Invalid mode does not exist!
    }
}

PXActionResult PXDirectXSceneBegin(PXDirectX* const pxDirectX)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            //pxDirectX->DX11Context->lpVtbl->Begin();

            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->BeginScene(pxDirectX->DX9);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }



    return PXActionSuccessful;
}

PXActionResult PXDirectXSceneEnd(PXDirectX* const pxDirectX)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {


            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->EndScene(pxDirectX->DX9);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }



    return PXActionSuccessful;
}

PXActionResult PXDirectXClear(PXDirectX* const pxDirectX, const PXColorRGBAF* const pxColorRGBAF)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            const FLOAT rgba[4] = { pxColorRGBAF->Red, pxColorRGBAF->Green, pxColorRGBAF->Blue, pxColorRGBAF->Alpha};

            pxDirectX->DX11Context->lpVtbl->ClearRenderTargetView
            (
                pxDirectX->DX11Context,
                PXNull,
                rgba
            );

            return PXActionNotSupportedByLibrary;
        }
#endif
#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const UINT colorI8 = D3DCOLOR_ARGB
            (
                (PXInt8U)(pxColorRGBAF->Alpha * 0xFF),
                (PXInt8U)(pxColorRGBAF->Red * 0xFF),
                (PXInt8U)(pxColorRGBAF->Green * 0xFF),
                (PXInt8U)(pxColorRGBAF->Blue * 0xFF)
            );

            const HRESULT result = pxDirectX->DX9->lpVtbl->Clear
            (
                pxDirectX->DX9,
                0,
                0,
                D3DCLEAR_TARGET,
                colorI8,
                1.0f,
                0
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }


    return PXActionSuccessful;
}

PXActionResult PXDirectXVertexFixedFunctionSet(PXDirectX* const pxDirectX, const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {


            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable

        case PXDirectXVersion9:
        {
            const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBufferFormat);

            const HRESULT result = pxDirectX->DX9->lpVtbl->SetFVF
            (
                pxDirectX->DX9,
                flagID
                );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }



    return PXActionSuccessful;
}

PXActionResult PXDirectXVertexFixedFunctionGet(PXDirectX* const pxDirectX, PXVertexBufferFormat* const pxVertexBufferFormat)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {


            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            DWORD flagID = 0;

            const HRESULT result = pxDirectX->DX9->lpVtbl->GetFVF
            (
                pxDirectX->DX9,
                &flagID
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }



    return PXActionSuccessful;
}

PXActionResult PXDirectXStreamSourceSet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {


            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->SetStreamSource
            (
                pxDirectX->DX9,
                StreamNumber,
                pxVertexBuffer->ResourceID.DirectXInterface,
                OffsetInBytes,
                Stride
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXStreamSourceGet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride)
{
    return PXActionSuccessful;
}

PXActionResult PXDirectXDrawScriptCreate(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            D3DSTATEBLOCKTYPE stateBlcokType = 0;

            switch (pxDrawScriptType)
            {
                case PXDrawScriptTypeAll: stateBlcokType = D3DSBT_ALL; break;
                case PXDrawScriptTypePixelState: stateBlcokType = D3DSBT_PIXELSTATE; break;
                case PXDrawScriptTypeVertexState: stateBlcokType = D3DSBT_VERTEXSTATE; break;

                case PXDrawScriptTypeInvalid:
                default:
                    return PXActionRefusedFormatNotSupported;
            }

            const HRESULT result = pxDirectX->DX9->lpVtbl->CreateStateBlock
            (
                pxDirectX->DX9,
                stateBlcokType,
                &(IDirect3DStateBlock9*)pxDrawScript->ResourceID.DirectXInterface
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}


PXActionResult PXDirectXDrawScriptBegin(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->BeginStateBlock(pxDirectX->DX9);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXDrawScriptEnd(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->EndStateBlock
            (
                pxDirectX->DX9,
                &(IDirect3DStateBlock9*)pxDrawScript->ResourceID.DirectXInterface
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXDrawScriptDelete(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            IDirect3DStateBlock9* const direct3DStateBlock9 = (IDirect3DStateBlock9*)pxDrawScript->ResourceID.DirectXInterface;

            const ULONG result = direct3DStateBlock9->lpVtbl->Release(direct3DStateBlock9);

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionNotSupportedByLibrary;
}

PXActionResult PXDirectXDrawScriptExecute(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXReset(PXDirectX* const pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXSceneDeploy(PXDirectX* const pxDirectX)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionNotSupportedByLibrary;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            const HRESULT result = pxDirectX->DX9->lpVtbl->Present
            (
                pxDirectX->DX9,
                0,
                0,
                0,
                0
            );

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXDirectXPrimitiveIndexedDraw(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U BaseVertexIndex, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U startIndex, const PXInt32U primCount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXShaderProgramCreateVP(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const PXText* const vertexShader, const PXText* const pixelShader)
{
    PXDirectXShaderCompile(pxDirectX, &pxShaderProgram->VertexShader, vertexShader);
    PXDirectXShaderCreate(pxDirectX, &pxShaderProgram->VertexShader);

    PXDirectXShaderCompile(pxDirectX, &pxShaderProgram->PixelShader, pixelShader);
    PXDirectXShaderCreate(pxDirectX, &pxShaderProgram->PixelShader);

    return PXActionSuccessful;
}

PXActionResult PXDirectXShaderProgramCreateVPA(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShader, const char* const pixelShader)
{
    PXText pxTextVertexShaderW;
    PXText pxTextPixelShaderW;

    PXTextConstructNamedBufferW(&pxTextVertexShaderW, pxTextVertexShaderBuffer, 260);
    PXTextConstructNamedBufferW(&pxTextPixelShaderW, pxTextPixelShaderBuffer, 260);

    PXTextCopyAW(vertexShader, PXTextUnkownLength, pxTextVertexShaderW.TextW, pxTextPixelShaderW.SizeAllocated);
    PXTextCopyAW(pixelShader, PXTextUnkownLength, pxTextPixelShaderW.TextW, pxTextPixelShaderW.SizeAllocated);

    return PXDirectXShaderProgramCreateVP(pxDirectX, pxShaderProgram, &pxTextVertexShaderW, &pxTextPixelShaderW);
}

PXActionResult PXDirectXShaderProgramCreateVPW(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const wchar_t* const vertexShader, const wchar_t* const pixelShader)
{
    PXText pxTextVertexShaderW;
    PXText pxTextPixelShaderW;

    PXTextConstructFromAdressW(&pxTextVertexShaderW, vertexShader, PXTextUnkownLength);
    PXTextConstructFromAdressW(&pxTextPixelShaderW, pixelShader, PXTextUnkownLength);

    return PXDirectXShaderProgramCreateVP(pxDirectX, pxShaderProgram, &pxTextVertexShaderW, &pxTextPixelShaderW);
}

PXActionResult PXDirectXShaderProgramCreate(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXShaderProgramSelect(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXShaderProgramDelete(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXShaderCreate(PXDirectX* const pxDirectX, PXShader* const pxShader)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            ID3DBlob* const shaderCode = (ID3DBlob*)pxShader->Content;
            const void* const shaderBytecode = shaderCode->lpVtbl->GetBufferPointer(shaderCode);
            const SIZE_T bytecodeLength = shaderCode->lpVtbl->GetBufferSize(shaderCode);

            switch (pxShader->Type)
            {
                case PXShaderTypeVertex:
                {
                    const HRESULT result = pxDirectX->DX11->lpVtbl->CreateVertexShader
                    (
                        pxDirectX->DX11,
                        shaderBytecode,
                        bytecodeLength,
                        PXNull,
                        &(ID3D11VertexShader*)pxShader->ResourceID.DirectXInterface
                    );

                    break;
                }
                case PXShaderTypeFragment:
                {
                    const HRESULT result = pxDirectX->DX11->lpVtbl->CreatePixelShader
                    (
                        pxDirectX->DX11,
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
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            switch (pxShader->Type)
            {
                case PXShaderTypeVertex:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->CreateVertexShader
                    (
                        pxDirectX->DX9,
                        0,
                        &(IDirect3DVertexShader9*)pxShader->ResourceID.DirectXInterface
                    );

                    break;
                }
                case PXShaderTypeFragment:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->CreatePixelShader
                    (
                        pxDirectX->DX9,
                        0,
                        &(IDirect3DPixelShader9*)pxShader->ResourceID.DirectXInterface
                    );

                    break;
                }
                default:
                    return PXActionRefusedFormatNotSupported;
            }

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXDirectXShaderSelect(PXDirectX* const pxDirectX, PXShader* const pxShader)
{
    switch (pxDirectX->DirectXVersion)
    {
#if PXDX11Enable
        case PXDirectXVersion11Emulate1x0Core:
        case PXDirectXVersion11Emulate9x1:
        case PXDirectXVersion11Emulate9x2:
        case PXDirectXVersion11Emulate9x3:
        case PXDirectXVersion11Emulate10x0:
        case PXDirectXVersion11Emulate10x1:
        case PXDirectXVersion11Emulate11x0:
        case PXDirectXVersion11Emulate11x1:
        {
            return PXActionRefusedFormatNotSupported;
        }
#endif

#if PXDX9Enable
        case PXDirectXVersion9:
        {
            switch (pxShader->Type)
            {
                case PXShaderTypeVertex:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->SetVertexShader
                    (
                        pxDirectX->DX9,
                        (IDirect3DVertexShader9*)pxShader->ResourceID.DirectXInterface
                    );

                    break;
                }
                case PXShaderTypeFragment:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->SetPixelShader
                    (
                        pxDirectX->DX9,
                        (IDirect3DPixelShader9*)pxShader->ResourceID.DirectXInterface
                    );

                    break;
                }
                default:
                    return PXActionRefusedFormatNotSupported;
            }

            return PXActionSuccessful;
        }
#endif

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXDirectXShaderCompile(PXDirectX* const pxDirectX, PXShader* const pxShader, const PXText* const shaderFilePath)
{
#if 0

    ID3DBlob* ps_blob_ptr = NULL;
    ID3DBlob* error_blob = NULL;
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG; // add more debug output
#endif

    // COMPILE VERTEX SHADER
    const HRESULT result = D3DCompileFromFile // D3DCompiler_47.dll, d3dcompiler.h
    (
        shaderFilePath->TextW,
        PXNull,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "vs_main",
        "vs_5_0",
        flags,
        0,
        &(ID3DBlob*)pxShader->Content,
        &error_blob
    );
    const PXBool failed = FAILED(result);

    if (!failed)
    {
        pxShader->ContentSize = ((ID3DBlob*)pxShader->Content)->lpVtbl->GetBufferSize((ID3DBlob*)pxShader->Content);

        return PXActionSuccessful;
    }


    if (error_blob)
    {
        OutputDebugStringA((char*)error_blob->lpVtbl->GetBufferPointer(error_blob));
        error_blob->lpVtbl->Release(error_blob);
    }
    /*
    if (shaderCode)
    {
        shaderCode->Release();
    }*/

#endif

    return PXActionCompilingError;
}

PXActionResult PXDirectXTransformSet(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXDirectXVertexStructureDraw(PXDirectX* const pxDirectX, PXVertexStructure* const pxVertexStructure, const PXCamera* const pxCamera)
{
    PXDirectXStreamSourceSet(pxDirectX, 0, &pxVertexStructure->VertexBuffer, 0, pxVertexStructure->VertexBuffer.VertexDataRowSize);
    PXDirectXVertexFixedFunctionSet(pxDirectX, pxVertexStructure->VertexBuffer.Format);
    PXDirectXPrimitiveDraw(pxDirectX, PXGraphicDrawModeTriangle, 0, 1);

    return PXActionSuccessful;
}
#endif
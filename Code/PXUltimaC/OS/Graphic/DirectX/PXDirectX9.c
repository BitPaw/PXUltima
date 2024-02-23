#include "PXDirectX9.h"

#include <OS/Console/PXConsole.h>

#if PXDX9Enable
#include <d3d9.h>
#include <d3d9types.h>
#endif

#if PXDX9Enable
//#include <dxdiag.h>
//#include <dsetup.h> // unsupported legacy lib, not updated
// #include <d3d9.h> // Already included

#include <d3dcompiler.h>
#include <d3d9caps.h>
//#include <d3dx9shader.h>

#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "D3d9.lib")
//#pragma comment(lib, "D3dx9.lib")

#endif

#include <OS/Graphic/PXGraphic.h>
#include <OS/Window/PXWindow.h>

typedef IDirect3D9* (WINAPI* PXDirect3DCreate9)(UINT SDKVersion);



#if OSWindows
PXColorFormat PXAPI PXDirectXColorFormatToID(const D3DFORMAT format);
D3DFORMAT PXAPI PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat);
void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial);
void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial);
D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXGraphicDrawMode PXGraphicDrawMode);
PXInt32U PXAPI PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat);
#endif


PXActionResult PXAPI PXDirectX9Initialize(PXDirectX9* const pxDirectX9, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
    // Open directX9 lib
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDirectX9->LibraryDirect3D, "D3D9.DLL");

        if (PXActionSuccessful != pxActionResult)
        {
            return PXActionRefusedNotSupported;
        }
    }

    // Get init function and execute
    {
        PXDirect3DCreate9 pxDirect3DCreate;

        const PXActionResult pxActionResult = PXLibraryGetSymbolA(&pxDirectX9->LibraryDirect3D, &pxDirect3DCreate, "Direct3DCreate9");

        if (PXActionSuccessful != pxActionResult)
        {
            PXLibraryClose(&pxDirectX9->LibraryDirect3D);
            return PXActionRefusedNotSupported;
        }

        pxDirectX9->Context = pxDirect3DCreate(D3D_SDK_VERSION); // Create DirectX context, alternative Direct3DCreate9Ex()  
        const PXBool success = pxDirectX9->Context;

        if (!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "DirectX9",
                "Initialize",
                "Instance creation failed",
                pxDirectX9->Context
            );
#endif

            return PXActionFailedInitialization;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "DirectX9",
            "Initialize",
            "Instance Created 0x%p",
            pxDirectX9->Context
        );
#endif
    }

    // Merge all functions
    {
        PXGraphic* pxGraphic = pxGraphicInitializeInfo->Graphic;
        pxGraphic->TextureAction = PXDirectX9TextureAction;
        pxGraphic->ShaderVariableIDFetch = PXDirectX9ShaderVariableIDFetch;
        pxGraphic->ShaderVariableSet = PXDirectX9ShaderVariableSetFunction;        
        pxGraphic->ScreenBufferRead = PXNull;
        pxGraphic->ShaderVariableIDFetch = PXNull;
        pxGraphic->DrawModeSet = PXNull;
        pxGraphic->DrawColorRGBAF = PXNull;
        pxGraphic->RectangleDraw = PXNull;
        pxGraphic->RectangleDrawTx = PXNull;
        pxGraphic->SwapIntervalSet = PXDirectX9SwapIntervalSet;
        pxGraphic->SwapIntervalGet = PXDirectX9SwapIntervalGet;
        pxGraphic->DevicePhysicalListAmount = PXNull;
        pxGraphic->DevicePhysicalListFetch = PXNull;
        pxGraphic->Release = PXNull;
        pxGraphic->Select = PXDirectX9Select;
        pxGraphic->Deselect = PXDirectX9Deselect;
        pxGraphic->Clear = PXDirectX9Clear;
        pxGraphic->SceneDeploy = PXDirectX9SceneDeploy;
        pxGraphic->ViewPortGet = PXDirectX9ViewportSet;
        pxGraphic->ViewPortSet = PXDirectX9ViewportGet;
        pxGraphic->ShaderProgramCreate = PXDirectX9ShaderProgramCreate;
        pxGraphic->ShaderProgramSelect = PXDirectX9ShaderProgramSelect;
        pxGraphic->ShaderProgramDelete = PXDirectX9ShaderProgramDelete;
        pxGraphic->SceneBegin = PXDirectX9SceneBegin;
        pxGraphic->SceneEnd = PXDirectX9SceneEnd;
        pxGraphic->DrawScriptCreate = PXDirectX9DrawScriptCreate;
        pxGraphic->DrawScriptBegin = PXDirectX9DrawScriptBegin;
        pxGraphic->DrawScriptEnd = PXDirectX9DrawScriptEnd;
        pxGraphic->DrawScriptDelete = PXDirectX9DrawScriptDelete;
        pxGraphic->DrawScriptExecute = PXDirectX9DrawScriptExecute;
        pxGraphic->ModelRegister = PXDirectX9ModelRegister;
        pxGraphic->ModelDraw = PXDirectX9ModelDraw;
        pxGraphic->ModelSelect = PXNull;
        pxGraphic->ModelRelease = PXNull;
        pxGraphic->LightSet = PXDirectX9LightSet;
        pxGraphic->LightGet = PXDirectX9LightGet;
        pxGraphic->LightEnableSet = PXDirectX9LightEnableSet;
        pxGraphic->LightEnableGet = PXDirectX9LightEnableGet;
    }

    //PXDirectX9DevicePhysicalListFetchFunction(pxDirectX9, pxGraphicInitializeInfo->Graphic->DevicePhysicalListSize, pxGraphicInitializeInfo->Graphic->DevicePhysicalList);

    D3DPRESENT_PARAMETERS presentParameters;
    PXClear(D3DPRESENT_PARAMETERS, &presentParameters);
    presentParameters.Windowed = TRUE;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.SwapEffect = D3DSWAPEFFECT_COPY; // D3DSWAPEFFECT_DISCARD

    const HRESULT deviceCapaResult = pxDirectX9->Context->lpVtbl->GetDeviceCaps(pxDirectX9->Context, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &pxDirectX9->DeviceCapabilitiesCurrent);

    const HRESULT createResult = pxDirectX9->Context->lpVtbl->CreateDevice
    (
        pxDirectX9->Context,
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        pxGraphicInitializeInfo->WindowReference->ID,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &presentParameters,
        &pxDirectX9->Device
    );
    const PXBool success = createResult == 0;

    if(!success)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "DirectX9",
            "Initialize",
            "Device creation failed 0x%p",
            pxDirectX9->Device
        );
#endif

        return PXActionFailedInitialization;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX9",
        "Initialize",
        "Device created 0x%p",
        pxDirectX9->Device
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9Release(PXDirectX9* const pxDirectX9)
{
    if (pxDirectX9->Device)
    {
        pxDirectX9->Device->lpVtbl->Release(pxDirectX9->Device);
    }

    if (pxDirectX9->Context)
    {
        pxDirectX9->Context->lpVtbl->Release(pxDirectX9->Context);
    }
}

void PXAPI PXDirectX9Select(PXDirectX9* const pxDirectX9)
{
  
}

void PXAPI PXDirectX9Deselect(PXDirectX9* const pxDirectX9)
{
 
}

PXActionResult PXAPI PXDirectX9TextureAction(PXDirectX9* const pxDirectX9, struct PXGraphicTexturInfo_* const pxGraphicTexturInfo)
{
    switch(pxGraphicTexturInfo->Action)
    {
        case PXResourceActionCreate:
        {
            const UINT levels = 0;
            const DWORD usage = 0; // D3DUSAGE
            const D3DPOOL pool = D3DPOOL_DEFAULT;
            HANDLE sharedHandle = PXNull;

            switch(pxGraphicTexturInfo->Type)
            {
                case PXGraphicTextureType2D:
                {
                    PXTexture2D* const pxTexture2D = (PXTexture2D*)pxGraphicTexturInfo->TextureReference;
                    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture2D->Image->Format);

                    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateTexture
                    (
                        pxDirectX9->Device,
                        pxTexture2D->Image->Width,
                        pxTexture2D->Image->Height,
                        levels,
                        usage,
                        format,
                        pool,
                        &(IDirect3DTexture9*)pxTexture2D->ResourceID.DirectXInterface,
                        &sharedHandle
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "Texture2D",
                        "Created (%ix%i) 0x%p",
                        pxTexture2D->Image->Width,
                        pxTexture2D->Image->Height,
                        pxTexture2D->ResourceID.DirectXInterface
                    );
#endif

                    break;
                }

                case PXGraphicTextureType3D: 
                {
                    PXTexture3D* const pxTexture3D = (PXTexture3D*)pxGraphicTexturInfo->TextureReference;
                    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTexture3D->Image->Format);

                    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateVolumeTexture
                    (
                        pxDirectX9->Device,
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

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "Texture3D created (%ix%ix%i) 0x%p",
                        pxTexture3D->Width,
                        pxTexture3D->Height,
                        pxTexture3D->Depth,
                        pxTexture3D->ResourceID.DirectXInterface
                    );
#endif

                    break;
                }

                case PXGraphicTextureTypeCubeContainer:
                {
                    PXTextureCube* const pxTextureCube = (PXTextureCube*)pxGraphicTexturInfo->TextureReference;
                    const D3DFORMAT format = PXDirectXColorFormatFromID(pxTextureCube->Format);
                    const UINT edgeLength = 0;

                    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateCubeTexture
                    (
                        pxDirectX9->Device,
                        edgeLength,
                        levels,
                        usage,
                        format,
                        pool,
                        &(IDirect3DCubeTexture9*)pxTextureCube->ResourceID.DirectXInterface,
                        &sharedHandle
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "TextureCube created 0x%p",
                        pxTextureCube->ResourceID.DirectXInterface
                    );
#endif

                    break;

                }

                default:
                    return PXActionNotSupportedByLibrary;
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
        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreate(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramSelect(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderVariableIDFetch(PXDirectX9* const pxDirectX9, const PXShaderProgram* const pxShaderProgram, PXInt32U* const shaderVariableID, const char* const name)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderVariableSetFunction(PXDirectX9* const pxDirectX9, struct PXGraphicShaderVariable_* const pxGraphicShaderVariable)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9SceneBegin(PXDirectX9* const pxDirectX9)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->BeginScene(pxDirectX9->Device);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9SceneEnd(PXDirectX9* const pxDirectX9)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->EndScene(pxDirectX9->Device);
}

PXActionResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType)
{
    D3DSTATEBLOCKTYPE stateBlcokType = D3DSBT_FORCE_DWORD;

    switch (pxDrawScriptType)
    {
    case PXDrawScriptTypeAll: stateBlcokType = D3DSBT_ALL; break;
    case PXDrawScriptTypePixelState: stateBlcokType = D3DSBT_PIXELSTATE; break;
    case PXDrawScriptTypeVertexState: stateBlcokType = D3DSBT_VERTEXSTATE; break;

    case PXDrawScriptTypeInvalid:
    default:
        return PXActionRefusedFormatNotSupported;
    }

    const HRESULT result = pxDirectX9->Device->lpVtbl->CreateStateBlock
    (
        pxDirectX9->Device,
        stateBlcokType,
        &(IDirect3DStateBlock9*)pxDrawScript->ResourceID.DirectXInterface
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->BeginStateBlock(pxDirectX9->Device);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->EndStateBlock
    (
        pxDirectX9->Device,
        &(IDirect3DStateBlock9*)pxDrawScript->ResourceID.DirectXInterface
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
    IDirect3DStateBlock9* const direct3DStateBlock9 = (IDirect3DStateBlock9*)pxDrawScript->ResourceID.DirectXInterface;

    const ULONG result = direct3DStateBlock9->lpVtbl->Release(direct3DStateBlock9);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ViewportSet(PXDirectX9* const pxDirectX9, const PXViewPort* const pxViewPort)
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

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetViewport
    (
        pxDirectX9->Device,
        &viewPort
    );
    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9ViewportGet(PXDirectX9* const pxDirectX9, PXViewPort* const pxViewPort)
{
    D3DVIEWPORT9 viewPort;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetViewport
    (
        pxDirectX9->Device,
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

PXActionResult PXAPI PXDirectX9MaterialSet(PXDirectX9* const pxDirectX9, const PXMaterial* const pxMaterial)
{
    D3DMATERIAL9 d3dMaterial;
    PXDirectXMaterialFromPXMaterial(&d3dMaterial, pxMaterial);

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetMaterial
    (
        pxDirectX9->Device,
        &d3dMaterial
    );
}

PXActionResult PXAPI PXDirectX9MaterialGet(PXDirectX9* const pxDirectX9, PXMaterial* const pxMaterial)
{
    D3DMATERIAL9 d3dMaterial;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetMaterial
    (
        pxDirectX9->Device,
        &d3dMaterial
    );

    PXDirectXMaterialToPXMaterial(pxMaterial, &d3dMaterial);
}

PXActionResult PXAPI PXDirectX9SwapIntervalSet(PXDirectX9* const pxDirectX9, const PXInt32U interval)
{
#if 0
    IDXGISwapChain x;

    // Get default swap chain
    IDirect3DSwapChain9* direct3DSwapChain = PXNull;

    const HRESULT resultA = pxDirectX->DX9->lpVtbl->GetSwapChain // DXGI.lib, dxgi.h
    (
        pxDirectX->DX9,
        0,
        &direct3DSwapChain
    );

    direct3DSwapChain->lpVtbl->Present
    (
        direct3DSwapChain,
        0,
        0,
        0,
        0,

        );


    if (bVysncEnabled)
    {
        presentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        presentParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    }
    else
    {
        presentParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        presentParams.FullScreen_RefreshRateInHz = 0;
    }


    const HRESULT resultB = pxDirectX->DX9->lpVtbl->Present // DXGI.lib, dxgi.h
    (
        pxDirectX->DX9,
        interval,

        );
#endif
}

PXActionResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9* const pxDirectX9, PXInt32U* const interval)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9* const pxDirectX9, PXInt32U* const value)
{
    *value = pxDirectX9->Device->lpVtbl->GetAvailableTextureMem(pxDirectX9->Device);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9* const pxDirectX9, PXInt32U* const amountOfAdapters)
{
    *amountOfAdapters = pxDirectX9->Context->lpVtbl->GetAdapterCount(pxDirectX9->Context);

    return PXActionSuccessful;;
}

PXActionResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9* const pxDirectX9, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList)
{
    PXInt32U amountOfDXDevices = 0;

    PXDirectX9DevicePhysicalListAmountFunction(pxDirectX9, &amountOfDXDevices);

    for (PXInt32U i = 0; i < amountOfDXDevices; i++)
    {
        D3DADAPTER_IDENTIFIER9 adapterIdentifier;

        PXClear(D3DADAPTER_IDENTIFIER9, &adapterIdentifier);

        pxDirectX9->Context->lpVtbl->GetAdapterIdentifier(pxDirectX9->Context, i, 0, &adapterIdentifier);

        for (PXInt32U deviceIndex = 0; deviceIndex < pxGraphicDevicePhysicalListSize; deviceIndex++)
        {
            PXGraphicDevicePhysical* pxGraphicDevicePhysicalCurrent = &pxGraphicDevicePhysicalList[i];

            PXBool found = PXTextCompareA(adapterIdentifier.DeviceName, 32, pxGraphicDevicePhysicalCurrent->DeviceDisplay, PXDeviceDisplaySize);

            if (found)
            {
                PXTextCopyA(adapterIdentifier.Driver, MAX_DEVICE_IDENTIFIER_STRING, pxGraphicDevicePhysicalCurrent->Driver, MAX_DEVICE_IDENTIFIER_STRING);
                break;
            }
        }
    }
}

PXActionResult PXAPI PXDirectX9Clear(PXDirectX9* const pxDirectX9, const PXColorRGBAF* const pxColorRGBAF)
{
    const UINT colorI8 = D3DCOLOR_ARGB
    (
        (PXInt8U)(pxColorRGBAF->Alpha * 0xFF),
        (PXInt8U)(pxColorRGBAF->Red * 0xFF),
        (PXInt8U)(pxColorRGBAF->Green * 0xFF),
        (PXInt8U)(pxColorRGBAF->Blue * 0xFF)
    );

    const HRESULT result = pxDirectX9->Device->lpVtbl->Clear
    (
        pxDirectX9->Device,
        0,
        0,
        D3DCLEAR_TARGET,
        colorI8,
        1.0f,
        0
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9SceneDeploy(PXDirectX9* const pxDirectX9)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->Present
    (
        pxDirectX9->Device,
        0,
        0,
        0,
        0
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9* const pxDirectX9, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U startVertex, const PXInt32U primitiveCount)
{
    const D3DPRIMITIVETYPE primitiveType = PXDirectXDrawTypeFromPX(PXGraphicDrawMode);

    const HRESULT result = pxDirectX9->Device->lpVtbl->DrawPrimitive
    (
        pxDirectX9->Device,
        primitiveType,
        startVertex,
        primitiveCount
    );


    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9* const pxDirectX9, const PXVertexBufferFormat pxVertexBufferFormat)
{
    const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBufferFormat);

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetFVF
    (
        pxDirectX9->Device,
        flagID
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9* const pxDirectX9, PXVertexBufferFormat* const pxVertexBufferFormat)
{
    DWORD flagID = 0;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetFVF
    (
        pxDirectX9->Device,
        &flagID
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9* const pxDirectX9, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->SetStreamSource
    (
        pxDirectX9->Device,
        StreamNumber,
        pxVertexBuffer->ResourceID.DirectXInterface,
        OffsetInBytes,
        Stride
    );

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9* const pxDirectX9, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ModelDraw(PXDirectX9* const pxDirectX9, PXModel* const pxModel, const PXCamera* const pxCamera)
{
    PXDirectX9StreamSourceSet(pxDirectX9, 0, &pxModel->VertexBuffer, 0, pxModel->VertexBuffer.VertexDataRowSize);
    PXDirectX9VertexFixedFunctionSet(pxDirectX9, pxModel->VertexBuffer.Format);
    PXDirectX9PrimitiveDraw(pxDirectX9, PXGraphicDrawModeTriangle, 0, 1);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index)
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

    switch(pxLight->Type)
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
            return PXActionRefusedArgumentInvalid;
    }

    const HRESULT result = pxDirectX9->Device->lpVtbl->SetLight(pxDirectX9->Device, index, &d3dLight9);

    return PXActionSuccessful;

  
}

PXActionResult PXAPI PXDirectX9LightGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index)
{
    BOOL enableBool = 0;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetLightEnable(pxDirectX9->Device, index, &enableBool);

    pxLight->Enabled = enableBool;
   // *enable = enableBool;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightEnableSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index, const PXBool enable)
{
    const HRESULT result = pxDirectX9->Device->lpVtbl->LightEnable(pxDirectX9->Device, index, enable);

    pxLight->Enabled = enable;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9LightEnableGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index, PXBool* const enable)
{
    D3DLIGHT9 d3dLight9;

    const HRESULT result = pxDirectX9->Device->lpVtbl->GetLight(pxDirectX9->Device, index, &d3dLight9);

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

    switch(d3dLight9.Type)
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

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderCreate(PXDirectX9* const pxDirectX9, PXShader* const pxShader)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderSelect(PXDirectX9* const pxDirectX9, PXShader* const pxShader)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ShaderCompile(PXDirectX9* const pxDirectX9, PXShader* const pxShader, const PXText* const shaderFilePath)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9* const pxDirectX9, PXVertexBuffer* const pxVertexBuffer)
{
    const DWORD flagID = PXDirectXVertexFormatFromPXVertexBufferFormat(pxVertexBuffer->Format);

    const HRESULT bufferCreateResult = pxDirectX9->Device->lpVtbl->CreateVertexBuffer
    (
        pxDirectX9->Device,
        pxVertexBuffer->VertexDataSize,
        0,
        flagID,
        D3DPOOL_DEFAULT,
        &(IDirect3DVertexBuffer9*)pxVertexBuffer->ResourceID.DirectXInterface,
        PXNull
    );
    const PXBool bufferCreateSuccess = bufferCreateResult > 0;


    const PXBool hasData = pxVertexBuffer->VertexData && pxVertexBuffer->VertexDataSize;

    if(!hasData)
    {
        return PXActionInvalid;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectX9",
        "Vertex buffer created 0x%p",
        pxVertexBuffer->ResourceID.DirectXInterface
    );
#endif

    IDirect3DVertexBuffer9* const vertexBuffer = (IDirect3DVertexBuffer9*)pxVertexBuffer->ResourceID.DirectXInterface;
    void* targetAdress = PXNull;

    const HRESULT lockResult = vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, pxVertexBuffer->VertexDataSize, &targetAdress, 0);

    PXMemoryCopy(pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize, targetAdress, MemorySizeUnkown);

    const HRESULT unlockResult = vertexBuffer->lpVtbl->Unlock(vertexBuffer);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9* const pxDirectX9, PXIndexBuffer* const pxIndexBuffer)
{
    D3DFORMAT dataFormat = D3DFMT_UNKNOWN;

    // fetch format
    {
        switch(pxIndexBuffer->DataType)
        {
            case PXDataTypeInt16SLE:
            case PXDataTypeInt16ULE:
            case PXDataTypeInt16SBE:
            case PXDataTypeInt16UBE:
            {
                dataFormat = D3DFMT_INDEX16;
                break;
            }
            case PXDataTypeInt32SLE:
            case PXDataTypeInt32ULE:
            case PXDataTypeInt32SBE:
            case PXDataTypeInt32UBE:
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

    const HRESULT bufferCreateResult = pxDirectX9->Device->lpVtbl->CreateIndexBuffer
    (
        pxDirectX9->Device,
        pxIndexBuffer->IndexDataSize,
        0,
        dataFormat,
        0,
        &(IDirect3DIndexBuffer9*)pxIndexBuffer->ResourceID.DirectXInterface,
        PXNull
    );


    // Fill data

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9* const pxDirectX9, PXRenderTarget* const pxRenderTarget)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9* const pxDirectX9, PXDepthStencilSurface* const pxDepthStencilSurface)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectX9ModelRegister(PXDirectX9* const pxDirectX9, PXModel* const pxModel)
{
    PXDirectX9VertexBufferCreate(pxDirectX9, &pxModel->VertexBuffer);
    PXDirectX9IndexBufferCreate(pxDirectX9, &pxModel->IndexBuffer);

    return PXActionSuccessful;
}





















D3DFORMAT PXAPI PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat)
{
    switch(pxColorFormat)
    {
        case PXColorFormatRGBI8: return D3DFMT_R8G8B8;
        case PXColorFormatARGBI8: return D3DFMT_A8R8G8B8;

        default:
            return D3DFMT_UNKNOWN;
    }
}

void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial)
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

    pxMaterial->Emission[0] = d3dMaterial->Emissive.r;
    pxMaterial->Emission[1] = d3dMaterial->Emissive.g;
    pxMaterial->Emission[2] = d3dMaterial->Emissive.b;
    pxMaterial->Emission[3] = d3dMaterial->Emissive.a;

    pxMaterial->Power = d3dMaterial->Power;
}


PXInt32U PXAPI PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatXYZ: return D3DFVF_XYZ;
        case PXVertexBufferFormatXYZC: return D3DFVF_XYZ | D3DFVF_DIFFUSE;
        case PXVertexBufferFormatXYZHWC: return D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

        default:
            return 0;
    }
}

void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial)
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

    d3dMaterial->Emissive.r = pxMaterial->Emission[0];
    d3dMaterial->Emissive.g = pxMaterial->Emission[1];
    d3dMaterial->Emissive.b = pxMaterial->Emission[2];
    d3dMaterial->Emissive.a = pxMaterial->Emission[3];

    d3dMaterial->Power = pxMaterial->Power;
}

PXColorFormat PXAPI PXDirectXColorFormatToID(const D3DFORMAT format)
{
    switch(format)
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

D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXGraphicDrawMode PXGraphicDrawMode)
{
    switch(PXGraphicDrawMode)
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



#if 0

PXActionResult PXAPI PXDirectXShaderVariableIDFetch(PXDirectX* const pxDirectX, const PXShader* pxShader, PXInt32U* const shaderVariableID, const char* const name)
{
#if WindowsAtleast10 && 0 // if the "D3DX9_43.DLL" is missing

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
    for(UINT i = 0; i < pDesc.Constants; ++i)
    {
        D3DXHANDLE Handle = pConstantTable->lpVtbl->GetConstant(pConstantTable, NULL, i);
        if(Handle == NULL) continue;
        pConstantTable->lpVtbl->GetConstantDesc(pConstantTable, Handle, pConstantDesc, &pConstantNum);


        printf("\n");

#if 0
        for(UINT j = 0; j < pConstantNum; j++)
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



PXActionResult PXAPI PXDirectXShaderProgramCreateFromFileVF(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath)
{
    PXFile vertexShaderFile;
    PXFile fragmentShaderFile;

    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.Text = *vertexShaderFilePath;
        pxFileOpenFromPathInfo.FileSize = 0;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        PXFileOpenFromPath(&vertexShaderFile, &pxFileOpenFromPathInfo);

        pxFileOpenFromPathInfo.Text = *fragmentShaderFilePath;

        PXFileOpenFromPath(&fragmentShaderFile, &pxFileOpenFromPathInfo);

        {
            PXText veretxShaderText;
            PXText pixelShaderText;

            PXTextConstructFromAdressA(&veretxShaderText, vertexShaderFile.Data, vertexShaderFile.DataSize, vertexShaderFile.DataSize);
            PXTextConstructFromAdressA(&pixelShaderText, fragmentShaderFile.Data, vertexShaderFile.DataSize, fragmentShaderFile.DataSize);

            PXActionResult shaderResult = PXDirectXShaderProgramCreateFromStringVF(pxDirectX, pxShaderProgram, &veretxShaderText, &pixelShaderText);
        }
    }

    PXFileDestruct(&vertexShaderFile);
    PXFileDestruct(&fragmentShaderFile);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectXShaderProgramCreateFromFileVFA(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    PXText veretxShaderText;
    PXText pixelShaderText;

    PXTextConstructFromAdressA(&veretxShaderText, vertexShaderFilePath, PXTextUnkownLength, PXTextUnkownLength);
    PXTextConstructFromAdressA(&pixelShaderText, fragmentShaderFilePath, PXTextUnkownLength, PXTextUnkownLength);

    return PXDirectXShaderProgramCreateFromFileVF(pxDirectX, pxShaderProgram, &veretxShaderText, &pixelShaderText);

    /*
        PXText pxTextVertexShaderW;
    PXText pxTextPixelShaderW;

    PXTextConstructFromAdressW(&pxTextVertexShaderW, vertexShader, PXTextUnkownLength);
    PXTextConstructFromAdressW(&pxTextPixelShaderW, pixelShader, PXTextUnkownLength);

    return PXDirectXShaderProgramCreateVP(pxDirectX, pxShaderProgram, &pxTextVertexShaderW, &pxTextPixelShaderW);
    */
}

PXActionResult PXAPI PXDirectXShaderProgramCreateFromStringVF(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath)
{
    PXDirectXShaderCompile(pxDirectX, &pxShaderProgram->VertexShader, vertexShaderFilePath);
    PXDirectXShaderCreate(pxDirectX, &pxShaderProgram->VertexShader);

    PXDirectXShaderCompile(pxDirectX, &pxShaderProgram->PixelShader, fragmentShaderFilePath);
    PXDirectXShaderCreate(pxDirectX, &pxShaderProgram->PixelShader);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectXShaderProgramCreateFromStringVFA(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    PXText veretxShaderText;
    PXText pixelShaderText;

    PXTextConstructFromAdressA(&veretxShaderText, vertexShaderFilePath, PXTextUnkownLength, PXTextUnkownLength);
    PXTextConstructFromAdressA(&pixelShaderText, fragmentShaderFilePath, PXTextUnkownLength, PXTextUnkownLength);

    return PXDirectXShaderProgramCreateFromStringVF(pxDirectX, pxShaderProgram, &veretxShaderText, &pixelShaderText);
}

PXActionResult PXAPI PXDirectXShaderCreate(PXDirectX* const pxDirectX, PXShader* const pxShader)
{
    switch(pxDirectX->DirectXVersion)
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

            switch(pxShader->Type)
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
            switch(pxShader->Type)
            {
                case PXShaderTypeVertex:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->CreateVertexShader
                    (
                        pxDirectX->DX9,
                        0,
                        &(IDirect3DVertexShader9*)pxShader->ResourceID.DirectXInterface
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "Shader:Vertex created 0x%p",
                        pxShader->ResourceID.DirectXInterface
                    );
#endif

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

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "Shader:Pixel created 0x%p",
                        pxShader->ResourceID.DirectXInterface
                    );
#endif

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

PXActionResult PXAPI PXDirectXShaderSelect(PXDirectX* const pxDirectX, PXShader* const pxShader)
{
    switch(pxDirectX->DirectXVersion)
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
            switch(pxShader->Type)
            {
                case PXShaderTypeVertex:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->SetVertexShader
                    (
                        pxDirectX->DX9,
                        (IDirect3DVertexShader9*)pxShader->ResourceID.DirectXInterface
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "Shader:Vertex select 0x%p",
                        pxShader->ResourceID.DirectXInterface
                    );
#endif

                    break;
                }
                case PXShaderTypeFragment:
                {
                    const HRESULT result = pxDirectX->DX9->lpVtbl->SetPixelShader
                    (
                        pxDirectX->DX9,
                        (IDirect3DPixelShader9*)pxShader->ResourceID.DirectXInterface
                    );

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "DirectX9",
                        "Shader:Pixel select 0x%p",
                        pxShader->ResourceID.DirectXInterface
                    );
#endif

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

PXActionResult PXAPI PXDirectXShaderCompile(PXDirectX* const pxDirectX, PXShader* const pxShader, const PXText* const shaderFilePath)
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

    if(!failed)
    {
        pxShader->ContentSize = ((ID3DBlob*)pxShader->Content)->lpVtbl->GetBufferSize((ID3DBlob*)pxShader->Content);

        return PXActionSuccessful;
    }


    if(error_blob)
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

#endif
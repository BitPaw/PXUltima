#ifndef PXDirectXINCLUDE
#define PXDirectXINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXDirectXUSE PXOSWindowsDestop
#if PXDirectXUSE

#include <OS/Graphic/PXGraphicGeneral.h>

#if OSUnix
// UNIX has no DirectX support
typedef unsigned int  IDirect3D9; // Dummy value
#define MAX_DEVICE_IDENTIFIER_STRING 32
#elif OSWindows
#include <d3d12.h>
#include <d3d11.h>
#include <d3d10.h>
#include <d3d9.h>

#endif


#ifdef __cplusplus
extern "C"
{
#endif

    typedef enum PXDirectXDriverType_
    {
        PXDirectXDriverTypeHardwareDevice,
        PXDirectXDriverTypeReferencDevice,
        PXDirectXDriverTypeReferencDeviceWithoutRender,
        PXDirectXDriverTypeSoftware,
    }
    PXDirectXDriverType;

    typedef enum PXDirectXVersion_
    {
        PXDirectXVersionInvalid,

        PXDirectXVersionNewest,

        PXDirectXVersion9, // Windows XP

        PXDirectXVersion10x0, // Windows Vista
        PXDirectXVersion10x1,  // Windows Vista
        PXDirectXVersion10x1Simulate9x1,
        PXDirectXVersion10x1Simulate9x2,
        PXDirectXVersion10x1Simulate9x3,
        PXDirectXVersion10x1Simulate10x0,


        // DirectX 11 - Windows 7, 8, 8.1 
        PXDirectXVersion11Emulate1x0Core,
        PXDirectXVersion11Emulate9x1,
        PXDirectXVersion11Emulate9x2,
        PXDirectXVersion11Emulate9x3,
        PXDirectXVersion11Emulate10x0,
        PXDirectXVersion11Emulate10x1,
        PXDirectXVersion11Emulate11x0,
        PXDirectXVersion11Emulate11x1,
        //PXDirectXVersion11Emulate12x0,
        //PXDirectXVersion11Emulate12x1,
        //PXDirectXVersion11Emulate12x2,


        // DirectX 12 - Windows 10, 11
        PXDirectXVersion12Emulate1x0Core,
        PXDirectXVersion12Emulate9x1,
        PXDirectXVersion12Emulate9x2,
        PXDirectXVersion12Emulate9x3,
        PXDirectXVersion12Emulate10x0,
        PXDirectXVersion12Emulate10x1,
        PXDirectXVersion12Emulate11x0,
        PXDirectXVersion12Emulate11x1,
        PXDirectXVersion12Emulate12x0,
        PXDirectXVersion12Emulate12x1,
        PXDirectXVersion12Emulate12x2,      
    }
    PXDirectXVersion;

	typedef struct PXDirectX
	{
        char Driver[MAX_DEVICE_IDENTIFIER_STRING];
        char Description[MAX_DEVICE_IDENTIFIER_STRING];
        char DeviceName[32];

        // Direct X - v.10+
        IDXGIAdapter VideoAdapter;

        // Direct X - v.9.x
        D3DCAPS9 DeviceCapabilitiesCurrent;
        IDirect3D9* DX9Context;
        IDirect3DDevice9* DX9;

        // Direct X - v.10.0
        ID3D10Device* DX10;

        // Direct X - v.10.1
        ID3D10Device1* DX10X1;

        // Direct X - v.11.0
        ID3D11Device* DX11;
        ID3D11DeviceContext* DX11Context;

        // Direct X - v.12
        ID3D12Device* DX12;
	}
	PXDirectX;


    PXPublic PXActionResult PXDirectCooperativeLevelTest(PXDirectX* const pxDirectX);

    PXPublic PXActionResult PXDirectXManagedResourcesEvict(PXDirectX* const pxDirectX);
    PXPublic PXActionResult PXDirectXDirect3DGet(PXDirectX* const pxDirectX, IDirect3D9** ppD3D9);
    PXPublic PXActionResult PXDirectXDeviceCapsGet(PXDirectX* const pxDirectX, D3DCAPS9* pCaps);
    PXPublic PXActionResult PXDirectXDisplayModeGet(PXDirectX* const pxDirectX, UINT iSwapChain, D3DDISPLAYMODE* pMode);
    PXPublic PXActionResult PXDirectXCreationParametersGet(PXDirectX* const pxDirectX, D3DDEVICE_CREATION_PARAMETERS* pParameters);
    PXPublic PXActionResult PXDirectXCursorPropertiesSet(PXDirectX* const pxDirectX, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
    PXPublic PXActionResult PXDirectXCursorPositionSet(PXDirectX* const pxDirectX, int X, int Y, DWORD Flags);
    PXPublic PXActionResult PXDirectXCursorShow(PXDirectX* const pxDirectX, const PXBool doShow);
    PXPublic PXActionResult PXDirectXSwapChainCreateAdditional(PXDirectX* const pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
    PXPublic PXActionResult PXDirectXSwapChainGet(PXDirectX* const pxDirectX, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
    PXPublic PXActionResult PXDirectXSwapChainsAmontGet(PXDirectX* const pxDirectX);
   
    PXPublic PXActionResult PXDirectXBackBufferGet(PXDirectX* const pxDirectX, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
    PXPublic PXActionResult PXDirectXRasterStatusGet(PXDirectX* const pxDirectX, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
    PXPublic PXActionResult PXDirectXDialogBoxModeSet(PXDirectX* const pxDirectX, BOOL bEnableDialogs);
    PXPublic PXActionResult PXDirectXGammaRampSet(PXDirectX* const pxDirectX, UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp);
    PXPublic PXActionResult PXDirectXGammaRampGet(PXDirectX* const pxDirectX, UINT iSwapChain, D3DGAMMARAMP* pRamp);
      
    PXPublic PXActionResult PXDirectXSurfaceUpdate(PXDirectX* const pxDirectX,IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint);
    PXPublic PXActionResult PXDirectXTextureUpdate(PXDirectX* const pxDirectX,IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
    PXPublic PXActionResult PXDirectXRenderTargetDataGet(PXDirectX* const pxDirectX,IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
    PXPublic PXActionResult PXDirectXFrontBufferDataGet(PXDirectX* const pxDirectX,UINT iSwapChain, IDirect3DSurface9* pDestSurface);
    PXPublic PXActionResult PXDirectXStretchRect(PXDirectX* const pxDirectX,IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);






    PXPublic PXActionResult PXDirectXClipPlaneSet(PXDirectX* const pxDirectX,DWORD Index, const float* pPlane);
    PXPublic PXActionResult PXDirectXClipPlaneGet(PXDirectX* const pxDirectX,DWORD Index, float* pPlane);
    PXPublic PXActionResult PXDirectXRenderStateSet(PXDirectX* const pxDirectX,D3DRENDERSTATETYPE State, DWORD Value);
    PXPublic PXActionResult PXDirectXRenderStateGet(PXDirectX* const pxDirectX,D3DRENDERSTATETYPE State, DWORD* pValue);
    PXPublic PXActionResult PXDirectXStateBlockCreate(PXDirectX* const pxDirectX,D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB);
    PXPublic PXActionResult PXDirectXStateBlockBegin(PXDirectX* const pxDirectX);
    PXPublic PXActionResult PXDirectXStateBlockEnd(PXDirectX* const pxDirectX,IDirect3DStateBlock9** ppSB);
    PXPublic PXActionResult PXDirectXClipStatusSet(PXDirectX* const pxDirectX,const D3DCLIPSTATUS9* pClipStatus);
    PXPublic PXActionResult PXDirectXClipStatusGet(PXDirectX* const pxDirectX,D3DCLIPSTATUS9* pClipStatus);

    PXPublic PXActionResult PXDirectXTextureStageStateGet(PXDirectX* const pxDirectX,DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
    PXPublic PXActionResult PXDirectXTextureStageStateSet(PXDirectX* const pxDirectX,DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
    PXPublic PXActionResult PXDirectXSamplerStateGet(PXDirectX* const pxDirectX,DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
    PXPublic PXActionResult PXDirectXSamplerStateSet(PXDirectX* const pxDirectX,DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
    PXPublic PXActionResult PXDirectXDeviceValidate(PXDirectX* const pxDirectX,DWORD* pNumPasses);
    PXPublic PXActionResult PXDirectXPaletteEntriesSet(PXDirectX* const pxDirectX,UINT PaletteNumber, const PALETTEENTRY* pEntries);
    PXPublic PXActionResult PXDirectXPaletteEntriesGet(PXDirectX* const pxDirectX,UINT PaletteNumber, PALETTEENTRY* pEntries);
    PXPublic PXActionResult PXDirectXTexturePaletteCurrentSet(PXDirectX* const pxDirectX,UINT PaletteNumber);
    PXPublic PXActionResult PXDirectXTexturePaletteCurrentGet(PXDirectX* const pxDirectX,UINT* PaletteNumber);
    PXPublic PXActionResult PXDirectXScissorRectSet(PXDirectX* const pxDirectX,const RECT* pRect);
    PXPublic PXActionResult PXDirectXScissorRectGet(PXDirectX* const pxDirectX,RECT* pRect);
    PXPublic PXActionResult PXDirectXSoftwareVertexProcessingSet(PXDirectX* const pxDirectX,BOOL bSoftware);
    PXPublic PXActionResult PXDirectXSoftwareVertexProcessingGet(PXDirectX* const pxDirectX);
    PXPublic PXActionResult PXDirectXNPatchModeSet(PXDirectX* const pxDirectX,float nSegments);
    PXPublic PXActionResult PXDirectXNPatchModeGet(PXDirectX* const pxDirectX);

   

 
  

  
    PXPublic PXActionResult PXDirectXRectPatchDraw(PXDirectX* const pxDirectX,UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo);
    PXPublic PXActionResult PXDirectXTriPatchDraw(PXDirectX* const pxDirectX,UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo);
    PXPublic PXActionResult PXDirectXPatchDelete(PXDirectX* const pxDirectX,UINT Handle);
    PXPublic PXActionResult PXDirectXQueryCreate(PXDirectX* const pxDirectX,D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);




    //-----------------------------------------------------
    // Direct X - Utility
    //-----------------------------------------------------
    PXPrivate PXColorFormat PXDirectXColorFormatToID(const D3DFORMAT format);
    PXPrivate D3DFORMAT PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat);
    PXPrivate void PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial);
    PXPrivate void PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial);
    PXPrivate D3DPRIMITIVETYPE PXDirectXDrawTypeFromPX(const PXGraphicRenderMode pxGraphicRenderMode);
    PXPrivate PXInt32U PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat); 

    PXPublic void PXDirectXContextConstruct(PXDirectX* const pxDirectX);
    PXPublic void PXDirectXContextDestruct(PXDirectX* const pxDirectX);

    PXPublic PXActionResult PXDirectXContextCreate(PXDirectX* const pxDirectX, const HWND pxWindowID, const PXDirectXVersion pxDirectXVersion, const PXDirectXDriverType pxDirectXDriverType);

    //-----------------------------------------------------
    // Direct X - Scene
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXSceneBegin(PXDirectX* const pxDirectX);
    PXPublic PXActionResult PXDirectXSceneEnd(PXDirectX* const pxDirectX);

    //-----------------------------------------------------
    // Direct X - Draw
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXClear(PXDirectX* const pxDirectX, const PXInt32U Count, const D3DRECT* pRects, const PXInt32U Flags, const PXInt32U colorRGBAID, const float Z, const PXInt32U Stencil);
    PXPublic PXActionResult PXDirectXColorFill(PXDirectX* const pxDirectX, IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color);
    PXPublic PXActionResult PXDirectXReset(PXDirectX* const pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters);
    PXPublic PXActionResult PXDirectXPresent(PXDirectX* const pxDirectX, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);
    PXPublic PXActionResult PXDirectXPrimitiveIndexedDraw(PXDirectX* const pxDirectX, const PXGraphicRenderMode pxGraphicRenderMode, const PXInt32U BaseVertexIndex, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U startIndex, const PXInt32U primCount);
    PXPublic PXActionResult PXDirectXPrimitiveDraw(PXDirectX* const pxDirectX, const PXGraphicRenderMode pxGraphicRenderMode, const PXInt32U startVertex, const PXInt32U primitiveCount);
    PXPublic PXActionResult PXDirectXPrimitiveUPDraw(PXDirectX* const pxDirectX, const PXGraphicRenderMode pxGraphicRenderMode, const PXInt32U PrimitiveCount, const void* pVertexStreamZeroData, const PXInt32U VertexStreamZeroStride);
    PXPublic PXActionResult PXDirectXPrimitiveUPIndexedDraw(PXDirectX* const pxDirectX, const PXGraphicRenderMode pxGraphicRenderMode, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, const PXInt32U VertexStreamZeroStride);


    PXPublic PXActionResult PXDirectXVertexFixedFunctionSet(PXDirectX* const pxDirectX, const PXVertexBufferFormat pxVertexBufferFormat);
    PXPublic PXActionResult PXDirectXVertexFixedFunctionGet(PXDirectX* const pxDirectX, PXVertexBufferFormat* const pxVertexBufferFormat);
    PXPublic PXActionResult PXDirectXStreamSourceSet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride);
    PXPublic PXActionResult PXDirectXStreamSourceGet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride);
    PXPublic PXActionResult PXDirectXStreamSourceFreqSet(PXDirectX* const pxDirectX, UINT StreamNumber, UINT Setting);
    PXPublic PXActionResult PXDirectXStreamSourceFreqGet(PXDirectX* const pxDirectX, UINT StreamNumber, UINT* pSetting);


    //-----------------------------------------------------
    // Direct X - Light
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXLightSet(PXDirectX* const pxDirectX, DWORD Index, const D3DLIGHT9*);
    PXPublic PXActionResult PXDirectXLightGet(PXDirectX* const pxDirectX, DWORD Index, D3DLIGHT9*);
    PXPublic PXActionResult PXDirectXLightEnable(PXDirectX* const pxDirectX, DWORD Index, BOOL Enable);
    PXPublic PXActionResult PXDirectXLightEnableGet(PXDirectX* const pxDirectX, DWORD Index, BOOL* pEnable);


    //-----------------------------------------------------
    // Direct X - Transform
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXTransformSet(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
    PXPublic PXActionResult PXDirectXTransformGet(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
    PXPublic PXActionResult PXDirectXTransformMultiply(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE, const D3DMATRIX*);


    //-----------------------------------------------------
    // Direct X - Viewport
    //-----------------------------------------------------PXViewPort D3DVIEWPORT9
    PXPublic PXActionResult PXDirectXViewportSet(PXDirectX* const pxDirectX, const PXViewPort* const pxViewPort);
    PXPublic PXActionResult PXDirectXViewportGet(PXDirectX* const pxDirectX, PXViewPort* const pxViewPort);


    //-----------------------------------------------------
    // Direct X - Material
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXMaterialSet(PXDirectX* const pxDirectX, const PXMaterial* const pxMaterial);
    PXPublic PXActionResult PXDirectXMaterialGet(PXDirectX* const pxDirectX, PXMaterial* const pxMaterial);


    //-----------------------------------------------------
    // Direct X - Shader
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXVertexShaderCreate(PXDirectX* const pxDirectX, const DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    PXPublic PXActionResult PXDirectXVertexShaderSet(PXDirectX* const pxDirectX, IDirect3DVertexShader9* pShader);
    PXPublic PXActionResult PXDirectXVertexShaderGet(PXDirectX* const pxDirectX, IDirect3DVertexShader9** ppShader);
    PXPublic PXActionResult PXDirectXVertexShaderConstantFSet(PXDirectX* const pxDirectX, UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXDirectXVertexShaderConstantFGet(PXDirectX* const pxDirectX, UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXDirectXVertexShaderConstantISet(PXDirectX* const pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXDirectXVertexShaderConstantIGet(PXDirectX* const pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXDirectXVertexShaderConstantBSet(PXDirectX* const pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
    PXPublic PXActionResult PXDirectXVertexShaderConstantBGet(PXDirectX* const pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);

    PXPublic PXActionResult PXDirectXPixelShaderCreate(PXDirectX* const pxDirectX, const DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    PXPublic PXActionResult PXDirectXPixelShaderSet(PXDirectX* const pxDirectX, IDirect3DPixelShader9* pShader);
    PXPublic PXActionResult PXDirectXPixelShaderGet(PXDirectX* const pxDirectX, IDirect3DPixelShader9** ppShader);
    PXPublic PXActionResult PXDirectXPixelShaderConstantFSet(PXDirectX* const pxDirectX, UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXDirectXPixelShaderConstantFGet(PXDirectX* const pxDirectX, UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXDirectXPixelShaderConstantISet(PXDirectX* const pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXDirectXPixelShaderConstantIGet(PXDirectX* const pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXDirectXPixelShaderConstantBSet(PXDirectX* const pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
    PXPublic PXActionResult PXDirectXPixelShaderConstantBGet(PXDirectX* const pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);




    //-----------------------------------------------------
    // Direct X - Buffers
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXVertexBufferCreate(PXDirectX* const pxDirectX, PXVertexBuffer* const pxVertexBuffer);
    PXPublic PXActionResult PXDirectXVerticesProcess(PXDirectX* const pxDirectX, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
    PXPublic PXActionResult PXDirectXVertexDeclarationCreate(PXDirectX* const pxDirectX, const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    PXPublic PXActionResult PXDirectXVertexDeclarationSet(PXDirectX* const pxDirectX, IDirect3DVertexDeclaration9* pDecl);
    PXPublic PXActionResult PXDirectXVertexDeclarationGet(PXDirectX* const pxDirectX, IDirect3DVertexDeclaration9** ppDecl);

    PXPublic PXActionResult PXDirectXIndexBufferCreate(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer);
    PXPublic PXActionResult PXDirectXIndicesSet(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer);
    PXPublic PXActionResult PXDirectXIndicesGet(PXDirectX* const pxDirectX, PXIndexBuffer** pxIndexBuffer);

    PXPublic PXActionResult PXDirectXRenderTargetCreate(PXDirectX* const pxDirectX, PXRenderTarget* const pxRenderTarget);
    PXPublic PXActionResult PXDirectXRenderTargetSet(PXDirectX* const pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    PXPublic PXActionResult PXDirectXRenderTargetGet(PXDirectX* const pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);

    PXPublic PXActionResult PXDirectXDepthStencilSurfaceCreate(PXDirectX* const pxDirectX, PXDepthStencilSurface* const pxDepthStencilSurface);
    PXPublic PXActionResult PXDirectXDepthStencilSurfaceSet(PXDirectX* const pxDirectX, IDirect3DSurface9* pNewZStencil);
    PXPublic PXActionResult PXDirectXDepthStencilSurfaceGet(PXDirectX* const pxDirectX, IDirect3DSurface9** ppZStencilSurface);

    PXPublic PXActionResult PXDirectXOffscreenPlainSurfaceCreate(PXDirectX* const pxDirectX, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);



    //-----------------------------------------------------
    // Direct X - Textures
    //-----------------------------------------------------
    PXPublic PXActionResult PXDirectXAvailableTextureMemoryGet(PXDirectX* const pxDirectX, PXInt32U* const value);
    PXPublic PXActionResult PXDirectXTextureGet(PXDirectX* const pxDirectX, DWORD Stage, IDirect3DBaseTexture9** ppTexture);
    PXPublic PXActionResult PXDirectXTextureSet(PXDirectX* const pxDirectX, DWORD Stage, IDirect3DBaseTexture9* pTexture);
	PXPublic PXActionResult PXDirectXTexture2DCreate(PXDirectX* const pxDirectX, PXTexture2D* const pxTexture2D);
	PXPublic PXActionResult PXDirectXTexture3DCreate(PXDirectX* const pxDirectX, PXTexture3D* const pxTexture3D);
	PXPublic PXActionResult PXDirectXTextureCubeCreate(PXDirectX* const pxDirectX, PXTextureCube* const pxTextureCube);


#ifdef __cplusplus
}
#endif

#endif
#endif
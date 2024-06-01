#ifndef PXDirectXINCLUDE
#define PXDirectXINCLUDE

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

//-------------------------------------
// DirectX - Settings
//-------------------------------------
#define PXDX9Enable OSWindows && 1u
#define PXDX10Enable OSWindows && 1u
#define PXDX10x1Enable OSWindows && 1u
#define PXDX11Enable OSWindows && 1u
#define PXDX12Enable OSWindows && 0

#define PXDX10Atleast PXDX10Enable || PXDX10x1Enable || PXDX11Enable || PXDX12Enable
//-------------------------------------

#if OSUnix
// UNIX has no DirectX support
typedef unsigned int  IDirect3D9; // Dummy value
#define MAX_DEVICE_IDENTIFIER_STRING 32

typedef void* D3DMATERIAL9;
#elif OSWindows
#include <d3d9.h>
#endif


typedef struct PXGraphicInitializeInfo_ PXGraphicInitializeInfo;
typedef struct PXGraphicDevicePhysical_	PXGraphicDevicePhysical;

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
    // 
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


// Direct X - v.13
typedef struct PXDirectX13_
{
    void* DX13;
}
PXDirectX13;


// Direct X - v.12
typedef struct PXDirectX12_
{
    struct ID3D12Device* Device;
    struct ID3D12CommandQueue* CommandQueue;
    struct IDXGISwapChain* SwapChain;
    struct ID3D12DescriptorHeap* HeapDescriptor;
    struct ID3D12CommandAllocator* CommandAllocator;
    struct IDXGIFactory* GIFactory;
}
PXDirectX12;


// Direct X - v.11
typedef struct PXDirectX11_
{
    struct ID3D11Device* Device;
    struct ID3D11DeviceContext* Context;
    struct IDXGISwapChain* SwapChain;
    struct ID3D11RenderTargetView* RenderTargetView;
    struct ID3D11Texture2D* FrameBuffer;

    PXLibrary DirectX11Library;
}
PXDirectX11;

// Direct X - v.10.1
typedef struct PXDirectX10x1_
{
    struct ID3D10Device1* Device;
}
PXDirectX10x1;


// DirectX - v.10.0
typedef struct PXDirectX10_
{
    struct ID3D10Device* Device;
    struct IDXGIAdapter* VideoAdapter;
}
PXDirectX10;

// DirectX - v.9.x
typedef struct PXDirectX9_
{
    struct IDirect3DDevice9* Device;
    struct IDirect3D9* Context;

    PXLibrary LibraryDirect3D;
    PXLibrary LibraryDirect3DExtension;
    PXLibrary LibraryDirectShaderCompiler;


    void* ShaderCompile; // D3DCompile

    void* ShaderConstantTableGet;
    void* ShaderConstantTableGetEx;

#if OSWindows
    D3DCAPS9 DeviceCapabilitiesCurrent;
#endif
}
PXDirectX9;



#if 0
typedef struct PXDirectX_ PXDirectX;



typedef PXActionResult(PXAPI* PXDirectXVInitialize)(PXDirectX* const pxDirectX, void* const pxDirectXX);
typedef PXActionResult(PXAPI* PXDirectXVRelease)(PXDirectX* const pxDirectX, void* const pxDirectXX);
typedef PXActionResult(PXAPI* PXDirectCooperativeLevelTest)(PXDirectX* const pxDirectX);

typedef PXActionResult(PXAPI* PXDirectXManagedResourcesEvict)(PXDirectX* const pxDirectX);

typedef PXActionResult(PXAPI* PXDirectXDisplayModeGet)(PXDirectX* const pxDirectX, UINT iSwapChain, D3DDISPLAYMODE* pMode);
typedef PXActionResult(PXAPI* PXDirectXCreationParametersGet)(PXDirectX* const pxDirectX, D3DDEVICE_CREATION_PARAMETERS* pParameters);
typedef PXActionResult(PXAPI* PXDirectXCursorPropertiesSet)(PXDirectX* const pxDirectX, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
typedef PXActionResult(PXAPI* PXDirectXCursorPositionSet)(PXDirectX* const pxDirectX, int X, int Y, DWORD Flags);
typedef PXActionResult(PXAPI* PXDirectXCursorShow)(PXDirectX* const pxDirectX, const PXBool doShow);
typedef PXActionResult(PXAPI* PXDirectXSwapChainCreateAdditional)(PXDirectX* const pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
typedef PXActionResult(PXAPI* PXDirectXSwapChainGet)(PXDirectX* const pxDirectX, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
typedef PXActionResult(PXAPI* PXDirectXSwapChainsAmontGet)(PXDirectX* const pxDirectX);

typedef PXActionResult(PXAPI* PXDirectXBackBufferGet)(PXDirectX* const pxDirectX, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
typedef PXActionResult(PXAPI* PXDirectXRasterStatusGet)(PXDirectX* const pxDirectX, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
typedef PXActionResult(PXAPI* PXDirectXDialogBoxModeSet)(PXDirectX* const pxDirectX, BOOL bEnableDialogs);
typedef PXActionResult(PXAPI* PXDirectXGammaRampSet)(PXDirectX* const pxDirectX, UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp);
typedef PXActionResult(PXAPI* PXDirectXGammaRampGet)(PXDirectX* const pxDirectX, UINT iSwapChain, D3DGAMMARAMP* pRamp);

typedef PXActionResult(PXAPI* PXDirectXSurfaceUpdate)(PXDirectX* const pxDirectX, IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint);
typedef PXActionResult(PXAPI* PXDirectXTextureUpdate)(PXDirectX* const pxDirectX, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
typedef PXActionResult(PXAPI* PXDirectXRenderTargetDataGet)(PXDirectX* const pxDirectX, IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
typedef PXActionResult(PXAPI* PXDirectXFrontBufferDataGet)(PXDirectX* const pxDirectX, UINT iSwapChain, IDirect3DSurface9* pDestSurface);
typedef PXActionResult(PXAPI* PXDirectXStretchRect)(PXDirectX* const pxDirectX, IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);






typedef PXActionResult(PXAPI* PXDirectXClipPlaneSet)(PXDirectX* const pxDirectX, DWORD Index, const float* pPlane);
typedef PXActionResult(PXAPI* PXDirectXClipPlaneGet)(PXDirectX* const pxDirectX, DWORD Index, float* pPlane);
typedef PXActionResult(PXAPI* PXDirectXRenderStateSet)(PXDirectX* const pxDirectX, D3DRENDERSTATETYPE State, DWORD Value);
typedef PXActionResult(PXAPI* PXDirectXRenderStateGet)(PXDirectX* const pxDirectX, D3DRENDERSTATETYPE State, DWORD* pValue);

typedef PXActionResult(PXAPI* PXDirectXClipStatusSet)(PXDirectX* const pxDirectX, const D3DCLIPSTATUS9* pClipStatus);
typedef PXActionResult(PXAPI* PXDirectXClipStatusGet)(PXDirectX* const pxDirectX, D3DCLIPSTATUS9* pClipStatus);

typedef PXActionResult(PXAPI* PXDirectXTextureStageStateGet)(PXDirectX* const pxDirectX, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
typedef PXActionResult(PXAPI* PXDirectXTextureStageStateSet)(PXDirectX* const pxDirectX, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
typedef PXActionResult(PXAPI* PXDirectXSamplerStateGet)(PXDirectX* const pxDirectX, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
typedef PXActionResult(PXAPI* PXDirectXSamplerStateSet)(PXDirectX* const pxDirectX, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
typedef PXActionResult(PXAPI* PXDirectXDeviceValidate)(PXDirectX* const pxDirectX, DWORD* pNumPasses);
typedef PXActionResult(PXAPI* PXDirectXPaletteEntriesSet)(PXDirectX* const pxDirectX, UINT PaletteNumber, const PALETTEENTRY* pEntries);
typedef PXActionResult(PXAPI* PXDirectXPaletteEntriesGet)(PXDirectX* const pxDirectX, UINT PaletteNumber, PALETTEENTRY* pEntries);
typedef PXActionResult(PXAPI* PXDirectXTexturePaletteCurrentSet)(PXDirectX* const pxDirectX, UINT PaletteNumber);
typedef PXActionResult(PXAPI* PXDirectXTexturePaletteCurrentGet)(PXDirectX* const pxDirectX, UINT* PaletteNumber);
typedef PXActionResult(PXAPI* PXDirectXScissorRectSet)(PXDirectX* const pxDirectX, const RECT* pRect);
typedef PXActionResult(PXAPI* PXDirectXScissorRectGet)(PXDirectX* const pxDirectX, RECT* pRect);
typedef PXActionResult(PXAPI* PXDirectXSoftwareVertexProcessingSet)(PXDirectX* const pxDirectX, BOOL bSoftware);
typedef PXActionResult(PXAPI* PXDirectXSoftwareVertexProcessingGet)(PXDirectX* const pxDirectX);
typedef PXActionResult(PXAPI* PXDirectXNPatchModeSet)(PXDirectX* const pxDirectX, float nSegments);
typedef PXActionResult(PXAPI* PXDirectXNPatchModeGet)(PXDirectX* const pxDirectX);







typedef PXActionResult(PXAPI* PXDirectXRectPatchDraw)(PXDirectX* const pxDirectX, UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo);
typedef PXActionResult(PXAPI* PXDirectXTriPatchDraw)(PXDirectX* const pxDirectX, UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo);
typedef PXActionResult(PXAPI* PXDirectXPatchDelete)(PXDirectX* const pxDirectX, UINT Handle);
typedef PXActionResult(PXAPI* PXDirectXQueryCreate)(PXDirectX* const pxDirectX, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);



//-----------------------------------------------------
// Direct X - Utility
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXInitialize)(PXDirectX* const pxDirectX, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
typedef PXActionResult(PXAPI* PXDirectXRelease)(PXDirectX* const pxDirectX);

//-----------------------------------------------------
// Direct X - Scene
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXSceneBegin)(PXDirectX* const pxDirectX);
typedef PXActionResult(PXAPI* PXDirectXSceneEnd)(PXDirectX* const pxDirectX);

//-----------------------------------------------------
// Direct X - Fixed Scripting
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXDrawScriptCreate)(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
typedef PXActionResult(PXAPI* PXDirectXDrawScriptBegin)(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);
typedef PXActionResult(PXAPI* PXDirectXDrawScriptEnd)(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);
typedef PXActionResult(PXAPI* PXDirectXDrawScriptDelete)(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);
typedef PXActionResult(PXAPI* PXDirectXDrawScriptExecute)(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);

//-----------------------------------------------------
// Direct X - Draw
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXClear)(PXDirectX* const pxDirectX, const PXColorRGBAF* const pxColorRGBAF);
typedef PXActionResult(PXAPI* PXDirectXColorFill)(PXDirectX* const pxDirectX, IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color);
typedef PXActionResult(PXAPI* PXDirectXReset)(PXDirectX* const pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef PXActionResult(PXAPI* PXDirectXSceneDeploy)(PXDirectX* const pxDirectX);
typedef PXActionResult(PXAPI* PXDirectXPrimitiveIndexedDraw)(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U BaseVertexIndex, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U startIndex, const PXInt32U primCount);
typedef PXActionResult(PXAPI* PXDirectXPrimitiveDraw)(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U startVertex, const PXInt32U primitiveCount);
typedef PXActionResult(PXAPI* PXDirectXPrimitiveUPDraw)(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U PrimitiveCount, const void* pVertexStreamZeroData, const PXInt32U VertexStreamZeroStride);
typedef PXActionResult(PXAPI* PXDirectXPrimitiveUPIndexedDraw)(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, const PXInt32U VertexStreamZeroStride);


typedef PXActionResult(PXAPI* PXDirectXVertexFixedFunctionSet)(PXDirectX* const pxDirectX, const PXVertexBufferFormat pxVertexBufferFormat);
typedef PXActionResult(PXAPI* PXDirectXVertexFixedFunctionGet)(PXDirectX* const pxDirectX, PXVertexBufferFormat* const pxVertexBufferFormat);
typedef PXActionResult(PXAPI* PXDirectXStreamSourceSet)(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride);
typedef PXActionResult(PXAPI* PXDirectXStreamSourceGet)(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride);
typedef PXActionResult(PXAPI* PXDirectXStreamSourceFreqSet)(PXDirectX* const pxDirectX, UINT StreamNumber, UINT Setting);
typedef PXActionResult(PXAPI* PXDirectXStreamSourceFreqGet)(PXDirectX* const pxDirectX, UINT StreamNumber, UINT* pSetting);

typedef PXActionResult(PXAPI* PXDirectXModelDraw)(PXDirectX* const pxDirectX, PXModel* const pxModel, const PXCamera* const pxCamera);


//-----------------------------------------------------
// Direct X - Light
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXLightSet)(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index); // D3DLIGHT9
typedef PXActionResult(PXAPI* PXDirectXLightGet)(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index);
typedef PXActionResult(PXAPI* PXDirectXLightEnableSet)(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
typedef PXActionResult(PXAPI* PXDirectXLightEnableGet)(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);


//-----------------------------------------------------
// Direct X - Transform
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXTransformSet)(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
typedef PXActionResult(PXAPI* PXDirectXTransformGet)(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
typedef PXActionResult(PXAPI* PXDirectXTransformMultiply)(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE, const D3DMATRIX*);


//-----------------------------------------------------
// Direct X - Viewport
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXViewportSet)(PXDirectX* const pxDirectX, const PXViewPort* const pxViewPort);
typedef PXActionResult(PXAPI* PXDirectXViewportGet)(PXDirectX* const pxDirectX, PXViewPort* const pxViewPort);


//-----------------------------------------------------
// Direct X - Material
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXMaterialSet)(PXDirectX* const pxDirectX, const PXMaterial* const pxMaterial);
typedef PXActionResult(PXAPI* PXDirectXMaterialGet)(PXDirectX* const pxDirectX, PXMaterial* const pxMaterial);


//-----------------------------------------------------
// Direct X - Shader
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXShaderProgramCreateFromFileVF)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
typedef PXActionResult(PXAPI* PXDirectXShaderProgramCreateFromFileVFA)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
typedef PXActionResult(PXAPI* PXDirectXShaderProgramCreateFromStringVF)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
typedef PXActionResult(PXAPI* PXDirectXShaderProgramCreateFromStringVFA)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);


typedef PXActionResult(PXAPI* PXDirectXShaderProgramCreate)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram);
typedef PXActionResult(PXAPI* PXDirectXShaderProgramSelect)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram);
typedef PXActionResult(PXAPI* PXDirectXShaderProgramDelete)(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram);


typedef PXActionResult(PXAPI* PXDirectXShaderCreate)(PXDirectX* const pxDirectX, PXShader* const pxShader);
typedef PXActionResult(PXAPI* PXDirectXShaderSelect)(PXDirectX* const pxDirectX, PXShader* const pxShader);
typedef PXActionResult(PXAPI* PXDirectXShaderCompile)(PXDirectX* const pxDirectX, PXShader* const pxShader, const PXText* const shaderFilePath);

typedef PXActionResult(PXAPI* PXDirectXShaderVariableIDFetch)(PXDirectX* const pxDirectX, const PXShader* pxShader, PXInt32U* const shaderVariableID, const char* const name);


typedef PXActionResult(PXAPI* PXDirectXVertexShaderCreate)(PXDirectX* const pxDirectX, const DWORD* pFunction, IDirect3DVertexShader9** ppShader);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderSet)(PXDirectX* const pxDirectX, IDirect3DVertexShader9* pShader);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderGet)(PXDirectX* const pxDirectX, IDirect3DVertexShader9** ppShader);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderConstantFSet)(PXDirectX* const pxDirectX, UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderConstantFGet)(PXDirectX* const pxDirectX, UINT StartRegister, float* pConstantData, UINT Vector4fCount);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderConstantISet)(PXDirectX* const pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderConstantIGet)(PXDirectX* const pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderConstantBSet)(PXDirectX* const pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
typedef PXActionResult(PXAPI* PXDirectXVertexShaderConstantBGet)(PXDirectX* const pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);

typedef PXActionResult(PXAPI* PXDirectXPixelShaderCreate)(PXDirectX* const pxDirectX, const DWORD* pFunction, IDirect3DPixelShader9** ppShader);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderSet)(PXDirectX* const pxDirectX, IDirect3DPixelShader9* pShader);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderGet)(PXDirectX* const pxDirectX, IDirect3DPixelShader9** ppShader);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderConstantFSet)(PXDirectX* const pxDirectX, UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderConstantFGet)(PXDirectX* const pxDirectX, UINT StartRegister, float* pConstantData, UINT Vector4fCount);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderConstantISet)(PXDirectX* const pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderConstantIGet)(PXDirectX* const pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderConstantBSet)(PXDirectX* const pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
typedef PXActionResult(PXAPI* PXDirectXPixelShaderConstantBGet)(PXDirectX* const pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);




//-----------------------------------------------------
// Direct X - Buffers
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXVertexBufferCreate)(PXDirectX* const pxDirectX, PXVertexBuffer* const pxVertexBuffer);
typedef PXActionResult(PXAPI* PXDirectXVerticesProcess)(PXDirectX* const pxDirectX, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
typedef PXActionResult(PXAPI* PXDirectXVertexDeclarationCreate)(PXDirectX* const pxDirectX, const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
typedef PXActionResult(PXAPI* PXDirectXVertexDeclarationSet)(PXDirectX* const pxDirectX, IDirect3DVertexDeclaration9* pDecl);
typedef PXActionResult(PXAPI* PXDirectXVertexDeclarationGet)(PXDirectX* const pxDirectX, IDirect3DVertexDeclaration9** ppDecl);

typedef PXActionResult(PXAPI* PXDirectXIndexBufferCreate)(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer);
typedef PXActionResult(PXAPI* PXDirectXIndicesSet)(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer);
typedef PXActionResult(PXAPI* PXDirectXIndicesGet)(PXDirectX* const pxDirectX, PXIndexBuffer** pxIndexBuffer);

typedef PXActionResult(PXAPI* PXDirectXRenderTargetCreate)(PXDirectX* const pxDirectX, PXRenderTarget* const pxRenderTarget);
typedef PXActionResult(PXAPI* PXDirectXRenderTargetSet)(PXDirectX* const pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
typedef PXActionResult(PXAPI* PXDirectXRenderTargetGet)(PXDirectX* const pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);

typedef PXActionResult(PXAPI* PXDirectXDepthStencilSurfaceCreate)(PXDirectX* const pxDirectX, PXDepthStencilSurface* const pxDepthStencilSurface);
typedef PXActionResult(PXAPI* PXDirectXDepthStencilSurfaceSet)(PXDirectX* const pxDirectX, IDirect3DSurface9* pNewZStencil);
typedef PXActionResult(PXAPI* PXDirectXDepthStencilSurfaceGet)(PXDirectX* const pxDirectX, IDirect3DSurface9** ppZStencilSurface);

typedef PXActionResult(PXAPI* PXDirectXOffscreenPlainSurfaceCreate)(PXDirectX* const pxDirectX, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);

typedef PXActionResult(PXAPI* PXDirectXModelRegister)(PXDirectX* const pxDirectX, PXModel* const pxModel);

//-----------------------------------------------------
// Direct X - Textures
//-----------------------------------------------------
typedef PXActionResult(PXAPI* PXDirectXTextureMemoryAvailable)(PXDirectX* const pxDirectX, PXInt32U* const value);
typedef PXActionResult(PXAPI* PXDirectXTexture2DCreate)(PXDirectX* const pxDirectX, PXTexture2D* const pxTexture2D);
typedef PXActionResult(PXAPI* PXDirectXTexture3DCreate)(PXDirectX* const pxDirectX, PXTexture3D* const pxTexture3D);
typedef PXActionResult(PXAPI* PXDirectXTextureCubeCreate)(PXDirectX* const pxDirectX, PXTextureCube* const pxTextureCube);
typedef PXActionResult(PXAPI* PXDirectXDevicePhysicalListAmountFunction)(PXDirectX* const pxDirectX, PXInt32U* const amountOfAdapters);
typedef PXActionResult(PXAPI* PXDirectXDevicePhysicalListFetchFunction)(PXDirectX* const pxDirectX, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);
typedef PXActionResult(PXAPI* PXDirectXSwapIntervalSet)(PXDirectX* const pxDirectX, const PXInt32U interval);
typedef PXActionResult(PXAPI* PXDirectXSwapIntervalGet)(PXDirectX* const pxDirectX, PXInt32U* const interval);

#endif

typedef struct PXDirectX
{
    union
    {
        PXDirectX9 X9;
        PXDirectX10 X10;
        PXDirectX10x1 X10x1;
        PXDirectX11 X11;
        PXDirectX12 X12;
        PXDirectX13 X13;
    };

    PXDirectXVersion DirectXVersion;

    void* DXTargetAPI;
}
PXDirectX;

PXPublic PXActionResult PXAPI PXDirectXInitialize(PXDirectX* const pxDirectX, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXActionResult PXAPI PXDirectXRelease(PXDirectX* const pxDirectX);

#endif
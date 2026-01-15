#pragma once

#ifndef PXDirectXIncluded
#define PXDirectXIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

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



typedef PXResult(PXAPI* PXDirectXVInitialize)(PXDirectX PXREF pxDirectX, void PXREF pxDirectXX);
typedef PXResult(PXAPI* PXDirectXVRelease)(PXDirectX PXREF pxDirectX, void PXREF pxDirectXX);
typedef PXResult(PXAPI* PXDirectCooperativeLevelTest)(PXDirectX PXREF pxDirectX);

typedef PXResult(PXAPI* PXDirectXManagedResourcesEvict)(PXDirectX PXREF pxDirectX);

typedef PXResult(PXAPI* PXDirectXDisplayModeGet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, D3DDISPLAYMODE* pMode);
typedef PXResult(PXAPI* PXDirectXCreationParametersGet)(PXDirectX PXREF pxDirectX, D3DDEVICE_CREATION_PARAMETERS* pParameters);
typedef PXResult(PXAPI* PXDirectXCursorPropertiesSet)(PXDirectX PXREF pxDirectX, UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap);
typedef PXResult(PXAPI* PXDirectXCursorPositionSet)(PXDirectX PXREF pxDirectX, int X, int Y, DWORD Flags);
typedef PXResult(PXAPI* PXDirectXCursorShow)(PXDirectX PXREF pxDirectX, const PXBool doShow);
typedef PXResult(PXAPI* PXDirectXSwapChainCreateAdditional)(PXDirectX PXREF pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain);
typedef PXResult(PXAPI* PXDirectXSwapChainGet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, IDirect3DSwapChain9** pSwapChain);
typedef PXResult(PXAPI* PXDirectXSwapChainsAmontGet)(PXDirectX PXREF pxDirectX);

typedef PXResult(PXAPI* PXDirectXBackBufferGet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer);
typedef PXResult(PXAPI* PXDirectXRasterStatusGet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus);
typedef PXResult(PXAPI* PXDirectXDialogBoxModeSet)(PXDirectX PXREF pxDirectX, BOOL bEnableDialogs);
typedef PXResult(PXAPI* PXDirectXGammaRampSet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp);
typedef PXResult(PXAPI* PXDirectXGammaRampGet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, D3DGAMMARAMP* pRamp);

typedef PXResult(PXAPI* PXDirectXSurfaceUpdate)(PXDirectX PXREF pxDirectX, IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const POINT* pDestPoint);
typedef PXResult(PXAPI* PXDirectXTextureUpdate)(PXDirectX PXREF pxDirectX, IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture);
typedef PXResult(PXAPI* PXDirectXRenderTargetDataGet)(PXDirectX PXREF pxDirectX, IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface);
typedef PXResult(PXAPI* PXDirectXFrontBufferDataGet)(PXDirectX PXREF pxDirectX, UINT iSwapChain, IDirect3DSurface9* pDestSurface);
typedef PXResult(PXAPI* PXDirectXStretchRect)(PXDirectX PXREF pxDirectX, IDirect3DSurface9* pSourceSurface, const RECT* pSourceRect, IDirect3DSurface9* pDestSurface, const RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter);






typedef PXResult(PXAPI* PXDirectXClipPlaneSet)(PXDirectX PXREF pxDirectX, DWORD Index, const PXF32* pPlane);
typedef PXResult(PXAPI* PXDirectXClipPlaneGet)(PXDirectX PXREF pxDirectX, DWORD Index, PXF32* pPlane);
typedef PXResult(PXAPI* PXDirectXRenderStateSet)(PXDirectX PXREF pxDirectX, D3DRENDERSTATETYPE State, DWORD Value);
typedef PXResult(PXAPI* PXDirectXRenderStateGet)(PXDirectX PXREF pxDirectX, D3DRENDERSTATETYPE State, DWORD* pValue);

typedef PXResult(PXAPI* PXDirectXClipStatusSet)(PXDirectX PXREF pxDirectX, const D3DCLIPSTATUS9* pClipStatus);
typedef PXResult(PXAPI* PXDirectXClipStatusGet)(PXDirectX PXREF pxDirectX, D3DCLIPSTATUS9* pClipStatus);

typedef PXResult(PXAPI* PXDirectXTextureStageStateGet)(PXDirectX PXREF pxDirectX, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue);
typedef PXResult(PXAPI* PXDirectXTextureStageStateSet)(PXDirectX PXREF pxDirectX, DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value);
typedef PXResult(PXAPI* PXDirectXSamplerStateGet)(PXDirectX PXREF pxDirectX, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue);
typedef PXResult(PXAPI* PXDirectXSamplerStateSet)(PXDirectX PXREF pxDirectX, DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value);
typedef PXResult(PXAPI* PXDirectXDeviceValidate)(PXDirectX PXREF pxDirectX, DWORD* pNumPasses);
typedef PXResult(PXAPI* PXDirectXPaletteEntriesSet)(PXDirectX PXREF pxDirectX, UINT PaletteNumber, const PALETTEENTRY* pEntries);
typedef PXResult(PXAPI* PXDirectXPaletteEntriesGet)(PXDirectX PXREF pxDirectX, UINT PaletteNumber, PALETTEENTRY* pEntries);
typedef PXResult(PXAPI* PXDirectXTexturePaletteCurrentSet)(PXDirectX PXREF pxDirectX, UINT PaletteNumber);
typedef PXResult(PXAPI* PXDirectXTexturePaletteCurrentGet)(PXDirectX PXREF pxDirectX, UINT* PaletteNumber);
typedef PXResult(PXAPI* PXDirectXScissorRectSet)(PXDirectX PXREF pxDirectX, const RECT* pRect);
typedef PXResult(PXAPI* PXDirectXScissorRectGet)(PXDirectX PXREF pxDirectX, RECT* pRect);
typedef PXResult(PXAPI* PXDirectXSoftwareVertexProcessingSet)(PXDirectX PXREF pxDirectX, BOOL bSoftware);
typedef PXResult(PXAPI* PXDirectXSoftwareVertexProcessingGet)(PXDirectX PXREF pxDirectX);
typedef PXResult(PXAPI* PXDirectXNPatchModeSet)(PXDirectX PXREF pxDirectX, PXF32 nSegments);
typedef PXResult(PXAPI* PXDirectXNPatchModeGet)(PXDirectX PXREF pxDirectX);







typedef PXResult(PXAPI* PXDirectXRectPatchDraw)(PXDirectX PXREF pxDirectX, UINT Handle, const PXF32* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo);
typedef PXResult(PXAPI* PXDirectXTriPatchDraw)(PXDirectX PXREF pxDirectX, UINT Handle, const PXF32* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo);
typedef PXResult(PXAPI* PXDirectXPatchDelete)(PXDirectX PXREF pxDirectX, UINT Handle);
typedef PXResult(PXAPI* PXDirectXQueryCreate)(PXDirectX PXREF pxDirectX, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);



//-----------------------------------------------------
// Direct X - Utility
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXInitialize)(PXDirectX PXREF pxDirectX, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
typedef PXResult(PXAPI* PXDirectXRelease)(PXDirectX PXREF pxDirectX);

//-----------------------------------------------------
// Direct X - Scene
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXSceneBegin)(PXDirectX PXREF pxDirectX);
typedef PXResult(PXAPI* PXDirectXSceneEnd)(PXDirectX PXREF pxDirectX);

//-----------------------------------------------------
// Direct X - Fixed Scripting
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXDrawScriptCreate)(PXDirectX PXREF pxDirectX, PXDrawScript PXREF pxDrawScript, const PXDrawScriptType pxDrawScriptType);
typedef PXResult(PXAPI* PXDirectXDrawScriptBegin)(PXDirectX PXREF pxDirectX, PXDrawScript PXREF pxDrawScript);
typedef PXResult(PXAPI* PXDirectXDrawScriptEnd)(PXDirectX PXREF pxDirectX, PXDrawScript PXREF pxDrawScript);
typedef PXResult(PXAPI* PXDirectXDrawScriptDelete)(PXDirectX PXREF pxDirectX, PXDrawScript PXREF pxDrawScript);
typedef PXResult(PXAPI* PXDirectXDrawScriptExecute)(PXDirectX PXREF pxDirectX, PXDrawScript PXREF pxDrawScript);

//-----------------------------------------------------
// Direct X - Draw
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXClear)(PXDirectX PXREF pxDirectX, const PXColorRGBAF PXREF pxColorRGBAF);
typedef PXResult(PXAPI* PXDirectXColorFill)(PXDirectX PXREF pxDirectX, IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color);
typedef PXResult(PXAPI* PXDirectXReset)(PXDirectX PXREF pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef PXResult(PXAPI* PXDirectXSceneDeploy)(PXDirectX PXREF pxDirectX);
typedef PXResult(PXAPI* PXDirectXPrimitiveIndexedDraw)(PXDirectX PXREF pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXI32U BaseVertexIndex, const PXI32U MinVertexIndex, const PXI32U NumVertices, const PXI32U startIndex, const PXI32U primCount);
typedef PXResult(PXAPI* PXDirectXPrimitiveDraw)(PXDirectX PXREF pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXI32U startVertex, const PXI32U primitiveCount);
typedef PXResult(PXAPI* PXDirectXPrimitiveUPDraw)(PXDirectX PXREF pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXI32U PrimitiveCount, const void* pVertexStreamZeroData, const PXI32U VertexStreamZeroStride);
typedef PXResult(PXAPI* PXDirectXPrimitiveUPIndexedDraw)(PXDirectX PXREF pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXI32U MinVertexIndex, const PXI32U NumVertices, const PXI32U PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, const PXI32U VertexStreamZeroStride);


typedef PXResult(PXAPI* PXDirectXVertexFixedFunctionSet)(PXDirectX PXREF pxDirectX, const PXVertexBufferFormat pxVertexBufferFormat);
typedef PXResult(PXAPI* PXDirectXVertexFixedFunctionGet)(PXDirectX PXREF pxDirectX, PXVertexBufferFormat PXREF pxVertexBufferFormat);
typedef PXResult(PXAPI* PXDirectXStreamSourceSet)(PXDirectX PXREF pxDirectX, const PXI32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXI32U OffsetInBytes, const PXI32U Stride);
typedef PXResult(PXAPI* PXDirectXStreamSourceGet)(PXDirectX PXREF pxDirectX, const PXI32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXI32U* pOffsetInBytes, PXI32U* pStride);
typedef PXResult(PXAPI* PXDirectXStreamSourceFreqSet)(PXDirectX PXREF pxDirectX, UINT StreamNumber, UINT Setting);
typedef PXResult(PXAPI* PXDirectXStreamSourceFreqGet)(PXDirectX PXREF pxDirectX, UINT StreamNumber, UINT* pSetting);

typedef PXResult(PXAPI* PXDirectXModelDraw)(PXDirectX PXREF pxDirectX, PXModel PXREF pxModel, const PXCamera PXREF pxCamera);


//-----------------------------------------------------
// Direct X - Light
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXLightSet)(PXDirectX PXREF pxDirectX, PXLight PXREF pxLight, const PXI32U index); // D3DLIGHT9
typedef PXResult(PXAPI* PXDirectXLightGet)(PXDirectX PXREF pxDirectX, PXLight PXREF pxLight, const PXI32U index);
typedef PXResult(PXAPI* PXDirectXLightEnableSet)(PXDirectX PXREF pxDirectX, PXLight PXREF pxLight, const PXI32U index, const PXBool enable);
typedef PXResult(PXAPI* PXDirectXLightEnableGet)(PXDirectX PXREF pxDirectX, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable);


//-----------------------------------------------------
// Direct X - Transform
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXTransformSet)(PXDirectX PXREF pxDirectX, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
typedef PXResult(PXAPI* PXDirectXTransformGet)(PXDirectX PXREF pxDirectX, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
typedef PXResult(PXAPI* PXDirectXTransformMultiply)(PXDirectX PXREF pxDirectX, D3DTRANSFORMSTATETYPE, const D3DMATRIX*);


//-----------------------------------------------------
// Direct X - Viewport
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXViewportSet)(PXDirectX PXREF pxDirectX, const PXViewPort PXREF pxViewPort);
typedef PXResult(PXAPI* PXDirectXViewportGet)(PXDirectX PXREF pxDirectX, PXViewPort PXREF pxViewPort);


//-----------------------------------------------------
// Direct X - Material
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXMaterialSet)(PXDirectX PXREF pxDirectX, const PXMaterial PXREF pxMaterial);
typedef PXResult(PXAPI* PXDirectXMaterialGet)(PXDirectX PXREF pxDirectX, PXMaterial PXREF pxMaterial);


//-----------------------------------------------------
// Direct X - Shader
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXShaderProgramCreateFromFileVF)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath);
typedef PXResult(PXAPI* PXDirectXShaderProgramCreateFromFileVFA)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath);
typedef PXResult(PXAPI* PXDirectXShaderProgramCreateFromStringVF)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath);
typedef PXResult(PXAPI* PXDirectXShaderProgramCreateFromStringVFA)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath);


typedef PXResult(PXAPI* PXDirectXShaderProgramCreate)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram);
typedef PXResult(PXAPI* PXDirectXShaderProgramSelect)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram);
typedef PXResult(PXAPI* PXDirectXShaderProgramDelete)(PXDirectX PXREF pxDirectX, PXShaderProgram PXREF pxShaderProgram);


typedef PXResult(PXAPI* PXDirectXShaderCreate)(PXDirectX PXREF pxDirectX, PXShader PXREF pxShader);
typedef PXResult(PXAPI* PXDirectXShaderSelect)(PXDirectX PXREF pxDirectX, PXShader PXREF pxShader);
typedef PXResult(PXAPI* PXDirectXShaderCompile)(PXDirectX PXREF pxDirectX, PXShader PXREF pxShader, const PXText PXREF shaderFilePath);

typedef PXResult(PXAPI* PXDirectXShaderVariableIDFetch)(PXDirectX PXREF pxDirectX, const PXShader* pxShader, PXI32U PXREF shaderVariableID, const char PXREF name);


typedef PXResult(PXAPI* PXDirectXVertexShaderCreate)(PXDirectX PXREF pxDirectX, const DWORD* pFunction, IDirect3DVertexShader9** ppShader);
typedef PXResult(PXAPI* PXDirectXVertexShaderSet)(PXDirectX PXREF pxDirectX, IDirect3DVertexShader9* pShader);
typedef PXResult(PXAPI* PXDirectXVertexShaderGet)(PXDirectX PXREF pxDirectX, IDirect3DVertexShader9** ppShader);
typedef PXResult(PXAPI* PXDirectXVertexShaderConstantFSet)(PXDirectX PXREF pxDirectX, UINT StartRegister, const PXF32* pConstantData, UINT Vector4fCount);
typedef PXResult(PXAPI* PXDirectXVertexShaderConstantFGet)(PXDirectX PXREF pxDirectX, UINT StartRegister, PXF32* pConstantData, UINT Vector4fCount);
typedef PXResult(PXAPI* PXDirectXVertexShaderConstantISet)(PXDirectX PXREF pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
typedef PXResult(PXAPI* PXDirectXVertexShaderConstantIGet)(PXDirectX PXREF pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
typedef PXResult(PXAPI* PXDirectXVertexShaderConstantBSet)(PXDirectX PXREF pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
typedef PXResult(PXAPI* PXDirectXVertexShaderConstantBGet)(PXDirectX PXREF pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);

typedef PXResult(PXAPI* PXDirectXPixelShaderCreate)(PXDirectX PXREF pxDirectX, const DWORD* pFunction, IDirect3DPixelShader9** ppShader);
typedef PXResult(PXAPI* PXDirectXPixelShaderSet)(PXDirectX PXREF pxDirectX, IDirect3DPixelShader9* pShader);
typedef PXResult(PXAPI* PXDirectXPixelShaderGet)(PXDirectX PXREF pxDirectX, IDirect3DPixelShader9** ppShader);
typedef PXResult(PXAPI* PXDirectXPixelShaderConstantFSet)(PXDirectX PXREF pxDirectX, UINT StartRegister, const PXF32* pConstantData, UINT Vector4fCount);
typedef PXResult(PXAPI* PXDirectXPixelShaderConstantFGet)(PXDirectX PXREF pxDirectX, UINT StartRegister, PXF32* pConstantData, UINT Vector4fCount);
typedef PXResult(PXAPI* PXDirectXPixelShaderConstantISet)(PXDirectX PXREF pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
typedef PXResult(PXAPI* PXDirectXPixelShaderConstantIGet)(PXDirectX PXREF pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
typedef PXResult(PXAPI* PXDirectXPixelShaderConstantBSet)(PXDirectX PXREF pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
typedef PXResult(PXAPI* PXDirectXPixelShaderConstantBGet)(PXDirectX PXREF pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);




//-----------------------------------------------------
// Direct X - Buffers
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXVertexBufferCreate)(PXDirectX PXREF pxDirectX, PXVertexBuffer PXREF pxVertexBuffer);
typedef PXResult(PXAPI* PXDirectXVerticesProcess)(PXDirectX PXREF pxDirectX, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
typedef PXResult(PXAPI* PXDirectXVertexDeclarationCreate)(PXDirectX PXREF pxDirectX, const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
typedef PXResult(PXAPI* PXDirectXVertexDeclarationSet)(PXDirectX PXREF pxDirectX, IDirect3DVertexDeclaration9* pDecl);
typedef PXResult(PXAPI* PXDirectXVertexDeclarationGet)(PXDirectX PXREF pxDirectX, IDirect3DVertexDeclaration9** ppDecl);

typedef PXResult(PXAPI* PXDirectXIndexBufferCreate)(PXDirectX PXREF pxDirectX, PXIndexBuffer PXREF pxIndexBuffer);
typedef PXResult(PXAPI* PXDirectXIndicesSet)(PXDirectX PXREF pxDirectX, PXIndexBuffer PXREF pxIndexBuffer);
typedef PXResult(PXAPI* PXDirectXIndicesGet)(PXDirectX PXREF pxDirectX, PXIndexBuffer** pxIndexBuffer);

typedef PXResult(PXAPI* PXDirectXRenderTargetCreate)(PXDirectX PXREF pxDirectX, PXRenderTarget PXREF pxRenderTarget);
typedef PXResult(PXAPI* PXDirectXRenderTargetSet)(PXDirectX PXREF pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
typedef PXResult(PXAPI* PXDirectXRenderTargetGet)(PXDirectX PXREF pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);

typedef PXResult(PXAPI* PXDirectXDepthStencilSurfaceCreate)(PXDirectX PXREF pxDirectX, PXDepthStencilSurface PXREF pxDepthStencilSurface);
typedef PXResult(PXAPI* PXDirectXDepthStencilSurfaceSet)(PXDirectX PXREF pxDirectX, IDirect3DSurface9* pNewZStencil);
typedef PXResult(PXAPI* PXDirectXDepthStencilSurfaceGet)(PXDirectX PXREF pxDirectX, IDirect3DSurface9** ppZStencilSurface);

typedef PXResult(PXAPI* PXDirectXOffscreenPlainSurfaceCreate)(PXDirectX PXREF pxDirectX, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);

typedef PXResult(PXAPI* PXDirectXModelRegister)(PXDirectX PXREF pxDirectX, PXModel PXREF pxModel);

//-----------------------------------------------------
// Direct X - Textures
//-----------------------------------------------------
typedef PXResult(PXAPI* PXDirectXTextureMemoryAvailable)(PXDirectX PXREF pxDirectX, PXI32U PXREF value);
typedef PXResult(PXAPI* PXDirectXTexture2DCreate)(PXDirectX PXREF pxDirectX, PXTexture PXREF PXTexture);
typedef PXResult(PXAPI* PXDirectXTexture3DCreate)(PXDirectX PXREF pxDirectX, PXTexture PXREF pxTexture);
typedef PXResult(PXAPI* PXDirectXTextureCubeCreate)(PXDirectX PXREF pxDirectX, PXTexture PXREF pxTexture);
typedef PXResult(PXAPI* PXDirectXDevicePhysicalListAmountFunction)(PXDirectX PXREF pxDirectX, PXI32U PXREF amountOfAdapters);
typedef PXResult(PXAPI* PXDirectXDevicePhysicalListFetchFunction)(PXDirectX PXREF pxDirectX, const PXI32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical PXREF pxGraphicDevicePhysicalList);
typedef PXResult(PXAPI* PXDirectXSwapIntervalSet)(PXDirectX PXREF pxDirectX, const PXI32U interval);
typedef PXResult(PXAPI* PXDirectXSwapIntervalGet)(PXDirectX PXREF pxDirectX, PXI32U PXREF interval);

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

PXPublic PXResult PXAPI PXDirectXInitialize(PXDirectX PXREF pxDirectX, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectXRelease(PXDirectX PXREF pxDirectX);

#endif

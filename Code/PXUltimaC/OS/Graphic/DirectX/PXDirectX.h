#ifndef PXDirectXINCLUDE
#define PXDirectXINCLUDE

#include <Media/PXResource.h>

//-------------------------------------
// DirectX - Settings
//-------------------------------------
#define PXDX9Enable OSWindows && 1u
#define PXDX10Enable OSWindows && 0u
#define PXDX10x1Enable OSWindows && 0u
#define PXDX11Enable OSWindows && 0u
#define PXDX12Enable OSWindows && 0u

#define PXDX10Atleast PXDX10Enable || PXDX10x1Enable || PXDX11Enable || PXDX12Enable

//-------------------------------------

#if OSUnix
// UNIX has no DirectX support
typedef unsigned int  IDirect3D9; // Dummy value
#define MAX_DEVICE_IDENTIFIER_STRING 32

typedef void* D3DMATERIAL9;

#elif OSWindows

#if PXDX12Enable
#include <d3d12.h>
#endif

#if PXDX11Enable
#include <d3d11.h>
#endif

#if PXDX10Enable
#include <d3d10.h>
#endif

#if PXDX9Enable
#include <d3d9.h>
#endif

#endif


#ifdef __cplusplus
extern "C"
{
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

	typedef struct PXDirectX
	{
        PXDirectXVersion DirectXVersion;

#if OSWindows && PXDX10Atleast // Direct X - v.10+
        IDXGIAdapter VideoAdapter;
#endif

#if OSWindows && PXDX12Enable   // Direct X - v.12
        ID3D12Device* DX12;
#endif

#if OSWindows && PXDX11Enable      // Direct X - v.11.0
        ID3D11Device* DX11;
        ID3D11DeviceContext* DX11Context;
#endif

#if OSWindows && PXDX10x1Enable   // Direct X - v.10.1
        ID3D10Device1* DX10X1;
#endif

#if OSWindows && PXDX10Enable   // Direct X - v.10.0
        ID3D10Device* DX10;
#endif

#if OSWindows && PXDX9Enable  // Direct X - v.9.x
        D3DCAPS9 DeviceCapabilitiesCurrent;
        IDirect3D9* DX9Context;
        IDirect3DDevice9* DX9;
#endif
	}
	PXDirectX;

#if OSWindows // TODO: Temp fix, cant compile

    PXPublic PXActionResult PXDirectCooperativeLevelTest(PXDirectX* const pxDirectX);

    PXPublic PXActionResult PXDirectXManagedResourcesEvict(PXDirectX* const pxDirectX);

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







    PXPublic PXActionResult PXAPI PXDirectXRectPatchDraw(PXDirectX* const pxDirectX,UINT Handle, const float* pNumSegs, const D3DRECTPATCH_INFO* pRectPatchInfo);
    PXPublic PXActionResult PXAPI PXDirectXTriPatchDraw(PXDirectX* const pxDirectX,UINT Handle, const float* pNumSegs, const D3DTRIPATCH_INFO* pTriPatchInfo);
    PXPublic PXActionResult PXAPI PXDirectXPatchDelete(PXDirectX* const pxDirectX,UINT Handle);
    PXPublic PXActionResult PXAPI PXDirectXQueryCreate(PXDirectX* const pxDirectX,D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);



    //-----------------------------------------------------
    // Direct X - Magic number conversion
    //-----------------------------------------------------
    PXPrivate PXColorFormat PXAPI PXDirectXColorFormatToID(const D3DFORMAT format);
    PXPrivate D3DFORMAT PXAPI PXDirectXColorFormatFromID(const PXColorFormat pxColorFormat);
    PXPrivate void PXAPI PXDirectXMaterialToPXMaterial(PXMaterial* const pxMaterial, const D3DMATERIAL9* const d3dMaterial);
    PXPrivate void PXAPI PXDirectXMaterialFromPXMaterial(D3DMATERIAL9* const d3dMaterial, const PXMaterial* const pxMaterial);
    PXPrivate D3DPRIMITIVETYPE PXAPI PXDirectXDrawTypeFromPX(const PXGraphicDrawMode PXGraphicDrawMode);
    PXPrivate PXInt32U PXAPI PXDirectXVertexFormatFromPXVertexBufferFormat(const PXVertexBufferFormat pxVertexBufferFormat);

    //-----------------------------------------------------
    // Direct X - Utility
    //-----------------------------------------------------
    PXPublic void PXAPI PXDirectXContextConstruct(PXDirectX* const pxDirectX);
    PXPublic void PXAPI PXDirectXContextDestruct(PXDirectX* const pxDirectX);

    PXPublic PXActionResult PXAPI PXDirectXInitialize(PXDirectX* const pxDirectX, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
    PXPublic PXActionResult PXAPI PXDirectXRelease(PXDirectX* const pxDirectX);

    //-----------------------------------------------------
    // Direct X - Scene
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXSceneBegin(PXDirectX* const pxDirectX);
    PXPublic PXActionResult PXAPI PXDirectXSceneEnd(PXDirectX* const pxDirectX);

    //-----------------------------------------------------
    // Direct X - Fixed Scripting
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXDrawScriptCreate(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
    PXPublic PXActionResult PXAPI PXDirectXDrawScriptBegin(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);
    PXPublic PXActionResult PXAPI PXDirectXDrawScriptEnd(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);
    PXPublic PXActionResult PXAPI PXDirectXDrawScriptDelete(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);
    PXPublic PXActionResult PXAPI PXDirectXDrawScriptExecute(PXDirectX* const pxDirectX, PXDrawScript* const pxDrawScript);

    //-----------------------------------------------------
    // Direct X - Draw
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXClear(PXDirectX* const pxDirectX, const PXColorRGBAF* const pxColorRGBAF);
    PXPublic PXActionResult PXAPI PXDirectXColorFill(PXDirectX* const pxDirectX, IDirect3DSurface9* pSurface, const RECT* pRect, D3DCOLOR color);
    PXPublic PXActionResult PXAPI PXDirectXReset(PXDirectX* const pxDirectX, D3DPRESENT_PARAMETERS* pPresentationParameters);
    PXPublic PXActionResult PXAPI PXDirectXSceneDeploy(PXDirectX* const pxDirectX);
    PXPublic PXActionResult PXAPI PXDirectXPrimitiveIndexedDraw(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U BaseVertexIndex, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U startIndex, const PXInt32U primCount);
    PXPublic PXActionResult PXAPI PXDirectXPrimitiveDraw(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U startVertex, const PXInt32U primitiveCount);
    PXPublic PXActionResult PXAPI PXDirectXPrimitiveUPDraw(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U PrimitiveCount, const void* pVertexStreamZeroData, const PXInt32U VertexStreamZeroStride);
    PXPublic PXActionResult PXAPI PXDirectXPrimitiveUPIndexedDraw(PXDirectX* const pxDirectX, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U MinVertexIndex, const PXInt32U NumVertices, const PXInt32U PrimitiveCount, const void* pIndexData, D3DFORMAT IndexDataFormat, const void* pVertexStreamZeroData, const PXInt32U VertexStreamZeroStride);


    PXPublic PXActionResult PXAPI PXDirectXVertexFixedFunctionSet(PXDirectX* const pxDirectX, const PXVertexBufferFormat pxVertexBufferFormat);
    PXPublic PXActionResult PXAPI PXDirectXVertexFixedFunctionGet(PXDirectX* const pxDirectX, PXVertexBufferFormat* const pxVertexBufferFormat);
    PXPublic PXActionResult PXAPI PXDirectXStreamSourceSet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride);
    PXPublic PXActionResult PXAPI PXDirectXStreamSourceGet(PXDirectX* const pxDirectX, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride);
    PXPublic PXActionResult PXAPI PXDirectXStreamSourceFreqSet(PXDirectX* const pxDirectX, UINT StreamNumber, UINT Setting);
    PXPublic PXActionResult PXAPI PXDirectXStreamSourceFreqGet(PXDirectX* const pxDirectX, UINT StreamNumber, UINT* pSetting);

    PXPublic PXActionResult PXAPI PXDirectXVertexStructureDraw(PXDirectX* const pxDirectX, PXVertexStructure* const pxVertexStructure, const PXCamera* const pxCamera);


    //-----------------------------------------------------
    // Direct X - Light
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXLightSet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index); // D3DLIGHT9
    PXPublic PXActionResult PXAPI PXDirectXLightGet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index);
    PXPublic PXActionResult PXAPI PXDirectXLightEnableSet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
    PXPublic PXActionResult PXAPI PXDirectXLightEnableGet(PXDirectX* const pxDirectX, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);


    //-----------------------------------------------------
    // Direct X - Transform
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXTransformSet(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
    PXPublic PXActionResult PXAPI PXDirectXTransformGet(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
    PXPublic PXActionResult PXAPI PXDirectXTransformMultiply(PXDirectX* const pxDirectX, D3DTRANSFORMSTATETYPE, const D3DMATRIX*);


    //-----------------------------------------------------
    // Direct X - Viewport
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXViewportSet(PXDirectX* const pxDirectX, const PXViewPort* const pxViewPort);
    PXPublic PXActionResult PXAPI PXDirectXViewportGet(PXDirectX* const pxDirectX, PXViewPort* const pxViewPort);


    //-----------------------------------------------------
    // Direct X - Material
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXMaterialSet(PXDirectX* const pxDirectX, const PXMaterial* const pxMaterial);
    PXPublic PXActionResult PXAPI PXDirectXMaterialGet(PXDirectX* const pxDirectX, PXMaterial* const pxMaterial);


    //-----------------------------------------------------
    // Direct X - Shader
    //-----------------------------------------------------

    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateVP(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const PXText* const vertexShader, const PXText* const pixelShader);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateVPA(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShader, const char* const pixelShader);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateVPW(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const wchar_t* const vertexShader, const wchar_t* const pixelShader);

    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateFromFileVF(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateFromFileVFA(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateFromStringVF(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreateFromStringVFA(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);


    PXPublic PXActionResult PXAPI PXDirectXShaderProgramCreate(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramSelect(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram);
    PXPublic PXActionResult PXAPI PXDirectXShaderProgramDelete(PXDirectX* const pxDirectX, PXShaderProgram* const pxShaderProgram);


    PXPublic PXActionResult PXAPI PXDirectXShaderCreate(PXDirectX* const pxDirectX, PXShader* const pxShader);
    PXPublic PXActionResult PXAPI PXDirectXShaderSelect(PXDirectX* const pxDirectX, PXShader* const pxShader);
    PXPublic PXActionResult PXAPI PXDirectXShaderCompile(PXDirectX* const pxDirectX, PXShader* const pxShader, const PXText* const shaderFilePath);

    PXPublic PXActionResult PXAPI PXDirectXShaderVariableIDFetch(PXDirectX* const pxDirectX, const PXShader* pxShader, PXInt32U* const shaderVariableID, const char* const name);


    PXPublic PXActionResult PXAPI PXDirectXVertexShaderCreate(PXDirectX* const pxDirectX, const DWORD* pFunction, IDirect3DVertexShader9** ppShader);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderSet(PXDirectX* const pxDirectX, IDirect3DVertexShader9* pShader);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderGet(PXDirectX* const pxDirectX, IDirect3DVertexShader9** ppShader);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderConstantFSet(PXDirectX* const pxDirectX, UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderConstantFGet(PXDirectX* const pxDirectX, UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderConstantISet(PXDirectX* const pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderConstantIGet(PXDirectX* const pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderConstantBSet(PXDirectX* const pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
    PXPublic PXActionResult PXAPI PXDirectXVertexShaderConstantBGet(PXDirectX* const pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);

    PXPublic PXActionResult PXAPI PXDirectXPixelShaderCreate(PXDirectX* const pxDirectX, const DWORD* pFunction, IDirect3DPixelShader9** ppShader);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderSet(PXDirectX* const pxDirectX, IDirect3DPixelShader9* pShader);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderGet(PXDirectX* const pxDirectX, IDirect3DPixelShader9** ppShader);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderConstantFSet(PXDirectX* const pxDirectX, UINT StartRegister, const float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderConstantFGet(PXDirectX* const pxDirectX, UINT StartRegister, float* pConstantData, UINT Vector4fCount);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderConstantISet(PXDirectX* const pxDirectX, UINT StartRegister, const int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderConstantIGet(PXDirectX* const pxDirectX, UINT StartRegister, int* pConstantData, UINT Vector4iCount);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderConstantBSet(PXDirectX* const pxDirectX, UINT StartRegister, const BOOL* pConstantData, UINT  BoolCount);
    PXPublic PXActionResult PXAPI PXDirectXPixelShaderConstantBGet(PXDirectX* const pxDirectX, UINT StartRegister, BOOL* pConstantData, UINT BoolCount);




    //-----------------------------------------------------
    // Direct X - Buffers
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXVertexBufferCreate(PXDirectX* const pxDirectX, PXVertexBuffer* const pxVertexBuffer);
    PXPublic PXActionResult PXAPI PXDirectXVerticesProcess(PXDirectX* const pxDirectX, UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags);
    PXPublic PXActionResult PXAPI PXDirectXVertexDeclarationCreate(PXDirectX* const pxDirectX, const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);
    PXPublic PXActionResult PXAPI PXDirectXVertexDeclarationSet(PXDirectX* const pxDirectX, IDirect3DVertexDeclaration9* pDecl);
    PXPublic PXActionResult PXAPI PXDirectXVertexDeclarationGet(PXDirectX* const pxDirectX, IDirect3DVertexDeclaration9** ppDecl);

    PXPublic PXActionResult PXAPI PXDirectXIndexBufferCreate(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer);
    PXPublic PXActionResult PXAPI PXDirectXIndicesSet(PXDirectX* const pxDirectX, PXIndexBuffer* const pxIndexBuffer);
    PXPublic PXActionResult PXAPI PXDirectXIndicesGet(PXDirectX* const pxDirectX, PXIndexBuffer** pxIndexBuffer);

    PXPublic PXActionResult PXAPI PXDirectXRenderTargetCreate(PXDirectX* const pxDirectX, PXRenderTarget* const pxRenderTarget);
    PXPublic PXActionResult PXAPI PXDirectXRenderTargetSet(PXDirectX* const pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget);
    PXPublic PXActionResult PXAPI PXDirectXRenderTargetGet(PXDirectX* const pxDirectX, DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget);

    PXPublic PXActionResult PXAPI PXDirectXDepthStencilSurfaceCreate(PXDirectX* const pxDirectX, PXDepthStencilSurface* const pxDepthStencilSurface);
    PXPublic PXActionResult PXAPI PXDirectXDepthStencilSurfaceSet(PXDirectX* const pxDirectX, IDirect3DSurface9* pNewZStencil);
    PXPublic PXActionResult PXAPI PXDirectXDepthStencilSurfaceGet(PXDirectX* const pxDirectX, IDirect3DSurface9** ppZStencilSurface);

    PXPublic PXActionResult PXAPI PXDirectXOffscreenPlainSurfaceCreate(PXDirectX* const pxDirectX, UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);

    PXPublic PXActionResult PXAPI PXDirectXVertexStructureRegister(PXDirectX* const pxDirectX, PXVertexStructure* const pxVertexStructure);

    //-----------------------------------------------------
    // Direct X - Textures
    //-----------------------------------------------------
    PXPublic PXActionResult PXAPI PXDirectXTextureMemoryAvailable(PXDirectX* const pxDirectX, PXInt32U* const value);
   // PXPublic PXActionResult PXDirectXTextureGet(PXDirectX* const pxDirectX, DWORD Stage, IDirect3DBaseTexture9** ppTexture);
   // PXPublic PXActionResult PXDirectXTextureSet(PXDirectX* const pxDirectX, DWORD Stage, IDirect3DBaseTexture9* pTexture);
	PXPublic PXActionResult PXAPI PXDirectXTexture2DCreate(PXDirectX* const pxDirectX, PXTexture2D* const pxTexture2D);
	PXPublic PXActionResult PXAPI PXDirectXTexture3DCreate(PXDirectX* const pxDirectX, PXTexture3D* const pxTexture3D);
	PXPublic PXActionResult PXAPI PXDirectXTextureCubeCreate(PXDirectX* const pxDirectX, PXTextureCube* const pxTextureCube);



    PXPublic PXActionResult PXAPI PXDirectXDevicePhysicalListAmountFunction(PXDirectX* const pxDirectX, PXInt32U* const amountOfAdapters);
    PXPublic PXActionResult PXAPI PXDirectXDevicePhysicalListFetchFunction(PXDirectX* const pxDirectX, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);


#endif


#ifdef __cplusplus
}
#endif

#endif
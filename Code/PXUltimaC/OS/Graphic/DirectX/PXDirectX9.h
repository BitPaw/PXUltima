#ifndef PXDirectX9INCLUDE
#define PXDirectX9INCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXActionResult PXAPI PXDirectX9Initialize(PXDirectX9* const pxDirectX9, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXActionResult PXAPI PXDirectX9Release(PXDirectX9* const pxDirectX9);

PXPublic void PXAPI PXDirectX9Select(PXDirectX9* const pxDirectX9);
PXPublic void PXAPI PXDirectX9Deselect(PXDirectX9* const pxDirectX9);

PXPublic PXActionResult PXAPI PXDirectX9TextureAction(PXDirectX9* const pxDirectX9, PXGraphicTexturInfo* const pxGraphicTexturInfo);

PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramCreate(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount);
PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramSelect(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram);
PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram);

PXPublic PXActionResult PXAPI PXDirectX9ShaderVariableSet(PXDirectX9* const pxDirectX9, const PXShaderProgram* const pxShaderProgram, PXShaderVariable* const pxShaderVariable);

PXPublic PXActionResult PXAPI PXDirectX9SceneBegin(PXDirectX9* const pxDirectX9);
PXPublic PXActionResult PXAPI PXDirectX9SceneEnd(PXDirectX9* const pxDirectX9);

PXPublic PXActionResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
PXPublic PXActionResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);

PXPublic PXActionResult PXAPI PXDirectX9ViewportSet(PXDirectX9* const pxDirectX9, const PXViewPort* const pxViewPort);
PXPublic PXActionResult PXAPI PXDirectX9ViewportGet(PXDirectX9* const pxDirectX9, PXViewPort* const pxViewPort);

PXPublic PXActionResult PXAPI PXDirectX9MaterialSet(PXDirectX9* const pxDirectX9, const PXMaterial* const pxMaterial);
PXPublic PXActionResult PXAPI PXDirectX9MaterialGet(PXDirectX9* const pxDirectX9, PXMaterial* const pxMaterial);

PXPublic PXActionResult PXAPI PXDirectX9SwapIntervalSet(PXDirectX9* const pxDirectX9, const PXInt32U interval);
PXPublic PXActionResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9* const pxDirectX9, PXInt32U* const interval);


PXPublic PXActionResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9* const pxDirectX9, PXInt32U* const value);
PXPublic PXActionResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9* const pxDirectX9, PXInt32U* const amountOfAdapters);
PXPublic PXActionResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9* const pxDirectX9, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);


//-----------------------------------------------------
// Direct X - Draw
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectX9Clear(PXDirectX9* const pxDirectX9, const PXColorRGBAF* const pxColorRGBAF);
PXPublic PXActionResult PXAPI PXDirectX9SceneDeploy(PXDirectX9* const pxDirectX9);
PXPublic PXActionResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9* const pxDirectX9, const PXGraphicDrawMode PXGraphicDrawMode, const PXInt32U startVertex, const PXInt32U primitiveCount);
PXPublic PXActionResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9* const pxDirectX9, const PXVertexBufferFormat pxVertexBufferFormat);
PXPublic PXActionResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9* const pxDirectX9, PXVertexBufferFormat* const pxVertexBufferFormat);
PXPublic PXActionResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9* const pxDirectX9, const PXInt32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXInt32U OffsetInBytes, const PXInt32U Stride);
PXPublic PXActionResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9* const pxDirectX9, const PXInt32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXInt32U* pOffsetInBytes, PXInt32U* pStride);
PXPublic PXActionResult PXAPI PXDirectX9ModelDraw(PXDirectX9* const pxDirectX9, PXModel* const pxModel, const PXCamera* const pxCamera);


//-----------------------------------------------------
// Direct X - Light
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectX9LightSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index); // D3DLIGHT9
PXPublic PXActionResult PXAPI PXDirectX9LightGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index);
PXPublic PXActionResult PXAPI PXDirectX9LightEnableSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
PXPublic PXActionResult PXAPI PXDirectX9LightEnableGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);


//-----------------------------------------------------
// Direct X - Shader
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
PXPublic PXActionResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
PXPublic PXActionResult PXAPI PXDirectX9ShaderCreate(PXDirectX9* const pxDirectX9, PXShader* const pxShader);
PXPublic PXActionResult PXAPI PXDirectX9ShaderSelect(PXDirectX9* const pxDirectX9, PXShader* const pxShader);
PXPublic PXActionResult PXAPI PXDirectX9ShaderCompile(PXDirectX9* const pxDirectX9, PXShader* const pxShader, const PXText* const shaderFilePath);
PXPublic PXActionResult PXAPI PXDirectX9ShaderVariableIDFetch(PXDirectX9* const pxDirectX9, const PXShaderProgram* const pxShaderProgram, PXInt32U* const shaderVariableID, const char* const name);


//-----------------------------------------------------
// Direct X - Buffers
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9* const pxDirectX9, PXVertexBuffer* const pxVertexBuffer);
PXPublic PXActionResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9* const pxDirectX9, PXIndexBuffer* const pxIndexBuffer);
PXPublic PXActionResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9* const pxDirectX9, PXRenderTarget* const pxRenderTarget);
PXPublic PXActionResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9* const pxDirectX9, PXDepthStencilSurface* const pxDepthStencilSurface);
PXPublic PXActionResult PXAPI PXDirectX9ModelRegister(PXDirectX9* const pxDirectX9, PXModel* const pxModel);

#endif
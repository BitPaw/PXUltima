#ifndef PXDirectX9Included
#define PXDirectX9Included

#include <PX/OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXResult PXAPI PXDirectX9Initialize(PXDirectX9* const pxDirectX9, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectX9Release(PXDirectX9* const pxDirectX9);

PXPublic void PXAPI PXDirectX9Select(PXDirectX9* const pxDirectX9);
PXPublic void PXAPI PXDirectX9Deselect(PXDirectX9* const pxDirectX9);

PXPublic PXResult PXAPI PXDirectX9TextureAction(PXDirectX9* const pxDirectX9, PXTexturInfo* const pxGraphicTexturInfo);

PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreate(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramSelect(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram);

PXPublic PXResult PXAPI PXDirectX9ShaderVariableSet(PXDirectX9* const pxDirectX9, const PXShaderProgram* const pxShaderProgram, PXShaderVariable* const pxShaderVariable);

PXPublic PXResult PXAPI PXDirectX9SceneBegin(PXDirectX9* const pxDirectX9);
PXPublic PXResult PXAPI PXDirectX9SceneEnd(PXDirectX9* const pxDirectX9);

PXPublic PXResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript, const PXDrawScriptType pxDrawScriptType);
PXPublic PXResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);
PXPublic PXResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);
PXPublic PXResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);
PXPublic PXResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9* const pxDirectX9, PXDrawScript* const pxDrawScript);

PXPublic PXResult PXAPI PXDirectX9ViewportSet(PXDirectX9* const pxDirectX9, const PXViewPort* const pxViewPort);
PXPublic PXResult PXAPI PXDirectX9ViewportGet(PXDirectX9* const pxDirectX9, PXViewPort* const pxViewPort);

PXPublic PXResult PXAPI PXDirectX9MaterialSet(PXDirectX9* const pxDirectX9, const PXMaterial* const pxMaterial);
PXPublic PXResult PXAPI PXDirectX9MaterialGet(PXDirectX9* const pxDirectX9, PXMaterial* const pxMaterial);

PXPublic PXResult PXAPI PXDirectX9SwapIntervalSet(PXDirectX9* const pxDirectX9, const PXI32U interval);
PXPublic PXResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9* const pxDirectX9, PXI32U* const interval);


PXPublic PXResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9* const pxDirectX9, PXI32U* const value);
PXPublic PXResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9* const pxDirectX9, PXI32U* const amountOfAdapters);
PXPublic PXResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9* const pxDirectX9, const PXI32U pxGraphicDevicePhysicalListSize, PXGPUPhysical* const pxGraphicDevicePhysicalList);


//-----------------------------------------------------
// Direct X - Draw
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9Clear(PXDirectX9* const pxDirectX9, const PXColorRGBAF* const pxColorRGBAF);
PXPublic PXResult PXAPI PXDirectX9SceneDeploy(PXDirectX9* const pxDirectX9);
PXPublic PXResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9* const pxDirectX9, const PXDrawMode PXGraphicDrawMode, const PXI32U startVertex, const PXI32U primitiveCount);
PXPublic PXResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9* const pxDirectX9, const void* pxVertexBufferFormat);
PXPublic PXResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9* const pxDirectX9, void** const pxVertexBufferFormat);
PXPublic PXResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9* const pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXI32U OffsetInBytes, const PXI32U Stride);
PXPublic PXResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9* const pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXI32U* pOffsetInBytes, PXI32U* pStride);
PXPublic PXResult PXAPI PXDirectX9ModelDraw(PXDirectX9* const pxDirectX9, PXModel* const pxModel, const PXCamera* const pxCamera);


//-----------------------------------------------------
// Direct X - Light
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9LightSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXI32U index); // D3DLIGHT9
PXPublic PXResult PXAPI PXDirectX9LightGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXI32U index);
PXPublic PXResult PXAPI PXDirectX9LightEnableSet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXI32U index, const PXBool enable);
PXPublic PXResult PXAPI PXDirectX9LightEnableGet(PXDirectX9* const pxDirectX9, PXLight* const pxLight, const PXI32U index, PXBool* const enable);


//-----------------------------------------------------
// Direct X - Shader
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, PXText* const vertexShaderFilePath, PXText* const fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9* const pxDirectX9, PXShaderProgram* const pxShaderProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderCreate(PXDirectX9* const pxDirectX9, PXShader* const pxShader);
PXPublic PXResult PXAPI PXDirectX9ShaderSelect(PXDirectX9* const pxDirectX9, PXShader* const pxShader);
PXPublic PXResult PXAPI PXDirectX9ShaderCompile(PXDirectX9* const pxDirectX9, PXShader* const pxShader, const PXText* const shaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderVariableIDFetch(PXDirectX9* const pxDirectX9, const PXShaderProgram* const pxShaderProgram, PXI32U* const shaderVariableID, const char* const name);


//-----------------------------------------------------
// Direct X - Buffers
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9* const pxDirectX9, PXVertexBuffer* const pxVertexBuffer);
PXPublic PXResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9* const pxDirectX9, PXIndexBuffer* const pxIndexBuffer);
PXPublic PXResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9* const pxDirectX9, PXRenderTarget* const pxRenderTarget);
PXPublic PXResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9* const pxDirectX9, PXDepthStencilSurface* const pxDepthStencilSurface);
PXPublic PXResult PXAPI PXDirectX9ModelRegister(PXDirectX9* const pxDirectX9, PXModel* const pxModel);

#endif

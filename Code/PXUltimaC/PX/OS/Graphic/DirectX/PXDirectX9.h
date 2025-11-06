#pragma once

#ifndef PXDirectX9Included
#define PXDirectX9Included

#include <PX/OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXResult PXAPI PXDirectX9Initialize(PXDirectX9 PXREF pxDirectX9, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectX9Release(PXDirectX9 PXREF pxDirectX9);

PXPublic void PXAPI PXDirectX9Select(PXDirectX9 PXREF pxDirectX9);
PXPublic void PXAPI PXDirectX9Deselect(PXDirectX9 PXREF pxDirectX9);

PXPublic PXResult PXAPI PXDirectX9TextureAction(PXDirectX9 PXREF pxDirectX9, PXTexturInfo PXREF pxGraphicTexturInfo);

PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreate(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXShader PXREF shaderList, const PXSize amount);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramSelect(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramDelete(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram);

PXPublic PXResult PXAPI PXDirectX9ShaderVariableSet(PXDirectX9 PXREF pxDirectX9, const PXShaderProgram PXREF pxShaderProgram, PXShaderVariable PXREF pxShaderVariable);

PXPublic PXResult PXAPI PXDirectX9SceneBegin(PXDirectX9 PXREF pxDirectX9);
PXPublic PXResult PXAPI PXDirectX9SceneEnd(PXDirectX9 PXREF pxDirectX9);

PXPublic PXResult PXAPI PXDirectX9DrawScriptCreate(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript, const PXDrawScriptType pxDrawScriptType);
PXPublic PXResult PXAPI PXDirectX9DrawScriptBegin(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript);
PXPublic PXResult PXAPI PXDirectX9DrawScriptEnd(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript);
PXPublic PXResult PXAPI PXDirectX9DrawScriptDelete(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript);
PXPublic PXResult PXAPI PXDirectX9DrawScriptExecute(PXDirectX9 PXREF pxDirectX9, PXDrawScript PXREF pxDrawScript);

PXPublic PXResult PXAPI PXDirectX9ViewportSet(PXDirectX9 PXREF pxDirectX9, const PXViewPort PXREF pxViewPort);
PXPublic PXResult PXAPI PXDirectX9ViewportGet(PXDirectX9 PXREF pxDirectX9, PXViewPort PXREF pxViewPort);

PXPublic PXResult PXAPI PXDirectX9MaterialSet(PXDirectX9 PXREF pxDirectX9, const PXMaterial PXREF pxMaterial);
PXPublic PXResult PXAPI PXDirectX9MaterialGet(PXDirectX9 PXREF pxDirectX9, PXMaterial PXREF pxMaterial);

PXPublic PXResult PXAPI PXDirectX9SwapIntervalSet(PXDirectX9 PXREF pxDirectX9, const PXI32U interval);
PXPublic PXResult PXAPI PXDirectX9SwapIntervalGet(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF interval);


PXPublic PXResult PXAPI PXDirectX9TextureMemoryAvailable(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF value);
PXPublic PXResult PXAPI PXDirectX9DevicePhysicalListAmountFunction(PXDirectX9 PXREF pxDirectX9, PXI32U PXREF amountOfAdapters);
PXPublic PXResult PXAPI PXDirectX9DevicePhysicalListFetchFunction(PXDirectX9 PXREF pxDirectX9, const PXI32U pxGraphicDevicePhysicalListSize, PXGPUPhysical PXREF pxGraphicDevicePhysicalList);


//-----------------------------------------------------
// Direct X - Draw
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9Clear(PXDirectX9 PXREF pxDirectX9, const PXColorRGBAF PXREF pxColorRGBAF);
PXPublic PXResult PXAPI PXDirectX9SceneDeploy(PXDirectX9 PXREF pxDirectX9);
PXPublic PXResult PXAPI PXDirectX9PrimitiveDraw(PXDirectX9 PXREF pxDirectX9, const PXDrawMode PXGraphicDrawMode, const PXI32U startVertex, const PXI32U primitiveCount);
PXPublic PXResult PXAPI PXDirectX9VertexFixedFunctionSet(PXDirectX9 PXREF pxDirectX9, const void* pxVertexBufferFormat);
PXPublic PXResult PXAPI PXDirectX9VertexFixedFunctionGet(PXDirectX9 PXREF pxDirectX9, void* PXREF pxVertexBufferFormat);
PXPublic PXResult PXAPI PXDirectX9StreamSourceSet(PXDirectX9 PXREF pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer* pxVertexBuffer, const PXI32U OffsetInBytes, const PXI32U Stride);
PXPublic PXResult PXAPI PXDirectX9StreamSourceGet(PXDirectX9 PXREF pxDirectX9, const PXI32U StreamNumber, PXVertexBuffer** pxVertexBuffer, PXI32U* pOffsetInBytes, PXI32U* pStride);
PXPublic PXResult PXAPI PXDirectX9ModelDraw(PXDirectX9 PXREF pxDirectX9, PXModel PXREF pxModel, const PXCamera PXREF pxCamera);


//-----------------------------------------------------
// Direct X - Light
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9LightSet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index); // D3DLIGHT9
PXPublic PXResult PXAPI PXDirectX9LightGet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index);
PXPublic PXResult PXAPI PXDirectX9LightEnableSet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index, const PXBool enable);
PXPublic PXResult PXAPI PXDirectX9LightEnableGet(PXDirectX9 PXREF pxDirectX9, PXLight PXREF pxLight, const PXI32U index, PXBool PXREF enable);


//-----------------------------------------------------
// Direct X - Shader
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVF(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromFileVFA(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVF(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, PXText PXREF vertexShaderFilePath, PXText PXREF fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderProgramCreateFromStringVFA(PXDirectX9 PXREF pxDirectX9, PXShaderProgram PXREF pxShaderProgram, const char PXREF vertexShaderFilePath, const char PXREF fragmentShaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderCreate(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader);
PXPublic PXResult PXAPI PXDirectX9ShaderSelect(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader);
PXPublic PXResult PXAPI PXDirectX9ShaderCompile(PXDirectX9 PXREF pxDirectX9, PXShader PXREF pxShader, const PXText PXREF shaderFilePath);
PXPublic PXResult PXAPI PXDirectX9ShaderVariableIDFetch(PXDirectX9 PXREF pxDirectX9, const PXShaderProgram PXREF pxShaderProgram, PXI32U PXREF shaderVariableID, const char PXREF name);


//-----------------------------------------------------
// Direct X - Buffers
//-----------------------------------------------------
PXPublic PXResult PXAPI PXDirectX9VertexBufferCreate(PXDirectX9 PXREF pxDirectX9, PXVertexBuffer PXREF pxVertexBuffer);
PXPublic PXResult PXAPI PXDirectX9IndexBufferCreate(PXDirectX9 PXREF pxDirectX9, PXIndexBuffer PXREF pxIndexBuffer);
PXPublic PXResult PXAPI PXDirectX9RenderTargetCreate(PXDirectX9 PXREF pxDirectX9, PXRenderTarget PXREF pxRenderTarget);
PXPublic PXResult PXAPI PXDirectX9DepthStencilSurfaceCreate(PXDirectX9 PXREF pxDirectX9, PXDepthStencilSurface PXREF pxDepthStencilSurface);
PXPublic PXResult PXAPI PXDirectX9ModelRegister(PXDirectX9 PXREF pxDirectX9, PXModel PXREF pxModel);

#endif

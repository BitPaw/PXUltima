#pragma once

#ifndef PXDirectX11Included
#define PXDirectX11Included

#include <PX/OS/Graphic/DirectX/PXDirectX.h>
#include <PX/Engine/ECS/Resource/Mesh/PXVertexBuffer.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexture.h>

PXPublic PXResult PXAPI PXDirectX11Initialize(PXDirectX11 PXREF pxDirectX11, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectX11Release(PXDirectX11 PXREF pxDirectX11);

PXPublic PXResult PXAPI PXDirectX11TextureAction(PXDirectX11 PXREF pxDirectX11, PXTextureInfo PXREF pxGraphicTexturInfo);

PXPublic PXResult PXAPI PXDirectX11Clear(PXDirectX11 PXREF pxDirectX11, const PXColorRGBAF PXREF pxColorRGBAF);
PXPublic PXResult PXAPI PXDirectX11VertexBufferCreate(PXDirectX11 PXREF pxDirectX11, PXVertexBuffer PXREF pxVertexBuffer);

PXPublic PXResult PXAPI PXDirectX11ShaderProgramCreate(PXDirectX11 PXREF pxDirectX11, PXShaderProgram PXREF pxShaderProgram, PXShader PXREF shaderList, const PXSize amount);

PXPublic PXResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11 PXREF pxDirectX11, PXI32U PXREF value);

PXPublic PXResult PXAPI PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11 PXREF pxDirectX11, PXI32U PXREF amountOfAdapters);
PXPublic PXResult PXAPI PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11 PXREF pxDirectX11, const PXI32U pxGraphicDevicePhysicalListSize, PXGPUPhysical PXREF pxGraphicDevicePhysicalList);

#endif

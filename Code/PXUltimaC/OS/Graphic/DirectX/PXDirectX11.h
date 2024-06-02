#ifndef PXDirectX11INCLUDE
#define PXDirectX11INCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXActionResult PXAPI PXDirectX11Initialize(PXDirectX11* const pxDirectX11, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXActionResult PXAPI PXDirectX11Release(PXDirectX11* const pxDirectX11);

PXPublic PXActionResult PXAPI PXDirectX11TextureAction(PXDirectX11* const pxDirectX11, PXGraphicTexturInfo* const pxGraphicTexturInfo);

PXPublic PXActionResult PXAPI PXDirectX11Clear(PXDirectX11* const pxDirectX11, const PXColorRGBAF* const pxColorRGBAF);
PXPublic PXActionResult PXAPI PXDirectX11VertexBufferCreate(PXDirectX11* const pxDirectX11, PXVertexBuffer* const pxVertexBuffer);

PXPublic PXActionResult PXAPI PXDirectX11ShaderProgramCreate(PXDirectX11* const pxDirectX11, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount);

PXPublic PXActionResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11* const pxDirectX11, PXInt32U* const value);

PXPublic PXActionResult PXAPI PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11* const pxDirectX11, PXInt32U* const amountOfAdapters);
PXPublic PXActionResult PXAPI PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11* const pxDirectX11, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);

#endif
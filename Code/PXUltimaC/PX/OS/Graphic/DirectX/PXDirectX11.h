#ifndef PXDirectX11Included
#define PXDirectX11Included

#include <PX/OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXResult PXAPI PXDirectX11Initialize(PXDirectX11* const pxDirectX11, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectX11Release(PXDirectX11* const pxDirectX11);

PXPublic PXResult PXAPI PXDirectX11TextureAction(PXDirectX11* const pxDirectX11, PXTexturInfo* const pxGraphicTexturInfo);

PXPublic PXResult PXAPI PXDirectX11Clear(PXDirectX11* const pxDirectX11, const PXColorRGBAF* const pxColorRGBAF);
PXPublic PXResult PXAPI PXDirectX11VertexBufferCreate(PXDirectX11* const pxDirectX11, PXVertexBuffer* const pxVertexBuffer);

PXPublic PXResult PXAPI PXDirectX11ShaderProgramCreate(PXDirectX11* const pxDirectX11, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount);

PXPublic PXResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11* const pxDirectX11, PXI32U* const value);

PXPublic PXResult PXAPI PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11* const pxDirectX11, PXI32U* const amountOfAdapters);
PXPublic PXResult PXAPI PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11* const pxDirectX11, const PXI32U pxGraphicDevicePhysicalListSize, PXGPUPhysical* const pxGraphicDevicePhysicalList);

#endif

#ifndef PXDirectX11INCLUDE
#define PXDirectX11INCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXDirectX11Initialize(PXDirectX11* const pxDirectX);
	PXPublic PXActionResult PXAPI PXDirectX11Release(PXDirectX11* const pxDirectX);


    PXPublic PXActionResult PXAPI PXDirectX11Clear(PXDirectX11* const pxDirectX11, const PXColorRGBAF* const pxColorRGBAF);
    PXPublic PXActionResult PXAPI PXDirectX11VertexBufferCreate(PXDirectX11* const pxDirectX11, PXVertexBuffer* const pxVertexBuffer);

    PXPublic PXActionResult PXAPI PXDirectX11TextureMemoryAvailable(PXDirectX11* const pxDirectX, PXInt32U* const value);
    PXPublic PXActionResult PXAPI PXDirectX11Texture2DCreate(PXDirectX11* const pxDirectX, PXTexture2D* const pxTexture2D);
    PXPublic PXActionResult PXAPI PXDirectX11Texture3DCreate(PXDirectX11* const pxDirectX, PXTexture3D* const pxTexture3D);
    PXPublic PXActionResult PXAPI PXDirectX11TextureCubeCreate(PXDirectX11* const pxDirectX, PXTextureCube* const pxTextureCube);
    PXPublic PXActionResult PXAPI PXDirectX11DevicePhysicalListAmountFunction(PXDirectX11* const pxDirectX, PXInt32U* const amountOfAdapters);
    PXPublic PXActionResult PXAPI PXDirectX11DevicePhysicalListFetchFunction(PXDirectX11* const pxDirectX, const PXInt32U pxGraphicDevicePhysicalListSize, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);


#ifdef __cplusplus
}
#endif

#endif
#ifndef PXDirectX12INCLUDE
#define PXDirectX12INCLUDE

#include <OS/Graphic/DirectX/PXDirectX.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXDirectX12Initialize(PXDirectX12* const pxDirectX12, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
	PXPublic PXActionResult PXAPI PXDirectX12Release(PXDirectX12* const pxDirectX12);


	PXPublic PXActionResult PXAPI PXDirectX12DevicePhysicalListAmountFunction(PXDirectX12* const pxDirectX12, PXInt32U* const amountOfAdapters);


#ifdef __cplusplus
}
#endif

#endif
#ifndef PXDirectX12Included
#define PXDirectX12Included

#include <PX/OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXResult PXAPI PXDirectX12Initialize(PXDirectX12* const pxDirectX12, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectX12Release(PXDirectX12* const pxDirectX12);

PXPublic PXResult PXAPI PXDirectX12DevicePhysicalListAmountFunction(PXDirectX12* const pxDirectX12, PXI32U* const amountOfAdapters);

#endif

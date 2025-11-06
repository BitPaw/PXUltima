#pragma once

#ifndef PXDirectX12Included
#define PXDirectX12Included

#include <PX/OS/Graphic/DirectX/PXDirectX.h>

PXPublic PXResult PXAPI PXDirectX12Initialize(PXDirectX12 PXREF pxDirectX12, PXGraphicInitializeInfo PXREF pxGraphicInitializeInfo);
PXPublic PXResult PXAPI PXDirectX12Release(PXDirectX12 PXREF pxDirectX12);

PXPublic PXResult PXAPI PXDirectX12DevicePhysicalListAmountFunction(PXDirectX12 PXREF pxDirectX12, PXI32U PXREF amountOfAdapters);

#endif

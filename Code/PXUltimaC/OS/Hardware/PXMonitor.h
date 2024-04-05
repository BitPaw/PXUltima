#ifndef PXMonitorINCLUDE
#define PXMonitorINCLUDE

#include <Media/PXResource.h>

PXPublic PXActionResult PXAPI PXMonitorDeviceAmount(PXSize* const amount);

PXPublic void PXAPI PXMonitorFetchAll(PXMonitor* const monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize);

PXPublic void PXAPI PXMonitorGetSize(PXInt32S* const width, PXInt32S* const height);

// QueryDisplayConfig 

PXPublic void PXAPI PXPhysicalDeviceFetchAll(PXGraphicDevicePhysical* const pxGraphicDevicePhysical, const PXSize amount);

#endif
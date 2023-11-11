#ifndef PXMonitorINCLUDE
#define PXMonitorINCLUDE

#include <Media/PXResource.h>

#if OSUnix
#elif OSWindows
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif


#if OSUnix
#elif PXOSWindowsDestop
	PXPrivate BOOL WINAPI PXMonitorListCallBack(HMONITOR monitorHandle, HDC hdcMonitor, LPRECT rectangle, LPARAM data);
#endif

	PXPublic PXActionResult PXAPI PXMonitorDeviceAmount(PXSize* const amount);

	PXPublic void PXAPI PXMonitorFetchAll(PXMonitor* const monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize);

	PXPublic void PXMonitorGetSize(PXInt32S* const width, PXInt32S* const height);

	// QueryDisplayConfig 

	PXPublic void PXAPI PXPhysicalDeviceFetchAll(PXGraphicDevicePhysical* const pxGraphicDevicePhysical, const PXSize amount);

#ifdef __cplusplus
}
#endif

#endif
#include "PXMonitor.h"

#include <OS/Memory/PXMemory.h>
#include <OS/Console/PXConsole.h>
#include <Media/PXText.h>


#if OSUnix
#elif OSWindows
#include <windows.h>
#endif


#if OSUnix
#elif PXOSWindowsDestop
BOOL WINAPI PXMonitorListCallBack(HMONITOR monitorHandle, HDC hdcMonitor, LPRECT rectangle, LPARAM data);
#endif


#if OSUnix
#elif PXOSWindowsDestop
BOOL WINAPI PXMonitorListCallBack(HMONITOR monitorHandle, HDC hdcMonitor, LPRECT rectangle, LPARAM data)
{
	MONITORINFOEXA monitorInfo;

	monitorInfo.cbSize = sizeof(monitorInfo);

	const BOOL result = GetMonitorInfoA(monitorHandle, &monitorInfo);

	if(!result)
	{
		return 1;
	}

	PXMonitor monitor;
	monitor.X = monitorInfo.rcMonitor.left;
	monitor.Y = monitorInfo.rcMonitor.top;
	monitor.Width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	monitor.Height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	PXTextCopyA(monitorInfo.szDevice, CCHDEVICENAME, monitor.Name, MonitorNameLength);

	/*
	printf
	(
		"[Monitor] %p <%ls> %ix%i\n",
		monitorHandle,
		monitorInfo.szDevice,
		monitor.Width,
		monitor.Height
	);
	*/

	return 1;
}
#endif

PXActionResult PXAPI PXMonitorDeviceAmount(PXSize* const amount)
{
	*amount = 0;

#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows
	DISPLAY_DEVICEA displayDevice;
	displayDevice.cb = sizeof(DISPLAY_DEVICEA);

	// Count how many devices we have.
	while (EnumDisplayDevicesA(NULL, *amount, &displayDevice, 0)) ++(*amount);


#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"Monitor",
		"Device",
		"Detected <%i> monitor devices",
		*amount
	);
#endif

	return PXActionSuccessful;
#endif	
}

void PXAPI PXMonitorFetchAll(PXMonitor* const monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize)
{
#if OSUnix
#elif PXOSWindowsDestop
	HDC             hdc = 0;
	RECT         rectangleClip;
	MONITORENUMPROC lpfnEnum = PXMonitorListCallBack;
	LPARAM          dwData = 0;

	PXClear(RECT, &rectangleClip);

#if 0
	while(EnumDisplayMonitors(hdc, &rectangleClip, lpfnEnum, dwData));
#endif

#endif
}

void PXAPI PXMonitorGetSize(PXInt32S* const width, PXInt32S* const height)
{
#if OSUnix
    *width = 1200;
    *height = 800;

	// XRRGetScreenResources ?

#elif PXOSWindowsDestop
	RECT desktop;

	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();

	if(hDesktop)
	{
		// Get the size of screen to the variable desktop
		const BOOL rectResult = GetWindowRect(hDesktop, &desktop);

		// The top left corner will have coordinates (0,0)
		// and the bottom right corner will have coordinates
		// (horizontal, vertical)
		*width = desktop.right;
		*height = desktop.bottom;
	}
#else   
	*width = -1;
	*height = -1;
#endif

#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"Monitor",
		"Size-Get",
		"%i x %i",
		*width,
		*height
	);
#endif
}
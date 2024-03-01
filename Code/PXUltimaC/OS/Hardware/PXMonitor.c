#include "PXMonitor.h"

#include <OS/Memory/PXMemory.h>
#include <OS/Console/PXConsole.h>

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

	PXTextCopyW(monitorInfo.szDevice, CCHDEVICENAME, monitor.Name, MonitorNameLength);

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
	for ( ; EnumDisplayDevicesA(NULL, *amount, &displayDevice, 0); ++(*amount));


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

	while(EnumDisplayMonitors(hdc, &rectangleClip, lpfnEnum, dwData));
#endif
}

void PXAPI PXMonitorGetSize(PXInt32S* const width, PXInt32S* const height)
{
#if OSUnix
    *width = 1200;
    *height = 800;

#elif PXOSWindowsDestop
	RECT desktop;

	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();

	if(hDesktop)
	{
		// Get the size of screen to the variable desktop
		GetWindowRect(hDesktop, &desktop);

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
}

void PXAPI PXPhysicalDeviceFetchAll(PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList, const PXSize amount)
{
#if PXLogEnable
	PXLogPrint
	(
		PXLoggingInfo,
		"Monitor",
		"Device",
		"Fetching <%i> monitor devices",
		amount
	);
#endif

#if OSUnix
#elif OSWindows
	DISPLAY_DEVICEA displayDevice;
	DWORD            dwFlags = 0;
	displayDevice.cb = sizeof(displayDevice);

	for (PXSize deviceID = 0; EnumDisplayDevicesA(0, deviceID, &displayDevice, dwFlags); deviceID++)
	{
		PXGraphicDevicePhysical* const pxGraphicDevicePhysical = &pxGraphicDevicePhysicalList[deviceID];

		DWORD    iModeNum = ENUM_CURRENT_SETTINGS;
		DEVMODEA devMode = { 0 };
		DWORD    dwFlags = EDS_RAWMODE;

		devMode.dmSize = sizeof(DEVMODEA);

		const BOOL settingsResult = EnumDisplaySettingsExA
		(
			displayDevice.DeviceName,
			iModeNum,
			&devMode,
			dwFlags
		);

		pxGraphicDevicePhysical->AttachedMonitor.Width = devMode.dmPelsWidth;
		pxGraphicDevicePhysical->AttachedMonitor.Height = devMode.dmPelsHeight;

		PXTextCopyA(displayDevice.DeviceName, 32, pxGraphicDevicePhysical->DeviceDisplay, PXDeviceDisplaySize);
		PXTextCopyA(displayDevice.DeviceString, 128, pxGraphicDevicePhysical->DeviceName, PXDeviceNameSize);
		PXTextCopyA(displayDevice.DeviceID, 128, pxGraphicDevicePhysical->DeviceID, PXDeviceIDSize);
		PXTextCopyA(displayDevice.DeviceKey, 128, pxGraphicDevicePhysical->DeviceKey, PXDeviceKeySize);

		pxGraphicDevicePhysical->IsConnectedToMonitor = displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;

	

		{
			DISPLAY_DEVICEA monitorDeviceA;
			monitorDeviceA.cb = sizeof(monitorDeviceA);


			for (PXSize deviceMonitorID = 0; EnumDisplayDevicesA(displayDevice.DeviceName, deviceMonitorID, &monitorDeviceA, 0); deviceMonitorID++)
			{
				PXMonitor* const monitor = &pxGraphicDevicePhysical->AttachedMonitor;;

				PXTextCopyA(monitorDeviceA.DeviceString, 128, monitor->Driver, MonitorNameLength);

				PXSize position = PXTextFindFirstCharacterA(monitorDeviceA.DeviceID, 128, '\\');

				PXSize targetZize = 128 - position + 1;
				char* target = monitorDeviceA.DeviceID + position + 1;

				PXSize positionB = PXTextFindFirstCharacterA(target, targetZize, '\\');

				PXTextCopyA(target, positionB, monitor->Name, MonitorNameLength);
			}
		}

#if 0
		printf
		(
			"+--------------------------------------------------------+\n"
			"| DeviceName     : %-27s %-4ix%4i |\n"
			"| DeviceString   : %-37.37s |\n"
			"| Monitor Name   : %-37.37s |\n"
			"| Monitor Driver : %-37.37s |\n"
			"| DeviceID       : %-37.37s |\n"
			"| DeviceKey      : %-37.37s |\n"
			"+--------------------------------------------------------+\n",
			displayDevice.DeviceName,
			pxGraphicDevicePhysical->AttachedMonitor.Width,
			pxGraphicDevicePhysical->AttachedMonitor.Height,
			displayDevice.DeviceString,
			pxGraphicDevicePhysical->AttachedMonitor.Name,
			pxGraphicDevicePhysical->AttachedMonitor.Driver,
			displayDevice.DeviceID,
			displayDevice.DeviceKey
		);
#endif
	}	
#endif
}
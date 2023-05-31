#include "PXMonitor.h"

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif PXOSWindowsDestop
BOOL WINAPI PXMonitorListCallBack(HMONITOR monitorHandle, HDC hdcMonitor, LPRECT rectangle, LPARAM data)
{
	MONITORINFOEXW monitorInfo;

	monitorInfo.cbSize = sizeof(monitorInfo);

	const unsigned char result = GetMonitorInfoW(monitorHandle, &monitorInfo);

	if(!result)
	{
		return 1;
	}

	PXMonitor monitor;
	monitor.X = monitorInfo.rcMonitor.left;
	monitor.Y = monitorInfo.rcMonitor.top;
	monitor.Width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
	monitor.Height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

	PXTextCopyW(monitorInfo.szDevice, 32, monitor.Name, MonitorNameLength);

	printf
	(
		"[Monitor] %p <%ls> %ix%i\n",
		monitorHandle,
		monitorInfo.szDevice,
		monitor.Width,
		monitor.Height
	);

	return 1;
}
#endif

void PXMonitorFetchAll(PXMonitor* const monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize)
{
#if OSUnix
#elif PXOSWindowsDestop
	HDC             hdc = 0;
	RECT         rectangleClip;
	MONITORENUMPROC lpfnEnum = PXMonitorListCallBack;
	LPARAM          dwData = 0;

	PXMemoryClear(&rectangleClip, sizeof(RECT));

	while(EnumDisplayMonitors(hdc, &rectangleClip, lpfnEnum, dwData));


	DISPLAY_DEVICEW displayDevice;
	DWORD            dwFlags = 0;

	PXMemoryClear(&displayDevice, sizeof(DISPLAY_DEVICEW));

	displayDevice.cb = sizeof(DISPLAY_DEVICEW);

	for(PXSize deviceID = 0; EnumDisplayDevicesW(0, deviceID, &displayDevice, dwFlags); deviceID++)
	{
		DWORD    iModeNum = ENUM_CURRENT_SETTINGS;
		DEVMODEW devMode = {0};
		DWORD    dwFlags = EDS_RAWMODE;

		const BOOL settingsResult = EnumDisplaySettingsExW
		(
			displayDevice.DeviceName,
			iModeNum,
			&devMode,
			dwFlags
		);

		printf
		(
			"+------------------------\n"
			"| DeviceName   : %-10ls |\n"
			"| DeviceString : %-10ls |\n"
			"| StateFlags   : %-10i |\n"
			"| DeviceID     : %-10ls |\n"
			"| DeviceKey    : %-10ls |\n"
			"+------------------------\n",
			displayDevice.DeviceName,
			displayDevice.DeviceString,
			displayDevice.StateFlags,
			displayDevice.DeviceID,
			displayDevice.DeviceKey

		);
	}
#endif
}

void PXMonitorGetSize(unsigned int* width, unsigned int* height)
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

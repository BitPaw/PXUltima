#ifndef MonitorInclude
#define MonitorInclude

#include <Format/Type.h>

#include <OS/System/OSVersion.h>

#if OSUnix
#elif OSWindows
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define MonitorNameLength 32

	typedef struct Monitor_
	{
		unsigned short X;
		unsigned short Y;
		unsigned short Width;
		unsigned short Height;
		wchar_t Name[MonitorNameLength];
	}
	Monitor;

#if OSUnix
#elif OSWindows
	PXPrivate BOOL _stdcall MonitorListCallBack(HMONITOR monitorHandle, HDC hdcMonitor, LPRECT rectangle, LPARAM data);
#endif

	PXPublic void MonitorFetchAll(Monitor* monitorList, const PXSize monitorListSizeMax, const PXSize monitorListSize);

	PXPublic void MonitorGetSize(unsigned int* width, unsigned int* height);

#ifdef __cplusplus
}
#endif

#endif
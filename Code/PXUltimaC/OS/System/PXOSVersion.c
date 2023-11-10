#include "PXOSVersion.h"

#include <OS/Memory/PXMemory.h>

void PXOSVersionGet()
{
#if OSUnix
#elif OSWindows

#if WindowsAtleast10

    // TODO: Detect version for newer windows

#else
    OSVERSIONINFOEXA osVersionInfo;

    PXClear(OSVERSIONINFOEXA, &osVersionInfo);
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

    GetVersionEx(&osVersionInfo); // Windows 2000, Debricated after Windows 8

    const BOOL bIsWindowsXPorLater =
        ((osVersionInfo.dwMajorVersion > 5) ||
            ((osVersionInfo.dwMajorVersion == 5) && (osVersionInfo.dwMinorVersion >= 1)));
#endif

#endif
}
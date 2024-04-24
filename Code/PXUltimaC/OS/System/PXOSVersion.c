#include "PXOSVersion.h"

#include <OS/Memory/PXMemory.h>

void PXOSVersionGet(PXOSVersionInfo* const pxOSVersionInfo)
{
    PXClear(PXOSVersionInfo, pxOSVersionInfo);

#if OSUnix
   // pxOSVersionInfo->

#elif OSWindows

    pxOSVersionInfo->Manufacturer = PXOSManufacturerWindows;


#if WindowsAtleast10

    // TODO: Detect version for newer windows

#else
    OSVERSIONINFOEXA osVersionInfo;

    PXClear(OSVERSIONINFOEXA, &osVersionInfo);
    osVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

    const BOOL success = GetVersionExA(&osVersionInfo); // Windows 2000, Debricated after Windows 8

    pxOSVersionInfo->VersionMajor = osVersionInfo.dwMajorVersion;
    pxOSVersionInfo->VersionMinor = osVersionInfo.dwMinorVersion;

    switch(pxOSVersionInfo->VersionMajor)
    {
        case 4:
        {
            switch(pxOSVersionInfo->VersionMinor)
            {
                case 0:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversionNT;
                    break;

                case 1:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversionXP;
                    break;
            }

            break;
        }
        case 5:
        {
            switch(pxOSVersionInfo->VersionMinor)
            {
                case 0:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversion2000;
                    break;

                case 1:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversionXP;
                    break;
            }

            break;
        }
        case 6:
        {
            switch(pxOSVersionInfo->VersionMinor)
            {
                case 0:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversionVista;
                    break;

                case 1:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversion7;
                    break;

                case 2:
                    pxOSVersionInfo->VersionWindows = PXOSWindowsversion8;
                    break;
            }

            break;
        }
    }


#endif

#endif
}
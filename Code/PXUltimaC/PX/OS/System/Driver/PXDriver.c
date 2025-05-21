#include "PXDriver.h"

#if OSUnix
// ToDo:
#elif OSWindows
#include <windows.h>
#include <psapi.h>
#endif

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

PXActionResult PXAPI PXDriverListFetch(PXDriver** const pxDriverList, PXSize* const listSize)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    DWORD sizeOfList = 0;

    // Count size
    const BOOL resA = EnumDeviceDrivers(PXNull, 0, &sizeOfList);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Driver",
        "Scan",
        "Detected <%i> drivers",
        sizeOfList
    );
#endif

    // Alloc space
    PXSize driverImageBaseAdressListAmount = sizeOfList / sizeof(void*);
    void** driverImageBaseAdressList = PXMemoryHeapCallocT(void*, driverImageBaseAdressListAmount);

    const BOOL resB = EnumDeviceDrivers(driverImageBaseAdressList, sizeOfList, &sizeOfList);


    *pxDriverList = PXMemoryHeapCallocT(PXDriver, driverImageBaseAdressListAmount);

    for(PXSize i = 0; i < driverImageBaseAdressListAmount; ++i)
    {
        PXDriver* const pxDriver = &(*pxDriverList)[i];
        void* baseAdress = driverImageBaseAdressList[i];

        pxDriver->ImageBaseAdress = baseAdress;

        const DWORD resC = GetDeviceDriverBaseName(baseAdress, pxDriver->Name, 64);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Driver",
            "Scan",
            "[%p] %s",
            baseAdress,
            pxDriver->Name
        );
#endif
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedTypeNotSupported;
#endif
}
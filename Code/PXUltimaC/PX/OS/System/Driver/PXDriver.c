#include "PXDriver.h"

#if OSUnix
// ToDo:
#elif OSWindows
#include <windows.h>
#include <psapi.h>
#endif

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

PXResult PXAPI PXDriverListFetch(PXDriver* PXREF pxDriverList, PXSize PXREF listSize)
{
#if OSUnix
    return PXResultRefusedNotImplemented;

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
    PXSize driverImageBaseAddressListAmount = sizeOfList / sizeof(void*);
    void** driverImageBaseAddressList = PXMemoryHeapCallocT(void*, driverImageBaseAddressListAmount);

    const BOOL resB = EnumDeviceDrivers(driverImageBaseAddressList, sizeOfList, &sizeOfList);


    *pxDriverList = PXMemoryHeapCallocT(PXDriver, driverImageBaseAddressListAmount);

    for(PXSize i = 0; i < driverImageBaseAddressListAmount; ++i)
    {
        PXDriver PXREF pxDriver = &(*pxDriverList)[i];
        void* baseAddress = driverImageBaseAddressList[i];

        pxDriver->ImageBaseAddress = baseAddress;

        const DWORD resC = GetDeviceDriverBaseName(baseAddress, pxDriver->Name, 64);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Driver",
            "Scan",
            "[%p] %s",
            baseAddress,
            pxDriver->Name
        );
#endif
    }

    return PXResultOK;

#else
    return PXResultRefusedTypeNotSupported;
#endif
}
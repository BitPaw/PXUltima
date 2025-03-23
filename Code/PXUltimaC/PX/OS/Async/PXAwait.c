#include "PXAwait.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Console/PXConsole.h>

#if WindowsAtleast8
#pragma comment(lib, "Synchronization.lib")
#endif

/*
PXBool PXAPI PXAwaitChange(volatile void* const dataAdress, const PXSize dataSize)
{
#if OSUnix

    // futex ?

#elif OSWindows

    // Check
    {
        switch (dataSize)
        {
            case 1u:
            case 2u:
            case 4u:
            case 8u:
                break;

            default:
                return PXFalse;
        }
    }

    unsigned char compareValue[8];

    PXMemoryCopy(dataAdress, dataSize, compareValue, 8u);

#if WindowsAtleast8 && 0
    const DWORD timeoutMilliseconds = INFINITE;
    const PXBool sucessfull = WaitOnAddress(dataAdress, &compareValue, dataSize, timeoutMilliseconds); // Windows 8 (+UWP), Synchronization.lib, synchapi.h

    return sucessfull;
#else // Under windows 8

    for(;;)
    {
        const PXBool didChange = !PXMemoryCompare(compareValue, 8u, dataAdress, dataSize);

        if (didChange)
        {
            return PXTrue; // Done
        }

        //__asm volatile ("nop"); // This does not compile. Bad solution anyway

        // [Fix?] Could we tell the scheduler not to bother with this thread and skip it?
        // So it just uses very smal timeframe to do stuff.

    }

    return PXFalse;

#endif

#endif
}
*/

PXBool PXAPI PXAwaitChange(PXAwaitInfo* const pxAwaitInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Await",
        "Wait",
        "..."
    );
#endif

    for(;;)
    {
        const PXBool isSame = PXMemoryCompare(pxAwaitInfo->DataTarget, pxAwaitInfo->DataSize, pxAwaitInfo->DataExpect, pxAwaitInfo->DataSize);

        if(isSame)
        {
            break;
        }

        PXThreadSleep(PXNull, 1);
        // PXThreadYieldToOtherThreads();
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Await",
        "Wait",
        "Done!"
    );
#endif

    return PXTrue;
}

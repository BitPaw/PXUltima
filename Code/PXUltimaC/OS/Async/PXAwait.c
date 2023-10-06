#include "PXAwait.h"

#include <OS/System/PXOSVersion.h>
#include <OS/Memory/PXMemory.h>

#if WindowsAtleast8
#pragma comment(lib, "Synchronization.lib")
#endif

PXBool PXAwaitChange(volatile void* const dataAdress, const PXSize dataSize)
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

#if WindowsAtleast8
    const DWORD timeoutMilliseconds = INFINITE;
	const PXBool sucessfull = WaitOnAddress(dataAdress, &compareValue, dataSize, timeoutMilliseconds); // Windows 8 (+UWP), Synchronization.lib, synchapi.h 

    return sucessfull;
#else // Under windows 8      

    unsigned int dummyCounter = 0;

    while(PXMemoryCompare(compareValue, 8u, dataAdress, dataSize))
    {
        //__asm volatile ("nop"); // This does not compile. Bad solution anyway

        ++dummyCounter; // Prevent optimization

        // [Fix?] Could we tell the scheduler not to bother with this thread and skip it? 
        // So it just uses very smal timeframe to do stuff.      
    }

    return PXTrue;

#endif

#endif
}

PXBool PXAwaitChangeCU(volatile unsigned char* const dataAdress)
{
    return PXAwaitChange(dataAdress, sizeof(unsigned char));
}
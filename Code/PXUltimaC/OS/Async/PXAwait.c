#include "PXAwait.h"

#include <OS/System/PXOSVersion.h>
#include <OS/Memory/PXMemory.h>

PXBool PXAwaitChange(const volatile void* const dataAdress, const PXSize dataSize)
{
#if OSUnix

    // futex ?

#elif OSWindows    

    // Check
    {
        PXBool isValidSize;

        switch (dataSize)
        {
            case 1u:
            case 2u:
            case 4u:
            case 8u:
                isValidSize = 1u;
                break;

            default:
                isValidSize = 0u;
                break;
        }

        if (!isValidSize)
        {
            return PXFalse;
        }     
    }

    unsigned char compareValue[8];

    PXMemoryCopy(dataAdress, dataSize, compareValue, 8u);

#if 0//WindowsAtleast8

    const DWORD timeoutMilliseconds = INFINITE;
	const PXBool sucessfull = WaitOnAddress(dataAdress, &compareValue, dataSize, timeoutMilliseconds); // Minimum OS: Windows 8

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

PXBool PXAwaitChangeCU(const volatile unsigned char* const dataAdress)
{
    return PXAwaitChange(dataAdress, sizeof(unsigned char));
}
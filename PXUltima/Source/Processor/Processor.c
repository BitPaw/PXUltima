#include "Processor.h"

#include <OS/OSVersion.h>

#if defined(OSUnix)
#include <time.h>
#include <sys/time.h>
#elif defined(OSWindows)
#include <Windows.h>
#include <intrin.h>
#endif

void ProcessorFetchInfo(Processor* const processor)
{
#if defined(OSUnix)


#elif defined(OSWindows)
    int CPUInfo[4] = { -1 };
    unsigned   nExIds, i = 0;

    // Get the information associated with each extended ID.

    __cpuid(CPUInfo, 0x80000000);

    nExIds = CPUInfo[0];
    for (i = 0x80000000; i <= nExIds; ++i)
    {
        __cpuid(CPUInfo, i);
        // Interpret CPU brand string

        switch (i)
        {
        case 0x80000002:
        {
            memcpy(processor->BrandName, CPUInfo, sizeof(CPUInfo));
            break;
        }
        case 0x80000003:
        {
            memcpy(processor->BrandName + 16, CPUInfo, sizeof(CPUInfo));
            break;
        }
        case 0x80000004:
        {
            memcpy(processor->BrandName + 32, CPUInfo, sizeof(CPUInfo));
            break;
        }
        default:
            break;
        }
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    processor->NumberOfProcessors = sysInfo.dwNumberOfProcessors;
#endif   
}

unsigned int ProcessorFrequencyCurrent()
{
#if defined(OSWindows)
    FILETIME a, b, c, d;
    const HANDLE process = GetCurrentProcess();
    const unsigned char sucessful = GetProcessTimes(process, &a, & b, & c, & d);

    if (!sucessful)
    {
        return -1;
    }

    const size_t deltaTimeIU = d.dwLowDateTime | ((unsigned long long)d.dwHighDateTime << 32);
    const size_t deltaTimeD = deltaTimeIU * 0.0000001;

    //  Returns total user time.
    //  Can be tweaked to include kernel times as well.
    return deltaTimeIU;
        
#elif defined(OSUnix)
    return (double)clock() / CLOCKS_PER_SEC;
#endif
}

unsigned int ProcessorTimeReal()
{
#if defined(OSWindows)
    LARGE_INTEGER time, freq;
    if (!QueryPerformanceFrequency(&freq)) {
        //  Handle error
        return 0;
}
    if (!QueryPerformanceCounter(&time)) {
        //  Handle error
        return 0;
    }

    const double x = (double)time.QuadPart / (double)freq.QuadPart;

    return x;
#elif defined(OSUnix)
    struct timeval time;
    if (gettimeofday(&time, NULL)) {
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * 0.000001;
#endif
}
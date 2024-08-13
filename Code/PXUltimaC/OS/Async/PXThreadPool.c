#include "PXThreadPool.h"

#include <OS/Error/PXActionResult.h>

#include "PXThread.h"


// Linux does not have native support for threadpools.
// To solve this, you need to create a class yourself with pthreads


void PXAPI PXThreadPoolClose(PXThreadPool* const pxThreadPool)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows
    CloseThreadpool(pxThreadPool->Pool);

    pxThreadPool->Pool = PXNull;
#endif
}

void PXAPI PXThreadPoolCreate(PXThreadPool* const pxThreadPool)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows
    pxThreadPool->Pool = CreateThreadpool(PXNull);

    // Get stack info
    {
        PTP_POOL_STACK_INFORMATION pool_STACK_INFORMATION;

        const PXBool succ = QueryThreadpoolStackInformation(pxThreadPool->Pool, &pool_STACK_INFORMATION);

        pxThreadPool->StackCommit = pool_STACK_INFORMATION->StackCommit;
        pxThreadPool->StackReserve = pool_STACK_INFORMATION->StackReserve;

        SetThreadpoolThreadMaximum(pxThreadPool->Pool, pxThreadPool->ThreadsMaximum);
        const PXBool minResult = SetThreadpoolThreadMinimum(pxThreadPool->Pool, pxThreadPool->ThreadsMinimum);
    }

    // Associate the pool with a callback environment
    TP_CALLBACK_ENVIRON cbe;
    InitializeThreadpoolEnvironment(&cbe);
    SetThreadpoolCallbackPool(&cbe, pxThreadPool->Pool);
#endif
}

void PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows
    SetThreadpoolThreadMaximum(pxThreadPool->Pool, amount);
#endif
}

void PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows
    const PXBool x = SetThreadpoolThreadMinimum(pxThreadPool->Pool, amount);
#endif
}

PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows

    WaitForThreadpoolWorkCallbacks(pxThreadPool->Work, cancelRunning); // Windows Vista (+UWP), Kernel32.dll, threadpoolapiset.h

    // Clean up
    CloseThreadpoolWork(pxThreadPool->Work);
    CloseThreadpool(pxThreadPool->Pool);

#endif
}

void NTAPI PXWindowsVistaPTP_WORK_CALLBACK(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
    
}

PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows

    // Creates a new work object.
    pxThreadPool->Work = CreateThreadpoolWork(PXWindowsVistaPTP_WORK_CALLBACK, parameter, PXNull); // 3rd Parameter -> Enviroment TP_CALLBACK_ENVIRON 
    
    PXActionOnErrorFetchAndReturn(PXNull == pxThreadPool->Work);
    
    SubmitThreadpoolWork(pxThreadPool->Work); // Queue the work

    PXActionOnErrorFetchAndReturn(PXNull != pxThreadPool->Work);




    /*

#elif WindowsAtleastXP

    // Debricated since Vista
    const PXBool result = QueueUserWorkItem((LPTHREAD_START_ROUTINE)function, parameter, WT_EXECUTEDEFAULT); // Windows XP, Kernel32.dll, threadpoollegacyapiset.h 

#endif */

#endif






}

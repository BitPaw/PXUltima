#include "PXThreadPool.h"

#include <OS/Error/PXActionResult.h>

// Linux does not have native support for threadpools.
// To solve this, you need to create a class yourself with pthreads


void PXAPI PXThreadPoolClose(PXThreadPool* const pxThreadPool)
{
    CloseThreadpool(pxThreadPool->Pool);

    pxThreadPool->Pool = PXNull;     
}

void PXAPI PXThreadPoolCreate(PXThreadPool* const pxThreadPool)
{
    pxThreadPool->Pool = CreateThreadpool(PXNull);

    // Get stack info
    {
        PTP_POOL_STACK_INFORMATION pool_STACK_INFORMATION;

        const PXBool succ = QueryThreadpoolStackInformation(pxThreadPool->Pool, &pool_STACK_INFORMATION);

        pxThreadPool->StackCommit = pool_STACK_INFORMATION->StackCommit;
        pxThreadPool->StackReserve = pool_STACK_INFORMATION->StackReserve;
    }


    SetThreadpoolThreadMaximum(pool, 4);
    SetThreadpoolThreadMinimum(pool, 4);


    // Associate the pool with a callback environment
    TP_CALLBACK_ENVIRON cbe;
    InitializeThreadpoolEnvironment(&cbe);
    SetThreadpoolCallbackPool(&cbe, pool);


}

void PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
{
#if OSUnix
#elif OSWindows
    SetThreadpoolThreadMaximum(pxThreadPool->Pool, amount);
#endif
}

void PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
{
#if OSUnix
#elif OSWindows
    const PXBool x = SetThreadpoolThreadMinimum(pxThreadPool->Pool, amount);
#endif
}

PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning)
{
#if OSUnix
#elif OSWindows

#if WindowsAtleastVista
    WaitForThreadpoolWorkCallbacks(pxThreadPool->Work, cancelRunning); // Windows Vista (+UWP), Kernel32.dll, threadpoolapiset.h

    // Clean up
    CloseThreadpoolWork(work);
    CloseThreadpool(pool);
#elif WindowsAtleastXP

#endif

#endif
}


void NTAPI PXWindowsVistaPTP_WORK_CALLBACK(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{
    
}



PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter)
{
#if OSUnix
#elif OSWindows

#if WindowsAtleastVista && 1

    // Creates a new work object.
    pxThreadPool->Work = CreateThreadpoolWork(PXWindowsVistaPTP_WORK_CALLBACK, parameter, PXNull); // 3rd Parameter -> Enviroment TP_CALLBACK_ENVIRON 
    
    PXActionOnErrorFetchAndReturn(PXNull == pxThreadPool->Work);
    
    SubmitThreadpoolWork(pxThreadPool->Work); // Queue the work

    PXActionOnErrorFetchAndReturn(PXNull != pxThreadPool->Work);




    

#elif WindowsAtleastXP

    // Debricated since Vista
    const PXBool result = QueueUserWorkItem((LPTHREAD_START_ROUTINE)function, parameter, WT_EXECUTEDEFAULT); // Windows XP, Kernel32.dll, threadpoollegacyapiset.h 

#endif




#endif





}

#include "PXThreadPool.h"

#include <OS/Error/PXActionResult.h>
#include <OS/Hardware/PXProcessor.h>
#include <OS/Async/PXThread.h>


// Linux does not have native support for threadpools.
// To solve this, you need to create a class yourself with pthreads


PXThreadResult __stdcall PXThreadPoolCallBack(PXThreadPool* const pxThreadPool)
{
    return 0;
}

PXActionResult PXAPI PXThreadPoolClose(PXThreadPool* const pxThreadPool)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows
    CloseThreadpool(pxThreadPool->Pool);

    pxThreadPool->Pool = PXNull;
#endif
}

PXActionResult PXAPI PXThreadPoolCreate(PXThreadPool* const pxThreadPool)
{
    // Lookup how many cores we have.
    pxThreadPool->ThreadListSize = 32;
    pxThreadPool->ThreadList = PXMemoryCallocT(PXThread, pxThreadPool->ThreadListSize);


#if (OSUnix || (OSWindows && !WindowsAtleastVista)) | 1

    for(PXSize i = 0; i < pxThreadPool->ThreadListSize; ++i)
    {
        PXThread* const pxThread = &pxThreadPool->ThreadList[i];

        char nameBuffer[32];
        PXTextPrintA(nameBuffer, 32, "PX-ThreadPool-%3.3i", i);

        PXThreadCreate(pxThread, nameBuffer, PXNull, PXThreadPoolCallBack, pxThreadPool, PXThreadBehaviourCreateSuspended);
    }

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

PXActionResult PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows
    SetThreadpoolThreadMaximum(pxThreadPool->Pool, amount);
#endif
}

PXActionResult PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
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

#if OSWindows
void NTAPI PXWindowsVistaPTP_WORK_CALLBACK(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{

}
#endif

PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter)
{
#if (OSUnix || (OSWindows && !WindowsAtleastVista))
#elif OSWindows

    // Creates a new work object.
    pxThreadPool->Work = CreateThreadpoolWork(PXWindowsVistaPTP_WORK_CALLBACK, parameter, PXNull); // 3rd Parameter -> Enviroment TP_CALLBACK_ENVIRON
    const PXActionResult pxActionResult = PXErrorCurrent(PXNull != pxThreadPool->Work);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    SubmitThreadpoolWork(pxThreadPool->Work); // Queue the work

    /*

#elif WindowsAtleastXP

    // Debricated since Vista
    const PXBool result = QueueUserWorkItem((LPTHREAD_START_ROUTINE)function, parameter, WT_EXECUTEDEFAULT); // Windows XP, Kernel32.dll, threadpoollegacyapiset.h

#endif */

#endif






}

PXActionResult PXAPI PXThreadPoolQueueTask(PXThreadPool* const pxThreadPool, PXTask* const pxTask)
{
    return PXActionSuccessful;
}
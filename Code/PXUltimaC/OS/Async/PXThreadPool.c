#include "PXThreadPool.h"

#include <OS/Error/PXActionResult.h>
#include <OS/Hardware/PXProcessor.h>
#include <OS/Async/PXThread.h>
#include <OS/Console/PXConsole.h>

#define PXThreadPoolUsePXIMPLForce  1
#define PXThreadPoolUsePXIMPL       ((OSUnix || (OSWindows && !WindowsAtleastVista)) || PXThreadPoolUsePXIMPLForce)



PXThreadPool _GLOBALThreadPool;


// Linux does not have native support for threadpools.
// To solve this, you need to create a class yourself with pthreads


PXThread* PXAPI PXThreadPoolThreadSelf(PXThreadPool* const pxThreadPool)
{
    PXThreadHandleID pxThreadHandleID = PXThreadCurrentID();

    for(size_t i = 0; i < pxThreadPool->ThreadListSize; ++i)
    {
        PXThread* const pxThread = &pxThreadPool->ThreadList[i];

        PXBool isSame = pxThreadHandleID == pxThread->HandleID;

        if(isSame)
        {
            return pxThread;
        }
    }

    return PXNull;
}


PXThreadResult __stdcall PXThreadPoolCallBack(PXThreadPool* const pxThreadPool)
{
    PXThread* const pxThread = PXThreadPoolThreadSelf(pxThreadPool);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "ThreadPool",
        "ASYNC-Worker",
        "Starting job..."
    );
#endif


    for(;;)
    {
        PXTask* const pxTask = PXThreadPoolTaskNextWorkGet(pxThreadPool);

        if(!pxTask)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "ThreadPool",
                "ASYNC-Worker",
                "No current jobs in queue. Suspensing self..."
            );
#endif

            PXThreadSuspend(pxThread);

            continue;
        }

        if(!pxTask->FunctionX1Adress)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                "ThreadPool",
                "ASYNC-Worker",
                "Task no callback! Task:<%i>",
                pxTask->Info.ID
            );
#endif

            PXTaskStateChange(pxTask, PXExecuteStateFailed);
            continue;
        }

        if(pxTask->ArgumentObject2)
        {
            pxTask->FunctionReturnCode = pxTask->FunctionX2Adress(pxTask->ArgumentObject1, pxTask->ArgumentObject2);
        }
        else
        {
            pxTask->FunctionReturnCode = pxTask->FunctionX1Adress(pxTask->ArgumentObject1);
        }


        if(PXActionSuccessful != pxTask->FunctionReturnCode)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                "ThreadPool",
                "ASYNC-Worker",
                "Task Failed! Task:<%i>, Thread returned <%i>",
                pxTask->Info.ID,
                pxTask->FunctionReturnCode
            );
#endif

            PXTaskStateChange(pxTask, PXExecuteStateFailed);

            continue;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "ThreadPool",
            "ASYNC-Worker",
            "Task DONE! Task:<%i>, Thread:<%i>",
            pxTask->Info.ID,
            pxTask
        );
#endif

        PXTaskStateChange(pxTask, PXExecuteStateFinished);
    }

    return 0;
}

PXActionResult PXAPI PXThreadPoolClose(PXThreadPool* pxThreadPool)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "ThreadPool",
        "Close",
        "-"
    );
#endif

#if PXThreadPoolUsePXIMPL
#elif OSWindows
    CloseThreadpool(pxThreadPool->Pool);

    pxThreadPool->Pool = PXNull;
#endif
}

PXActionResult PXAPI PXThreadPoolCreate(PXThreadPool* pxThreadPool)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    // Lookup how many cores we have.
    pxThreadPool->ThreadListSize = 32;
    pxThreadPool->ThreadList = PXMemoryCallocT(PXThread, pxThreadPool->ThreadListSize);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "ThreadPool",
        "Create",
        "Amount:%i",
        pxThreadPool->ThreadListSize
    );
#endif


#if PXThreadPoolUsePXIMPL

    // init lock
    PXLockCreate(&pxThreadPool->TaskLock, PXLockTypeProcessOnly);

    // Create task buffer
    PXListInitialize(&pxThreadPool->TaskQueue, sizeof(PXTask), 512);

    // Create threads
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
#if PXThreadPoolUsePXIMPL
#elif OSWindows
    SetThreadpoolThreadMaximum(pxThreadPool->Pool, amount);
#endif
}

PXActionResult PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount)
{
#if PXThreadPoolUsePXIMPL
#elif OSWindows
    const PXBool x = SetThreadpoolThreadMinimum(pxThreadPool->Pool, amount);
#endif
}

PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning)
{
#if PXThreadPoolUsePXIMPL
#elif OSWindows

    WaitForThreadpoolWorkCallbacks(pxThreadPool->Work, cancelRunning); // Windows Vista (+UWP), Kernel32.dll, threadpoolapiset.h

    // Clean up
    CloseThreadpoolWork(pxThreadPool->Work);
    CloseThreadpool(pxThreadPool->Pool);

#endif
}

#if OSWindows && !PXThreadPoolUsePXIMPL
void NTAPI PXWindowsVistaPTP_WORK_CALLBACK(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{

}
#endif

void PXAPI PXTaskStateChange(PXTask* const pxTask, const PXInt32U newState)
{
    pxTask->Info.Behaviour &= ~PXExecuteStateMask;
    pxTask->Info.Behaviour |= newState;
}

PXTask* PXAPI PXThreadPoolTaskNextWorkGet(PXThreadPool* pxThreadPool)
{
    PXTask* result = PXNull;

    PXLockEngage(&pxThreadPool->TaskLock);

    for(PXSize i = 0; i < pxThreadPool->TaskQueue.AmountUsed; ++i)
    {
        PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        const PXBool doDoWork = PXExecuteStateInit & pxTask->Info.Behaviour;

        if(doDoWork)
        {
            PXTaskStateChange(pxTask, PXExecuteStateDibs);

            result = pxTask;

            break;
        }
    }

    PXLockRelease(&pxThreadPool->TaskLock);

    return result;
}

PXTask* PXAPI PXThreadPoolTaskNextFreeGet(PXThreadPool* pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour)
{
    PXTask* result = PXNull;

    PXLockEngage(&pxThreadPool->TaskLock);

    // Can we recycle?
    for(PXSize i = 0; i < pxThreadPool->TaskQueue.AmountUsed; ++i)
    {
        PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        const PXBool isFree = 
            (PXExecuteStateIDLE & pxTask->Info.Behaviour) ||
            (PXExecuteStateInvalid & pxTask->Info.Behaviour);

        if(isFree)
        {
            result = pxTask;
            break;
        }
    }

    if(!result)
    {
        // Task coun't be recycled, we make a new one
        PXTask pxTask;
        PXClear(PXTask, &pxTask);

        result = PXListAddT(PXTask, &pxThreadPool->TaskQueue, &pxTask);
    }

    // Add data
    result->Info.Behaviour = behaviour;
    result->FunctionX1Adress = function;
    result->ArgumentObject1 = parameter1;
    result->ArgumentObject2 = parameter2;
    result->FunctionReturnCode = PXActionWaitOnResult;

    PXTaskStateChange(result, PXExecuteStateIDLE);

    PXLockRelease(&pxThreadPool->TaskLock);

    return result;
}

// If no or too few threads a awake, resume sleeping threads to improve speed 
void PXAPI PXThreadPoolWaking(PXThreadPool* pxThreadPool)
{
    // Are we doing 
    for(PXSize i = 0; i < pxThreadPool->ThreadListSize; i++)
    {
        PXThread* pxThread = &pxThreadPool->ThreadList[i];
        PXBool isRunning = PXExecuteStateRunning & pxThread->Info.Flags;

        if(!isRunning)
        {
            PXThreadResume(pxThread);
            break;
        }
    }
}

PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

#if PXThreadPoolUsePXIMPL

    PXTask* const pxTask = PXThreadPoolTaskNextFreeGet(pxThreadPool, function, parameter1, parameter2, behaviour);

    PXThreadPoolWaking(pxThreadPool);


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
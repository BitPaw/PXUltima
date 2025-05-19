#include "PXThreadPool.h"

#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

#define PXThreadPoolUsePXIMPLForce  1
#define PXThreadPoolUsePXIMPL       ((OSUnix || (OSWindows && !WindowsAtleastVista)) || PXThreadPoolUsePXIMPLForce)


const char PXTheadPoolText[] = "ThreadPool";


PXThreadPool _GLOBALThreadPool;
PXThread _mainThreadHandle;



// Linux does not have native support for threadpools.
// To solve this, you need to create a class yourself with pthreads


PXActionResult PXAPI PXThreadPoolThreadSelf(PXThreadPool* const pxThreadPool, PXThread* const pxThread)
{
    PXThread threadCurrent;

    PXThreadCurrent(&threadCurrent);

    for(PXSize i = 0; i < pxThreadPool->ThreadListSize; ++i)
    {
        PXThread* const pxThreadTarget = &pxThreadPool->ThreadList[i];

        PXBool isSame = threadCurrent.HandleID == pxThreadTarget->HandleID;
        
        if(isSame)
        {
            PXCopy(PXThread, pxThreadTarget, pxThread);

            return PXActionSuccessful;
        }
    }

    PXCopy(PXThread, &threadCurrent, pxThread);

    return PXActionRefusedNotFound;
}

PXActionResult PXAPI PXThreadPoolTaskInvoke(PXThreadPool* const pxThreadPool, PXTask* const pxTask)
{
    if(!pxTask->FunctionX1Adress)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXTheadPoolText,
            "Invoke",
            "Task no callback! Task:<%i>",
            pxTask->Info.ID
        );
#endif

        PXTaskStateChangeRemote(pxThreadPool, pxTask, PXExecuteStateFailed);

        return PXActionRefusedMissingCallBack;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Invoke",
        "Task:<%i>",
        pxTask->Info.ID
    );
#endif

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
            PXTheadPoolText,
            "Invoke",
            "Task Failed! Task:<%i>, Thread returned <%i>",
            pxTask->Info.ID,
            pxTask->FunctionReturnCode
        );
#endif

        PXTaskStateChangeRemote(pxThreadPool, pxTask, PXExecuteStateFailed);

        return pxTask->FunctionReturnCode;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Invoke",
        "Task DONE! Task:<%i>, Thread:<%i>",
        pxTask->Info.ID,
        pxTask
    );
#endif

    PXTaskStateChangeRemote(pxThreadPool, pxTask, PXExecuteStateFinished);

    return PXActionSuccessful;
}

PXThreadResult PXOSAPI PXThreadPoolProcessASYNC(PXThreadPool* const pxThreadPool)
{
    PXThread pxThread;
    PXThreadPoolThreadSelf(pxThreadPool, &pxThread);

    for(;;)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXTheadPoolText,
            "ASYNC-Worker",
            "Fetching job... Detected:<%i>",
            pxThreadPool->TaskQueue.EntryAmountUsed
        );
#endif

        PXTask pxTask;
        PXBool success = PXThreadPoolTaskNextWorkGet(pxThreadPool, &pxTask);     

        if(!success)
        {
            PXBool isMainThread = PXThreadPoolIsMainThread();

            if(isMainThread)
            {
                return 0;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXTheadPoolText,
                "ASYNC-Worker",
                "No current jobs in queue. Suspensing self..."
            );
#endif
            PXThreadStateChange(&pxThread, PXExecuteStateSuspended);

            continue;
        }

        PXThreadPoolTaskInvoke(pxThreadPool, &pxTask);
    }

    return 0;
}

PXBool PXAPI PXThreadPoolIsMainThread()
{
    PXThread pxThreadCurrent;

    PXThreadCurrent(&pxThreadCurrent);

    return _mainThreadHandle.HandleID == pxThreadCurrent.HandleID;
}

PXActionResult PXAPI PXThreadPoolClose(PXThreadPool* pxThreadPool)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
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
        PXThreadCurrent(&_mainThreadHandle);
    }

    if(!(PXThreadPoolCreated & pxThreadPool->Flags))
    {
        pxThreadPool->Flags |= PXThreadPoolCreated;
        pxThreadPool->Flags |= PXThreadPoolEnableASYNC; // TODO: TEST!!
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Create",
        "Task-Cache:<%i>",
        32
    );
#endif

    // Lookup how many cores we have.
    pxThreadPool->ThreadListSize = 64;
    pxThreadPool->ThreadList = PXMemoryHeapCallocT(PXThread, pxThreadPool->ThreadListSize);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Create",
        "Amount:%i",
        pxThreadPool->ThreadListSize
    );
#endif


#if PXThreadPoolUsePXIMPL

    // init lock
    PXLockCreate(&pxThreadPool->TaskLock, PXLockTypeGlobal);

    // Create task buffer
    PXListInitialize(&pxThreadPool->TaskQueue, sizeof(PXTask), 0);
    pxThreadPool->TaskQueue.EntryGrowthOnAllocation += 512;
    pxThreadPool->TaskCounter = 1000;

    // Create threads
    for(PXSize i = 0; i < pxThreadPool->ThreadListSize; ++i)
    {
        PXThread* const pxThread = &pxThreadPool->ThreadList[i];
        pxThread->Info.Behaviour &= ~PXExecuteStateMask;
        pxThread->Info.Behaviour |= PXExecuteStateSuspended;

        char nameBuffer[32];
        PXTextPrintA(nameBuffer, 32, "PX-ThreadPool-%3.3i", i + 1);

        PXThreadCreate(pxThread, nameBuffer, PXNull, PXThreadPoolProcessASYNC, pxThreadPool, PXThreadBehaviourCreateSuspended);
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

PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* pxThreadPool, const PXBool cancelRunning)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

#if PXThreadPoolUsePXIMPL
#elif OSWindows

    WaitForThreadpoolWorkCallbacks(pxThreadPool->Work, cancelRunning); // Windows Vista (+UWP), Kernel32.dll, threadpoolapiset.h

    // Clean up
    CloseThreadpoolWork(pxThreadPool->Work);
    CloseThreadpool(pxThreadPool->Pool);

#endif
}

#include <PX/OS/Async/PXThread.h>

PXActionResult PXAPI PXThreadPoolWaitForSpesific(PXThreadPool* pxThreadPool, PXInt32U* listIDs, const PXSize amount, const PXBool cancelRunning)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    PXThreadPoolProcessASYNC(pxThreadPool);


    for(;;) // while we are not done..
    {
        PXSize counter = 0;

        for(PXSize i = 0; i < amount; ++i) // .. loop over all tasks ..
        {
            const PXInt32U id = listIDs[i];

            for(PXSize w = 0; w < pxThreadPool->TaskQueue.EntryAmountAllocated; ++w)
            {
                PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, w);

                if(pxTask->Info.ID != id)
                {
                    continue;
                }

                counter += PXExecuteStateFinished == (PXExecuteStateMask & pxTask->Info.Behaviour);
            }
        }

        if(counter == amount)
        {
            break;
        }

        Sleep(100);
    }


    return PXActionSuccessful;
}

PXActionResult PXAPI PXThreadPoolProcessSYNC(PXThreadPool* const pxThreadPool)
{
    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountAllocated; ++i)
    {
        PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        const PXBool doDoWork = PXTaskExecuteSYNC & pxTask->Info.Behaviour;

        if(!doDoWork)
        {
            continue;
        }

        PXThreadPoolTaskInvoke(pxThreadPool, pxTask);
    }

    return PXActionSuccessful;
}

#if OSWindows && !PXThreadPoolUsePXIMPL
void NTAPI PXWindowsVistaPTP_WORK_CALLBACK(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WORK Work)
{

}
#endif

const char* PXExecuteStateToString(const PXInt32U behaviour)
{
    switch(PXExecuteStateMask & behaviour)
    {
        case PXExecuteStateInvalid: return "Invalid";
        case PXExecuteStateReady: return "Ready";
        case PXExecuteStateReserve: return "Reserve";
        case PXExecuteStateRunning: return "Running";
        case PXExecuteStateWaiting: return "Wait";
        case PXExecuteStateSuspended: return "Suspended";
        case PXExecuteStateFailed: return "Failed";
        case PXExecuteStateFinished: return "Finished";

        default:
            return "Error";
    }
}

void PXAPI PXTaskStateChange(PXTask* const pxTask, const PXInt32U newState)
{
    pxTask->Info.Behaviour &= ~PXExecuteStateMask;
    pxTask->Info.Behaviour |= newState;
}

void PXAPI PXTaskStateChangeRemote(PXThreadPool* pxThreadPool, PXTask* const pxTask, const PXInt32U newState)
{
    PXLockEngage(&pxThreadPool->TaskLock);

    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; ++i)
    {
        PXTask* const pxTaskCurrent = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        const PXBool isMatch = pxTaskCurrent->Info.ID == pxTask->Info.ID;

        if(isMatch)
        {
            PXTaskStateChange(pxTaskCurrent, newState);
            break;
        }
    }

    PXLockRelease(&pxThreadPool->TaskLock);
}

PXBool PXAPI PXThreadPoolTaskNextWorkGet(PXThreadPool* pxThreadPool, PXTask* const pxTask)
{
    PXBool success = PXFalse;

    PXLockEngage(&pxThreadPool->TaskLock);

    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; ++i)
    {
        PXTask* const pxTaskCurrent = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        const PXBool doDoWork = PXExecuteStateRunning == (PXExecuteStateMask & pxTaskCurrent->Info.Behaviour);

        if(doDoWork)
        {
            PXTaskStateChange(pxTaskCurrent, PXExecuteStateReserve);

            PXCopy(PXTask, pxTaskCurrent, pxTask);

            success = PXTrue;

            break;
        }
    }

    PXLockRelease(&pxThreadPool->TaskLock);

    return success;
}

PXTask* PXAPI PXThreadPoolTaskNextFreeGet(PXThreadPool* pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour)
{
    PXTask* pxTaskTarget = PXNull;

    PXLockEngage(&pxThreadPool->TaskLock);

    // Can we recycle?
    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; ++i)
    {
        PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        PXBool isFree = 0;

        switch(PXExecuteStateMask & pxTask->Info.Behaviour)
        {
            case PXExecuteStateInvalid:
            case PXExecuteStateReady:
                isFree = PXTrue;
                break;
        }

        if(!isFree)
        {
            continue;
        }

        pxTaskTarget = pxTask;
        pxTaskTarget->Info.ID = ++pxThreadPool->TaskCounter;
    }

    if(!pxTaskTarget)
    {
        // Task coun't be recycled, we make a new one
        PXTask pxTask;
        PXClear(PXTask, &pxTask);

        pxTask.Info.ID = ++pxThreadPool->TaskCounter;
        pxTaskTarget = PXListAddT(PXTask, &pxThreadPool->TaskQueue, &pxTask);
    }

    // Add data
    pxTaskTarget->Info.Behaviour = behaviour;
    pxTaskTarget->FunctionX1Adress = function;
    pxTaskTarget->ArgumentObject1 = parameter1;
    pxTaskTarget->ArgumentObject2 = parameter2;
    pxTaskTarget->FunctionReturnCode = PXActionWaitOnResult;

    PXTaskStateChange(pxTaskTarget, PXExecuteStateRunning);

    PXLockRelease(&pxThreadPool->TaskLock);

    return pxTaskTarget;
}

// If no or too few threads a awake, resume sleeping threads to improve speed 
void PXAPI PXThreadPoolWaking(PXThreadPool* pxThreadPool)
{
    PXSize threadCounterAwake = 0;
    PXSize threadCounterSleep = 0;

    PXLockEngage(&pxThreadPool->TaskLock);

    // How many awake?
    {
        for(PXSize i = 0; i < pxThreadPool->ThreadListSize; ++i)
        {
            PXThread* const pxThread = &pxThreadPool->ThreadList[i];

            switch(PXExecuteStateMask & pxThread->Info.Behaviour)
            {
                case PXExecuteStateSuspended:
                {
                    ++threadCounterSleep;
                    break;
                }
                case PXExecuteStateRunning:
                {
                    ++threadCounterAwake;
                    break;
                }
            }
        }
    }

    if(threadCounterSleep == 0)
    {
        // We are at FULL capacity. Cant do more.
        PXLockRelease(&pxThreadPool->TaskLock);
        return;
    }

    // Are we doing 
    for(PXSize i = 0; i < pxThreadPool->ThreadListSize; ++i)
    {
        PXThread* const pxThread = &pxThreadPool->ThreadList[i];
        const PXBool isSuspensed = PXExecuteStateSuspended == (PXExecuteStateMask & pxThread->Info.Behaviour);

        if(isSuspensed)
        {
            PXThreadStateChange(pxThread, PXExecuteStateRunning);
            break;
        }
    }

    PXLockRelease(&pxThreadPool->TaskLock);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Wakeing",
        "Threads:<%i>, Awake:<%i>, Sleep:<%i>",
        pxThreadPool->ThreadListSize,
        threadCounterAwake,
        threadCounterSleep
    );
#endif
}

PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    const PXBool callSyncronously = !(PXThreadPoolEnableASYNC & pxThreadPool->Flags);

    if(callSyncronously)
    {
        PXTask pxTask;
        PXClear(PXTask, &pxTask);
        pxTask.Info.Behaviour = behaviour;
        pxTask.FunctionX1Adress = function;
        pxTask.ArgumentObject1 = parameter1;
        pxTask.ArgumentObject2 = parameter2;
        pxTask.FunctionReturnCode = PXActionWaitOnResult;

        return PXThreadPoolTaskInvoke(pxThreadPool, &pxTask);
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

PXActionResult PXAPI PXThreadPoolQueuePrepare(PXThreadPool* pxThreadPool, PXInt32U** listIDs, const PXSize amount)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Queue",
        "Prepare space for <%i> tasks",
        amount
    );
#endif

    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    if(listIDs)
    {
        *listIDs = PXMemoryHeapCallocT(PXInt32U, amount);
    }

    PXLockEngage(&pxThreadPool->TaskLock);

    PXListReserve(&pxThreadPool->TaskQueue, amount);

    if(listIDs)
    {
        PXSize counter = 0;

        for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; ++i)
        {
            PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

            switch(PXExecuteStateMask & pxTask->Info.Behaviour)
            {
                case PXExecuteStateInvalid:
                case PXExecuteStateReady:
                {
                    break;
                }
                default:
                    continue;
            }

            pxTask->Info.ID = ++pxThreadPool->TaskCounter;
            PXTaskStateChange(pxTask, PXExecuteStateReserve);

            listIDs[counter] = pxTask->Info.ID;

            ++counter;

            if(counter == amount)
            {
                break;
            }
        }
    }

    PXLockRelease(&pxThreadPool->TaskLock);

    return PXActionSuccessful;
}
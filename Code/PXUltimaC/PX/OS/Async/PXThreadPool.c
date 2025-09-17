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


PXResult PXAPI  PXThreadPoolThreadSelf(PXThreadPool* const pxThreadPool, PXThread* const pxThread)
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

PXResult PXAPI  PXThreadPoolTaskInvoke(PXThreadPool* const pxThreadPool, PXTask* const pxTask)
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

    PXThreadYieldToOtherThreads();

    return PXActionSuccessful;
}

PXThreadResult PXOSAPI PXThreadPoolProcessASYNC(PXThreadPool* const pxThreadPool)
{
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

            PXThread pxThread;
            PXThreadPoolThreadSelf(pxThreadPool, &pxThread);

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

PXResult PXAPI  PXThreadPoolClose(PXThreadPool* pxThreadPool)
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

PXResult PXAPI  PXThreadPoolCreate(PXThreadPool* pxThreadPool)
{
    PXOS* pxOS = PXSystemGet();
    const PXI32U amountOfThreads = pxOS->ProcessorAmountLogical;

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
        amountOfThreads
    );
#endif

    pxThreadPool->ThreadListSize = amountOfThreads;
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
        PXThreadCPUCoreAffinitySet(pxThread, i);
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

PXResult PXAPI  PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXI32U amount)
{
#if PXThreadPoolUsePXIMPL
#elif OSWindows
    SetThreadpoolThreadMaximum(pxThreadPool->Pool, amount);
#endif
}

PXResult PXAPI  PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXI32U amount)
{
#if PXThreadPoolUsePXIMPL
#elif OSWindows
    const PXBool x = SetThreadpoolThreadMinimum(pxThreadPool->Pool, amount);
#endif
}

PXResult PXAPI  PXThreadPoolWaitForAll(PXThreadPool* pxThreadPool, const PXBool cancelRunning)
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

PXResult PXAPI  PXThreadPoolWaitForSpesific(PXThreadPool* pxThreadPool, PXI32U* listIDs, const PXSize amount, const PXBool cancelRunning)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

#if PXLogEnable && 0
    for(size_t i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; i++)
    {
        PXTask* const pxTask = PXListEntyrGetT(PXTask, &pxThreadPool->TaskQueue, i);

        char* textState = PXExecuteStateToString(pxTask->Info.Behaviour);

        PXLogPrint
        (
            PXLoggingInfo,
            PXTheadPoolText,
            "Task-Info",
            "ID:%4i, State:%10s, Return:%i",
            pxTask->Info.ID,
            textState,
            pxTask->FunctionReturnCode
        );
    }
#endif


    PXThreadPoolProcessASYNC(pxThreadPool);


    // while we are not done..
    PXSize counter = 0;

    for(PXSize i = 0; i < amount; ++i) // .. loop over all tasks ..
    {
        // For every task..
        const PXI32U id = listIDs[i];

        // check the list..
        for(PXSize w = 0; w < pxThreadPool->TaskQueue.EntryAmountUsed; )
        {
            PXTask* const pxTask = PXListItemAtIndexGetT(PXTask, &pxThreadPool->TaskQueue, w);

            // When task is OK, go next
            if(pxTask->Info.ID != id)
            {
                ++w;
                continue;
            }

            PXBool isDone = PXExecuteStateFinished == (PXExecuteStateMask & pxTask->Info.Behaviour);

            if(isDone)
            {
                break;
            }

            Sleep(100);

            ++w;
        }
    }

    // Signal to all finished tasks they are now relesed (deleted)
    
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTheadPoolText,
        "Task-SYNC",
        "DONE!"
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXThreadPoolProcessSYNC(PXThreadPool* const pxThreadPool)
{
    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountAllocated; ++i)
    {
        PXTask* const pxTask = PXListItemAtIndexGetT(PXTask, &pxThreadPool->TaskQueue, i);

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

const char* PXExecuteStateToString(const PXI32U behaviour)
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

void PXAPI PXTaskStateChange(PXTask* const pxTask, const PXI32U newState)
{
    pxTask->Info.Behaviour &= ~PXExecuteStateMask;
    pxTask->Info.Behaviour |= newState;
}

void PXAPI PXTaskStateChangeRemote(PXThreadPool* pxThreadPool, PXTask* const pxTask, const PXI32U newState)
{
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    PXLockEngage(&pxThreadPool->TaskLock);

    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; ++i)
    {
        PXTask* const pxTaskCurrent = PXListItemAtIndexGetT(PXTask, &pxThreadPool->TaskQueue, i);

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

    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    PXLockEngage(&pxThreadPool->TaskLock);

    for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountUsed; ++i)
    {
        PXTask* const pxTaskCurrent = PXListItemAtIndexGetT(PXTask, &pxThreadPool->TaskQueue, i);

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

PXTask* PXAPI PXThreadPoolTaskUpdateWork(PXThreadPool* pxThreadPool, const PXI32U taskID, void* function, void* parameter1, void* parameter2, const PXI32U behaviour)
{
    PXTask* pxTaskTarget = PXNull;

    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    PXLockEngage(&pxThreadPool->TaskLock);

    // We defined a known task, find it!
    if(0 != taskID)
    {
        for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountAllocated; ++i)
        {
            PXTask* const pxTask = PXListItemAtIndexGetT(PXTask, &pxThreadPool->TaskQueue, i);

            if(pxTask->Info.ID == taskID)
            {
                pxTaskTarget = pxTask;
                break;
            }
        }

        if(!pxTaskTarget)
        {
            DebugBreak();
        }
    }
    else
    {
        // Can we recycle?
        for(PXSize i = 0; i < pxThreadPool->TaskQueue.EntryAmountAllocated; ++i)
        {
            PXTask* const pxTask = PXListItemAtIndexGetT(PXTask, &pxThreadPool->TaskQueue, i);

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
            //pxTaskTarget->Info.ID = ++pxThreadPool->TaskCounter;
        }

        if(!pxTaskTarget)
        {
            // Task coun't be recycled, we make a new one
            PXTask pxTask;
            PXClear(PXTask, &pxTask);

            pxTask.Info.ID = ++pxThreadPool->TaskCounter;
            pxTaskTarget = PXListAddT(PXTask, &pxThreadPool->TaskQueue, &pxTask);
        }
    }    

    // Add data
    pxTaskTarget->Info.Behaviour = behaviour;
    pxTaskTarget->FunctionX1Adress = function;
    pxTaskTarget->ArgumentObject1 = parameter1;
    pxTaskTarget->ArgumentObject2 = parameter2;
    pxTaskTarget->FunctionReturnCode = PXActionWaitOnResult;

    PXTaskStateChange(pxTaskTarget, PXExecuteStateRunning);

    PXLockRelease(&pxThreadPool->TaskLock);

    PXThreadPoolWaking(pxThreadPool);

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

PXResult PXAPI  PXThreadPoolQueueWork(PXThreadPool* pxThreadPool, const PXI32U taskID, void* function, void* parameter1, void* parameter2, const PXI32U behaviour)
{
    // Fetch default threadpool if not set
    if(!pxThreadPool)
    {
        pxThreadPool = &_GLOBALThreadPool;
    }

    // We can force the sync processing of the whole task
    PXBool callSyncronously = !(PXThreadPoolEnableASYNC & pxThreadPool->Flags);

#if 1 // FORCE SINGLE THREAD
    callSyncronously = PXTrue;
#endif

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

    PXTask* const pxTask = PXThreadPoolTaskUpdateWork(pxThreadPool, taskID, function, parameter1, parameter2, behaviour);

    // Poke any sleeping thread that new work is to be done
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

PXResult PXAPI  PXThreadPoolQueueTask(PXThreadPool* const pxThreadPool, PXTask* const pxTask)
{
    return PXActionSuccessful;
}

PXResult PXAPI  PXThreadPoolQueuePrepare(PXThreadPool* pxThreadPool, PXI32U** listIDs, const PXSize amount)
{
    PXI32U* list = 0;

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
        list = PXMemoryHeapCallocT(PXI32U, amount);
        *listIDs = list;
    }

    PXLockEngage(&pxThreadPool->TaskLock);

    PXListReserve(&pxThreadPool->TaskQueue, amount);

    if(list)
    {
      //  PXSize length = pxThreadPool->TaskQueue.EntryAmountUsed;

        for(PXSize i = 0; i < amount; ++i)
        {
            PXTask* const pxTask = PXListAddT(PXTask, &pxThreadPool->TaskQueue, PXNull);

            pxTask->Info.ID = ++pxThreadPool->TaskCounter;
            list[i] = pxTask->Info.ID;

            PXTaskStateChange(pxTask, PXExecuteStateReserve);
        }
    }

    PXLockRelease(&pxThreadPool->TaskLock);

    return PXActionSuccessful;
}
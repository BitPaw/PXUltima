#include "PXThread.h"

#include <OS/Console/PXConsole.h>
#include <OS/File/PXFile.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Debug/PXDebug.h>

#if OSUnix
#include <sys/resource.h> // getpriority, setpriority
#elif OSWindows
#include <Windows.h>
#include <dbghelp.h>
#include <winternl.h>
#include <winternl.h>
#endif

// Note, TerminateThread() should not be used, it can lead to a memory leak in Windows-XP

void PXAPI PXThreadDestruct(PXThread* const pxThread)
{
    if (!pxThread)
    {
        return;
    }

    if (!pxThread->Info.Handle.ThreadHandle)
    {
        return;
    }

#if OSUnix

#elif OSWindows
    const HANDLE currentThread = GetCurrentThread();
    const PXBool isTargetCurrentThread = currentThread == pxThread->Info.Handle.ThreadHandle;
    const PXBool success = CloseHandle(pxThread->Info.Handle.ThreadHandle); // Windows 2000 Professional (+UWP), Kernel32.dll, handleapi.h

    if (!success)
    {
        return;
    }

#endif

    PXClear(PXThread, pxThread);
}


#if OSWindows
void PXAPI PXThreadConstructFromHandle(PXThread* const pxThread, HANDLE threadHandle)
{
    pxThread->Info.Handle.ThreadHandle = threadHandle;

#if WindowsAtleastVista
    pxThread->HandleID = GetThreadId(threadHandle); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h
#else
    pxThread->ThreadID = 0xFFFFFFFF;
#endif
}
#endif


PXActionResult PXAPI PXThreadCreate(PXThread* const pxThread, const char* const threadName, const PXProcessHandle targetProcessHandle, ThreadFunction threadFunction, void* parameter, const PXInt32U behaviour)
{
    if(!pxThread || !threadFunction)
    {
        return PXActionRefusedArgumentNull;
    }

    PXClear(PXThread, pxThread);

    pxThread->Info.Behaviour = behaviour;

#if OSUnix

    const int errorID = pthread_create
    (
        &pxThread->Info.Handle.ThreadHandle,
        0,
        threadFunction,
        (void*)parameter
    );
    const PXActionResult threadCreateResult = PXErrorCurrent(0 != errorID);


#elif OSWindows


    LPSECURITY_ATTRIBUTES lpThreadAttributes = PXNull;
    SIZE_T dwStackSize = 0;
    LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)threadFunction;
    LPVOID lpParameter = (LPVOID)parameter;
    DWORD dwCreationFlags = 0;
    LPDWORD lpThreadId = &pxThread->HandleID;

    if(PXThreadBehaviourCreateSuspended & pxThread->Info.Behaviour)
    {
        dwCreationFlags |= CREATE_SUSPENDED;
    }

    if(!targetProcessHandle) //  Target own process
    {
        pxThread->Info.Handle.ThreadHandle = CreateThread
        (
            lpThreadAttributes,
            dwStackSize,
            lpStartAddress,
            lpParameter,
            dwCreationFlags,
            lpThreadId
        ); // Windows NT (+UWP), Kernel32.dll, processthreadsapi.h
    }
    else
    {
        pxThread->Info.Handle.ThreadHandle = CreateRemoteThread // Windows XP, Kernel32.dll, processthreadsapi.h
        (
            targetProcessHandle,
            lpThreadAttributes,
            dwStackSize,
            lpStartAddress,
            lpParameter,
            dwCreationFlags,
            lpThreadId
        );
    }
   
    const PXActionResult threadCreateResult = PXErrorCurrent(PXNull != pxThread->Info.Handle.ThreadHandle);
#else
    const PXActionResult threadCreateResult = PXActionRefusedNotSupported;
#endif

    if(PXActionSuccessful != threadCreateResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "Thread",
            "Create",
            "Failed to create Name:<%s>",
            threadName
        );
#endif
        return threadCreateResult;
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Thread",
        "Create",
        "OK. HANDLE:<%p>, ID:<%i> Name:<%s>",
        pxThread->Info.Handle.ThreadHandle,
        pxThread->HandleID,
        threadName
    );
#endif

    pxThread->Info.Behaviour = PXExecuteStateRunning;

    // Name thread if possible
    {
        PXText pxText;
        PXTextConstructFromAdressA(&pxText, threadName, 0, PXTextLengthUnkown);

        PXThreadNameSet(pxThread, &pxText);
    }

}

PXActionResult PXAPI PXThreadExitCurrent(const PXInt32U exitCode)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    ExitThread(exitCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAPI PXThreadYieldToOtherThreads()
{
#if OSUnix
    const int yieldResultID = 
#if 0
        pthread_yield(); // [deprecated] pthread.h
#else
        sched_yield(); // sched.h
#endif
    const PXActionResult yieldResult = PXErrorCurrent(0 != yieldResultID);

    return yieldResult;

#elif OSWindows
    // UmsThreadYield() // Windows 7 (64-Bit only), Kernel32.dll, winbase.h [Debcricated in Windows 11]

    const PXBool switchSuccessful = SwitchToThread(); // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h
    const PXActionResult pxActionResult = PXActionSuccessful; // cant fail

    return pxActionResult;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXThreadOpen(PXThread* const pxThread)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    DWORD dwDesiredAccess = 0;
    BOOL  bInheritHandle = 0;
    DWORD dwThreadId = 0;

    const HANDLE threadID = OpenThread(dwDesiredAccess, bInheritHandle, dwThreadId); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXActionResult pxActionResult = PXErrorCurrent(PXNull != threadID);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXPrivate int PXAPI PXThreadPriorityToID(const PXThreadPriorityMode pxThreadPriorityMode)
{
    switch(pxThreadPriorityMode)
    {
    case PXThreadPriorityModeLowest:
    {
        return
#if OSUnix
            20;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow19:
    {
        return
#if OSUnix
            19;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_LOWEST
#endif
        ;

    }
    case PXThreadPriorityModeLow18:
    {
        return
#if OSUnix
            18;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_BELOW_NORMAL
#endif
        ;

    }
    case PXThreadPriorityModeLow17:
    {
        return
#if OSUnix
            17;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_NORMAL
#endif
        ;

    }
    case PXThreadPriorityModeLow16:
    {
        return
#if OSUnix
            16;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_ABOVE_NORMAL
#endif
        ;

    }
    case PXThreadPriorityModeLow15:
    {
        return
#if OSUnix
            15;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_HIGHEST
#endif
        ;

    }
    case PXThreadPriorityModeLow14:
    {
        return
#if OSUnix
            14;
#elif OSWindows
            IDLE_PRIORITY_CLASS | THREAD_PRIORITY_TIME_CRITICAL
#endif
        ;

    }
    case PXThreadPriorityModeLow13:
    {
        return
#if OSUnix
            13;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow12:
    {
        return
#if OSUnix
            12;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow11:
    {
        return
#if OSUnix
            11;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow10:
    {
        return
#if OSUnix
            10;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow09:
    {
        return
#if OSUnix
            9;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow08:
    {
        return
#if OSUnix
            8;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow07:
    {
        return
#if OSUnix
            7;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow06:
    {
        return
#if OSUnix
            6;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow05:
    {
        return
#if OSUnix
            5;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow04:
    {
        return
#if OSUnix
            4;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow03:
    {
        return
#if OSUnix
            3;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow02:
    {
        return
#if OSUnix
            2;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeLow01:
    case PXThreadPriorityModeLower:
    {
        return
#if OSUnix
            1;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeNormal:
    {
        return
#if OSUnix
            0;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigher:
    case PXThreadPriorityModeHigh01:
    {
        return
#if OSUnix
            - 1;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh02:
    {
        return
#if OSUnix
            - 2;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh03:
    {
        return
#if OSUnix
            - 3;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh04:
    {
        return
#if OSUnix
            - 4;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh05:
    {
        return
#if OSUnix
            - 5;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh06:
    {
        return
#if OSUnix
            - 6;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh07:
    {
        return
#if OSUnix
            - 7;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh08:
    {
        return
#if OSUnix
            - 8;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh09:
    {
        return
#if OSUnix
            - 9;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh10:
    {
        return
#if OSUnix
            - 10;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh11:
    {
        return
#if OSUnix
            - 11;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh12:
    {
        return
#if OSUnix
            - 12;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh13:
    {
        return
#if OSUnix
            - 13;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh14:
    {
        return
#if OSUnix
            - 14;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh15:
    {
        return
#if OSUnix
            - 15;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh16:
    {
        return
#if OSUnix
            - 16;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh17:
    {
        return
#if OSUnix
            - 17;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh18:
    {
        return
#if OSUnix
            - 18;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHigh19:
    {
        return
#if OSUnix
            - 19;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;

    }
    case PXThreadPriorityModeHighest:
    {
        return
#if OSUnix
            20;
#elif OSWindows
            THREAD_PRIORITY_IDLE
#endif
        ;
    }

    default:
        return 0;
    }
}

PXActionResult PXAPI PXThreadPrioritySet(PXThread* pxThread, const PXThreadPriorityMode pxThreadPriorityMode)
{
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrentGet(&dummyThread);
        pxThread = &dummyThread;
    }

    const int threadPriority = PXThreadPriorityToID(pxThreadPriorityMode);

#if OSUnix
    const int newPriority = setpriority(0, pxThread->ThreadHandle, threadPriority);
    const PXActionResult pxActionResult = PXErrorCurrent(-1 != newPriority);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    return PXActionSuccessful;

#elif OSWindows
    // SetPriorityClass() also exists, but is it needed?
    const BOOL success = SetThreadPriority(pxThread->Info.Handle.ThreadHandle, threadPriority);
    const PXActionResult pxActionResult = PXErrorCurrent(PXNull != success);

    if(PXActionSuccessful != pxActionResult)
    {
        return pxActionResult;
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXThreadPriorityGet(PXThread* pxThread, PXThreadPriorityMode* const pxThreadPriorityMode)
{
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrentGet(&dummyThread);
        pxThread = &dummyThread;
    }

    int priorityID = 0;

#if OSUnix
    priorityID = getpriority(0, pxThread->ThreadHandle); // sys/resource.h
#elif OSWindows
    priorityID = GetThreadPriority(pxThread->Info.Handle.ThreadHandle);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXThreadStateChange(PXThread* const pxThread, const PXThreadState pxThreadState)
{
    if(!pxThread)
    {
        return PXActionRefusedArgumentNull;
    }

    PXProcessThreadHandle threadHandle = pxThread->Info.Handle.ThreadHandle;

    if(!threadHandle)
    {
        return PXActionRefusedObjectNotCreated;
    }

    switch(pxThreadState)
    {
        case PXThreadStateSleeping:
        {
            pxThread->ReturnResultCode = 0;

#if OSUnix
            const int resultID = pthread_join(pxThread->ThreadHandle, &pxThread->ReturnResult); // pthread.h
            const PXBool success = 0 == resultID;

            if(!success)
            {
                return PXErrorCodeFromID(resultID);
            }

            return PXActionSuccessful;

#elif OSWindows

            const DWORD resultID = WaitForSingleObject(threadHandle, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

            for(;;)
            {
                const BOOL result = GetExitCodeThread(threadHandle, &pxThread->ReturnResultCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
                const PXBool isDone = STILL_ACTIVE != pxThread->ReturnResultCode;

                if(isDone)
                {
                    break;
                }

                PXThreadYieldToOtherThreads();
            }
#endif

            break;
        }
        case PXThreadStateInit:
        {
            break;
        }
        case PXThreadStateRunning:
        {
#if OSUnix
            return PXActionRefusedNotImplemented;

#elif OSWindows
            const DWORD suspendCount = ResumeThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
            const PXActionResult pxActionResult = PXErrorCurrent(-1 != suspendCount);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }
#endif

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Thread",
                "Resume",
                "Handle:<%p>, ID:<%i>",
                threadHandle,
                pxThread->HandleID
            );
#endif

            break;
        }
        case PXThreadStateWaiting:
        {
            break;
        }
        case PXThreadStateSuspended:
        {
#if OSUnix
            return PXActionRefusedNotImplemented;

#elif OSWindows
            const DWORD result = SuspendThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
            const PXActionResult pxActionResult = PXErrorCurrent(-1 != result);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }
#endif

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Thread",
                "Suspend",
                "Handle:<%p>, ID:<%i>",
                threadHandle,
                pxThread->HandleID
            );
#endif

            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= pxThreadState;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime)
{
    if(pxThread)
    {
        // Thread known
#if OSUnix
        return PXActionRefusedNotImplemented;
#elif OSWindows
        return PXActionRefusedNotImplemented;
#else
        return PXActionRefusedNotImplemented;
#endif
    }
    else
    {
        // Use current thread
#if OSUnix
        return PXActionRefusedNotImplemented;

#elif OSWindows
        Sleep(sleepTime); // Windows XP (+UWP), Kernel32.dll, synchapi.h

        return PXActionSuccessful;
#else
        return PXActionRefusedNotImplemented;
#endif
    }
}

PXActionResult PXAPI PXThreadCurrentProcessorID(PXInt32U* const processorID)
{
    *processorID = 0xFFFFFFFF;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista
    *processorID = GetCurrentProcessorNumber(); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

#else
    return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText* const threadName)
{
    PXThread pxThreadOverride;
    PXClear(PXThread, &pxThreadOverride);

    if (!pxThread)
    {
        pxThread = &pxThreadOverride;
        PXThreadCurrentGet(pxThread);
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

#if WindowsAtleast10

    wchar_t threadNameW[64];
    PXTextCopyAW(threadName->TextA, threadName->SizeUsed, threadNameW, 64);

    const HRESULT resultID = SetThreadDescription(pxThread->Info.Handle.ThreadHandle, threadNameW); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
    const PXActionResult result = PXWindowsHandleErrorFromID(resultID);

    return result;

#else
    typedef struct tagTHREADNAME_INFO
    {
        DWORD dwType; // Must be 0x1000.
        LPCSTR szName; // Pointer to name (in user addr space).
        DWORD dwThreadID; // Thread ID (-1=caller thread).
        DWORD dwFlags; // Reserved for future use, must be zero.
    }
    THREADNAME_INFO;

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = threadName->TextA;
    info.dwThreadID = pxThread->ThreadID;
    info.dwFlags = 0;

    const DWORD MS_VC_EXCEPTION = 0x406D1388;
    const PXSize numberOfArguments = sizeof(info) / sizeof(ULONG_PTR);

#if 0 // Does not compile as well in 32-Bit under no default lib
    __try
    {
        RaiseException(MS_VC_EXCEPTION, 0, 3, (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
#endif

#endif

#else
    return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAPI PXThreadNameGet(PXDebug* const pxDebug,PXThread* const pxThread, PXText* const threadName)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

#if WindowsAtleast10 && 0

    PXBool createdHandle = PXFalse;

    // if we dont have a thread handle and only a thread ID
    if(!pxThread->ThreadHandle)
    {
        pxThread->ThreadHandle = OpenThread(THREAD_ALL_ACCESS, PXFalse, pxThread->ThreadID);
        //PXBool xxa = DuplicateHandle(GetCurrentProcess(), PXNull, GetCurrentProcess(), &pxThread->ThreadHandle, THREAD_QUERY_INFORMATION, FALSE, 0);


        createdHandle = PXTrue;
    }


    wchar_t* tempThreadDescription = 0;
    const HRESULT resultID = GetThreadDescription(pxThread->ThreadHandle, &tempThreadDescription); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
    const PXBool success = SUCCEEDED(resultID) && (tempThreadDescription > 0);

    //PXActionReturnOnError(result);

    if (success)
    {
        threadName->SizeUsed = PXTextCopyWA(tempThreadDescription, 256, threadName->TextA, threadName->SizeAllocated);
    }
    else
    {
        threadName->SizeUsed = PXTextCopyA("[N/A]", 5, threadName->TextA, threadName->SizeAllocated);
    }

    LocalFree(tempThreadDescription);


    if(createdHandle && success)
    {
        CloseHandle(pxThread->ThreadHandle);
        pxThread->ThreadHandle = PXNull;

        return PXActionSuccessful;
    }

#else

#endif

    PXSymbol pxSymbol;


    PXDebugFetchSymbolThread(pxDebug, &pxSymbol, pxThread);



    const PXBool hasFullName = (pxSymbol.LineNumber > 0) && (pxSymbol.NameFile[0] != '\0');

    // [DLLL]
    if(hasFullName)
    {
        PXTextAppendF
        (
            threadName,
            "[%s] %s - %s:%i",
            pxSymbol.NameModule,
            pxSymbol.NameSymbol,
            pxSymbol.NameFile,
            pxSymbol.LineNumber
        );

    }
    else
    {
        PXTextAppendF
        (
            threadName,
            "[%s] %s",
            pxSymbol.NameModule,
            pxSymbol.NameSymbol
        );
    }









    /*
    threadNameInformation.ThreadName.Buffer = subsystemInfomationType;
    threadNameInformation.ThreadName.Length = 20;
    threadNameInformation.ThreadName.MaximumLength = 128;


    const NTSTATUS x = NtQueryInformationThread
    (
       pxThread->ThreadHandle,
       ThreadNameInformation,
       &threadNameInformation,
       ThreadInformationLength,
       &returnLength
    );



    CloseHandle(pxThread->ThreadHandle);
    pxThread->ThreadHandle = PXNull;*/


#else
    return PXActionRefusedNotSupported;
#endif
}

PXThreadHandleID PXAPI PXThreadCurrentID()
{
#if OSUnix
    return getpid();
#elif OSWindows
    return GetCurrentThreadId(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#else
    return -1;
#endif
}

PXProcessThreadHandle PXAPI PXThreadCurrentGet()
{
#if OSUnix
    return pthread_self();
#elif OSWindows
    // Getting the thread handle yields a pseudo handle that is not useful
    return GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#else
    return -1;
#endif
}


#define PXThreadContextUse (1<<0)

typedef struct PXThreadContext32_
{
    int x;
}
PXThreadContext32;

typedef struct PXThreadContext64_
{
 PXInt32U Flags;

    // Parameter adress for integers. What are they for`?
     DWORD64 P1Home;
  DWORD64 P2Home;
  DWORD64 P3Home;
  DWORD64 P4Home;
  DWORD64 P5Home;
  DWORD64 P6Home;

  DWORD   MxCsr; // SSE - float unit flags


    // Code Segment registers.
    WORD  SegCs;
    WORD  SegDs;
    WORD  SegEs;
    WORD  SegFs;
    WORD  SegGs;
    WORD  SegSs; // Stack segment register.


 // General flags
  DWORD   EFlags;

  // Debug register, Used for hardware breakpoints.
  DWORD64 Dr0;
  DWORD64 Dr1;
  DWORD64 Dr2;
  DWORD64 Dr3;
  // Debug status and control registers.
  DWORD64 Dr6;
  DWORD64 Dr7;

  // General-purpose registers.
  DWORD64 RAX; // Accumulator
  DWORD64 RBX; // Base register
  DWORD64 RCX; // Counter register
  DWORD64 RDX; // Data register 

  DWORD64 RSI; // Source index register
  DWORD64 RDI; // Destination index register

  DWORD64 RBP; // Base pointer register.
  DWORD64 RSP; // Stack pointer register.

  // Extended 64-Bit registers
  DWORD64 R8;
  DWORD64 R9;
  DWORD64 R10;
  DWORD64 R11;
  DWORD64 R12;
  DWORD64 R13;
  DWORD64 R14;
  DWORD64 R15;

  DWORD64 RIP; // Instruction pointer register.
    
union 
{
    XMM_SAVE_AREA32 FltSave;
    NEON128         Q[16];
    ULONGLONG       D[32];
    struct 
    {
      M128A Header[2];
      M128A Legacy[8];
      M128A Xmm0;
      M128A Xmm1;
      M128A Xmm2;
      M128A Xmm3;
      M128A Xmm4;
      M128A Xmm5;
      M128A Xmm6;
      M128A Xmm7;
      M128A Xmm8;
      M128A Xmm9;
      M128A Xmm10;
      M128A Xmm11;
      M128A Xmm12;
      M128A Xmm13;
      M128A Xmm14;
      M128A Xmm15;
    };
    DWORD           S[32];
  };
  M128A   VectorRegister[26];
  DWORD64 VectorControl;
  DWORD64 DebugControl;
  DWORD64 LastBranchToRip;
  DWORD64 LastBranchFromRip;
  DWORD64 LastExceptionToRip;
  DWORD64 LastExceptionFromRip;
}
PXThreadContext64;


// https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/x86-architecture
typedef struct PXThreadContext_
{
    union
    {
        PXThreadContext64 X64;
        PXThreadContext32 X86;
    }
}
PXThreadContext;

#if OSUnix
#include <sys/user.h>
#elif OSWindows
#endif


PXActionResult PXAPI PXThreadContextGet(PXThreadContext* const pxThreadContext, PXThreadHandle pxThreadHandle)
{
#if OSUnix
    // getcontext(); // ucontext.h // Introduced with POSIX:2001, removed in POSIX:2008

    // Data can be found here:
    // /proc/[pid]/task/[tid]/status
    
    struct user_regs_struct regs;

    const long attachResultID = ptrace(PTRACE_ATTACH, tid, NULL, NULL);
    const PXBool attachSuccess = attachResultID == -1;
    
    if (!attachSuccess) 
    {
        perror("ptrace attach");
        return;
    }
    
    waitpid(tid, NULL, 0);

    const long getRegisterID = ptrace(PTRACE_GETREGS, tid, NULL, &regs);
    const PXBool getRegisterSuccess = getRegisterID == -1;
    
    if (!getRegisterSuccess) 
    {
        perror("ptrace getregs");
        ptrace(PTRACE_DETACH, tid, NULL, NULL);
        return;
    }
    printf("RIP: %llx\n", regs.rip);
    // Print other registers as needed
    ptrace(PTRACE_DETACH, tid, NULL, NULL);
    
#elif OSWindows
    CONTEXT context;
    
    const BOOL getResultID = GetThreadContext(pxThreadHandle, &context); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h    
    const PXActionResult getResult = PXErrorCurrent(getResultID);

    return getResult;
#else
    return PXNotimplemented;
#endif    
}


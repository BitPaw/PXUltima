#include "PXThread.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/PXOS.h>


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


PXResult PXAPI  PXThreadCreate(PXThread* const pxThread, const char* const threadName, const PXProcessHandle targetProcessHandle, ThreadFunction threadFunction, void* parameter, const PXI32U behaviour)
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
        pxThread->Info.Behaviour |= PXExecuteStateSuspended;
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
        "OK. HANDLE:<%p>, ID:<%5.5i>, Name:<%s>",
        pxThread->Info.Handle.ThreadHandle,
        pxThread->HandleID,
        threadName
    );
#endif

    if(!(PXThreadBehaviourCreateSuspended & pxThread->Info.Behaviour))
    {
        pxThread->Info.Behaviour |= PXExecuteStateRunning;
    }

    // Name thread if possible
    {
        PXText pxText;
        PXTextConstructFromAdressA(&pxText, threadName, 0, PXTextLengthUnkown);

        PXThreadNameSet(pxThread, &pxText);
    }

}

PXResult PXAPI  PXThreadExitCurrent(const PXI32U exitCode)
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



PXResult PXAPI  PXThreadOpen(PXThread* const pxThread)
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

PXResult PXAPI  PXThreadPrioritySet(PXThread* pxThread, const PXThreadPriorityMode pxThreadPriorityMode)
{
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrent(&dummyThread);
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

PXResult PXAPI  PXThreadPriorityGet(PXThread* pxThread, PXThreadPriorityMode* const pxThreadPriorityMode)
{
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrent(&dummyThread);
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

PXResult PXAPI  PXThreadStateChange(PXThread* const pxThread, const PXI32U pxThreadState)
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

    if(!threadHandle)
    {
        DebugBreak();
    }

    switch(pxThreadState)
    {
        case PXExecuteStateWaiting:
        { 
            PXThreadWait(pxThread);
            break;
        }
        case PXExecuteStateRunning:
        {
            PXThreadResume(pxThread);
            break;
        }
        case PXExecuteStateSuspended:
        {
            PXThreadSuspend(pxThread);
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime)
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

PXResult PXAPI  PXThreadCurrentProcessorID(PXI32U* const processorID)
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

PXResult PXAPI  PXThreadNameSet(PXThread* pxThread, PXText* const threadName)
{
    PXThread pxThreadOverride;
    PXClear(PXThread, &pxThreadOverride);

    if (!pxThread)
    {
        pxThread = &pxThreadOverride;
        PXThreadCurrent(pxThread);
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

#if WindowsAtleast10

    wchar_t threadNameW[64];
    PXTextCopyAW(threadName->A, threadName->SizeUsed, threadNameW, 64);

    const HRESULT resultID = SetThreadDescription(pxThread->Info.Handle.ThreadHandle, threadNameW); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
    const PXActionResult result = PXErrorFromHRESULT(resultID);

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
    info.szName = threadName->A;
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

PXResult PXAPI  PXThreadNameGet(PXDebug* const pxDebug,PXThread* const pxThread, PXText* const threadName)
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
        threadName->SizeUsed = PXTextCopyWA(tempThreadDescription, 256, threadName->A, threadName->SizeAllocated);
    }
    else
    {
        threadName->SizeUsed = PXTextCopyA("[N/A]", 5, threadName->A, threadName->SizeAllocated);
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
        PXAppendF
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
        PXAppendF
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

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}
#define PXThreadContextUse (1<<0)

typedef struct PXThreadContext32_
{
    PXByte ExtendedRegisters[512];
    //PXF32ING_SAVE_AREA PXF32Save;

    PXI32U Dr0;
    PXI32U Dr1;
    PXI32U Dr2;
    PXI32U Dr3;
    PXI32U Dr6;
    PXI32U Dr7;

    PXI32U SegGs;
    PXI32U SegFs;
    PXI32U SegEs;
    PXI32U SegDs;

    PXI32U EDI;
    PXI32U ESI;
    PXI32U EBX;
    PXI32U EDX;
    PXI32U ECX;
    PXI32U EAX;

    PXI32U EBP;
    PXI32U EIP;
    PXI32U SegCs;
    PXI32U EFlags;
    PXI32U ESP;
    PXI32U SegSs;
}
PXThreadContext32;

typedef struct PXThreadContext64_
{
    // Parameter adress for integers. What are they for`?
     PXI64U P1Home;
     PXI64U P2Home;
  PXI64U P3Home;
  PXI64U P4Home;
  PXI64U P5Home;
  PXI64U P6Home;

  PXI32U   MxCsr; // SSE - PXF32 unit flags


    // Code Segment registers.
  PXI16U  SegCs;
    PXI16U  SegDs;
    PXI16U  SegEs;
    PXI16U  SegFs;
    PXI16U  SegGs;
    PXI16U  SegSs; // Stack segment register.


 // General flags
    PXI32U   EFlags;

  // Debug register, Used for hardware breakpoints.
  PXI64U Dr0;
  PXI64U Dr1;
  PXI64U Dr2;
  PXI64U Dr3;
  // Debug status and control registers.
  PXI64U Dr6;
  PXI64U Dr7;

  // General-purpose registers.
  PXI64U RAX; // Accumulator
  PXI64U RBX; // Base register
  PXI64U RCX; // Counter register
  PXI64U RDX; // Data register 

  PXI64U RSI; // Source index register
  PXI64U RDI; // Destination index register

  PXI64U RBP; // Base pointer register.
  PXI64U RSP; // Stack pointer register.

  // Extended 64-Bit registers
  PXI64U R8;
  PXI64U R9;
  PXI64U R10;
  PXI64U R11;
  PXI64U R12;
  PXI64U R13;
  PXI64U R14;
  PXI64U R15;

  PXI64U RIP; // Instruction pointer register.
    
union 
{
   // XMM_SAVE_AREA32 FltSave;
    //NEON128         Q[16];
    PXI64U       D[32];
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
  PXI64U VectorControl;
  PXI64U DebugControl;
  PXI64U LastBranchToRip;
  PXI64U LastBranchFromRip;
  PXI64U LastExceptionToRip;
  PXI64U LastExceptionFromRip;
}
PXThreadContext64;


// https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/x86-architecture
typedef struct PXThreadContext_
{
    union
    {
        PXThreadContext64 X64;
        PXThreadContext32 X86;
    };
}
PXThreadContext;

#if OSUnix
#include <sys/user.h>
#elif OSWindows
#endif


PXResult PXAPI  PXThreadContextGet(PXThreadContext* const pxThreadContext, const PXProcessThreadHandle pxThreadHandle)
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
    PXClear(CONTEXT, &context);
    context.ContextFlags = CONTEXT_ALL;
    
    const BOOL getResultID = GetThreadContext(pxThreadHandle, &context); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h    
    const PXActionResult getResult = PXErrorCurrent(getResultID);

    if(PXActionSuccessful != getResult)
    {
        return getResult;
    }

#if OS32B

    pxThreadContext->X86.Dr0 = context.Dr0;
    pxThreadContext->X86.Dr1 = context.Dr1;
    pxThreadContext->X86.Dr2 = context.Dr2;
    pxThreadContext->X86.Dr3 = context.Dr3;
    pxThreadContext->X86.Dr6 = context.Dr6;
    pxThreadContext->X86.Dr7 = context.Dr7;

    pxThreadContext->X86.SegGs = context.SegGs;
    pxThreadContext->X86.SegFs = context.SegFs;
    pxThreadContext->X86.SegEs = context.SegEs;
    pxThreadContext->X86.SegDs = context.SegDs;

    pxThreadContext->X86.EDI = context.Edi;
    pxThreadContext->X86.ESI = context.Esi;
    pxThreadContext->X86.EBX = context.Ebx;
    pxThreadContext->X86.EDX = context.Edx;
    pxThreadContext->X86.ECX = context.Ecx;
    pxThreadContext->X86.EAX = context.Eax;

    pxThreadContext->X86.EBP = context.Ebp;
    pxThreadContext->X86.EIP = context.Eip;
    pxThreadContext->X86.SegCs = context.SegCs;
    pxThreadContext->X86.EFlags = context.EFlags;
    pxThreadContext->X86.ESP = context.Esp;
    pxThreadContext->X86.SegSs = context.SegSs;

    //pxThreadContext->X86.PXF32Save = context.PXF32Save;
    PXCopyList(BYTE, MAXIMUM_SUPPORTED_EXTENSION, context.ExtendedRegisters, pxThreadContext->X86.ExtendedRegisters);
 
#elif OS64B
#endif



    return getResult;
#else
    return PXNotimplemented;
#endif    
}


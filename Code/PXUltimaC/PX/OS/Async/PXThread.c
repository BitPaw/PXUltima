#include "PXThread.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/PXOS.h>


#if OSUnix
#include <sys/resource.h> // getpriority, setpriority
#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/syscall.h> // SYS_gettid
#elif OSWindows
#include <Windows.h>
#include <dbghelp.h>
#include <winternl.h>
#include <winternl.h>
#endif





const char PXThreadName[] = "Thread";
const PXI8U PXThreadLength = sizeof(PXThreadName);
const PXECSRegisterInfoStatic PXThreadRegisterInfoStatic =
{
    {sizeof(PXThreadName), sizeof(PXThreadName), PXThreadName, TextFormatASCII},
    sizeof(PXThread),
    __alignof(PXThread),
    PXECSTypeResource,
    PXThreadCreate,
    PXThreadDestruct,
    PXNull
};
PXECSRegisterInfoDynamic PXThreadRegisterInfoDynamic;

void PXAPI PXThreadRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXThreadRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXThreadRegisterInfoDynamic;
}

PXThreadState PXAPI PXThreadStateGet(const PXThread PXREF pxThread)
{
    if(!pxThread)
    {
        return PXThreadStateNULL;
    }

#if OSUnix
#elif OSWindows
    HANDLE handle = pxThread->Handle.Data;
    DWORD milliseconds = 0;
    DWORD result = WaitForSingleObject(handle, milliseconds);
  
    if(result == WAIT_OBJECT_0) 
    {  
        return PXThreadStateFinished; // Thread is done
    }
    else if(result == WAIT_TIMEOUT) 
    {
  
        return PXThreadStateRunning; // Thread is still running
    }
#endif

    return PXThreadStateInvalid;
}

// Note, TerminateThread() should not be used, it can lead to a memory leak in Windows-XP
void PXAPI PXThreadDestruct(PXThread PXREF pxThread)
{
    if (!pxThread)
    {
        return;
    }

    if (!pxThread->Handle.Data)
    {
        return;
    }

#if OSUnix

#elif OSWindows
    const HANDLE currentThread = GetCurrentThread();
    const PXBool isTargetCurrentThread = currentThread == pxThread->Handle.Data;
    const PXBool success = CloseHandle(pxThread->Handle.Data); // Windows 2000 Professional (+UWP), Kernel32.dll, handleapi.h

    if (!success)
    {
        return;
    }

#endif

    PXClear(PXThread, pxThread);
}


#if OSWindows
void PXAPI PXThreadConstructFromHandle(PXThread PXREF pxThread, HANDLE threadHandle)
{
    pxThread->Handle.Data = threadHandle;

#if WindowsAtleastVista
    pxThread->ID.Data = GetThreadId(threadHandle); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h
#else
    pxThread->ID.Data = 0xFFFFFFFF;
#endif
}
PXResult PXAPI PXThreadContextFromSystem(PXThreadContext PXREF pxThreadContext, const CONTEXT PXREF windowsContext)
{
#if OS32B

    pxThreadContext->X86.Dr0 = windowsContext->Dr0;
    pxThreadContext->X86.Dr1 = windowsContext->Dr1;
    pxThreadContext->X86.Dr2 = windowsContext->Dr2;
    pxThreadContext->X86.Dr3 = windowsContext->Dr3;
    pxThreadContext->X86.Dr6 = windowsContext->Dr6;
    pxThreadContext->X86.Dr7 = windowsContext->Dr7;

    pxThreadContext->X86.SegGs = windowsContext->SegGs;
    pxThreadContext->X86.SegFs = windowsContext->SegFs;
    pxThreadContext->X86.SegEs = windowsContext->SegEs;
    pxThreadContext->X86.SegDs = windowsContext->SegDs;
    pxThreadContext->X86.SegCs = windowsContext->SegCs;
    pxThreadContext->X86.SegSs = windowsContext->SegSs;
    pxThreadContext->X86.EFlags = windowsContext->EFlags;

    pxThreadContext->X86.EDI = windowsContext->Edi;
    pxThreadContext->X86.ESI = windowsContext->Esi;
    pxThreadContext->X86.EBX = windowsContext->Ebx;
    pxThreadContext->X86.EDX = windowsContext->Edx;
    pxThreadContext->X86.ECX = windowsContext->Ecx;
    pxThreadContext->X86.EAX = windowsContext->Eax;
    pxThreadContext->X86.EBP = windowsContext->Ebp;
    pxThreadContext->X86.EIP = windowsContext->Eip;
    pxThreadContext->X86.ESP = windowsContext->Esp;


    PXCopyList(BYTE, MAXIMUM_SUPPORTED_EXTENSION, windowsContext->ExtendedRegisters, pxThreadContext->X86.ExtendedRegisters);

#elif OS64B

    pxThreadContext->X64.Dr0 = windowsContext->Dr0;
    pxThreadContext->X64.Dr1 = windowsContext->Dr1;
    pxThreadContext->X64.Dr2 = windowsContext->Dr2;
    pxThreadContext->X64.Dr3 = windowsContext->Dr3;
    pxThreadContext->X64.Dr6 = windowsContext->Dr6;
    pxThreadContext->X64.Dr7 = windowsContext->Dr7;

    pxThreadContext->X64.SegGs = windowsContext->SegGs;
    pxThreadContext->X64.SegFs = windowsContext->SegFs;
    pxThreadContext->X64.SegEs = windowsContext->SegEs;
    pxThreadContext->X64.SegDs = windowsContext->SegDs;
    pxThreadContext->X64.SegCs = windowsContext->SegCs;
    pxThreadContext->X64.SegSs = windowsContext->SegSs;
    pxThreadContext->X64.EFlags = windowsContext->EFlags;

    pxThreadContext->X64.RDI = windowsContext->Rdi;
    pxThreadContext->X64.RSI = windowsContext->Rsi;
    pxThreadContext->X64.RBX = windowsContext->Rbx;
    pxThreadContext->X64.RDX = windowsContext->Rdx;
    pxThreadContext->X64.RCX = windowsContext->Rcx;
    pxThreadContext->X64.RAX = windowsContext->Rax;
    pxThreadContext->X64.RBP = windowsContext->Rbp;
    pxThreadContext->X64.RIP = windowsContext->Rip;
    pxThreadContext->X64.RSP = windowsContext->Rsp;

    //PXCopyList(BYTE, MAXIMUM_SUPPORTED_EXTENSION, context.ExtendedRegisters, pxThreadContext->X86.ExtendedRegisters);

#endif

    return PXResultOK;
}
#endif


PXResult PXAPI PXThreadCreate(PXThread** pxThreadREF, PXThreadCreateInfo PXREF pxThreadCreateInfo)
{
    PXThread* pxThread = PXNull;

    if(!(pxThreadREF && pxThreadCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxThreadCreateInfo->Info.Static = &PXThreadRegisterInfoStatic;
    pxThreadCreateInfo->Info.Dynamic = &PXThreadRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxThreadREF, pxThreadCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxThread = *pxThreadREF;
    //PXECSInfoFlagStateAdd(&pxThread->Info, pxThreadCreateInfo->);

    const PXBool createSuspended = pxThreadCreateInfo->CreateSuspended;

#if OSUnix

    const int errorID = pthread_create
    (
        &pxThread->Handle.Data,
        0,
        pxThreadCreateInfo->ThreadFunction,
        (void*)pxThreadCreateInfo->Parameter
    );
    const PXResult threadCreateResult = PXErrorCurrent(0 != errorID);


#elif OSWindows


    LPSECURITY_ATTRIBUTES threadAttributes = PXNull;
    SIZE_T stackSize = 0;
    LPTHREAD_START_ROUTINE startAddress = (LPTHREAD_START_ROUTINE)pxThreadCreateInfo->ThreadFunction;
    LPVOID parameter = (LPVOID)pxThreadCreateInfo->Parameter;
    DWORD creationFlags = 0;
    LPDWORD threadId = &pxThread->ID.Data;

    if(createSuspended)
    {
        creationFlags |= CREATE_SUSPENDED;
    }

    if(!pxThreadCreateInfo->TargetProcessHandle.Data) //  Target own process
    {
        pxThread->Handle.Data = CreateThread
        (
            threadAttributes,
            stackSize,
            startAddress,
            parameter,
            creationFlags,
            threadId
        ); // Windows NT (+UWP), Kernel32.dll, processthreadsapi.h
    }
    else
    {
        pxThread->Handle.Data = CreateRemoteThread
        (
            pxThreadCreateInfo->TargetProcessHandle.Data,
            threadAttributes,
            stackSize,
            startAddress,
            parameter,
            creationFlags,
            threadId
        ); // Windows XP, Kernel32.dll, processthreadsapi.h
    }

    const PXResult threadCreateResult = PXErrorCurrent(PXNull != pxThread->Handle.Data);
#else
    const PXResult threadCreateResult = PXResultRefusedNotSupported;
#endif

    if(PXResultOK != threadCreateResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXThreadName,
            "Create",
            "Failed to create Name:<%s>",
            pxThreadCreateInfo->Info.Name
        );
#endif
        return threadCreateResult;
    }

    if(createSuspended)
    {
        PXECSInfoStateSet(&pxThread->Info, PXECSFlagStateSUSPENDED);
    }
    else
    {
        PXECSInfoStateSet(&pxThread->Info, PXECSFlagStateRUNNING);
    }

    // Name thread if possible
    PXThreadNameSet(pxThread, &pxThreadCreateInfo->Info.Name);

    return PXResultOK;
}

PXResult PXAPI PXThreadExitCurrent(const PXI32U exitCode)
{
#if OSUnix
    return PXResultRefusedNotImplemented;

#elif OSWindows
    ExitThread(exitCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

    return PXResultOK;
#else
    return PXResultRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadOpen(PXThread PXREF pxThread, const PXThreadID pxThreadID)
{
    if(!pxThread)
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;

#if OSUnix
    pxResult = PXResultRefusedNotImplemented;

#elif OSWindows
    DWORD dwDesiredAccess = THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME;
    BOOL  bInheritHandle = FALSE;

    const HANDLE threadHandle = OpenThread(dwDesiredAccess, bInheritHandle, pxThreadID.Data); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    pxResult = PXErrorCurrent(PXNull != threadHandle);
#else
    pxResult = PXResultNotSupportedByOperatingSystem;
#endif

    return pxResult;
}

int PXAPI PXThreadPriorityToID(const PXThreadPriorityMode pxThreadPriorityMode)
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

PXResult PXAPI PXThreadPrioritySet(PXThread* pxThread, const PXThreadPriorityMode pxThreadPriorityMode)
{
    PXResult pxResult = PXResultInvalid;
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrent(&dummyThread);
        pxThread = &dummyThread;
    }

    const int threadPriority = PXThreadPriorityToID(pxThreadPriorityMode);

#if OSUnix
    const int newPriority = setpriority(0, pxThread->Handle.Data, threadPriority);
    pxResult = PXErrorCurrent(-1 != newPriority);

#elif OSWindows
    // SetPriorityClass() also exists, but is it needed?
    const BOOL success = SetThreadPriority(pxThread->Handle.Data, threadPriority);
    pxResult = PXErrorCurrent(FALSE != success);

#else
    pxResult = PXResultRefusedNotSupportedByOperatingSystem;
#endif

    return pxResult;
}

PXResult PXAPI PXThreadPriorityGet(PXThread* pxThread, PXThreadPriorityMode PXREF pxThreadPriorityMode)
{
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrent(&dummyThread);
        pxThread = &dummyThread;
    }

    int priorityID = 0;

#if OSUnix
    priorityID = getpriority(0, pxThread->Handle.Data); // sys/resource.h
#elif OSWindows
    priorityID = GetThreadPriority(pxThread->Handle.Data);
#endif

    return PXResultOK;
}

PXResult PXAPI PXThreadStateChange(PXThread PXREF pxThread, const PXI32U pxThreadState)
{
    if(!pxThread)
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxThread->Handle.Data)
    {
        return PXResultRefusedObjectNotCreated;
    }

    switch(pxThreadState)
    {
        case PXECSFlagStateWAITING:
        {
            PXThreadWait(pxThread);
            break;
        }
        case PXECSFlagStateRUNNING:
        {
            PXThreadResume(pxThread);
            break;
        }
        case PXECSFlagStateSUSPENDED:
        {
            PXThreadSuspend(pxThread);
            break;
        }
        default:
        {
            return PXResultRefusedParameterInvalid;
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXThreadSleep(PXThread PXREF pxThread, const PXSize sleepTime)
{
    if(pxThread)
    {
        // Thread known
#if OSUnix
        return PXResultRefusedNotImplemented;
#elif OSWindows
        return PXResultRefusedNotImplemented;
#else
        return PXResultRefusedNotImplemented;
#endif
    }
    else
    {
        // Use current thread
#if OSUnix
        return PXResultRefusedNotImplemented;

#elif OSWindows
        Sleep(sleepTime); // Windows XP (+UWP), Kernel32.dll, synchapi.h

        return PXResultOK;
#else
        return PXResultRefusedNotImplemented;
#endif
    }
}

PXResult PXAPI PXThreadCurrentProcessorID(PXI32U PXREF processorID)
{
    *processorID = 0xFFFFFFFF;

#if OSUnix
    return PXResultRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista
    *processorID = GetCurrentProcessorNumber(); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h

    return PXResultOK;
#else
    return PXResultRefusedNotSupportedByOperatingSystem;
#endif

#else
    return PXResultRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText PXREF threadName)
{
    PXThread pxThreadOverride;
    PXClear(PXThread, &pxThreadOverride);

    PXBool hasName = PXTextHasData(threadName);

    // If we dont have a name, we cant set it.
    if(!hasName)
    {
        return PXResultOK;
    }

    // If we didnt spesify any thread, get the current one.
    if (!pxThread)
    {
        pxThread = &pxThreadOverride;
        PXThreadCurrent(pxThread);
    }

#if OSUnix
    return PXResultRefusedNotImplemented;
#elif OSWindows

#if WindowsAtleast10

    wchar_t threadNameW[64];
    PXTextCopyAW(threadName->A, threadName->SizeUsed, threadNameW, 64);

    const HRESULT resultID = SetThreadDescription(pxThread->Handle.Data, threadNameW); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult result = PXErrorFromHRESULT(resultID);

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
    return PXResultRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadNameGet(PXDebug PXREF pxDebug,PXThread PXREF pxThread, PXText PXREF threadName)
{
#if OSUnix
    return PXResultRefusedNotImplemented;
#elif OSWindows

#if WindowsAtleast10

    HRESULT resultID = 0;
    PXResult pxResult = PXResultInvalid;

    PXBool createdHandle = PXFalse;

    // if we dont have a thread handle and only a thread ID
    if(!pxThread->Handle.Data)
    {
        pxThread->Handle.Data = OpenThread(THREAD_ALL_ACCESS, PXFalse, pxThread->ID.Data);
        //PXBool xxa = DuplicateHandle(GetCurrentProcess(), PXNull, GetCurrentProcess(), &pxThread->ThreadHandle, THREAD_QUERY_INFORMATION, FALSE, 0);


        createdHandle = PXTrue;
    }

    wchar_t* tempThreadDescription = 0;
    resultID = GetThreadDescription(pxThread->Handle.Data, &tempThreadDescription); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
    pxResult = PXErrorFromHRESULT(resultID);

    if (PXResultOK != resultID)
    {
        threadName->SizeUsed = PXTextCopyWA(tempThreadDescription, 256, threadName->A, threadName->SizeAllocated);
    }
    else
    {
        threadName->SizeUsed = PXTextCopyA("[N/A]", 5, threadName->A, threadName->SizeAllocated);
    }

    LocalFree(tempThreadDescription);

    if(createdHandle && (PXResultOK != resultID))
    {
        CloseHandle(pxThread->Handle.Data);
        pxThread->Handle.Data = PXNull;

        return PXResultOK;
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

    return PXResultOK;

#else
    return PXResultRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadContextGet(PXThreadContext PXREF pxThreadContext, const PXThreadID pxThreadID)
{
#if OSUnix
    // getcontext(); // ucontext.h // Introduced with POSIX:2001, removed in POSIX:2008

    // Data can be found here:
    // /proc/[pid]/task/[tid]/status

    struct user_regs_struct regs;

    const long attachResultID = ptrace(PTRACE_ATTACH, pxThreadID.Data, NULL, NULL);
    const PXBool attachSuccess = attachResultID == -1;

    if (!attachSuccess)
    {
        perror("ptrace attach");
        return;
    }

    waitpid(pxThreadID.Data, NULL, 0);

    const long getRegisterID = ptrace(PTRACE_GETREGS, pxThreadID.Data, NULL, &regs);
    const PXBool getRegisterSuccess = getRegisterID == -1;

    if (!getRegisterSuccess)
    {
        perror("ptrace getregs");
        ptrace(PTRACE_DETACH, pxThreadID.Data, NULL, NULL);
        return;
    }
    printf("RIP: %llx\n", regs.rip);
    // Print other registers as needed
    ptrace(PTRACE_DETACH, pxThreadID.Data, NULL, NULL);

#elif OSWindows
    CONTEXT context;
    PXClear(CONTEXT, &context);
    context.ContextFlags = CONTEXT_ALL;

    PXThread pxThread;


    PXThreadOpen(&pxThread, pxThreadID);

    HANDLE handle = pxThread.Handle.Data;
    SuspendThread(handle);
    const BOOL getResultID = GetThreadContext(handle, &context); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult getResult = PXErrorCurrent(getResultID);

    ResumeThread(handle);
    CloseHandle(handle);

    if(PXResultOK != getResult)
    {
        return getResult;
    }

    PXThreadContextFromSystem(pxThreadContext, &context);

    return getResult;
#else
    return PXNotimplemented;
#endif
}

PXResult PXAPI PXThreadCurrent(PXThread PXREF pxThread)
{
    PXClear(PXThread, pxThread);

    pxThread->Handle = PXThreadCurrentHandle();
    pxThread->ID = PXThreadCurrentID();

    return PXResultOK;
}

PXThreadHandle PXAPI PXThreadCurrentHandle()
{
    PXThreadHandle pxThreadHandle;

#if OSUnix
    pxThreadHandle.Data = pthread_self();
#elif OSWindows
    // Getting the thread handle yields a pseudo handle that is not useful
    pxThreadHandle.Data = GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

    return pxThreadHandle;
}

PXThreadID PXAPI PXThreadCurrentID()
{
    PXThreadID pxThreadID;

#if OSUnix
    pxThreadID.Data = syscall(SYS_gettid); // Linux, Is this equal?
#elif OSWindows
    pxThreadID.Data = GetCurrentThreadId(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

    return pxThreadID;
}

PXResult PXAPI PXThreadResume(PXThread PXREF pxThread)
{
    if(!pxThread)
    {
        return PXResultRefusedParameterNull;
    }

    PXECSInfoStateSet(&pxThread->Info, PXECSFlagStateRUNNING);

#if OSUnix
    return PXResultRefusedNotImplemented;

#elif OSWindows
    const HANDLE threadHandle = pxThread->Handle.Data;
    const DWORD suspendCount = ResumeThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxResult = PXErrorCurrent(-1 != suspendCount);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXThreadName,
        "Resume",
        "Handle:<%p>, ID:<%i>",
        pxThread->Handle.Data,
        pxThread->ID.Data
    );
#endif
}

PXResult PXAPI PXThreadSuspend(PXThread PXREF pxThread)
{
    PXECSInfoStateSet(&pxThread->Info, PXECSFlagStateSUSPENDED);

#if OSUnix
    return PXResultRefusedNotImplemented;

#elif OSWindows
    const DWORD result = SuspendThread(pxThread->Handle.Data); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxResult = PXErrorCurrent(-1 != result);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXThreadName,
        "Suspend",
        "Handle:<%p>, ID:<%i>",
        pxThread->Handle.Data,
        pxThread->ID.Data
    );
#endif
}

PXResult PXAPI PXThreadWait(PXThread PXREF pxThread)
{
    PXECSInfoStateSet(&pxThread->Info, PXECSFlagStateWAITING);

    pxThread->ReturnResultCode.Data = 0; // Reset Code

#if OSUnix
    const int resultID = pthread_join(pxThread->Handle.Data, &pxThread->ReturnResultCode); // pthread.h
    const PXBool success = 0 == resultID;

    if(!success)
    {
        return PXErrorCodeFromID(resultID);
    }

    return PXResultOK;

#elif OSWindows

    const PXThreadHandle threadHandle = pxThread->Handle;
    const DWORD resultID = WaitForSingleObject(threadHandle.Data, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

    for(;;)
    {
        const BOOL result = GetExitCodeThread(threadHandle.Data, &pxThread->ReturnResultCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
        const PXBool isDone = STILL_ACTIVE != pxThread->ReturnResultCode.Data;

        if(isDone)
        {
            break;
        }

        PXThreadYieldToOtherThreads();
    }
#endif
}

PXResult PXAPI PXThreadCPUCoreAffinitySet(PXThread PXREF pxThread, const PXI16U coreIndex)
{
    PXResult result = PXResultInvalid;

    //MAXIMUM_PROCESSORS;

#if OSUnix

    // TODO: ...

#elif OSWindows
    const DWORD resultID = SetThreadIdealProcessor
    (
        pxThread->Handle.Data,
        coreIndex
    ); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXBool successful = (DWORD)-1 != resultID;
    result = PXErrorCurrent(successful);
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXThreadName,
        "Affinity",
        "PXID:<%4i>, %3i",
        pxThread->Info.ID,
        coreIndex
    );
#endif

    /*
    DWORD_PTR SetThreadAffinityMask(
        [in] HANDLE    hThread,
        [in] DWORD_PTR dwThreadAffinityMask
    ); // Windows XP (+UWP), Kernel32.dll, winbase.h
    */

    return result;
}

PXResult PXAPI PXThreadYieldToOtherThreads()
{
    PXResult pxResult;

#if OSUnix
    const int yieldResultID =
#if 0
        pthread_yield(); // [deprecated] pthread.h
#else
        sched_yield(); // sched.h, libc, sets errno but on linux never fails
#endif
    pxResult = PXErrorCurrent(0 != yieldResultID);

#elif OSWindows
    // UmsThreadYield() // Windows 7 (64-Bit only), Kernel32.dll, winbase.h [Debcricated in Windows 11]

    // Yield to any thread in the same core.
    const BOOL switchSuccessful = SwitchToThread(); // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h

    if(!switchSuccessful)
    {
        // Yield to any process that is ready
        PXThreadSleep(PXNull,0);
    }

    pxResult = PXResultOK; // cant fail

#else
    pxResult = PXResultNotSupportedByOperatingSystem;
#endif

    return pxResult;
}

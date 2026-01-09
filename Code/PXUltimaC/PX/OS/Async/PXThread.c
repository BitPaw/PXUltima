#include "PXThread.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Debug/PXDebug.h>
#include <PX/OS/PXOS.h>


#if OSUnix
#include <sys/resource.h> // getpriority, setpriority
#include <sys/user.h>
#elif OSWindows
#include <Windows.h>
#include <dbghelp.h>
#include <winternl.h>
#include <winternl.h>
#endif


typedef struct PXThread_
{
    PXECSInfo Info;

    PXThreadResult ReturnResultCode;

#if OSUnix
    PXI32U ThreadID;
#elif OSWindows
    DWORD ThreadID;
    HANDLE ThreadHandle;
#endif 
}
PXThread;


const char PXThreadName[] = "Thread";
const PXI8U PXThreadLength = sizeof(PXThreadName);
const PXECSRegisterInfoStatic PXThreadRegisterInfoStatic =
{
    {sizeof(PXThreadName), sizeof(PXThreadName), PXThreadName, TextFormatASCII},
    sizeof(PXThread),
    __alignof(PXThread),
    PXECSTypeResource,
    PXThreadCreate
};
PXECSRegisterInfoDynamic PXThreadRegisterInfoDynamic;

PXResult PXAPI PXThreadRegisterToECS()
{
    PXECSRegister(&PXThreadRegisterInfoStatic, &PXThreadRegisterInfoDynamic);

    return PXResultOK;
}

// Note, TerminateThread() should not be used, it can lead to a memory leak in Windows-XP
void PXAPI PXThreadDestruct(PXThread PXREF pxThread)
{
    if (!pxThread)
    {
        return;
    }

    if (!pxThread->ThreadHandle)
    {
        return;
    }

#if OSUnix

#elif OSWindows
    const HANDLE currentThread = GetCurrentThread();
    const PXBool isTargetCurrentThread = currentThread == pxThread->ThreadHandle;
    const PXBool success = CloseHandle(pxThread->ThreadHandle); // Windows 2000 Professional (+UWP), Kernel32.dll, handleapi.h

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
    pxThread->ThreadHandle = threadHandle;

#if WindowsAtleastVista
    pxThread->ThreadID = GetThreadId(threadHandle); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h
#else
    pxThread->ThreadID = 0xFFFFFFFF;
#endif
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
    PXECSCreate(pxThreadREF, pxThreadCreateInfo);

    pxThread = *pxThreadREF;
    pxThread->Info.Behaviour = pxThreadCreateInfo->Behaviour;

#if OSUnix

    const int errorID = pthread_create
    (
        &pxThread->Info.Handle.ThreadHandle,
        0,
        threadFunction,
        (void*)parameter
    );
    const PXResult threadCreateResult = PXErrorCurrent(0 != errorID);


#elif OSWindows


    LPSECURITY_ATTRIBUTES lpThreadAttributes = PXNull;
    SIZE_T dwStackSize = 0;
    LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)pxThreadCreateInfo->ThreadFunction;
    LPVOID lpParameter = (LPVOID)pxThreadCreateInfo->Parameter;
    DWORD dwCreationFlags = 0;
    LPDWORD lpThreadId = &pxThread->ThreadID;

    if(PXThreadBehaviourCreateSuspended & pxThread->Info.Behaviour)
    {
        dwCreationFlags |= CREATE_SUSPENDED;
        pxThread->Info.Behaviour |= PXExecuteStateSuspended;
    }

    if(!pxThreadCreateInfo->TargetProcessHandle) //  Target own process
    {
        pxThread->ThreadHandle = CreateThread
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
        pxThread->ThreadHandle = CreateRemoteThread // Windows XP, Kernel32.dll, processthreadsapi.h
        (
            pxThreadCreateInfo->TargetProcessHandle,
            lpThreadAttributes,
            dwStackSize,
            lpStartAddress,
            lpParameter,
            dwCreationFlags,
            lpThreadId
        );
    }
   
    const PXResult threadCreateResult = PXErrorCurrent(PXNull != pxThread->ThreadHandle);
#else
    const PXResult threadCreateResult = PXActionRefusedNotSupported;
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

    if(!(PXThreadBehaviourCreateSuspended & pxThread->Info.Behaviour))
    {
        pxThread->Info.Behaviour |= PXExecuteStateRunning;
    }

    // Name thread if possible
    PXThreadNameSet(pxThread, &pxThreadCreateInfo->Info.Name);
}

PXResult PXAPI PXThreadExitCurrent(const PXI32U exitCode)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    ExitThread(exitCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

    return PXResultOK;
#else
    return PXActionRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadOpen(PXThread PXREF pxThread)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    DWORD dwDesiredAccess = 0;
    BOOL  bInheritHandle = 0;
    DWORD dwThreadId = 0;

    const HANDLE threadID = OpenThread(dwDesiredAccess, bInheritHandle, dwThreadId); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxActionResult = PXErrorCurrent(PXNull != threadID);

    if(PXResultOK != pxActionResult)
    {
        return pxActionResult;
    }

    return PXResultOK;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
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
    PXThread dummyThread;

    if(!pxThread)
    {
        PXThreadCurrent(&dummyThread);
        pxThread = &dummyThread;
    }

    const int threadPriority = PXThreadPriorityToID(pxThreadPriorityMode);

#if OSUnix
    const int newPriority = setpriority(0, pxThread->ThreadHandle, threadPriority);
    const PXResult pxActionResult = PXErrorCurrent(-1 != newPriority);

    if(PXResultOK != pxActionResult)
    {
        return pxActionResult;
    }

    return PXResultOK;

#elif OSWindows
    // SetPriorityClass() also exists, but is it needed?
    const BOOL success = SetThreadPriority(pxThread->ThreadHandle, threadPriority);
    const PXResult pxActionResult = PXErrorCurrent(FALSE != success);

    if(PXResultOK != pxActionResult)
    {
        return pxActionResult;
    }

    return PXResultOK;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
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
    priorityID = getpriority(0, pxThread->ThreadHandle); // sys/resource.h
#elif OSWindows
    priorityID = GetThreadPriority(pxThread->ThreadHandle);
#endif

    return PXResultOK;
}

PXResult PXAPI PXThreadStateChange(PXThread PXREF pxThread, const PXI32U pxThreadState)
{
    if(!pxThread)
    {
        return PXResultRefusedParameterNull;
    }

    PXProcessThreadHandle threadHandle = pxThread->ThreadHandle;

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
            return PXResultRefusedParameterInvalid;
    }

    return PXResultOK;
}

PXResult PXAPI PXThreadSleep(PXThread PXREF pxThread, const PXSize sleepTime)
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

        return PXResultOK;
#else
        return PXActionRefusedNotImplemented;
#endif
    }
}

PXResult PXAPI PXThreadCurrentProcessorID(PXI32U PXREF processorID)
{
    *processorID = 0xFFFFFFFF;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

#if WindowsAtleastVista
    *processorID = GetCurrentProcessorNumber(); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h

    return PXResultOK;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

#else
    return PXActionRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText PXREF threadName)
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

    const HRESULT resultID = SetThreadDescription(pxThread->ThreadHandle, threadNameW); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
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
    return PXActionRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadNameGet(PXDebug PXREF pxDebug,PXThread PXREF pxThread, PXText PXREF threadName)
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

    return PXResultOK;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXResult PXAPI PXThreadContextGet(PXThreadContext PXREF pxThreadContext, const PXProcessThreadHandle pxThreadHandle)
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
    const PXResult getResult = PXErrorCurrent(getResultID);

    if(PXResultOK != getResult)
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



PXResult PXAPI PXThreadCurrent(PXThread PXREF pxThread)
{
    PXClear(PXThread, pxThread);

    pxThread->ThreadID = PXThreadCurrentID();

#if OSUnix

    pxThread->ThreadHandle = pthread_self();

#elif OSWindows

    // Getting the thread handle yields a pseudo handle that is not useful
    pxThread->ThreadHandle = GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

    return PXResultOK;
}

PXI32U PXAPI PXThreadCurrentID()
{
#if OSUnix
    const int threadID = getpid();
#elif OSWindows
    const DWORD threadID = GetCurrentThreadId(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

    return threadID;
}

PXResult PXAPI PXThreadResume(PXThread PXREF pxThread)
{
    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= PXExecuteStateRunning;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    const PXProcessThreadHandle threadHandle = pxThread->ThreadHandle;
    const DWORD suspendCount = ResumeThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxActionResult = PXErrorCurrent(-1 != suspendCount);

    if(PXResultOK != pxActionResult)
    {
        return pxActionResult;
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXThreadName,
        "Resume",
        "Handle:<%p>, ID:<%i>",
        threadHandle,
        pxThread->ThreadID
    );
#endif
}

PXResult PXAPI PXThreadSuspend(PXThread PXREF pxThread)
{
    const PXProcessThreadHandle threadHandle = pxThread->ThreadHandle;

    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= PXExecuteStateSuspended;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    const DWORD result = SuspendThread(threadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXResult pxActionResult = PXErrorCurrent(-1 != result);

    if(PXResultOK != pxActionResult)
    {
        return pxActionResult;
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXThreadName,
        "Suspend",
        "Handle:<%p>, ID:<%i>",
        threadHandle,
        pxThread->ThreadID
    );
#endif
}

PXResult PXAPI PXThreadWait(PXThread PXREF pxThread)
{
    pxThread->Info.Behaviour &= ~PXExecuteStateMask;
    pxThread->Info.Behaviour |= PXExecuteStateWaiting;

    pxThread->ReturnResultCode = 0; // Reset Code

#if OSUnix
    const int resultID = pthread_join(pxThread->ThreadHandle, &pxThread->ReturnResult); // pthread.h
    const PXBool success = 0 == resultID;

    if(!success)
    {
        return PXErrorCodeFromID(resultID);
    }

    return PXResultOK;

#elif OSWindows

    const PXProcessThreadHandle threadHandle = pxThread->ThreadHandle;
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
}

PXResult PXAPI PXThreadCPUCoreAffinitySet(PXThread PXREF pxThread, const PXI16U coreIndex)
{
    //MAXIMUM_PROCESSORS;

    const DWORD resultID = SetThreadIdealProcessor
    (
        pxThread->ThreadHandle,
        coreIndex
    ); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXBool successful = (DWORD)-1 != resultID;
    PXResult result = PXErrorCurrent(successful);


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
    PXActionResult pxActionResult;

#if OSUnix
    const int yieldResultID =
#if 0
        pthread_yield(); // [deprecated] pthread.h
#else
        sched_yield(); // sched.h, libc, sets errno but on linux never fails
#endif
    pxActionResult = PXErrorCurrent(0 != yieldResultID);

#elif OSWindows
    // UmsThreadYield() // Windows 7 (64-Bit only), Kernel32.dll, winbase.h [Debcricated in Windows 11]

#if 0
    Sleep(0);
    pxActionResult = PXResultOK;
#else
    const BOOL switchSuccessful = SwitchToThread(); // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h
    pxActionResult = PXResultOK; // cant fail
#endif

#else
    pxActionResult = PXActionNotSupportedByOperatingSystem;
#endif

    return pxActionResult;
}
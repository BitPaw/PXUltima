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

void PXAPI PXThreadConstruct(PXThread* const pxThread)
{
    PXClear(PXThread, pxThread);
    pxThread->ThreadHandle = PXHandleNotSet;
    pxThread->Mode = PXThreadModeInvalid;
}

void PXAPI PXThreadDestruct(PXThread* const pxThread)
{
    if (!pxThread)
    {
        return;
    }

    if (pxThread->ThreadHandle == PXHandleNotSet)
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

    PXThreadConstruct(pxThread);
}


#if OSWindows
void PXAPI PXThreadConstructFromHandle(PXThread* const pxThread, HANDLE threadHandle)
{
    PXThreadConstruct(pxThread);

    pxThread->ThreadHandle = threadHandle;

#if WindowsAtleastVista
    pxThread->ThreadID = GetThreadId(threadHandle); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h
#else
    pxThread->ThreadID = 0xFFFFFFFF;
#endif
}
#endif

PXActionResult PXAPI PXThreadRun(PXThread* const pxThread, const char* const threadName, const ThreadFunction threadFunction, const void* parameter)
{
    if (!pxThread || !threadFunction)
    {
        return PXActionRefusedArgumentInvalid;
    }

#if OSUnix
    const int result = pthread_create(&pxThread->ThreadHandle, 0, threadFunction, (void*)parameter);
    const PXBool successful = 0 == result;

    PXActionOnErrorFetchAndReturn(!successful);

#elif OSWindows
    const HANDLE threadID = CreateThread // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    (
        NULL, // lpThreadAttributes
        0, // dwStackSize
        (LPTHREAD_START_ROUTINE)threadFunction,
        (LPVOID)parameter,
        0, // dwCreationFlags
        NULL // lpThreadId
    );
    const PXBool wasSucessful = threadID != 0;

    if(!wasSucessful)
    {
        const PXActionResult threadResult = PXErrorCurrent();

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

        return threadResult;
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Thread",
        "Create",
        "Success. ID:%i Name:%s",
        (int)threadID,
        threadName
    );
#endif


    if (pxThread)
    {
        pxThread->ThreadHandle = threadID;
        pxThread->Mode = PXThreadModeRunning;
    }

    // Name thread if possible
    {
        PXText pxText;
        PXTextConstructFromAdressA(&pxText, threadName, 0, PXTextLengthUnkown);

        PXThreadNameSet(pxThread, &pxText);
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAPI PXThreadRunInOtherProcess(PXThread* const pxThread, const void* processHandle, const ThreadFunction threadFunction, const void* parameter)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop

    LPSECURITY_ATTRIBUTES  lpThreadAttributes = 0;
    SIZE_T                 dwStackSize = 0;
    LPTHREAD_START_ROUTINE lpStartAddress = threadFunction;
    LPVOID                 lpParameter = parameter;
    DWORD                  dwCreationFlags = 0;
    LPDWORD                lpThreadId = 0;
    const HANDLE threadID = CreateRemoteThread // Windows XP, Kernel32.dll, processthreadsapi.h
    (
        processHandle,
        lpThreadAttributes,
        dwStackSize,
        lpStartAddress,
        lpParameter,
        dwCreationFlags,
        lpThreadId
    );
    const PXBool successful = PXNull != threadID;

    PXActionOnErrorFetchAndReturn(!successful);

    pxThread->ThreadHandle = threadID;

    return PXActionSuccessful;
#else 
    return PXActionRefusedNotSupported;
#endif
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

#if 0 // deprecated
    const int resultID = pthread_yield(); // [deprecated] pthread.h
    const PXActionResult yieldResult = PXErrorCodeFromID();

    return yieldResult;
#else
    const int resultID = sched_yield(); // sched.h
    const PXBool successful = -1 == resultID;

    PXActionOnErrorFetchAndReturn(!successful);

    return PXActionSuccessful;

#endif

#elif OSWindows
    // UmsThreadYield() // Windows 7 (64-Bit only), Kernel32.dll, winbase.h [Debcricated in Windows 11]

    const PXBool successful = SwitchToThread(); // Windows 2000 SP4 (+UWP), Kernel32.dll, processthreadsapi.h

    if(!successful)
    {
        return PXActionDidNothing;
    }

    return PXActionSuccessful;
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
    const PXBool success = PXNull != threadID;

    PXActionOnErrorFetchAndReturn(!success);

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
        case PXThreadPriorityModeLow16: {
            return 
#if OSUnix
                16;
#elif OSWindows
                IDLE_PRIORITY_CLASS | THREAD_PRIORITY_ABOVE_NORMAL
#endif
                ;
         
        }
        case PXThreadPriorityModeLow15: {
            return 
#if OSUnix
                15;
#elif OSWindows
                IDLE_PRIORITY_CLASS | THREAD_PRIORITY_HIGHEST
#endif
                ;
         
        }
        case PXThreadPriorityModeLow14: {
            return 
#if OSUnix
                14;
#elif OSWindows
                IDLE_PRIORITY_CLASS | THREAD_PRIORITY_TIME_CRITICAL
#endif
                ;
         
        }
        case PXThreadPriorityModeLow13: {
            return 
#if OSUnix
                13;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow12: {
            return 
#if OSUnix
                12;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow11: {
            return 
#if OSUnix
                11;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow10: {
            return 
#if OSUnix
                10;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow09: {
            return 
#if OSUnix
                9;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow08: {
            return 
#if OSUnix
                8;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow07: {
            return 
#if OSUnix
                7;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow06: {
            return 
#if OSUnix
                6;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow05: {
            return 
#if OSUnix
                5;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow04: {
            return 
#if OSUnix
                4;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeLow03: {
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
        case PXThreadPriorityModeHigh03: {
            return 
#if OSUnix
                - 3;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh04: {
            return 
#if OSUnix
                - 4;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh05: {
            return 
#if OSUnix
                - 5;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh06: {
            return 
#if OSUnix
                - 6;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh07: {
            return 
#if OSUnix
                - 7;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh08: {
            return 
#if OSUnix
                - 8;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh09: {
            return 
#if OSUnix
                - 9;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh10: {
            return 
#if OSUnix
                - 10;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh11: {
            return 
#if OSUnix
                - 11;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh12: {
            return 
#if OSUnix
                - 12;
#elif OSWindows
                THREAD_PRIORITY_IDLE
#endif
                ;
         
        }
        case PXThreadPriorityModeHigh13: {
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
    const PXBool success = -1 != newPriority;

    if(!success)
    {
        const PXActionResult setResult = PXErrorCurrent();

        return setResult;
    }

    return PXActionSuccessful;
#elif OSWindows
    // SetPriorityClass() also exists, but is it needed?
    const PXBool result = SetThreadPriority(pxThread->ThreadHandle, threadPriority); 

    if(!result)
    {
        const PXActionResult setResult = PXErrorCurrent();

        return setResult;
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
     priorityID = GetThreadPriority(pxThread->ThreadHandle);
#endif

     return PXActionSuccessful;
}

PXActionResult PXAPI PXThreadSuspend(PXThread* const pxThread)
{
    if (pxThread->ThreadHandle == PXHandleNotSet) return PXActionRefusedNotFound;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    const DWORD result = SuspendThread(pxThread->ThreadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXBool successful = result != -1;

    PXActionOnErrorFetchAndReturn(!successful);

    pxThread->Mode = PXThreadModeSuspended;

    return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXThreadResume(PXThread* const pxThread)
{
    if (pxThread->ThreadHandle == PXHandleNotSet) return PXActionRefusedNotFound;

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
    const DWORD suspendCount = ResumeThread(pxThread->ThreadHandle); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    const PXBool successful = suspendCount != -1;

    PXActionOnErrorFetchAndReturn(!successful);

    pxThread->Mode = PXThreadModeRunning;

    return PXActionSuccessful;
#endif    
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

    const HRESULT resultID = SetThreadDescription(pxThread->ThreadHandle, threadNameW); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
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




HMODULE GetModuleNameFromAddress(void* functionAddress) 
{
    HMODULE hModule = NULL;

    const PXBool success = GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)functionAddress, &hModule);

    if(!success)
    {
        return PXNull;
    }

    return hModule;

    /*

    if() {
        std::cerr << "Failed to get module handle!" << std::endl;
        return "";
    }
    char moduleName[MAX_PATH];
    if(GetModuleFileName(hModule, moduleName, sizeof(moduleName)) == 0) {
        std::cerr << "Failed to get module name!" << std::endl;
        return "";
    }
    return std::string(moduleName);
    */
}



PXActionResult PXAPI PXThreadNameGet(PXDebug* const pxDebug ,PXThread* const pxThread, PXText* const threadName)
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

PXSize PXAPI PXThreadCurrentID()
{
#if OSUnix
    const __pid_t threadID = getpid();
#elif OSWindows
    const DWORD threadID = GetCurrentThreadId(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

    return threadID;
}

void PXAPI PXThreadCurrentGet(PXThread* const pxThread)
{
    PXClear(PXThread, pxThread);

#if OSUnix
    pxThread->ThreadHandle = pthread_self();
#elif OSWindows
    pxThread->ThreadHandle = GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
    pxThread->ThreadID = GetCurrentThreadId();
#endif
}

PXActionResult PXAPI PXThreadWaitForFinish(PXThread* const pxThread)
{
    pxThread->ReturnResult = 0;

#if OSUnix
    const int resultID = pthread_join(pxThread->ThreadHandle, &pxThread->ReturnResult); // pthread.h
    const PXBool success = 0 == resultID;

    if (!success)
    {
        return PXErrorCodeFromID(resultID);
    }

    return PXActionSuccessful;

#elif OSWindows

    const DWORD resultID = WaitForSingleObject(pxThread->ThreadHandle, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

    for (;;)
    {
        DWORD returnCode = 0;

        const BOOL result = GetExitCodeThread(pxThread->ThreadHandle, &returnCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
        const PXBool isDone = STILL_ACTIVE != pxThread->ReturnResult;

        pxThread->ReturnResult = returnCode;

        if (isDone)
        {
            break;
        }

        PXThreadYieldToOtherThreads();
    }

    pxThread->Mode = PXThreadModeFinished;

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}
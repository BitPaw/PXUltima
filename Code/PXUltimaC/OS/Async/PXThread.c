#include "PXThread.h"

#include <OS/Console/PXConsole.h>
#include <OS/File/PXFile.h>

// Note, TerminateThread() should not be used, it can lead to a memory leak in XP

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
		const PXActionResult xx = PXErrorCurrent();

#if PXLogEnable && 0
		PXLogPrint
		(
			PXLoggingError,
			"Thread",
			"Yield",
			"Failed action."
		);
#endif

		return xx;
	}

#if PXLogEnable && 0
	PXLogPrint
	(
		PXLoggingInfo,
		"Thread",
		"Yield",
		"Done with work, swapping to next thread"
	);
#endif

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
	return PXActionRefusedNotSupported;
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

PXActionResult PXAPI PXThreadNameGet(PXThread* const pxThread, PXText* const threadName)
{
#if OSUnix
	return PXActionRefusedNotImplemented;
#elif OSWindows

#if WindowsAtleast10

	wchar_t* tempThreadDescription = 0;
	const HRESULT resultID = GetThreadDescription(pxThread->ThreadHandle, &tempThreadDescription); // Windows 10 - 1607 (+UWP), Kernel32.dll, processthreadsapi.h
	//const PXActionResult result = PXWindowsHandleErrorFromID(resultID);

	//PXActionReturnOnError(result);

	if (tempThreadDescription)
	{
		threadName->SizeUsed = PXTextCopyWA(tempThreadDescription, 256, threadName->TextA, threadName->SizeAllocated);
	}
	else
	{
		threadName->SizeUsed = PXTextCopyA("[N/A]", 5, threadName->TextA, threadName->SizeAllocated);
	}
	
	LocalFree(tempThreadDescription);

#else

#endif

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
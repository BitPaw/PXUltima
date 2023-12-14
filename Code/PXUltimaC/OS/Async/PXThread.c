#include "PXThread.h"

// Note, TerminateThread() should not be used, it can lead to a memory leak in XP

void PXAPI PXThreadConstruct(PXThread* const pxThread)
{
	pxThread->ThreadID = PXHandleNotSet;
	pxThread->Mode = PXThreadModeInvalid;
}

void PXAPI PXThreadDestruct(PXThread* const pxThread)
{
	if (!pxThread)
	{
		return;
	}

	if (pxThread->ThreadID == PXHandleNotSet)
	{
		return;
	}

#if OSUnix

#elif OSWindows
	const HANDLE currentThread = GetCurrentThread();
	const PXBool isTargetCurrentThread = currentThread == pxThread->ThreadID;
	const PXBool success = CloseHandle(pxThread->ThreadID); // Windows 2000 Professional (+UWP), Kernel32.dll, handleapi.h
	
	if (!success)
	{
		return;
	}

#endif

	PXThreadConstruct(pxThread);
}

PXActionResult PXAPI PXThreadRun(PXThread* const pxThread, const ThreadFunction threadFunction, const void* parameter)
{
#if OSUnix
	const int result = pthread_create(&pxThread->ThreadID, 0, threadFunction, (void*)parameter);

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

	PXActionOnErrorFetchAndReturn(!wasSucessful);

	if (pxThread)
	{
		pxThread->ThreadID = threadID;
		pxThread->Mode = PXThreadModeRunning;
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

	pxThread->ThreadID = threadID;

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

	const PXBool successful = SwitchToThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

	PXActionOnErrorFetchAndReturn(!successful);

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
	if (pxThread->ThreadID == PXHandleNotSet) return PXActionRefusedObjectNotFound;

#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows
	const DWORD result = SuspendThread(pxThread->ThreadID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = result != -1;

	PXActionOnErrorFetchAndReturn(!successful);

	pxThread->Mode = PXThreadModeSuspended;

	return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXThreadResume(PXThread* const pxThread)
{
	if (pxThread->ThreadID == PXHandleNotSet) return PXActionRefusedObjectNotFound;

#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows
	const DWORD suspendCount = ResumeThread(pxThread->ThreadID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = suspendCount != -1;

	PXActionOnErrorFetchAndReturn(!successful);

	pxThread->Mode = PXThreadModeRunning;

	return PXActionSuccessful;
#endif	
}

PXActionResult PXAPI PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime)
{
	//if (pxThread->ThreadID == PXHandleNotSet) return PXActionRefusedObjectNotFound;

#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows
	Sleep(sleepTime); // Windows XP (+UWP), Kernel32.dll, synchapi.h
#endif

	return PXActionSuccessful;
}

PXActionResult PXAPI PXThreadCurrentProcessorID(PXInt32U* const processorID)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows
	*processorID = GetCurrentProcessorNumber(); // Windows Vista (+UWP), Kernel32.dll, processthreadsapi.h
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
#if OSUnix
	pxThread->ThreadID = pthread_self();
#elif OSWindows
	pxThread->ThreadID = GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif
}

PXActionResult PXAPI PXThreadWaitForFinish(PXThread* const pxThread)
{
	pxThread->ReturnResult = 0;

#if OSUnix
	const int resultID = pthread_join(thread->ID, &pxThread->ReturnResult);
	const PXBool success = 0 == resultID;

	if (!success)
	{
		return PXErrorCodeFromID(resultID);
	}

	return PXActionSuccessful;

#elif OSWindows

	const DWORD resultID = WaitForSingleObject(pxThread->ThreadID, INFINITE); // Windows XP (+UWP), Kernel32.dll, synchapi.h

	for (;;)
	{
		const BOOL result = GetExitCodeThread(pxThread->ThreadID, &pxThread->ReturnResult); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
		const PXBool isDone = STILL_ACTIVE != pxThread->ReturnResult;

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
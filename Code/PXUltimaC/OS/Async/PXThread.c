#include "PXThread.h"

// Note, TerminateThread() should not be used, it can lead to a memory leak in XP

#include <stdio.h>

void PXThreadConstruct(PXThread* const pxThread)
{
	pxThread->ThreadID = PXHandleNotSet;
	pxThread->Mode = PXThreadModeInvalid;
}

void PXThreadDestruct(PXThread* const pxThread)
{
	if (!pxThread)
	{
		return;
	}

	printf("[C] Thread handle release %p : ", pxThread->ThreadID);

	if (pxThread->ThreadID == PXHandleNotSet)
	{
	
		printf("CANCLED\n");

		return;
	}

#if OSUnix

#elif OSWindows
	const HANDLE currentThread = GetCurrentThread();
	const PXBool isTargetCurrentThread = currentThread == pxThread->ThreadID;
	const PXBool success = CloseHandle(pxThread->ThreadID); // Windows 2000 Professional (+UWP), Kernel32.dll, handleapi.h
	
	if (!success)
	{
		printf("FAILED\n");

		return;
	}

	printf("OK\n");

#endif

	PXThreadConstruct(pxThread);
}

PXActionResult PXThreadRun(PXThread* const pxThread, const ThreadFunction threadFunction, const void* parameter)
{
#if OSUnix
	const int result = pthread_create(&pxThread->ThreadID, 0, threadFunction, (void*)parameter);

#elif OSWindows
	const LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
	const SIZE_T dwStackSize = 0;
	const LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)threadFunction;
	const LPVOID lpParameter = (const LPVOID)parameter;
	const DWORD dwCreationFlags = 0;
	const LPDWORD lpThreadId = NULL;
	const HANDLE threadID = CreateThread // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	(
		lpThreadAttributes,
		dwStackSize,
		lpStartAddress,
		lpParameter,
		dwCreationFlags,
		lpThreadId
	);
	const PXBool wasSucessful = threadID != 0;

	PXActionOnErrorFetchAndReturn(!wasSucessful);

	if (pxThread)
	{
		pxThread->ThreadID = threadID;
		pxThread->Mode = PXThreadModeRunning;
	}

#endif

	return PXActionSuccessful;
}

PXActionResult PXThreadRunInOtherProcess(PXThread* const pxThread, const void* processHandle, const ThreadFunction threadFunction, const void* parameter)
{
#if OSUnix
	return PXActionNotImplemented;

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
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXThreadExitCurrent(const PXInt32U exitCode)
{
#if OSUnix
	return PXActionNotImplemented;

#elif OSWindows
	ExitThread(exitCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

	return PXActionSuccessful;
#else 
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXThreadYieldToOtherThreads()
{
#if OSUnix
	return PXActionNotImplemented;

#elif OSWindows
	const PXBool successful = SwitchToThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h

	PXActionOnErrorFetchAndReturn(!successful);

	return PXActionSuccessful;
#else 
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXThreadOpen(PXThread* const pxThread)
{
#if OSUnix
	return PXActionNotImplemented;

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

PXActionResult PXThreadSuspend(PXThread* const pxThread)
{
	if (pxThread->ThreadID == PXHandleNotSet) return PXActionRefuedObjectIDInvalid;

#if OSUnix
	return PXActionNotImplemented;

#elif OSWindows
	const DWORD result = SuspendThread(pxThread->ThreadID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = result != -1;

	PXActionOnErrorFetchAndReturn(!successful);

	pxThread->Mode = PXThreadModeSuspended;

	return PXActionSuccessful;
#endif
}

PXActionResult PXThreadResume(PXThread* const pxThread)
{
	if (pxThread->ThreadID == PXHandleNotSet) return PXActionRefuedObjectIDInvalid;

#if OSUnix
	return PXActionNotImplemented;

#elif OSWindows
	const DWORD suspendCount = ResumeThread(pxThread->ThreadID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = suspendCount != -1;

	PXActionOnErrorFetchAndReturn(!successful);

	pxThread->Mode = PXThreadModeRunning;

	return PXActionSuccessful;
#endif	
}

PXActionResult PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime)
{
	//if (pxThread->ThreadID == PXHandleNotSet) return PXActionRefuedObjectIDInvalid;

#if OSUnix
	return PXActionNotImplemented;

#elif OSWindows
	Sleep(sleepTime); // Windows XP (+UWP), Kernel32.dll, synchapi.h
#endif

	return PXActionSuccessful;
}

PXSize PXThreadCurrentID()
{
#if OSUnix
	const __pid_t threadID = getpid();
#elif OSWindows
	const DWORD threadID = GetCurrentThreadId(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif

	return threadID;
}

void PXThreadCurrentGet(PXThread* const pxThread)
{
#if OSUnix
	pxThread->ThreadID = pthread_self();
#elif OSWindows
	pxThread->ThreadID = GetCurrentThread(); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
#endif
}

void PXThreadWaitForFinish(PXThread* const pxThread)
{
#if OSUnix
	//pthread_join(thread->ID, NULL);
#elif OSWindows
	DWORD exitCode = 0;

	do
	{
		GetExitCodeThread(pxThread->ThreadID, &exitCode); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	}
	while(exitCode == STILL_ACTIVE);

	pxThread->Mode = PXThreadModeFinished;

#endif
}
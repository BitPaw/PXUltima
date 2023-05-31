#include "PXThread.h"

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
	}

#endif

	return PXActionSuccessful;
}

void PXThreadSuspend(PXThread* const pxThread)
{
#if OSUnix
	
#elif OSWindows
	const DWORD result = SuspendThread(pxThread->ThreadID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = result != -1;
#endif
}

void PXThreadResume(PXThread* const pxThread)
{
#if OSUnix

#elif OSWindows
	const DWORD suspendCount = ResumeThread(pxThread->ThreadID); // Windows XP (+UWP), Kernel32.dll, processthreadsapi.h
	const PXBool successful = suspendCount != -1;

#endif
}

void PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime)
{
#if OSUnix

#elif OSWindows
	Sleep(sleepTime); // Windows XP (+UWP), Kernel32.dll, synchapi.h
#endif
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
#endif
}
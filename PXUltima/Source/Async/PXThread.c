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
	const HANDLE threadID = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
	const PXBool wasSucessful = threadID != 0;

	if (!wasSucessful)
	{
		const PXActionResult actionResult = GetCurrentError();

		return actionResult;
	}

	if (pxThread)
	{
		pxThread->ThreadID = threadID;
	}

#endif

	return PXActionSuccessful;
}

PXSize PXThreadCurrentID()
{
#if OSUnix
	const __pid_t threadID = getpid();
#elif OSWindows
	const DWORD threadID = GetCurrentThreadId();
#endif

	return threadID;
}

void PXThreadCurrentGet(PXThread* const pxThread)
{
#if OSUnix
	pxThread->ThreadID = pthread_self();
#elif OSWindows
	pxThread->ThreadID = GetCurrentThread(); // Minimum supported : Windows XP
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
		GetExitCodeThread(pxThread->ThreadID, &exitCode);
	}
	while(exitCode == STILL_ACTIVE);
#endif
}
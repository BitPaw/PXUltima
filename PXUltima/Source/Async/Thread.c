#include "Thread.h"

ThreadID ThreadRun(const ThreadFunction threadFunction, const void* parameter)
{
#if OSUnix
	ThreadID threadID = 0;
	const int result = pthread_create(&threadID, 0, threadFunction, (void*)parameter);
#elif OSWindows
	const LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
	const SIZE_T dwStackSize = 0;
	const LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)threadFunction;
	const LPVOID lpParameter = (const LPVOID)parameter;
	const DWORD dwCreationFlags = 0;
	const LPDWORD lpThreadId = NULL;
	const ThreadID threadID = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
#endif

	return threadID;
}

size_t ThreadCurrentID()
{
#if OSUnix
	const __pid_t threadID = getpid();
#elif OSWindows
	const DWORD threadID = GetCurrentThreadId();
#endif

	return threadID;
}

ThreadID ThreadCurrentGet()
{
#if OSUnix
	return 0;
#elif OSWindows
	return GetCurrentThread();
#endif
}

void ThreadWaitForFinish(const ThreadID threadID)
{
#if OSUnix
	//pthread_join(thread->ID, NULL);
#elif OSWindows
	DWORD exitCode = 0;

	do
	{
		GetExitCodeThread(threadID, &exitCode);
	}
	while(exitCode == STILL_ACTIVE);
#endif
}
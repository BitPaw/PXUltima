#ifndef PXThreadInclude
#define PXThreadInclude

#include <Media/PXResource.h>

// Return IDs

#if OSUnix
#include <pthread.h>
#include <unistd.h>
typedef void* PXThreadResult;
//typedef pthread_t PXThreadIDType;
#define PXThreadIDUnused 0  // Adress
#elif OSWindows
#include <windows.h>
typedef unsigned long PXThreadResult;
//typedef HANDLE PXThreadIDType;
#define PXThreadIDUnused nullptr
#if OSWindowsXP
typedef struct IUnknown IUnknown;
#endif
#endif

//#define PXThreadSucessful (PXThreadResult)0
//#define PXThreadActionFailed (PXThreadResult)1

typedef PXThreadResult(PXOSAPI* ThreadFunction)(void* const data);

typedef enum PXThreadMode_
{
	PXThreadModeInvalid,
	PXThreadModeInitializing,
	PXThreadModeRunning,
	PXThreadModeStopping,
	PXThreadModeSuspended,
	PXThreadModeFinished,
}
PXThreadMode;

typedef struct PXThread_
{
#if OSUnix
	void* ReturnResult;

	pthread_t ThreadHandle;
#elif OSWindows		
	HANDLE ThreadHandle;
	PXInt32U ThreadID;
	PXInt32U ReturnResult;
#endif

	PXThreadMode Mode;
}
PXThread;

PXPublic void PXAPI PXThreadConstruct(PXThread* const pxThread);
PXPublic void PXAPI PXThreadDestruct(PXThread* const pxThread);

#if OSWindows
PXPublic void PXAPI PXThreadConstructFromHandle(PXThread* const pxThread, HANDLE threadHandle);
#endif // OSWindows


// This function create a handle for the thread.
// The thread will clean itself up, yet you need to release the handle as a final step.
// "threadName" can be NULL
PXPublic PXActionResult PXAPI PXThreadRun(PXThread* const pxThread, const char* const threadName, const ThreadFunction threadFunction, const void* parameter);

PXPublic PXActionResult PXAPI PXThreadRunInOtherProcess(PXThread* const pxThread, const void* processHandle, const ThreadFunction threadFunction, const void* parameter);

// Dont call this function if you dont have to.
// In C++ this can cause memory leaks as destructors might not be called.
PXPublic PXActionResult PXAPI PXThreadExitCurrent(const PXInt32U exitCode);

// Causes the calling thread to yield execution to another
// thread that is ready to run on the current processor.
// The operating system selects the next thread to be executed.
PXPublic PXActionResult PXAPI PXThreadYieldToOtherThreads();

PXPublic PXActionResult PXAPI PXThreadOpen(PXThread* const pxThread);

PXPublic PXActionResult PXAPI PXThreadSuspend(PXThread* const pxThread);
PXPublic PXActionResult PXAPI PXThreadResume(PXThread* const pxThread);
PXPublic PXActionResult PXAPI PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime);

PXPublic PXActionResult PXAPI PXThreadCurrentProcessorID(PXInt32U* const processorID);

PXPublic PXActionResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText* const threadName);
PXPublic PXActionResult PXAPI PXThreadNameGet(PXThread* const pxThread, PXText* const threadName);

PXPublic PXSize PXAPI PXThreadCurrentID();
PXPublic void PXAPI PXThreadCurrentGet(PXThread* const pxThread);

PXPublic PXActionResult PXAPI PXThreadWaitForFinish(PXThread* const pxThread);

#endif
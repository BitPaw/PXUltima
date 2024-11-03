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
#endif


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
// The function returns true if a yield was caused, otherwise the
// current thread proceeds execution and false is returned.
PXPublic PXActionResult PXAPI PXThreadYieldToOtherThreads();

PXPublic PXActionResult PXAPI PXThreadOpen(PXThread* const pxThread);


// Windows : priority
// Linux : nice value
// A value indicating how important a thread is relavtive to
// others running. The schedulter uses this hint to give important
// processes a faster response time to act and slow down threads
// that are not as important to respond fast.
typedef enum PXThreadPriorityMode_
{
    PXThreadPriorityModeInvalid,

    PXThreadPriorityModeLowest, // Lowest possible priority
    PXThreadPriorityModeLow19,
    PXThreadPriorityModeLow18,
    PXThreadPriorityModeLow17,
    PXThreadPriorityModeLow16,
    PXThreadPriorityModeLow15,
    PXThreadPriorityModeLow14,
    PXThreadPriorityModeLow13,
    PXThreadPriorityModeLow12,
    PXThreadPriorityModeLow11,
    PXThreadPriorityModeLow10,
    PXThreadPriorityModeLow09,
    PXThreadPriorityModeLow08,
    PXThreadPriorityModeLow07,
    PXThreadPriorityModeLow06,
    PXThreadPriorityModeLow05,
    PXThreadPriorityModeLow04,
    PXThreadPriorityModeLow03,
    PXThreadPriorityModeLow02,
    PXThreadPriorityModeLow01,
    PXThreadPriorityModeLower,
    PXThreadPriorityModeNormal,
    PXThreadPriorityModeHigher,
    PXThreadPriorityModeHigh01,
    PXThreadPriorityModeHigh02,
    PXThreadPriorityModeHigh03,
    PXThreadPriorityModeHigh04,
    PXThreadPriorityModeHigh05,
    PXThreadPriorityModeHigh06,
    PXThreadPriorityModeHigh07,
    PXThreadPriorityModeHigh08,
    PXThreadPriorityModeHigh09,
    PXThreadPriorityModeHigh10,
    PXThreadPriorityModeHigh11,
    PXThreadPriorityModeHigh12,
    PXThreadPriorityModeHigh13,
    PXThreadPriorityModeHigh14,
    PXThreadPriorityModeHigh15,
    PXThreadPriorityModeHigh16,
    PXThreadPriorityModeHigh17,
    PXThreadPriorityModeHigh18,
    PXThreadPriorityModeHigh19,
    PXThreadPriorityModeHighest
}
PXThreadPriorityMode;



PXPublic PXActionResult PXAPI PXThreadPrioritySet(PXThread* pxThread, const PXThreadPriorityMode pxThreadPriorityMode);
PXPublic PXActionResult PXAPI PXThreadPriorityGet(PXThread* pxThread, PXThreadPriorityMode* const pxThreadPriorityMode);


PXPublic PXActionResult PXAPI PXThreadSuspend(PXThread* const pxThread);
PXPublic PXActionResult PXAPI PXThreadResume(PXThread* const pxThread);
PXPublic PXActionResult PXAPI PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime);

PXPublic PXActionResult PXAPI PXThreadCurrentProcessorID(PXInt32U* const processorID);

PXPublic PXActionResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText* const threadName);
PXPublic PXActionResult PXAPI PXThreadNameGet(struct PXDebug_* const pxDebug, PXThread* const pxThread, PXText* const threadName);

PXPublic PXSize PXAPI PXThreadCurrentID();
PXPublic void PXAPI PXThreadCurrentGet(PXThread* const pxThread);

PXPublic PXActionResult PXAPI PXThreadWaitForFinish(PXThread* const pxThread);

#endif

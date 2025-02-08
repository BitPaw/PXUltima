#ifndef PXThreadInclude
#define PXThreadInclude

#include <Media/PXResource.h>
#include "PXProcess.h"

// Return IDs

#if OSUnix
#include <pthread.h>
#include <unistd.h>
typedef void* PXThreadResult;
//typedef pthread_t PXThreadIDType;
#define PXThreadIDUnused 0  // Adress
#elif OSWindows
#include <windows.h>
typedef DWORD PXThreadResult;
//typedef HANDLE PXThreadIDType;
#define PXThreadIDUnused nullptr
#if OSWindowsXP
typedef struct IUnknown IUnknown;
#endif
#endif

//#define PXThreadSucessful (PXThreadResult)0
//#define PXThreadActionFailed (PXThreadResult)1

typedef PXThreadResult(PXOSAPI* ThreadFunction)(void* const data);



#define PXExecuteStateMask (0b00001111)
#define PXExecuteStateInvalid     0 // Resource does not exist. Not created or deleted
#define PXExecuteStateIDLE        1 // Resource is not doing anything, ready to be used
#define PXExecuteStateInit        2 // Resource is inizialized and ready to be executed
#define PXExecuteStateDibs        3 // Resource is taken by a handler and will be executed
#define PXExecuteStateRunning     4 // Resource is currently running
#define PXExecuteStateWaiting     5 // Resource waits for another resource
#define PXExecuteStateSuspended   6 // Resource 
#define PXExecuteStateFailed      7
#define PXExecuteStateFinished    8 // Resource is done
#define PXExecuteStateStale       9 // Resource is done

typedef enum PXThreadState_
{
    PXThreadStateInvalid   = PXExecuteStateInvalid,
    PXThreadStateSleeping  = PXExecuteStateIDLE,
    PXThreadStateInit      = PXExecuteStateInit,
    PXThreadStateRunning   = PXExecuteStateRunning,
    PXThreadStateWaiting   = PXExecuteStateWaiting,
    PXThreadStateSuspended = PXExecuteStateSuspended
}
PXThreadState;


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



#define PXTaskExecuteDirect    (1 <<  4) // Do not cache this task, execute it directly as is
#define PXTaskExecuteSYNC      (1 <<  5) // Cache task and execute it from the main thread 
#define PXTaskExecuteASYNC     (1 <<  6) // Cache task and execute it in another thread
#define PXTaskExecuteLoop      (1 <<  7) // Shall this task repeat itself after executing successful?
#define PXTaskParameterRelease (1 <<  8) // Delete the parameter after the task finished, to remove transphere objects
#define PXTaskDepended         (1 <<  9) // Task depends on other resources, wait for them to finish
#define PXTaskTimeDecay        (1 << 10) // Is a given Task can't be executed until a spesific time, mark it s failed and dont try again.

typedef PXActionResult (PXAPI* PXThreadX1CallFunction)(void* objectAdress);
typedef PXActionResult (PXAPI* PXThreadX2CallFunction)(void* objectAdressA, void* objectAdressB);

typedef struct PXTask_
{
    PXResourceInfo Info;

    union
    {
        // Task function that is to be executed
        PXThreadX1CallFunction FunctionX1Adress;
        PXThreadX2CallFunction FunctionX2Adress;
    };

    void* ArgumentObject1; // Parameter that is given into the function
    void* ArgumentObject2;

    int TimeStart; // Start time of the task. To know how much time has passed.
    int TimeDeadline;  // The targeted end time of the task. Can be a higher priority the less time is present

    PXActionResult FunctionReturnCode; // As this is a PX function call, we dont get a number back but this enum
}
PXTask;


void PXAPI PXTaskStateChange(PXTask* const pxTask, const PXInt32U newState);




#define PXThreadBehaviourDefault            0
#define PXThreadBehaviourCreateSuspended    (1<<8)
#define PXThreadStartOnAdd                  (1<<10)

#if OSUnix
typedef PXInt32U PXThreadHandleID;
#elif OSWindows
typedef DWORD PXThreadHandleID;
#endif 

typedef struct PXThread_
{
    PXResourceInfo Info;

    PXThreadResult ReturnResultCode;

    PXThreadHandleID HandleID;
}
PXThread;

PXPublic void PXAPI PXThreadDestruct(PXThread* const pxThread);

#if OSWindows
PXPublic void PXAPI PXThreadConstructFromHandle(PXThread* const pxThread, HANDLE threadHandle);
#endif


// Create thread and start it if defined.
// The thread will clean itself up, yet you need to release the handle as a final step.
// "targetProcessHandle" can be NULL, will target own process
// "threadName" can be NULL, it helps for debugging
PXPublic PXActionResult PXAPI PXThreadCreate
(
    PXThread* const pxThread,
    const char* const threadName,
    const PXProcessHandle targetProcessHandle,
    ThreadFunction threadFunction,
    void* parameter, 
    const PXInt32U behaviour
);

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






PXPublic PXActionResult PXAPI PXThreadPrioritySet(PXThread* pxThread, const PXThreadPriorityMode pxThreadPriorityMode);
PXPublic PXActionResult PXAPI PXThreadPriorityGet(PXThread* pxThread, PXThreadPriorityMode* const pxThreadPriorityMode);

// Change the current thread state to the wanted state if possible.
PXPublic PXActionResult PXAPI PXThreadStateChange(PXThread* const pxThread, const PXThreadState pxThreadState);

PXPublic PXActionResult PXAPI PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime);

PXPublic PXActionResult PXAPI PXThreadCurrentProcessorID(PXInt32U* const processorID);

PXPublic PXActionResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText* const threadName);
PXPublic PXActionResult PXAPI PXThreadNameGet(struct PXDebug_* const pxDebug, PXThread* const pxThread, PXText* const threadName);

PXPublic PXActionResult PXAPI PXThreadCurrent(PXThread* const pxThread);

#endif

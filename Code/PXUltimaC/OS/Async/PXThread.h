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



#define PXTaskExecuteSYNC      (1<<0)
#define PXTaskExecuteASYNC     (1<<1)
#define PXTaskExecuteLoop      (1<<2) // Shall this task repeat itself after executing successful?
#define PXTaskParameterRelease (1<<3)
#define PXTaskDepended         (1<<4)

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


PXPublic PXActionResult PXAPI PXThreadSuspend(PXThread* const pxThread);
PXPublic PXActionResult PXAPI PXThreadResume(PXThread* const pxThread);
PXPublic PXActionResult PXAPI PXThreadSleep(PXThread* const pxThread, const PXSize sleepTime);

PXPublic PXActionResult PXAPI PXThreadCurrentProcessorID(PXInt32U* const processorID);

PXPublic PXActionResult PXAPI PXThreadNameSet(PXThread* pxThread, PXText* const threadName);
PXPublic PXActionResult PXAPI PXThreadNameGet(struct PXDebug_* const pxDebug, PXThread* const pxThread, PXText* const threadName);

PXPublic PXThreadHandleID PXAPI PXThreadCurrentID();
PXPublic PXProcessThreadHandle PXAPI PXThreadCurrentGet();

PXPublic PXActionResult PXAPI PXThreadWaitForFinish(PXThread* const pxThread);

#endif
#ifndef PXThreadPoolInclude
#define PXThreadPoolInclude

#include <PX/Media/PXResource.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/Container/List/PXList.h>

#define PXThreadPoolEnableASYNC (1<<0)
#define PXThreadPoolCreated     (1<<1)

typedef struct PXThreadPool_
{
    PXSize ThreadListSize;
    PXThread* ThreadList;

    PXList TaskQueue;
    PXLock TaskLock;

    PXSize ThreadsAwake;

#if OSUnix
#elif OSWindows
    PTP_POOL Pool;
    PTP_WORK Work;
#endif

    PXSize StackReserve;
    PXSize StackCommit;

    PXInt16U ThreadsMinimum;
    PXInt16U ThreadsMaximum;

    PXInt32U Flags;

    PXInt32U TaskCounter;
}
PXThreadPool;

PXPrivate PXTask* PXAPI PXThreadPoolTaskNextWorkGet(PXThreadPool* pxThreadPool);
PXPrivate PXTask* PXAPI PXThreadPoolTaskNextFreeGet(PXThreadPool* pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour);

PXPrivate PXThread* PXAPI PXThreadPoolThreadSelf(PXThreadPool* const pxThreadPool);
PXPrivate PXActionResult PXAPI PXThreadPoolTaskInvoke(PXTask* const pxTask);


PXPublic PXBool PXAPI PXThreadPoolIsMainThread();
PXPublic PXActionResult PXAPI PXThreadPoolClose(PXThreadPool* pxThreadPool);
PXPublic PXActionResult PXAPI PXThreadPoolCreate(PXThreadPool* pxThreadPool);
PXPublic PXActionResult PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);
PXPublic PXActionResult PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);

PXPublic void PXAPI PXThreadPoolWaking(PXThreadPool* pxThreadPool);

// Task will be added to the task queue. 
// Inserted to any task that is avalibe. 
// If not a task will be created internally.
PXPublic PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour);
PXPublic PXActionResult PXAPI PXThreadPoolQueueTask(PXThreadPool* const pxThreadPool, PXTask* const pxTask);
PXPublic PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning);


// Call this function in the main thread for syncronous functions
PXPublic PXActionResult PXAPI PXThreadPoolProcessSYNC(PXThreadPool* const pxThreadPool);
// Internal function to process async calls
PXPrivate PXThreadResult PXOSAPI PXThreadPoolProcessASYNC(PXThreadPool* const pxThreadPool);

#endif

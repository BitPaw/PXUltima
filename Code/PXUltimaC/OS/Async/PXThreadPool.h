#ifndef PXThreadPoolInclude
#define PXThreadPoolInclude

#include <Media/PXResource.h>
#include <OS/Async/PXThread.h>
#include <Container/List/PXList.h>

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
}
PXThreadPool;

PXPrivate PXTask* PXAPI PXThreadPoolTaskNextWorkGet(PXThreadPool* pxThreadPool);
PXPrivate PXTask* PXAPI PXThreadPoolTaskNextFreeGet(PXThreadPool* pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour);

PXPrivate PXThreadResult __stdcall PXThreadPoolCallBack(PXThreadPool* const pxThreadPool);

PXPublic PXActionResult PXAPI PXThreadPoolClose(PXThreadPool* pxThreadPool);
PXPublic PXActionResult PXAPI PXThreadPoolCreate(PXThreadPool* pxThreadPool);
PXPublic PXActionResult PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);
PXPublic PXActionResult PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);

// Task will be added to the task queue. 
// Inserted to any task that is avalibe. 
// If not a task will be created internally.
PXPublic PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter1, void* parameter2, const PXInt32U behaviour);
PXPublic PXActionResult PXAPI PXThreadPoolQueueTask(PXThreadPool* const pxThreadPool, PXTask* const pxTask);
PXPublic PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning);

#endif

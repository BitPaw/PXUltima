#ifndef PXThreadPoolInclude
#define PXThreadPoolInclude

#include <Media/PXResource.h>
#include <OS/Async/PXThread.h>

typedef struct PXThreadPool_
{
    PXSize ThreadListSize;
    PXThread* ThreadList;

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

PXPrivate PXThreadResult __stdcall PXThreadPoolCallBack(PXThreadPool* const pxThreadPool);

PXPublic PXActionResult PXAPI PXThreadPoolClose(PXThreadPool* const pxThreadPool);
PXPublic PXActionResult PXAPI PXThreadPoolCreate(PXThreadPool* const pxThreadPool);
PXPublic PXActionResult PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);
PXPublic PXActionResult PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);
PXPublic PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter);
PXPublic PXActionResult PXAPI PXThreadPoolQueueTask(PXThreadPool* const pxThreadPool, PXTask* const pxTask);
PXPublic PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning);

#endif

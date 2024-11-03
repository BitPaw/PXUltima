#ifndef PXThreadPoolInclude
#define PXThreadPoolInclude

#include <Media/PXResource.h>

typedef struct PXThreadPool_
{
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


PXPublic void PXAPI PXThreadPoolClose(PXThreadPool* const pxThreadPool);
PXPublic void PXAPI PXThreadPoolCreate(PXThreadPool* const pxThreadPool);
PXPublic void PXAPI PXThreadPoolThreadMaximumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);
PXPublic void PXAPI PXThreadPoolThreadMinimumSet(PXThreadPool* const pxThreadPool, const PXInt32U amount);

PXPublic PXActionResult PXAPI PXThreadPoolQueueWork(PXThreadPool* const pxThreadPool, void* function, void* parameter);

PXPublic PXActionResult PXAPI PXThreadPoolWaitForAll(PXThreadPool* const pxThreadPool, const PXBool cancelRunning);

#endif

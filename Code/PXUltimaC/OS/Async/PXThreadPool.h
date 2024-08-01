#ifndef PXThreadPoolInclude
#define PXThreadPoolInclude

#include <Media/PXResource.h>

typedef struct PXThreadPool_
{
    PTP_POOL Pool;
    PXSize StackReserve;
    PXSize StackCommit;

    PTP_WORK Work;

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
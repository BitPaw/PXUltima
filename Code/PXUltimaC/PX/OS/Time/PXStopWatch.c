#include "PXStopWatch.h"

#include <PX/OS/Memory/PXMemory.h>

void PXAPI PXStopWatchConstruct(PXStopWatch PXREF stopWatch)
{
    PXMemoryClear(stopWatch, sizeof(PXStopWatch));
}

void PXAPI PXStopWatchSet(PXStopWatch PXREF stopWatch, void PXREF data, const PXSize dataSize)
{
    stopWatch->TimeStampData = stopWatch;
    stopWatch->TimeStampDataSizeAllocated = dataSize;
    stopWatch->TimeStampDataSizeUsed = 0;
}

void PXAPI PXStopWatchStart(PXStopWatch PXREF stopWatch)
{
    PXStopWatchReset(stopWatch);

    PXTimeNow(&stopWatch->TimeStartreference);
}

PXBool PXAPI PXStopWatchTrigger(PXStopWatch PXREF stopWatch, PXTime PXREF time)
{
    /*
    const PXBool canDo = stopWatch->TimeStampDataSizeAllocated > (stopWatch->TimeStampDataSizeUsed + 1);

    if (!canDo)
    {
        PXMemoryClear(time, sizeof(PXTime));

        return PXNo;
    }

    PXTime* pxTimeCurrent = &stopWatch->TimeStampData[stopWatch->TimeStampDataSizeUsed++];
    */
    PXTime rightNow;

    PXTimeNow(&rightNow);

    PXTimeDelta(&stopWatch->TimeStartreference, &rightNow, time);

    return PXYes;
}

void PXAPI PXStopWatchReset(PXStopWatch PXREF stopWatch)
{
    PXMemoryClear(stopWatch->TimeStampData, sizeof(PXTime) * stopWatch->TimeStampDataSizeAllocated);

    stopWatch->TimeStampDataSizeUsed = 0;
}

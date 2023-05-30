#include "PXStopWatch.h"

#include <OS/Memory/PXMemory.h>

void PXStopWatchConstruct(PXStopWatch* const stopWatch)
{
    PXMemoryClear(stopWatch, sizeof(PXStopWatch));
}

void PXStopWatchSet(PXStopWatch* const stopWatch, void* const data, const PXSize dataSize)
{
    stopWatch->TimeStampData = stopWatch;
    stopWatch->TimeStampDataSizeAllocated = dataSize;
    stopWatch->TimeStampDataSizeUsed = 0;
}

void PXStopWatchStart(PXStopWatch* const stopWatch)
{
    PXStopWatchReset(stopWatch);

    {
        PXTime pxTime;

        PXStopWatchTrigger(stopWatch, &pxTime);
    } 
}

PXBool PXStopWatchTrigger(PXStopWatch* const stopWatch, PXTime* const time)
{
    const PXBool canDo = stopWatch->TimeStampDataSizeAllocated > (stopWatch->TimeStampDataSizeUsed + 1);
   
    if (!canDo)
    {
        PXMemoryClear(time, sizeof(PXTime));

        return PXNo;
    }

    PXTime* pxTimeCurrent = &stopWatch->TimeStampData[stopWatch->TimeStampDataSizeUsed++];

    PXTimeNow(pxTimeCurrent);

    PXMemoryCopy(pxTimeCurrent, sizeof(PXTime), time, sizeof(PXTime));

    return PXYes;
}

void PXStopWatchReset(PXStopWatch* const stopWatch)
{
    PXMemoryClear(stopWatch->TimeStampData, sizeof(PXTime) * stopWatch->TimeStampDataSizeAllocated);

    stopWatch->TimeStampDataSizeUsed = 0;
}

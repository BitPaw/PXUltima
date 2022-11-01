#include "PXStopWatch.h"

#include <Memory/PXMemory.h>

void PXStopWatchConstruct(PXStopWatch* const stopWatch)
{
    MemorySet(stopWatch, sizeof(PXStopWatch), 0);
}

void PXStopWatchSet(PXStopWatch* const stopWatch, void* const data, const size_t dataSize)
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
        MemorySet(time, sizeof(PXTime), 0);

        return PXNo;
    }

    PXTime* pxTimeCurrent = &stopWatch->TimeStampData[stopWatch->TimeStampDataSizeUsed++];

    PXTimeNow(pxTimeCurrent);

    MemoryCopy(pxTimeCurrent, sizeof(PXTime), time, sizeof(PXTime));

    return PXYes;
}

void PXStopWatchReset(PXStopWatch* const stopWatch)
{
    MemorySet(stopWatch->TimeStampData, sizeof(PXTime) * stopWatch->TimeStampDataSizeAllocated, 0);

    stopWatch->TimeStampDataSizeUsed = 0;
}

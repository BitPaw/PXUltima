#pragma once

#ifndef PXStopWatchIncluded
#define PXStopWatchIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Time/PXTime.h>

typedef struct PXStopWatch_
{
    PXTime TimeStartreference;

    PXTime* TimeStampData;
    PXSize TimeStampDataSizeUsed;
    PXSize TimeStampDataSizeAllocated;
}
PXStopWatch;

PXPublic void PXAPI PXStopWatchConstruct(PXStopWatch PXREF stopWatch);

PXPublic void PXAPI PXStopWatchSet(PXStopWatch PXREF stopWatch, void PXREF data, const PXSize dataSize);

PXPublic void PXAPI PXStopWatchStart(PXStopWatch PXREF stopWatch);
PXPublic PXBool PXAPI PXStopWatchTrigger(PXStopWatch PXREF stopWatch, PXTime PXREF time);
PXPublic void PXAPI PXStopWatchReset(PXStopWatch PXREF stopWatch);

#endif
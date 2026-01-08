#pragma once

#ifndef PXTimerIncluded
#define PXTimerIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXTimerEventInfo_
{
    struct PXTimer_* TimerReference;

    PXI32U Acuracy;

    PXBool WasHandled;

    //PXI32S DelayShift;
}
PXTimerEventInfo;

typedef PXResult(PXAPI* PXTimerCallBack)(void PXREF owner, PXTimerEventInfo PXREF pxEngineTimerEventInfo);

typedef struct PXTimer_
{
    PXECSInfo Info;

    void* Owner;
    PXTimerCallBack CallBack;

    PXI32U TimeStampStart;

    PXI32U TimeDeltaTarget;
    PXI32S TimeDelayShift;
}
PXTimer;



typedef struct PXTimerCreateInfo_
{
    PXECSCreateInfo Info;

    void* Owner;
    void* CallBack;
    PXSize TimeDeltaTarget;
}
PXTimerCreateInfo;


PXPublic PXResult PXAPI PXTimerCreate(PXTimer PXREF pxTimer, PXTimerCreateInfo PXREF pxTimerCreateInfo);

#endif
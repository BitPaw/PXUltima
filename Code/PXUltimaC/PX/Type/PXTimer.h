#pragma once

#ifndef PXTimerIncluded
#define PXTimerIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Type/PXTime.h>

typedef struct PXTimer_ PXTimer;

typedef struct PXTimerEventInfo_
{
    void* CallBackOwner;

    PXTimer* TimerReference;

    PXI32U Acuracy;
    PXBool WasHandled;
    //PXI32S DelayShift;
}
PXTimerEventInfo;

typedef PXResult(PXAPI* PXTimerCallBack)(PXTimerEventInfo PXREF pxEngineTimerEventInfo);

typedef struct PXTimer_
{
    PXECSInfo Info;

    void* Owner;
    PXTimerCallBack CallBack;

    PXBool Enabled;

    PXTimeStamp TimeStampStart;

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

PXInternal void PXAPI PXTimerRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXTimerCreate(PXTimer** pxTimerREF, PXTimerCreateInfo PXREF pxTimerCreateInfo);
PXPublic PXResult PXAPI PXTimerRelease(PXTimer PXREF pxTimer);

PXPublic PXResult PXAPI PXTimerRun(PXTimer PXREF pxTimer);
PXPublic PXResult PXAPI PXTimerStop(PXTimer PXREF pxTimer);

#endif
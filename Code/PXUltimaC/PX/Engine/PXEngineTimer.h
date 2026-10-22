#pragma once

#include <PX/Engine/ECS/PXECS.h>
#include "PXEngineModule.h"

#include <PX/Type/PXTime.h>



typedef struct PXEngineTime_ PXEngineTime;


typedef struct PXEngineTime_
{
    PXEngineModule ModuleInfo;

    //-------------------------------------------
    // Timestamp
    //PXF64 TimeDeltaTimeTarget;
    //PXF64 TimeDeltaTimeCurrent;

    //PXF64 TimeStampLast; // Time after last frame.
    //PXF64 TimeStampCurrent; // Our "NOW"
    //PXF64 TimeStampNext; // Expected time to update
    //-------------------------------------------

    //-------------------------------------------
    // Timer handles, OS callback when it wants to do something
#if OSUnix
#elif OSWindows
    HANDLE TimerHiPrecisionWaitable;
#endif
    //-------------------------------------------

    PXTimeStamp TimeLastInNS;
    PXTimeStamp TimeFrameInNS; // Time between 
    PXTimeStamp TimeWorkInNS;
    PXTimeStamp TimeIDLEInNS;

    // In % how much we push the system.
    //  3%=We are chilling
    // 95%=We are close to drop in frames.
    PXF32 FrameTimeUtilization; 


    //-----------------------------------------------------
    // FrameTimeLimit
    // - When a update takes longer then our target Hz 
    //   update we have two choices. 
    //   A: Limit frametime, we "skip" updates and elements 
    //      will visually teleport.
    //   B: Redo all updates in a loop. We have all 
    //      calculations but the update will take as long 
    //      as it takes. Which is bad if we want fast updates.
    //-----------------------------------------------------
  
    // If true, we allow to skip "true" frames.
    // If false, when the system can't update in a given time, we will re
    PXBool FrameTimeLimitEnable;
    PXTimeStamp TimeFrameTargetInNS; // 1/Hz, 1/60=16.66ms, 1/144=6.94ms
    PXTimeStamp TimeFrameLimitedInNS; // Use this instead of our frametime

 
    PXTimeStamp TimeDeltaStep; // Time we target as internal update. Can be same as FPS but for internal ticks.
    PXTimeStamp TimeDeltaAccumulator;

    PXF64 TimeDelta; // ONLY for physics inside the game tick

    // ONLY for rendering, defines how much we differ form point A to point B
    // TimeAlpha = TimeDeltaAccumulator / TimeDelta
    PXF64 TimeAlpha;
    //-----------------------------------------------------

    //-----------------------------------------------------
    // FPS
    PXI64U FrameCounter; // Amount of frames we collected since FPS count.
    PXF64 FramesPerSecondTimerInNS; // Timer to get a more acurate FPS number.
    PXF64 FramesPerSecond; // Actual FPS.
    //-----------------------------------------------------
}
PXEngineTime;

typedef struct PXEngineTimeCreateInfo_
{
    PXECSCreateInfo Info;

    PXI32U RefreshHz; // Internal updaterate
}
PXEngineTimeCreateInfo;

PXPublic PXResult PXAPI PXEngineTimeCreate(PXEngineTime** pxEngineTimeREF, PXEngineTimeCreateInfo PXREF pxEngineTimeCreateInfo);
PXPublic PXResult PXAPI PXEngineTimeUpdate(PXEngineTime PXREF pxEngineTime, PXEngineUpdateInfo PXREF pxEngineUpdateInfo);


PXPublic PXResult PXAPI PXEngineTimeWaitUI(PXEngine PXREF pxEngine, PXEngineTime PXREF pxEngineTime);
PXPublic PXResult PXAPI PXEngineTimeWaitGAME(PXEngine PXREF pxEngine, PXEngineTime PXREF pxEngineTime);
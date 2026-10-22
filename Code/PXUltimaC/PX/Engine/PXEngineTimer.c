#include "PXEngineTimer.h"
#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Type/PXTimer.h>
#include <PX/Math/PXMath.h>
#include <PX/Engine/PXEngine.h>
#include <PX/OS/Async/PXThread.h>


const char PXEngineTimeText[] = "Time-PX";
const PXI8U PXEngineTimeLength = sizeof(PXEngineTimeText);
const PXECSRegisterInfoStatic PXEngineTimeInfoStatic =
{
    {sizeof(PXEngineTimeText), sizeof(PXEngineTimeText), PXEngineTimeText, TextFormatASCII},
    sizeof(PXEngineTime),
    __alignof(PXEngineTime),
    PXECSTypeSystem,
    PXEngineTimeCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineTimeInfoDynamic;


PXResult PXAPI PXEngineTimeCreate(PXEngineTime** pxEngineTimeREF, PXEngineTimeCreateInfo PXREF pxEngineTimeCreateInfo)
{
    PXEngineTime* pxEngineTime = PXNull;

    if(!(pxEngineTimeREF && pxEngineTimeCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineTimeCreateInfo->Info.Static = &PXEngineTimeInfoStatic;
    pxEngineTimeCreateInfo->Info.Dynamic = &PXEngineTimeInfoDynamic;
    PXResult pxResult = PXECSCreate(pxEngineTimeREF, pxEngineTimeCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    if(!pxEngineTimeCreateInfo->RefreshHz)
    {
        pxEngineTimeCreateInfo->RefreshHz = 60;
    }

    PXTimeStampInit();

    pxEngineTime = *pxEngineTimeREF;
    pxEngineTime->TimeFrameTargetInNS.Value = 1000000000LL / pxEngineTimeCreateInfo->RefreshHz;
    pxEngineTime->FrameTimeLimitEnable = PXTrue;

    PXEngineModuleCallbackUpdateASYNCSet(&pxEngineTime->ModuleInfo, PXEngineTimeUpdate);

#if OSUnix
#elif OSWindows
    pxEngineTime->TimerHiPrecisionWaitable = CreateWaitableTimerExA
    (
        NULL,
        PXNull, // High precition timers can't be named.
        CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, // Tell Windows to use maximum precision
        TIMER_ALL_ACCESS
    );     
    pxResult = PXErrorCurrent(pxEngineTime->TimerHiPrecisionWaitable > 0);
#endif

    pxEngineTime->TimeLastInNS = PXTimeStampNow();

    PXI64U timePrecision = PXTimeStampPrecision();

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineTimeText,
        "Create",
        "\n"
        "%20s : %6i Hz\n"
        "%20s : %6i ns",
        "Update rate", pxEngineTimeCreateInfo->RefreshHz,
        "Time precision", timePrecision
    );
#endif


    return pxResult;
}

PXResult PXAPI PXEngineTimeUpdate(PXEngineTime PXREF pxEngineTime, PXEngineUpdateInfo PXREF pxEngineUpdateInfo)
{
    PXResult pxResult = PXResultInvalid;

#if 0
    PXDictionary PXREF timerList = 0;// &pxEngine->ResourceManager->TimerLookUp;

    for(PXSize timerIndex = 0; timerIndex < timerList->List.EntryAmountUsed; ++timerIndex)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXTimer* pxTimer = PXNull;

        PXDictionaryIndex(timerList, timerIndex, &pxDictionaryEntry);

        pxTimer = *(PXTimer**)pxDictionaryEntry.ValueAddress;

        const PXBool isActive = PXECSInfoFlagStateCheck(&pxTimer->Info, PXECSFlagBehaviourActive);

        if(!isActive)
        {
            continue;
        }

        // Check timing, is it time to call yet?
        const PXI32U timeStamp = PXTimeStampNow();
        const PXI32U timeerDeltaTime = timeStamp - pxTimer->TimeStampStart;
        const PXBool isTimeDelayToStrong = ((PXI32S)pxTimer->TimeDeltaTarget + pxTimer->TimeDelayShift) < 0;
        const PXBool isLongEnough =
            isTimeDelayToStrong ||
            (timeerDeltaTime > (pxTimer->TimeDeltaTarget + pxTimer->TimeDelayShift));

        if(isLongEnough)
        {
            pxTimer->TimeStampStart = timeStamp;

#if 1

            PXText pxText;
            PXTextConstructBufferA(&pxText, 64);

            PXTextFormatTime(&pxText, timeerDeltaTime);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "PX",
                "Timer",
                "Trigger, Waited for %10s",
                pxText.A
            );
#endif

#endif

            if(pxTimer->CallBack)
            {
                PXTimerEventInfo pxEngineTimerEventInfo;
                PXClear(PXTimerEventInfo, &pxEngineTimerEventInfo);
                pxEngineTimerEventInfo.CallBackOwner = pxEngine;
                pxEngineTimerEventInfo.TimerReference = pxTimer;

                pxTimer->CallBack(&pxEngineTimerEventInfo);
            }
        }
    }
#endif

    return pxResult;
}

PXResult PXAPI PXEngineTimeWaitUI(PXEngine PXREF pxEngine, PXEngineTime PXREF pxEngineTime)
{
    if(!(pxEngine && pxEngineTime))
    {
        return PXResultRefusedParameterNull;
    }

    if(!PXEngineModuleIsActive(pxEngineTime))
    {
        return PXResultOK;
    }

    PXResult pxResult = PXResultInvalid;

#if OSUnix
    Sleep(1);
#elif OSWindows

#if 0
    PXConsoleGoToXY(0, 0);
    printf
    (
        "%25s : %12.3f ms\n"
        "%25s : %12.3f ms\n"
        "%25s : %12.3f s\n"
        "%25s : %12.3f s\n"
        "%25s : %12.3f s\n"
        "%25s : %12.3f s\n"
        "%25s : %12.3f s\n"
        "%25s : %12.3f ms\n",
        "TimeDeltaTimeTarget", pxEngine->TimeDeltaTimeTarget * 1000,
        "TimeDeltaTimeCurrent", pxEngine->TimeDeltaTimeCurrent * 1000,
        "TimeStampLast", pxEngine->TimeStampLast,
        "TimeStampCurrent", pxEngine->TimeStampCurrent,
        "TimeStampNext", pxEngine->TimeStampNext,
        "FPSTarget", pxEngine->FPSTarget,
        "FPSCurrent", pxEngine->FPSCurrent,
        "FrameTime", pxEngine->FrameTime * 1000
    );
#endif

    PXThread PXREF pxThread = PXEngineRootThread(pxEngine);

    HANDLE* handleList = PXNull;
    DWORD count = 0;
    BOOL  waitAll = FALSE;
    DWORD timeoutTimeMS = INFINITE;
    DWORD wakeMask = QS_ALLINPUT;
    DWORD dwFlags = MWMO_INPUTAVAILABLE;
    DWORD resultIndex = 0;

    // If we have the main thread of the engine, we want to know if it fails.
    PXThreadState threadState = PXThreadStateGet(pxThread);

    if(PXThreadStateRunning == threadState)
    {
        handleList = &pxThread->Handle.Data;
        ++count;
    }

    if(1)
    {
        resultIndex = MsgWaitForMultipleObjectsEx
        (
            count,
            handleList,
            timeoutTimeMS,
            wakeMask,
            dwFlags
        ); // Windows NT 4.0
    }
    else
    {
        resultIndex = MsgWaitForMultipleObjects
        (
            count,
            handleList,
            waitAll,
            timeoutTimeMS,
            wakeMask
        ); // Windows NT 3.1
    }

    // Events are always +1 more than we provided. 
    // The +1 one is a window event.
    PXBool isWindowUIMessage = resultIndex == WAIT_OBJECT_0 + count;
    PXBool isRenderLoop = resultIndex == WAIT_OBJECT_0;

    if(isWindowUIMessage)
    {
       // pxResult = PXEngineEventOnUI(pxEngine);
        PXEngineEventUIAviableSet(pxEngine);
    }

    if(isRenderLoop)
    {
        PXEngineEventThreadDoneSet(pxEngine);
    }

    // Handle??

#endif

    return pxResult;
}

PXResult PXAPI PXEngineTimeWaitGAME(PXEngine PXREF pxEngine, PXEngineTime PXREF pxEngineTime)
{
    if(!(pxEngine && pxEngineTime))
    {
        Sleep(1);
        return PXResultRefusedParameterNull;
    }

    if(!PXEngineModuleIsActive(pxEngineTime))
    {
        Sleep(1);
        return PXResultOK;
    }

    PXResult pxResult = PXResultInvalid;
    PXTimeStamp timeFrameStart = pxEngineTime->TimeLastInNS;
    PXTimeStamp timeCurrent = PXTimeStampNow();

    pxEngineTime->TimeLastInNS = timeCurrent; // Set new time
    pxEngineTime->TimeFrameInNS.Value = timeCurrent.Value - timeFrameStart.Value;
    pxEngineTime->TimeFrameLimitedInNS = pxEngineTime->TimeFrameInNS;

    // Under heavy load, the CPU might not be able to handle our frame in time.
    // For this we cap the accumulation to a maximum buffer.
    // This allows catch-ups under lag, but prevents an infinite cascading tick loop (SPIRAL OF DEATH).
    if(pxEngineTime->FrameTimeLimitEnable)
    {
        // Cap maximum catch-up time to 4 frames worth of physics simulation per loop
        PXI64U maxSimulationLagInNS = pxEngineTime->TimeFrameTargetInNS.Value * 4;
        if(pxEngineTime->TimeFrameInNS.Value > maxSimulationLagInNS)
        {
            pxEngineTime->TimeFrameLimitedInNS.Value = maxSimulationLagInNS;
        }
    }

    PXTimeStampAddNS(&pxEngineTime->TimeDeltaAccumulator, pxEngineTime->TimeFrameLimitedInNS); 

    //-----------------------------------------------------
    // Drain the accumulator in rigid chunks
    while(pxEngineTime->TimeDeltaAccumulator.Value >= pxEngineTime->TimeFrameTargetInNS.Value)
    {
        pxEngineTime->TimeDelta = PXTimeNSToSF(pxEngineTime->TimeFrameTargetInNS.Value);

        pxResult = PXEngineEventOnTick(pxEngine);

        if(PXResultOK != pxResult)
        {
            // Skip all delta steps, this keeps the time true as if we did calculations.
            // 
            // [WARNING] This only works with int. use modf() for floats!
            pxEngineTime->TimeDeltaAccumulator.Value %= pxEngineTime->TimeFrameTargetInNS.Value;

            break;
        }

        PXTimeStampReduceNS(&pxEngineTime->TimeDeltaAccumulator, pxEngineTime->TimeFrameTargetInNS);
    }

    // We are now in a "too early for next imgame tick"
    pxEngineTime->TimeAlpha = (PXF64)pxEngineTime->TimeDeltaAccumulator.Value / (PXF64)pxEngineTime->TimeFrameTargetInNS.Value;

    PXBool didNothingUntilNow = PXResultNoWorkToDo == pxResult;

    pxResult = PXEngineEventOnRender(pxEngine);

    PXBool noComputingDone = (PXResultNoWorkToDo == pxResult) && didNothingUntilNow;

    if(noComputingDone)
    {
        return PXResultNoWorkToDo;
    }

   // PXThreadYieldToOtherThreads();


    // --- THE TRULY UNBURDENED WAIT PHASE ---
    // Calculate exactly how much time is left before we need to start the next frame loop.
    timeCurrent = PXTimeStampNow();

    pxEngineTime->TimeWorkInNS.Value = timeCurrent.Value - timeFrameStart.Value;
    pxEngineTime->TimeIDLEInNS.Value = pxEngineTime->TimeFrameTargetInNS.Value - pxEngineTime->TimeWorkInNS.Value;

    pxEngineTime->FrameTimeUtilization = ((PXF32)pxEngineTime->TimeWorkInNS.Value / (PXF32)pxEngineTime->TimeFrameTargetInNS.Value) * 100;

    //-----------------------------------------------------
    // FPS
    pxEngineTime->FrameCounter++;
    pxEngineTime->FramesPerSecondTimerInNS += ((PXF64)pxEngineTime->TimeWorkInNS.Value + max(0.0, (PXF64)pxEngineTime->TimeIDLEInNS.Value)); // Accumulate total real-world time passed

    if(pxEngineTime->FramesPerSecondTimerInNS >= 1000000000.0) // Once every second, update the final FPS number
    {
        PXF64 totalSecondsPassed = pxEngineTime->FramesPerSecondTimerInNS / 1000000000.0;

        pxEngineTime->FramesPerSecond = (PXF64)pxEngineTime->FrameCounter / totalSecondsPassed;
        pxEngineTime->FrameCounter = 0;
        pxEngineTime->FramesPerSecondTimerInNS = 0.0;
    }
    //-----------------------------------------------------



    //-----------------------------------------------------
    // If late, return now.
    PXBool isLateAlready = 0.0 >= pxEngineTime->TimeIDLEInNS.Value;

    if(isLateAlready)
    {
        // We are late (lag?) dont wait and return now.
        return pxResult;
    }
    //-----------------------------------------------------


    //-----------------------------------------------------
    // Wait remaining time.
    // Target deadline timestamp in real time
    PXI64U targetWakeTimestamp = timeFrameStart.Value + pxEngineTime->TimeFrameTargetInNS.Value;

    // Windows timer, even if the DOC claims it, are very imprecise (~2ms).
    // Because of this, we only wait if we can be sure we would wait for a long time.
    if(pxEngineTime->TimeIDLEInNS.Value > 2000000) // 2 milliseconds = 2,000,000 nanoseconds
    {
        // Leave a 1.5ms buffer for the spin lock to catch perfectly and absorb OS wake-up jitter
        PXI64U sleepDurationInNS = pxEngineTime->TimeIDLEInNS.Value - 1500000;

        LARGE_INTEGER liDueTime;
        liDueTime.QuadPart = -(LONGLONG)(sleepDurationInNS / 100); // 100-nanosecond relative intervals

        SetWaitableTimer(pxEngineTime->TimerHiPrecisionWaitable, &liDueTime, 0, NULL, NULL, 0);
        WaitForSingleObject(pxEngineTime->TimerHiPrecisionWaitable, INFINITE);
    }

    // Phase 2: Ultra-Precise Spin Lock
    // Burn the remaining microsecond fractions using a low-overhead CPU hint instruction
    while(PXTimeStampNow().Value < targetWakeTimestamp)
    {
#if defined(_MSC_VER)
        _mm_pause(); // Yield processor execution pipeline on Windows/MSVC compiler
#elif defined(__GNUC__) || defined(__clang__)
        __builtin_ia32_pause(); // GCC/Clang equivalent
#endif
    }
    //-----------------------------------------------------

    return pxResult;
}
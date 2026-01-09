#include "PXTimer.h"

#include <PX/OS/Console/PXConsole.h>

const char PXTimerText[] = "Timer";

PXResult PXAPI PXTimerCreate(PXTimer PXREF pxTimer, PXTimerCreateInfo PXREF pxTimerCreateInfo)
{
    pxTimer->Owner = pxTimerCreateInfo->Owner;
    pxTimer->CallBack = pxTimerCreateInfo->CallBack;
    pxTimer->TimeDeltaTarget = pxTimerCreateInfo->TimeDeltaTarget;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXTimerText,
        "Create",
        "PXID:%i",
        pxTimer->Info.ID
    );
#endif

    pxTimer->TimeStampStart = PXTimeCounterStampGet();

    //---------------------------------------------
    // Trigger enable
    //---------------------------------------------
    if(PXResourceCreateBehaviourSpawnInScene & pxTimerCreateInfo->Info.Flags)
    {
#if 0
        PXEngineResourceActionInfo pxEngineResourceActionInfo;
        PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

        pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
        pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
        pxEngineResourceActionInfo.ChangeInfo.Type = PXEngineCreateTypeTimer;
        pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineTimer;

        PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
#endif
    }
    //---------------------------------------------

    return PXResultOK;
}
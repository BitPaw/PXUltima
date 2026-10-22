#include "PXTimer.h"

#include <PX/OS/Console/PXConsole.h>

const char PXTimerText[] = "Timer";
const PXI8U PXTimerTextLength = sizeof(PXTimerText);
const PXECSRegisterInfoStatic PXTimerRegisterInfoStatic =
{
    {sizeof(PXTimerText), sizeof(PXTimerText), PXTimerText, TextFormatASCII},
    sizeof(PXTimer),
    __alignof(PXTimer),
    PXECSTypeSystem,
    PXTimerCreate,
    PXTimerRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXTimerRegisterInfoDynamic;

void PXAPI PXTimerRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXTimerRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXTimerRegisterInfoDynamic;
}

PXResult PXAPI PXTimerCreate(PXTimer** pxTimerREF, PXTimerCreateInfo PXREF pxTimerCreateInfo)
{
    PXTimer* pxTimer = PXNull;

    if(!(pxTimerREF && pxTimerCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxTimerCreateInfo->Info.Static = &PXTimerRegisterInfoStatic;
    pxTimerCreateInfo->Info.Dynamic = &PXTimerRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxTimerREF, pxTimerCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTimer = *pxTimerREF;

    pxTimer->Owner = pxTimerCreateInfo->Owner;
    pxTimer->CallBack = pxTimerCreateInfo->CallBack;
    pxTimer->TimeDeltaTarget = pxTimerCreateInfo->TimeDeltaTarget;
    pxTimer->TimeStampStart = PXTimeStampNow();

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

PXResult PXAPI PXTimerRelease(PXTimer PXREF pxTimer)
{
    return PXResultOK;
}

PXResult PXAPI PXTimerRun(PXTimer PXREF pxTimer)
{
    pxTimer->Enabled = PXTrue;

    return PXResultOK;
}

PXResult PXAPI PXTimerStop(PXTimer PXREF pxTimer)
{
    pxTimer->Enabled = PXFalse;

    return PXResultOK;
}
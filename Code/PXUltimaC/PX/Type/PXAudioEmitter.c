#include "PXAudioEmitter.h"

#include <PX/OS/PXOS.h>

const char PXAudioEmitterName[] = "AudioEmitter";
const PXI8U PXAudioEmitterNameLength = sizeof(PXAudioEmitterName);
const PXECSRegisterInfoStatic PXAudioEmitterRegisterInfoStatic =
{
    {sizeof(PXAudioEmitterName), sizeof(PXAudioEmitterName), PXAudioEmitterName, TextFormatASCII},
    sizeof(PXAudioEmitter),
    __alignof(PXAudioEmitter),
    PXECSTypeEntity,
    PXAudioEmitterCreate,
    PXAudioEmitterRelease,
    PXAudioEmitterDraw
};
PXECSRegisterInfoDynamic PXAudioEmitterRegisterInfoDynamic;

void PXAPI PXAudioEmitterRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXAudioEmitterRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXAudioEmitterRegisterInfoDynamic;
}

PXResult PXAPI PXAudioEmitterCreate(PXAudioEmitter** pxAudioEmitterREF, PXAudioEmitterCreateInfo PXREF pxAudioEmitterCreateInfo)
{
    PXAudioEmitter* pxAudioEmitter = PXNull;

    pxAudioEmitterCreateInfo->Info.Static = &PXAudioEmitterRegisterInfoStatic;
    pxAudioEmitterCreateInfo->Info.Dynamic = &PXAudioEmitterRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxAudioEmitterREF, pxAudioEmitterCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxAudioEmitter = *pxAudioEmitterREF;
    pxAudioEmitter->Mode = pxAudioEmitterCreateInfo->Mode;

    if(PXAudioEmitterModeInvalid == pxAudioEmitterCreateInfo->Mode)
    {
        PXDebugBreak();
        return PXResultInvalid;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioEmitterRelease(const PXAudioEmitter PXREF pxAudioEmitter)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXAudioEmitterDraw(const PXAudioEmitter PXREF pxAudioEmitter, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    return PXResultRefusedNotImplemented;
}
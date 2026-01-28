#include "PXAudioEmitter.h"

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

PXResult PXAPI PXAudioEmitterRegisterToECS(void)
{
    PXECSRegister(&PXAudioEmitterRegisterInfoStatic, &PXAudioEmitterRegisterInfoDynamic);

    return PXResultOK;
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

    return PXResultOK;
}

PXResult PXAPI PXAudioEmitterRelease(const PXAudioEmitter PXREF pxAudioEmitter)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXAudioEmitterDraw(const PXAudioEmitter PXREF pxAudioEmitter, PXDrawInfo PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}
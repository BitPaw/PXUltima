#include "PXAudioListener.h"

const char PXAudioListenerName[] = "AudioListener";
const PXI8U PXAudioListenerNameLength = sizeof(PXAudioListenerName);
const PXECSRegisterInfoStatic PXAudioListenerRegisterInfoStatic =
{
    {sizeof(PXAudioListenerName), sizeof(PXAudioListenerName), PXAudioListenerName, TextFormatASCII},
    sizeof(PXAudioListener),
    __alignof(PXAudioListener),
    PXECSTypeEntity,
    PXAudioListenerCreate,
    PXAudioListenerRelease,
    PXAudioListenerDraw
};
PXECSRegisterInfoDynamic PXAudioListenerRegisterInfoDynamic;

PXResult PXAPI PXAudioListenerRegisterToECS(void)
{
    PXECSRegister(&PXAudioListenerRegisterInfoStatic, &PXAudioListenerRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXAudioListenerCreate(PXAudioListener** pxAudioListenerREF, PXAudioListenerCreateInfo PXREF pxAudioListenerCreateInfo)
{
    PXAudioListener* pxAudioListener = PXNull;

    pxAudioListenerCreateInfo->Info.Static = &PXAudioListenerRegisterInfoStatic;
    pxAudioListenerCreateInfo->Info.Dynamic = &PXAudioListenerRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxAudioListenerREF, pxAudioListenerCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxAudioListener = *pxAudioListenerREF;

    return PXResultOK;
}

PXResult PXAPI PXAudioListenerRelease(const PXAudioListener PXREF pxAudioListener)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXAudioListenerDraw(const PXAudioListener PXREF pxAudioListener, PXDrawInfo PXREF pxDrawInfo)
{
    return PXActionRefusedNotImplemented;
}
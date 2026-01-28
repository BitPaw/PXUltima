#include "PXAudioDevice.h"
#include <PX/Engine/ECS/PXECS.h>

const char PXAudioDeviceName[] = "AudioDevice";
const PXI8U PXAudioDeviceNameLength = sizeof(PXAudioDeviceName);
const PXECSRegisterInfoStatic PXAudioDeviceRegisterInfoStatic =
{
    {sizeof(PXAudioDeviceName), sizeof(PXAudioDeviceName), PXAudioDeviceName, TextFormatASCII},
    sizeof(PXAudioDevice),
    __alignof(PXAudioDevice),
    PXECSTypeResource,
    PXAudioDeviceCreate,
    PXAudioDeviceRelase,
    PXNull

};
PXECSRegisterInfoDynamic PXAudioDeviceRegisterInfoDynamic;

PXResult PXAPI PXAudioDeviceRegisterToECS()
{
    PXECSRegister(&PXAudioDeviceRegisterInfoStatic, &PXAudioDeviceRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXAudioDeviceCreate(PXAudioDevice** pxAudioDeviceREF, PXAudioDeviceCreateInfo PXREF pxAudioDeviceCreateInfo)
{
    if(!(pxAudioDeviceREF && pxAudioDeviceCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXAudioDevice* pxAudioDevice = PXNull;

    pxAudioDeviceCreateInfo->Info.Static = &PXAudioDeviceRegisterInfoStatic;
    pxAudioDeviceCreateInfo->Info.Dynamic = &PXAudioDeviceRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxAudioDeviceREF, pxAudioDeviceCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxAudioDevice = *pxAudioDeviceREF;

    return PXResultOK;
}

PXResult PXAPI PXAudioDeviceRelase(PXAudioDevice PXREF pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

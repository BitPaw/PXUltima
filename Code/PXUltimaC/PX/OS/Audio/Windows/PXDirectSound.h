#pragma once

#ifndef PXDirectSoundIncluded
#define PXDirectSoundIncluded

#include <PX/Type/PXAudioDevice.h>
#include <PX/Engine/PXAudioSystem.h>

//typedef struct PXDirectSound_ PXDirectSound;

/*
typedef struct PXAudioDeviceDetectObjectEventData_
{
    PXI32U IndexCurrent;
    PXI32U IndexTarget;
    PXAudioDevice* AudioDevice;
}
PXAudioDeviceDetectObjectEventData;
*/


PXPublic PXResult PXAPI PXDirectSoundInitialize(PXAudioSystem PXREF pxAudioSystem, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo);

PXPublic PXResult PXAPI PXDirectSoundDeviceListUpdate(PXAudioSystem PXREF pxAudioSystem);
PXPublic PXResult PXAPI PXDirectSoundDeviceFetch(PXAudioSystem PXREF pxAudioSystem, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);

PXPublic PXResult PXAPI PXDirectSoundDeviceOpen(PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXDirectSoundDeviceClose(PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundDeviceBufferCreate
(
    PXAudioDevice PXREF pxAudioDevice,
    PXSound PXREF pxSound
);

// Retrieves or writes a property value on a sound device
PXPublic PXResult PXAPI PXDirectSoundDeviceProperty(PXAudioDevice PXREF pxAudioDevice, PXSoundDeviceProperty PXREF pxSoundDeviceProperty);


PXPublic PXResult PXAPI PXDirectSoundEffectEnable(PXAudioSystem PXREF pxAudioSystem, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundEffectUpdate(PXAudioSystem PXREF pxAudioSystem, PXAudioDevice PXREF pxAudioDevice, PXAudioEffect PXREF pxAudioEffect);


#endif

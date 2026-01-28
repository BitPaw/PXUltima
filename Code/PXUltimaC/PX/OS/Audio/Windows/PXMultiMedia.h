#pragma once

#ifndef PXMultiMediaIncluded
#define PXMultiMediaIncluded
#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/System/PXAudioSystem.h>

PXPublic PXResult PXAPI PXMultiMediaInitialize(PXAudioMultiMedia* PXREF pxAudioMultiMedia, PXAudioSystemCreateInfo PXREF pxAudioInitializeInfo);

PXPublic PXResult PXAPI PXMultiMediaDeviceProperty(PXAudioDevice PXREF pxAudioDevice, PXSoundDeviceProperty PXREF pxSoundDeviceProperty);


PXPublic PXResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, PXSound PXREF pxSound);

#endif
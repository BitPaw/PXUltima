#pragma once

#ifndef PXMultiMediaIncluded
#define PXMultiMediaIncluded
#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/AudioDevice/PXAudioDevice.h>
#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXAudioMultiMedia_ PXAudioMultiMedia;

PXPublic PXResult PXAPI PXMultiMediaInitialize(PXAudioSystem PXREF pxAudioSystem, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo);

PXPublic PXResult PXAPI PXMultiMediaDeviceProperty(PXAudioDevice PXREF pxAudioDevice, PXSoundDeviceProperty PXREF pxSoundDeviceProperty);


PXPublic PXResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, PXSound PXREF pxSound);

#endif
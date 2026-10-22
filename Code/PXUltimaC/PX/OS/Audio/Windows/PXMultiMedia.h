#pragma once

#ifndef PXMultiMediaIncluded
#define PXMultiMediaIncluded

#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXVector.h>

PXPublic PXResult PXAPI PXMultiMediaInitialize(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo);

PXPublic PXSize PXAPI PXMultiMediaInputAmount(PXEngineAudio PXREF pxEngineAudio);
PXPublic PXAudioDevice* PXAPI PXMultiMediaInputIndex(PXEngineAudio PXREF pxEngineAudio, const PXSize index);
PXPublic PXResult PXAPI PXMultiMediaInputOpen(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaInputClose(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);

PXPublic PXSize PXAPI PXMultiMediaOutputAmount(PXEngineAudio PXREF pxEngineAudio);
PXPublic PXAudioDevice* PXAPI PXMultiMediaOutputIndex(PXEngineAudio PXREF pxEngineAudio, const PXSize index);
PXPublic PXResult PXAPI PXMultiMediaOutputOpen(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaOutputClose(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);




/*
PXPublic PXResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia PXREF pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia PXREF pxAudioMultiMedia, PXAudioDevice PXREF pxAudioDevice, PXSound PXREF pxSound);
*/

#endif
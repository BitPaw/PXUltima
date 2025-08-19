#ifndef PXMultiMediaInclude
#define PXMultiMediaInclude

#include <PX/OS/Audio/PXAudio.h>

PXPublic PXActionResult PXAPI PXMultiMediaInitialize(PXAudioMultiMedia** const pxAudioMultiMedia, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXActionResult PXAPI PXMultiMediaDeviceProperty(PXAudioDevice* const pxAudioDevice, PXSoundDeviceProperty* const pxSoundDeviceProperty);


PXPublic PXActionResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXActionResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound);

#endif
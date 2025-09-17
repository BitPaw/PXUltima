#ifndef PXMultiMediaIncluded
#define PXMultiMediaIncluded

#include <PX/OS/Audio/PXAudio.h>

PXPublic PXResult PXAPI PXMultiMediaInitialize(PXAudioMultiMedia** const pxAudioMultiMedia, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXResult PXAPI PXMultiMediaDeviceProperty(PXAudioDevice* const pxAudioDevice, PXSoundDeviceProperty* const pxSoundDeviceProperty);


PXPublic PXResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound);

#endif
#ifndef PXMultiMediaInclude
#define PXMultiMediaInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXMultiMediaInitialize(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXActionResult PXAPI PXMultiMediaDeviceAmount(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceFetch(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceFetchAll(PXAudioMultiMedia* const pxAudioMultiMedia, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXActionResult PXAPI PXMultiMediaDeviceOpen(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceClose(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceLoad(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound);

//--------------------------------------------------------
// Device - Input
//--------------------------------------------------------


//--------------------------------------------------------
// Device - Output
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXMultiMediaDeviceBreakLoop(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDevicePositionGet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDevicePlaybackRateGet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, PXInt32U* const playbackRate);
PXPublic PXActionResult PXAPI PXMultiMediaDevicePlaybackRateSet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const PXInt32U playbackRate);

//--------------------------------------------------------
// Device - Pitch
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXMultiMediaDevicePitchIncrease(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, float amount);
PXPublic PXActionResult PXAPI PXMultiMediaDevicePitchSet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
PXPublic PXActionResult PXAPI PXMultiMediaDevicePitchReduce(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, float amount);

//--------------------------------------------------------
// Device - Volume
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXMultiMediaDeviceVolumeGet(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceVolumeSetEqual(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceVolumeSetIndividual(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

//--------------------------------------------------------
// Device - Playback
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXMultiMediaDeviceStart(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceStop(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDevicePause(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceRestart(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXMultiMediaDeviceReset(PXAudioMultiMedia* const pxAudioMultiMedia, PXAudioDevice* const pxAudioDevice);

#endif
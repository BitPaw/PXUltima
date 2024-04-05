#ifndef PXXAudioInclude
#define PXXAudioInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

PXPublic PXActionResult PXAPI PXXAudioInitialize(PXAudioXSystem* const pxAudioXSystem, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXActionResult PXAPI PXXAudioDeviceAmount(PXAudioXSystem* const pxAudioXSystem, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo);
PXPublic PXActionResult PXAPI PXXAudioDeviceFetch(PXAudioXSystem* const pxAudioXSystem, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXXAudioDeviceFetchAll(PXAudioXSystem* const pxAudioXSystem, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXActionResult PXAPI PXXAudioDeviceOpen(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
PXPublic PXActionResult PXAPI PXXAudioDeviceClose(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXXAudioDeviceLoad
(
	PXAudioXSystem* const pxAudioXSystem,
	PXAudioDevice* const pxAudioDevice,
	PXSound* const pxSound
);

//--------------------------------------------------------
// Device - Input
//--------------------------------------------------------


//--------------------------------------------------------
// Device - Output
//--------------------------------------------------------


//--------------------------------------------------------
// Device - Pitch
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXXAudioDevicePitchIncrease(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, float amount);
PXPublic PXActionResult PXAPI PXXAudioDevicePitchSet(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
PXPublic PXActionResult PXAPI PXXAudioDevicePitchReduce(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, float amount);

//--------------------------------------------------------
// Device - Volume
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXXAudioDeviceVolumeGet(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
PXPublic PXActionResult PXAPI PXXAudioDeviceVolumeSetEqual(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
PXPublic PXActionResult PXAPI PXXAudioDeviceVolumeSetIndividual(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

//--------------------------------------------------------
// Device - Playback
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXXAudioDeviceStart(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXXAudioDeviceStop(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXXAudioDevicePause(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice);

#endif
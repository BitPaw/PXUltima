#ifndef PXXAudioInclude
#define PXXAudioInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

#define PXAudioSystemWindowsXAudioEnable 1u

#ifdef __cplusplus
extern "C"
{
#endif


#if PXAudioSystemWindowsXAudioEnable


	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXXAudioInitialize(PXAudio* const pxAudio);

	PXPublic PXActionResult PXAPI PXXAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	PXPublic PXActionResult PXAPI PXXAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXXAudioDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	PXPublic PXActionResult PXAPI PXXAudioDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	PXPublic PXActionResult PXAPI PXXAudioDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXXAudioDeviceLoad
	(
		PXAudio* const pxAudio,
		PXAudioDevice* const pxAudioDevice,
		PXSound* const pxSound,
		const PXAudioDeviceType pxAudioDeviceType,
		const PXInt32U deviceID
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
	PXPublic PXActionResult PXAPI PXXAudioDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	PXPublic PXActionResult PXAPI PXXAudioDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	PXPublic PXActionResult PXAPI PXXAudioDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

	//--------------------------------------------------------
	// Device - Volume
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXXAudioDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	PXPublic PXActionResult PXAPI PXXAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	PXPublic PXActionResult PXAPI PXXAudioDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

	//--------------------------------------------------------
	// Device - Playback
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXXAudioDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXXAudioDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXXAudioDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

#endif

#ifdef __cplusplus
}
#endif

#endif
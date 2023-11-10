#ifndef PXMultiMediaInclude
#define PXMultiMediaInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

#define PXAudioSystemWindowsMultimediaEnabled OSWindows && 1u

#ifdef __cplusplus
extern "C"
{
#endif
	// waveInAddBuffer
	// waveInGetPosition
	// waveInAddBuffer
	// waveOutGetErrorTextA
	// waveOutUnprepareHeader
	// waveOutWrite
	// waveOutGetID, waveInGetID
	// waveOutMessage, waveInMessage

#if PXAudioSystemWindowsMultimediaEnabled

	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXMultiMediaInitialize(PXAudio* const pxAudio);

	PXPublic PXActionResult PXAPI PXMultiMediaDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	PXPublic PXActionResult PXAPI PXMultiMediaDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceLoad
		(
			PXAudio* const pxAudio,
			PXAudioDevice* const pxAudioDevice,
			PXSound* const pxSound
			);

	//--------------------------------------------------------
	// Device - Input
	//--------------------------------------------------------


	//--------------------------------------------------------
	// Device - Output
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceBreakLoop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePlaybackRateGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const playbackRate);
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePlaybackRateSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U playbackRate);

	//--------------------------------------------------------
	// Device - Pitch
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

	//--------------------------------------------------------
	// Device - Volume
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

	//--------------------------------------------------------
	// Device - Playback
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceRestart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMultiMediaDeviceReset(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

#endif


#ifdef __cplusplus
}
#endif

#endif
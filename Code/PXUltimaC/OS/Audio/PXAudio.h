#ifndef PXAudioINCLUDE
#define PXAudioINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXAudioUSE OSDeviceToUse == OSDeviceDestop
#if PXAudioUSE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

#define PitchMaximum 2.9f
#define PitchMinimum 0.2f
#define SoundDeviceProductName 32u

#if OSUnix
typedef int PXAudioDeviceOutputHandle;
typedef int PXAudioDeviceInputHandle;
#elif OSWindows

#include <windows.h>
#include <Media/PXResource.h>

#if WindowsAtleastVista && PXOSWindowsDestop
#include <mmeapi.h> // MISSING
typedef HWAVEOUT PXAudioDeviceOutputHandle;
typedef HWAVEIN PXAudioDeviceInputHandle;
#else
typedef void* PXAudioDeviceOutputHandle;
typedef void* PXAudioDeviceInputHandle;
#endif

#endif

#define PXAudioSystemWindowsXAudioEnable 0u
#define PXAudioSystemWindowsMultimediaEnable 1u



#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXSoundPlayStyle_
	{
		SoundPlayStyleOnce,
		SoundPlayStyleLoop
	}
	PXSoundPlayStyle;

	typedef struct PXAudioClip_
	{
		unsigned int ID;
		PXSoundPlayStyle PlayStyle;
	}
	PXAudioClip;

	typedef struct PXAudioConfig_
	{
		float VolumeMusic;
		float VolumeSFX;
	}
	PXAudioConfig;

	





	typedef enum PXAudioSystem_
	{
		PXAudioSystemInvalid,
		PXAudioSystemWindowsMultiMedia,
		PXAudioSystemWindowsKernalMixer,
		PXAudioSystemWindowsXAudio
	}
	PXAudioSystem;


	typedef struct PXAudio_
	{
#if OSUnix

#elif OSWindows
		void* XAudioInterface;
		void* XAudioMasterVoice;
#endif

		PXInt32U DeviceOutputAmount;
		PXInt32U DeviceInputAmount;

		PXAudioDevice* DeviceOutputList;

		PXAudioSystem System;
	}
	PXAudio;

#if PXOSWindowsDestop
	PXPrivate PXActionResult PXAudioConvertMMResult(const unsigned int mmResultID);
#endif

	//--------------------------------------------------------
	// Utility
	//--------------------------------------------------------
	PXPublic void PXAudioConstruct(PXAudio* const pxAudio);
	PXPublic void PXAudioDestruct(PXAudio* const pxAudio);

	PXPublic PXActionResult PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem);

	//--------------------------------------------------------
	// Device
	//--------------------------------------------------------
	PXPublic PXActionResult PXAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	PXPublic PXActionResult PXAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);

	PXPublic PXActionResult PXAudioDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	PXPublic PXActionResult PXAudioDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAudioDeviceLoad
	(
		PXAudio* const pxAudio,
		PXAudioDevice* const pxAudioDevice, 
		PXSound* const pxSound,
		const PXAudioDeviceType pxAudioDeviceType,
		const PXInt32U deviceID
	);

	//--------------------------------------------------------
	// Device - Pitch
	//--------------------------------------------------------
	PXPublic PXActionResult PXAudioDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	PXPublic PXActionResult PXAudioDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	PXPublic PXActionResult PXAudioDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

	//--------------------------------------------------------
	// Device - Volume
	//--------------------------------------------------------
	PXPublic PXActionResult PXAudioDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	PXPublic PXActionResult PXAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	PXPublic PXActionResult PXAudioDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

	//--------------------------------------------------------
	// Device - Playback
	//--------------------------------------------------------
	PXPublic PXActionResult PXAudioDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAudioDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAudioDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAudioDevicePlaybackRateSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);

#ifdef __cplusplus
}
#endif

#endif
#endif
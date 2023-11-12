#ifndef PXAudioINCLUDE
#define PXAudioINCLUDE

#include <Media/PXResource.h>

#define PitchMaximum 2.9f
#define PitchMinimum 0.2f

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
		PXAudioSystemWindowsMultiMedia, // Windows 2000 
		PXAudioSystemWindowsMIDI,
		PXAudioSystemWindowsDirectSound, // Windows XP (successor to WINMM)
		PXAudioSystemWindowsIAudio, // Windows Vista (successor to DirectSound)
		PXAudioSystemWindowsXAudio // Windows 8 (successor to IAudio)
	}
	PXAudioSystem;


	typedef struct PXAudio_ PXAudio;

	typedef PXActionResult (PXAPI* PXAudioInitializeFunction)(PXAudio* const pxAudio);

	//--------------------------------------------------------
	// Device
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDeviceAmountFunction)(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	typedef PXActionResult(PXAPI* PXAudioDeviceFetchFunction)(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceFetchAllFunction)(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	typedef PXActionResult(PXAPI* PXAudioDeviceOpenFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	typedef PXActionResult(PXAPI* PXAudioDeviceCloseFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceLoadFunction)
		(
			PXAudio* const pxAudio,
			PXAudioDevice* const pxAudioDevice,
			PXSound* const pxSound
			);

	typedef PXActionResult(PXAPI* PXAudioDeviceUnloadFunction)
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
	typedef PXActionResult (PXAPI* PXAudioDevicePlayCursorSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset);
	typedef PXActionResult (PXAPI* PXAudioDevicePlayCursorGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


	typedef PXActionResult (PXAPI* PXAudioDevicePlaySpeedSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
	typedef PXActionResult (PXAPI* PXAudioDevicePlaySpeedGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);


	typedef PXActionResult (PXAPI* PXAudioDevicePositionSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
	typedef PXActionResult (PXAPI* PXAudioDevicePositionGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);

	typedef PXActionResult (PXAPI* PXAudioDeviceConeAnglesSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
	typedef PXActionResult (PXAPI* PXAudioDeviceConeAnglesGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);

	typedef PXActionResult (PXAPI* PXAudioDeviceConeOrientationGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);
	typedef PXActionResult (PXAPI* PXAudioDeviceConeOrientationSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

	typedef PXActionResult (PXAPI* PXAudioDeviceConeOutsideVolumeGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
	typedef PXActionResult (PXAPI* PXAudioDeviceConeOutsideVolumeSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMaxGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMaxSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMinGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMinSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

	typedef PXActionResult (PXAPI* PXAudioDeviceModeGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
	typedef PXActionResult (PXAPI* PXAudioDeviceModeSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

	typedef PXActionResult (PXAPI* PXAudioDeviceVelocityGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	typedef PXActionResult (PXAPI* PXAudioDeviceVelocitySetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);


	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceFactorGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceFactorSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);


	typedef PXActionResult (PXAPI* PXAudioDeviceDopplerFactorGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
	typedef PXActionResult (PXAPI* PXAudioDeviceDopplerFactorSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);

	typedef PXActionResult (PXAPI* PXAudioDeviceOrientationGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
	typedef PXActionResult (PXAPI* PXAudioDeviceOrientationSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

	typedef PXActionResult (PXAPI* PXAudioDeviceRolloffFactorGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
	typedef PXActionResult (PXAPI* PXAudioDeviceRolloffFactorSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

	typedef PXActionResult (PXAPI* PXAudioDeviceDeferredSettingsCommitFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);



	typedef PXActionResult(PXAPI* PXAudioDeviceBreakLoopFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDevicePlaybackRateGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const playbackRate);
	typedef PXActionResult(PXAPI* PXAudioDevicePlaybackRateSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U playbackRate);

	//--------------------------------------------------------
	// Device - Pitch
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDevicePitchIncreaseFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	typedef PXActionResult(PXAPI* PXAudioDevicePitchSetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	typedef PXActionResult(PXAPI* PXAudioDevicePitchReduceFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	typedef PXActionResult(PXAPI* PXAudioDevicePitchGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);

	//--------------------------------------------------------
	// Device - Volume
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDeviceVolumeGetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	typedef PXActionResult(PXAPI* PXAudioDeviceVolumeSetEqualFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	typedef PXActionResult(PXAPI* PXAudioDeviceVolumeSetIndividualFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

	//--------------------------------------------------------
	// Device - Playback
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDeviceStartFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceStopFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDevicePauseFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceRestartFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceResetFunction)(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

	typedef struct PXAudio_
	{
#if OSUnix

#elif OSWindows
		void* DirectSoundInterface;

		void* XAudioInterface;
		void* XAudioMasterVoice;
#endif

		PXInt32U DeviceOutputAmount;
		PXInt32U DeviceInputAmount;

		PXAudioDevice* DeviceOutputList;

		PXAudioSystem System;

		PXAudioInitializeFunction Initialize;
		PXAudioDeviceAmountFunction DeviceAmount;
		PXAudioDeviceFetchFunction DeviceFetch;
		PXAudioDeviceFetchAllFunction DeviceFetchAll;
		PXAudioDeviceOpenFunction DeviceOpen;
		PXAudioDeviceCloseFunction DeviceClose;
		PXAudioDeviceLoadFunction DeviceLoad;
		PXAudioDeviceBreakLoopFunction DeviceBreakLoop;
		PXAudioDevicePositionGetFunction DevicePositionGet;
		PXAudioDevicePlaybackRateGetFunction DevicePlaybackRateGet;
		PXAudioDevicePlaybackRateSetFunction DevicePlaybackRateSet;
		PXAudioDevicePitchIncreaseFunction DevicePitchIncrease;
		PXAudioDevicePitchSetFunction DevicePitchSet;
		PXAudioDevicePitchReduceFunction DevicePitchReduce;
		PXAudioDeviceVolumeGetFunction DeviceVolumeGet;
		PXAudioDeviceVolumeSetEqualFunction DeviceVolumeSetEqual;
		PXAudioDeviceVolumeSetIndividualFunction DeviceVolumeSetIndividual;
		PXAudioDeviceStartFunction DeviceStart;
		PXAudioDeviceStopFunction DeviceStop;
		PXAudioDevicePauseFunction DevicePause;
		PXAudioDeviceRestartFunction DeviceRestart;
		PXAudioDeviceResetFunction DeviceReset;
		PXAudioDevicePlayCursorSetFunction	PlayCursorSet;
		PXAudioDevicePlayCursorGetFunction	PlayCursorGet;
		PXAudioDevicePlaySpeedSetFunction	PlaySpeedSet;
		PXAudioDevicePlaySpeedGetFunction	PlaySpeedGet;
		PXAudioDevicePositionSetFunction	PositionSet;
		PXAudioDevicePositionGetFunction	PositionGet;
		PXAudioDeviceConeAnglesSetFunction	ConeAnglesSet;
		PXAudioDeviceConeAnglesGetFunction	ConeAnglesGet;
		PXAudioDeviceConeOrientationGetFunction	ConeOrientationGet;
		PXAudioDeviceConeOrientationSetFunction	ConeOrientationSet;
		PXAudioDeviceConeOutsideVolumeGetFunction	ConeOutsideVolumeGet;
		PXAudioDeviceConeOutsideVolumeSetFunction	ConeOutsideVolumeSet;
		PXAudioDeviceDistanceMaxGetFunction	DistanceMaxGet;
		PXAudioDeviceDistanceMaxSetFunction	DistanceMaxSet;
		PXAudioDeviceDistanceMinGetFunction	DistanceMinGet;
		PXAudioDeviceDistanceMinSetFunction	DistanceMinSet;
		PXAudioDeviceModeGetFunction	ModeGet;
		PXAudioDeviceModeSetFunction	ModeSet;
		PXAudioDeviceVelocityGetFunction	VelocityGet;
		PXAudioDeviceVelocitySetFunction	VelocitySet;
		PXAudioDeviceDistanceFactorGetFunction	DistanceFactorGet;
		PXAudioDeviceDistanceFactorSetFunction	DistanceFactorSet;
		PXAudioDeviceDopplerFactorGetFunction	DopplerFactorGet;
		PXAudioDeviceDopplerFactorSetFunction	DopplerFactorSet;
		PXAudioDeviceOrientationGetFunction	OrientationGet;
		PXAudioDeviceOrientationSetFunction	OrientationSet;
		PXAudioDeviceRolloffFactorGetFunction	RolloffFactorGet;
		PXAudioDeviceRolloffFactorSetFunction	RolloffFactorSet;
		PXAudioDeviceDeferredSettingsCommitFunction	DeferredSettingsCommit;
	}
	PXAudio;

	PXPublic PXActionResult PXAPI PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem);

#ifdef __cplusplus
}
#endif

#endif
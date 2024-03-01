#ifndef PXAudioINCLUDE
#define PXAudioINCLUDE

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

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

	typedef struct PXAudioInitializeInfo_
	{
		struct PXAudio_* AudioReference;
		PXAudioSystem AudioSystem;
	}
	PXAudioInitializeInfo;



	typedef PXActionResult (PXAPI* PXAudioInitializeFunction)(void* const audioAPI, PXAudioInitializeInfo* const pxAudioInitializeInfo);

	//--------------------------------------------------------
	// Device
	//--------------------------------------------------------
	typedef struct PXAudioDeviceAmountInfo_
	{
		PXInt32U DeviceInput;
		PXInt32U DeviceOutput;
	}
	PXAudioDeviceAmountInfo;

	typedef PXActionResult(PXAPI* PXAudioDeviceAmountFunction)(void* const audioAPI, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo);
	
	typedef PXActionResult(PXAPI* PXAudioDeviceFetchFunction)(void* const audioAPI, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceFetchAllFunction)(void* const audioAPI, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	typedef PXActionResult(PXAPI* PXAudioDeviceOpenFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	typedef PXActionResult(PXAPI* PXAudioDeviceCloseFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceLoadFunction)
		(
			void* const audioAPI,
			PXAudioDevice* const pxAudioDevice,
			PXSound* const pxSound
			);

	typedef PXActionResult(PXAPI* PXAudioDeviceUnloadFunction)
		(
			void* const audioAPI,
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
	typedef PXActionResult (PXAPI* PXAudioDevicePlayCursorSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXSize offset);
	typedef PXActionResult (PXAPI* PXAudioDevicePlayCursorGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXSize* const offset);

	typedef PXActionResult (PXAPI* PXAudioDevicePlaySpeedSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
	typedef PXActionResult (PXAPI* PXAudioDevicePlaySpeedGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);

	typedef PXActionResult (PXAPI* PXAudioDevicePositionSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
	typedef PXActionResult (PXAPI* PXAudioDevicePositionGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);

	typedef PXActionResult (PXAPI* PXAudioDeviceConeAnglesSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
	typedef PXActionResult (PXAPI* PXAudioDeviceConeAnglesGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);

	typedef PXActionResult (PXAPI* PXAudioDeviceConeOrientationGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);
	typedef PXActionResult (PXAPI* PXAudioDeviceConeOrientationSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

	typedef PXActionResult (PXAPI* PXAudioDeviceConeOutsideVolumeGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
	typedef PXActionResult (PXAPI* PXAudioDeviceConeOutsideVolumeSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMaxGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMaxSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMinGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceMinSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

	typedef PXActionResult (PXAPI* PXAudioDeviceModeGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
	typedef PXActionResult (PXAPI* PXAudioDeviceModeSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

	typedef PXActionResult (PXAPI* PXAudioDeviceVelocityGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	typedef PXActionResult (PXAPI* PXAudioDeviceVelocitySetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceFactorGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
	typedef PXActionResult (PXAPI* PXAudioDeviceDistanceFactorSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);

	typedef PXActionResult (PXAPI* PXAudioDeviceDopplerFactorGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
	typedef PXActionResult (PXAPI* PXAudioDeviceDopplerFactorSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);

	typedef PXActionResult (PXAPI* PXAudioDeviceOrientationGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
	typedef PXActionResult (PXAPI* PXAudioDeviceOrientationSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

	typedef PXActionResult (PXAPI* PXAudioDeviceRolloffFactorGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
	typedef PXActionResult (PXAPI* PXAudioDeviceRolloffFactorSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

	typedef PXActionResult (PXAPI* PXAudioDeviceDeferredSettingsCommitFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);



	typedef PXActionResult(PXAPI* PXAudioDeviceBreakLoopFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDevicePlaybackRateGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXInt32U* const playbackRate);
	typedef PXActionResult(PXAPI* PXAudioDevicePlaybackRateSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXInt32U playbackRate);

	//--------------------------------------------------------
	// Device - Pitch
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDevicePitchIncreaseFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float amount);
	typedef PXActionResult(PXAPI* PXAudioDevicePitchSetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	typedef PXActionResult(PXAPI* PXAudioDevicePitchReduceFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, float amount);
	typedef PXActionResult(PXAPI* PXAudioDevicePitchGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);

	//--------------------------------------------------------
	// Device - Volume
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDeviceVolumeGetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	typedef PXActionResult(PXAPI* PXAudioDeviceVolumeSetEqualFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	typedef PXActionResult(PXAPI* PXAudioDeviceVolumeSetIndividualFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

	//--------------------------------------------------------
	// Device - Playback
	//--------------------------------------------------------
	typedef PXActionResult(PXAPI* PXAudioDeviceStartFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceStopFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDevicePauseFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceRestartFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
	typedef PXActionResult(PXAPI* PXAudioDeviceResetFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);









	typedef struct PXAudioMultiMedia_
	{
		PXLibrary MultiMediaLibrary;

		void* WaveOutGetDevCapsA;
		void* WaveOutGetDevCapsW;
		void* WaveOutGetNumDevs;
		void* WaveOutGetVolume;
		void* WaveOutSetVolume;
		void* WaveOutGetErrorTextA;
		void* WaveOutGetErrorTextW;
		void* WaveOutOpen;
		void* WaveOutClose;
		void* WaveOutPrepareHeader;
		void* WaveOutUnprepareHeader;
		void* WaveOutWrite;
		void* WaveOutPause;
		void* WaveOutRestart;
		void* WaveOutReset;
		void* WaveOutBreakLoop;
		void* WaveOutGetPosition;
		void* WaveOutGetPitch;
		void* WaveOutSetPitch;
		void* WaveOutGetPlaybackRate;
		void* WaveOutSetPlaybackRate;
		void* WaveOutGetID;
		void* WaveOutMessage;
		void* WaveInGetNumDevs;
		void* WaveInGetDevCapsA;
		void* WaveInGetDevCapsW;
		void* WaveInGetErrorTextA;
		void* WaveInGetErrorTextW;
		void* WaveInOpen;
		void* WaveInClose;
		void* WaveInPrepareHeader;
		void* WaveInUnprepareHeader;
		void* WaveInAddBuffer;
		void* WaveInStart;
		void* WaveInStop;
		void* WaveInReset;
		void* WaveInGetPosition;
		void* WaveInGetID;
		void* WaveInMessage;
		void* MidiOutGetNumDevs;
		void* MidiStreamOpen;
		void* MidiStreamClose;
		void* MidiStreamProperty;
		void* MidiStreamPosition;
		void* MidiStreamOut;
		void* MidiStreamPause;
		void* MidiStreamRestart;
		void* MidiStreamStop;
		void* MidiConnect;
		void* MidiDisconnect;
		void* MidiOutGetDevCapsA;
		void* MidiOutGetDevCapsW;
		void* MidiOutGetVolume;
		void* MidiOutSetVolume;
		void* MidiOutGetErrorTextA;
		void* MidiOutGetErrorTextW;
		void* MidiOutOpen;
		void* MidiOutClose;
		void* MidiOutPrepareHeader;
		void* MidiOutUnprepareHeader;
		void* MidiOutShortMsg;
		void* MidiOutLongMsg;
		void* MidiOutReset;
		void* MidiOutCachePatches;
		void* MidiOutCacheDrumPatches;
		void* MidiOutGetID;
		void* MidiOutMessage;
		void* MidiInGetNumDevs;
		void* MidiInGetDevCapsA;
		void* MidiInGetDevCapsW;
		void* MidiInGetErrorTextA;
		void* MidiInGetErrorTextW;
		void* MidiInOpen;
		void* MidiInClose;
		void* MidiInPrepareHeader;
		void* MidiInUnprepareHeader;
		void* MidiInAddBuffer;
		void* MidiInStart;
		void* MidiInStop;
		void* MidiInReset;
		void* MidiInGetID;
		void* MidiInMessage;
		void* AuxGetNumDevs;
		void* AuxGetDevCapsA;
		void* AuxGetDevCapsW;
		void* AuxSetVolume;
		void* AuxGetVolume;
		void* AuxOutMessage;
		void* MixerGetNumDevs;
		void* MixerGetDevCapsA;
		void* MixerGetDevCapsW;
		void* MixerOpen;
		void* MixerClose;
		void* MixerMessage;
		void* MixerGetLineInfoA;
		void* MixerGetLineInfoW;
		void* MixerGetID;
		void* MixerGetLineControlsA;
		void* MixerGetLineControlsW;
		void* MixerGetControlDetailsA;
		void* MixerGetControlDetailsW;
	}
	PXAudioMultiMedia;

	// Direct Sound
	typedef struct PXAudioDirectSound_
	{
		PXLibrary DirectSoundLibrary;

		void* DirectSoundInterface;

		void* SoundCreate;
		void* SoundEnumerateA;
		void* SoundEnumerateW;
		void* SoundCaptureCreate;
		void* SoundCaptureEnumerateA;
		void* SoundCaptureEnumerateW;
		void* ClassObjectGet;
		void* CanUnloadNow;

		void* SoundCreate8;
		void* SoundCaptureCreate8;
	}
	PXAudioDirectSound;

	typedef struct PXAudioXSystem_
	{
		PXLibrary XSystemLibrary;

		struct IXAudio2* XAudioInterface;
		void* XAudioMasterVoice;
	}
	PXAudioXSystem;

	typedef struct PXAudio_
	{
		union
		{
			PXAudioDirectSound DirectSound;
			PXAudioMultiMedia MultiMedia;
			PXAudioXSystem XSystem;
		};

		void* SystemReference;
		PXAudioSystem System;


		PXInt32U DeviceOutputAmount;
		PXInt32U DeviceInputAmount;

		PXAudioDevice* DeviceOutputList;
	

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
		PXAudioDevicePlayCursorSetFunction PlayCursorSet;
		PXAudioDevicePlayCursorGetFunction PlayCursorGet;
		PXAudioDevicePlaySpeedSetFunction PlaySpeedSet;
		PXAudioDevicePlaySpeedGetFunction PlaySpeedGet;
		PXAudioDevicePositionSetFunction PositionSet;
		PXAudioDevicePositionGetFunction PositionGet;
		PXAudioDeviceConeAnglesSetFunction ConeAnglesSet;
		PXAudioDeviceConeAnglesGetFunction ConeAnglesGet;
		PXAudioDeviceConeOrientationGetFunction	ConeOrientationGet;
		PXAudioDeviceConeOrientationSetFunction	ConeOrientationSet;
		PXAudioDeviceConeOutsideVolumeGetFunction ConeOutsideVolumeGet;
		PXAudioDeviceConeOutsideVolumeSetFunction ConeOutsideVolumeSet;
		PXAudioDeviceDistanceMaxGetFunction	DistanceMaxGet;
		PXAudioDeviceDistanceMaxSetFunction	DistanceMaxSet;
		PXAudioDeviceDistanceMinGetFunction	DistanceMinGet;
		PXAudioDeviceDistanceMinSetFunction	DistanceMinSet;
		PXAudioDeviceModeGetFunction ModeGet;
		PXAudioDeviceModeSetFunction ModeSet;
		PXAudioDeviceVelocityGetFunction VelocityGet;
		PXAudioDeviceVelocitySetFunction VelocitySet;
		PXAudioDeviceDistanceFactorGetFunction DistanceFactorGet;
		PXAudioDeviceDistanceFactorSetFunction DistanceFactorSet;
		PXAudioDeviceDopplerFactorGetFunction DopplerFactorGet;
		PXAudioDeviceDopplerFactorSetFunction DopplerFactorSet;
		PXAudioDeviceOrientationGetFunction	OrientationGet;
		PXAudioDeviceOrientationSetFunction	OrientationSet;
		PXAudioDeviceRolloffFactorGetFunction RolloffFactorGet;
		PXAudioDeviceRolloffFactorSetFunction RolloffFactorSet;
		PXAudioDeviceDeferredSettingsCommitFunction	DeferredSettingsCommit;
	}
	PXAudio;

	PXPublic PXActionResult PXAPI PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem);

#ifdef __cplusplus
}
#endif

#endif
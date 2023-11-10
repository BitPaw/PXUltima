#ifndef PXDirectSoundInclude
#define PXDirectSoundInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

#define PXAudioSystemWindowsDirectSoundEnable OSWindows && 1u

#ifdef __cplusplus
extern "C"
{
#endif

#if PXAudioSystemWindowsDirectSoundEnable
	typedef struct PXAudioDeviceDetectObjectEventData_
	{
		PXInt32U IndexCurrent;
		PXInt32U IndexTarget;
		PXAudioDevice* AudioDevice;
	}
	PXAudioDeviceDetectObjectEventData;

	PXPrivate BOOL CALLBACK PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXInt32U* const amount);
	PXPrivate BOOL CALLBACK PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDeviceDetectObjectEventData* const pxAudioDeviceDetectObjectEventData);



	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXDirectSoundInitialize(PXAudio* const pxAudio);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceLoad
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


	PXPublic PXActionResult PXAPI PXDirectSoundDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset);
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


	PXPublic PXActionResult PXAPI PXDirectSoundDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);


	PXPublic PXActionResult PXAPI PXDirectSoundDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);


	//PXPublic PXActionResult PXAPI PXDirectSoundDeviceParametersAllGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, __out LPDS3DBUFFER pDs3dBuffer);
	//PXPublic PXActionResult PXAPI PXDirectSoundDeviceParametersAllSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, __in LPCDS3DBUFFER pcDs3dBuffer, DWORD dwApply);
	
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);


	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);

	
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);
	
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

	PXPublic PXActionResult PXAPI PXDirectSoundDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);


	//--------------------------------------------------------
	// Device - Pitch
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

	//--------------------------------------------------------
	// Device - Volume
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

	//--------------------------------------------------------
	// Device - Playback
	//--------------------------------------------------------
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXDirectSoundDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXDirectSoundDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

#endif

#ifdef __cplusplus
}
#endif

#endif
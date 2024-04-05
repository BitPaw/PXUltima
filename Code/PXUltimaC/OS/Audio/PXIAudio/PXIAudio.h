#ifndef PXIAudioINCLUDE
#define PXIAudioINCLUDE

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXIAudioInitialize(PXAudio* const pxAudio);

PXPublic PXActionResult PXAPI PXIAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
PXPublic PXActionResult PXAPI PXIAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXIAudioDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXActionResult PXAPI PXIAudioDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
PXPublic PXActionResult PXAPI PXIAudioDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXIAudioDeviceLoad
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
PXPublic PXActionResult PXAPI PXIAudioDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset);
PXPublic PXActionResult PXAPI PXIAudioDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


PXPublic PXActionResult PXAPI PXIAudioDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
PXPublic PXActionResult PXAPI PXIAudioDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);


PXPublic PXActionResult PXAPI PXIAudioDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
PXPublic PXActionResult PXAPI PXIAudioDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);

PXPublic PXActionResult PXAPI PXIAudioDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
PXPublic PXActionResult PXAPI PXIAudioDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);


//PXPublic PXActionResult PXAPI PXIAudioDeviceParametersAllGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, __out LPDS3DBUFFER pDs3dBuffer);
//PXPublic PXActionResult PXAPI PXIAudioDeviceParametersAllSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, __in LPCDS3DBUFFER pcDs3dBuffer, DWORD dwApply);

PXPublic PXActionResult PXAPI PXIAudioDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);
PXPublic PXActionResult PXAPI PXIAudioDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

PXPublic PXActionResult PXAPI PXIAudioDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
PXPublic PXActionResult PXAPI PXIAudioDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


PXPublic PXActionResult PXAPI PXIAudioDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
PXPublic PXActionResult PXAPI PXIAudioDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
PXPublic PXActionResult PXAPI PXIAudioDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
PXPublic PXActionResult PXAPI PXIAudioDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

PXPublic PXActionResult PXAPI PXIAudioDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
PXPublic PXActionResult PXAPI PXIAudioDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

PXPublic PXActionResult PXAPI PXIAudioDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
PXPublic PXActionResult PXAPI PXIAudioDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);


PXPublic PXActionResult PXAPI PXIAudioDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
PXPublic PXActionResult PXAPI PXIAudioDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);

//  DS3D_MINDOPPLERFACTOR to DS3D_MAXDOPPLERFACTOR
PXPublic PXActionResult PXAPI PXIAudioDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
PXPublic PXActionResult PXAPI PXIAudioDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);

PXPublic PXActionResult PXAPI PXIAudioDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
PXPublic PXActionResult PXAPI PXIAudioDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

PXPublic PXActionResult PXAPI PXIAudioDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
PXPublic PXActionResult PXAPI PXIAudioDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

PXPublic PXActionResult PXAPI PXIAudioDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);


//--------------------------------------------------------
// Device - Pitch
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXIAudioDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
PXPublic PXActionResult PXAPI PXIAudioDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
PXPublic PXActionResult PXAPI PXIAudioDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

//--------------------------------------------------------
// Device - Volume
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXIAudioDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
PXPublic PXActionResult PXAPI PXIAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
PXPublic PXActionResult PXAPI PXIAudioDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

//--------------------------------------------------------
// Device - Playback
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXIAudioDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXIAudioDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXIAudioDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

#endif
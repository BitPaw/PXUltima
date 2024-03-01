#define PXAudioMIDIInclude
#ifndef PXAudioMIDIInclude
#define PXAudioMIDIInclude

#include <Media/PXResource.h>
#include <OS/Audio/PXAudio.h>

#include <Windows.h>
#include <mmsystem.h>

#ifdef __cplusplus
extern "C"
{
#endif



#if OSWindows
	PXPrivate void CALLBACK PXMIDIInputMessageEvent(void* hMidiIn, PXInt32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2);
	PXPrivate void CALLBACK PXMIDIOutputMessageEvent(void* hMidiIn, PXInt32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2);


	PXPublic PXActionResult PXAPI PXMIDIInitialize(PXAudio* const pxAudio);
	PXPublic PXActionResult PXAPI PXMIDIRelease(PXAudio* const pxAudio);

	PXPublic PXActionResult PXAPI PXMIDIDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount);
	PXPublic PXActionResult PXAPI PXMIDIDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

	PXPublic PXActionResult PXAPI PXMIDIDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
	PXPublic PXActionResult PXAPI PXMIDIDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDeviceLoad
	(
		PXAudio* const pxAudio,
		PXAudioDevice* const pxAudioDevice,
		PXSound* const pxSound
	);

	PXPublic PXActionResult PXAPI PXMIDIDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset);
	PXPublic PXActionResult PXAPI PXMIDIDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


	PXPublic PXActionResult PXAPI PXMIDIDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
	PXPublic PXActionResult PXAPI PXMIDIDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);


	PXPublic PXActionResult PXAPI PXMIDIDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
	PXPublic PXActionResult PXAPI PXMIDIDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);

	PXPublic PXActionResult PXAPI PXMIDIDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
	PXPublic PXActionResult PXAPI PXMIDIDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);

	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
	PXPublic PXActionResult PXAPI PXMIDIDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

	PXPublic PXActionResult PXAPI PXMIDIDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
	PXPublic PXActionResult PXAPI PXMIDIDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

	PXPublic PXActionResult PXAPI PXMIDIDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
	PXPublic PXActionResult PXAPI PXMIDIDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);


	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);


	PXPublic PXActionResult PXAPI PXMIDIDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
	PXPublic PXActionResult PXAPI PXMIDIDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);

	PXPublic PXActionResult PXAPI PXMIDIDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
	PXPublic PXActionResult PXAPI PXMIDIDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

	PXPublic PXActionResult PXAPI PXMIDIDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
	PXPublic PXActionResult PXAPI PXMIDIDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

	PXPublic PXActionResult PXAPI PXMIDIDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);


	PXPublic PXActionResult PXAPI PXMIDIDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);
	PXPublic PXActionResult PXAPI PXMIDIDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
	PXPublic PXActionResult PXAPI PXMIDIDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount);

	PXPublic PXActionResult PXAPI PXMIDIDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
	PXPublic PXActionResult PXAPI PXMIDIDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
	PXPublic PXActionResult PXAPI PXMIDIDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);


	PXPublic PXActionResult PXAPI PXMIDIDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
	PXPublic PXActionResult PXAPI PXMIDIDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

#endif

#ifdef __cplusplus
}
#endif

#endif

/*
midiOutGetNumDevs
midiStreamOpen
midiStreamClose
midiStreamProperty
midiStreamPosition
midiStreamOut
midiStreamPause
midiStreamRestart
midiStreamStop
midiConnect
midiDisconnect


midiOutGetDevCapsA
midiOutGetDevCapsW
midiOutGetVolume
midiOutSetVolume
midiOutGetErrorTextA
midiOutOpen
midiOutClose
midiOutPrepareHeader
midiOutUnprepareHeader
midiOutShortMsg
midiOutLongMsg
midiOutReset
midiOutCachePatches
midiOutCacheDrumPatches
midiOutGetID
midiOutMessage



midiInGetNumDevs
midiInGetDevCapsW
midiInGetErrorTextA

midiInOpen
midiInClose
midiInPrepareHeader
midiInUnprepareHeader
midiInAddBuffer
midiInStart
midiInStop
midiInReset
midiInGetID
midiInMessage




auxGetNumDevs
auxGetDevCapsW
auxSetVolume
auxGetVolume


mixerGetNumDevs
mixerGetDevCapsW
mixerOpen
mixerClose
mixerMessage
mixerGetLineInfoW
mixerGetID
mixerGetLineControlsW
mixerGetControlDetailsW
mixerSetControlDetails
*/
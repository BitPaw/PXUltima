#define PXAudioMIDIIncluded
#ifndef PXAudioMIDIIncluded
#define PXAudioMIDIIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Audio/PXAudio.h>

#include <Windows.h>
#include <mmsystem.h>

#ifdef __cplusplus
extern "C"
{
#endif



#if OSWindows
PXPrivate void CALLBACK PXMIDIInputMessageEvent(void* hMidiIn, PXI32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2);
PXPrivate void CALLBACK PXMIDIOutputMessageEvent(void* hMidiIn, PXI32U wMsg, void* dwInstance, void* dwParam1, void* dwParam2);


PXPublic PXResult PXAPI PXMIDIInitialize(PXAudio* const pxAudio);
PXPublic PXResult PXAPI PXMIDIRelease(PXAudio* const pxAudio);

PXPublic PXResult PXAPI PXMIDIDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXI32U* const amount);
PXPublic PXResult PXAPI PXMIDIDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXMIDIDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXMIDIDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDeviceLoad
(
    PXAudio* const pxAudio,
    PXAudioDevice* const pxAudioDevice,
    PXSound* const pxSound
);

PXPublic PXResult PXAPI PXMIDIDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset);
PXPublic PXResult PXAPI PXMIDIDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


PXPublic PXResult PXAPI PXMIDIDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U frequency);
PXPublic PXResult PXAPI PXMIDIDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const frequency);


PXPublic PXResult PXAPI PXMIDIDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic PXResult PXAPI PXMIDIDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z);

PXPublic PXResult PXAPI PXMIDIDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U dwInsideConeAngle, const PXI32U dwOutsideConeAngle);
PXPublic PXResult PXAPI PXMIDIDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const pdwInsideConeAngle, PXI32U* const pdwOutsideConeAngle);

PXPublic PXResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z);
PXPublic PXResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic PXResult PXAPI PXMIDIDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const plConeOutsideVolume);
PXPublic PXResult PXAPI PXMIDIDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U lConeOutsideVolume);


PXPublic PXResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const pflMaxDistance);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 flMaxDistance);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const pflMinDistance);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 flMinDistance);

PXPublic PXResult PXAPI PXMIDIDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const pdwMode);
PXPublic PXResult PXAPI PXMIDIDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U dwMode);

PXPublic PXResult PXAPI PXMIDIDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z);
PXPublic PXResult PXAPI PXMIDIDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z);


PXPublic PXResult PXAPI PXMIDIDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const pflDistanceFactor);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U flDistanceFactor);


PXPublic PXResult PXAPI PXMIDIDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const pflDopplerFactor);
PXPublic PXResult PXAPI PXMIDIDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U flDopplerFactor);

PXPublic PXResult PXAPI PXMIDIDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const pvOrientFront, PXI32U* const pvOrientTop);
PXPublic PXResult PXAPI PXMIDIDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U xFront, const PXI32U yFront, const PXI32U zFront, const PXI32U xTop, const PXI32U yTop, const PXI32U zTop);

PXPublic PXResult PXAPI PXMIDIDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXI32U* const pflRolloffFactor);
PXPublic PXResult PXAPI PXMIDIDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXI32U flRolloffFactor);

PXPublic PXResult PXAPI PXMIDIDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);


PXPublic PXResult PXAPI PXMIDIDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32 amount);
PXPublic PXResult PXAPI PXMIDIDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
PXPublic PXResult PXAPI PXMIDIDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXF32 amount);

PXPublic PXResult PXAPI PXMIDIDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
PXPublic PXResult PXAPI PXMIDIDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
PXPublic PXResult PXAPI PXMIDIDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);


PXPublic PXResult PXAPI PXMIDIDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice);

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
midiOutGetErrorA
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
midiInGetErrorA

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

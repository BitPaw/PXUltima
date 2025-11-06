#pragma once

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


PXPublic PXResult PXAPI PXMIDIInitialize(PXAudio PXREF pxAudio);
PXPublic PXResult PXAPI PXMIDIRelease(PXAudio PXREF pxAudio);

PXPublic PXResult PXAPI PXMIDIDeviceAmount(PXAudio PXREF pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXI32U PXREF amount);
PXPublic PXResult PXAPI PXMIDIDeviceFetch(PXAudio PXREF pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDeviceFetchAll(PXAudio PXREF pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXMIDIDeviceOpen(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXMIDIDeviceClose(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDeviceLoad
(
    PXAudio PXREF pxAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXSound PXREF pxSound
);

PXPublic PXResult PXAPI PXMIDIDevicePlayCursorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXSize offset);
PXPublic PXResult PXAPI PXMIDIDevicePlayCursorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXSize PXREF offset);


PXPublic PXResult PXAPI PXMIDIDevicePlaySpeedSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U frequency);
PXPublic PXResult PXAPI PXMIDIDevicePlaySpeedGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF frequency);


PXPublic PXResult PXAPI PXMIDIDevicePositionSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z);
PXPublic PXResult PXAPI PXMIDIDevicePositionGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z);

PXPublic PXResult PXAPI PXMIDIDeviceConeAnglesSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U dwInsideConeAngle, const PXI32U dwOutsideConeAngle);
PXPublic PXResult PXAPI PXMIDIDeviceConeAnglesGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pdwInsideConeAngle, PXI32U PXREF pdwOutsideConeAngle);

PXPublic PXResult PXAPI PXMIDIDeviceConeOrientationGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z);
PXPublic PXResult PXAPI PXMIDIDeviceConeOrientationSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z);

PXPublic PXResult PXAPI PXMIDIDeviceConeOutsideVolumeGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF plConeOutsideVolume);
PXPublic PXResult PXAPI PXMIDIDeviceConeOutsideVolumeSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U lConeOutsideVolume);


PXPublic PXResult PXAPI PXMIDIDeviceDistanceMaxGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF pflMaxDistance);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceMaxSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 flMaxDistance);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceMinGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF pflMinDistance);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceMinSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 flMinDistance);

PXPublic PXResult PXAPI PXMIDIDeviceModeGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pdwMode);
PXPublic PXResult PXAPI PXMIDIDeviceModeSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U dwMode);

PXPublic PXResult PXAPI PXMIDIDeviceVelocityGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 PXREF x, PXF32 PXREF y, PXF32 PXREF z);
PXPublic PXResult PXAPI PXMIDIDeviceVelocitySet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z);


PXPublic PXResult PXAPI PXMIDIDeviceDistanceFactorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pflDistanceFactor);
PXPublic PXResult PXAPI PXMIDIDeviceDistanceFactorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U flDistanceFactor);


PXPublic PXResult PXAPI PXMIDIDeviceDopplerFactorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pflDopplerFactor);
PXPublic PXResult PXAPI PXMIDIDeviceDopplerFactorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U flDopplerFactor);

PXPublic PXResult PXAPI PXMIDIDeviceOrientationGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pvOrientFront, PXI32U PXREF pvOrientTop);
PXPublic PXResult PXAPI PXMIDIDeviceOrientationSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U xFront, const PXI32U yFront, const PXI32U zFront, const PXI32U xTop, const PXI32U yTop, const PXI32U zTop);

PXPublic PXResult PXAPI PXMIDIDeviceRolloffFactorGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXI32U PXREF pflRolloffFactor);
PXPublic PXResult PXAPI PXMIDIDeviceRolloffFactorSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const PXI32U flRolloffFactor);

PXPublic PXResult PXAPI PXMIDIDeviceDeferredSettingsCommit(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice);


PXPublic PXResult PXAPI PXMIDIDevicePitchIncrease(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 amount);
PXPublic PXResult PXAPI PXMIDIDevicePitchSet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const unsigned int pitch);
PXPublic PXResult PXAPI PXMIDIDevicePitchReduce(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, PXF32 amount);

PXPublic PXResult PXAPI PXMIDIDeviceVolumeGet(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, unsigned short PXREF volume);
PXPublic PXResult PXAPI PXMIDIDeviceVolumeSetEqual(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const unsigned int volume);
PXPublic PXResult PXAPI PXMIDIDeviceVolumeSetIndividual(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);


PXPublic PXResult PXAPI PXMIDIDeviceStart(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDeviceStop(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXMIDIDevicePause(PXAudio PXREF pxAudio, PXAudioDevice PXREF pxAudioDevice);

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

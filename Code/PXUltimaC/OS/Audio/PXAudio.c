#include "PXAudio.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#include "PXDirectSound/PXDirectSound.h"
#include "PXMultiMedia/PXMultiMedia.h"
#include "PXIAudio/PXIAudio.h"
#include "PXXAudio/PXXAudio.h"
#include "PXAudioMIDI/PXAudioMIDI.h"

PXActionResult PXAPI PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem)
{
	PXClear(PXAudio, pxAudio);

	pxAudio->System = pxAudioSystem;

	switch (pxAudioSystem)
	{
#if OSWindows
		case PXAudioSystemWindowsMIDI:
		{
			pxAudio->Initialize = PXMIDIInitialize;
			pxAudio->DeviceAmount = PXMIDIDeviceAmount;
			pxAudio->DeviceFetch = PXMIDIDeviceFetch;
			pxAudio->DeviceFetchAll = PXMIDIDeviceFetchAll;
			pxAudio->DeviceOpen = PXMIDIDeviceOpen;
			pxAudio->DeviceClose = PXMIDIDeviceClose;
			pxAudio->DeviceLoad = PXMIDIDeviceLoad;
			pxAudio->DevicePitchIncrease = PXMIDIDevicePitchIncrease;
			pxAudio->DevicePitchSet = PXMIDIDevicePitchSet;
			pxAudio->DevicePitchReduce = PXMIDIDevicePitchReduce;
			pxAudio->DeviceVolumeGet = PXMIDIDeviceVolumeGet;
			pxAudio->DeviceVolumeSetEqual = PXMIDIDeviceVolumeSetEqual;
			pxAudio->DeviceVolumeSetIndividual = PXMIDIDeviceVolumeSetIndividual;
			pxAudio->DeviceStart = PXMIDIDeviceStart;
			pxAudio->DeviceStop = PXMIDIDeviceStop;
			pxAudio->DevicePause = PXMIDIDevicePause;
			pxAudio->PlayCursorSet = PXMIDIDevicePlayCursorSet;
			pxAudio->PlayCursorGet = PXMIDIDevicePlayCursorGet;
			pxAudio->PlaySpeedSet = PXMIDIDevicePlaySpeedSet;
			pxAudio->PlaySpeedGet = PXMIDIDevicePlaySpeedGet;
			pxAudio->PositionSet = PXMIDIDevicePositionSet;
			pxAudio->PositionGet = PXMIDIDevicePositionGet;
			pxAudio->ConeAnglesSet = PXMIDIDeviceConeAnglesSet;
			pxAudio->ConeAnglesGet = PXMIDIDeviceConeAnglesGet;
			pxAudio->ConeOrientationGet = PXMIDIDeviceConeOrientationGet;
			pxAudio->ConeOrientationSet = PXMIDIDeviceConeOrientationSet;
			pxAudio->ConeOutsideVolumeGet = PXMIDIDeviceConeOutsideVolumeGet;
			pxAudio->ConeOutsideVolumeSet = PXMIDIDeviceConeOutsideVolumeSet;
			pxAudio->DistanceMaxGet = PXMIDIDeviceDistanceMaxGet;
			pxAudio->DistanceMaxSet = PXMIDIDeviceDistanceMaxSet;
			pxAudio->DistanceMinGet = PXMIDIDeviceDistanceMinGet;
			pxAudio->DistanceMinSet = PXMIDIDeviceDistanceMinSet;
			pxAudio->ModeGet = PXMIDIDeviceModeGet;
			pxAudio->ModeSet = PXMIDIDeviceModeSet;
			pxAudio->VelocityGet = PXMIDIDeviceVelocityGet;
			pxAudio->VelocitySet = PXMIDIDeviceVelocitySet;
			pxAudio->DistanceFactorGet = PXMIDIDeviceDistanceFactorGet;
			pxAudio->DistanceFactorSet = PXMIDIDeviceDistanceFactorSet;
			pxAudio->DopplerFactorGet = PXMIDIDeviceDopplerFactorGet;
			pxAudio->DopplerFactorSet = PXMIDIDeviceDopplerFactorSet;
			pxAudio->OrientationGet = PXMIDIDeviceOrientationGet;
			pxAudio->OrientationSet = PXMIDIDeviceOrientationSet;
			pxAudio->RolloffFactorGet = PXMIDIDeviceRolloffFactorGet;
			pxAudio->RolloffFactorSet = PXMIDIDeviceRolloffFactorSet;
			pxAudio->DeferredSettingsCommit = PXMIDIDeviceDeferredSettingsCommit;

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnabled
		case PXAudioSystemWindowsMultiMedia:
		{
			pxAudio->Initialize = PXMultiMediaInitialize;
			pxAudio->DeviceAmount = PXMultiMediaDeviceAmount;
			pxAudio->DeviceFetch = PXMultiMediaDeviceFetch;
			pxAudio->DeviceFetchAll = PXMultiMediaDeviceFetchAll;
			pxAudio->DeviceOpen = PXMultiMediaDeviceOpen;
			pxAudio->DeviceClose = PXMultiMediaDeviceClose;
			pxAudio->DeviceLoad = PXMultiMediaDeviceLoad;
			pxAudio->DeviceBreakLoop = PXMultiMediaDeviceBreakLoop;
			pxAudio->DevicePositionGet = PXMultiMediaDevicePositionGet;
			pxAudio->DevicePlaybackRateGet = PXMultiMediaDevicePlaybackRateGet;
			pxAudio->DevicePlaybackRateSet = PXMultiMediaDevicePlaybackRateSet;
			pxAudio->DevicePitchIncrease = PXMultiMediaDevicePitchIncrease;
			pxAudio->DevicePitchSet = PXMultiMediaDevicePitchSet;
			pxAudio->DevicePitchReduce = PXMultiMediaDevicePitchReduce;
			pxAudio->DeviceVolumeGet = PXMultiMediaDeviceVolumeGet;
			pxAudio->DeviceVolumeSetEqual = PXMultiMediaDeviceVolumeSetEqual;
			pxAudio->DeviceVolumeSetIndividual = PXMultiMediaDeviceVolumeSetIndividual;
			pxAudio->DeviceStart = PXMultiMediaDeviceStart;
			pxAudio->DeviceStop = PXMultiMediaDeviceStop;
			pxAudio->DevicePause = PXMultiMediaDevicePause;
			pxAudio->DeviceRestart = PXMultiMediaDeviceRestart;
			pxAudio->DeviceReset = PXMultiMediaDeviceReset;

			break;
		}
#endif

#if PXAudioSystemWindowsDirectSoundEnable
		case PXAudioSystemWindowsDirectSound:
		{
			pxAudio->Initialize = PXDirectSoundInitialize;
			pxAudio->DeviceAmount = PXDirectSoundDeviceAmount;
			pxAudio->DeviceFetch = PXDirectSoundDeviceFetch;
			pxAudio->DeviceFetchAll = PXDirectSoundDeviceFetchAll;
			pxAudio->DeviceOpen = PXDirectSoundDeviceOpen;
			pxAudio->DeviceClose = PXDirectSoundDeviceClose;
			pxAudio->DeviceLoad = PXDirectSoundDeviceLoad;
			pxAudio->DevicePitchIncrease = PXDirectSoundDevicePitchIncrease;
			pxAudio->DevicePitchSet = PXDirectSoundDevicePitchSet;
			pxAudio->DevicePitchReduce = PXDirectSoundDevicePitchReduce;
			pxAudio->DeviceVolumeGet = PXDirectSoundDeviceVolumeGet;
			pxAudio->DeviceVolumeSetEqual = PXDirectSoundDeviceVolumeSetEqual;
			pxAudio->DeviceVolumeSetIndividual = PXDirectSoundDeviceVolumeSetIndividual;
			pxAudio->DeviceStart = PXDirectSoundDeviceStart;
			pxAudio->DeviceRestart = PXDirectSoundDeviceRestart;
			pxAudio->DeviceStop = PXDirectSoundDeviceStop;
			pxAudio->DevicePause = PXDirectSoundDevicePause;
			pxAudio->PlayCursorSet = PXDirectSoundDevicePlayCursorSet;
			pxAudio->PlayCursorGet = PXDirectSoundDevicePlayCursorGet;
			pxAudio->PlaySpeedSet = PXDirectSoundDevicePlaySpeedSet;
			pxAudio->PlaySpeedGet = PXDirectSoundDevicePlaySpeedGet;
			pxAudio->PositionSet = PXDirectSoundDevicePositionSet;
			pxAudio->PositionGet = PXDirectSoundDevicePositionGet;
			pxAudio->ConeAnglesSet = PXDirectSoundDeviceConeAnglesSet;
			pxAudio->ConeAnglesGet = PXDirectSoundDeviceConeAnglesGet;
			pxAudio->ConeOrientationGet = PXDirectSoundDeviceConeOrientationGet;
			pxAudio->ConeOrientationSet = PXDirectSoundDeviceConeOrientationSet;
			pxAudio->ConeOutsideVolumeGet = PXDirectSoundDeviceConeOutsideVolumeGet;
			pxAudio->ConeOutsideVolumeSet = PXDirectSoundDeviceConeOutsideVolumeSet;
			pxAudio->DistanceMaxGet = PXDirectSoundDeviceDistanceMaxGet;
			pxAudio->DistanceMaxSet = PXDirectSoundDeviceDistanceMaxSet;
			pxAudio->DistanceMinGet = PXDirectSoundDeviceDistanceMinGet;
			pxAudio->DistanceMinSet = PXDirectSoundDeviceDistanceMinSet;
			pxAudio->ModeGet = PXDirectSoundDeviceModeGet;
			pxAudio->ModeSet = PXDirectSoundDeviceModeSet;
			pxAudio->VelocityGet = PXDirectSoundDeviceVelocityGet;
			pxAudio->VelocitySet = PXDirectSoundDeviceVelocitySet;
			pxAudio->DistanceFactorGet = PXDirectSoundDeviceDistanceFactorGet;
			pxAudio->DistanceFactorSet = PXDirectSoundDeviceDistanceFactorSet;
			pxAudio->DopplerFactorGet = PXDirectSoundDeviceDopplerFactorGet;
			pxAudio->DopplerFactorSet = PXDirectSoundDeviceDopplerFactorSet;
			pxAudio->OrientationGet = PXDirectSoundDeviceOrientationGet;
			pxAudio->OrientationSet = PXDirectSoundDeviceOrientationSet;
			pxAudio->RolloffFactorGet = PXDirectSoundDeviceRolloffFactorGet;
			pxAudio->RolloffFactorSet = PXDirectSoundDeviceRolloffFactorSet;
			pxAudio->DeferredSettingsCommit = PXDirectSoundDeviceDeferredSettingsCommit;

			break;
		}
#endif

#if PXAudioSystemWindowsIAudioEnable
		case PXAudioSystemWindowsIAudio:
		{
			pxAudio->Initialize = PXIAudioInitialize;
			pxAudio->DeviceAmount = PXIAudioDeviceAmount;
			pxAudio->DeviceFetch = PXIAudioDeviceFetch;
			pxAudio->DeviceFetchAll = PXIAudioDeviceFetchAll;
			pxAudio->DeviceOpen = PXIAudioDeviceOpen;
			pxAudio->DeviceClose = PXIAudioDeviceClose;
			pxAudio->DeviceLoad = PXIAudioDeviceLoad;
			pxAudio->DevicePitchIncrease = PXIAudioDevicePitchIncrease;
			pxAudio->DevicePitchSet = PXIAudioDevicePitchSet;
			pxAudio->DevicePitchReduce = PXIAudioDevicePitchReduce;
			pxAudio->DeviceVolumeGet = PXIAudioDeviceVolumeGet;
			pxAudio->DeviceVolumeSetEqual = PXIAudioDeviceVolumeSetEqual;
			pxAudio->DeviceVolumeSetIndividual = PXIAudioDeviceVolumeSetIndividual;
			pxAudio->DeviceStart = PXIAudioDeviceStart;
			pxAudio->DeviceStop = PXIAudioDeviceStop;
			pxAudio->DevicePause = PXIAudioDevicePause;
			pxAudio->PlayCursorSet = PXIAudioDevicePlayCursorSet;
			pxAudio->PlayCursorGet = PXIAudioDevicePlayCursorGet;
			pxAudio->PlaySpeedSet = PXIAudioDevicePlaySpeedSet;
			pxAudio->PlaySpeedGet = PXIAudioDevicePlaySpeedGet;
			pxAudio->PositionSet = PXIAudioDevicePositionSet;
			pxAudio->PositionGet = PXIAudioDevicePositionGet;
			pxAudio->ConeAnglesSet = PXIAudioDeviceConeAnglesSet;
			pxAudio->ConeAnglesGet = PXIAudioDeviceConeAnglesGet;
			pxAudio->ConeOrientationGet = PXIAudioDeviceConeOrientationGet;
			pxAudio->ConeOrientationSet = PXIAudioDeviceConeOrientationSet;
			pxAudio->ConeOutsideVolumeGet = PXIAudioDeviceConeOutsideVolumeGet;
			pxAudio->ConeOutsideVolumeSet = PXIAudioDeviceConeOutsideVolumeSet;
			pxAudio->DistanceMaxGet = PXIAudioDeviceDistanceMaxGet;
			pxAudio->DistanceMaxSet = PXIAudioDeviceDistanceMaxSet;
			pxAudio->DistanceMinGet = PXIAudioDeviceDistanceMinGet;
			pxAudio->DistanceMinSet = PXIAudioDeviceDistanceMinSet;
			pxAudio->ModeGet = PXIAudioDeviceModeGet;
			pxAudio->ModeSet = PXIAudioDeviceModeSet;
			pxAudio->VelocityGet = PXIAudioDeviceVelocityGet;
			pxAudio->VelocitySet = PXIAudioDeviceVelocitySet;
			pxAudio->DistanceFactorGet = PXIAudioDeviceDistanceFactorGet;
			pxAudio->DistanceFactorSet = PXIAudioDeviceDistanceFactorSet;
			pxAudio->DopplerFactorGet = PXIAudioDeviceDopplerFactorGet;
			pxAudio->DopplerFactorSet = PXIAudioDeviceDopplerFactorSet;
			pxAudio->OrientationGet = PXIAudioDeviceOrientationGet;
			pxAudio->OrientationSet = PXIAudioDeviceOrientationSet;
			pxAudio->RolloffFactorGet = PXIAudioDeviceRolloffFactorGet;
			pxAudio->RolloffFactorSet = PXIAudioDeviceRolloffFactorSet;
			pxAudio->DeferredSettingsCommit = PXIAudioDeviceDeferredSettingsCommit;
			break;
		}
#endif

#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			pxAudio->Initialize = PXXAudioInitialize;
			pxAudio->DeviceAmount = PXXAudioDeviceAmount;
			pxAudio->DeviceFetch = PXXAudioDeviceFetch;
			pxAudio->DeviceFetchAll = PXXAudioDeviceFetchAll;
			pxAudio->DeviceOpen = PXXAudioDeviceOpen;
			pxAudio->DeviceClose = PXXAudioDeviceClose;
			pxAudio->DeviceLoad = PXXAudioDeviceLoad;
			pxAudio->DevicePitchIncrease = PXXAudioDevicePitchIncrease;
			pxAudio->DevicePitchSet = PXXAudioDevicePitchSet;
			pxAudio->DevicePitchReduce = PXXAudioDevicePitchReduce;
			pxAudio->DeviceVolumeGet = PXXAudioDeviceVolumeGet;
			pxAudio->DeviceVolumeSetEqual = PXXAudioDeviceVolumeSetEqual;
			pxAudio->DeviceVolumeSetIndividual = PXXAudioDeviceVolumeSetIndividual;
			pxAudio->DeviceStart = PXXAudioDeviceStart;
			pxAudio->DeviceStop = PXXAudioDeviceStop;
			pxAudio->DevicePause = PXXAudioDevicePause;

			break;
		}
#endif
		default:
			return PXActionRefusedArgumentInvalid;
	}

	pxAudio->Initialize(pxAudio);

	return PXActionSuccessful;
}

/*
printf
	(
		"+----------------+--------+\n"
		"| Type           : %5i\n"
		"| DeviceID       : %5i\n"
		"| ManufacturerID : %5i\n"
		"| ProductID      : %5i\n"
		"| DriverVersion  : Major:%i, Minor:%i\n"
		"| ProductName    : %ls\n"
		"| Formats        : %i\n"
		"| Channels       : %i\n"
		"| Support        : %i\n"
		"+----------------+--------+\n",
		audioDeviceCapabilities->Type,
		audioDeviceCapabilities->DeviceID,
		audioDeviceCapabilities->ManufacturerID,
		audioDeviceCapabilities->ProductID,
		audioDeviceCapabilities->DriverVersionMajor,
		audioDeviceCapabilities->DriverVersionMinor,
		audioDeviceCapabilities->ProductName,
		audioDeviceCapabilities->dwFormats,
		audioDeviceCapabilities->Channels,
		audioDeviceCapabilities->dwSupport
	);
*/
#include "PXAudio.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#include "PXDirectSound/PXDirectSound.h"
#include "PXMultiMedia/PXMultiMedia.h"
#include "PXXAudio/PXXAudio.h"

PXActionResult PXAPI PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem)
{
	PXClear(PXAudio, pxAudio);

	pxAudio->System = pxAudioSystem;

	switch (pxAudioSystem)
	{
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
		case PXAudioSystemWindowsKernalMixer:
		{
			break;
		}
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
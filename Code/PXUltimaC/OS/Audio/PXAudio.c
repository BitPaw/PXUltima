#include "PXAudio.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <OS/Console/PXConsole.h>

#include "PXDirectSound/PXDirectSound.h"
#include "PXMultiMedia/PXMultiMedia.h"
#include "PXIAudio/PXIAudio.h"
#include "PXXAudio/PXXAudio.h"
#include "PXAudioMIDI/PXAudioMIDI.h"

PXActionResult PXAPI PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem)
{
#if PXLogEnable
	PXLogPrint
	(
		PXLoggingError,
		"Audio",
		"Initialize",
		"---Start---"
	);
#endif

	PXClear(PXAudio, pxAudio);
	pxAudio->System = pxAudioSystem;

	switch (pxAudioSystem)
	{
#if OSWindows
		case PXAudioSystemWindowsMIDI:
		{
			pxAudio->Initialize = PXMIDIInitialize;
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnabled
		case PXAudioSystemWindowsMultiMedia:
		{
			pxAudio->Initialize = PXMultiMediaInitialize;
			break;
		}
#endif

#if PXAudioSystemWindowsDirectSoundEnable
		case PXAudioSystemWindowsDirectSound:
		{
			pxAudio->Initialize = PXDirectSoundInitialize;
			break;
		}
#endif

#if PXAudioSystemWindowsIAudioEnable
		case PXAudioSystemWindowsIAudio:
		{
			pxAudio->Initialize = PXIAudioInitialize;
			
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
		{
#if PXLogEnable
			PXLogPrint
			(
				PXLoggingError,
				"PXAudio",
				"Initialize",
				"There is no audio system"
			);
#endif
			return PXActionRefusedArgumentInvalid;

		}	
	}

	// Initialize
	{
		const PXActionResult initializeResult = pxAudio->Initialize(pxAudio);

		if(PXActionSuccessful != initializeResult)
		{
#if PXLogEnable
			PXLogPrint
			(
				PXLoggingError,
				"Audio",
				"Initialize",
				"Failed!"
			);
#endif
		}

#if PXLogEnable
		PXLogPrint
		(
			PXLoggingInfo,
			"Audio",
			"Initialize",
			"Success"
		);
#endif

	}



#if PXLogEnable
	PXLogPrint
	(
		PXLoggingError,
		"Audio",
		"Initialize",
		"---Done---"
	);
#endif


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
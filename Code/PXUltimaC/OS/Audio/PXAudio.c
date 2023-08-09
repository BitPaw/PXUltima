#include "PXAudio.h"

#if PXAudioUSE

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#if OSUnix
// include audio lib
#elif PXOSWindowsDestop

#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else 
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif

#pragma comment(lib, "winmm.lib") // Library: Windows Multimedia 
#endif

#if PXOSWindowsDestop
PXActionResult PXAudioConvertMMResult(const unsigned int mmResultID)
{
	switch (mmResultID)
	{
		case MMSYSERR_NOERROR:  return PXActionSuccessful;
		case MMSYSERR_ERROR:return AudioResultErrorUnspecified;
		case MMSYSERR_BADDEVICEID:  return AudioResultDeviceIDInvalid;
		case MMSYSERR_NOTENABLED:   return AudioResultDriverNotEnabled;
		case MMSYSERR_ALLOCATED: return AudioResultDeviceAlreadyAllocated;
		case MMSYSERR_INVALHANDLE:  return AudioResultDeviceHandleInvalid;
		case MMSYSERR_NODRIVER:   return AudioResultDeviceNoDriver;
		case MMSYSERR_NOMEM:return AudioResultOutOfMemory;
		case MMSYSERR_NOTSUPPORTED: return AudioResultNotSupported;
		case MMSYSERR_BADERRNUM:  return AudioResultErrorValueInvalid;
		case MMSYSERR_INVALFLAG: return AudioResultFlagInvalid;
		case MMSYSERR_INVALPARAM: return AudioResultParameterInvalid;
		case MMSYSERR_HANDLEBUSY:return AudioResultDeviceHandleBusy;
		case MMSYSERR_INVALIDALIAS:return AudioResultAliasNotFound;
		case MMSYSERR_BADDB: return AudioResultPXRegistryDatabaseInvalid;
		case MMSYSERR_KEYNOTFOUND: return AudioResultPXRegistryKeyNotFound;
		case MMSYSERR_READERROR:  return AudioResultPXRegistryReadError;
		case MMSYSERR_WRITEERROR: return AudioResultPXRegistryWriteError;
		case MMSYSERR_DELETEERROR:   return AudioResultPXRegistryDeleteError;
		case MMSYSERR_VALNOTFOUND: return AudioResultPXRegistryValueNotFound;
		case MMSYSERR_NODRIVERCB:  return AudioResultDriverNoCallback;
			//	case MMSYSERR_MOREDATA:   return AudioResultMoreData;

		case WAVERR_BADFORMAT: return AudioResultWaveFormatUnsupported;
		case WAVERR_STILLPLAYING: return AudioResultDeviceIsStillPlaying;
		case WAVERR_UNPREPARED: return AudioResultReaderIsNotPrepared;
		case WAVERR_SYNC: return AudioResultDeviceIsSynchronous;

		default:
			return PXActionInvalid;
	}
}
#endif

void PXAudioSourceConstruct(PXAudioSource* const audioSource)
{
	PXMemoryClear(audioSource, sizeof(PXAudioSource));

	audioSource->Pitch = 0.4f;
	audioSource->Volume = 0.02f;
	audioSource->Looping = PXTrue;
}

void PXAudioSourcePitchIncrease(PXAudioSource* const audioSource, float amount)
{
	//assert(amount > 0.0f);

	audioSource->Pitch += amount;

	if (audioSource->Pitch > PitchMaximum)
	{
		audioSource->Pitch = PitchMaximum;
	}
}

void PXAudioSourcePitchReduce(PXAudioSource* const audioSource, float amount)
{
	//assert(amount > 0.0f);

	audioSource->Pitch -= amount;

	if (audioSource->Pitch < PitchMinimum)
	{
		audioSource->Pitch = PitchMinimum;
	}
}

PXActionResult PXAudioOutputOpen(PXAudioDeviceOutput* audioDeviceOutput, unsigned int deviceID, unsigned short formatTag, unsigned short channels, unsigned int samplesPerSec, unsigned int avgBytesPerSec, unsigned short blockAlign, unsigned short bitsPerSample, unsigned short cbSize)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	WAVEFORMATEX waveFormatEX;
	DWORD_PTR dwCallback = 0;
	DWORD_PTR dwInstance = 0;
	DWORD fdwOpen = CALLBACK_NULL;

	waveFormatEX.wFormatTag = formatTag;
	waveFormatEX.nChannels = channels;
	waveFormatEX.nSamplesPerSec = samplesPerSec;
	waveFormatEX.nAvgBytesPerSec = avgBytesPerSec;
	waveFormatEX.nBlockAlign = blockAlign;
	waveFormatEX.wBitsPerSample = bitsPerSample;
	waveFormatEX.cbSize = cbSize;

	const MMRESULT result = waveOutOpen
	(
		&audioDeviceOutput->Handle,
		deviceID,
		&waveFormatEX,
		dwCallback,
		dwInstance,
		fdwOpen
	);
	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputPrepare(PXAudioDeviceOutput* audioDeviceOutput)
{
#if OSUnix
	return PXActionRefusedNotImplemented;
#elif PXOSWindowsDestop
	return PXActionRefusedNotImplemented;
#else 
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputWrite(PXAudioDeviceOutput* audioDeviceOutput, void* dataBuffer, PXSize bufferLength, unsigned int bytesRecorded, unsigned int user, unsigned int flags, unsigned int loopControlCounter)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	WAVEHDR waveHeader;
	const UINT cbwh = sizeof(waveHeader);

	waveHeader.lpData = (LPSTR)dataBuffer;
	waveHeader.dwBufferLength = bufferLength;
	waveHeader.dwBytesRecorded = bytesRecorded;
	waveHeader.dwUser = user;
	waveHeader.dwFlags = flags;
	waveHeader.dwLoops = loopControlCounter;

	// Prepare
	{
		const MMRESULT prepareResultID = waveOutPrepareHeader(audioDeviceOutput->Handle, &waveHeader, cbwh);
		const PXActionResult prepareResult = PXAudioConvertMMResult(prepareResultID);
		
		PXActionReturnOnError(prepareResult);
	}

	// Write Data
	{
		const MMRESULT writeResultID = waveOutWrite(audioDeviceOutput->Handle, &waveHeader, cbwh);
		const PXActionResult writeResult = PXAudioConvertMMResult(writeResultID);

		PXActionReturnOnError(writeResult);
	}

	return PXActionSuccessful;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputClose(PXAudioDeviceOutput* audioDeviceOutput)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop

	const MMRESULT result = waveOutClose(audioDeviceOutput->Handle);
	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	audioDeviceOutput->Handle = 0;

	return audioResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputVolumeGet(PXAudioDeviceOutput* audioDeviceOutput, unsigned short* const volume)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	DWORD volumeDW = 0;

	const MMRESULT volumeResultID = waveOutGetVolume(audioDeviceOutput->Handle, &volumeDW);
	const PXActionResult audioResult = PXAudioConvertMMResult(volumeResultID);
	const PXBool successful = PXActionSuccessful == audioResult;

	if (!successful)
	{
		*volume = -1;
		return audioResult;
	}

	*volume = volumeDW;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputVolumeSetEqual(PXAudioDeviceOutput* audioDeviceOutput, const unsigned int volume)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT volumeResultID = waveOutSetVolume(audioDeviceOutput->Handle, volume);
	const PXActionResult audioResult = PXAudioConvertMMResult(volumeResultID);

	return audioResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputVolumeSetIndividual(PXAudioDeviceOutput* audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
	const PXActionResult audioResult = PXAudioOutputVolumeSetEqual(audioDeviceOutput, volumeCombined);

	return audioResult;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputPause(PXAudioDeviceOutput* audioDeviceOutput)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAudioOutputPitchSet(PXAudioDeviceOutput* audioDeviceOutput, const unsigned int pitch)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT pitchResultID = waveOutSetPitch(audioDeviceOutput->Handle, pitch);
	const PXActionResult pitchResult = PXAudioConvertMMResult(pitchResultID);

	return pitchResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioOutputPlaybackRateSet(PXAudioDeviceOutput* audioDeviceOutput, const unsigned int pitch)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT playbackRateResultID = waveOutSetPlaybackRate(audioDeviceOutput->Handle, pitch);
	const PXActionResult playbackRateResult = PXAudioConvertMMResult(playbackRateResultID);

	return playbackRateResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDevicesFetchOutput(PXAudioDeviceCapabilities* audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* audioDeviceCapabilitiesListSize)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const UINT numberOfPutpudevices = waveOutGetNumDevs();
	const PXBool isListBigEngough = numberOfPutpudevices < audioDeviceCapabilitiesListSizeMax;

	if (!isListBigEngough)
	{
		audioDeviceCapabilitiesListSize = 0;

		return AudioResultDeviceListNotBigEnough;
	}

	for (PXSize i = 0; i < numberOfPutpudevices; i++)
	{
		const UINT size = sizeof(WAVEOUTCAPSW);
		WAVEOUTCAPSW wAVEOUTCAPSW;

		{
			const MMRESULT result = waveOutGetDevCapsW(i, &wAVEOUTCAPSW, size);
			const PXActionResult audioResult = PXAudioConvertMMResult(result);

			PXActionReturnOnError(audioResult);
		}

		PXAudioDeviceCapabilities* audioDeviceCapabilities = &audioDeviceCapabilitiesList[i];

		audioDeviceCapabilities->DeviceID = i;
		audioDeviceCapabilities->Type = AudioDeviceTypeOutput;
		audioDeviceCapabilities->ManufacturerID = wAVEOUTCAPSW.wMid;
		audioDeviceCapabilities->ProductID = wAVEOUTCAPSW.wPid;
		audioDeviceCapabilities->DriverVersionMajor = HIBYTE(wAVEOUTCAPSW.vDriverVersion);
		audioDeviceCapabilities->DriverVersionMinor = LOBYTE(wAVEOUTCAPSW.vDriverVersion);

		PXTextCopyW(wAVEOUTCAPSW.szPname, MAXPNAMELEN, audioDeviceCapabilities->ProductName, SoundDeviceProductName);

		audioDeviceCapabilities->Channels = wAVEOUTCAPSW.wChannels;
	}

	*audioDeviceCapabilitiesListSize = numberOfPutpudevices;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDevicesFetchInput(PXAudioDeviceCapabilities* audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* audioDeviceCapabilitiesListSize)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const UINT numberOfInputDevices = waveInGetNumDevs();
	const PXBool isListBigEngough = numberOfInputDevices < audioDeviceCapabilitiesListSizeMax;

	if (!isListBigEngough)
	{
		audioDeviceCapabilitiesListSize = 0;

		return AudioResultDeviceListNotBigEnough;
	}

	for (PXSize i = 0; i < numberOfInputDevices; ++i)
	{
		WAVEINCAPSW waveInputCapabilitiesW;

		{
			const UINT waveInputCapabilitiesWSize = sizeof(waveInputCapabilitiesW);
			const MMRESULT result = waveInGetDevCapsW(i, &waveInputCapabilitiesW, waveInputCapabilitiesWSize);
			const PXActionResult audioResult = PXAudioConvertMMResult(result);

			PXActionReturnOnError(audioResult);
		}

		PXAudioDeviceCapabilities* audioDeviceCapabilities = &audioDeviceCapabilitiesList[i];

		audioDeviceCapabilities->Type = AudioDeviceTypeInput;
		audioDeviceCapabilities->ManufacturerID = waveInputCapabilitiesW.wMid;
		audioDeviceCapabilities->ProductID = waveInputCapabilitiesW.wPid;
		audioDeviceCapabilities->DriverVersionMajor = HIBYTE(waveInputCapabilitiesW.vDriverVersion);
		audioDeviceCapabilities->DriverVersionMinor = LOBYTE(waveInputCapabilitiesW.vDriverVersion);

		PXTextCopyW(waveInputCapabilitiesW.szPname, MAXPNAMELEN, audioDeviceCapabilities->ProductName, SoundDeviceProductName);

		audioDeviceCapabilities->Channels = waveInputCapabilitiesW.wChannels;
	}

	*audioDeviceCapabilitiesListSize = numberOfInputDevices;

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}
#endif

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
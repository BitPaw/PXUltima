#include "PXAudio.h"

#if PXAudioUSE

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#if OSUnix
#elif OSWindows

#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else 
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif

#pragma comment(lib, "winmm.lib")
#endif

#include <stdio.h>

#if OSWindows
PXAudioResult PXAudioConvertMMResult(const unsigned int mmResultID)
{
	switch (mmResultID)
	{
		case MMSYSERR_NOERROR:  return AudioResultSucessful;
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
			return AudioResultInvalid;
	}
}
#endif

void PXAudioDeviceCapabilitiesPrinf(PXAudioDeviceCapabilities* const audioDeviceCapabilities)
{
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
}

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

PXAudioResult PXAudioOutputOpen(PXAudioDeviceOutput* audioDeviceOutput, unsigned int deviceID, unsigned short formatTag, unsigned short channels, unsigned int samplesPerSec, unsigned int avgBytesPerSec, unsigned short blockAlign, unsigned short bitsPerSample, unsigned short cbSize)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
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
	const PXAudioResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
#endif
}

PXAudioResult PXAudioOutputPrepare(PXAudioDeviceOutput* audioDeviceOutput)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	return AudioResultInvalid;

#endif
}

PXAudioResult PXAudioOutputWrite(PXAudioDeviceOutput* audioDeviceOutput, void* dataBuffer, PXSize bufferLength, unsigned int bytesRecorded, unsigned int user, unsigned int flags, unsigned int loopControlCounter)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
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
		const PXAudioResult prepareResult = PXAudioConvertMMResult(prepareResultID);
		const unsigned char wasSucessful = prepareResult == AudioResultSucessful;

		if (!wasSucessful)
		{
			return prepareResult;
		}
	}

	// Write Data
	{
		const MMRESULT writeResultID = waveOutWrite(audioDeviceOutput->Handle, &waveHeader, cbwh);
		const PXAudioResult writeResult = PXAudioConvertMMResult(writeResultID);
		const unsigned char wasSucessful = writeResult == AudioResultSucessful;

		if (!wasSucessful)
		{
			return writeResult;
		}
	}
#endif

	return AudioResultSucessful;
}

PXAudioResult PXAudioOutputClose(PXAudioDeviceOutput* audioDeviceOutput)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows

	const MMRESULT result = waveOutClose(audioDeviceOutput->Handle);
	const PXAudioResult audioResult = PXAudioConvertMMResult(result);

	audioDeviceOutput->Handle = 0;

	return audioResult;

#endif
}

PXAudioResult PXAudioOutputVolumeGet(PXAudioDeviceOutput* audioDeviceOutput, unsigned short* const volume)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	DWORD volumeDW = 0;

	const MMRESULT volumeResultID = waveOutGetVolume(audioDeviceOutput->Handle, &volumeDW);
	const PXAudioResult audioResult = PXAudioConvertMMResult(volumeResultID);
	const unsigned char sucessful = audioResult == AudioResultSucessful;

	if (!sucessful)
	{
		*volume = 0;
		return audioResult;
	}

	*volume = volumeDW;
#endif

	return AudioResultSucessful;
}

PXAudioResult PXAudioOutputVolumeSetEqual(PXAudioDeviceOutput* audioDeviceOutput, const unsigned int volume)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	const MMRESULT volumeResultID = waveOutSetVolume(audioDeviceOutput->Handle, volume);
	const PXAudioResult audioResult = PXAudioConvertMMResult(volumeResultID);

	return audioResult;
#endif
}

PXAudioResult PXAudioOutputVolumeSetIndividual(PXAudioDeviceOutput* audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
	const PXAudioResult audioResult = PXAudioOutputVolumeSetEqual(audioDeviceOutput, volumeCombined);

	return audioResult;
}

PXAudioResult PXAudioOutputPause(PXAudioDeviceOutput* audioDeviceOutput)
{
	return AudioResultInvalid;
}

PXAudioResult PXAudioOutputPitchSet(PXAudioDeviceOutput* audioDeviceOutput, const unsigned int pitch)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	const MMRESULT pitchResultID = waveOutSetPitch(audioDeviceOutput->Handle, pitch);
	const PXAudioResult pitchResult = PXAudioConvertMMResult(pitchResultID);

	return pitchResult;
#endif
}

PXAudioResult PXAudioOutputPlaybackRateSet(PXAudioDeviceOutput* audioDeviceOutput, const unsigned int pitch)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	const MMRESULT playbackRateResultID = waveOutSetPlaybackRate(audioDeviceOutput->Handle, pitch);
	const PXAudioResult playbackRateResult = PXAudioConvertMMResult(playbackRateResultID);

	return playbackRateResult;
#endif
}

PXAudioResult PXAudioDevicesFetchOutput(PXAudioDeviceCapabilities* audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* audioDeviceCapabilitiesListSize)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	const UINT numberOfPutpudevices = waveOutGetNumDevs();
	const unsigned char isListBigEngough = numberOfPutpudevices < audioDeviceCapabilitiesListSizeMax;

	if (!isListBigEngough)
	{
		audioDeviceCapabilitiesListSize = 0;

		return AudioResultDeviceListNotBigEnough;
	}

	for (PXSize i = 0; i < numberOfPutpudevices; i++)
	{
		const UINT size = sizeof(WAVEOUTCAPSW);
		WAVEOUTCAPSW wAVEOUTCAPSW;

		const MMRESULT result = waveOutGetDevCapsW(i, &wAVEOUTCAPSW, size);
		const PXAudioResult audioResult = PXAudioConvertMMResult(result);
		const unsigned char sucessful = audioResult == AudioResultSucessful;

		if (!sucessful)
		{
			return audioResult;
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

	audioDeviceCapabilitiesListSize = numberOfPutpudevices;

	return AudioResultSucessful;
#endif
}

PXAudioResult PXAudioDevicesFetchInput(PXAudioDeviceCapabilities* audioDeviceCapabilitiesList, const PXSize audioDeviceCapabilitiesListSizeMax, PXSize* audioDeviceCapabilitiesListSize)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	const UINT numberOfInputDevices = waveInGetNumDevs();
	const unsigned char isListBigEngough = numberOfInputDevices < audioDeviceCapabilitiesListSizeMax;

	if (!isListBigEngough)
	{
		audioDeviceCapabilitiesListSize = 0;

		return AudioResultDeviceListNotBigEnough;
	}

	for (PXSize i = 0; i < numberOfInputDevices; ++i)
	{
		WAVEINCAPSW waveInputCapabilitiesW;
		const UINT waveInputCapabilitiesWSize = sizeof(waveInputCapabilitiesW);
		const MMRESULT result = waveInGetDevCapsW(i, &waveInputCapabilitiesW, waveInputCapabilitiesWSize);
		const PXAudioResult audioResult = PXAudioConvertMMResult(result);
		const unsigned char sucessful = audioResult == AudioResultSucessful;

		if (!sucessful)
		{
			return audioResult;
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

	audioDeviceCapabilitiesListSize = numberOfInputDevices;

	return AudioResultSucessful;
#endif
}
#endif
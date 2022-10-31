#include "Sound.h"

#include <assert.h>
#include <stdio.h>
#include <Memory/PXMemory.h>
#include <Text/Text.h>

#if OSUnix
#elif OSWindows
#pragma comment(lib, "winmm.lib")
#endif

#if OSWindowsEE
AudioResult AudioConvertMMResult(const unsigned int mmResultID)
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
	case MMSYSERR_BADDB: return AudioResultRegistryDatabaseInvalid;
	case MMSYSERR_KEYNOTFOUND: return AudioResultRegistryKeyNotFound;
	case MMSYSERR_READERROR:  return AudioResultRegistryReadError;
	case MMSYSERR_WRITEERROR: return AudioResultRegistryWriteError;
	case MMSYSERR_DELETEERROR:   return AudioResultRegistryDeleteError;
	case MMSYSERR_VALNOTFOUND: return AudioResultRegistryValueNotFound;
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

void AudioDeviceCapabilitiesPrinf(AudioDeviceCapabilities* const audioDeviceCapabilities)
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

void AudioSourceConstruct(AudioSource* const audioSource)
{
	MemorySet(audioSource, sizeof(AudioSource), 0);

	audioSource->Pitch = 0.4;
	audioSource->Volume = 0.02;
	audioSource->Looping = 1;
}

void AudioSourcePitchIncrease(AudioSource* const audioSource, float amount)
{
	assert(amount > 0.0f);

	audioSource->Pitch += amount;

	if (audioSource->Pitch > PitchMaximum)
	{
		audioSource->Pitch = PitchMaximum;
	}
}

void AudioSourcePitchReduce(AudioSource* const audioSource, float amount)
{
	assert(amount > 0.0f);

	audioSource->Pitch -= amount;

	if (audioSource->Pitch < PitchMinimum)
	{
		audioSource->Pitch = PitchMinimum;
	}
}

AudioResult AudioOutputOpen(AudioDeviceOutput* audioDeviceOutput, unsigned int deviceID, unsigned short formatTag, unsigned short channels, unsigned int samplesPerSec, unsigned int avgBytesPerSec, unsigned short blockAlign, unsigned short bitsPerSample, unsigned short cbSize)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
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
	const AudioResult audioResult = AudioConvertMMResult(result);

	return audioResult;
#endif
}

AudioResult AudioOutputPrepare(AudioDeviceOutput* audioDeviceOutput)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindows
	return AudioResultInvalid;

#endif
}

AudioResult AudioOutputWrite(AudioDeviceOutput* audioDeviceOutput, void* dataBuffer, size_t bufferLength, unsigned int bytesRecorded, unsigned int user, unsigned int flags, unsigned int loopControlCounter)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
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
		const AudioResult prepareResult = AudioConvertMMResult(prepareResultID);
		const unsigned char wasSucessful = prepareResult == AudioResultSucessful;

		if (!wasSucessful)
		{
			return prepareResult;
		}
	}

	// Write Data
	{
		const MMRESULT writeResultID = waveOutWrite(audioDeviceOutput->Handle, &waveHeader, cbwh);
		const AudioResult writeResult = AudioConvertMMResult(writeResultID);
		const unsigned char wasSucessful = writeResult == AudioResultSucessful;

		if (!wasSucessful)
		{
			return writeResult;
		}
	}
#endif

	return AudioResultSucessful;
}

AudioResult AudioOutputClose(AudioDeviceOutput* audioDeviceOutput)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE

	const MMRESULT result = waveOutClose(audioDeviceOutput->Handle);
	const AudioResult audioResult = AudioConvertMMResult(result);

	audioDeviceOutput->Handle = 0;

	return audioResult;

#endif
}

AudioResult AudioOutputVolumeGet(AudioDeviceOutput* audioDeviceOutput, unsigned short* volume)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
	DWORD volumeDW = 0;

	const MMRESULT volumeResultID = waveOutGetVolume(audioDeviceOutput->Handle, &volumeDW);
	const AudioResult audioResult = AudioConvertMMResult(volumeResultID);
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

AudioResult AudioOutputVolumeSetEqual(AudioDeviceOutput* audioDeviceOutput, const unsigned int volume)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
	const MMRESULT volumeResultID = waveOutSetVolume(audioDeviceOutput->Handle, volume);
	const AudioResult audioResult = AudioConvertMMResult(volumeResultID);

	return audioResult;
#endif

	return AudioResultInvalid;
}

AudioResult AudioOutputVolumeSetIndividual(AudioDeviceOutput* audioDeviceOutput, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
	const AudioResult audioResult = AudioOutputVolumeSetEqual(audioDeviceOutput, volumeCombined);

	return audioResult;
}

AudioResult AudioOutputPause(AudioDeviceOutput* audioDeviceOutput)
{
	return AudioResultInvalid;
}

AudioResult AudioOutputPitchSet(AudioDeviceOutput* audioDeviceOutput, const unsigned int pitch)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
	const MMRESULT pitchResultID = waveOutSetPitch(audioDeviceOutput->Handle, pitch);
	const AudioResult pitchResult = AudioConvertMMResult(pitchResultID);

	return pitchResult;
#endif
}

AudioResult AudioOutputPlaybackRateSet(AudioDeviceOutput* audioDeviceOutput, const unsigned int pitch)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
	const MMRESULT playbackRateResultID = waveOutSetPlaybackRate(audioDeviceOutput->Handle, pitch);
	const AudioResult playbackRateResult = AudioConvertMMResult(playbackRateResultID);

	return playbackRateResult;
#endif
}

AudioResult AudioDevicesFetchOutput(AudioDeviceCapabilities* audioDeviceCapabilitiesList, const size_t audioDeviceCapabilitiesListSizeMax, size_t* audioDeviceCapabilitiesListSize)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
	const UINT numberOfPutpudevices = waveOutGetNumDevs();
	const unsigned char isListBigEngough = numberOfPutpudevices < audioDeviceCapabilitiesListSizeMax;

	if (!isListBigEngough)
	{
		audioDeviceCapabilitiesListSize = 0;

		return AudioResultDeviceListNotBigEnough;
	}

	for (size_t i = 0; i < numberOfPutpudevices; i++)
	{
		const UINT size = sizeof(WAVEOUTCAPSW);
		WAVEOUTCAPSW wAVEOUTCAPSW;

		const MMRESULT result = waveOutGetDevCapsW(i, &wAVEOUTCAPSW, size);
		const AudioResult audioResult = AudioConvertMMResult(result);
		const unsigned char sucessful = audioResult == AudioResultSucessful;

		if (!sucessful)
		{
			return audioResult;
		}

		AudioDeviceCapabilities* audioDeviceCapabilities = &audioDeviceCapabilitiesList[i];

		audioDeviceCapabilities->DeviceID = i;
		audioDeviceCapabilities->Type = AudioDeviceTypeOutput;
		audioDeviceCapabilities->ManufacturerID = wAVEOUTCAPSW.wMid;
		audioDeviceCapabilities->ProductID = wAVEOUTCAPSW.wPid;
		audioDeviceCapabilities->DriverVersionMajor = HIBYTE(wAVEOUTCAPSW.vDriverVersion);
		audioDeviceCapabilities->DriverVersionMinor = LOBYTE(wAVEOUTCAPSW.vDriverVersion);

		TextCopyW(wAVEOUTCAPSW.szPname, MAXPNAMELEN, audioDeviceCapabilities->ProductName, SoundDeviceProductName);

		audioDeviceCapabilities->Channels = wAVEOUTCAPSW.wChannels;
	}


	audioDeviceCapabilitiesListSize = numberOfPutpudevices;

	return AudioResultSucessful;
#endif
}

AudioResult AudioDevicesFetchInput(AudioDeviceCapabilities* audioDeviceCapabilitiesList, const size_t audioDeviceCapabilitiesListSizeMax, size_t* audioDeviceCapabilitiesListSize)
{
#if OSUnix
	return AudioResultInvalid;

#elif OSWindowsEE
	const UINT numberOfInputDevices = waveInGetNumDevs();
	const unsigned char isListBigEngough = numberOfInputDevices < audioDeviceCapabilitiesListSizeMax;

	if (!isListBigEngough)
	{
		audioDeviceCapabilitiesListSize = 0;

		return AudioResultDeviceListNotBigEnough;
	}

	for (size_t i = 0; i < numberOfInputDevices; ++i)
	{
		WAVEINCAPSW waveInputCapabilitiesW;
		const UINT waveInputCapabilitiesWSize = sizeof(waveInputCapabilitiesW);
		const MMRESULT result = waveInGetDevCapsW(i, &waveInputCapabilitiesW, waveInputCapabilitiesWSize);
		const AudioResult audioResult = AudioConvertMMResult(result);
		const unsigned char sucessful = audioResult == AudioResultSucessful;

		if (!sucessful)
		{
			return audioResult;
		}

		AudioDeviceCapabilities* audioDeviceCapabilities = &audioDeviceCapabilitiesList[i];

		audioDeviceCapabilities->Type = AudioDeviceTypeInput;
		audioDeviceCapabilities->ManufacturerID = waveInputCapabilitiesW.wMid;
		audioDeviceCapabilities->ProductID = waveInputCapabilitiesW.wPid;
		audioDeviceCapabilities->DriverVersionMajor = HIBYTE(waveInputCapabilitiesW.vDriverVersion);
		audioDeviceCapabilities->DriverVersionMinor = LOBYTE(waveInputCapabilitiesW.vDriverVersion);

		TextCopyW(waveInputCapabilitiesW.szPname, MAXPNAMELEN, audioDeviceCapabilities->ProductName, SoundDeviceProductName);

		audioDeviceCapabilities->Channels = waveInputCapabilitiesW.wChannels;
	}

	audioDeviceCapabilitiesListSize = numberOfInputDevices;

	return AudioResultSucessful;
#endif
}

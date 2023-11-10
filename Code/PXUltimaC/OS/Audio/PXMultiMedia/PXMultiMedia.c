#include "PXMultiMedia.h"

#if PXAudioSystemWindowsMultimediaEnabled
#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else 
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif
#pragma comment(lib, "winmm.lib") // Library: Windows Multimedia 

PXActionResult PXAudioConvertMMResult(const PXInt32U mmResultID)
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
		case MMSYSERR_NOMEM:return PXActionFailedMemoryAllocation;
		case MMSYSERR_NOTSUPPORTED: return PXActionRefusedNotSupported;
		case MMSYSERR_BADERRNUM:  return AudioResultErrorValueInvalid;
		case MMSYSERR_INVALFLAG: return AudioResultFlagInvalid;
		case MMSYSERR_INVALPARAM: return PXActionRefusedArgumentInvalid;
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

		case WAVERR_BADFORMAT: return PXActionRefusedFormatNotSupported;
		case WAVERR_STILLPLAYING: return AudioResultDeviceIsStillPlaying;
		case WAVERR_UNPREPARED: return AudioResultReaderIsNotPrepared;
		case WAVERR_SYNC: return AudioResultDeviceIsSynchronous;

		default:
			return PXActionInvalid;
	}
}

PXActionResult PXAPI PXMultiMediaDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			result = waveInStart((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutRestart((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			result = waveInStop((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutPause((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		//case PXAudioDeviceTypeInput:
		//{
		//	result = waveInPause((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
		//	break;
		//}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutPause((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDeviceRestart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		//case PXAudioDeviceTypeInput:
		//{
		//	result = waveInRestart((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
		//	break;
		//}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutRestart((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	pxAudioDevice->ResourceID.DirectXInterface = PXNull;

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDeviceReset(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			result = waveInReset((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutReset((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	pxAudioDevice->ResourceID.DirectXInterface = PXNull;

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaInitialize(PXAudio* const pxAudio)
{
	return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
	switch (pxAudioDeviceType)
	{	
		case PXAudioDeviceTypeInput:
			*amount = waveInGetNumDevs();
			break;

		case PXAudioDeviceTypeOutput:
			*amount = waveOutGetNumDevs();
			break;

		default:
			*amount = 0;
			return PXActionRefusedArgumentInvalid;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
	PXClear(PXAudioDevice, pxAudioDevice);

	MMRESULT result;

	pxAudioDevice->Type = pxAudioDeviceType;

	switch (pxAudioDeviceType)
	{
		case PXAudioDeviceTypeInput:
		{
			const UINT size = sizeof(WAVEINCAPS);
			WAVEINCAPS waveInCapsA;			

			result = waveInGetDevCapsA(deviceID, &waveInCapsA, size);

			pxAudioDevice->ManufacturerID = waveInCapsA.wMid;
			pxAudioDevice->ProductID = waveInCapsA.wPid;
			pxAudioDevice->DriverVersionMajor = HIBYTE(waveInCapsA.vDriverVersion);
			pxAudioDevice->DriverVersionMinor = LOBYTE(waveInCapsA.vDriverVersion);
			pxAudioDevice->Channels = waveInCapsA.wChannels;

			PXTextCopyA(waveInCapsA.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			const UINT size = sizeof(WAVEOUTCAPS);
			WAVEOUTCAPS waveOutCapA;

			result = waveOutGetDevCapsA(deviceID, &waveOutCapA, size);

			pxAudioDevice->ManufacturerID = waveOutCapA.wMid;
			pxAudioDevice->ProductID = waveOutCapA.wPid;
			pxAudioDevice->DriverVersionMajor = HIBYTE(waveOutCapA.vDriverVersion);
			pxAudioDevice->DriverVersionMinor = LOBYTE(waveOutCapA.vDriverVersion);
			pxAudioDevice->Channels = waveOutCapA.wChannels;

			PXTextCopyA(waveOutCapA.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	{
		const PXActionResult audioResult = PXAudioConvertMMResult(result);

		PXActionReturnOnError(audioResult);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
	for (PXSize i = 0; i < amount; ++i)
	{
		const PXActionResult fetchResult = PXMultiMediaDeviceFetch(pxAudio, pxAudioDeviceType, i, &pxAudioDevice[i]);

		PXActionReturnOnError(fetchResult);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
	DWORD_PTR dwCallback = 0;
	DWORD_PTR dwInstance = 0;
	DWORD fdwOpen = CALLBACK_NULL;

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;// pxAudioSource->F;         /* format type */
	waveFormat.nChannels = pxAudioDevice->Channels;          /* number of channels (i.e. mono, stereo...) */
	waveFormat.nSamplesPerSec = pxAudioDevice->SamplesPerSecound;     /* sample rate */
	waveFormat.nAvgBytesPerSec = pxAudioDevice->AverageBytesPerSecound;    /* for buffer estimation */
	waveFormat.nBlockAlign = pxAudioDevice->BlockAlignSize;        /* block size of data */
	waveFormat.wBitsPerSample = pxAudioDevice->BitsPerSample;     /* number of bits per sample of mono data */
	waveFormat.cbSize = sizeof(WAVEFORMATEX);

	///aveFormat.wFormatTag = WAVE_FORMAT_PCM;
	//waveFormat.nChannels = 2;
	//waveFormat.nSamplesPerSec = 44100;
	//waveFormat.wBitsPerSample = 8;

	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			result = waveInOpen
			(
				&pxAudioDevice->ResourceID.DirectXInterface,
				pxAudioDevice->DeviceID,
				&waveFormat,
				dwCallback,
				dwInstance,
				fdwOpen
			);

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutOpen
			(
				&pxAudioDevice->ResourceID.DirectXInterface,
				pxAudioDevice->DeviceID,
				&waveFormat,
				dwCallback,
				dwInstance,
				fdwOpen
			);

			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	pxAudioDevice->Type = pxAudioDeviceType;

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			result = waveInClose((HWAVEIN)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutClose((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	pxAudioDevice->ResourceID.DirectXInterface = PXNull;

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDeviceLoad(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
{
	//pxAudioDevice->FormatTag = pxSound->mod

	pxAudioDevice->Channels = pxSound->NumerOfChannels;
	pxAudioDevice->SamplesPerSecound = pxSound->SampleRate;
	pxAudioDevice->AverageBytesPerSecound = pxSound->SampleRate / pxSound->DataSize;
	pxAudioDevice->BlockAlignSize = pxSound->BlockAllign;
	pxAudioDevice->BitsPerSample = pxSound->BitsPerSample;

	MMRESULT result;

	WAVEHDR waveHeader;
	const UINT waveHeaderSize = sizeof(waveHeader);
	waveHeader.lpData = pxSound->Data;
	waveHeader.dwBufferLength = pxSound->DataSize;
	waveHeader.dwBytesRecorded = 0;
	waveHeader.dwUser = 0;
	waveHeader.dwFlags = 0;
	waveHeader.dwLoops = 0;
	waveHeader.lpNext = 0;
	waveHeader.reserved = 0;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			// Prepare
			{
				const MMRESULT prepareResultID = waveInPrepareHeader((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, &waveHeader, waveHeaderSize);
				const PXActionResult prepareResult = PXAudioConvertMMResult(prepareResultID);

				PXActionReturnOnError(prepareResult);
			}

			// Write Data
			{
				return PXActionInvalid;
			}

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			// Prepare
			{
				const MMRESULT prepareResultID = waveOutPrepareHeader((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, &waveHeader, waveHeaderSize);
				const PXActionResult prepareResult = PXAudioConvertMMResult(prepareResultID);

				PXActionReturnOnError(prepareResult);
			}

			// Write Data
			{
				const MMRESULT writeResultID = waveOutWrite((HWAVEOUT*)pxAudioDevice->ResourceID.DirectXInterface, &waveHeader, waveHeaderSize);
				const PXActionResult writeResult = PXAudioConvertMMResult(writeResultID);

				PXActionReturnOnError(writeResult);
			}
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceBreakLoop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{
			
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutBreakLoop((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			MMTIME time;
			UINT xx = 0;

			result = waveOutGetPosition((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, &time, xx);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDevicePlaybackRateGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const playbackRate)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutGetPlaybackRate((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, playbackRate);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDevicePlaybackRateSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U playbackRate)
{
	MMRESULT result;

	switch (pxAudioDevice->Type)
	{
		case PXAudioDeviceTypeInput:
		{

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			result = waveOutSetPlaybackRate((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, playbackRate);
			break;
		}

		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult audioResult = PXAudioConvertMMResult(result);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	//assert(amount > 0.0f);

	pxAudioDevice->Pitch += amount;

	if (pxAudioDevice->Pitch > PitchMaximum)
	{
		pxAudioDevice->Pitch = PitchMaximum;
	}
}

PXActionResult PXAPI PXMultiMediaDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT pitchResultID = waveOutSetPitch((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, pitch);
	const PXActionResult pitchResult = PXAudioConvertMMResult(pitchResultID);

	return pitchResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXMultiMediaDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	//assert(amount > 0.0f);

	pxAudioDevice->Pitch -= amount;

	if (pxAudioDevice->Pitch < PitchMinimum)
	{
		pxAudioDevice->Pitch = PitchMinimum;
	}
}

PXActionResult PXAPI PXMultiMediaDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
	DWORD volumeDW = 0;

	const MMRESULT volumeResultID = waveOutGetVolume((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, &volumeDW);
	const PXActionResult audioResult = PXAudioConvertMMResult(volumeResultID);
	const PXBool successful = PXActionSuccessful == audioResult;

	if (!successful)
	{
		*volume = -1;
		return audioResult;
	}

	*volume = volumeDW;

	return PXActionSuccessful;
}

PXActionResult PXAPI PXMultiMediaDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
	MMRESULT volumeResultID;

	switch (pxAudioDevice->Type)
	{
		//case PXAudioDeviceTypeInput:
		//	volumeResultID = waveInSetVolume((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, volume);
		//	break;

		case PXAudioDeviceTypeOutput:
			volumeResultID = waveOutSetVolume((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, volume);
			break;

		default:
			return PXActionRefusedArgumentInvalid;
	}
	
	const PXActionResult audioResult = PXAudioConvertMMResult(volumeResultID);

	return audioResult;
}

PXActionResult PXAPI PXMultiMediaDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	const unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
	const PXActionResult audioResult = PXMultiMediaDeviceVolumeSetEqual(pxAudio, pxAudioDevice, volumeCombined);

	return audioResult;
}

#endif
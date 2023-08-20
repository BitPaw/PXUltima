#include "PXAudio.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#if OSUnix
// include audio lib
#elif PXOSWindowsDestop




#if WindowsAtleast8 && PXAudioSystemWindowsXAudioEnable
#include <xaudio2.h> // minimum windows 8?


#pragma comment(lib, "XAUDIO2_9.LIB") 
//#pragma comment(lib, "XAUDIO2_10.LIB") 
#pragma comment(lib, "Xaudio2.lib") // Library: Windows XAudio
#pragma comment(lib, "SetupApi.lib")

#endif


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
		case MMSYSERR_NOMEM:return PXActionFailedMemoryAllocation;
		case MMSYSERR_NOTSUPPORTED: return PXActionRefusedNotSupported;
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

		case WAVERR_BADFORMAT: return PXActionRefusedFormatNotSupported;
		case WAVERR_STILLPLAYING: return AudioResultDeviceIsStillPlaying;
		case WAVERR_UNPREPARED: return AudioResultReaderIsNotPrepared;
		case WAVERR_SYNC: return AudioResultDeviceIsSynchronous;

		default:
			return PXActionInvalid;
	}
}
#endif

void PXAudioConstruct(PXAudio* const pxAudio)
{
	PXObjectClear(PXAudio, pxAudio);
}

void PXAudioDestruct(PXAudio* const pxAudio)
{

}

PXActionResult PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem)
{
	PXObjectClear(PXAudio, pxAudio);

	pxAudio->System = pxAudioSystem;


#if PXAudioSystemWindowsXAudioEnable
	IXAudio2* xAudio = (IXAudio2*)pxAudio->XAudioInterface;

	// Setup COM pbject
	{
		const HRESULT hr = CoInitializeEx(PXNull, COINIT_MULTITHREADED);
		const PXBool success = FAILED(hr);

		if (!success)
			return PXActionInvalid;
	}

	// Create API interface 
	{
		const HRESULT hr = XAudio2Create(&xAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
		const PXBool success = FAILED(hr);

		if (!success)
			return PXActionInvalid;
	}

	// Output
	{
		const HRESULT getCountResult = xAudio->lpVtbl->GetDeviceCount(xAudio, &pxAudio->DeviceOutputAmount);
		const HRESULT getCountResult = xAudio->lpVtbl->GetDeviceCount(xAudio, &pxAudio->DeviceInputAmount);


		pxAudio->DeviceOutputList = PXMemoryAllocateTypeCleared(PXAudioDevice, pxAudio->DeviceOutputAmount);

		for (PXInt32U i = 0; i < pxAudio->DeviceOutputAmount; ++i)
		{
			XAUDIO2_DEVICE_DETAILS xAudioDeviceDetails;
			PXAudioDevice* const pxAudioDeviceCurrent = &pxAudio->DeviceOutputList[i];

			const HRESULT getDataResult = xAudio->lpVtbl->GetDeviceDetails(xAudio, i, &xAudioDeviceDetails);

			PXTextCopyWU(xAudioDeviceDetails.DeviceID, 256, pxAudioDeviceCurrent->DeviceID, 256);
			PXTextCopyWU(xAudioDeviceDetails.DisplayName, 256, pxAudioDeviceCurrent->DisplayName, 256);
			
			pxAudioDeviceCurrent->Role = xAudioDeviceDetails.Role;

			pxAudioDeviceCurrent->FormatTag = xAudioDeviceDetails.OutputFormat.Format.wFormatTag;
			pxAudioDeviceCurrent->Channels = xAudioDeviceDetails.OutputFormat.Format.nChannels;
			pxAudioDeviceCurrent->SamplesPerSec = xAudioDeviceDetails.OutputFormat.Format.nSamplesPerSec;
			pxAudioDeviceCurrent->AvgBytesPerSec = xAudioDeviceDetails.OutputFormat.Format.nAvgBytesPerSec;
			pxAudioDeviceCurrent->BlockAlign = xAudioDeviceDetails.OutputFormat.Format.nBlockAlign;
			pxAudioDeviceCurrent->BitsPerSample = xAudioDeviceDetails.OutputFormat.Format.wBitsPerSample;
		}
	}

	{
		const HRESULT hr = xAudio->lpVtbl->CreateMasteringVoice
		(
			xAudio,
			&(IXAudio2MasteringVoice*)pxAudio->XAudioMasterVoice,
			XAUDIO2_DEFAULT_CHANNELS,
			XAUDIO2_DEFAULT_SAMPLERATE,
			0,
			0,
			0,
			0
		);
	}
#endif

	return PXActionSuccessful;
}

PXActionResult PXAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
#if OSUnix
	return PXActionRefusedNotSupported;
#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{

			switch (pxAudioDeviceType)
			{
				case AudioDeviceTypeInput:
				{
					*amount = waveInGetNumDevs();
					break;
				}
				case AudioDeviceTypeOutput:
				{
					*amount = waveOutGetNumDevs();
					break;
				}

				default:
					return InvalidParameter;
			}
	
			return PXActionSuccessful;
		}
#endif
		default:
			return PXActionRefusedNotSupported;
	}	
#else
	return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
	return PXActionRefusedNotSupported;
#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			pxAudioDevice->DeviceID = deviceID;

			switch (pxAudioDeviceType)
			{
				case AudioDeviceTypeInput:
				{
					const UINT size = sizeof(WAVEINCAPSW);
					WAVEINCAPSW waveInCapsW;

					pxAudioDevice->Type = AudioDeviceTypeInput;

					result = waveInGetDevCapsW(deviceID, &waveInCapsW, size);

					pxAudioDevice->ManufacturerID = waveInCapsW.wMid;
					pxAudioDevice->ProductID = waveInCapsW.wPid;
					pxAudioDevice->DriverVersionMajor = HIBYTE(waveInCapsW.vDriverVersion);
					pxAudioDevice->DriverVersionMinor = LOBYTE(waveInCapsW.vDriverVersion);
					pxAudioDevice->Channels = waveInCapsW.wChannels;

					PXTextCopyWU(waveInCapsW.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, SoundDeviceProductName);

					break;
				}
				case AudioDeviceTypeOutput:
				{
					const UINT size = sizeof(WAVEOUTCAPSW);
					WAVEOUTCAPSW waveOutCap;

					pxAudioDevice->Type = AudioDeviceTypeOutput;

					result = waveOutGetDevCapsW(deviceID, &waveOutCap, size);

					pxAudioDevice->ManufacturerID = waveOutCap.wMid;
					pxAudioDevice->ProductID = waveOutCap.wPid;
					pxAudioDevice->DriverVersionMajor = HIBYTE(waveOutCap.vDriverVersion);
					pxAudioDevice->DriverVersionMinor = LOBYTE(waveOutCap.vDriverVersion);
					pxAudioDevice->Channels = waveOutCap.wChannels;

					PXTextCopyWU(waveOutCap.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, SoundDeviceProductName);
					break;
				}

				default:
					return InvalidParameter;
			}

			{
				const PXActionResult audioResult = PXAudioConvertMMResult(result);

				PXActionReturnOnError(audioResult);
			}		

			return PXActionSuccessful;
		}
#endif
		default:
			return PXActionRefusedNotSupported;
	}
#else
	return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAudioDeviceInputAmount(PXAudio* const pxAudio, PXInt32U* const amount)
{
#if OSUnix
	return PXActionRefusedNotSupported;
#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			*amount = waveInGetNumDevs();
			break;
		}
#endif
		default:
			return PXActionRefusedNotSupported;
	}

	return PXActionSuccessful;

#else
	return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAudioDeviceInputFetch(PXAudio* const pxAudio, PXInt32U* const deviceID, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
	return PXActionRefusedNotSupported;
#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			const UINT size = sizeof(WAVEINCAPSW);
			WAVEINCAPSW wAVEOUTCAPSW;

			PXObjectClear(PXAudioDevice, pxAudioDevice);

			{
				const MMRESULT result = waveInGetDevCapsW(deviceID, &wAVEOUTCAPSW, size);
				const PXActionResult audioResult = PXAudioConvertMMResult(result);

				PXActionReturnOnError(audioResult);
			}

			pxAudioDevice->DeviceID = deviceID;
			pxAudioDevice->Type = AudioDeviceTypeOutput;
			pxAudioDevice->ManufacturerID = wAVEOUTCAPSW.wMid;
			pxAudioDevice->ProductID = wAVEOUTCAPSW.wPid;
			pxAudioDevice->DriverVersionMajor = HIBYTE(wAVEOUTCAPSW.vDriverVersion);
			pxAudioDevice->DriverVersionMinor = LOBYTE(wAVEOUTCAPSW.vDriverVersion);
			pxAudioDevice->Channels = wAVEOUTCAPSW.wChannels;

			PXTextCopyWU(wAVEOUTCAPSW.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, SoundDeviceProductName);

			return PXActionSuccessful;
		}
#endif
		default:
			return PXActionRefusedNotSupported;
}
#else
	return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAudioDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioSource, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{


	pxAudioSource->DeviceID = deviceID;
	pxAudioSource->Type = pxAudioDeviceType;
	pxAudioSource->Pitch = 0.4f;
	pxAudioSource->Volume = 0.02f;
	pxAudioSource->Looping = PXTrue;

	

	PXAudioDeviceFetch(pxAudio, pxAudioDeviceType, deviceID, pxAudioSource);


#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows


	DWORD_PTR dwCallback = 0;
	DWORD_PTR dwInstance = 0;
	DWORD fdwOpen = CALLBACK_NULL;

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;// pxAudioSource->F;         /* format type */
	waveFormat.nChannels = pxAudioSource->Channels;          /* number of channels (i.e. mono, stereo...) */
	waveFormat.nSamplesPerSec = pxAudioSource->SamplesPerSecound;     /* sample rate */
	waveFormat.nAvgBytesPerSec = pxAudioSource->AverageBytesPerSecound;    /* for buffer estimation */
	waveFormat.nBlockAlign = pxAudioSource->BlockAlignSize;        /* block size of data */
	waveFormat.wBitsPerSample = pxAudioSource->BitsPerSample;     /* number of bits per sample of mono data */
	waveFormat.cbSize = sizeof(WAVEFORMATEX);

	///aveFormat.wFormatTag = WAVE_FORMAT_PCM;
	//waveFormat.nChannels = 2;
	//waveFormat.nSamplesPerSec = 44100;
	//waveFormat.wBitsPerSample = 8;


	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			const HRESULT result = ((IXAudio2*)pxAudio->XAudioInterface)->lpVtbl->CreateSourceVoice
			(
				(IXAudio2*)pxAudio->XAudioInterface,
				&(IXAudio2SourceVoice*)pxAudioSource->Xinterface,
				&waveFormat,
				0,
				XAUDIO2_MAX_FREQ_RATIO,
				0,
				0,
				0
			);
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioSource->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInOpen
					(
						&pxAudioSource->Handle,
						pxAudioSource->DeviceID,
						&waveFormat,
						dwCallback,
						dwInstance,
						fdwOpen
					);

					break;
				}
				case AudioDeviceTypeOutput:
				{
					result = waveOutOpen
					(
						&pxAudioSource->Handle,
						pxAudioSource->DeviceID,
						&waveFormat,
						dwCallback,
						dwInstance,
						fdwOpen
					);

					break;
				}

				default:
					return InvalidParameter;
			}		

			const PXActionResult audioResult = PXAudioConvertMMResult(result);

			return audioResult;
		}
		default:
		{
			return InvalidParameter;
		}
	}
#endif

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			((IXAudio2SourceVoice*)pxAudioSource->Xinterface)->lpVtbl->DestroyVoice((IXAudio2SourceVoice*)pxAudioSource->Xinterface);

			pxAudioSource->Xinterface = PXNull;

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioDevice->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInClose(*(HWAVEOUT*)&pxAudioDevice->Handle);
					break;
				}
				case AudioDeviceTypeOutput:
				{
					result = waveOutClose(*(HWAVEOUT*)&pxAudioDevice->Handle);
					break;
				}

				default:
					return InvalidParameter;
			}
		
			const PXActionResult audioResult = PXAudioConvertMMResult(result);

			pxAudioDevice->Handle.HandleID = PXNull;

			return audioResult;
		}
		default:
		{
			return InvalidParameter;
		}
	}
#endif

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDeviceLoad(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
	//pxAudioDevice->FormatTag = pxSound->mod
	pxAudioDevice->Channels = pxSound->NumerOfChannels;
	pxAudioDevice->SamplesPerSecound = pxSound->SampleRate;
	pxAudioDevice->AverageBytesPerSecound = pxSound->SampleRate / pxSound->DataSize;
	pxAudioDevice->BlockAlignSize = pxSound->BlockAllign;
	pxAudioDevice->BitsPerSample = pxSound->BitsPerSample;

	PXAudioDeviceOpen(pxAudio, pxAudioDevice, pxAudioDeviceType, deviceID);

#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			XAUDIO2_BUFFER xAudioBuffer;
			XAUDIO2_BUFFER_WMA xAudioBufferWMA;

			const HRESULT result = ((IXAudio2SourceVoice*)pxAudioSource->Xinterface)->lpVtbl->SubmitSourceBuffer
			(
				(IXAudio2SourceVoice*)pxAudioSource->Xinterface,
				&xAudioBuffer,
				&xAudioBufferWMA
			);

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
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
				case AudioDeviceTypeInput:
				{
					// Prepare
					{
						const MMRESULT prepareResultID = waveInPrepareHeader(*(HWAVEOUT*)&pxAudioDevice->Handle, &waveHeader, waveHeaderSize);
						const PXActionResult prepareResult = PXAudioConvertMMResult(prepareResultID);

						PXActionReturnOnError(prepareResult);
					}

					// Write Data
					{
						return PXActionInvalid;
					}

					break;
				}
				case AudioDeviceTypeOutput:
				{
					// Prepare
					{
						const MMRESULT prepareResultID = waveOutPrepareHeader(*(HWAVEOUT*)&pxAudioDevice->Handle, &waveHeader, waveHeaderSize);
						const PXActionResult prepareResult = PXAudioConvertMMResult(prepareResultID);

						PXActionReturnOnError(prepareResult);
					}

					// Write Data
					{
						const MMRESULT writeResultID = waveOutWrite(*(HWAVEOUT*)&pxAudioDevice->Handle, &waveHeader, waveHeaderSize);
						const PXActionResult writeResult = PXAudioConvertMMResult(writeResultID);

						PXActionReturnOnError(writeResult);
					}					
					break;
				}

				default:
					return InvalidParameter;
			}

			return PXActionSuccessful;
		}
		default:
		{
			return InvalidParameter;
		}
	}
#endif

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			const HRESULT result = ((IXAudio2SourceVoice*)pxAudioSource->Xinterface)->lpVtbl->Start((IXAudio2SourceVoice*)pxAudioSource->Xinterface, 0, 0);

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioDevice->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInStart(*(HWAVEOUT*)&pxAudioDevice->Handle);
					break;
				}
				case AudioDeviceTypeOutput:
				{
					result = waveOutRestart(*(HWAVEOUT*)&pxAudioDevice->Handle);
					break;
				}
				default:
					return InvalidParameter;
			}

			return PXActionSuccessful;
		}
		default:
		{
			return InvalidParameter;
		}
	}
#endif

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioEnable
		case PXAudioSystemWindowsXAudio:
		{
			const HRESULT result = ((IXAudio2SourceVoice*)pxAudioSource->Xinterface)->lpVtbl->Stop((IXAudio2SourceVoice*)pxAudioSource->Xinterface, 0, 0);

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaEnable
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioDevice->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInStop(*(HWAVEOUT*)&pxAudioDevice->Handle);
					break;
				}
				case AudioDeviceTypeOutput:
				{
					result = waveOutPause(*(HWAVEOUT*)&pxAudioDevice->Handle);				
					break;
				}

				default:
					return InvalidParameter;
			}

			return PXActionSuccessful;
		}
		default:
		{
			return InvalidParameter;
		}
	}
#endif

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	//assert(amount > 0.0f);

	pxAudioDevice->Pitch += amount;

	if (pxAudioDevice->Pitch > PitchMaximum)
	{
		pxAudioDevice->Pitch = PitchMaximum;
	}
}

PXActionResult PXAudioDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	//assert(amount > 0.0f);

	pxAudioDevice->Pitch -= amount;

	if (pxAudioDevice->Pitch < PitchMinimum)
	{
		pxAudioDevice->Pitch = PitchMinimum;
	}
}

PXActionResult PXAudioDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	DWORD volumeDW = 0;

	const MMRESULT volumeResultID = waveOutGetVolume(*(HWAVEOUT*)&pxAudioDevice->Handle, &volumeDW);
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

PXActionResult PXAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT volumeResultID = waveOutSetVolume(*(HWAVEOUT*)&pxAudioDevice->Handle, volume);
	const PXActionResult audioResult = PXAudioConvertMMResult(volumeResultID);

	return audioResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const unsigned int volumeCombined = (volumeLeft << 16) | volumeRight;
	const PXActionResult audioResult = PXAudioDeviceVolumeSetEqual(pxAudio, pxAudioDevice, volumeCombined);

	return audioResult;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAudioDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT pitchResultID = waveOutSetPitch(*(HWAVEOUT*)&pxAudioDevice->Handle, pitch);
	const PXActionResult pitchResult = PXAudioConvertMMResult(pitchResultID);

	return pitchResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDevicePlaybackRateSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT playbackRateResultID = waveOutSetPlaybackRate(*(HWAVEOUT*)&pxAudioDevice->Handle, pitch);
	const PXActionResult playbackRateResult = PXAudioConvertMMResult(playbackRateResultID);

	return playbackRateResult;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
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
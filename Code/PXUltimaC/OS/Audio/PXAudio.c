#include "PXAudio.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#define PXAudioSystemWindowsMultimediaCanUse OSWindows && PXAudioSystemWindowsMultimediaEnabled
#define PXAudioSystemWindowsKernalMixerCanUse OSWindows && PXAudioSystemWindowsKernalMixerEnabled
#define PXAudioSystemWindowsDirectSoundCanUse OSWindows && PXAudioSystemWindowsDirectSoundEnable
#define PXAudioSystemWindowsXAudioCanUse WindowsAtleast8 && PXAudioSystemWindowsXAudioEnable

#if OSUnix
// include audio lib
#elif PXOSWindowsDestop

#if PXAudioSystemWindowsXAudioCanUse
#include <initguid.h>
#include <xaudio2.h> // minimum windows 8?
//#pragma comment(lib, "XAUDIO2_9.LIB") 
//#pragma comment(lib, "XAUDIO2_10.LIB") 
#pragma comment(lib, "Xaudio2.lib") // Library: Windows XAudio
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "SetupApi.lib")
#endif

#if PXAudioSystemWindowsDirectSoundCanUse
#include <dsound.h> // Windows Direct Sound
#pragma comment(lib, "dsound.lib") 
#endif

#if PXAudioSystemWindowsMultimediaCanUse
#if WindowsAtleastVista
#include <mmeapi.h> // Header should be found under XP build but it is not.
#else 
#include <MMSystem.h> // alternavive header of MMEAPI.h?
#endif
#pragma comment(lib, "winmm.lib") // Library: Windows Multimedia 
#endif
#endif

#if PXAudioSystemWindowsMultimediaCanUse
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

#if OSWindows
PXActionResult PXAudioConvertFromHandleResultID(const HRESULT handleResult)
{
	switch (handleResult)
	{
		case S_OK:
			return PXActionSuccessful;
			
		case S_FALSE:
			return PXActionSuccessful;

		case RPC_E_CHANGED_MODE:
			return PXActionRedundantAlreadyInitialized;

			// The call succeeded, but we had to substitute the 3D algorithm
		case DS_NO_VIRTUALIZATION:
			return PXActionSuccessWithSubstitution;

			// The call failed because resources (such as a priority level) were already being used by another caller
		case DSERR_ALLOCATED:
			return PXActionRefuedObjectAlreadyExists;

			// The control (vol, pan, etc.) requested by the caller is not available
		case DSERR_CONTROLUNAVAIL:
			return PXActionRefuedObjectPropertyNotAvailable;

			// An invalid parameter was passed to the returning function
		case DSERR_INVALIDPARAM:
			return PXActionRefuedParameterInvalid;

			// This call is not valid for the current state of this object
		case DSERR_INVALIDCALL:
			return PXActionRefuedObjectStateDoesNotAllowAction;

			// An undetermined error occurred inside the DirectSound subsystem
		case DSERR_GENERIC:
			return PXActionFailedUnkownError;

			// The caller does not have the priority level required for the function to succeed
		case DSERR_PRIOLEVELNEEDED:
			return PXActionDeniedPriorityTooLow;

			// Not enough free memory is available to complete the operation
		case DSERR_OUTOFMEMORY:
			return PXActionFailedMemoryAllocation;

			// The specified WAVE format is not supported
		case DSERR_BADFORMAT:
			return PXActionRefuedFormatIllegal;

			// The function called is not supported at this time
		case DSERR_UNSUPPORTED:
			return PXActionRefuedFormatNotSupported;

			// No sound driver is available for use
		case DSERR_NODRIVER:
			return PXActionRefuedDriverMissing;

			// This object is already initialized
		case DSERR_ALREADYINITIALIZED:
			return PXActionRefuedObjectAlreadyInizialized;

			// This object does not support aggregation
		case DSERR_NOAGGREGATION:
			return PXActionRefuedObjectAggregationNotSupported;

			// The buffer memory has been lost, and must be restored
		case DSERR_BUFFERLOST:
			return PXActionFailedBufferRefernceLost;

			// Another app has a higher priority level, preventing this call from succeeding
		case DSERR_OTHERAPPHASPRIO:
			return PXActionYieldToOtherProcessWithHigherPriority;

			// This object has not been initialized
		case DSERR_UNINITIALIZED:
			return PXActionRefuedObjectNotInizialized;

			// The requested COM interface is not available
		case DSERR_NOINTERFACE:
			return PXActionRefuedObjectInterfaceNotAvailable;

			// Access is denied
		case DSERR_ACCESSDENIED:
			return PXActionDeniedNoPermission;

			// Tried to create a DSBCAPS_CTRLFX buffer shorter than DSBSIZE_FX_MIN milliseconds
		case DSERR_BUFFERTOOSMALL:
			return PXActionRefuedBufferSizeTooSmal;

			// Attempt to use DirectSound 8 functionality on an older DirectSound object
		case DSERR_DS8_REQUIRED:
			return PXActionRefusedLibraryRequiredUpgrade;

			// A circular loop of send effects was detected
		case DSERR_SENDLOOP:
			return PXActionCanceledLoopDeteced;

			// The GUID specified in an audiopath file does not match a valid MIXIN buffer
		case DSERR_BADSENDBUFFERGUID:
			return PXActionRefuedObjectIDInvalid;

			// The object requested was not found (numerically equal to DMUS_E_NOT_FOUND)
		case DSERR_OBJECTNOTFOUND:
			return PXActionRefuedObjectIDInvalid;

			// The effects requested could not be found on the system, or they were found
			// but in the wrong order, or in the wrong hardware/software locations.
		case DSERR_FXUNAVAILABLE:
			return PXActionRefusedEffectNotAvailable;

		default:
			return PXActionInvalid;
	}
}

BOOL PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXInt32U* const amount)
{
	if (!guid)
	{
		return PXTrue;
	}

	++(*amount);

	return PXTrue;
}

BOOL PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDevice* const pxAudioDevice)
{
	if (!guid) // First device gets listed twice for no reason. GUID is NULL for first element.
	{
		return PXTrue;
	}

	PXClear(PXAudioDevice, pxAudioDevice);

	PXTextCopyA(cstrDescription, PXTextUnkownLength, pxAudioDevice->DeviceName, 256);
	PXTextCopyA(cstrDescription, PXTextUnkownLength, pxAudioDevice->DisplayName, 256);

	return PXTrue;
}
#endif

void PXAudioConstruct(PXAudio* const pxAudio)
{
	PXClear(PXAudio, pxAudio);
}

void PXAudioDestruct(PXAudio* const pxAudio)
{

}

PXActionResult PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem)
{
	PXClear(PXAudio, pxAudio);

	pxAudio->System = pxAudioSystem;

	switch (pxAudioSystem)
	{
		case PXAudioSystemWindowsMultiMedia:
		{
			// No initializing needed
			break;
		}
		case PXAudioSystemWindowsKernalMixer:
		{
			break;
		}
#if PXAudioSystemWindowsDirectSoundCanUse
		case PXAudioSystemWindowsDirectSound:
		{
			IDirectSound* directSound = PXNull;

			{
				const HRESULT crateResultID = DirectSoundCreate(PXNull, (LPDIRECTSOUND*)&pxAudio->DirectSoundInterface, PXNull); // [Deprecated] dsound.h
				const PXActionResult crateResult = PXAudioConvertFromHandleResultID(crateResultID);

				PXActionReturnOnError(crateResult);

				directSound = (LPDIRECTSOUND)pxAudio->DirectSoundInterface;
			}

			{
				const HRESULT initResultID = directSound->lpVtbl->Initialize(directSound, PXNull);
				const PXActionResult initResult = PXAudioConvertFromHandleResultID(initResultID);

				PXActionReturnOnError(initResult);
			}

			/// Capability
			{
				DSCAPS capability;

				const HRESULT capResultID = directSound->lpVtbl->GetCaps(directSound, &capability);
				const PXActionResult initResult = PXAudioConvertFromHandleResultID(capResultID);

				PXActionReturnOnError(initResult);
			}

			//DirectSoundCaptureCreate();

			break;
		}
#endif

#if PXAudioSystemWindowsXAudioCanUse
		case PXAudioSystemWindowsXAudio:
		{
			IXAudio2* const xAudio = (IXAudio2*)pxAudio->XAudioInterface;

			// Setup COM pbject
			{
				const HRESULT initializeResultID = CoInitializeEx(PXNull, COINIT_MULTITHREADED);
				const PXActionResult initializeResult = PXAudioConvertFromHandleResultID(initializeResultID);

				PXActionReturnOnError(initializeResult);
			}

			// Create API interface 
			{
				const HRESULT createResultID = XAudio2Create(&xAudio, 0, XAUDIO2_DEFAULT_PROCESSOR); //  Xaudio2.lib, xaudio2.h
				const PXActionResult createResult = PXAudioConvertFromHandleResultID(createResultID);

				PXActionReturnOnError(createResult);
			}

			// Output
			{
				const HRESULT getCountResult = xAudio->lpVtbl->GetDeviceCount(xAudio, &pxAudio->DeviceOutputAmount);
				//const HRESULT getCountResult = xAudio->lpVtbl->GetDeviceCount(xAudio, &pxAudio->DeviceInputAmount);

				pxAudio->DeviceOutputList = PXNewList(PXAudioDevice, pxAudio->DeviceOutputAmount);

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
					pxAudioDeviceCurrent->SamplesPerSecound = xAudioDeviceDetails.OutputFormat.Format.nSamplesPerSec;
					pxAudioDeviceCurrent->AverageBytesPerSecound= xAudioDeviceDetails.OutputFormat.Format.nAvgBytesPerSec;
					pxAudioDeviceCurrent->BlockAlignSize = xAudioDeviceDetails.OutputFormat.Format.nBlockAlign;
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

			break;
		}
#endif
		default:
			break;
	}

	return PXActionSuccessful;
}

PXActionResult PXAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
	switch (pxAudioDeviceType)
	{
		case AudioDeviceTypeInput:
		{

#if OSUnix
			return PXActionRefusedNotSupported;
#elif OSWindows

			switch (pxAudio->System)
			{
#if PXAudioSystemWindowsXAudioCanUse
				case PXAudioSystemWindowsXAudio:
				{
					const HRESULT enumResultID = DirectSoundCaptureEnumerateA(PXAudioDeviceDetectAmountCallBack, amount);
					const PXActionResult enumResult = PXAudioConvertFromHandleResultID(enumResultID);

					PXActionReturnOnError(enumResult);

					return PXActionSuccessful;
				}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
				case PXAudioSystemWindowsMultiMedia:
				{
					*amount = waveInGetNumDevs();

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
		case AudioDeviceTypeOutput:
		{
#if OSUnix
			return PXActionRefusedNotSupported;
#elif OSWindows

			switch (pxAudio->System)
			{
#if PXAudioSystemWindowsXAudioCanUse
				case PXAudioSystemWindowsXAudio:
				{
					const HRESULT enumResultID = DirectSoundEnumerateA(PXAudioDeviceDetectAmountCallBack, amount);
					const PXActionResult enumResult = PXAudioConvertFromHandleResultID(enumResultID);

					PXActionReturnOnError(enumResult);

					return PXActionSuccessful;
				}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
				case PXAudioSystemWindowsMultiMedia:
				{
					*amount = waveOutGetNumDevs();

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

		default:
			return PXActionRefusedArgumentInvalid;
	}
}

PXActionResult PXAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
	return PXActionRefusedNotSupported;
#elif OSWindows

	switch (pxAudio->System)
	{
#if PXAudioSystemWindowsXAudioCanUse
		case PXAudioSystemWindowsXAudio:
		{
			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			pxAudioDevice->DeviceID = deviceID;

			switch (pxAudioDeviceType)
			{
				case AudioDeviceTypeInput:
				{
					const UINT size = sizeof(WAVEINCAPS);
					WAVEINCAPS waveInCapsA;

					pxAudioDevice->Type = AudioDeviceTypeInput;

					result = waveInGetDevCapsA(deviceID, &waveInCapsA, size);

					pxAudioDevice->ManufacturerID = waveInCapsA.wMid;
					pxAudioDevice->ProductID = waveInCapsA.wPid;
					pxAudioDevice->DriverVersionMajor = HIBYTE(waveInCapsA.vDriverVersion);
					pxAudioDevice->DriverVersionMinor = LOBYTE(waveInCapsA.vDriverVersion);
					pxAudioDevice->Channels = waveInCapsA.wChannels;

					PXTextCopyA(waveInCapsA.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, SoundDeviceProductName);

					break;
				}
				case AudioDeviceTypeOutput:
				{
					const UINT size = sizeof(WAVEOUTCAPS);
					WAVEOUTCAPS waveOutCapA;

					pxAudioDevice->Type = AudioDeviceTypeOutput;

					result = waveOutGetDevCapsW(deviceID, &waveOutCapA, size);

					pxAudioDevice->ManufacturerID = waveOutCapA.wMid;
					pxAudioDevice->ProductID = waveOutCapA.wPid;
					pxAudioDevice->DriverVersionMajor = HIBYTE(waveOutCapA.vDriverVersion);
					pxAudioDevice->DriverVersionMinor = LOBYTE(waveOutCapA.vDriverVersion);
					pxAudioDevice->Channels = waveOutCapA.wChannels;

					PXTextCopyA(waveOutCapA.szPname, MAXPNAMELEN, pxAudioDevice->DeviceName, SoundDeviceProductName);
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
#endif
		default:
			return PXActionRefusedNotSupported;
	}
#else
	return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAudioDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
	for (PXSize i = 0; i < amount; ++i)
	{
		const PXActionResult fetchResult = PXAudioDeviceFetch(pxAudio, pxAudioDeviceType, i, &pxAudioDevice[i]);

		PXActionReturnOnError(fetchResult);
	}

	return PXActionSuccessful;
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
#if PXAudioSystemWindowsXAudioCanUse
		case PXAudioSystemWindowsXAudio:
		{
			const HRESULT result = ((IXAudio2*)pxAudio->XAudioInterface)->lpVtbl->CreateSourceVoice
			(
				(IXAudio2*)pxAudio->XAudioInterface,
				&(IXAudio2SourceVoice*)pxAudioSource->ResourceID.DirectXInterface,
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

#if PXAudioSystemWindowsMultimediaCanUse
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioSource->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInOpen
					(
						&pxAudioSource->ResourceID.DirectXInterface,
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
						&pxAudioSource->ResourceID.DirectXInterface,
						pxAudioSource->DeviceID,
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

			return audioResult;
		}
#endif
		default:
		{
			return PXActionRefusedArgumentInvalid;
		}
	}

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
#if PXAudioSystemWindowsXAudioCanUse
		case PXAudioSystemWindowsXAudio:
		{
			IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

			audio2SourceVoice->lpVtbl->DestroyVoice(audio2SourceVoice);

			pxAudioDevice->ResourceID.DirectXInterface = PXNull;

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioDevice->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInClose((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
					break;
				}
				case AudioDeviceTypeOutput:
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
#endif
		default:
		{
			return PXActionRefusedArgumentInvalid;
		}
	}


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
#if PXAudioSystemWindowsXAudioCanUse
		case PXAudioSystemWindowsXAudio:
		{
			IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

			XAUDIO2_BUFFER xAudioBuffer;
			XAUDIO2_BUFFER_WMA xAudioBufferWMA;

			const HRESULT result = audio2SourceVoice->lpVtbl->SubmitSourceBuffer
			(
				audio2SourceVoice,
				&xAudioBuffer,
				&xAudioBufferWMA
			);

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
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
				case AudioDeviceTypeOutput:
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
		default:
		{
			return PXActionRefusedArgumentInvalid;
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
#if PXAudioSystemWindowsXAudioCanUse
		case PXAudioSystemWindowsXAudio:
		{
			IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

			const HRESULT result = audio2SourceVoice->lpVtbl->Start(audio2SourceVoice, 0, 0);

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioDevice->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInStart((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
					break;
				}
				case AudioDeviceTypeOutput:
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
#endif
		default:
		{
			return PXActionRefusedArgumentInvalid;
		}
	}

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
			IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

			const HRESULT result = audio2SourceVoice->lpVtbl->Stop(audio2SourceVoice, 0, 0);

			break;
		}
#endif

#if PXAudioSystemWindowsMultimediaCanUse
		case PXAudioSystemWindowsMultiMedia:
		{
			MMRESULT result;

			switch (pxAudioDevice->Type)
			{
				case AudioDeviceTypeInput:
				{
					result = waveInStop((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface);
					break;
				}
				case AudioDeviceTypeOutput:
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
#endif
		default:
		{
			return PXActionRefusedArgumentInvalid;
		}
	}

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
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
#if OSUnix
	return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
	const MMRESULT volumeResultID = waveOutSetVolume((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, volume);
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
	const MMRESULT pitchResultID = waveOutSetPitch((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, pitch);
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
	const MMRESULT playbackRateResultID = waveOutSetPlaybackRate((HWAVEOUT)pxAudioDevice->ResourceID.DirectXInterface, pitch);
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
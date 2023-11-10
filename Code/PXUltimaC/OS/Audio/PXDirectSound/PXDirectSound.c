#include "PXDirectSound.h"

#if PXAudioSystemWindowsDirectSoundEnable

#include <Windows.h>
#include <mmeapi.h>
#include <dsound.h> // Windows Direct Sound
#pragma comment(lib, "dsound.lib") 
#pragma comment(lib, "dxguid.lib")

#define PXDirectSound8Use DIRECTSOUND_VERSION >= 0x0800

#if PXDirectSound8Use
#define PXDirectSoundInputInterface IDirectSoundCapture8
#define PXDirectSoundOutputInterface IDirectSound8
#define PXDirectSoundInputCreate DirectSoundCaptureCreate8
#define PXDirectSoundOutputCreate DirectSoundCreate8
#define PXDirectSoundBuffer IDirectSoundBuffer8
#else
#define PXDirectSoundInputInterface IDirectSoundCapture
#define PXDirectSoundOutputInterface IDirectSound
#define PXDirectSoundInputCreate DirectSoundCaptureCreate8
#define PXDirectSoundOutputCreate DirectSoundCreate
#define PXDirectSoundBuffer IDirectSoundBuffer
#endif


BOOL CALLBACK PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXInt32U* const amount)
{
	if (!guid)
	{
		return PXTrue;
	}

	++(*amount);

	return PXTrue;
}

BOOL CALLBACK PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDeviceDetectObjectEventData* const pxAudioDeviceDetectObjectEventData)
{
	if (!guid) // First device gets listed twice for no reason. GUID is NULL for first element.
	{
		return PXTrue;
	}

	if (pxAudioDeviceDetectObjectEventData->IndexCurrent > pxAudioDeviceDetectObjectEventData->IndexTarget)
	{
		return PXTrue;
	}

	if (pxAudioDeviceDetectObjectEventData->IndexCurrent != pxAudioDeviceDetectObjectEventData->IndexTarget)
	{
		++pxAudioDeviceDetectObjectEventData->IndexCurrent;
		return PXTrue;
	}
	PXAudioDevice* const pxAudioDevice = pxAudioDeviceDetectObjectEventData->AudioDevice;

	PXClear(PXAudioDevice, pxAudioDevice);

	pxAudioDevice->DeviceGUID = *guid;

	PXTextCopyA(cstrDescription, PXTextUnkownLength, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);

	return PXFalse;
}


PXActionResult PXAPI PXDirectSoundInitialize(PXAudio* const pxAudio)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
	HRESULT enumResultID;

	*amount = 0;

	switch (pxAudioDeviceType)
	{
		case PXAudioDeviceTypeInput:
		{
			enumResultID = DirectSoundCaptureEnumerateA(PXAudioDeviceDetectAmountCallBack, amount);
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			enumResultID = DirectSoundEnumerateA(PXAudioDeviceDetectAmountCallBack, amount);
			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult enumResult = PXWindowsHandleErrorFromID(enumResultID);

	PXActionReturnOnError(enumResult);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
	PXAudioDeviceDetectObjectEventData pxAudioDeviceDetectObjectEventData;
	pxAudioDeviceDetectObjectEventData.IndexCurrent = 0;
	pxAudioDeviceDetectObjectEventData.IndexTarget = deviceID;
	pxAudioDeviceDetectObjectEventData.AudioDevice = pxAudioDevice;

	HRESULT enumResultID;

	switch (pxAudioDeviceType)
	{
		case PXAudioDeviceTypeInput:
		{
			enumResultID = DirectSoundCaptureEnumerateA(PXAudioDeviceDetectObjectCallBack, &pxAudioDeviceDetectObjectEventData);
			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			enumResultID = DirectSoundEnumerateA(PXAudioDeviceDetectObjectCallBack, &pxAudioDeviceDetectObjectEventData);
			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}

	const PXActionResult enumResult = PXWindowsHandleErrorFromID(enumResultID);

	PXActionReturnOnError(enumResult);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
	for (size_t i = 0; i < amount; i++)
	{
		PXDirectSoundDeviceFetch(pxAudio, pxAudioDeviceType, i, &pxAudioDevice[i]);
	}

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
	pxAudioDevice->Type = pxAudioDeviceType;

	switch (pxAudioDeviceType)
	{
		case PXAudioDeviceTypeInput:
		{
			IDirectSoundCapture8* directSoundCapture;

			const HRESULT crateResultID = PXDirectSoundInputCreate(PXNull, (IDirectSoundCapture8**)&pxAudio->DirectSoundInterface, PXNull); // dsound.h		
			const PXActionResult crateResult = PXWindowsHandleErrorFromID(crateResultID);

			PXActionReturnOnError(crateResult);

			directSoundCapture = (IDirectSoundCapture8*)pxAudio->DirectSoundInterface;

			DSCCAPS capabiltys;
			PXClear(DSCCAPS, &capabiltys);
			capabiltys.dwSize = sizeof(DSCCAPS);

			directSoundCapture->lpVtbl->GetCaps(directSoundCapture, &capabiltys);

			pxAudioDevice->SupportFlags = capabiltys.dwFlags;
			pxAudioDevice->FormatSupportFlags = capabiltys.dwFormats;
			pxAudioDevice->Channels = capabiltys.dwChannels;

			break;
		}
		case PXAudioDeviceTypeOutput:
		{
			PXDirectSoundOutputInterface* directSound = PXNull;

			{
				const HRESULT crateResultID = PXDirectSoundOutputCreate(PXNull, (PXDirectSoundOutputInterface**)&pxAudio->DirectSoundInterface, PXNull); // [Deprecated] dsound.h
				const PXActionResult crateResult = PXWindowsHandleErrorFromID(crateResultID);

				PXActionReturnOnError(crateResult);

				directSound = (PXDirectSoundOutputInterface*)pxAudio->DirectSoundInterface;
			}

			/// Capability
			{
				DSCAPS capabiltys;
				PXClear(DSCAPS,&capabiltys);
				capabiltys.dwSize = sizeof(DSCAPS);

				const HRESULT capResultID = directSound->lpVtbl->GetCaps(directSound, &capabiltys);
				const PXActionResult initResult = PXWindowsHandleErrorFromID(capResultID);

				PXActionReturnOnError(initResult);

				pxAudioDevice->SupportFlags = capabiltys.dwFlags;
	
			}

			{
				HWND windowHandle = GetForegroundWindow();
				
				if (windowHandle == NULL)
				{
					windowHandle = GetDesktopWindow();
				}

				directSound->lpVtbl->SetCooperativeLevel(directSound, windowHandle, DSSCL_PRIORITY);
			}		

			break;
		}
		default:
			return PXActionRefusedArgumentInvalid;
	}

	return PXActionSuccessful;	
}

PXActionResult PXAPI PXDirectSoundDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceLoad(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
	PXDirectSoundOutputInterface* const directSound = (PXDirectSoundOutputInterface*)pxAudio->DirectSoundInterface;
	PXDirectSoundBuffer* soundBuffer = PXNull;
	const PXBool canUse3DStuff = pxSound->NumerOfChannels == 1u; // DirectSoundBuffer3D can only use MONO sounds 

	// Create buffer to load data into
	{		
		

		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nChannels = pxSound->NumerOfChannels;
		waveFormat.nSamplesPerSec = pxSound->SampleRate;
		waveFormat.nAvgBytesPerSec = pxSound->ByteRate;
		waveFormat.nBlockAlign = pxSound->BlockAllign;
		waveFormat.wBitsPerSample = pxSound->BitsPerSample;
		waveFormat.cbSize = sizeof(WAVEFORMATEX);


		DSBUFFERDESC pcDSBufferDesc;
		pcDSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
		pcDSBufferDesc.dwFlags = 
			//DSBCAPS_PRIMARYBUFFER |
			DSBCAPS_STATIC |
			//DSBCAPS_CTRLPAN |
			//DSBCAPS_CTRLVOLUME |
			DSBCAPS_CTRLFREQUENCY |
			//DSBCAPS_CTRLPOSITIONNOTIFY |
			DSBCAPS_GLOBALFOCUS; // Prevent the music from stopping if you TAB out;

		if (canUse3DStuff)
		{
			pcDSBufferDesc.dwFlags |=
				DSBCAPS_CTRL3D | // Only works with MONO sound
				DSBCAPS_GETCURRENTPOSITION2;
				//DSBCAPS_PRIMARYBUFFER; // Add position capability
		}

		pcDSBufferDesc.dwBufferBytes = pxSound->DataSize; // DSBSIZE_MIN, DSBSIZE_MAX  
		pcDSBufferDesc.dwReserved = 0;
		pcDSBufferDesc.lpwfxFormat = &waveFormat;
#if DIRECTSOUND_VERSION >= 0x0700
		pcDSBufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
		//PXClear(GUID, &pcDSBufferDesc.guid3DAlgorithm);
#endif

		

		directSound->lpVtbl->Initialize(directSound, &pxAudioDevice->DeviceGUID);


		const HRESULT createResultID = directSound->lpVtbl->CreateSoundBuffer
		(
			directSound,
			&pcDSBufferDesc,
			&(IDirectSoundBuffer*)pxAudioDevice->SoundBuffer,
			PXNull
		);
		const PXActionResult createResult = PXWindowsHandleErrorFromID(createResultID);

		PXActionReturnOnError(createResult);

		soundBuffer = pxAudioDevice->SoundBuffer;
	}

	PXSize dataOffset = 0;
	void* dataBlockAdressA = 0;
	void* dataBlockAdressB = 0;
	DWORD dataBlockSizeA = 0;
	DWORD dataBlockSizeB = 0;
	DWORD flags = 0;
	// Lock directSound buffer so we are allowed to write to it.
	{
		const HRESULT lockResultID = soundBuffer->lpVtbl->Lock(soundBuffer, dataOffset, pxSound->DataSize, &dataBlockAdressA, &dataBlockSizeA, &dataBlockAdressB, dataBlockSizeB, flags);
		const PXActionResult lockResult = PXWindowsHandleErrorFromID(lockResultID);

		PXActionReturnOnError(lockResult);
	}

	// Write sound data to actual soundBuffer.
	{
		PXCopyList(PXByte, dataBlockSizeA, pxSound->Data, dataBlockAdressA);

		if (dataBlockAdressB)
		{
			PXCopyList(PXByte, dataBlockSizeB, ((PXByte*)pxSound->Data) + dataBlockSizeA, dataBlockAdressB);
		}
	}

	// Unlock buffer to release it back to DirectSound
	{
		const HRESULT lockResultID = soundBuffer->lpVtbl->Unlock(soundBuffer, dataBlockAdressA, dataBlockSizeA, dataBlockAdressB, dataBlockSizeB);
		const PXActionResult lockResult = PXWindowsHandleErrorFromID(lockResultID);

		PXActionReturnOnError(lockResult);
	}

	//{
		IDirectSound3DListener8* listener;

		const HRESULT listenResultID = soundBuffer->lpVtbl->QueryInterface(soundBuffer, &IID_IDirectSound3DListener, &pxAudioDevice->Listen3DInterface);
		const PXActionResult lockResult = PXWindowsHandleErrorFromID(listenResultID);

		//listener->lpVtbl->SetPosition

		//listener->lpVtbl->

		//PXActionReturnOnError(lockResult);
	//}

		if (canUse3DStuff)
		{
			const HRESULT bufferResultID = soundBuffer->lpVtbl->QueryInterface(soundBuffer, &IID_IDirectSound3DBuffer, &pxAudioDevice->Buffer3DInterface);
			const PXActionResult bufferResult = PXWindowsHandleErrorFromID(bufferResultID);
		}




	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlayCursorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXSize offset)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	const HRESULT setResultID = soundBuffer->lpVtbl->SetCurrentPosition(soundBuffer, offset);
	const PXActionResult setResult = PXWindowsHandleErrorFromID(setResultID);

	PXActionReturnOnError(setResult);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlayCursorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSize* const offset)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		DWORD positionRead = 0;
		DWORD positionWrite = 0;

		const HRESULT getResultID = soundBuffer->lpVtbl->GetCurrentPosition(soundBuffer, &positionRead, &positionWrite);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);

		*offset = positionRead;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlaySpeedSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		const HRESULT getResultID = soundBuffer->lpVtbl->SetFrequency(soundBuffer, frequency);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlaySpeedGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		DWORD frequencyDW = 0;

		const HRESULT getResultID = soundBuffer->lpVtbl->GetFrequency(soundBuffer, &frequencyDW);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);

		*frequency = frequencyDW;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePositionSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

	if (!directSound3DBuffer)
	{
		return PXActionRefuedObjectInterfaceNotAvailable;
	}

	const HRESULT getResultID = directSound3DBuffer->lpVtbl->SetPosition(directSound3DBuffer,x, y, z, DS3D_IMMEDIATE);
	const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

	PXActionReturnOnError(getResult);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePositionGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

	if (!directSound3DBuffer)
	{
		return PXActionRefuedObjectInterfaceNotAvailable;
	}

	D3DVECTOR vector;

	const HRESULT getResultID = directSound3DBuffer->lpVtbl->GetPosition(directSound3DBuffer, &vector);
	const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

	PXActionReturnOnError(getResult);

	*x = vector.x;
	*y = vector.y;
	*z = vector.z;

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceConeAnglesSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeAnglesGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		LONG volumeL = 0;

		const HRESULT getResultID = soundBuffer->lpVtbl->GetVolume(soundBuffer, &volumeL);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);

		*volume = volumeL;
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		const HRESULT getResultID = soundBuffer->lpVtbl->SetVolume(soundBuffer, 0, 0, DSBPLAY_LOOPING);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		const HRESULT getResultID = soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

	if (!soundBuffer)
	{
		return PXActionRefusedArgumentInvalid;
	}

	{
		DWORD frequencyDW = 0;

		const HRESULT getResultID = soundBuffer->lpVtbl->Stop(soundBuffer);
		const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

		PXActionReturnOnError(getResult);
	}

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMaxDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMinGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMinSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float flMinDistance)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceModeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceModeSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceVelocityGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

	if (!directSound3DBuffer)
	{
		return PXActionRefuedObjectInterfaceNotAvailable;
	}

	D3DVECTOR vector;

	const HRESULT getResultID = directSound3DBuffer->lpVtbl->GetVelocity(directSound3DBuffer, &vector);
	const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

	PXActionReturnOnError(getResult);

	*x = vector.x;
	*y = vector.y;
	*z = vector.z;

	return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOrientationGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOrientationSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDeferredSettingsCommit(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceVelocitySet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z)
{
	if (!pxAudio || !pxAudioDevice)
	{
		return PXActionRefusedArgumentNull;
	}

	IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

	if (!directSound3DBuffer)
	{
		return PXActionRefuedObjectInterfaceNotAvailable;
	}

	const HRESULT getResultID = directSound3DBuffer->lpVtbl->SetVelocity(directSound3DBuffer, x, y, z, DS3D_DEFERRED);
	const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

	PXActionReturnOnError(getResult);

	return PXActionSuccessful;
}
#endif
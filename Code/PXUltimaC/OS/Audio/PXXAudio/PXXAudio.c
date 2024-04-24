#include "PXXAudio.h"


#if PXAudioSystemWindowsXAudioEnable && 1
#include <initguid.h>
#include <xaudio2.h> // minimum windows 8?
//#pragma comment(lib, "XAUDIO2_9.LIB") 
//#pragma comment(lib, "XAUDIO2_10.LIB") 
#pragma comment(lib, "Xaudio2.lib") // Library: Windows XAudio
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "SetupApi.lib")


PXActionResult PXAPI PXXAudioInitialize(PXAudioXSystem* const pxAudioXSystem, PXAudioInitializeInfo* const pxAudioInitializeInfo)
{
	// Setup COM pbject
	{
		const HRESULT initializeResultID = CoInitializeEx(PXNull, COINIT_MULTITHREADED);
		const PXActionResult initializeResult = PXWindowsHandleErrorFromID(initializeResultID);

		PXActionReturnOnError(initializeResult);
	}

	// Create API interface 
	{
		const HRESULT createResultID = XAudio2Create(&pxAudioXSystem->XAudioInterface, 0, XAUDIO2_DEFAULT_PROCESSOR); //  Xaudio2.lib, xaudio2.h
		const PXActionResult createResult = PXWindowsHandleErrorFromID(createResultID);

		PXActionReturnOnError(createResult);
	}

	// Output
	{
		/*/
		

		pxAudio->DeviceOutputList = PXNewList(PXAudioDevice, pxAudio->DeviceOutputAmount);

		for (PXInt32U i = 0; i < pxAudio->DeviceOutputAmount; ++i)
		{
			XAUDIO2_DEVICE_DETAILS xAudioDeviceDetails;
			PXAudioDevice* const pxAudioDeviceCurrent = &pxAudio->DeviceOutputList[i];

			const HRESULT getDataResult = xAudio->lpVtbl->GetDeviceDetails(xAudio, i, &xAudioDeviceDetails);

			//PXTextCopyWU(xAudioDeviceDetails.DeviceID, 256, pxAudioDeviceCurrent->DeviceID, 256);
			PXTextCopyWU(xAudioDeviceDetails.DisplayName, 256, pxAudioDeviceCurrent->DeviceName, 256);

			pxAudioDeviceCurrent->Role = xAudioDeviceDetails.Role;

			pxAudioDeviceCurrent->FormatTag = xAudioDeviceDetails.OutputFormat.Format.wFormatTag;
			pxAudioDeviceCurrent->Channels = xAudioDeviceDetails.OutputFormat.Format.nChannels;
			pxAudioDeviceCurrent->SamplesPerSecound = xAudioDeviceDetails.OutputFormat.Format.nSamplesPerSec;
			pxAudioDeviceCurrent->AverageBytesPerSecound = xAudioDeviceDetails.OutputFormat.Format.nAvgBytesPerSec;
			pxAudioDeviceCurrent->BlockAlignSize = xAudioDeviceDetails.OutputFormat.Format.nBlockAlign;
			pxAudioDeviceCurrent->BitsPerSample = xAudioDeviceDetails.OutputFormat.Format.wBitsPerSample;
		}
		*/
	}

	{
		/*
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
		
		*/
	}

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceAmount(PXAudioXSystem* const pxAudioXSystem, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo)
{
	if(!(pxAudioXSystem && pxAudioDeviceAmountInfo))
	{
		return PXActionRefusedArgumentNull;
	}

	pxAudioDeviceAmountInfo->DeviceInput = 0;
	pxAudioDeviceAmountInfo->DeviceOutput = 0;

	if(!pxAudioXSystem->XAudioInterface)
	{
		return PXActionRefuedObjectNotInizialized;
	}

	const HRESULT getCountResult = pxAudioXSystem->XAudioInterface->lpVtbl->GetDeviceCount(pxAudioXSystem->XAudioInterface, &pxAudioDeviceAmountInfo->DeviceOutput);
	

	return PXActionSuccessful;;
}

PXActionResult PXAPI PXXAudioDeviceFetch(PXAudioXSystem* const pxAudioXSystem, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceFetchAll(PXAudioXSystem* const pxAudioXSystem, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceOpen(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
	IXAudio2SourceVoice* audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	/*
	const HRESULT result = pxAudioXSystem->XAudioInterface->lpVtbl->CreateSourceVoice
	(
		pxAudioXSystem->XAudioInterface,
		&audio2SourceVoice,
		&waveFormat,
		0,
		XAUDIO2_MAX_FREQ_RATIO,
		0,
		0,
		0
	);
	*/

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceClose(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
	IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	audio2SourceVoice->lpVtbl->DestroyVoice(audio2SourceVoice);

	pxAudioDevice->ResourceID.DirectXInterface = PXNull;

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceLoad(PXAudioXSystem* const pxAudioXSystem,	PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
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

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePitchIncrease(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePitchSet(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePitchReduce(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceVolumeGet(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceVolumeSetEqual(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceVolumeSetIndividual(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceStart(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
	IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	const HRESULT result = audio2SourceVoice->lpVtbl->Start(audio2SourceVoice, 0, 0);

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceStop(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
	IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	const HRESULT result = audio2SourceVoice->lpVtbl->Stop(audio2SourceVoice, 0, 0);

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePause(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

#endif
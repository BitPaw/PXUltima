#include "PXXAudio.h"


#if PXAudioSystemWindowsXAudioEnable
#include <initguid.h>
#include <xaudio2.h> // minimum windows 8?
//#pragma comment(lib, "XAUDIO2_9.LIB") 
//#pragma comment(lib, "XAUDIO2_10.LIB") 
#pragma comment(lib, "Xaudio2.lib") // Library: Windows XAudio
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "SetupApi.lib")


PXActionResult PXAPI PXXAudioInitialize(PXAudio* const pxAudio)
{
	IXAudio2* xAudio = PXNull;

	// Setup COM pbject
	{
		const HRESULT initializeResultID = CoInitializeEx(PXNull, COINIT_MULTITHREADED);
		const PXActionResult initializeResult = PXWindowsHandleErrorFromID(initializeResultID);

		PXActionReturnOnError(initializeResult);
	}

	// Create API interface 
	{
		const HRESULT createResultID = XAudio2Create((IXAudio2**)&pxAudio->XAudioInterface, 0, XAUDIO2_DEFAULT_PROCESSOR); //  Xaudio2.lib, xaudio2.h
		const PXActionResult createResult = PXWindowsHandleErrorFromID(createResultID);

		PXActionReturnOnError(createResult);

		xAudio = (IXAudio2*)pxAudio->XAudioInterface;
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

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceAmount(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXInt32U* const amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceFetch(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceFetchAll(PXAudio* const pxAudio, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceOpen(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
#if 0
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
#endif

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceClose(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	audio2SourceVoice->lpVtbl->DestroyVoice(audio2SourceVoice);

	pxAudioDevice->ResourceID.DirectXInterface = PXNull;

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceLoad(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
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

PXActionResult PXAPI PXXAudioDevicePitchIncrease(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePitchSet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePitchReduce(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, float amount)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceVolumeGet(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceVolumeSetEqual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceVolumeSetIndividual(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceStart(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	const HRESULT result = audio2SourceVoice->lpVtbl->Start(audio2SourceVoice, 0, 0);

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDeviceStop(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

	const HRESULT result = audio2SourceVoice->lpVtbl->Stop(audio2SourceVoice, 0, 0);

	return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXXAudioDevicePause(PXAudio* const pxAudio, PXAudioDevice* const pxAudioDevice)
{
	return PXActionRefusedNotImplemented;
}

#endif
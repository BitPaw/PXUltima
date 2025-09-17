#include "PXCoreAudio.h"

#if OSUnix
#elif OSWindows

#define INITGUID
#include <windows.h>
#include <initguid.h>       // Must come first to define the IIDs, unresolved references otherwise
#include <mmdeviceapi.h>
#include <audioclient.h>    // Windows Vista (+UWP)
#include <xaudio2.h> // minimum windows 8?

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "avrt.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "SetupApi.lib")

//#pragma comment(lib, "XAUDIO2_9.LIB")
//#pragma comment(lib, "XAUDIO2_10.LIB")
#pragma comment(lib, "Xaudio2.lib") // Library: Windows XAudio

#endif



// Shit solution
// CLSID_MMDeviceEnumerator
// IID_IMMDeviceEnumerator
// IID_IAudioClient
// IID_IAudioRenderClient
#if 1
DEFINE_GUID(CLSID_MMDeviceEnumerator, 0xbcde0395, 0xe52f, 0x467c, 0x8e, 0x3d, 0xc4, 0x57, 0x92, 0x91, 0x69, 0x2e);
DEFINE_GUID(IID_IMMDeviceEnumerator, 0xa95664d2, 0x9614, 0x4f35, 0xa7, 0x46, 0xde, 0x8d, 0xb6, 0x36, 0x17, 0xe6);
DEFINE_GUID(IID_IAudioClient, 0x1cb9ad4c, 0xdbfa, 0x4c32, 0xb1, 0x78, 0xc2, 0xf5, 0x8c, 0x4c, 0x6e, 0x2b);
DEFINE_GUID(IID_IAudioRenderClient, 0xf294acfc, 0x3146, 0x4483, 0xa7, 0xbf, 0xad, 0xdc, 0xa7, 0xc2, 0x60, 0xe2);
#endif




PXCoreAudio _pxCoreAudio;

PXResult PXAPI  PXCoreAudioInitialize(PXAudio* const pxAudio)
{
    PXClear(PXCoreAudio, &_pxCoreAudio);

    // Open Library
    {

    }

    // Load functions
    {

    }

    // Link functions
    {
     
    }


    HRESULT resultID = 0;


    // Setup COM object
    {
       // const HRESULT initializeResultID = CoInitialize(NULL); // Use this or line below?
        resultID = CoInitializeEx(PXNull, COINIT_MULTITHREADED);
        const PXActionResult initializeResult = PXErrorFromHRESULT(resultID);

        PXActionReturnOnError(initializeResult);
    }


    //const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    //const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    //const IID IID_IAudioClient = __uuidof(IAudioClient);
    //const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);
  

    // Get default audio device
    {
        resultID = CoCreateInstance
        (
            &CLSID_MMDeviceEnumerator,
            NULL,
            CLSCTX_ALL,
            &IID_IMMDeviceEnumerator,
            (void**)&_pxCoreAudio.AudioDeviceEnumerator
        );

        resultID = _pxCoreAudio.AudioDeviceEnumerator->lpVtbl->GetDefaultAudioEndpoint
        (
            _pxCoreAudio.AudioDeviceEnumerator,
            eRender, 
            eConsole,
            &_pxCoreAudio.AudioDevice
        );
    }



    // Create XAudio2 API interface
    {
        resultID = XAudio2Create(&_pxCoreAudio.XAudio2API, 0, XAUDIO2_DEFAULT_PROCESSOR); //  Xaudio2.lib, xaudio2.h
        const PXActionResult createResult = PXErrorFromHRESULT(resultID);

        PXActionReturnOnError(createResult);
    }

 
    // Output
    {
        /*/


        pxAudio->DeviceOutputList = PXNewList(PXAudioDevice, pxAudio->DeviceOutputAmount);

        for (PXI32U i = 0; i < pxAudio->DeviceOutputAmount; ++i)
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









    resultID = _pxCoreAudio.AudioDevice->lpVtbl->Activate
    (
        _pxCoreAudio.AudioDevice,
        &IID_IAudioClient, 
        CLSCTX_ALL,
        NULL,
        (void**)&_pxCoreAudio.AudioClient
    );



    WAVEFORMATEX* pwfx = NULL;
    resultID = _pxCoreAudio.AudioClient->lpVtbl->GetMixFormat(_pxCoreAudio.AudioClient, &pwfx);

    REFERENCE_TIME hnsBufferDuration = 10000000; // 1 second

    resultID = _pxCoreAudio.AudioClient->lpVtbl->Initialize
    (
        _pxCoreAudio.AudioClient,
        AUDCLNT_SHAREMODE_SHARED, 
        0,
        hnsBufferDuration,
        0, 
        pwfx,
        NULL
    );


    IAudioRenderClient* pRenderClient = NULL;

    UINT32 bufferFrameCount;
    resultID = _pxCoreAudio.AudioClient->lpVtbl->GetBufferSize(_pxCoreAudio.AudioClient, &bufferFrameCount);
    resultID = _pxCoreAudio.AudioClient->lpVtbl->GetService(_pxCoreAudio.AudioClient, &IID_IAudioRenderClient, (void**)&pRenderClient);

    BYTE* pData;
    double t = 0.0;
    double dt = 1.0 / 1.0;// SAMPLE_RATE;

    resultID = _pxCoreAudio.AudioClient->lpVtbl->Start(_pxCoreAudio.AudioClient);

    while(1) {
        UINT32 numFramesPadding;
        resultID = _pxCoreAudio.AudioClient->lpVtbl->GetCurrentPadding(_pxCoreAudio.AudioClient, &numFramesPadding);
        UINT32 numFramesAvailable = bufferFrameCount - numFramesPadding;

        resultID = pRenderClient->lpVtbl->GetBuffer(pRenderClient, numFramesAvailable, &pData);

        for(UINT32 i = 0; i < numFramesAvailable; i++) {
            float sample = 0;// generate_sample(t);
            PXI16U s = (PXI16U)(sample * 32767);
            ((PXI16U*)pData)[2 * i + 0] = s; // Left
            ((PXI16U*)pData)[2 * i + 1] = s; // Right
            t += dt;
        }

        //printf(".");

        resultID = pRenderClient->lpVtbl->ReleaseBuffer(pRenderClient, numFramesAvailable, 0);
        Sleep(10);
    }

    // Cleanup (not reached in this loop)
    CoTaskMemFree(pwfx);
    pRenderClient->lpVtbl->Release(pRenderClient);
   // pAudioClient->lpVtbl->Release(pAudioClient);
   // pDevice->lpVtbl->Release(pDevice);
   // pEnumerator->lpVtbl->Release(pEnumerator);
    CoUninitialize();


    return PXActionRefusedNotImplemented;
}


/*

PXResult PXAPI  PXXAudioDeviceAmount(PXAudioXSystem* const pxAudioXSystem, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo)
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

PXResult PXAPI  PXXAudioDeviceOpen(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID)
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
    * /

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXXAudioDeviceClose(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
    IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

    audio2SourceVoice->lpVtbl->DestroyVoice(audio2SourceVoice);

    pxAudioDevice->ResourceID.DirectXInterface = PXNull;

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXXAudioDeviceLoad(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
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

PXResult PXAPI  PXXAudioDeviceStart(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
    IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

    const HRESULT result = audio2SourceVoice->lpVtbl->Start(audio2SourceVoice, 0, 0);

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXXAudioDeviceStop(PXAudioXSystem* const pxAudioXSystem, PXAudioDevice* const pxAudioDevice)
{
    IXAudio2SourceVoice* const audio2SourceVoice = (IXAudio2SourceVoice*)pxAudioDevice->ResourceID.DirectXInterface;

    const HRESULT result = audio2SourceVoice->lpVtbl->Stop(audio2SourceVoice, 0, 0);

    return PXActionRefusedNotImplemented;
}

*/
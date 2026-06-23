#include "PXWASAPI.h"

#if OSUnix
#elif OSWindows

//#define INITGUID
#define COBJMACROS
#include <windows.h>
#include <initguid.h>       // Must come first to define the IIDs, unresolved references otherwise

#include <xaudio2.h> // minimum windows 8?


//-------------------------------------
// Windows WASAPI
#include <mmdeviceapi.h> // Device seek
#include <audioclient.h>    // Windows Vista (+UWP)
#include <functiondiscoverykeys_devpkey.h>
//-------------------------------------

//-------------------------------------
// Windows MediaFoundation
#include <mfapi.h>
#include <mfidl.h> 
#include <mfreadwrite.h> 
#include <mferror.h>
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")
//-------------------------------------

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "avrt.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "SetupApi.lib")



//#pragma comment(lib, "XAUDIO2_9.LIB")
//#pragma comment(lib, "XAUDIO2_10.LIB")
#pragma comment(lib, "Xaudio2.lib") // Library: Windows XAudio

#endif

typedef struct PXWASAPI_
{
    IMMDeviceCollection* collection;
    IMMDevice* device;
    DWORD count;


    // Default
    IMMDevice* DefaultInput;
    IMMDevice* DefaultOutput;


    IMMDeviceEnumerator* AudioDeviceEnumerator;
  //  IMMDevice* AudioDevice;

    IAudioClient* AudioClient;
    IAudioRenderClient* AudioClientRender;

    HANDLE EventHandle;

    UINT32 BufferFrames;
    WORD    FormatTag;        /* format type */
    WORD    Channels;         /* number of channels (i.e. mono, stereo...) */
    DWORD   SamplesPerSec;    /* sample rate */
    DWORD   AvgBytesPerSec;   /* for buffer estimation */
    WORD    BlockAlign;       /* block size of data */
    WORD    BitsPerSample;    /* Number of bits per sample of mono data */
}
PXWASAPI;

PXWASAPI _pxWASAPI;

#include <math.h>
#define PI 3.14159265358979323846

// Call this each time WSAPI asks you to fill the buffer
void fill_audio_buffer(float* buffer, int frames, int channels, float frequency, int sampleRate)
{
    static double phase = 0.0;
    double phaseIncrement = 2.0 * PI * frequency / sampleRate;

    for(int i = 0; i < frames; i++) {
        float sample = (float)sin(phase);

        // Write same sample to all channels
        for(int c = 0; c < channels; c++) {
            buffer[i * channels + c] = sample * 0.2f; // 20% volume
        }

        phase += phaseIncrement;
        if(phase >= 2.0 * PI)
            phase -= 2.0 * PI;
    }
}


PXResult PXAPI PXWASAPIInitialize(PXAudioSystem PXREF pxAudioSystem, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo)
{

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
    PXResult result = PXResultInvalid;

    // Setup COM object
    {
        // const HRESULT initializeResultID = CoInitialize(NULL); // Use this or line below?
        resultID = CoInitializeEx(PXNull, COINIT_MULTITHREADED);
        result = PXErrorFromHRESULT(resultID);

        if(PXResultOK != result)
            return result;
    }


    //const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    //const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
    //const IID IID_IAudioClient = __uuidof(IAudioClient);
    //const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);


    //EXTERN_C const IID IID_IAudioClient;
    // {1CB9AD4C-DBFA-4c32-B178-C2F568A703B2}
    const IID IID_IAudioClient = { 0x1CB9AD4C, 0xDBFA, 0x4c32, {0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2} };


    PXWASAPIDeviceListFetch(pxAudioSystem);


    // Activate an IAudioClient on that device
    {
        resultID = _pxWASAPI.DefaultOutput->lpVtbl->Activate
        (
            _pxWASAPI.DefaultOutput,
            &IID_IAudioClient,
            CLSCTX_ALL,
            NULL,
            (void**)&_pxWASAPI.AudioClient
        );
        result = PXErrorFromHRESULT(resultID);
    }

    // Query the device’s preferred mix format
    {
        WAVEFORMATEX* mixFormat = NULL; 
        resultID = _pxWASAPI.AudioClient->lpVtbl->GetMixFormat
        (
            _pxWASAPI.AudioClient, 
            &mixFormat
        );

        _pxWASAPI.FormatTag =  mixFormat->wFormatTag;
        _pxWASAPI.Channels = mixFormat->nChannels;
        _pxWASAPI.SamplesPerSec = mixFormat->nSamplesPerSec;
        _pxWASAPI.AvgBytesPerSec = mixFormat->nAvgBytesPerSec;
        _pxWASAPI.BlockAlign = mixFormat->nBlockAlign;
        _pxWASAPI.BitsPerSample = mixFormat->wBitsPerSample;
                               



        // Initialize the audio client (shared mode, event-driven)
        REFERENCE_TIME bufferDuration = 10000000; // 1 second

        resultID = _pxWASAPI.AudioClient->lpVtbl->Initialize
        (
            _pxWASAPI.AudioClient,
            AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
            bufferDuration,
            0,
            mixFormat,
            NULL
        );

        // Get the buffer size (in frames)
        _pxWASAPI.EventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

        resultID = _pxWASAPI.AudioClient->lpVtbl->SetEventHandle
        (
            _pxWASAPI.AudioClient, 
            _pxWASAPI.EventHandle
        );

        resultID = _pxWASAPI.AudioClient->lpVtbl->GetBufferSize
        (
            _pxWASAPI.AudioClient, 
            &_pxWASAPI.BufferFrames
        );


        // Get the IAudioRenderClient
        resultID = _pxWASAPI.AudioClient->lpVtbl->GetService
        (
            _pxWASAPI.AudioClient,
            &IID_IAudioRenderClient,
            (void**)&_pxWASAPI.AudioClientRender
        );      
        
    }


    PXWASAPIUpload(pxAudioSystem, PXNull);




#if 1
    IAudioRenderClient* render = NULL;

    WAVEFORMATEX* pwfx = NULL;
    UINT32 bufferFrameCount, padding; 
    BYTE* data; 
    double t = 0.0;
    double freq = 440.0; // A4 
    double sampleRate;

        // Get mix format
    resultID = IAudioClient_GetMixFormat(_pxWASAPI.AudioClient, &pwfx);
        if(FAILED(resultID)) return resultID;

        sampleRate = pwfx->nSamplesPerSec;

        /*
        // Initialize shared-mode stream
        resultID = IAudioClient_Initialize(_pxWASAPI.AudioClient,
                               AUDCLNT_SHAREMODE_SHARED,
                               0,
                               0, 0,
                               pwfx,
                               NULL);
        */
        if(FAILED(resultID)) return resultID;

        // Get buffer size
        resultID = IAudioClient_GetBufferSize(_pxWASAPI.AudioClient, &bufferFrameCount);
        if(FAILED(resultID)) return resultID;

        // Get render client
        resultID = IAudioClient_GetService(_pxWASAPI.AudioClient, &IID_IAudioRenderClient,
                                     (void**)&render);
        if(FAILED(resultID)) return resultID;

        // Start audio
        resultID = IAudioClient_Start(_pxWASAPI.AudioClient);
        if(FAILED(resultID)) return resultID;

        // Render loop (~3 seconds)



        PXText pxText;
        PXTextFromAdressW(&pxText, L"T:/TEST/SFX.wav", PXTextLengthUnkown, PXTextLengthUnkown);

        PXBuffer pxBuffer;
        PXClear(PXBuffer, &pxBuffer);
        //PXBufferSet(&pxBuffer, data, numFrames * 10);

        PXWSAPISoundLoad(&pxText, &pxBuffer);



        // Start the audio client
        {
            _pxWASAPI.AudioClient->lpVtbl->Start(_pxWASAPI.AudioClient);
        }


        size_t offset = 0;
        while(offset < pxBuffer.CursorOffsetByte)
        {
            Sleep(500);

            resultID = IAudioClient_GetCurrentPadding(_pxWASAPI.AudioClient, &padding);

           // padding = bufferFrameCount - padding;
            UINT32 framesAvail = bufferFrameCount - padding;
            UINT32 bytesAvail = framesAvail * pwfx->nBlockAlign;
            DWORD remaining = pxBuffer.CursorOffsetByte - offset;

            resultID = IAudioRenderClient_GetBuffer(render, padding, &data);
            if(FAILED(resultID)) break;


          
          //  size_t bytesToCopy = min(padding, pxBuffer.CursorOffsetByte - offset);

            if(!data)
            {
                break;
            }

#if 1

            if(bytesAvail <= remaining)
            {
                memcpy(data, pxBuffer.Data4 + offset, bytesAvail);
                offset += bytesAvail;
            }
            else 
            {
                memcpy(data, pxBuffer.Data4 + offset, remaining);
               // memcpy(data + remaining, pxBuffer.Data, bytesAvail - remaining);
                offset = bytesAvail - remaining; 
            }

       
          //  PXMemoryCopy(&pxBuffer.Data[offset], data, bytesToCopy);
    

            //audioClient->lpVtbl->Start(audioClient);


       

          //  audioClient->lpVtbl->Stop();
            


         
#else
            fill_audio_buffer(data, bytesToCopy, pwfx->nChannels, 80.0f, sampleRate);
#endif

          //  offset += bytesToCopy;

            resultID = IAudioRenderClient_ReleaseBuffer(render, padding, 0);
            if(FAILED(resultID)) break;

        }



        // Start the audio client
        {
           // _pxWASAPI.AudioClient->lpVtbl->Stop(_pxWASAPI.AudioClient);
        }

#endif





    return PXResultOK;
}

PXResult PXAPI PXWASAPIDeviceListFetch(PXAudioSystem PXREF pxAudioSystem)
{
    HRESULT resultID;
    PXResult pxResult;






    // Create the device enumerator
    {
#if PXLanguageCPP
        resultID = CoCreateInstance
        (
            CLSID_MMDeviceEnumerator,
            NULL,
            CLSCTX_ALL,
            IID_IMMDeviceEnumerator,
            (void**)&_pxCoreAudio.AudioDeviceEnumerator
        );
#else
        resultID = CoCreateInstance
        (
            &CLSID_MMDeviceEnumerator,
            NULL,
            CLSCTX_ALL,
            &IID_IMMDeviceEnumerator,
            (void**)&_pxWASAPI.AudioDeviceEnumerator
        );
#endif
        pxResult = PXErrorFromHRESULT(resultID);
    }

    const EDataFlow dataFlowList[] = { eRender, eCapture };
    const PXAudioDeviceType inputTypeList[] = { PXAudioDeviceTypeOutput, PXAudioDeviceTypeInput };
    IMMDevice** deviceList[] = { &_pxWASAPI.DefaultOutput, &_pxWASAPI.DefaultInput};

    for(PXI8U i = 0; i < 2; ++i)
    {
        const EDataFlow dataFlow = dataFlowList[i];
        const PXAudioDeviceType inputType = inputTypeList[i];

        resultID = IMMDeviceEnumerator_EnumAudioEndpoints
        (
            _pxWASAPI.AudioDeviceEnumerator,
            dataFlow,
            DEVICE_STATE_ACTIVE,
            &_pxWASAPI.collection
        );

        resultID = IMMDeviceCollection_GetCount(_pxWASAPI.collection, &_pxWASAPI.count);

        IMMDevice* device = 0;
        IPropertyStore* props;

        for(DWORD i = 0; i < _pxWASAPI.count; i++)
        {
            LPWSTR id = NULL;
            PROPVARIANT name;

            PropVariantInit(&name);

            resultID = IMMDeviceCollection_Item(_pxWASAPI.collection, i, &device);

            if(FAILED(resultID))
                continue;

            IMMDevice_GetId(device, &id);

            resultID = IMMDevice_OpenPropertyStore(device, STGM_READ, &props);

            if(SUCCEEDED(resultID))
            {
                resultID = IPropertyStore_GetValue(props, &PKEY_Device_FriendlyName, &name);

                PXAudioDeviceDetectInfo pxAudioDeviceDetectInfo;
                PXClear(PXAudioDeviceDetectInfo, &pxAudioDeviceDetectInfo);
                PXTextFromAdressW(&pxAudioDeviceDetectInfo.Name, name.pwszVal, PXTextUnkownLength, PXTextUnkownLength);
                pxAudioDeviceDetectInfo.Type = inputType;

                PXAudioDeviceDetect(pxAudioSystem, &pxAudioDeviceDetectInfo);

                PropVariantClear(&name);
                IPropertyStore_Release(props);
            }

            CoTaskMemFree(id);
            IMMDevice_Release(device);
        }




        // Get the default audio output device
        {
#if PXLanguageCPP
            resultID = _pxCoreAudio.AudioDeviceEnumerator->GetDefaultAudioEndpoint
            (
                dataFlow,
                eConsole,
                deviceList[i]
            );
#else
            resultID = _pxWASAPI.AudioDeviceEnumerator->lpVtbl->GetDefaultAudioEndpoint
            (
                _pxWASAPI.AudioDeviceEnumerator,
                dataFlow,
                eConsole,
                deviceList[i]
            );
#endif
            resultID = PXErrorFromHRESULT(resultID);
        }




    }
  
    if(_pxWASAPI.collection)
        IMMDeviceCollection_Release(_pxWASAPI.collection);

   // if(enumerator)
     //   IMMDeviceEnumerator_Release(enumerator);


    // Get default objects








    return PXResultOK;
}

PXResult PXAPI PXWASAPIUpload(PXAudioSystem PXREF pxAudio, PXBuffer PXREF pxBuffer)
{
    IAudioRenderClient* renderClient = _pxWASAPI.AudioClientRender;
    HRESULT resultID = 0;
    PXResult pxResult = PXResultInvalid;

    // Get the address of the device space
    BYTE* deviceBufferAdress = NULL;
    resultID = renderClient->lpVtbl->GetBuffer
    (
        renderClient,
        _pxWASAPI.BufferFrames,
        &deviceBufferAdress
    );
    pxResult = PXErrorFromHRESULT(pxResult);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    if(pxBuffer)
    {
        // Copy from out buffer to the device
        PXMemoryCopy
        (
            deviceBufferAdress, 
            pxBuffer->Adress,
            pxBuffer->SizeAllowedToUse
        );
    }
    else
    {
        // We write zeros
        PXMemoryClear(deviceBufferAdress, _pxWASAPI.BufferFrames * _pxWASAPI.BlockAlign);
    } 

    resultID = renderClient->lpVtbl->ReleaseBuffer
    (
        renderClient, 
        _pxWASAPI.BufferFrames,
        0
    );
    pxResult = PXErrorFromHRESULT(pxResult);

    return pxResult;
}

PXResult PXAPI PXWASAPIFilterApply
(
    PXAudioSystem PXREF pxAudio, 
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioFilterFunction pxAudioFilterFunction,
    void PXREF filterData
)
{
    if(!(pxAudio && pxAudioFilterFunction && pxAudioDevice && filterData))
    {
        return PXResultRefusedParameterNull;
    }

    IAudioRenderClient* renderClient = _pxWASAPI.AudioClientRender;
    HRESULT resultID = 0;
    PXResult pxResult = PXResultInvalid;

    // Get the address of the device space
    BYTE* deviceBufferAdress = NULL;
    resultID = renderClient->lpVtbl->GetBuffer
    (
        renderClient,
        _pxWASAPI.BufferFrames,
        &deviceBufferAdress
    );
    pxResult = PXErrorFromHRESULT(pxResult);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    PXBufferSet(&pxAudioDevice->FilterBuffer, deviceBufferAdress, _pxWASAPI.BufferFrames);

    pxAudioFilterFunction(filterData, &pxAudioDevice->FilterBuffer);

    resultID = renderClient->lpVtbl->ReleaseBuffer
    (
        renderClient,
        _pxWASAPI.BufferFrames,
        0
    );
    pxResult = PXErrorFromHRESULT(pxResult);

    PXBufferSet(&pxAudioDevice->FilterBuffer, 0, 0);

    return pxResult;
}

PXResult PXAPI PXWSAPISoundLoad(PXText PXREF pxText, PXBuffer PXREF pxBuffer)
{
    HRESULT resultID = 0;
    PXResult pxResult = 0;

    return PXEndianInvalid;

    // Initialize COM + Media Foundation
    resultID = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    resultID = MFStartup(MF_VERSION, 0);

    IMFSourceReader* reader = NULL;  

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            wchar_t url[_MAX_PATH];

            PXTextCopyAW(pxText->A, pxText->SizeUsed, url, _MAX_PATH);

            resultID = MFCreateSourceReaderFromURL(url, PXNull, &reader);

            break;
        }
        case TextFormatUNICODE:
        {
            // I/O dependend function!
            resultID = MFCreateSourceReaderFromURL(pxText->W, PXNull, &reader);
            break;
        }
        default:
            break;
    }

    // Request uncompressed PCM output
    IMFMediaType* pcmType = NULL;
    MFCreateMediaType(&pcmType);
    pcmType->lpVtbl->SetGUID(pcmType, &MF_MT_MAJOR_TYPE, &MFMediaType_Audio);
    pcmType->lpVtbl->SetGUID(pcmType, &MF_MT_SUBTYPE, &MFAudioFormat_PCM);
    reader->lpVtbl->SetCurrentMediaType(reader, MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, pcmType);
    pcmType->lpVtbl->Release(pcmType);

    // Get the resulting PCM format
    IMFMediaType* nativeType = NULL;
    reader->lpVtbl->GetCurrentMediaType(reader, MF_SOURCE_READER_FIRST_AUDIO_STREAM, &nativeType);

    WAVEFORMATEX* wfx = NULL;
    UINT32 wfxSize = 0;
    MFCreateWaveFormatExFromMFMediaType(nativeType, &wfx, &wfxSize, 0);
    nativeType->lpVtbl->Release(nativeType);



    for(;;)
    {
        IMFSample* sample = NULL;
        DWORD flags = 0;

        resultID = reader->lpVtbl->ReadSample
        (
            reader,
            MF_SOURCE_READER_FIRST_AUDIO_STREAM, 
            0,
            NULL,
            &flags,
            NULL,
            &sample
        );

        if(flags & MF_SOURCE_READERF_ENDOFSTREAM)
        {
            break;
        }

        if(!sample)
        {
            continue;
        }

     
        IMFMediaBuffer* buffer = NULL;
        resultID = sample->lpVtbl->ConvertToContiguousBuffer(sample, &buffer);

        BYTE* data = NULL;
        DWORD maxLen = 0;
        DWORD curLen = 0;
        resultID = buffer->lpVtbl->Lock(buffer, &data, &maxLen, &curLen);

        // Collect decoded PCM samples
        PXBufferAppend(pxBuffer, data, curLen);

        resultID = buffer->lpVtbl->Unlock(buffer);
        resultID = buffer->lpVtbl->Release(buffer);
        resultID = sample->lpVtbl->Release(sample);
        
    }

    reader->lpVtbl->Release(reader);

    /*
    // === WASAPI PLAYBACK ===

    IMMDeviceEnumerator* enumDev = nullptr;
    IMMDevice* device = nullptr;
    IAudioClient* audioClient = nullptr;
    IAudioRenderClient* renderClient = nullptr;

    CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
                     IID_PPV_ARGS(&enumDev));
    enumDev->lpVtbl->GetDefaultAudioEndpoint(eRender, eConsole, &device);
    device->lpVtbl->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
                     (void**)&audioClient);

    audioClient->lpVtbl->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 0, 0, wfx, nullptr);
    audioClient->lpVtbl->GetService(IID_PPV_ARGS(&renderClient));

    UINT32 bufferFrames = 0;
    audioClient->lpVtbl->GetBufferSize(audioClient, &bufferFrames);

    audioClient->lpVtbl->Start(audioClient);

    size_t offset = 0;
    while(offset < pcmData.size())
    {
        UINT32 padding = 0;
        audioClient->lpVtbl->GetCurrentPadding(&padding);

        UINT32 framesAvail = bufferFrames - padding;
        UINT32 bytesAvail = framesAvail * wfx->nBlockAlign;

        BYTE* buffer = nullptr;
        renderClient->lpVtbl->GetBuffer(framesAvail, &buffer);

        size_t bytesToCopy = min(bytesAvail, pcmData.size() - offset);
        memcpy(buffer, pcmData.data() + offset, bytesToCopy);
        offset += bytesToCopy;

        renderClient->lpVtbl->ReleaseBuffer(framesAvail, 0);
    }

    audioClient->lpVtbl->Stop();

    // Cleanup
    CoTaskMemFree(wfx);
    renderClient->lpVtbl->Release();
    audioClient->lpVtbl->Release();
    device->lpVtbl->Release();
    enumDev->lpVtbl->Release();

    MFShutdown();
    CoUninitialize();

    */

    return PXResultOK;
}

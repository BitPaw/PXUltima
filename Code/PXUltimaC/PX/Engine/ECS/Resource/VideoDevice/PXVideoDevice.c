#include "PXVideoDevice.h"

#if OSWindows
#include <windows.h>

#include <dshow.h> // Direct Show
#include <mfapi.h> // Windows Vista
#include <mfidl.h>
#include <combaseapi.h>
#include <mfreadwrite.h>
#include <shlwapi.h>

//#include <mfobjects.h>
//#pragma comment(lib, "mfidl.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "Shlwapi.lib")
#endif

#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/OS/PXOS.h>


#if OSWindows
#include <Windows.h>
#include <mfapi.h>
#include <mftransform.h>
#endif



#if OSWindows
typedef HRESULT(WINAPI* PXVideoOnReadSampleFunction)(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample);

HRESULT WINAPI QueryInterface(IMFSourceReaderCallback* callBack, REFIID iid, void** ppv)
{
    static const QITAB qit[] =
    {
        { (IID*)&IID_IMFSourceReaderCallback, 0 },
        { 0 },
    };
    return QISearch(callBack, qit, iid, ppv); // Windows 2000, Shlwapi.dll, shlwapi.h
}

HRESULT WINAPI AddRef(IMFSourceReaderCallback* callBack)
{
#if 0
    return InterlockedIncrement(&m_nRefCount);
#endif

    return S_OK;
}

HRESULT WINAPI Release(IMFSourceReaderCallback* callBack)
{
#if 0
    ULONG uCount = InterlockedDecrement(&m_nRefCount);
    if(uCount == 0)
    {
        delete this;
    }
    return uCount;
#endif

    return S_OK;
}



HRESULT WINAPI OnFlush(IMFSourceReaderCallback* This, _In_  DWORD dwStreamIndex)
{
    PXConsoleWrite(0, "EVENT - OnFlush\n");

    return S_OK;
};

HRESULT WINAPI OnEvent(IMFSourceReaderCallback* This, _In_  DWORD dwStreamIndex, _In_  IMFMediaEvent* pEvent)
{
    PXConsoleWrite(0, "EVENT - OnEvent\n");

    return S_OK;
};

HRESULT WINAPI PXVideoOnReadSample(HRESULT hrStatus, DWORD dwStreamIndex, DWORD dwStreamFlags, LONGLONG llTimestamp, IMFSample* pSample)
{
    PXConsoleWrite(0, "EVENT - PXVideoOnReadSample\n");

    return S_OK;
}
#endif

PXResult PXAPI PXVideoCaptureDeviceList()
{
#if OSUnix
#elif OSWindows

    PXVideoDeviceEE* pxVideoDeviceList = PXNull;

    const HRESULT c = CoInitialize(PXNull);

    IMFMediaSource* imfMediaSource;

    IMFAttributes* attributeList = NULL;
    IMFActivate** ppDevices = NULL;

    // Create an attribute store to specify the enumeration parameters.
    {
        const HRESULT hr = MFCreateAttributes(&attributeList, 2); // Windows Vista (+UWP), Mfplat.dll, mfapi.h

        if(FAILED(hr))
        {
            goto done;
        }
    }

    {
        // Windows Vista, Mfuuid.lib, mfobjects.h
        // Source type: video capture devices
        const HRESULT hr = attributeList->lpVtbl->SetGUID
        (
            attributeList,
            &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID // MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_AUDCAP_GUID
        );

        if(FAILED(hr))
        {
            goto done;
        }
    }



    // Enumerate devices.
  //  {
    UINT32 count = 0;
    HRESULT hr = MFEnumDeviceSources(attributeList, &ppDevices, &count); // Windows 7, Mf.dll, mfidl.h

    if(FAILED(hr))
    {
        goto done;
    }

    if(count == 0)
    {
        goto done;
    }


    pxVideoDeviceList = PXMemoryHeapCallocT(PXVideoDeviceEE, count);


    for(PXSize i = 0; i < count; ++i)
    {
        PXVideoDeviceEE* pxVideoDevice = &pxVideoDeviceList[i];
        IMFActivate* device = ppDevices[i];

        const HRESULT wwe = device->lpVtbl->ActivateObject
        (
            device,
            &IID_IMFMediaSource,
            &imfMediaSource
        );

        UINT32 bufferSize = 0;
        wchar_t* buffer = PXNull;

        const HRESULT wweAA = device->lpVtbl->GetAllocatedString
        (
            device,
            &MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
            &buffer,
            &bufferSize
        );

        PXTextCopyWA(buffer, bufferSize, pxVideoDevice->Name, 32);


        const HRESULT aaaaa = device->lpVtbl->GetAllocatedString
        (
            device,
            &MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME,
            &buffer,
            &bufferSize
        );

        const HRESULT bbbbbb = device->lpVtbl->GetAllocatedString
        (
            device,
            &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
            &buffer,
            &bufferSize
        );

        const HRESULT cccccc = attributeList->lpVtbl->SetString
        (
            attributeList,
            &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK,
            buffer
        );

        const HRESULT ddddd = MFCreateDeviceSource(attributeList, &imfMediaSource);

        PXConsoleWrite(0, 0);
    }




    PCWSTR pszURL;
    IMFSourceReaderCallback* pCallback;
    IMFSourceReader* ppReader;


    hr = S_OK;
    IMFAttributes* pAttributes = NULL;

    hr = MFCreateAttributes(&pAttributes, 1);
    if(FAILED(hr))
    {
        goto done;
    }

    IMFSourceReaderCallbackVtbl mbfSourceReaderCallbackVtbl;
    PXClear(IMFSourceReaderCallbackVtbl, &mbfSourceReaderCallbackVtbl);

    IMFSourceReaderCallback imfSourceReaderCallback;
    imfSourceReaderCallback.lpVtbl = &mbfSourceReaderCallbackVtbl;
    imfSourceReaderCallback.lpVtbl->QueryInterface = QueryInterface;
    imfSourceReaderCallback.lpVtbl->AddRef = AddRef;
    imfSourceReaderCallback.lpVtbl->Release = Release;
    imfSourceReaderCallback.lpVtbl->OnReadSample = PXVideoOnReadSample;
    imfSourceReaderCallback.lpVtbl->OnFlush = OnFlush;
    imfSourceReaderCallback.lpVtbl->OnEvent = OnEvent;


    //   IUnknown iUnknown;
     //  PXClear(IUnknown, &iUnknown);
     //  iUnknown.lpVtbl->


    hr = pAttributes->lpVtbl->SetUnknown
    (
        pAttributes,
        &MF_SOURCE_READER_ASYNC_CALLBACK,
        &imfSourceReaderCallback
    );

    if(FAILED(hr))
    {
        goto done;
    }

    hr = MFCreateSourceReaderFromURL(pszURL, pAttributes, &ppReader);







    // Request the first sample.
    hr = ppReader->lpVtbl->ReadSample
    (
        ppReader,
        MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,
        NULL,
        NULL,
        NULL,
        NULL
    );

    PXConsoleWrite(0, 0);

    // }





     // Set the endpoint ID.
    {

    }


    // xxxxxxxxxx
    {
        // const HRESULT haaar = MFCreateDeviceSource(pAttributes, ppSource);
    }










    int x = 1;

    while(1)
    {
        x <<= 1;
    }



done:
    /*
    {
        SafeRelease(&pAttributes);

        for(DWORD i = 0; i < count; i++)
        {
            SafeRelease(&ppDevices[i]);
        }
        CoTaskMemFree(ppDevices);
        SafeRelease(&pSource);
    }
    */

    return PXResultOK;
#else
    return ..;
#endif
}






//# https://learn.microsoft.com/en-us/windows/win32/medfound/enumerating-video-capture-devices
//# https://learn.microsoft.com/en-us/windows/win32/medfound/audio-video-capture-in-media-foundation

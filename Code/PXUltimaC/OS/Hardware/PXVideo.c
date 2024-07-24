#include "PXVideo.h"

#if OSWindows
#include <windows.h>
#include <mfapi.h> // Windows Vista
#include <mfidl.h>
#include <combaseapi.h>

//#include <mfobjects.h>

#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mf.lib")
#endif

PXActionResult PXAPI PXVideoCaptureDeviceList()
{
  
    IMFMediaSource *ppSource = NULL;
    IMFMediaSource* pSource = NULL;
    IMFAttributes* pAttributes = NULL;
    IMFActivate** ppDevices = NULL;

    // Create an attribute store to specify the enumeration parameters.
    {
        const HRESULT hr = MFCreateAttributes(&pAttributes, 1); // Windows Vista (+UWP), Mfplat.dll, mfapi.h

        if(FAILED(hr))
        {
            goto done;
        }
    }


    {
        // Windows Vista, Mfuuid.lib, mfobjects.h
        // Source type: video capture devices
        const HRESULT hr = pAttributes->lpVtbl->SetGUID 
        (
            pAttributes,            
            &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
            &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
        );

        if(FAILED(hr))
        {
            goto done;
        }
    }



    // Enumerate devices.
    {
        UINT32 count = 0;
        HRESULT hr = MFEnumDeviceSources(pAttributes, &ppDevices, &count);

        if(FAILED(hr))
        {
            goto done;
        }

        if(count == 0)
        {
            hr = E_FAIL;
            goto done;
        }


        for(PXSize i = 0; i < count; ++i)
        {
            IMFActivate* device = ppDevices[i];

           // device->lpVtbl->
        }

    }




    // Create the media source object.
    {
        IMFActivate* dev = ppDevices[0];
        const HRESULT hr = 0;// dev->lpVtbl->ActivateObject(dev, IID_PPV_ARGS(&pSource)); // ???

        if(FAILED(hr))
        {
            goto done;
        }

       // *ppSource = pSource;
       // (*ppSource)->AddRef();
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

    return PXActionSuccessful;
}






//# https://learn.microsoft.com/en-us/windows/win32/medfound/enumerating-video-capture-devices
//# https://learn.microsoft.com/en-us/windows/win32/medfound/audio-video-capture-in-media-foundation

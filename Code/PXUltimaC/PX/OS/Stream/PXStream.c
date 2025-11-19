#include "PXStream.h"

#if OSUnix
#elif OSWindows
#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfobjects.h>
#include <shlwapi.h>
#include <stdio.h>

#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mf.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "shlwapi.lib")
#endif

PXI16U _windowHandleAmount = 0;
HWND _windowHandleList[16];

PXI16U _monitorHandleAmount = 0;
PXMonitor _monitorHandleList[16];

BOOL IsAltTabWindow(HWND hwnd)
{
    if(!IsWindowVisible(hwnd))
        return FALSE;

    HWND hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
    HWND hwndWalk = NULL;

    while((hwndWalk = GetLastActivePopup(hwndTry)) != hwndWalk)
    {
        if(IsWindowVisible(hwndWalk)) 
            break;

        hwndTry = hwndWalk;
    }

    return hwndWalk == hwnd;
}



BOOL CALLBACK PXWindowsEnumWindowsProc(HWND hwnd, LPARAM lParam) 
{
    BOOL isOK;

    for(;;)
    {
        isOK = IsWindowVisible(hwnd);

        if(!isOK)
        {
            break;
        }

        isOK = IsAltTabWindow(hwnd);

        if(!isOK)
        {
            break;
        }

        RECT rectangle;
        isOK = GetClientRect(hwnd, &rectangle);

        if(!isOK)
        {
            break;
        }

        LONG width = rectangle.right - rectangle.left;
        LONG height = rectangle.bottom - rectangle.top;

        // Check if the window is a logical size and not 0x0
        isOK = (0 < width) && (0 < height);

        if(!isOK)
        {
            break;
        }

        char title[256];
        int length = GetWindowTextA(hwnd, title, sizeof(title) / sizeof(char));
        char* name = title;

        isOK = length > 0;

        if(length == 0)
        {
            name = "---";
        }

        printf("Window: <%2i> <%4ix%-4i> %s\n", _windowHandleAmount, width, height, title);

        if(!isOK)
        {
            break;
        }

        _windowHandleList[_windowHandleAmount++] = hwnd;

        break;
    }

    return TRUE;
}

BOOL CALLBACK PXWindowsMonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) 
{
    PXMonitor* monitor = &_monitorHandleList[_monitorHandleAmount++];     
    monitor->ID = hMonitor;
    monitor->DeciveContect = hdcMonitor;
    monitor->Left = lprcMonitor->left;
    monitor->Top = lprcMonitor->top;
    monitor->Right = lprcMonitor->right;
    monitor->Bottom = lprcMonitor->bottom;

    return TRUE; // continue enumeration
}

PXResult PXAPI PXWindowListUpdate()
{
    _windowHandleAmount = 0;

    const BOOL res = EnumWindows(PXWindowsEnumWindowsProc, 0);
}

PXResult PXAPI PXMonitorListUpdate()
{
    _monitorHandleAmount = 0;

    const BOOL res = EnumDisplayMonitors(NULL, NULL, PXWindowsMonitorEnumProc, 0);
}


PXResult PXAPI PXStreamCreateWindow(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo)
{
    PXWindowListUpdate();

    // Check if we are out of bounce
    PXBool isValidWindowIndex = _windowHandleAmount > pxStreamOpenInfo->DeviceIndex;

    if(!isValidWindowIndex)
    {
        return PXActionInvalid;
    }

    PXStreamWindow PXREF pxStreamWindow = &pxStream->Window;

    pxStreamWindow->WindowHandle = _windowHandleList[pxStreamOpenInfo->DeviceIndex];

    GetClientRect(pxStreamWindow->WindowHandle, &pxStreamWindow->Rectangle);

    pxStreamWindow->WindowDC = GetDC(pxStreamWindow->WindowHandle);
    pxStreamWindow->MemoryDC = CreateCompatibleDC(pxStreamWindow->WindowDC);

    pxStreamWindow->BitmapHandle = CreateCompatibleBitmap
    (
        pxStreamWindow->WindowDC, 
        pxStreamWindow->Rectangle.right - pxStreamWindow->Rectangle.left,
        pxStreamWindow->Rectangle.bottom - pxStreamWindow->Rectangle.top
    );

    SelectObject(pxStreamWindow->MemoryDC, pxStreamWindow->BitmapHandle);
}

PXResult PXAPI PXStreamCreateMonitor(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo)
{
    PXStreamWindow PXREF pxStreamWindow = &pxStream->Window;

    PXMonitorListUpdate();

    PXMonitor* monitor = &_monitorHandleList[pxStreamOpenInfo->DeviceIndex];
    
    pxStreamWindow->WindowDC = monitor->DeciveContect;

    if(!monitor->DeciveContect)
    {
        pxStreamWindow->WindowDC = GetDC(PXNull);
    }

    pxStreamWindow->MemoryDC = CreateCompatibleDC(pxStreamWindow->WindowDC);
    pxStreamWindow->Rectangle.right = monitor->Right;
    pxStreamWindow->Rectangle.left = monitor->Left;
    pxStreamWindow->Rectangle.bottom = monitor->Bottom;
    pxStreamWindow->Rectangle.top = monitor->Top;

    pxStreamWindow->BitmapHandle = CreateCompatibleBitmap
    (
        pxStreamWindow->WindowDC,
        pxStreamWindow->Rectangle.right - pxStreamWindow->Rectangle.left,
        pxStreamWindow->Rectangle.bottom - pxStreamWindow->Rectangle.top
    );

    SelectObject(pxStreamWindow->MemoryDC, pxStreamWindow->BitmapHandle);

    return PXActionSuccessful;
}















typedef enum PXVideoFormat_
{
    PXVideoFormatInvalid,
    PXVideoFormatBase,
    PXVideoFormatRGB32,
    PXVideoFormatARGB32,
    PXVideoFormatRGB24,
    PXVideoFormatRGB555,
    PXVideoFormatRGB565,
    PXVideoFormatRGB8,
    PXVideoFormatL8,
    PXVideoFormatL16,
    PXVideoFormatD16,
    PXVideoFormatAI44,
    PXVideoFormatAYUV,
    PXVideoFormatYUY2,
    PXVideoFormatYVYU,
    PXVideoFormatYVU9,
    PXVideoFormatUYVY,
    PXVideoFormatNV11,
    PXVideoFormatNV12,
    PXVideoFormatNV21,
    PXVideoFormatYV12,
    PXVideoFormatI420,
    PXVideoFormatIYUV,
    PXVideoFormatY210,
    PXVideoFormatY216,
    PXVideoFormatY410,
    PXVideoFormatY416,
    PXVideoFormatY41P,
    PXVideoFormatY41T,
    PXVideoFormatY42T,
    PXVideoFormatP210,
    PXVideoFormatP216,
    PXVideoFormatP010,
    PXVideoFormatP016,
    PXVideoFormatv210,
    PXVideoFormatv216,
    PXVideoFormatv410,
    PXVideoFormatMP43,
    PXVideoFormatMP4S,
    PXVideoFormatM4S2,
    PXVideoFormatMP4V,
    PXVideoFormatWMV1,
    PXVideoFormatWMV2,
    PXVideoFormatWMV3,
    PXVideoFormatWVC1,
    PXVideoFormatMSS1,
    PXVideoFormatMSS2,
    PXVideoFormatMPG1,
    PXVideoFormatDVSL,
    PXVideoFormatDVSD,
    PXVideoFormatDVHD,
    PXVideoFormatDV25,
    PXVideoFormatDV50,
    PXVideoFormatDVH1,
    PXVideoFormatDVC,
    PXVideoFormatH264,
    PXVideoFormatH265,
    PXVideoFormatMJPG,
    PXVideoFormat420O,
    PXVideoFormatHEVC,
    PXVideoFormatHEVC_ES,
    PXVideoFormatVP80,
    PXVideoFormatVP90,
    PXVideoFormatORAW,
    PXVideoFormatH263,
    PXVideoFormatA2R10G10B10,
    PXVideoFormatA16B16G16R16F,
    PXVideoFormatVP10,
    PXVideoFormatAV1,
    PXVideoFormatTheora
}
PXVideoFormat;

typedef struct PXVideoFormatEntry_
{
    GUID* ID;
    PXVideoFormat Format;
}
PXVideoFormatEntry;


const PXVideoFormatEntry PXVideoFormatList[] =
{
    {&MFVideoFormat_Base,           PXVideoFormatBase},
{&MFVideoFormat_RGB32,           PXVideoFormatRGB32},
{&MFVideoFormat_ARGB32,           PXVideoFormatARGB32},
{&MFVideoFormat_RGB24,           PXVideoFormatRGB24},
{&MFVideoFormat_RGB555,           PXVideoFormatRGB555},
{&MFVideoFormat_RGB565,           PXVideoFormatRGB565},
{&MFVideoFormat_RGB8,           PXVideoFormatRGB8},
{&MFVideoFormat_L8,           PXVideoFormatL8},
{&MFVideoFormat_L16,           PXVideoFormatL16},
{&MFVideoFormat_D16,           PXVideoFormatD16},
{&MFVideoFormat_AI44,           PXVideoFormatAI44},
{&MFVideoFormat_AYUV,           PXVideoFormatAYUV},
{&MFVideoFormat_YUY2,           PXVideoFormatYUY2},
{&MFVideoFormat_YVYU,           PXVideoFormatYVYU},
{&MFVideoFormat_YVU9,           PXVideoFormatYVU9},
{&MFVideoFormat_UYVY,           PXVideoFormatUYVY},
{&MFVideoFormat_NV11,           PXVideoFormatNV11},
{&MFVideoFormat_NV12,           PXVideoFormatNV12},
{&MFVideoFormat_NV21,           PXVideoFormatNV21},
{&MFVideoFormat_YV12,           PXVideoFormatYV12},
{&MFVideoFormat_I420,           PXVideoFormatI420},
{&MFVideoFormat_IYUV,           PXVideoFormatIYUV},
{&MFVideoFormat_Y210,           PXVideoFormatY210},
{&MFVideoFormat_Y216,           PXVideoFormatY216},
{&MFVideoFormat_Y410,           PXVideoFormatY410},
{&MFVideoFormat_Y416,           PXVideoFormatY416},
{&MFVideoFormat_Y41P,           PXVideoFormatY41P},
{&MFVideoFormat_Y41T,           PXVideoFormatY41T},
{&MFVideoFormat_Y42T,           PXVideoFormatY42T},
{&MFVideoFormat_P210,           PXVideoFormatP210},
{&MFVideoFormat_P216,           PXVideoFormatP216},
{&MFVideoFormat_P010,           PXVideoFormatP010},
{&MFVideoFormat_P016,           PXVideoFormatP016},
{&MFVideoFormat_v210,           PXVideoFormatv210},
{&MFVideoFormat_v216,           PXVideoFormatv216},
{&MFVideoFormat_v410,           PXVideoFormatv410},
{&MFVideoFormat_MP43,           PXVideoFormatMP43},
{&MFVideoFormat_MP4S,           PXVideoFormatMP4S},
{&MFVideoFormat_M4S2,           PXVideoFormatM4S2},
{&MFVideoFormat_MP4V,           PXVideoFormatMP4V},
{&MFVideoFormat_WMV1,           PXVideoFormatWMV1},
{&MFVideoFormat_WMV2,           PXVideoFormatWMV2},
{&MFVideoFormat_WMV3,           PXVideoFormatWMV3},
{&MFVideoFormat_WVC1,           PXVideoFormatWVC1},
{&MFVideoFormat_MSS1,           PXVideoFormatMSS1},
{&MFVideoFormat_MSS2,           PXVideoFormatMSS2},
{&MFVideoFormat_MPG1,           PXVideoFormatMPG1},
{&MFVideoFormat_DVSL,           PXVideoFormatDVSL},
{&MFVideoFormat_DVSD,           PXVideoFormatDVSD},
{&MFVideoFormat_DVHD,           PXVideoFormatDVHD},
{&MFVideoFormat_DV25,           PXVideoFormatDV25},
{&MFVideoFormat_DV50,           PXVideoFormatDV50},
{&MFVideoFormat_DVH1,           PXVideoFormatDVH1},
{&MFVideoFormat_DVC,           PXVideoFormatDVC},
{&MFVideoFormat_H264,           PXVideoFormatH264},
{&MFVideoFormat_H265,           PXVideoFormatH265},
{&MFVideoFormat_MJPG,           PXVideoFormatMJPG},
{&MFVideoFormat_420O,           PXVideoFormat420O},
{&MFVideoFormat_HEVC,           PXVideoFormatHEVC},
{&MFVideoFormat_HEVC_ES,           PXVideoFormatHEVC_ES},
{&MFVideoFormat_VP80,           PXVideoFormatVP80},
{&MFVideoFormat_VP90,           PXVideoFormatVP90},
{&MFVideoFormat_ORAW,           PXVideoFormatORAW},

#if (WINVER >= _WIN32_WINNT_WIN8)
        {&MFVideoFormat_H263,           PXVideoFormatH263},
#endif

#if (WDK_NTDDI_VERSION >= NTDDI_WIN10)
        {&MFVideoFormat_A2R10G10B10,           PXVideoFormatA2R10G10B10},
        {&MFVideoFormat_A16B16G16R16F,           PXVideoFormatA16B16G16R16F},
#endif

#if (WDK_NTDDI_VERSION >= NTDDI_WIN10_RS3)
        {&MFVideoFormat_VP10,           PXVideoFormatVP10},
        {&MFVideoFormat_AV1,           PXVideoFormatAV1},
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_FE)
        {&MFVideoFormat_Theora,           PXVideoFormatTheora}
#endif 
};
const PXSize PXVideoFormatAmount = sizeof(PXVideoFormatList) / sizeof(PXVideoFormat);


PXResult PXAPI PXStreamCreateCamera(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo)
{
    const PXI16U deviceIndex = pxStreamOpenInfo->DeviceIndex;
    PXStreamCamera PXREF pxStreamCamera = &pxStream->Camera;
  

    HRESULT resultID = 0;
    PXResult pxResult = PXActionInvalid;

   // IMFMediaSource* pSource = NULL; 
   // IMFMediaBuffer* pBuffer = NULL;

    BYTE* pData = NULL;
    DWORD maxLen = 0;

    resultID = MFStartup(MF_VERSION, MFSTARTUP_FULL);
    pxResult = PXErrorFromHRESULT(resultID);

    if(PXActionSuccessful != pxResult)
        return pxResult;

    // Activate webcam
    IMFAttributes* pAttributes = NULL;
    MFCreateAttributes(&pAttributes, 1);
    resultID = pAttributes->lpVtbl->SetGUID
    (
        pAttributes, 
        &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
        &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
    );
    pxResult = PXErrorFromHRESULT(resultID);

    IMFActivate** ppDevices = NULL;
    UINT32 count = 0;
    MFEnumDeviceSources(pAttributes, &ppDevices, &count);

    if(count == 0) 
        return -2;

    IMFActivate* device = ppDevices[deviceIndex];

    resultID = device->lpVtbl->ActivateObject
    (
        device, 
        &IID_IMFMediaSource,
        (void**)&pxStreamCamera->MediaSource
    );
    pxResult = PXErrorFromHRESULT(resultID);

    // Create source reader
    resultID = MFCreateSourceReaderFromMediaSource(pxStreamCamera->MediaSource, NULL, &pxStreamCamera->SourceReader);
    pxResult = PXErrorFromHRESULT(resultID);

    IMFSourceReader PXREF sourceReader = pxStreamCamera->SourceReader;

    // Set media type (RGB32)
    {
        IMFMediaType* pType = NULL;

        resultID = MFCreateMediaType(&pType);
        pxResult = PXErrorFromHRESULT(resultID);

        resultID = sourceReader->lpVtbl->GetNativeMediaType
        (
            sourceReader,
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            NULL,
            &pType
        );
        pxResult = PXErrorFromHRESULT(resultID);

        GUID mediaType;
        GUID videoFormat;

        PXVideoFormat pxVideoFormat = PXVideoFormatInvalid;

        resultID = pType->lpVtbl->GetGUID(pType, &MF_MT_MAJOR_TYPE, &mediaType);
        pxResult = PXErrorFromHRESULT(resultID);

        resultID = pType->lpVtbl->GetGUID(pType, &MF_MT_SUBTYPE, &videoFormat);
        pxResult = PXErrorFromHRESULT(resultID);

        for(size_t i = 0; i < PXVideoFormatAmount; i++)
        {
            const PXBool isEqual = PXMemoryCompare(PXVideoFormatList[i].ID, sizeof(GUID), &videoFormat, sizeof(GUID));

            if(isEqual)
            {
                pxVideoFormat = PXVideoFormatList[i].Format;
                break;
            }
        }








#if 0
        resultID = pType->lpVtbl->SetGUID(pType, &MF_MT_MAJOR_TYPE, &MFMediaType_Video);
        resultID = pType->lpVtbl->SetGUID(pType, &MF_MT_SUBTYPE, &MFVideoFormat_RGB32);

        resultID = sourceReader->lpVtbl->SetCurrentMediaType
        (
            sourceReader,
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            NULL,
            pType
        );
#endif


        resultID = sourceReader->lpVtbl->SetStreamSelection
        (
            sourceReader,
            MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            TRUE
        );

        /*
        sourceReader->lpVtbl->GetCurrentMediaType
        (
            sourceReader,
            (DWORD)MF_SOURCE_READER_FIRST_VIDEO_STREAM,
            NULL,
            pType
        );
        */


      //  IMFAttributes* attributes = 0;

       // resultID = device->lpVtbl->QueryInterface(device, &IID_IMFAttributes, (void**)&attributes);

        UINT64 size;
        resultID = pType->lpVtbl->GetUINT64(pType, &MF_MT_FRAME_SIZE, &size);
      
        if(SUCCEEDED(resultID))
        {
            pxStreamCamera->Width = (UINT32)(size >> 32);
            pxStreamCamera->Height = (UINT32)(size & 0xFFFFFFFF);
        }
    }


   // IMFSourceReader::SetStreamSelection();



    
    // Maybe there is a way to add a callback..?
    // IMFSourceReaderCallback

    return PXActionSuccessful;
}

PXResult PXAPI PXStreamCreateTV(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo)
{
    HRESULT resultHandle = S_OK;
    PXResult pxActionResult = PXActionInvalid;

    IMFActivate** ppDevices = NULL;
    UINT32 count = 0;

    printf("Initializing COM...\n");
    resultHandle = CoInitializeEx(NULL, COINIT_MULTITHREADED);
 

    printf("Initializing Media Foundation...\n");
    resultHandle = MFStartup(MF_VERSION, 0);


    printf("Enumerating video capture devices...\n");
    IMFAttributes* pAttributes = NULL;
    resultHandle = MFCreateAttributes(&pAttributes, 1);


    resultHandle = pAttributes->lpVtbl->SetGUID(pAttributes, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
   // LogHR(hr, "SetGUID for video capture");

    resultHandle = MFEnumDeviceSources(pAttributes, &ppDevices, &count);
   // LogHR(hr, "MFEnumDeviceSources");

    printf("Found %u video capture devices.\n", count);

    for(UINT32 i = 0; i < count; i++) 
    {
        WCHAR* szFriendlyName = NULL;
        WCHAR* szSymbolicLink = NULL;

        UINT32 szFriendlyNameSize = 0;
        UINT32 szSymbolicLinkSize = 0;

        IMFActivate* device = ppDevices[i];

        resultHandle = device->lpVtbl->GetAllocatedString(device, &MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_SYMBOLIC_LINK, &szSymbolicLink, &szSymbolicLinkSize);
        resultHandle = device->lpVtbl->GetAllocatedString(device, &MF_DEVSOURCE_ATTRIBUTE_FRIENDLY_NAME, &szFriendlyName, &szFriendlyNameSize);


        printf("Device %u: %S\n", i, szFriendlyName);
        CoTaskMemFree(szFriendlyName);
    }

    // Assuming the first device is the TV tuner
    IMFMediaSource* pSource = NULL;
    printf("Activating device 0...\n");
    resultHandle = ppDevices[0]->lpVtbl->ActivateObject(ppDevices[0], &IID_IMFMediaSource, (void**)&pSource); 
    pxActionResult = PXErrorFromHRESULT(resultHandle);

    if(resultHandle != S_OK)
    {
        printf("ERROR: ActivateObject failed with HRESULT 0x%lx\n", resultHandle);
    }

    // Create source reader
    IMFSourceReader* pReader = NULL;
    printf("Creating source reader...\n");
    resultHandle = MFCreateSourceReaderFromMediaSource(pSource, NULL, &pReader);
  //  LogHR(hr, "MFCreateSourceReaderFromMediaSource");

   

    pxStream->Camera.MediaSource = pSource;
    pxStream->Camera.SourceReader = pReader;



    return PXActionSuccessful;
}

PXColorRGBAI8 buffer[3840 *1080];

PXResult PXAPI PXStreamUpdateWindow(PXStream PXREF pxStream)
{
    PXStreamWindow PXREF pxStreamWindow = &pxStream->Window;
    
    PXBool validCall = pxStreamWindow->MemoryDC > 0 && pxStreamWindow->WindowDC > 0;

    if(!validCall)
    {
        return  PXActionInvalid;
    }

    const BOOL result = BitBlt
    (
        pxStreamWindow->MemoryDC,
        0,
        0,
        pxStreamWindow->Rectangle.right - pxStreamWindow->Rectangle.left,
        pxStreamWindow->Rectangle.bottom - pxStreamWindow->Rectangle.top,
        pxStreamWindow->WindowDC,
        0, 
        0,
        SRCCOPY
    );

    if(!result)
    {
        // Resolve!
    }


    BITMAP bmp;
   // BitmapInfo info = { 0 };

    if(GetObject(pxStreamWindow->BitmapHandle, sizeof(BITMAP), &bmp) > 0) {
  
    }
    else {
        printf("Failed to get bitmap info\n");
    }


    BITMAPINFOHEADER infoHeader = { 0 };
    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = pxStreamWindow->Rectangle.right - pxStreamWindow->Rectangle.left;
    infoHeader.biHeight = (pxStreamWindow->Rectangle.bottom - pxStreamWindow->Rectangle.top); // negative to get top-down image
    infoHeader.biPlanes = bmp.bmPlanes;
    infoHeader.biBitCount = bmp.bmBitsPixel;    // RGB, 8 bits per channel
    infoHeader.biCompression = BI_RGB;

    GetDIBits
    (
        pxStreamWindow->WindowDC,
        pxStreamWindow->BitmapHandle, 
        0,
        pxStreamWindow->Rectangle.bottom - pxStreamWindow->Rectangle.top,
        buffer,
        (BITMAPINFO*)&infoHeader, 
        DIB_RGB_COLORS
    );

    PXStreamOnFrameInfo pxStreamOnFrameInfo;
    pxStreamOnFrameInfo.DataAdress = buffer;
    pxStreamOnFrameInfo.Width = pxStreamWindow->Rectangle.right - pxStreamWindow->Rectangle.left;
    pxStreamOnFrameInfo.Height = pxStreamWindow->Rectangle.bottom - pxStreamWindow->Rectangle.top;
    pxStreamOnFrameInfo.DataSize = (infoHeader.biBitCount /8) * pxStreamOnFrameInfo.Width * pxStreamOnFrameInfo.Height;
    pxStreamOnFrameInfo.BitPerPixel = infoHeader.biBitCount;

    pxStream->OnFrame(pxStream, &pxStreamOnFrameInfo);

    return PXActionSuccessful;
}





void ConvertYUY2ToRGB
(
    const PXI8U* yuy2Buffer,
    int width,
    int height,
    PXColorRGBI8* rgbBuffer
)
{
    int frameSize = width * height;
    //rgbBuffer.resize(frameSize * 3); // 3 bytes per pixel for RGB


    int indexRGB = 0;

    for(int i = 0; i < frameSize * 2; i += 4)
    {
        // YUY2: 4 bytes for 2 pixels
        PXI8U Y0 = yuy2Buffer[i + 0];
        PXI8U U = yuy2Buffer[i + 1];
        PXI8U Y1 = yuy2Buffer[i + 2];
        PXI8U V = yuy2Buffer[i + 3];


        PXColorRGBI8* pxColorRGBI8_1 = &rgbBuffer[indexRGB++];
        PXColorRGBI8* pxColorRGBI8_2 = &rgbBuffer[indexRGB++];

        PXColorYUV pxColorYUV;
        pxColorYUV.U = U;
        pxColorYUV.V = V;

        pxColorYUV.Y = Y0;
        PXColorYUYToRGB(&pxColorYUV, pxColorRGBI8_1);

        pxColorYUV.Y = Y1;
        PXColorYUYToRGB(&pxColorYUV, pxColorRGBI8_2);
    }
}

PXResult PXAPI PXStreamUpdateCamera(PXStream PXREF pxStream)
{
    PXStreamCamera PXREF pxStreamCamera = &pxStream->Camera;
    IMFSourceReader PXREF sourceReader = pxStreamCamera->SourceReader;
    IMFMediaBuffer* mediaBuffer = pxStreamCamera->MediaBuffer;

    PXStreamOnFrameInfo pxStreamOnFrameInfo;
    pxStreamOnFrameInfo.Width = pxStreamCamera->Width;
    pxStreamOnFrameInfo.Height = pxStreamCamera->Height;

    HRESULT resultID = 0;



    // Read a frame
    DWORD streamIndex = 0;
    MF_SOURCE_READER_FLAG flags = MF_SOURCE_READERF_ERROR;
    LONGLONG timestamp = 0;

    resultID = sourceReader->lpVtbl->ReadSample
    (
        sourceReader,
        MF_SOURCE_READER_FIRST_VIDEO_STREAM,
        0,
        &streamIndex,
        &flags,
        &timestamp,
        &pxStreamCamera->Sample
    );
    const PXBool success = S_OK == resultID;

    if(!success)
    {
        return PXActionInvalid;
    }

    IMFSample* sample = pxStreamCamera->Sample;

    // if no data
    if(!sample)
    {
        return PXActionSuccessful;
    }


    resultID = sample->lpVtbl->ConvertToContiguousBuffer(sample, &pxStreamCamera->MediaBuffer);
    mediaBuffer = pxStreamCamera->MediaBuffer;


    DWORD dataSize = 0;

    resultID = mediaBuffer->lpVtbl->Lock(mediaBuffer, &pxStreamOnFrameInfo.DataAdress, NULL, &dataSize);

    pxStreamOnFrameInfo.DataSize = dataSize;


    ConvertYUY2ToRGB
    (
        pxStreamOnFrameInfo.DataAdress,
        pxStreamOnFrameInfo.Width,
        pxStreamOnFrameInfo.Height,
        buffer
    );

    pxStreamOnFrameInfo.DataAdress = buffer;
    pxStreamOnFrameInfo.BitPerPixel = 24;

    pxStream->OnFrame(pxStream, &pxStreamOnFrameInfo);

    resultID = mediaBuffer->lpVtbl->Unlock(mediaBuffer);

    return PXActionSuccessful;
}

PXResult PXAPI PXStreamUpdateTV(PXStream PXREF pxStream)
{
    /*
    // Read samples
    printf("Starting sample reading loop...\n");
    for(int i = 0; i < 100; i++)
    {
        IMFSample* pSample = NULL;
        DWORD dwStreamIndex, dwFlags;
        LONGLONG llTimestamp;

        hr = pReader->lpVtbl->ReadSample
        (
            pReader, 
            MF_SOURCE_READER_FIRST_VIDEO_STREAM, 
            0,
            &dwStreamIndex, 
            &dwFlags, 
            &llTimestamp,
            &pSample
        );
        //   LogHR(hr, "ReadSample");

        if(SUCCEEDED(hr) && pSample) {
            printf("Sample %d received at timestamp %lld\n", i, llTimestamp);
            pSample->lpVtbl->Release(pSample);
        }
        else {
            printf("No sample received at iteration %d\n", i);
        }

        Sleep(100); // simulate processing delay
    }*/


    return PXActionSuccessful;
}

PXResult PXAPI PXStreamReleaseWindow(PXStream PXREF pxStream)
{
    //DeleteDC(hMemoryDC);
    //ReleaseDC(hwnd, hWindowDC);

    return PXActionSuccessful;
}

PXResult PXAPI PXStreamReleaseMonitor(PXStream PXREF pxStream)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXStreamReleaseCamera(PXStream PXREF pxStream)
{
    IMFMediaBuffer PXREF mediaBuffer = pxStream->Camera.MediaBuffer;
    mediaBuffer->lpVtbl->Release(mediaBuffer);

    return PXActionSuccessful;
}

PXResult PXAPI PXStreamOpen(PXStream PXREF pxStream, PXStreamOpenInfo PXREF pxStreamOpenInfo)
{
    PXClear(PXStream, pxStream);

    pxStream->OnFrame = pxStreamOpenInfo->OnFrameCallBack;

	switch(pxStreamOpenInfo->Mode)
	{
		case PXStreamModeWindow:
		{
            pxStream->Create = PXStreamCreateWindow;
            pxStream->Update = PXStreamUpdateWindow;
            pxStream->Release = PXStreamReleaseWindow;
			break;
		}
		case PXStreamModeMonitor:
		{
            pxStream->Create = PXStreamCreateMonitor;
            pxStream->Update = PXStreamUpdateWindow;
            pxStream->Release = PXStreamReleaseMonitor;
			break;
		}
		case PXStreamModeCamera:
		{
            pxStream->Create = PXStreamCreateCamera;
            pxStream->Update = PXStreamUpdateCamera;
            pxStream->Release = PXStreamReleaseCamera;
			break;
		}
        case PXStreamModeTV:
        {
            pxStream->Create = PXStreamCreateTV;
            pxStream->Update = PXStreamUpdateCamera;
            pxStream->Release = 0;
            break;
        }
		default:
		{
            return PXActionRefusedArgumentInvalid;
		}
	}

    // Create
    const PXResult pxActionResult = pxStream->Create(pxStream, pxStreamOpenInfo);

    return pxActionResult;
}

PXResult PXAPI PXStreamClose(PXStream PXREF pxStream)
{
    return PXActionSuccessful;
}
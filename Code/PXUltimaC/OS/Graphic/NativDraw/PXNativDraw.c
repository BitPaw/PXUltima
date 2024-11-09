#include "PXNativDraw.h"

#if OSUnix 
#include <X11/Xlib.h> 
#include <X11/Xutil.h>
#elif OSWindows
#include <windows.h>
#endif

#include <Media/PXText.h>
#include <OS/Console/PXConsole.h>


typedef struct PXEindowsDisplayEnumInfo_
{
    PXMonitor* MonitorList;
    int CheckMode;
    int OffsetCurrent;
    int OffsetMaxmimal;
}
PXEindowsDisplayEnumInfo;

BOOL PXWindowsMonitorFetch(HMONITOR monitorHandle, HDC unnamedParam2, LPRECT unnamedParam3, PXEindowsDisplayEnumInfo* pxEindowsDisplayEnumInfo)
{
    if(pxEindowsDisplayEnumInfo->CheckMode == 1)
    {
        pxEindowsDisplayEnumInfo->OffsetMaxmimal++;
        return PXTrue;
    }

    PXMonitor* const pxMonitor = &pxEindowsDisplayEnumInfo->MonitorList[pxEindowsDisplayEnumInfo->OffsetCurrent];
    pxMonitor->ID = monitorHandle;

    MONITORINFOEX  monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEX);

    GetMonitorInfoA(monitorHandle, &monitorInfo);

    PXTextCopyA(monitorInfo.szDevice, CCHDEVICENAME, pxMonitor->Name, 32);

    ++pxEindowsDisplayEnumInfo->OffsetCurrent;

    return PXTrue;
}



PXActionResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw* const pxNativDraw)
{
#if OSUnix

    Display* xDisplay = pxGUISystem->DisplayCurrent.DisplayHandle;

    pxGUISystem->DisplayCurrent.ScreenDefaultID = XDefaultScreen(xDisplay);
    pxGUISystem->DisplayCurrent.ScreenListAmount = XScreenCount(xDisplay);

    for(PXSize screenID = 0; screenID < pxGUISystem->DisplayCurrent.ScreenListAmount; ++screenID)
    {
        PXDisplayScreen* pxDisplayScreen = &pxGUISystem->DisplayCurrent.DisplayScreenList[screenID];

        pxDisplayScreen->Width = XDisplayWidth(xDisplay, screenID);
        pxDisplayScreen->Height = XDisplayHeight(xDisplay, screenID);
        pxDisplayScreen->Cells = XDisplayCells(xDisplay, screenID);
        pxDisplayScreen->Planes = XDisplayPlanes(xDisplay, screenID);
        pxDisplayScreen->WidthMM = XDisplayWidthMM(xDisplay, screenID);
        pxDisplayScreen->HeightMM = XDisplayHeightMM(xDisplay, screenID);

        pxDisplayScreen->IsConnected = PXTrue;
        pxDisplayScreen->IsPrimary = screenID == pxGUISystem->DisplayCurrent.ScreenDefaultID;
    }

#elif OSWindows




    //
    {
        PXEindowsDisplayEnumInfo pxEindowsDisplayEnumInfo;   
        pxEindowsDisplayEnumInfo.MonitorList = PXNull;
        pxEindowsDisplayEnumInfo.OffsetCurrent = 0;
        pxEindowsDisplayEnumInfo.OffsetMaxmimal = 0;
        pxEindowsDisplayEnumInfo.CheckMode = 1;

        pxNativDraw->MonitorListAmount = 0;

        const BOOL success = EnumDisplayMonitors(NULL, NULL, PXWindowsMonitorFetch, &pxEindowsDisplayEnumInfo);

        pxNativDraw->MonitorListAmount = pxEindowsDisplayEnumInfo.OffsetMaxmimal;
        pxNativDraw->MonitorList = PXMemoryReallocT(PXMonitor, PXNull, pxNativDraw->MonitorListAmount);

        pxEindowsDisplayEnumInfo.MonitorList = pxNativDraw->MonitorList;
        pxEindowsDisplayEnumInfo.CheckMode = 0;

        EnumDisplayMonitors(NULL, NULL, PXWindowsMonitorFetch, &pxEindowsDisplayEnumInfo);
    }





    // List all devices
    {
        DWORD amount = 0;
        DWORD dwFlags = 0;

        DISPLAY_DEVICEA displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICEA);       

        for(;;)
        {
            const PXBool didDetectDevice = EnumDisplayDevicesA(NULL, amount, &displayDevice, dwFlags);

            if(!didDetectDevice)
            {
                break;
            }

            ++(amount);
        }



        if(0 == amount) // No devices found
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                "NativDraw",
                "Display-Fetch",
                "Devices detected : none"
            );
#endif

            return PXActionSuccessful;
        }


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "NativDraw",
            "Display-Fetch",
            "Devices detected : %i",
            amount
        );
#endif

        PXDisplay* pxDisplayList = PXMemoryReallocT(PXDisplay, PXNull, amount);

        pxNativDraw->DisplayList = pxDisplayList;
        pxNativDraw->DisplayListAmount = amount;


        for(PXSize deviceID = 0; EnumDisplayDevicesA(NULL, deviceID, &displayDevice, dwFlags); deviceID++)
        {
            PXDisplayScreen* const pxDisplayScreen = &pxDisplayList[deviceID];
            pxDisplayScreen->IsConnected = PXFlagIsSet(displayDevice.StateFlags, DISPLAY_DEVICE_ATTACHED_TO_DESKTOP);
            pxDisplayScreen->IsPrimary = PXFlagIsSet(displayDevice.StateFlags, DISPLAY_DEVICE_PRIMARY_DEVICE);
       
            PXTextCopyA(displayDevice.DeviceName, 32, pxDisplayScreen->NameID, PXDisplayScreenNameLength);
            PXTextCopyA(displayDevice.DeviceString, 128, pxDisplayScreen->GraphicDeviceName, PXDisplayScreenDeviceLength);
            // PXTextCopyA(displayDevice.DeviceID, 128, pxDisplayScreen->DeviceID, PXDeviceIDSize);
            //PXTextCopyA(displayDevice.DeviceKey, 128, pxDisplayScreen->DeviceKey, PXDeviceKeySize);

            /*
             PXSize position = PXTextFindFirstCharacterA(monitorDeviceA.DeviceID, 128, '\\');

                    PXSize targetZize = 128 - position + 1;
                    char* target = monitorDeviceA.DeviceID + position + 1;

                    PXSize positionB = PXTextFindFirstCharacterA(target, targetZize, '\\');

                    PXTextCopyA(target, positionB, pxDisplayScreen->NameMonitor, PXDisplayScreenMonitorLength);            
            */


            DWORD    iModeNum = ENUM_CURRENT_SETTINGS;
            DEVMODEA devMode = { 0 };
            DWORD    dwFlags = EDS_RAWMODE;

            devMode.dmSize = sizeof(DEVMODEA);

            const BOOL settingsResult = EnumDisplaySettingsExA
            (
                displayDevice.DeviceName,
                iModeNum,
                &devMode,
                dwFlags
            );      

            if(!pxDisplayScreen->IsConnected)
            {
                continue;
            }

            pxDisplayScreen->Width = devMode.dmPelsWidth;
            pxDisplayScreen->Height = devMode.dmPelsHeight;
        }
    }

#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Display-Device",
        "Fetching <%i> monitor devices",
        pxNativDraw->DisplayListAmount
    );

    for(PXSize i = 0; i < pxNativDraw->DisplayListAmount; ++i)
    {
        PXDisplayScreen* const pxDisplayScreen = &pxNativDraw->DisplayList[i];

        PXLogPrint
        (
            PXLoggingInfo,
            "GUI",
            "Display-Device",
            "Detected:\n"
            "+--------------------------------------------------------+\n"
            "| DeviceName     : %-27s %-4ix%4i |\n"
            "| DeviceString   : %-37.37s |\n"
            "+--------------------------------------------------------+\n",
            pxDisplayScreen->NameMonitor,
            pxDisplayScreen->Width,
            pxDisplayScreen->Height,
            pxDisplayScreen->NameID
        );
    }
#endif

    return PXActionSuccessful;

    
}

PXActionResult PXAPI PXNativDrawDisplayOpen(PXNativDraw* const pxNativDraw, PXDisplay* const pxDisplay, const char* const displayName)
{
#if OSUnix

    // Use X11

    int screen;     
    
    Display* const display = XOpenDisplay(displayName); // Open connection to the X server    
    const PXBool success = NULL != display;

    if(true)
    {

    }

    if (display == NULL) 
    {
        fprintf(stderr, "Cannot open display\n"); exit(1); } screen =


#elif OSWindows
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

 
}



PXActionResult PXAPI PXNativDrawWindowCreate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo)
{
#if OSUnix


    const int attributeList[] =
    {
        GLX_RGBA,
        GLX_DEPTH_SIZE,
        24,
        GLX_DOUBLEBUFFER,
        None
    };

    const XVisualInfo* const visualInfo = glXChooseVisual(pxGUISystem->DisplayCurrent.DisplayHandle, 0, attributeList);

    {
        const PXBool successful = visualInfo != 0;

        if(!successful)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "X-System",
                "Visual",
                "Choosing failed"
            );
#endif

            return PXActionSuccessful; // no appropriate visual found
        }


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "X-System",
            "Visual",
            "OK (0x%p)",
            visualInfo
        );
#endif

}

    // default color map?

    // Create colormapping
    Colormap colormap = XCreateColormap
    (
        pxGUISystem->DisplayCurrent.DisplayHandle,
        pxGUISystem->DisplayCurrent.WindowRootHandle,
        visualInfo->visual,
        AllocNone
    );

    XSetWindowAttributes setWindowAttributes;
    //setWindowAttributes.cursor = ;
    setWindowAttributes.colormap = colormap;
    setWindowAttributes.event_mask =
        KeyPressMask |
        KeyReleaseMask |
        ButtonPressMask |
        ButtonReleaseMask |
        EnterWindowMask |
        LeaveWindowMask |
        PointerMotionMask |
        PointerMotionHintMask |
        Button1MotionMask |
        Button2MotionMask |
        Button3MotionMask |
        Button4MotionMask |
        Button5MotionMask |
        ButtonMotionMask |
        KeymapStateMask |
        ExposureMask |
        VisibilityChangeMask |
        StructureNotifyMask |
        ResizeRedirectMask |
        SubstructureNotifyMask |
        SubstructureRedirectMask |
        FocusChangeMask |
        PropertyChangeMask |
        ColormapChangeMask |
        OwnerGrabButtonMask |
        // XI_RawMotion |
        0;




    if(pxNativDrawWindow->Simple)
    {
        const Window windowID = XCreateSimpleWindow
        (
            pxNativDrawWindow->DisplayCurrent.Handle,
            pxNativDrawWindow->ParentID,
            pxNativDrawWindow->X,
            pxNativDrawWindow->Y,
            pxNativDrawWindow->Width,
            pxNativDrawWindow->Height,
            pxNativDrawWindow->BorderWidth,
            pxNativDrawWindow->Border,
            PXNull
        );      
    }
    else
    {
        const Window windowID = XCreateWindow
        (
            pxNativDrawWindow->DisplayCurrent.Handle,
            pxNativDrawWindow->ParentID,
            pxNativDrawWindow->X,
            pxNativDrawWindow->Y,
            pxNativDrawWindow->Width,
            pxNativDrawWindow->Height,
            pxNativDrawWindow->BorderWidth,
            visualInfo->depth,
            InputOutput,
            visualInfo->visual,
            CWColormap | CWEventMask,
            &setWindowAttributes
        );
    }







    // Attach to render engine
    {
        const int mapResultID = XMapWindow(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.Handle.WindowID);
        const PXBool success = Success == mapResultID;

        if(!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "X-System",
                "Window-Mapping",
                "Failed"
            );
#endif
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "X-System",
            "Window-Mapping",
            "OK"
        );
#endif
    }

    // Set title
    {
        const PXBool setTextSuccess = PXWindowTextSet(pxGUISystem, pxGUIElement, pxGUIElementCreateWindowInfo->Title);
    }


#elif OSWindows

    const HWND windowHandle = CreateWindowExA // Windows 2000, User32.dll, winuser.h
    (
        pxWindowCreateInfo->WindowsWindowsStyleFlagsExtended,
        pxWindowCreateInfo->WindowsClassName,
        pxWindowCreateInfo->WindowsTextContent, // Text content
        pxWindowCreateInfo->WindowsStyleFlags,
        pxWindowCreateInfo->X,
        pxWindowCreateInfo->Y,
        pxWindowCreateInfo->Width,
        pxWindowCreateInfo->Height,
        pxWindowCreateInfo->ParentID,
        PXNull,  // No menu.
        pxWindowCreateInfo->InstanceHandle,
        NULL // Pointer not needed.
    );
    const PXActionResult createResult = PXErrorCurrent(PXNull != windowHandle);

    if(PXActionSuccessful != createResult)
    {
        return createResult;
    }

    pxWindow->Info.Handle.WindowID = windowHandle;
    pxWindow->DeviceContextHandle = GetDC(windowHandle);  

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXNativDrawWindowEventPendingAmount(PXNativDraw* const pxNativDraw, PXSize* const amount)
{
#if OSUnix

    *amount = XPending(pxNativDraw->Display); // X11

    return PXActionSuccessful;

#elif OSWindows

    *amount = GetQueueStatus(QS_ALLINPUT); // Windows 2000 

    return PXActionSuccessful;

#else
    *amount = 0;

    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXNativDrawWindowBufferSwap(PXNativDraw* const pxNativDraw, const PXWindow* const pxWindow)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    glXSwapBuffers(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.Handle.WindowID);
    pxActionResult = PXActionSuccessful; // TODO: Do we have error checking?

#elif OSWindows
    const PXBool result = SwapBuffers(pxWindow->DeviceContextHandle);
    pxActionResult = PXErrorCurrent(result);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}



PXActionResult PXAPI PXWindowDrawBegin(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement)
{
#if OSUnix

#elif OSWindows
    PAINTSTRUCT ps;

    HDC hdc = BeginPaint(pxGUIElement->Info.Handle.WindowID, &ps);
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXWindowDrawEnd(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement)
{
#if OSUnix

#elif OSWindows
    PAINTSTRUCT ps;

    const PXBool end = EndPaint(pxGUIElement->Info.Handle.WindowID, &ps);
#endif

    return PXActionRefusedNotImplemented;
}









PXActionResult PXAPI PXGUIDrawColorSetBrush(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowBrush* const pxGUIElementBrush, const char mode)
{
    PXColorRGBI8* colorRef = PXNull;

    if(PXWindowBrushBehaviourColorEmbeded & pxGUIElementBrush->Info.Behaviour)
    {
        colorRef = &pxGUIElementBrush->ColorDate;
    }
    else
    {
        colorRef = pxGUIElementBrush->ColorReference;
    }

    return PXGUIDrawColorSetV3(pxGUISystem, pxGUIElement, colorRef, mode);
}

PXActionResult PXAPI PXGUIDrawColorSetV3(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXColorRGBI8* const pxColorRGBI8, const char mode)
{
    if(mode == PXGUIDrawModeFront)
    {
#if OSUnix
        const PXInt32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const int resultID = XSetForeground
        (
            pxGUISystem->DisplayCurrent.DisplayHandle,
            pxGUISystem->DisplayCurrent.GraphicContent,
            color
        );
#elif OSWindows
        const COLORREF colorNew = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const COLORREF colorPrevious = SetTextColor(pxGUIElement->DeviceContextHandle, colorNew);
#endif
    }
    else
    {
        PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
        const PXInt32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const int resultID = XSetBackground
        (
            pxGUISystem->DisplayCurrent.DisplayHandle,
            pxGUISystem->DisplayCurrent.GraphicContent,
            color
        );
#elif OSWindows

        SetBkMode(pxGUIElement->DeviceContextHandle, TRANSPARENT);

        const COLORREF colorNew = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const COLORREF colorPrevious = SetBkColor(pxGUIElement->DeviceContextHandle, colorNew);
        const PXBool successful = CLR_INVALID != colorPrevious;

        if(!successful)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "GUI",
                "Color-Set",
                "Failed set backgroundcolor"
            );
#endif

            return PXActionRefusedArgumentInvalid;
        }

#endif
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIDrawColorSetRGB(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, char red, char green, char blue, const char mode)
{
    PXColorRGBI8 color = { red, green, blue };

    return PXGUIDrawColorSetV3(pxGUISystem, pxGUIElement, &color, mode);
}

PXActionResult PXAPI PXNativDrawFontListFetch(PXNativDraw* const pxNativDraw)
{
  
}

PXActionResult PXAPI PXNativDrawFontLoadA(PXNativDraw* const pxNativDraw, PXFont* const pxFont, const char* const name, const PXSize nameLength)
{
    // "Consolas"

#if OSUnix
    XFontStruct* font = XLoadQueryFont
    (
        pxGUISystem->DisplayCurrent.DisplayHandle,
        name
    ); // "9x15" <--- linux font? Sometimes not found
#elif OSWindows


    const DWORD antialiased = (PXFontAntialiased & pxFont->Info.Behaviour) > 0 ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;
    const DWORD isItalics = (PXFontItalics & pxFont->Info.Behaviour) > 0;
    const DWORD isUnderline = (PXFontUnderline & pxFont->Info.Behaviour) > 0;
    const DWORD isStrikeOut = (PXFontStrikeOut & pxFont->Info.Behaviour) > 0;

    pxFont->Info.Handle.FontHandle = CreateFontA
    (
        pxFont->Size, 0, 0, 0,
        FW_HEAVY, //  FW_HEAVY
        isItalics,
        isUnderline,
        isStrikeOut,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        antialiased,
        DEFAULT_PITCH | FF_DONTCARE,
        name
    );

#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXNativDrawFontRelease(PXNativDraw* const pxNativDraw, PXFont* const pxFont)
{
#if OSUnix
    const int resultID = XFreeFont(pxGUISystem->DisplayCurrent.DisplayHandle, 0);
#elif OSWindows

#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXGUIFontSet(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXFont* const pxFont)
{
#if OSUnix
    const int resultID = XSetFont
    (
        pxGUISystem->DisplayCurrent.DisplayHandle,
        pxGUISystem->DisplayCurrent.GraphicContent,
        0
    );
#elif OSWindows
    //HFONT fontThatWasUsedBefore = (HFONT)SelectObject(hdc, hFont1);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawFontSet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXFont* const pxFont)
{
    
}








PXActionResult PXAPI PXNativeDrawClear(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement)
{
#if OSUnix
    const int resultID = XClearWindow(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.Handle.WindowID);
#elif OSWindows
    // Does this exists?

    //GetUpdateRect();
#endif

    return PXActionSuccessful;
}








/*
#if OSUnix
#elif OSWindows
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif




#if OSUnix
    return -1;
#elif PXOSWindowsDestop
    return GetWindowThreadProcessId(windowID, PXNull);
#else
    return -1;
#endif


#if 0
    switch(window->CursorModeCurrent)
    {
        default:
        case PXWindowCursorIgnore:
        case PXWindowCursorShow:
            return PXFalse;

        case PXWindowCursorInvisible:
        case PXWindowCursorLock:
        case PXWindowCursorLockAndHide:
            return PXTrue;
    }
#endif
*/
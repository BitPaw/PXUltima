#include "PXNativDraw.h"

PXNativDraw _internalNativDraw;

#if OSUnix
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h> // XI_RawMotion
#elif OSWindows
#include <windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include <wtypes.h>
#include <Dbt.h>
#include <ole2.h> // Object Linking and Embedding
#include <dwmapi.h> // Set tilebar color
#include <shellapi.h>

//#include <gdiplusgraphics.h>
//#include <ddrawgdi.h>
//#include <hidusage.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Msimg32.lib")

#define Windows10DarkModeID 20 // DWMWA_USE_IMMERSIVE_DARK_MODE

#endif

#include <Media/PXText.h>
#include <Engine/PXEngine.h>
#include <Math/PXCollision.h>
#include <OS/Console/PXConsole.h>
#include "assert.h"


#if OSWindows

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

    const BOOL result = GetMonitorInfoA(monitorHandle, (LPMONITORINFO)&monitorInfo);

    PXTextCopyA(monitorInfo.szDevice, CCHDEVICENAME, pxMonitor->Name, 32);

    ++pxEindowsDisplayEnumInfo->OffsetCurrent;

    return PXTrue;
}

int CALLBACK PXWindowsFontEnumCallBack(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM lParam)
{
    PXFont* pxFont;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativDraw",
        "Font-Fetch",
        "detected : %s",
        lpelfe->lfFaceName
    );
#endif

    return PXTrue;
}

#endif

#if OSUnix
// X11 error handling functions
int PXAPI PXNativeDrawErrorHandler(Display* displayHandle, XErrorEvent* errorEventData)
{
#if PXLogEnable
    char errorBuffer[256];

    XGetErrorText(displayHandle, errorEventData->error_code, errorBuffer, 256);

    PXLogPrint
    (
        PXLoggingInfo,
        "X-System",
        "Error",
        "(%i) %s",
        errorEventData->error_code,
        errorBuffer
    );
#endif

    return 0;
}

PXActionResult PXAPI PXNativeDrawErrorFetch(const PXBool condition)
{
    if(condition)
    {
        return PXActionSuccessful;
    }


    return PXActionSuccessful;
}
#endif

PXActionResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw* const pxNativDraw)
{
#if OSUnix && 0

    Display* xDisplay = &pxNativDraw->GUISystem->DisplayCurrent;

    pxNativDraw->GUISystem->DisplayCurrent.ScreenDefaultID = XDefaultScreen(xDisplay);
    pxNativDraw->GUISystem->DisplayCurrent.ScreenListAmount = XScreenCount(xDisplay);

    for(PXSize screenID = 0; screenID < pxNativDraw->GUISystem->DisplayCurrent.ScreenListAmount; ++screenID)
    {
        PXDisplayScreen* pxDisplayScreen = &pxNativDraw->GUISystem->DisplayCurrent.DisplayScreenList[screenID];

        pxDisplayScreen->Width = XDisplayWidth(xDisplay, screenID);
        pxDisplayScreen->Height = XDisplayHeight(xDisplay, screenID);
        pxDisplayScreen->Cells = XDisplayCells(xDisplay, screenID);
        pxDisplayScreen->Planes = XDisplayPlanes(xDisplay, screenID);
        pxDisplayScreen->WidthMM = XDisplayWidthMM(xDisplay, screenID);
        pxDisplayScreen->HeightMM = XDisplayHeightMM(xDisplay, screenID);

        pxDisplayScreen->IsConnected = PXTrue;
        pxDisplayScreen->IsPrimary = screenID == pxNativDraw->GUISystem->DisplayCurrent.ScreenDefaultID;
    }

#elif OSWindows && 0




    //
    {
        PXEindowsDisplayEnumInfo pxEindowsDisplayEnumInfo;
        pxEindowsDisplayEnumInfo.MonitorList = PXNull;
        pxEindowsDisplayEnumInfo.OffsetCurrent = 0;
        pxEindowsDisplayEnumInfo.OffsetMaxmimal = 0;
        pxEindowsDisplayEnumInfo.CheckMode = 1;

        pxNativDraw->MonitorListAmount = 0;

        const BOOL success = EnumDisplayMonitors
        (
            NULL,
            NULL,
            (MONITORENUMPROC)PXWindowsMonitorFetch,
            (LPARAM)&pxEindowsDisplayEnumInfo
        );

        pxNativDraw->MonitorListAmount = pxEindowsDisplayEnumInfo.OffsetMaxmimal;
        pxNativDraw->MonitorList = PXMemoryReallocT(PXMonitor, PXNull, pxNativDraw->MonitorListAmount);

        pxEindowsDisplayEnumInfo.MonitorList = pxNativDraw->MonitorList;
        pxEindowsDisplayEnumInfo.CheckMode = 0;

        EnumDisplayMonitors
        (
            NULL,
            NULL,
            (MONITORENUMPROC)PXWindowsMonitorFetch,
            (LPARAM)&pxEindowsDisplayEnumInfo
        );
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
            PXDisplay* const pxDisplay = &pxDisplayList[deviceID];
            pxDisplay->IsConnected = PXFlagIsSet(displayDevice.StateFlags, DISPLAY_DEVICE_ATTACHED_TO_DESKTOP);
            pxDisplay->IsPrimary = PXFlagIsSet(displayDevice.StateFlags, DISPLAY_DEVICE_PRIMARY_DEVICE);

            PXTextCopyA(displayDevice.DeviceName, 32, pxDisplay->NameID, PXDisplayScreenNameLength);
            PXTextCopyA(displayDevice.DeviceString, 128, pxDisplay->GraphicDeviceName, PXDisplayScreenDeviceLength);
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

            if(!pxDisplay->IsConnected)
            {
                continue;
            }

            pxDisplay->Width = devMode.dmPelsWidth;
            pxDisplay->Height = devMode.dmPelsHeight;
        }
    }

#endif


#if PXLogEnable && 0
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
        PXDisplay* const pxDisplay = &pxNativDraw->DisplayList[i];

        char* Data;
        char* Name;

        int ProtocolVersion;
        int ProtocolRevision;

        char* ServerVendor;
        int VendorRelease;

        int ScreenDefaultID;
        int ScreenListAmount;

        PXSize DisplayScreenListAmount;
        PXDisplayScreen* DisplayScreenList;


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
            pxDisplay->Name,
            pxDisplay->Width,
            pxDisplay->Height,
            pxDisplay->NameID
        );
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawDisplayOpen(PXNativDraw* const pxNativDraw, PXDisplay* const pxDisplay, const char* const displayName)
{
#if OSUnix
    // open a connection to the x-server. NULL here uses the default display.
    pxDisplay->DisplayHandle = XOpenDisplay(displayName); // X11/Xlib.h, Open connection to the X server
    const PXBool openSuccess = NULL != pxDisplay->DisplayHandle;

    if(!openSuccess)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "X-System",
            "Display-Open",
            "<%s> failed to open!",
            displayName
        );
#endif

        return PXActionInvalid;
    }

    // Attach error callback
    XSetErrorHandler(PXNativeDrawErrorHandler);

    // Get default values
    pxDisplay->WindowRootHandle = XDefaultRootWindow(pxDisplay->DisplayHandle); // Make windows root
    pxDisplay->GraphicContent = XCreateGC(pxDisplay->DisplayHandle, pxDisplay->WindowRootHandle, 0, 0);

    // Fetch propertys
    pxDisplay->Data = XDisplayString(pxDisplay->DisplayHandle);
    pxDisplay->Name = XDisplayName(pxDisplay->DisplayHandle); // if NULL, this is the atempted name what XOpen would use
    pxDisplay->ProtocolVersion = XProtocolVersion(pxDisplay->DisplayHandle); // for X11, it is 11
    pxDisplay->ProtocolRevision = XProtocolRevision(pxDisplay->DisplayHandle);
    pxDisplay->ServerVendor = XServerVendor(pxDisplay->DisplayHandle);
    pxDisplay->VendorRelease = XVendorRelease(pxDisplay->DisplayHandle);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "X-System",
        "Display-Open",
        "OK (0x%p)\n"
        "%10s: %i\n"
        "%10s: %s, Data: %s\n"
        "%10s: v.%i.%i\n"
        "%10s: %s (Relase %i)",
        pxDisplay->DisplayHandle,
        "Root", pxDisplay->WindowRootHandle,
        "Name", pxDisplay->Name, pxDisplay->Data,
        "Protocol", pxDisplay->ProtocolVersion, pxDisplay->ProtocolRevision,
        "Server", pxDisplay->ServerVendor, pxDisplay->VendorRelease
    );
#endif

    // Get amount of screens

   // Fetch additional data of your display

   /*


           // Default values
           Colormap XDefaultColormap(Display *display, int screen_number);
           int XDefaultDepth(Display *display, int screen_number);
           int *XListDepths(Display *display, int screen_number, int *count_return);
           GC XDefaultGC(Display *display, int screen_number);
           Visual *XDefaultVisual(Display *display, int screen_number);





           // UI Element needs function to override drawing by OS
           // Linux does not even have drawing
           PXWindowDrawFunction(GUISystem, PXWindow);


           PXWindowDrawRectangleFill();



           // Drawing routines?


           #if OSUnix
               const int resultID = XFillRectangles(Display *display, Drawable d, GC gc, XRectangle *rectangles, int nrectangles);
           #elif OSWindows
               const int resultID = FillRect();
           #endif


   Window XCreateSimpleWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, unsigned long border, unsigned long background);


           XGetErrorText();
           XGetErrorDatabaseText();

           XSetIOErrorHandler():
           int(int(*handler)(Display *));


    */

    return PXActionSuccessful;

#elif OSWindows

    // Does not exists?

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}



PXActionResult PXAPI PXNativDrawWindowCreate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo)
{
    PXWindow* const pxWindowParent = pxWindowCreateInfo->WindowParent;
    PXNativDrawWindowHandle pxWindowParentHandle = pxWindowParent ? pxWindowParent->Info.Handle.WindowID : PXNull;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativDraw",
        "Window",
        "Creating window ..."
    );
#endif

#if OSUnix
    Display* displayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;
    const int screenHandle = DefaultScreen(displayHandle);

    // if we dont have a parent, we need to use the root window
    if(!pxWindowParentHandle)
    {
        pxWindowParentHandle = RootWindow(displayHandle, screenHandle);
    }

#if 0
    const int attributeList[] =
    {
        GLX_RGBA,
        GLX_DEPTH_SIZE,
        24,
        GLX_DOUBLEBUFFER,
        None
    };

    const XVisualInfo* const visualInfo = glXChooseVisual(displayHandle, screenHandle, attributeList);

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
            "OK, (%p)",
            visualInfo
        );
#endif

}


    // default color map?

    // Create colormapping
    Colormap colormap = XCreateColormap
    (
        displayHandle,
        pxWindowParentHandle,
        visualInfo->visual,
        AllocNone
    );
    #endif











    // Create window
    {
        if(pxWindowCreateInfo->Simple)
        {
            pxWindow->Info.Handle.WindowID = XCreateSimpleWindow
            (
                displayHandle,
                pxWindowParentHandle,
                pxWindowCreateInfo->X,
                pxWindowCreateInfo->Y,
                pxWindowCreateInfo->Width,
                pxWindowCreateInfo->Height,
                pxWindowCreateInfo->BorderWidth,
                WhitePixel(displayHandle, screenHandle),
                BlackPixel(displayHandle, screenHandle)
            );

            if(pxWindow->Info.Handle.WindowID)
            {
                // Create events
              //  XSelectInput(displayHandle, pxWindow->Info.Handle.WindowID, inputEventsToListen);
            }
        }
        else
        {



            pxWindow->Info.Handle.WindowID = XCreateWindow
            (
                displayHandle,
                pxWindowParentHandle,
                pxWindowCreateInfo->X,
                pxWindowCreateInfo->Y,
                pxWindowCreateInfo->Width,
                pxWindowCreateInfo->Height,
                pxWindowCreateInfo->BorderWidth,
                0, //visualInfo->depth,
                0,//InputOutput,
                0,//visualInfo->visual,
                CWColormap | CWEventMask,
                0//&setWindowAttributes
            );
        }

        const PXBool successful = PXNull != pxWindow->Info.Handle.WindowID;

        if(!successful)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "X-System",
                "Window-Create",
                "Failed: ID:<%i>",
                pxWindow->Info.ID
            );
#endif

            return PXActionInvalid;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "X-System",
            "Window-Create",
            "OK, ID:%i, X11-ID:<0x%8.8x>\n"
            "%15s : %i,%i\n"
            "%15s : %ix%i\n"
            "%15s : %i\n"
            "%15s : %i",
            pxWindow->Info.ID,
            pxWindow->Info.Handle.WindowID,
            "Position", (int)pxWindowCreateInfo->X, (int)pxWindowCreateInfo->Y,
            "Size", (int)pxWindowCreateInfo->Width, (int)pxWindowCreateInfo->Height,
            "BorderWidth", pxWindowCreateInfo->BorderWidth,
            "Depth", 0//visualInfo->depth
        );
#endif
    }

   // const int flushResultID = XFlush(displayHandle);


    // Add callbacks
    const unsigned long inputEventsToListen =
           KeyPressMask |
       //    KeyReleaseMask |
       //    ButtonPressMask |
        //   ButtonReleaseMask |
        //   EnterWindowMask |
        //   LeaveWindowMask |
         //  PointerMotionMask |
         //  PointerMotionHintMask |
        //   Button1MotionMask |
        //   Button2MotionMask |
        //   Button3MotionMask |
        //   Button4MotionMask |
        //   Button5MotionMask |
        //   ButtonMotionMask |
         //  KeymapStateMask |
        ExposureMask |
        //  VisibilityChangeMask |
        //  StructureNotifyMask |
        //  ResizeRedirectMask |
        //  SubstructureNotifyMask |
         // SubstructureRedirectMask |
         // FocusChangeMask |
        //  PropertyChangeMask |
        //  ColormapChangeMask |
         // OwnerGrabButtonMask |
          // XI_RawMotion |
        0;


    //XSetWindowAttributes setWindowAttributes;
    //setWindowAttributes.cursor = ;
   // setWindowAttributes.colormap = colormap;
   // setWindowAttributes.event_mask = inputEventsToListen;


    XSelectInput(displayHandle, pxWindow->Info.Handle.WindowID, inputEventsToListen);


    // Attach to render engine
    {
        const int mapResultID = XMapWindow(displayHandle, pxWindow->Info.Handle.WindowID);

        // Linux doc states, mapwindow shall be ignored?
#if 0
        const PXBool success = 0 == mapResultID; // Success

        if(!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "X-System",
                "Window-Mapping",
                "Failed, Display:<%p>, X11-ID:<0x%8.8x>",
                displayHandle,
                pxWindow->Info.Handle.WindowID
            );
#endif

            return PXActionInvalid;
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
#endif
    }

    // Set events
    {

    }

    // Set title. Windows does this with the creation function but X11 does not.
    {
        const PXBool setTextSuccess = PXNativDrawTextSet(pxNativDraw, pxWindow, pxWindowCreateInfo->Name, -1);
    }

#elif OSWindows

    pxWindow->Info.Handle.WindowID = CreateWindowExA // Windows 2000, User32.dll, winuser.h
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
    const PXActionResult createResult = PXErrorCurrent(PXNull != pxWindow->Info.Handle.WindowID);

    if(PXActionSuccessful != createResult)
    {
        return createResult;
    }

    // Get additional device context for rendering purpose
    pxWindow->DeviceContextHandle = GetDC(pxWindow->Info.Handle.WindowID);

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

    pxWindow->Info.Flags |= PXResourceInfoExist | PXResourceInfoActive | PXResourceInfoStorageDevice;
    pxWindow->Position.Form.X = pxWindowCreateInfo->X;
    pxWindow->Position.Form.Y = pxWindowCreateInfo->Y;
    pxWindow->Position.Form.Width = pxWindowCreateInfo->Width;
    pxWindow->Position.Form.Height = pxWindowCreateInfo->Height;

    // Window create, register..
    PXDictionaryAdd(&pxNativDraw->ResourceManager->GUIElementLookup, &pxWindow->Info.Handle.WindowID, pxWindow);


#if PXLogEnable && 0
    const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

    PXLogPrint
    (
        PXLoggingError,
        "GUI",
        "Element-Create",
        "Failed: X:%4i, Y:%4i, W:%4i, H:%4i, (%s) : [%s]",
        (int)pxWindowCreateInfo->X,
        (int)pxWindowCreateInfo->Y,
        (int)pxWindowCreateInfo->Width,
        (int)pxWindowCreateInfo->Height,
        uielementName,
#if OSWindows
        pxGUIElementCreateInfo->WindowsTextContent
#else
        pxGUIElementCreateInfo->Name
#endif
    );
#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawRectangleParent(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXRectangleXYWHI32* const pxRectangleXYWHI32)
{
#if OSUnix
   /* Get the parent window */

    Display* const displayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;

    Window windowRoot;
    Window windowParent;
    Window* windowChildren;
    unsigned int* windowChildrenAmount = 0;

    XQueryTree
    (
        displayHandle,
        pxWindow->Info.Handle.WindowID,
        &windowRoot,
        &windowParent,
        &windowChildren,
        &windowChildrenAmount
    );

    /* Get parent window attributes */
    XWindowAttributes xWindowAttributes;


    XGetWindowAttributes
    (
        displayHandle,
        windowParent,
        &xWindowAttributes
    );

    /* Get the absolute position of the parent window */

    /*
    XTranslateCoordinates
    (
        displayHandle,
        parent_window,
        root_window,
        0,
        0,
        &abs_x,
        &child_return
    );
    */

    pxRectangleXYWHI32->X = 0;
    pxRectangleXYWHI32->Y = 0;
    pxRectangleXYWHI32->Width = xWindowAttributes.width - xWindowAttributes.x;
    pxRectangleXYWHI32->Height = xWindowAttributes.height - xWindowAttributes.y;


#elif OSWindows
    RECT rect;

    //const PXBool result = GetWindowRect(pxGUIElement->ID, &rect); // Windows 2000, User32.dll, winuser.h
    const PXBool result = GetClientRect(pxWindow->Info.Handle.WindowID, &rect); // Windows 2000, User32.dll, winuser.h
    // Get Last Error

    if(!result)
    {
        pxRectangleXYWHI32->X = 0;
        pxRectangleXYWHI32->Y = 0;
        pxRectangleXYWHI32->Width = 0;
        pxRectangleXYWHI32->Height = 0;

        return PXActionRefusedNotFound;
    }

    PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&rect, pxRectangleXYWHI32);
#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativeDraw",
        "Size-Parent",
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i - %s",
        pxWindow->Info.ID,
        pxRectangleXYWHI32->X,
        pxRectangleXYWHI32->Y,
        pxRectangleXYWHI32->Width,
        pxRectangleXYWHI32->Height,
        pxWindow->NameContent
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawWindowEventPendingAmount(PXNativDraw* const pxNativDraw, PXSize* const amount)
{
#if OSUnix

    *amount = XPending(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle); // X11

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
    glXSwapBuffers(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowID);
    pxActionResult = PXActionSuccessful; // TODO: Do we have error checking?

#elif OSWindows
    const PXBool result = SwapBuffers(pxWindow->DeviceContextHandle);
    pxActionResult = PXErrorCurrent(result);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "NativDraw",
        "BufferSwap",
        "%i",
        pxActionResult
    );
#endif

    return pxActionResult;
}



PXActionResult PXAPI PXNativDrawBegin(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow)
{
#if OSUnix

#elif OSWindows
    PAINTSTRUCT paintSturct;
    PXClear(PAINTSTRUCT, &paintSturct);

    const HDC hdc = BeginPaint(pxWindow->Info.Handle.WindowID, &paintSturct);
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXNativDrawEnd(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow)
{
#if OSUnix

#elif OSWindows
    PAINTSTRUCT paintSturct;
    PXClear(PAINTSTRUCT, &paintSturct);

    const PXBool end = EndPaint(pxWindow->Info.Handle.WindowID, &paintSturct);
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXNativDrawColorSetBrush(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowBrush* const pxGUIElementBrush, const char mode)
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

    return PXNativDrawSetV3(pxNativDraw, pxWindow, colorRef, mode);
}

PXActionResult PXAPI PXNativDrawSetV3(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXColorRGBI8* pxColorRGBI8, const char mode)
{
    PXColorRGBI8 noTexturePink = { 0xFF, 0, 0xFF };

    if(!(pxNativDraw && pxWindow))
    {
        return PXActionRefusedArgumentInvalid;
    }

    // if no color is provided, use the "no-texture-pink"
    if(!pxColorRGBI8)
    {
        pxColorRGBI8 = &noTexturePink;
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Brush-Set",
        "%2.2x%2.2x%2.2x",
        pxColorRGBI8->Red,
        pxColorRGBI8->Green,
        pxColorRGBI8->Blue
    );
#endif


    if(mode == PXGUIDrawModeFront)
    {
#if OSUnix
        const PXInt32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const int resultID = XSetForeground
        (
            pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
            pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
            color
        );
#elif OSWindows
        const COLORREF colorNew = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const COLORREF colorPrevious = SetTextColor(pxWindow->DeviceContextHandle, colorNew);
#endif
    }
    else
    {
        PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
        const PXInt32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const int resultID = XSetBackground
        (
            pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
            pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
            color
        );
#elif OSWindows

        SetBkMode(pxWindow->DeviceContextHandle, TRANSPARENT);

        const COLORREF colorNew = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
        const COLORREF colorPrevious = SetBkColor(pxWindow->DeviceContextHandle, colorNew);
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

PXActionResult PXAPI PXNativDrawSetRGB(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, char red, char green, char blue, const char mode)
{
    PXColorRGBI8 color = { red, green, blue };

    return PXNativDrawSetV3(pxNativDraw, pxWindow, &color, mode);
}

PXActionResult PXAPI PXNativDrawFontListFetch(PXNativDraw* const pxNativDraw)
{
#if OSUnix

    /*
     TODO: implement

    char **XListFonts(display, pattern, maxnames, actual_count_return)
      Display *display;
      char *pattern;
      int maxnames;
      int *actual_count_return;
*/

#elif OSWindows
    const HDC defaultDeviceContext = GetDC(NULL);

    LOGFONT logfont = { 0 };
    logfont.lfCharSet = DEFAULT_CHARSET;

    EnumFontFamiliesExA(defaultDeviceContext, &logfont, PXWindowsFontEnumCallBack, 0, 0);

    ReleaseDC(PXNull, defaultDeviceContext);
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXNativDrawFontLoadA(PXNativDraw* const pxNativDraw, PXFont* const pxFont, const char* const name, const PXSize nameLength)
{
#if OSUnix
     // "9x15" <--- linux font? Sometimes not found
    pxFont->Info.Handle.FontHandle = XLoadQueryFont //  XFontStruct*
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        name
    );

#elif OSWindows

    // "Consolas"
    const DWORD antialiased = (PXFontAntialiased & pxFont->Info.Behaviour) > 0 ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;
    const DWORD isItalics = (PXFontItalic & pxFont->Info.Behaviour) > 0;
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

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativDraw",
        "Font-Load",
        "PXID:%-4i %p %s",
        pxFont->Info.ID,
        pxFont->Info.Handle.FontHandle,
        name
    );
#endif

    PXTextCopyA(name, -1, pxFont->Name, 32);

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXNativDrawFontRelease(PXNativDraw* const pxNativDraw, PXFont* const pxFont)
{
#if OSUnix
    const int resultID = XFreeFont(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, 0);
#elif OSWindows

#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXNativDrawFontSelect(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXFont* const pxFont)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativDraw",
        "Font-Select",
        "PXID:%-4i %p %s",
        pxFont->Info.ID,
        pxFont->Info.Handle.FontHandle,
        pxFont->Name

    );
#endif

#if OSUnix
    const int resultID = XSetFont
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        pxFont->Info.Handle.FontHandle->fid
    );
#elif OSWindows
    const HFONT fontHandleOld = (HFONT)SelectObject(pxWindow->DeviceContextHandle, pxFont->Info.Handle.FontHandle);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawWindowProperty(PXNativDraw* const pxNativDraw, PXWindowPropertyInfo* const pxWindowPropertyInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXWindowPropertyInfo* const pxWindowPropertyInfo = &pxWindowPropertyInfoList[i];
        PXWindow* const pxWindow = pxWindowPropertyInfo->WindowCurrent;

        if(!pxWindow)
        {
            continue;
        }

        switch(pxWindowPropertyInfo->Property)
        {
            case PXUIElementPropertyTextContent:
            {
#if 0
                PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementUpdateInfo->Data.Text;

                if(!pxGUIElement)
                {
                    return PXActionInvalid;
                }

#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "Update-Text",
                    "(0x%p) %s",
                    (int)pxGUIElement->ID,
                    pxUIElementTextInfo->Content
                );
#endif

                PXWindowTextSet(pxGUISystem, pxGUIElement, pxUIElementTextInfo->Content);
#endif


#if OSWindows



                //   SendMessageA(pxGUIElement->ID, PBM_SETPOS, stepsConverted, 0);


#if 0
                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                PXWindowSizeInfo pxWindowSizeInfo;

                PXWindowID pxWindowID = pxGUIElementUpdateInfo->WindowReference ? pxGUIElementUpdateInfo->WindowReference->ID : PXNull;

                PXWindowSizeGet(pxWindowID, &pxWindowSizeInfo);

                pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Height;

                PXUIElementPositionCalculcate(pxGUIElement, &pxUIElementPositionCalulcateInfo);

                const PXBool succes54s = MoveWindow
                (
                    pxGUIElement->ID,
                    pxUIElementPositionCalulcateInfo.X,
                    pxUIElementPositionCalulcateInfo.Y,
                    pxUIElementPositionCalulcateInfo.Width,
                    pxUIElementPositionCalulcateInfo.Height,
                    PXTrue
                );
#endif
#endif




                break;
            }
            case PXUIElementPropertyTextAllign:
            {
                // PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows


                HDC xx = GetDC(pxWindow->Info.Handle.WindowID);

                LONG ww = SetWindowLongA(pxWindow->Info.Handle.WindowID, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

                // const UINT allign = SetTextAlign(xx, TA_CENTER);
                // const PXBool success = GDI_ERROR == allign;

                if(!ww)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        "UI",
                        "Text-Update",
                        "Allign Failed"
                    );
#endif
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "UI",
                    "Text-Update",
                    "Allign successful"
                );
#endif

#endif

                break;
            }
            case PXUIElementPropertySize:
            {
                PXWindowPropertyInfo sizeFetchInfo;
                PXClear(PXWindowPropertyInfo, &sizeFetchInfo);
                sizeFetchInfo.WindowCurrent = pxWindowPropertyInfo->WindowReference;
                sizeFetchInfo.Property = PXUIElementPropertySize;
                sizeFetchInfo.UpdateType = PXWindowPropertyUpdateTypeRead;

                PXNativDrawWindowProperty(pxNativDraw, &sizeFetchInfo, 1);

                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                //PXWindowSizeInfo pxWindowSizeInfo;
                //PXWindowID pxWindowID = pxGUIElement->Parent ? pxGUIElement->Parent->ID : PXNull;

                //PXWindowSizeGet(PXNull, &pxWindowSizeInfo); // TODO: Problematic

                pxUIElementPositionCalulcateInfo.WindowWidth = sizeFetchInfo.Data.Size.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = sizeFetchInfo.Data.Size.Height;

                PXUIElementPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);



                // If the window is an actual window, we will have bad allignment if we use MoveWindow
                //if(pxGUIElement->Type != PXUIElementTypeWindow)

                if(0)
                {
                    const PXBool success = MoveWindow
                    (
                        pxWindow->Info.Handle.WindowID,
                        pxUIElementPositionCalulcateInfo.X,
                        pxUIElementPositionCalulcateInfo.Y,
                        pxUIElementPositionCalulcateInfo.Width,
                        pxUIElementPositionCalulcateInfo.Height,
                        PXTrue
                    );
                }
#if 0
                else
                {
                    const BOOL result = SetWindowPos
                    (
                        pxGUIElement->ID,
                        PXNull,
                        pxUIElementPositionCalulcateInfo.X,
                        pxUIElementPositionCalulcateInfo.Y,
                        pxUIElementPositionCalulcateInfo.Width,
                        pxUIElementPositionCalulcateInfo.Height,
                        SWP_NOZORDER
                    );
                }
#endif

                // If we have a render window, we also need to fix the viewport
                if(PXUIElementTypeRenderFrame == pxWindow->Type)
                {
                    PXUIElementSceneRenderInfo* const pxUIElementSceneRenderInfo = &pxWindowPropertyInfo->Data.SceneRender;
                    PXEngine* pxEngine = pxUIElementSceneRenderInfo->Engine;

                    if(pxEngine)
                    {
                        PXViewPort pxViewPort;

                        PXFunctionInvoke(pxEngine->Graphic.ViewPortGet, pxEngine->Graphic.EventOwner, &pxViewPort);

                        pxViewPort.X = 0;//  pxUIElementPositionCalulcateInfo.X;
                        pxViewPort.Y = 0;// pxUIElementPositionCalulcateInfo.Y;
                        pxViewPort.Width = pxUIElementPositionCalulcateInfo.Width;
                        pxViewPort.Height = pxUIElementPositionCalulcateInfo.Height;

                        PXFunctionInvoke(pxEngine->Graphic.ViewPortSet, pxEngine->Graphic.EventOwner, &pxViewPort);
                    }

                }


#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "Update-Size",
                    "X:%4i, Y:%4i, W:%4i, H:%4i",
                    (int)pxUIElementPositionCalulcateInfo.X,
                    (int)pxUIElementPositionCalulcateInfo.Y,
                    (int)pxUIElementPositionCalulcateInfo.Width,
                    (int)pxUIElementPositionCalulcateInfo.Height
                );
#endif

                break;
            }
            case PXUIElementPropertyProgressbarPercentage:
            {
#if 0
                PXUIElementProgressBarInfo* const progressBar = &pxGUIElementUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                PXInt32U stepsConverted = progressBar->Percentage * 100;
                SendMessageA(pxGUIElement->Info.Handle.WindowID, PBM_SETPOS, stepsConverted, 0);
#endif
#endif

                break;
            }
            case PXUIElementPropertyProgressbarBarColor:
            {
#if 0
                PXUIElementProgressBarInfo* const progressBar = &pxGUIElementUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                COLORREF color = RGB(progressBar->BarColor.Red, progressBar->BarColor.Green, progressBar->BarColor.Blue);
                SendMessageA(pxGUIElement->Info.Handle.WindowID, PBM_SETBARCOLOR, 0, color);
#endif
#endif

                break;
            }
            case PXUIElementPropertyVisibility:
            {
#if OSUnix
#elif OSWindows

                PXBool show = pxWindowPropertyInfo->Show;
                int showID = show ? SW_SHOW : SW_HIDE;

                const PXBool isWindowValid = IsWindow(pxWindow->Info.Handle.WindowID);

                if(!isWindowValid)
                {
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "GUI",
                        "Visibility",
                        "%20s (0x%p), Invalid",
                        "",//pxGUIElement->NameData,
                        pxWindow->Info.Handle.WindowID
                    );

                    break;
                }

                if(show)
                {
                    pxWindow->Info.Flags |= PXResourceInfoRender;
                }
                else
                {
                    pxWindow->Info.Flags &= ~PXResourceInfoRender;
                }




                // EnableWindow(pxGUIElement->ID, show);

                ShowWindow(pxWindow->Info.Handle.WindowID, showID);
                BOOL res = ShowWindow(pxWindow->Info.Handle.WindowID, showID); // Twice to prevent some errors

                //  HWND parrent = GetParent(pxGUIElement->ID);

                //  HWND grandParrent = GetParent(parrent);

                //  UpdateWindow(pxGUIElement->ID);
                //  UpdateWindow(parrent);
                //  UpdateWindow(grandParrent);

                //  UpdateWindow(sourceObject);
                // FlashWindow(sourceObject, 1);

#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "Visibility",
                    "Page %20s, (0x%p), Mode:%s Status:%s",
                    "",//pxGUIElement->NameData,
                    pxGUIElement->Info.Handle.WindowID,
                    show ? "Show" : "Hide",
                    res ? "OK" : "FAIL"
                );
#endif


                // Own solution
                {
                  //  PXWindowChildListEnumerate(pxGUISystem, pxWindow, show);



                }


                const BOOL success = EnumChildWindows
                (
                    pxWindow->Info.Handle.WindowID,
                    PXWindowEnumChildProc,
                    &showID
                );
#endif

                break;
            }
            case PXUIElementPropertyComboBoxAdd:
            {
                PXUIElementComboBoxInfo* const pxUIElementComboBoxInfo = &pxWindowPropertyInfo->Data.ComboBox;

#if OSUnix
#elif OSWindows

                for(size_t i = 0; i < pxUIElementComboBoxInfo->DataListAmount; i++)
                {
                    char* name = pxUIElementComboBoxInfo->DataList[i];

                    ComboBox_AddString(pxWindow->Info.Handle.WindowID, name);
                }

                ComboBox_SetCurSel(pxWindow->Info.Handle.WindowID, 0);
#endif

                break;
            }
            case PXUIElementPropertyItemAdd:
            {
                PXUIElementTreeViewItemInfo* const pxUIElementTreeViewItemInfo = &pxWindowPropertyInfo->Data.TreeViewItem;

#if OSUnix
#elif OSWindows

                TVINSERTSTRUCT item;
                PXClear(TVINSERTSTRUCT, &item);

                const char text[] = "[N/A]";

                item.item.pszText = text;
                item.item.cchTextMax = sizeof(text);
                item.hInsertAfter = TVI_ROOT;
                item.item.mask = TVIF_TEXT;

                TreeView_InsertItem(pxWindow->Info.Handle.WindowID, &item);

#endif


                break;
            }
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawTextSet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const char* const text, const PXSize textLength)
{
    if(!(pxNativDraw && pxWindow && text && textLength))
    {
        return PXActionRefusedArgumentNull;
    }


    PXActionResult result = PXActionInvalid;

#if OSUnix
    // Will BadAlloc, BadWindow
    const int resultID = XStoreName(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowID, text);
    result = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    const PXBool success = SetWindowTextA(pxWindow->Info.Handle.WindowID, text);
    result = PXErrorCurrent(success);
#else
    result = PXActionRefusedNotSupportedByLibrary;
#endif

    if(PXActionSuccessful != result)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "GUI",
            "Window-Text",
            "Failed Set: %s on (0x%p)",
            text,
            pxWindow
        );
#endif

        return result;
    }

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Window-Text",
        "Set: %s on (0x%p)",
        text,
        pxGUIElement
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawTextGet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const char* const text, const PXSize textLength)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    pxActionResult = PXActionRefusedNotImplemented;
#elif OSWindows

    const int size = GetWindowTextA
    (
        pxWindow->Info.Handle.WindowID,
        (LPSTR)text,
        textLength
    );

#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}



PXActionResult PXAPI PXNativDrawWindowIsEnabled(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow)
{
#if OSUnix
    return PXFalse;
#elif OSWindows
    const BOOL success = IsWindowEnabled(pxWindow->Info.Handle.WindowID); // Windows 2000, User32.dll, winuser.h
    const PXActionResult pxActionResult = PXErrorCurrent(success);

    return pxActionResult;
#else
    return PXFalse;
#endif
}

PXActionResult PXAPI PXNativDrawWindowPosition(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXVector2I* const position, const PXBool doWrite)
{
    if(doWrite)
    {
#if OSUnix
        const int resultID = XMoveWindow(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowID, position->X, position->Y);
        const PXActionResult pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop
        // Note:
        // MoveWindow() is a bad function. SetWindowPos() seems to be better in every case.

        const UINT flags = SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER;
        const PXBool success = SetWindowPos(pxWindow->Info.Handle.WindowID, PXNull, position->X, position->Y, PXNull, PXNull, flags); // Windows 2000, User32.dll
        const PXActionResult pxActionResult = PXErrorCurrent(success);

        return pxActionResult;

#else
        return PXActionRefusedNotSupportedByLibrary;
#endif
    }
    else
    {
#if OSUnix
        return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
        RECT rectangle;

        const PXBool success = GetWindowRect(pxWindow->Info.Handle.WindowID, &rectangle); // Windows 2000, User32.dll, winuser.h
        const PXActionResult pxActionResult = PXErrorCurrent(success);

        if(PXActionSuccessful != pxActionResult)
        {
            position->X = -1;
            position->Y = -1;

            return pxActionResult;
        }

        position->X = rectangle.left;
        position->Y = rectangle.top;
    //   window->Width = rectangle.right - rectangle.left;
    //   window->Height = rectangle.bottom - rectangle.top;

        return PXActionSuccessful;

#else
        return PXActionNotSupportedByOperatingSystem;
#endif
    }
}

PXActionResult PXAPI PXNativDrawWindowMove(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y)
{

}

PXActionResult PXAPI PXNativDrawWindowResize(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int width, const int height)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    const int resultID = 0;//XResizeWindow(Display *display, Window w, width, height);
    pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop
    const UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER;
    const PXBool success = SetWindowPos(pxWindow->Info.Handle.WindowID, PXNull, PXNull, PXNull, width, height, flags); // Windows 2000, User32.dll
    pxActionResult = PXErrorCurrent(success);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXActionResult PXAPI PXNativDrawWindowMoveAndResize(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    const int resultID = 0;//XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned int width, unsigned int height);
    pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop
    const PXBool success = MoveWindow(pxWindow->Info.Handle.WindowID, x, y, width, height, PXTrue); // Windows 2000, User32.dll, winuser.h
    pxActionResult = PXErrorCurrent(success);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXActionResult PXAPI PXNativDrawIcon(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXIcon* const pxIcon, const int x, const int y, const int size)
{
#if OSUnix
#elif OSWindows
    const BOOL success = DrawIconEx
    (
        pxWindow->DeviceContextHandle,
        x,
        y,
        pxIcon->Info.Handle.IconHandle,
        size, // Width same as height
        size,
        0,
        0,
        DI_NORMAL
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawCursorPosition(PXNativDraw* const pxNativDraw, PXVector2I* const position, const PXBool isWrite)
{
#if OXUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    BOOL success = 0;

    if(!isWrite)
    {
        POINT point;

        success = GetCursorPos(&point); // Windows 2000, user32.dll, winuser.h

        position->X = point.x;
        position->Y = point.y;
    }
    else
    {
        success = SetCursorPos(position->X, position->Y); // Windows 2000, user32.dll, winuser.h
    }

    const PXActionResult result = PXErrorCurrent(success);

    return result;

#else
    return PXActionRefusedTypeNotSupported;
#endif
}

PXActionResult PXAPI PXNativDrawCursorCollisionCheck(PXNativDraw* const pxNativDraw)
{
    const int cursorSize = 16;
    PXVector2I cursorPosition;

    PXNativDrawCursorPosition(pxNativDraw, &cursorPosition, 0);

    PXDictionary* const pxDictionaryUI = &pxNativDraw->ResourceManager->GUIElementLookup;

    for(size_t i = 0; i < pxDictionaryUI->EntryAmountCurrent; i++)
    {
        PXVector2I windowPosition;

        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(pxDictionaryUI, i, &pxDictionaryEntry);

        PXWindow* const pxWindow = *(PXWindow**)pxDictionaryEntry.Value;

        #if OSWindows
        RECT winRec;

        GetWindowRect(pxWindow->Info.Handle.WindowID, &winRec);
       // PXNativDrawWindowPosition(pxNativDraw, pxWindow, &windowPosition, 0);

        const PXBool isColliding = PXCollisionAABB
        (
            winRec.left,
            winRec.top,
            winRec.right,// - winRec.left,
            winRec.bottom,// - winRec.top,
            cursorPosition.X - cursorSize,
            cursorPosition.Y - cursorSize,
            cursorPosition.X + cursorSize,
            cursorPosition.Y + cursorSize
        );

        #endif // OSWindows
        const PXBool shallDetect = (pxWindow->Info.Behaviour & PXWindowBehaviourHoverable);
        const PXInt32U valueBefore = pxWindow->Info.Behaviour;

        if(!shallDetect)
        {
          //  continue;
        }


            #if OSWindows
        if(isColliding)
        {
            pxWindow->Info.Behaviour |= PXWindowBehaviourIsBeingHovered;
        }
        else
        {
            pxWindow->Info.Behaviour &= ~PXWindowBehaviourIsBeingHovered;
        }


        const PXBool valueChanged = valueBefore != pxWindow->Info.Behaviour;

        if(valueChanged)
        {
            const BOOL xx = RedrawWindow(pxWindow->Info.Handle.WindowID, PXNull, PXNull, RDW_INVALIDATE);

            PXBool hovernow = (pxWindow->Info.Behaviour & PXWindowBehaviourIsBeingHovered) > 0;

            PXLogPrint
            (
                PXLoggingInfo,
                "UI-Event",
                "Hover",
                "ID:%i - %s",
                pxWindow->Info.ID,
                hovernow ? "Yes" : "No"
            );
        }
         #endif // OSWindows
    }

    return PXActionSuccessful;
}

PXNativDraw* PXAPI PXNativDrawInstantance(void)
{
    return &_internalNativDraw;
}

PXActionResult PXAPI PXNativDrawWindowEventPoll(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow)
{
    if(!(pxNativDraw && pxWindow))
    {
        return PXActionRefusedArgumentNull;
    }

    if(!pxWindow->Info.Handle.WindowID)
    {
        return PXActionRefusedArgumentInvalid;
    }

    // pxWindow->MouseCurrentInput.ButtonsDelta = 0; // Reset mouse data

#if OSUnix
    XEvent windowEvent;

    assert(pxNativDraw->GUISystem);
    assert(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle);

    Display* displayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;

    /*
        Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;
        unsigned int mask_return;
        /*
         * We need:
         *     child_return - the active window under the cursor
         *     win_{x,y}_return - pointer coordinate with respect to root window
         * /
        int retval = XQueryPointer
        (
            display,
            root_window,
            &root_return,
            &child_return,
            &root_x_return,
            &root_y_return,
            &win_x_return,
            &win_y_return,
            &mask_return
        );*/



    // Multithread lock
    XLockDisplay(displayHandle);

    // poll all events
    for(;;)
    {
        // Check if we have events
        const int amountOfEventsPending = XPending(displayHandle);

        if(0 == amountOfEventsPending)
        {
            // Done
            break;
        }

        for(PXInt32U i = 0; i < amountOfEventsPending; ++i)
        {
            // Fetch event form the display
            XNextEvent(displayHandle, &windowEvent);

            // Translate event
            PXNativDrawEventTranslator(pxNativDraw, &windowEvent);
        }
    }

    // Multithread unlock
    XUnlockDisplay(displayHandle);

#elif PXOSWindowsDestop
    const HWND windowHandle = pxWindow->Info.Handle.WindowID;

    for(;;)
    {
        MSG message;

        const PXBool peekResult = PeekMessage(&message, windowHandle, 0, 0, PM_NOREMOVE); // Windows 2000, User32.dll, winuser.h

        if(!peekResult)
        {
            break; // Stop, no more messages
        }

        const PXBool messageResult = GetMessage(&message, windowHandle, 0, 0); // Windows 2000, User32.dll, winuser.h

        if(!messageResult)
        {
            continue; // Skip, message fetch failed
        }

        const BOOL translateResult = TranslateMessage(&message); // Windows 2000, User32.dll, winuser.h
        const LRESULT dispatchResult = DispatchMessage(&message); // Windows 2000, User32.dll, winuser.h
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawWindowIDValid(PXNativDraw* const pxNativDraw, const PXNativDrawWindowHandle pxNativDrawWindowHandle)
{
    if(!(pxNativDraw && pxNativDrawWindowHandle))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix

    // This function does not exist, we emulate this behaviour
    // by just fetching attributes. if we fail, the ID is invalid.
    PXWindow pxWindow;

    const PXActionResult fetchResult = PXNativDrawWindowFetch(pxNativDraw, &pxWindow, pxNativDrawWindowHandle);

    return fetchResult;

#elif OSWindows

    // Dont handle windows that are not your own.
    // The window could be destroyed right after this function returns yielding invalid results

    const BOOL isWindow = IsWindow(pxNativDrawWindowHandle);

    if(!isWindow)
    {
        return PXActionRefusedObjectIDInvalid;
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXNativDrawWindowFetch(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const PXNativDrawWindowHandle pxNativDrawWindowHandle)
{
#if OSUnix

    XWindowAttributes attributes;

    const Status status = XGetWindowAttributes(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxNativDrawWindowHandle, &attributes);
    const PXBool validID = 0 != status;

    if(!validID)
    {
        return PXActionRefusedObjectIDInvalid;
    }

    return PXActionSuccessful;

#elif OSWindows

    return PXActionRefusedNotImplemented;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

void PXAPI PXNativDrawScreenSizeGet(PXRectangleXYWHI32* const pxRectangleXYWH)
{
#if OSUnix

    // To direct eqivilant, emulate with X11.
    Display* const displayHandle = XOpenDisplay(NULL);

    if(!displayHandle)
    {
        return;
    }

    const int screenHandle = DefaultScreen(displayHandle);

    pxRectangleXYWH->X      = 0;
    pxRectangleXYWH->Y      = 0;
    pxRectangleXYWH->Width  = XDisplayWidth(displayHandle, screenHandle);
    pxRectangleXYWH->Height = XDisplayHeight(displayHandle, screenHandle);

    XCloseDisplay(displayHandle);

#elif PXOSWindowsDestop
    RECT desktop;

    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();

    if(!hDesktop)
    {
        return;
    }

    // Get the size of screen to the variable desktop
    const BOOL rectResult = GetWindowRect(hDesktop, &desktop);

    if(!rectResult)
    {
        return;
    }

    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)

    pxRectangleXYWH->X = desktop.left;
    pxRectangleXYWH->Y = desktop.top;
    pxRectangleXYWH->Width = desktop.right;
    pxRectangleXYWH->Height = desktop.bottom;
#else
    pxRectangleXYWH->X = 0;
    pxRectangleXYWH->Y = 0;
    pxRectangleXYWH->Width = 0;
    pxRectangleXYWH->Height = 0;
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Monitor",
        "Size-Get",
        "%i x %i",
        pxRectangleXYWH->Width,
        pxRectangleXYWH->Height
    );
#endif
}

PXActionResult PXAPI PXNativDrawClear(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativeDraw",
        "Clear",
        "PXID:%i",
        pxGUIElement->Info.ID
    );
#endif

#if OSUnix
    const int resultID = XClearWindow(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.Handle.WindowID);
#elif OSWindows
    // Does this exists?

    //GetUpdateRect();
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXNativDrawTextA
(
    PXNativDraw* const pxNativDraw,
    PXWindow* const pxWindow,
    int x,
    int y,
    int width,
    int height,
    const char* const text,
    const PXSize textSize
)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativeDraw",
        "Text",
        "PXID:%-4i X:%-4i Y:%-4i %2i %s",
        pxWindow->Info.ID,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        textSize,
        text
    );
#endif

    if(!(text && textSize))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix

    PXRectangleXYWHI32 pxRectangleXYWHI32;

    PXNativDrawRectangleParent(pxNativDraw, pxWindow, &pxRectangleXYWHI32);

    int xMX = x;
    int yMX = y;


    if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignLeft))
    {
        xMX = pxRectangleXYWHI32.X + x;
    }

    if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignRight))
    {
        xMX = width - (pxRectangleXYWHI32.X + x);
    }


    if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignTop))
    {
        yMX = pxRectangleXYWHI32.Y + y;
    }

    if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignBottom))
    {
        yMX = height - (pxRectangleXYWHI32.Y + y);
    }

    if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignCenter))
    {
        yMX = (height / 2) - (pxRectangleXYWHI32.Y + y);
    }


    // For ANSI and UTF-8 strings
    const int resultID = XDrawString
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxWindow->Info.Handle.WindowID,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        xMX,
        yMX,
        text,
        textSize
    );
#elif OSWindows

    const PXRectangleXYWHI32 pxRectangleXYWHI32 = {x, y, width, height};

    RECT rectangle;
    PXRectangleXYWHI32ToLTRBI32(&pxRectangleXYWHI32, (PXRectangleLTRBI32*)&rectangle);

    UINT format = DT_SINGLELINE | DT_NOCLIP;
    format |= PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignTop) * DT_TOP;
    format |= PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignLeft) * DT_LEFT;
    format |= PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignRight) * DT_RIGHT;
    format |= PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignBottom) * DT_BOTTOM;

    if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignCenter))
    {
        format |= DT_VCENTER | DT_CENTER;
    }

    const int nextHeightBBB = DrawTextExA
    (
        pxWindow->DeviceContextHandle,
        text,
        textSize,
        &rectangle,
        format,
        PXNull
    ); // Windows 2000, User32.dll, winuser.h
#endif
}

PXActionResult PXAPI PXNativDrawTextW(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const wchar_t* const text, const PXSize textSize)
{
#if OSUnix
    // For UNICODE
    // const int resultID = XDrawString16(Display * display, Drawable d, GC gc, int x, int y, XChar2b * string, int length);

#elif OSWindows

    RECT rectangle;
    UINT format;

    const int nextHeight = DrawTextW(pxWindow->DeviceContextHandle, text, textSize, &rectangle, format); // Windows 2000, User32.dll, winuser.h
    const PXBool success = 0 != nextHeight;
    const PXActionResult result = PXErrorCurrent(success);

    return result;

#else
    return NotSupport;
#endif
}

PXActionResult PXAPI PXNativDrawPoint(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y)
{
#if OSUnix
    const int resultID = 0;//XDrawPoint(Display *display, Drawable d, GC gc, int x, int y);
    const PXActionResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXNativDrawPoints(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix
    const int resultID = 0;//XDrawPoints(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
    const PXActionResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXNativDrawLine(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x1, const int y1, const int x2, const int y2)
{
#if OSUnix
    const resultID = 0;//XDrawLine(Display *display, Drawable d, GC gc, x1, y1, x2, y2);
    const PXActionResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows

    POINT point;

    // Move to a start point, no drawing happens here
    const BOOL successMove = MoveToEx(pxGUIElement->DeviceContextHandle, x1, y1, &point);

    // use the previous step to draw a line from there to here
    const BOOL successDraw = LineTo(pxGUIElement->DeviceContextHandle, x2, y2);

#else
    return PXNotSupport;
#endif
}

PXActionResult PXAPI PXNativDrawLines(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix
    XPoint points;
    int npoints = 0;
    int mode = 0;

    const resultID = XDrawLines
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxGUIElement->Info.Handle.WindowID,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        &points,
        npoints,
        mode
    );
    const PXActionResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    const BOOL success = PolylineTo(pxGUIElement->DeviceContextHandle, 0, 0);
#else
    return PXNotSupport;
#endif
}

PXActionResult PXAPI PXNativDrawRectangle(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativeDraw",
        "Draw-Rectangle",
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        x,
        y,
        width,
        height
    );
#endif

    const PXBool isHovered = (PXWindowBehaviourIsBeingHovered & pxWindow->Info.Behaviour) > 0;

    PXActionResult pxActionResult = PXActionInvalid;

    PXWindowBrush* const brushForeground = pxWindow->BrushFront;
    PXWindowBrush* const brushBackGround = pxWindow->BrushBackground;

#if OSUnix
    PXNativDrawColorSetBrush(pxNativDraw, pxWindow, brushBackGround, PXGUIDrawModeBack);

    const resultID = XFillRectangle
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxWindow->Info.Handle.WindowID,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        x,
        y,
        width,
        height
    );
    // pxActionResultresult = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    //  const BOOL bbbbb = SelectObject(pxGUIElement->DeviceContextHandle, GetStockObject(GRAY_BRUSH));



    RECT rect;
    PXRectangleXYWHI32 pxRectangleXYWHI32 = {x, y, width, height};
    PXRectangleXYWHI32ToLTRBI32(&pxRectangleXYWHI32, (PXRectangleLTRBI32*)&rect);

    // const COLORREF color = RGB(255, 0, 200);
    // const HBRUSH brushAA = CreateSolidBrush(color);

    if(isHovered)
    {
        const BOOL aaaaaa = FillRect(pxWindow->DeviceContextHandle, &rect, brushForeground->Info.Handle.BrushHandle);
    }
    else
    {
        const BOOL aaaaaa = FillRect(pxWindow->DeviceContextHandle, &rect, brushBackGround->Info.Handle.BrushHandle);
    }



    // DeleteObject();


    const BOOL success = 1;// Rectangle(pxGUIElement->DeviceContextHandle, x, y, width, height);

    if(!success)
    {
        pxActionResult = PXActionInvalid;
    }

#else
    pxActionResult = PXNotSupport;
#endif


#if PXLogEnable && 0
    if(PXActionSuccessful == pxActionResult)
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "GUI",
            "Draw-Button",
            "%i, X:%i,Y:%i,W:%i,H:%i",
            pxGUIElement->Info.ID,
            x, y, width, height
        );
    }
    else
    {
        PXLogPrint
        (
            PXLoggingInfo,
            "GUI",
            "Draw-Rectangle",
            "%i, X:%i,Y:%i,W:%i,H:%i",
            pxGUIElement->Info.ID,
            x, y, width, height
        );
    }
#endif

    return pxActionResult;
}

PXActionResult PXAPI PXNativDrawRectangleRounded(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix

    // does not exist?

#elif OSWindows


    const BOOL success = RoundRect
    (
        pxGUIElement->DeviceContextHandle,
        0,
        0,
        0,
        0,
        0,
        0
    );
#endif
}

PXActionResult PXAPI PXNativDrawIconLoad(PXNativDraw* const pxGUISystem, PXIcon* const pxIcon, const char* iconName)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "NativDraw",
        "Icon-Load",
        "%s",
        iconName
    );
#endif


#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    HINSTANCE instanceHandle = NULL;

    const HICON iconHandle = LoadIconA
    (
        instanceHandle,
        IDI_EXCLAMATION
    );

    pxIcon->Info.Handle.IconHandle = iconHandle;

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXNativDrawIconDraw(PXNativDraw* const pxGUISystem, PXWindow* const pxGUIElement, PXIcon* const pxIcon, const int x, const int y, const int width, const int height)
{
    if(!(pxGUISystem && pxGUIElement && pxIcon))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    const BOOL result = DrawIconEx
    (
        pxGUIElement->DeviceContextHandle,
        x,
        y,
        pxIcon->Info.Handle.IconHandle,
        width,
        height,
        0,
        0,
        DI_NORMAL
    );

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXNativDrawEventConsumer(PXNativDraw* const pxNativDraw, PXWindowEvent* const pxWindowEvent)
{
    // Invoke

    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeElementClick:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event-Click",
                "Sender (%0xp)",
                pxWindowEvent->UIElementSender->Info.ID
            );

            break;
        }
        case PXWindowEventTypeElementSelect:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event-Select",
                "ID:<%i>"
            );
            break;
        }
        case PXWindowEventTypeElementDestroy:
        {

            // window->IsRunning = PXFalse;

            break;
        }

        case PXWindowEventTypeElementMove:
        {
#if PXLogEnable && 1
            PXLogPrint
            (
                PXLoggingEvent,
                "Window",
                "Event-Move",
                ""
            );
#endif

            break;
        }
        case PXWindowEventTypeElementResize:
        {
            PXWindowEventResize* const pxWindowEventResize = &pxWindowEvent->Resize;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                "Window",
                "Event-Resize",
                "<%ix%i>",
                pxWindowEventResize->Width,
                pxWindowEventResize->Height
            );
#endif

            // PXWindowhSizeRefresAll(pxGUISystem, pxWindowEvent->UIElementReference);


            // window->Width = pxWindowEventResize->Width;
            // window->Height = pxWindowEventResize->Height;
            // window->HasSizeChanged = PXYes;

            //  PXFunctionInvoke(window->WindowSizeChangedCallBack, window->EventReceiver, window);

            break;
        }
        case PXWindowEventTypeInputMouseButton:
        {
            break;
        }
        case PXWindowEventTypeInputMouseMove:
        {
            break;
        }
        case PXWindowEventTypeInputKeyboard:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    if(pxWindowEvent->UIElementReference)
    {
        PXWindow* const pxGUIElement = pxWindowEvent->UIElementReference;

        if(pxGUIElement->InteractCallBack)
        {
            pxGUIElement->InteractCallBack(pxGUIElement->InteractOwner, pxWindowEvent);
        }
    }

    if(pxWindowEvent->UIElementSender)
    {
        PXWindow* const pxGUIElement = pxWindowEvent->UIElementSender;

        if(pxGUIElement->InteractCallBack)
        {
            pxGUIElement->InteractCallBack(pxGUIElement->InteractOwner, pxWindowEvent);
        }
    }

    return PXActionSuccessful;
}


#if OSUnix
void PXNativDrawEventTranslator(PXNativDraw* const pxNativDraw, const XEvent* const xEventData)
{
    PXGUISystem* pxGUISystem = pxNativDraw->GUISystem;

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.EventData = xEventData;

    switch(xEventData->type)
    {
        case KeyPress:
        case KeyRelease:
        {
            const XKeyEvent* keyEvent = &xEventData->xkey;
            const unsigned int keyCode = keyEvent->keycode;
            const PXBool release = KeyRelease == xEventData->type;
            const KeySym keySym = XKeycodeToKeysym(pxGUISystem->DisplayCurrent.DisplayHandle, keyCode, 0);
            const char* keyName = XKeysymToString(keySym);

            pxWindowEvent.Type = PXWindowEventTypeInputKeyboard;
            pxWindowEvent.InputKeyboard.VirtualKey = PXVirtualKeyFromID(keySym);
            pxWindowEvent.InputKeyboard.CharacterID = keySym;

            switch(xEventData->type)
            {
                case KeyPress:
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateDown;
                    break;

                case KeyRelease:
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateUp;
                    break;

                default:
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateInvalid;
                    break;
            }

            PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case ButtonRelease:
        case ButtonPress:
        {
            const XButtonEvent* buttonEvent = &xEventData->xbutton;

            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;

            switch(xEventData->type)
            {
                case ButtonPress:
                    pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;
                    break;

                case ButtonRelease:
                    pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;
                    break;

                default:
                    pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateInvalid;
                    break;
            }

            switch(buttonEvent->button)
            {
                /*
                case MouseButtonLeft:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
                    break;

                case MouseButtonMiddle:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
                    break;

                case MouseButtonRight:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
                    break;

                case MouseScrollUp:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonScrollUp;
                    break;

                case MouseScrollDown:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonRightDown;
                    break;
                    */

                default:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonInvalid;
                    break;
            }

            PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case MotionNotify:
        {
            // printf("[Event] MotionNotify \n");

            //  pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
            //  PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case EnterNotify:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusEnter;
            PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case LeaveNotify:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusLeave;
            PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case FocusIn:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusEnter;
            // PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case FocusOut:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusLeave;
            // PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case KeymapNotify:
        {
            printf("[Event] KeymapNotify \n");

            break;
        }
        case Expose:
        {
            XExposeEvent* const xExposeEventData = &xEventData->xexpose;
            PXWindow* windowCurrent = PXNull;

            if(pxNativDraw->ResourceManager)
            {
                PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &xExposeEventData->window, &pxWindowEvent.UIElementReference);
            }

            windowCurrent = pxWindowEvent.UIElementReference;

            if(windowCurrent)
            {
                PXWindowDrawInfo pxWindowDrawInfo;
                PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);
                pxWindowDrawInfo.RectangleXYWH.X = xExposeEventData->x;
                pxWindowDrawInfo.RectangleXYWH.Y = xExposeEventData->y;
                pxWindowDrawInfo.RectangleXYWH.Width = xExposeEventData->width;
                pxWindowDrawInfo.RectangleXYWH.Height = xExposeEventData->height;
                pxWindowDrawInfo.ScreenIDHandle = DefaultScreen(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle);
                pxWindowDrawInfo.DisplayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;
                pxWindowDrawInfo.WindowIDHandle = xExposeEventData->window;
                pxWindowDrawInfo.GraphicContntainerHandle = DefaultGC(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindowDrawInfo.ScreenIDHandle);

                // Sync current pos with element
                windowCurrent->Position.Form.X = xExposeEventData->x;
                windowCurrent->Position.Form.Y = xExposeEventData->y;
                windowCurrent->Position.Form.Width = xExposeEventData->width;
                windowCurrent->Position.Form.Height = xExposeEventData->height;


                const PXBool shallDraw = windowCurrent->DrawFunction;

                if(windowCurrent->DrawFunction)
                {
                    if(PXResourceInfoRender & windowCurrent->Info.Flags)
                    {
                        printf("[Event] Expose <%8.8x>. Custom draw for PXID:%i\n", windowCurrent->Info.ID);

                        PXNativDrawClear(pxNativDraw, windowCurrent);

                        windowCurrent->DrawFunction(pxNativDraw->GUISystem, windowCurrent, &pxWindowDrawInfo);
                    }
                    else
                    {
                        printf("[Event] Expose <%8.8x>. Skipped draw for PXID:%i, not visible!\n", windowCurrent->Info.ID);
                    }
                }
                else
                {
                    printf("[Event] Expose <%8.8x>. No custom draw for PXID:%i\n", windowCurrent->Info.ID);
                }

            }
            else
            {
                printf("[Event] Expose <%8.8x>. No window registerd!\n", xExposeEventData->window);
            }

           // PXNativDrawWindowBufferSwap(pxNativDraw, pxWindowEvent.UIElementReference); ??

            break;
        }
        case GraphicsExpose:
        {
            printf("[Event] PXGraphicsExpose \n");

            break;
        }
        case NoExpose:
        {
            printf("[Event] NoExpose \n");

            break;
        }
        case VisibilityNotify:
        {
            printf("[Event] VisibilityNotify \n");

            break;
        }
        case CreateNotify:
        {
            printf("[Event] CreateNotify \n");

            break;
        }
        case DestroyNotify:
        {
            printf("[Event] DestroyNotify \n");

            break;
        }
        case UnmapNotify:
        {
            printf("[Event] UnmapNotify \n");

            break;
        }
        case MapNotify:
        {
            printf("[Event] MapNotify \n");

            break;
        }
        case MapRequest:
        {
            printf("[Event] MapRequest \n");

            break;
        }
        case ReparentNotify:
        {
            printf("[Event] ReparentNotify \n");

            break;
        }
        case ConfigureNotify:
        {
            printf("[Event] ConfigureNotify \n");

            break;
        }
        case ConfigureRequest:
        {
            printf("[Event] Destroyed \n");

            break;
        }
        case GravityNotify:
        {
            printf("[Event] GravityNotify \n");

            break;
        }
        case ResizeRequest:
        {
            const XResizeRequestEvent* resizeRequestEvent = &xEventData->xresizerequest;
            const int width = resizeRequestEvent->width;
            const int height = resizeRequestEvent->height;

            pxWindowEvent.Type = PXWindowEventTypeElementResize;
            pxWindowEvent.Resize.Width = width;
            pxWindowEvent.Resize.Height = height;

            PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case CirculateNotify:
        {
            printf("[Event] CirculateNotify \n");

            break;
        }
        case CirculateRequest:
        {
            printf("[Event] CirculateRequest \n");

            break;
        }
        case PropertyNotify:
        {
            printf("[Event] PropertyNotify \n");

            break;
        }
        case SelectionClear:
        {
            printf("[Event] SelectionClear \n");

            break;
        }
        case SelectionRequest:
        {
            printf("[Event] SelectionRequest \n");

            break;
        }
        case SelectionNotify:
        {
            printf("[Event] SelectionNotify \n");

            break;
        }
        case ColormapNotify:
        {
            printf("[Event] ColormapNotify \n");

            break;
        }
        case ClientMessage:
        {
            printf("[Event] ClientMessage \n");

            break;
        }
        case MappingNotify:
        {
            printf("[Event] MappingNotify \n");

            break;
        }
        case GenericEvent:
        {
            const XGenericEventCookie* const cookie = &xEventData->xcookie; // Make Copy
            const int result = XGetEventData(pxGUISystem->DisplayCurrent.DisplayHandle, &cookie);
            const unsigned char sucessful = result != 0 && cookie->data;

            if(sucessful)
            {
                switch(cookie->evtype)
                {
                    case XI_RawMotion:
                    {
                        XIRawEvent* re = (XIRawEvent*)cookie->data;

                        if(re->valuators.mask_len)
                        {
                            const double* values = re->raw_values;
                            const unsigned char isX = XIMaskIsSet(re->valuators.mask, 0);
                            const unsigned char isY = XIMaskIsSet(re->valuators.mask, 1);
                            double xpos = 0;
                            double ypos = 0;

                            if(isX)
                            {
                                xpos += *values;
                                ++values;
                            }

                            if(isY)
                            {
                                ypos += *values;
                            }

                            pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
                            pxWindowEvent.InputMouseMove.AxisX = xpos;
                            pxWindowEvent.InputMouseMove.AxisY = ypos;
                            pxWindowEvent.InputMouseMove.DeltaX = 0;// mouse->Position[0] - xpos;
                            pxWindowEvent.InputMouseMove.DeltaY = 0;// mouse->Position[1] - ypos;

                            PXNativDrawEventConsumer(pxGUISystem, &pxWindowEvent);
                        }
                    }
                }
            }
            else
            {
                printf("[Event] GenericEvent %i\n", cookie->evtype);
            }

            XFreeEventData(pxGUISystem->DisplayCurrent.DisplayHandle, &cookie);

            break;
        }
        default:
        {
            printf("[Event] default: unkown event \n");

            break;
        }
    }
}
#elif OSWindows
LRESULT CALLBACK PXNativDrawEventTranslator(const HWND windowID, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    PXNativDraw* const pxNativDraw = PXNativDrawInstantance();

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowHandle = windowID;
    pxWindowEvent.EventID = eventID;
    pxWindowEvent.ParamW = wParam;
    pxWindowEvent.ParamL = lParam;

    if(pxNativDraw->ResourceManager)
    {
        PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &windowID, &pxWindowEvent.UIElementReference);
    }

#if 0
    PXLogPrint
    (
        PXLoggingInfo,
        "Windows",
        "Event",
        "ID:%4i, Parent (0x%p), Sender (0x%p)",
        eventID,
        windowID,
        pxWindowEvent.UIElementReference
    );
#endif

    switch(eventID)
    {
        default:
        case WM_NULL:
        {
            break; // Do nothing
        }
        case WM_CREATE: // Gets called inside the "CreateWindow" function.
        {
            // Do nothíng here, as it's too soon to regard the window as 'created'

            PXWindow pxWindow;
            pxWindow.Info.Handle.WindowID = windowID;

           // PXNativeDrawMouseTrack(&pxWindow);

            return DefWindowProc(windowID, eventID, wParam, lParam);
        }
        case WM_DESTROY:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementDestroy;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_MOVE:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementMove;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_SIZE:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementResize;
            pxWindowEvent.Resize.Width = LOWORD(lParam);
            pxWindowEvent.Resize.Height = HIWORD(lParam);

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_CLOSE:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementClose;
            pxWindowEvent.Close.CommitToClose = PXTrue;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            if(!pxWindowEvent.Close.CommitToClose)
            {
                return PXFalse;
            }

            break;
        }
        case WM_SETCURSOR:
        {
            const HWND windowsHandle = (HWND)wParam;
            const WORD hitTestResult = LOWORD(lParam);
            const WORD sourceMessage = HIWORD(lParam);
            const PXBool showCursor = PXTrue;//!(window->CursorModeCurrent == PXWindowCursorInvisible || window->CursorModeCurrent == PXWindowCursorLockAndHide);

            if(showCursor)
            {
                while(ShowCursor(1) < 0);
            }
            else
            {
                while(ShowCursor(0) >= 0);
            }

            return 1; // prevent further processing
        }
        case WM_NOTIFY:
        {
            const HWND windowsHandle = (HWND)wParam;
            NMHDR* const notificationCode = (NMHDR*)lParam;
            HWND sourceObject = notificationCode->hwndFrom;

            switch(notificationCode->code)
            {
                case LVN_BEGINDRAG:
                {
                    //  if(!bDragging)
                    //  break;

                    // p.x = LOWORD(lParam);
                    //  p.y = HIWORD(lParam);

                    //ClientToScreen(hWndMain, &p);
                    // ImageList_DragMove(p.x, p.y);

                    break;
                }
                case TVN_SELCHANGED:
                {
                    // NOTE:
                    // On treeview item selection, "wParam" will might always be NULL.
                    // It's a copy of the item "lParam" parameter on creation.

                    PXWindow* pxTreeViewContainer = PXNull;
                    PXWindow* pxTreeViewItem = PXNull;

                    if(pxNativDraw)
                    {
                        const HTREEITEM treeItemHandle = TreeView_GetSelection(sourceObject); // Event does not give us the handle, fetch manually.
                        const HWND itemHandle = (HWND)treeItemHandle;

                        // Fetch treeview object
                        PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &sourceObject, &pxTreeViewContainer);

                        // Fetch treeviewitem object
                        PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &itemHandle, &pxTreeViewItem);
                    }

                    pxWindowEvent.UIElementReference = pxTreeViewContainer;
                    pxWindowEvent.UIElementSender = pxTreeViewItem;

                    pxWindowEvent.Type = PXWindowEventTypeElementSelect;
                    pxWindowEvent.Select.UIElementSelected = pxTreeViewItem;

                    PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

                    break;
                }
                case TCN_SELCHANGE:
                {


                    // Setup data

                    const int pageID = TabCtrl_GetCurSel(sourceObject);

#if 1
                    PXWindow* pxGUIElement = PXNull;

                    PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &sourceObject, &pxGUIElement);

                    PXWindowExtendedBehaviourTab* const pxWindowExtendedBehaviourTab = (PXWindowExtendedBehaviourTab*)pxGUIElement->ExtendedData;

                    // Hide current page
                    const PXBool isValidIndex = pxWindowExtendedBehaviourTab->TABPageAmount >= (pageID + 1);

                    BOOL success = 0;

                    for(PXSize i = 0; i < pxWindowExtendedBehaviourTab->TABPageAmount; i++)
                    {
                        PXWindow* const pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

                        PXWindowPropertyInfo pxGUIElementUpdateInfo;
                        PXClear(PXWindowPropertyInfo, &pxGUIElementUpdateInfo);
                        pxGUIElementUpdateInfo.Show = PXFalse;
                        pxGUIElementUpdateInfo.WindowCurrent = pxWindowTABPage;
                        pxGUIElementUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXNativDrawWindowProperty(pxNativDraw, &pxGUIElementUpdateInfo, 1);
                    }

                    // Show new page
                    if(isValidIndex)
                    {
                        PXWindow* const pxWindowTABPageSelected = &pxWindowExtendedBehaviourTab->TABPageList[pageID];

                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "GUI",
                            "Event",
                            "Tab Select <0x%p>, %15s ID:<%i>",
                            sourceObject,
                            "", //pxUIElementEEE->NameData,
                            pageID
                        );

                        PXWindowPropertyInfo pxGUIElementUpdateInfo;
                        PXClear(PXWindowPropertyInfo, &pxGUIElementUpdateInfo);
                        pxGUIElementUpdateInfo.Show = PXTrue;
                        pxGUIElementUpdateInfo.WindowCurrent = pxWindowTABPageSelected;
                        pxGUIElementUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXNativDrawWindowProperty(pxNativDraw, &pxGUIElementUpdateInfo, 1);
                    }

#endif

                    break;
                }
                default:
                {
#if 0
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "Windows",
                        "Event",
                        "Notify <0x%p>, ID:<%i>",
                        sourceObject,
                        notificationCode->code
                    );
#endif

                    break;
                }
            }

            break;
        }

        case WM_ERASEBKGND:
#if 0
            return FALSE;  // Defer erasing into WM_PAINT
#else
            break;
#endif

        case WM_PAINT:
        {
            // No parameters

            PXWindow* const pxGUIElement = PXNull;

            if(pxNativDraw->ResourceManager)
            {
                PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &windowID, &pxGUIElement);
            }

            if(!pxGUIElement)
            {
                break; // break: not found
            }

            const PXBool shallDraw = pxGUIElement->DrawFunction && (PXResourceInfoRender & pxGUIElement->Info.Flags);

            if(shallDraw)
            {
                PAINTSTRUCT paintStruct;

                const HWND windowHandle = pxGUIElement->Info.Handle.WindowID;
                const HDC hdc = BeginPaint(windowHandle, &paintStruct);

                PXWindowDrawInfo pxGUIElementDrawInfo;
                PXClear(PXWindowDrawInfo, &pxGUIElementDrawInfo);
                pxGUIElementDrawInfo.hwnd = pxGUIElement->Info.Handle.WindowID;
                pxGUIElementDrawInfo.hDC = hdc;
                // pxGUIElementDrawInfo.bErase = paintStruct.fErase;

                PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&paintStruct.rcPaint, &pxGUIElement->Position.Form);

                pxGUIElement->DrawFunction(pxNativDraw->GUISystem, pxGUIElement, &pxGUIElementDrawInfo);

                const BOOL endSuccess = EndPaint(windowHandle, &paintStruct);

                return TRUE; // We did a custom draw, so return true to mark this as handled
            }

            break;
        }
        case WM_DRAWITEM:
        {
            const HWND identifier = (HWND)wParam;
            DRAWITEMSTRUCT* const drawItemInfo = (DRAWITEMSTRUCT*)lParam;

            PXWindow* const pxGUIElement = PXNull;

            if(pxNativDraw->ResourceManager)
            {
                PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &drawItemInfo->hwndItem, &pxGUIElement);
            }

            if(!pxGUIElement)
            {
                break; // break: not found
            }

            const PXBool shallDraw = pxGUIElement->DrawFunction && (pxGUIElement->Info.Flags& PXResourceInfoRender);

            if(shallDraw)
            {
                PXWindowDrawInfo pxGUIElementDrawInfo;
                PXClear(PXWindowDrawInfo, &pxGUIElementDrawInfo);
                pxGUIElementDrawInfo.hwnd = pxGUIElement->Info.Handle.WindowID;
                pxGUIElementDrawInfo.hDC = drawItemInfo->hDC;
                // pxGUIElementDrawInfo.bErase = paintStruct.fErase;

                PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&drawItemInfo->rcItem, &pxGUIElement->Position.Form);

                pxGUIElement->DrawFunction(pxNativDraw->GUISystem, pxGUIElement, &pxGUIElementDrawInfo);

                return TRUE; // We did a custom draw, so return true to mark this as handled
            }

            break;
        }
        case WM_PRINTCLIENT:
        {
            PXWindow* const pxGUIElement = PXNull;

            if(pxNativDraw->ResourceManager)
            {
                PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &windowID, &pxGUIElement);
            }

            if(!pxGUIElement)
            {
                break; // break: not found
            }

            if(pxGUIElement->DrawFunction)
            {
                RECT rc;

                GetClientRect(windowID, &rc);

                PXWindowDrawInfo pxGUIElementDrawInfo;
                PXClear(PXWindowDrawInfo, &pxGUIElementDrawInfo);
                pxGUIElementDrawInfo.hwnd = windowID;
                pxGUIElementDrawInfo.hDC = (HDC)wParam;
                //  pxGUIElementDrawInfo.rcDirty = &rc;
                pxGUIElementDrawInfo.bErase = TRUE;

                pxGUIElement->DrawFunction(pxNativDraw->GUISystem, pxGUIElement, &pxGUIElementDrawInfo);

                return 0; // We custom draw, handled
            }

            break;
        }


#if 0
        case WM_ACTIVATE:
            break;
        case WM_SETREDRAW:
            break;
        case WM_SETFOCUS:
            break;
        case WM_KILLFOCUS:
            break;
        case WM_ENABLE:
            break;
        case WM_SETTEXT:
            break;
        case WM_GETTEXT:
            break;
        case WM_GETTEXTLENGTH:
            break;

        case WM_QUERYENDSESSION:
            break;
        case WM_ENDSESSION:
            break;
        case WM_QUERYOPEN:
            break;
        case WM_QUIT:
            break;
        case WM_ERASEBKGND:
            break;
        case WM_SYSCOLORCHANGE:
            break;
        case WM_SHOWWINDOW:
            break;
        case WM_WININICHANGE:
            break;
            //  case WM_SETTINGCHANGE:
            //     break;
        case WM_DEVMODECHANGE:
            return WindowEventDeviceModeChange;
        case WM_ACTIVATEAPP:
            return WindowEventActivateApp;
        case WM_FONTCHANGE:
            return WindowEventFontChange;
        case WM_TIMECHANGE:
            return WindowEventTimeChange;
        case WM_CANCELMODE:
            return WindowEventCancelMode;
        case WM_MOUSEACTIVATE:
            return WindowEventMouseActivate;
        case WM_CHILDACTIVATE:
            return WindowEventChildActivate;
        case WM_QUEUESYNC:
            return WindowEventQueueSync;
        case WM_GETMINMAXINFO:
            return WindowEventSizeChange;
        case WM_PAINTICON:
            return WindowEventIconPaint;
        case WM_ICONERASEBKGND:
            return WindowEventIconBackgroundErase;
        case WM_NEXTDLGCTL:
            return WindowEventDialogControlNext;
        case WM_SPOOLERSTATUS:
            return WindowEventSPOOLERSTATUS;
        case WM_DRAWITEM:
            return WindowEventItemDraw;
        case WM_MEASUREITEM:
            return WindowEventItemMeasure;
        case WM_DELETEITEM:
            return WindowEventItemDelete;
        case WM_VKEYTOITEM:
            return WindowEventVKEYTOITEM;
        case WM_CHARTOITEM:
            return WindowEventCHARTOITEM;
        case WM_SETFONT:
            return WindowEventFontSet;
        case WM_GETFONT:
            return WindowEventFontGet;
        case WM_SETHOTKEY:
            return WindowEventSETHOTKEY;
        case WM_GETHOTKEY:
            return WindowEventGETHOTKEY;
        case WM_QUERYDRAGICON:
            return WindowEventQUERYDRAGICON;
        case WM_COMPAREITEM:
            return WindowEventCOMPAREITEM;
        case WM_GETOBJECT:
            return WindowEventGETOBJECT;
        case WM_COMPACTING:
            return WindowEventCOMPACTING;
        case WM_COMMNOTIFY:
            return WindowEventCOMMNOTIFY;   /* no longer suported */
        case WM_WINDOWPOSCHANGING:
            return WindowEventWINDOWPOSCHANGING;
        case WM_WINDOWPOSCHANGED:
            return WindowEventWINDOWPOSCHANGED;
        case WM_POWER:
            return WindowEventPOWER;
        case WM_COPYDATA:
            return WindowEventCOPYDATA;
        case WM_CANCELJOURNAL:
            return WindowEventCANCELJOURNAL;

        case WM_INPUTLANGCHANGEREQUEST:
            return WindowEventINPUTLANGCHANGEREQUEST;
        case WM_INPUTLANGCHANGE:
            return WindowEventINPUTLANGCHANGE;
        case WM_TCARD:
            return WindowEventTCARD;
        case WM_HELP:
            return WindowEventHELP;
        case WM_USERCHANGED:
            return WindowEventUSERCHANGED;
        case WM_NOTIFYFORMAT:
            return WindowEventNOTIFYFORMAT;
        case WM_CONTEXTMENU:
            return WindowEventCONTEXTMENU;
        case WM_STYLECHANGING:
            return WindowEventSTYLECHANGING;
        case WM_STYLECHANGED:
            return WindowEventSTYLECHANGED;
        case WM_DISPLAYCHANGE:
            return WindowEventDISPLAYCHANGE;
        case WM_GETICON:
            return WindowEventGETICON;
        case WM_SETICON:
            return WindowEventSETICON;
        case WM_NCCREATE:
            return WindowEventNCCREATE;
        case WM_NCDESTROY:
            return WindowEventNCDESTROY;
        case WM_NCCALCSIZE:
            return WindowEventNCCALCSIZE;
        case WM_NCHITTEST:
            return WindowEventNCHITTEST;
        case WM_NCPAINT:
            return WindowEventNCPAINT;
        case WM_NCACTIVATE:
            return WindowEventNCACTIVATE;
        case WM_GETDLGCODE:
            return WindowEventGETDLGCODE;
        case WM_SYNCPAINT:
            return WindowEventSYNCPAINT;
        case WM_NCMOUSEMOVE:
            return WindowEventNCMOUSEMOVE;
        case WM_NCLBUTTONDOWN:
            return WindowEventNCLBUTTONDOWN;
        case WM_NCLBUTTONUP:
            return WindowEventNCLBUTTONUP;
        case WM_NCLBUTTONDBLCLK:
            return WindowEventNCLBUTTONDBLCLK;
        case WM_NCRBUTTONDOWN:
            return WindowEventNCRBUTTONDOWN;
        case WM_NCRBUTTONUP:
            return WindowEventNCRBUTTONUP;
        case WM_NCRBUTTONDBLCLK:
            return WindowEventNCRBUTTONDBLCLK;
        case WM_NCMBUTTONDOWN:
            return WindowEventNCMBUTTONDOWN;
        case WM_NCMBUTTONUP:
            return WindowEventNCMBUTTONUP;
        case WM_NCMBUTTONDBLCLK:
            return WindowEventNCMBUTTONDBLCLK;
        case WM_NCXBUTTONDOWN:
            return WindowEventNCXBUTTONDOWN;
        case WM_NCXBUTTONUP:
            return WindowEventNCXBUTTONUP;
        case WM_NCXBUTTONDBLCLK:
            return WindowEventNCXBUTTONDBLCLK;
        case WM_INPUT_DEVICE_CHANGE:
            return WindowEventINPUT_DEVICE_CHANGE;
            // ?? case WM_KEYFIRST: return WindowEventKEYFIRST;

        case WM_CHAR:
            return WindowEventCHAR;
        case WM_DEADCHAR:
            return WindowEventDEADCHAR;
        case WM_SYSKEYDOWN:
            return WindowEventSYSKEYDOWN;
        case WM_SYSKEYUP:
            return WindowEventSYSKEYUP;
        case WM_SYSCHAR:
            return WindowEventSYSCHAR;
        case WM_SYSDEADCHAR:
            return WindowEventSYSDEADCHAR;
            // case WM_UNICHAR: return WindowEventUNICHAR;
            //case WM_KEYLAST: return WindowEventKEYLAST;
            // case UNICODE_NOCHAR: return WindowEventXXXXXXXXXXXXXXX;

        case WM_IME_STARTCOMPOSITION:
            return WindowEventIME_STARTCOMPOSITION;
        case WM_IME_ENDCOMPOSITION:
            return WindowEventIME_ENDCOMPOSITION;
        case WM_IME_COMPOSITION:
            return WindowEventIME_COMPOSITION;
            //case WM_IME_KEYLAST: return WindowEventIME_KEYLAST;
        case WM_INITDIALOG:
            return WindowEventINITDIALOG;

        case WM_SYSCOMMAND:
            return WindowEventSYSCOMMAND;
        case WM_TIMER:
            return WindowEventTIMER;
        case WM_HSCROLL:
            return WindowEventHSCROLL;
        case WM_VSCROLL:
            return WindowEventVSCROLL;
        case WM_INITMENU:
            return WindowEventINITMENU;
        case WM_INITMENUPOPUP:
            return WindowEventINITMENUPOPUP;
            //  case WM_GESTURE: return WindowEventGESTURE;
            //   case WM_GESTURENOTIFY: return WindowEventGESTURENOTIFY;
        case WM_MENUSELECT:
            return WindowEventMENUSELECT;
        case WM_MENUCHAR:
            return WindowEventMENUCHAR;
        case WM_ENTERIDLE:
            return WindowEventENTERIDLE;
        case WM_MENURBUTTONUP:
            return WindowEventMENURBUTTONUP;
        case WM_MENUDRAG:
            return WindowEventMENUDRAG;
        case WM_MENUGETOBJECT:
            return WindowEventMENUGETOBJECT;
        case WM_UNINITMENUPOPUP:
            return WindowEventUNINITMENUPOPUP;
        case WM_MENUCOMMAND:
            return WindowEventMENUCOMMAND;
        case WM_CHANGEUISTATE:
            return WindowEventCHANGEUISTATE;
        case WM_UPDATEUISTATE:
            return WindowEventUPDATEUISTATE;
        case WM_QUERYUISTATE:
            return WindowEventQUERYUISTATE;
        case WM_CTLCOLORMSGBOX:
            return WindowEventCTLCOLORMSGBOX;
        case WM_CTLCOLOREDIT:
            return WindowEventCTLCOLOREDIT;
        case WM_CTLCOLORLISTBOX:
            return WindowEventCTLCOLORLISTBOX;
        case WM_CTLCOLORBTN:
            return WindowEventCTLCOLORBTN;
        case WM_CTLCOLORDLG:
            return WindowEventCTLCOLORDLG;
        case WM_CTLCOLORSCROLLBAR:
            return WindowEventCTLCOLORSCROLLBAR;

        case WM_CTLCOLORSTATIC:
            return WindowEventCTLCOLORSTATIC;
            //  case MN_GETHMENU: return WindowEventGETHMENU;
            //case WM_MOUSEFIRST: return WindowEventMOUSEFIRST;


#endif
        case WM_MOUSEMOVE:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
            pxWindowEvent.InputMouseMove.PositionX = GET_X_LPARAM(lParam);
            pxWindowEvent.InputMouseMove.PositionY = GET_Y_LPARAM(lParam);

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }


            // case WM_PAINT:
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
        {
            // HWND windowFocusedHandle = (HWND)GetFocus();;
            HWND windowHandleNow = (HWND)lParam;
            HDC hdc = (HDC)wParam;

#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                "Window",
                "Event",
                "Color Edit (0x%p) -> (0x%p)",
                windowID,
                windowHandleNow
            );
#endif

            // can you fetch the object?
            //  PXWindow* pxGUIElement = PXNull;

            SetTextColor(hdc, RGB(200, 200, 200));
            SetBkColor(hdc, RGB(50, 50, 50));            // yellow


            HBRUSH brush = 0;
#if 0
            brush = GetStockObject(WHITE_BRUSH);
#else
            brush = CreateSolidBrush(RGB(20, 20, 20));
#endif

            return brush;
        }
        case WM_INPUT:
        {
#if 0

            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event-Input",
                "ID:%4i, Parent (0x%p), Sender (0x%p)",
                eventID,
                windowID,
                pxWindowEvent.UIElementReference
            );
#endif

            // MISSING
            const PXSize inputCode = GET_RAWINPUT_CODE_WPARAM(wParam);
            const HRAWINPUT handle = (HRAWINPUT)lParam;
            const UINT uiCommand = RID_INPUT; // RID_HEADER

            switch(inputCode)
            {
                case RIM_INPUT: // Input occurred while the application was in the foreground.
                {
                    RAWINPUT rawInput;
                    UINT rawInputSize = sizeof(RAWINPUT); // Can't be 'const' !

#if WindowsAtleastXP
                    const UINT result = GetRawInputData // Windows XP, User32.dll, winuser.h
                    (
                        handle,
                        uiCommand,
                        &rawInput,
                        &rawInputSize,
                        sizeof(RAWINPUTHEADER)
                    );
                    const PXBool sucessful = result != -1;

                    if(sucessful)
                    {
#if UseRawMouseData
                        if(RIM_TYPEMOUSE == rawInput.header.dwType)
                        {
                            pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
                            pxWindowEvent.InputMouseMove.AxisX = 0;
                            pxWindowEvent.InputMouseMove.AxisY = 0;
                            pxWindowEvent.InputMouseMove.DeltaX = rawInput.data.mouse.lLastX;
                            pxWindowEvent.InputMouseMove.DeltaY = rawInput.data.mouse.lLastY;

                            PXWindowCursorPositionInWindowGet(windowID, &pxWindowEvent.InputMouseMove.AxisX, &pxWindowEvent.InputMouseMove.AxisY);

                            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);


                            //PXWindowTriggerOnMouseMoveEvent(window, positionX, positionY, deltaX, deltaY);



                            // Wheel data needs to be pointer casted to interpret an unsigned short as a short, with no conversion
                            // otherwise it'll overflow when going negative.
                            // Didn't happen before some minor changes in the code, doesn't seem to go away
                            // so it's going to have to be like this.
                            // if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
                            //     input.mouse.wheel = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
                        }
#endif


                    }
#endif

                    break;
                }
                case RIM_INPUTSINK: // Input occurred while the application was not in the foreground.
                {
                    break;
                }

                default:
                    break;
            }

            break;
        }
        case WM_COMMAND:
        {
            const PXInt32U buttonType = wParam; // IDOK;
            const HANDLE handle = (HANDLE)lParam;

            PXWindow* const pxGUIElement = PXNull;

            if(pxNativDraw)
            {
                PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &handle, &pxGUIElement);
            }

            if(!pxGUIElement) // if we did not find the object, stop
            {
                break;
            }

            if(!(pxGUIElement->Info.Flags & PXResourceInfoActive))
            {
                // ShowWindow(pxGUIElement->ID, SW_HIDE);
            }

            pxWindowEvent.Type = PXWindowEventTypeElementClick;
            pxWindowEvent.UIElementSender = pxGUIElement;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_LBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_LBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_LBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_RBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_RBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_RBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_MBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_MBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_MBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);

            break;
        }
        case WM_MOUSEWHEEL:
        {

            break;
        }
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            const PXSize characterInfo = lParam;

            pxWindowEvent.Type = PXWindowEventTypeInputKeyboard;
            pxWindowEvent.InputKeyboard.CharacterID = wParam;
            pxWindowEvent.InputKeyboard.VirtualKey = PXVirtualKeyFromID(wParam);

            switch(eventID)
            {
                case WM_KEYUP:
                {
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateUp;
                    break;
                }
                case WM_KEYDOWN:
                {
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateDown;
                    break;
                }
            }

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);


            /*
            PXKeyBoardKeyInfo buttonInfo;
            buttonInfo.KeyID = character;
            buttonInfo.Key = virtualKey;
            buttonInfo.Mode = mode;
            */

            // TODO: on system 32 Bit error
            /*
            buttonInfo.Repeat = (characterInfo & 0b00000000000000001111111111111111); // Die Wiederholungsanzahl für die aktuelle Meldung.Der Wert gibt an, wie oft die Tastatureingabe automatisch angezeigt wird, wenn der Benutzer den Schlüssel hält.Die Wiederholungsanzahl ist immer 1 für eine WM _ KEYUP - Nachricht.
            buttonInfo.ScanCode = (characterInfo & 0b00000000111111110000000000000000) >> 16; // Der Scancode.Der Wert hängt vom OEM ab.
            buttonInfo.SpecialKey = (characterInfo & 0b00000001000000000000000000000000) >> 24; // Gibt an, ob es sich bei der Taste um eine erweiterte Taste handelt, z.B.die rechte ALT - und STRG - Taste, die auf einer erweiterten Tastatur mit 101 oder 102 Tasten angezeigt werden.Der Wert ist 1, wenn es sich um einen erweiterten Schlüssel handelt.andernfalls ist es 0.
            //buttonInfo.ReservedDontUse = (characterInfo & 0b00011110000000000000000000000000) >> 25; //    Reserviert; nicht verwenden.
            buttonInfo.KontextCode = (characterInfo & 0b00100000000000000000000000000000) >> 29; // Der Kontextcode.Der Wert ist für eine WM _ KEYUP - Nachricht immer 0.
            buttonInfo.PreState = (characterInfo & 0b01000000000000000000000000000000) >> 30; // Der vorherige Schlüsselzustand.Der Wert ist immer 1 für eine WM _ KEYUP - Nachricht.
            buttonInfo.GapState = (characterInfo & 0b10000000000000000000000000000000) >> 31; // Der Übergangszustand.Der Wert ist immer 1 für eine WM _ KEYUP - Nachricht.
            */

            //  PXWindowTriggerOnKeyBoardKeyEvent(window, &buttonInfo);

            break;
        }
        case WM_XBUTTONDOWN: // Fall though
        case WM_XBUTTONUP:
        {
            PXMouseButton mouseButton = PXMouseButtonInvalid;
            PXKeyPressState buttonState = PXKeyPressStateInvalid;

            const WORD releaseID = HIWORD(wParam);
            // const WORD xxxxx = LOWORD(wParam);
            // const WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            // const WORD fwButton = GET_XBUTTON_WPARAM(wParam);
            // const int xPos = GET_X_LPARAM(lParam);
            // const int yPos = GET_Y_LPARAM(lParam);

            {
                switch(eventID)
                {

                    case WindowEventXBUTTONUP:
                    {
                        buttonState = PXKeyPressStateUp;
                        break;
                    }

                    case WindowEventXBUTTONDOWN:
                    {
                        buttonState = PXKeyPressStateDown;
                        break;
                    }
                }


                switch(releaseID)
                {
                    case XBUTTON1:
                        mouseButton = PXMouseButtonSpecialA;
                        break;

                    case XBUTTON2:
                        mouseButton = PXMouseButtonSpecialB;
                        break;
                }

                /*
                        switch(fwKeys)
                        {
                            case MK_XBUTTON1:
                                mouseButton = MouseButtonSpecialA;
                                break;

                            case MK_XBUTTON2:
                                mouseButton = MouseButtonSpecialB;
                                break;

                            default:
                                mouseButton = MouseButtonInvalid;
                                break;
                        }
                */


                // PXWindowTriggerOnMouseClickEvent(window, mouseButton, buttonState);
            }

            break;
        }



        case WM_DEVICECHANGE:
        {
#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "DEVICECHANGE"
            );
#endif

            break;
        }

        // Track non client area
        case WM_NCMOUSEMOVE:
        {
            const int xPos = GET_X_LPARAM(lParam);
            const int yPos = GET_Y_LPARAM(lParam);

#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "MouseMove <%ix%i>",
                xPos,
                yPos
            );
#endif

         //   PXNativeDrawMouseTrack(pxWindowEvent.UIElementReference);

            break;
        }

        case WM_MOUSEHOVER: // Never trigger?
        {
#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "WM_MOUSEHOVER"
            );
#endif
          //  PXNativeDrawMouseTrack(pxWindowEvent.UIElementReference);

            break;
        }
        case WM_MOUSELEAVE:  // Never trigger?
        {
#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "WM_MOUSELEAVE"
            );
#endif

           // PXNativeDrawMouseTrack(pxWindowEvent.UIElementReference);

            break;
        }

        case WM_NCHITTEST:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            int res = DefWindowProc(windowID, eventID, wParam, lParam);


#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "NCHITTEST : %s",
                pxWindowEvent.UIElementReference->NameContent
            );
#endif

            // TODO: update window pos data


            //pxWindowEvent.UIElementReference.

            return res;
        }


        /*

        case WM_XBUTTONDBLCLK: return WindowEventXBUTTONDBLCLK;
            //   case WM_MOUSEHWHEEL: return WindowEventMOUSEHWHEEL;
                   //case WM_MOUSELAST: return WindowEventMOUSELAST;
        case WM_PARENTNOTIFY: return WindowEventPARENTNOTIFY;
        case WM_ENTERMENULOOP: return WindowEventENTERMENULOOP;
        case WM_EXITMENULOOP: return WindowEventEXITMENULOOP;
        case WM_NEXTMENU: return WindowEventNEXTMENU;
        case WM_SIZING: return WindowEventSIZING;
        case WM_CAPTURECHANGED: return WindowEventCAPTURECHANGED;
        case WM_MOVING: return WindowEventMOVING;
        case WM_POWERBROADCAST: return WindowEventPOWERBROADCAST;

        case WM_MDICREATE: return WindowEventMDICREATE;
        case WM_MDIDESTROY: return WindowEventMDIDESTROY;
        case WM_MDIACTIVATE: return WindowEventMDIACTIVATE;
        case WM_MDIRESTORE: return WindowEventMDIRESTORE;
        case WM_MDINEXT: return WindowEventMDINEXT;

        case WM_MDIMAXIMIZE: return WindowEventMDIMAXIMIZE;
        case WM_MDITILE: return WindowEventMDITILE;
        case WM_MDICASCADE: return WindowEventMDICASCADE;
        case WM_MDIICONARRANGE: return WindowEventMDIICONARRANGE;
        case WM_MDIGETACTIVE: return WindowEventMDIGETACTIVE;
        case WM_MDISETMENU: return WindowEventMDISETMENU;
        case WM_ENTERSIZEMOVE: return WindowEventENTERSIZEMOVE;
        case WM_EXITSIZEMOVE: return WindowEventEXITSIZEMOVE;
        case WM_DROPFILES: return WindowEventDROPFILES;
        case WM_MDIREFRESHMENU: return WindowEventMDIREFRESHMENU;
            //case WM_POINTERDEVICECHANGE: return WindowEventPOINTERDEVICECHANGE;
        //case WM_POINTERDEVICEINRANGE: return WindowEventPOINTERDEVICEINRANGE;
        //case WM_POINTERDEVICEOUTOFRANGE: return WindowEventPOINTERDEVICEOUTOFRANGE;
        //  case WM_TOUCH: return WindowEventTOUCH;
            //case WM_NCPOINTERUPDATE: return WindowEventNCPOINTERUPDATE;
        //case WM_NCPOINTERDOWN: return WindowEventNCPOINTERDOWN;
        //case WM_NCPOINTERUP: return WindowEventNCPOINTERUP;
        //case WM_POINTERUPDATE: return WindowEventPOINTERUPDATE;
        //case WM_POINTERDOWN: return WindowEventPOINTERDOWN;
        //case WM_POINTERUP: return WindowEventPOINTERUP;
        //case WM_POINTERENTER: return WindowEventPOINTERENTER;
        //case WM_POINTERLEAVE: return WindowEventPOINTERLEAVE;
        //case WM_POINTERACTIVATE: return WindowEventPOINTERACTIVATE;
        //case WM_POINTERCAPTURECHANGED: return WindowEventPOINTERCAPTURECHANGED;
        //case WM_TOUCHHITTESTING: return WindowEventTOUCHHITTESTING;
        //case WM_POINTERWHEEL: return WindowEventPOINTERWHEEL;
        //case WM_POINTERHWHEEL: return WindowEventPOINTERHWHEEL;
            //case WM_POINTERROUTEDTO: return WindowEventPOINTERROUTEDTO;
            //case WM_POINTERROUTEDAWAY: return WindowEventPOINTERROUTEDAWAY;
        //case WM_POINTERROUTEDRELEASED: return WindowEventPOINTERROUTEDRELEASED;
        case WM_IME_SETCONTEXT: return WindowEventIME_SETCONTEXT;
        case WM_IME_NOTIFY: return WindowEventIME_NOTIFY;
        case WM_IME_CONTROL: return WindowEventIME_CONTROL;
        case WM_IME_COMPOSITIONFULL: return WindowEventIME_COMPOSITIONFULL;
        case WM_IME_SELECT: return WindowEventIME_SELECT;
        case WM_IME_CHAR: return WindowEventIME_CHAR;
        case WM_IME_REQUEST: return WindowEventIME_REQUEST;
        case WM_IME_KEYDOWN: return WindowEventIME_KEYDOWN;
        case WM_IME_KEYUP: return WindowEventIME_KEYUP;


            //  case WM_WTSSESSION_CHANGE: return WindowEventWTSSESSION_CHANGE;
            //  case WM_TABLET_FIRST: return WindowEventTABLET_FIRST;
            //  case WM_TABLET_LAST: return WindowEventTABLET_LAST;
                  //case WM_DPICHANGED: return WindowEventDPICHANGED;
                  //case WM_DPICHANGED_BEFOREPARENT: return WindowEventDPICHANGED_BEFOREPARENT;
                  //case WM_DPICHANGED_AFTERPARENT: return WindowEventDPICHANGED_AFTERPARENT;
                  // case WM_GETDPISCALEDSIZE: return WindowEventGETDPISCALEDSIZE;
        case WM_CUT: return WindowEventCUT;
        case WM_COPY: return WindowEventCOPY;
        case WM_PASTE: return WindowEventPASTE;
        case WM_CLEAR: return WindowEventCLEAR;
        case WM_UNDO: return WindowEventUNDO;
        case WM_RENDERFORMAT: return WindowEventRENDERFORMAT;
        case WM_RENDERALLFORMATS: return WindowEventRENDERALLFORMATS;
        case WM_DESTROYCLIPBOARD: return WindowEventDESTROYCLIPBOARD;
        case WM_DRAWCLIPBOARD: return WindowEventDRAWCLIPBOARD;
        case WM_PAINTCLIPBOARD: return WindowEventPAINTCLIPBOARD;
        case WM_VSCROLLCLIPBOARD: return WindowEventVSCROLLCLIPBOARD;
        case WM_SIZECLIPBOARD: return WindowEventSIZECLIPBOARD;
        case WM_ASKCBFORMATNAME: return WindowEventASKCBFORMATNAME;
        case WM_CHANGECBCHAIN: return WindowEventCHANGECBCHAIN;
        case WM_HSCROLLCLIPBOARD: return WindowEventHSCROLLCLIPBOARD;
        case WM_QUERYNEWPALETTE: return WindowEventQUERYNEWPALETTE;
        case WM_PALETTEISCHANGING: return WindowEventPALETTEISCHANGING;
        case WM_PALETTECHANGED: return WindowEventPALETTECHANGED;
        case WM_HOTKEY: return WindowEventHOTKEY;
        case WM_PRINT: return WindowEventPRINT;
        case WM_PRINTCLIENT: return WindowEventPRINTCLIENT;
        case WM_APPCOMMAND: return WindowEventAPPCOMMAND;
        case WM_THEMECHANGED: return WindowEventTHEMECHANGED;
            //  case WM_CLIPBOARDUPDATE: return WindowEventCLIPBOARDUPDATE;
            //  case WM_DWMCOMPOSITIONCHANGED: return WindowEventDWMCOMPOSITIONCHANGED;
           //   case WM_DWMNCRENDERINGCHANGED: return WindowEventDWMNCRENDERINGCHANGED;
           //   case WM_DWMCOLORIZATIONCOLORCHANGED: return WindowEventDWMCOLORIZATIONCOLORCHANGED;
          //    case WM_DWMWINDOWMAXIMIZEDCHANGE: return WindowEventDWMWINDOWMAXIMIZEDCHANGE;
          //    case WM_DWMSENDICONICTHUMBNAIL: return WindowEventDWMSENDICONICTHUMBNAIL;
           //   case WM_DWMSENDICONICLIVEPREVIEWBITMAP: return WindowEventDWMSENDICONICLIVEPREVIEWBITMAP;
           //   case WM_GETTITLEBARINFOEX: return WindowEventGETTITLEBARINFOEX;
        case WM_HANDHELDFIRST: return WindowEventHANDHELDFIRST;
        case WM_HANDHELDLAST: return WindowEventHANDHELDLAST;
        case WM_AFXFIRST: return WindowEventAFXFIRST;
        case WM_AFXLAST: return WindowEventAFXLAST;
        case WM_PENWINFIRST: return WindowEventPENWINFIRST;
        case WM_PENWINLAST: return WindowEventPENWINLAST;
        case WM_APP: return WindowEventAPP;




        */

        case WM_DISPLAYCHANGE:
        {
            // Some display got plugged in, out or some setting changed
            // We need to update our monitors/displays



            break;
        }











#if 0 // Not useable for resize event. Its not what it seems
        case WindowEventSizeChange:
        {
            //wParam is unused
            const MINMAXINFO* minmaxInfo = (MINMAXINFO*)lParam;
            const LONG width = minmaxInfo->ptMaxSize.x;
            const LONG height = minmaxInfo->ptMaxSize.y;

            break;
        }
#endif

    }

    // If nothing had grabbed the event by now, let the default funcion handle it.
    return DefWindowProc(windowID, eventID, wParam, lParam);
}
#endif













#if OSUnix
#elif PXOSWindowsDestop
void PXAPI PXWindowChildListEnumerate(PXGUISystem* const pxGUISystem, PXWindow* const parent, PXBool visible)
{
    for(size_t i = 0; i < pxGUISystem->ResourceManager->GUIElementLookup.EntryAmountCurrent; i++)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXWindow* childElement = PXNull;

        PXDictionaryIndex(&pxGUISystem->ResourceManager->GUIElementLookup, i, &pxDictionaryEntry);

        childElement = *(PXWindow**)pxDictionaryEntry.Value;

        PXWindow* parrent = (PXWindow*)childElement->Info.Hierarchy.Parrent;

        if(parrent)
        {
            if(parrent->Info.Handle.WindowID == parent->Info.Handle.WindowID)
            {
                // found child.

                PXWindowPropertyInfo pxGUIElementUpdateInfo;
                PXClear(PXWindowPropertyInfo, &pxGUIElementUpdateInfo);

                pxGUIElementUpdateInfo.WindowCurrent = childElement;
                pxGUIElementUpdateInfo.Show = visible;
                pxGUIElementUpdateInfo.Property = PXUIElementPropertyVisibility;

               // PXWindowUpdate(pxGUISystem, &pxGUIElementUpdateInfo, 1);
            }
        }
    }
}

BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam)
{
    int mode = *(int*)lParam;

    // Recursion
    {
        const BOOL success = EnumChildWindows
        (
            hwnd,
            PXWindowEnumChildProc,
            lParam
        );
    }

    ShowWindow(hwnd, mode);
}
#endif

void PXAPI PXNativeDrawMouseTrack(PXWindow* const window)
{
#if OSUnix
#elif OSWindows
    TRACKMOUSEEVENT trackMouseEvent;
    trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
    trackMouseEvent.hwndTrack = window->Info.Handle.WindowID;
    trackMouseEvent.dwHoverTime = HOVER_DEFAULT;

    const PXBool rs = TrackMouseEvent(&trackMouseEvent);
#endif
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

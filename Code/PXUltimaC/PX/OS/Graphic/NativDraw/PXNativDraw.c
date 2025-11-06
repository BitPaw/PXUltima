#include "PXNativDraw.h"

PXNativDraw _internalNativDraw;

const char PXNativDrawText[] = "NativDraw";
const char PXDrawText[] = "Draw";

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

#include <PX/Media/PXText.h>
#include <PX/Engine/PXEngine.h>
#include <PX/Math/PXCollision.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Input/PXInput.h>

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

    PXMonitor PXREF pxMonitor = &pxEindowsDisplayEnumInfo->MonitorList[pxEindowsDisplayEnumInfo->OffsetCurrent];
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
        PXNativDrawText,
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

PXResult PXAPI PXNativeDrawErrorFetch(const PXBool condition)
{
    if(condition)
    {
        return PXActionSuccessful;
    }


    return PXActionSuccessful;
}
#endif

PXResult PXAPI PXNativDrawWindowPrintHirachy(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, int depth)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
        "Hirachy",
        "%4i, PXID:%4i, Name:%i",
        pxWindow->Info.ID,
        pxWindow->NameContent
    );
#endif

    // Loop siblings

    /*for(PXWindow* sibling = pxWindow->Info.Hierarchy.Sibling; sibling; sibling = pxWindow->Info.Hierarchy.Sibling)
    {

    }*/

    for(PXWindow* sibling = (PXWindow*)pxWindow->Info.Hierarchy.Sibling->Yourself; sibling; sibling = (PXWindow*)sibling->Info.Hierarchy.ChildFirstborn->Yourself)
    {
        PXNativDrawWindowPrintHirachy(pxNativDraw, sibling, depth + 1);
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw PXREF pxNativDraw)
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
        pxNativDraw->MonitorList = PXMemoryHeapReallocT(PXMonitor, PXNull, pxNativDraw->MonitorListAmount);

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
                PXNativDraw,
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
            PXNativDraw,
            "Display-Fetch",
            "Devices detected : %i",
            amount
        );
#endif

        PXDisplay* pxDisplayList = PXMemoryHeapReallocT(PXDisplay, PXNull, amount);

        pxNativDraw->DisplayList = pxDisplayList;
        pxNativDraw->DisplayListAmount = amount;

        for(PXSize deviceID = 0; EnumDisplayDevicesA(NULL, deviceID, &displayDevice, dwFlags); deviceID++)
        {
            PXDisplay PXREF pxDisplay = &pxDisplayList[deviceID];
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
        PXNativDraw,
        "Display-Device",
        "Fetching <%i> monitor devices",
        pxNativDraw->DisplayListAmount
    );

    for(PXSize i = 0; i < pxNativDraw->DisplayListAmount; ++i)
    {
        PXDisplay PXREF pxDisplay = &pxNativDraw->DisplayList[i];

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
            PXNativDraw,
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

PXResult PXAPI PXNativDrawDisplayOpen(PXNativDraw PXREF pxNativDraw, PXDisplay PXREF pxDisplay, const char PXREF displayName)
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

PXResult PXAPI PXNativDrawWindowCreate(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    PXWindow PXREF pxWindowParent = (PXWindow PXREF)pxWindow->Info.Hierarchy.Parrent->Yourself;
    PXWindowHandle pxWindowParentHandle = pxWindowParent ? pxWindowParent->Info.Handle.WindowHandle : PXNull;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
        "Window",
        "PXID:%i, Creating...",
        pxWindow->Info.ID
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

    const XVisualInfo PXREF visualInfo = glXChooseVisual(displayHandle, screenHandle, attributeList);

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
            pxWindow->Info.Handle.WindowHandle = XCreateSimpleWindow
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

            if(pxWindow->Info.Handle.WindowHandle)
            {
                // Create events
              //  XSelectInput(displayHandle, pxWindow->Info.Handle.WindowHandle, inputEventsToListen);
            }
        }
        else
        {



            pxWindow->Info.Handle.WindowHandle = XCreateWindow
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

        const PXBool successful = PXNull != pxWindow->Info.Handle.WindowHandle;

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
            pxWindow->Info.Handle.WindowHandle,
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


    XSelectInput(displayHandle, pxWindow->Info.Handle.WindowHandle, inputEventsToListen);


    // Attach to render engine
    {
        const int mapResultID = XMapWindow(displayHandle, pxWindow->Info.Handle.WindowHandle);

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
                pxWindow->Info.Handle.WindowHandle
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

    pxWindow->Info.Handle.WindowHandle = CreateWindowExA // Windows 2000, User32.dll, winuser.h
    (
        pxWindowCreateInfo->WindowsWindowsStyleFlagsExtended,
        pxWindowCreateInfo->WindowClassName.A,
        pxWindowCreateInfo->WindowText.A, // Text content
        pxWindowCreateInfo->WindowsStyleFlags,
        pxWindowCreateInfo->Size.X,
        pxWindowCreateInfo->Size.Y,
        pxWindowCreateInfo->Size.Width,
        pxWindowCreateInfo->Size.Height,
        pxWindowParentHandle,
        PXNull, // No menu.
        pxWindowCreateInfo->InstanceHandle,
        NULL // Pointer not needed.
    );
    const PXResult createResult = PXErrorCurrent(PXNull != pxWindow->Info.Handle.WindowHandle);

    if(PXActionSuccessful != createResult)
    {
        return createResult;
    }

    // Get additional device context for rendering purpose
    pxWindow->DeviceContextHandle = GetDC(pxWindow->Info.Handle.WindowHandle);

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

    pxWindow->Info.Behaviour |=
        PXResourceInfoExist | 
        PXResourceInfoActive | 
        PXResourceInfoRender | 
        PXResourceInfoUseByOS;
    pxWindow->Position.Form.X = pxWindowCreateInfo->Size.X;
    pxWindow->Position.Form.Y = pxWindowCreateInfo->Size.Y;
    pxWindow->Position.Form.Width = pxWindowCreateInfo->Size.Width;
    pxWindow->Position.Form.Height = pxWindowCreateInfo->Size.Height;

    // Window create, register..
    PXDictionaryEntryAdd
    (
        &pxNativDraw->ResourceManager->GUIElementLookup, 
        &pxWindow->Info.Handle.WindowHandle,
        pxWindow
    );
    
    if(pxWindowParent)
    {
        PXHierarchicalNodeParent(&pxWindow->Info.Hierarchy, &pxWindowParent->Info.Hierarchy);
    }


#if PXLogEnable && 0
    const char* uielementName = PXUIElementTypeToString(pxWindowCreateInfo->Type);

    PXLogPrint
    (
        PXLoggingError,
        PXNativDraw,
        "Element-Create",
        "Failed: X:%4i, Y:%4i, W:%4i, H:%4i, (%s) : [%s]",
        (int)pxWindowCreateInfo->X,
        (int)pxWindowCreateInfo->Y,
        (int)pxWindowCreateInfo->Width,
        (int)pxWindowCreateInfo->Height,
        uielementName,
#if OSWindows
        pxWindowCreateInfo->WindowsTextContent
#else
        pxWindowCreateInfo->Name
#endif
    );
#endif

    PXScollbar pxScollbar;
    PXClear(PXScollbar, &pxScollbar);
    pxScollbar.Info.Behaviour |= PXScollbarBehaviourBoth;

   // PXNativDrawScrollbarUpdate(pxNativDraw, pxWindow, &pxScollbar);


    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawWindowEventPendingAmount(PXNativDraw PXREF pxNativDraw, PXSize PXREF amount)
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

PXResult PXAPI PXNativDrawWindowBufferSwap(PXNativDraw PXREF pxNativDraw, const PXWindow PXREF pxWindow)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    glXSwapBuffers(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowHandle);
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
        PXNativDraw,
        "BufferSwap",
        "%i",
        pxActionResult
    );
#endif

    return pxActionResult;
}



PXResult PXAPI PXNativDrawBegin(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
#if OSUnix

#elif OSWindows
    PAINTSTRUCT paintSturct;
    PXClear(PAINTSTRUCT, &paintSturct);

    const HDC hdc = BeginPaint(pxWindow->Info.Handle.WindowHandle, &paintSturct);
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXNativDrawEnd(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
#if OSUnix

#elif OSWindows
    PAINTSTRUCT paintSturct;
    PXClear(PAINTSTRUCT, &paintSturct);

    const PXBool end = EndPaint(pxWindow->Info.Handle.WindowHandle, &paintSturct);
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXNativDrawBrushCreate(PXNativDraw PXREF pxNativDraw, PXWindowBrush PXREF pxWindowBrush, PXColorRGBI8 PXREF pxColorRGBI8)
{
#if OSUnix
    // X11 does not have a brush, right?

#elif OSWindows

    COLORREF brushColor = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
    HBRUSH brushHandle = CreateSolidBrush(brushColor);




    pxWindowBrush->Info.Handle.BrushHandle = brushHandle;

    // Color xx = Color(255, 0, 0, 255);
    // SolidBrush ww = opaqueBrush();
#endif


    PXWindowBrushColorSet(pxWindowBrush, pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);

    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawColorSetBrush(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXWindowBrush PXREF pxWindowBrush, const char mode)
{
    PXColorRGBI8* colorRef = PXNull;

    if(PXWindowBrushBehaviourColorEmbeded & pxWindowBrush->Info.Behaviour)
    {
        colorRef = &pxWindowBrush->ColorDate;
    }
    else
    {
        colorRef = pxWindowBrush->ColorReference;
    }

    return PXNativDrawSetV3(pxNativDraw, pxWindow, colorRef, mode);
}

PXResult PXAPI PXNativDrawSetV3(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXColorRGBI8* pxColorRGBI8, const char mode)
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
        PXNativDrawText,
        "Brush-Set",
        "%2.2X%2.2X%2.2X",
        pxColorRGBI8->Red,
        pxColorRGBI8->Green,
        pxColorRGBI8->Blue
    );
#endif


    if(mode == PXGUIDrawModeFront)
    {
#if OSUnix
        const PXI32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
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
        const PXI32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
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
                PXNativDrawText,
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

PXResult PXAPI PXNativDrawSetRGB(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, char red, char green, char blue, const char mode)
{
    PXColorRGBI8 color = { red, green, blue };

    return PXNativDrawSetV3(pxNativDraw, pxWindow, &color, mode);
}

PXResult PXAPI PXNativDrawFontListFetch(PXNativDraw PXREF pxNativDraw)
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

PXResult PXAPI PXNativDrawFontLoadA(PXNativDraw PXREF pxNativDraw, PXFont PXREF pxFont, const char PXREF name, const PXSize nameLength)
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
        0, 
        0,
        0,
        0,
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
        PXNativDrawText,
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

PXResult PXAPI PXNativDrawFontRelease(PXNativDraw PXREF pxNativDraw, PXFont PXREF pxFont)
{
#if OSUnix
    const int resultID = XFreeFont(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, 0);
#elif OSWindows

#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXNativDrawFontSelect(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXFont PXREF pxFont)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
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

PXResult PXAPI PXNativDrawScrollbarUpdate(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXScollbar PXREF pxScollbar)
{
#if OSUnix
#elif OSWindows

    UINT flags = 0;

    if(PXScollbarBehaviourVertical & pxScollbar->Info.Behaviour)
    {
        flags |= SB_VERT;
    }

    if(PXScollbarBehaviourHorrizontal & pxScollbar->Info.Behaviour)
    {
        flags |= SB_HORZ;
    }


    const BOOL enable = EnableScrollBar
    (
        pxWindow->Info.Handle.WindowHandle,
        flags,
        ESB_ENABLE_BOTH
    );

    const BOOL show = ShowScrollBar
    (
        pxWindow->Info.Handle.WindowHandle,
        flags,
        TRUE
    );

    SCROLLINFO scrollInfo;
    PXClear(SCROLLINFO, &scrollInfo);
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.nMin = 0;
    scrollInfo.nMax = 5;
    scrollInfo.nPos = SIF_POS | SIF_RANGE;

    int xx = SetScrollInfo
    (
        pxWindow->Info.Handle.WindowHandle,
        flags,
        &scrollInfo,
        TRUE
    );

#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
        "Scrollbar",
        "-"

    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawIconFromImage(PXNativDraw PXREF pxNativDraw, PXIcon PXREF pxIcon, PXTexture PXREF PXTexture)
{
    if(!(pxNativDraw && pxIcon && PXTexture))
    {
        return PXActionRefusedArgumentNull;
    }

    if(!PXTexture->PixelData)
    {
        return PXActionRefusedObjectInternalDataMissing;
    }

#if OSUnix

#elif OSWindows
    BYTE* bitmapData[2];
    HBITMAP bitmapHandle[2];

    BITMAPINFO bitmapInfo[2];
    PXClearList(BITMAPINFO, bitmapInfo, 2);
    bitmapInfo[0].bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo[0].bmiHeader.biWidth = PXTexture->Width;
    bitmapInfo[0].bmiHeader.biHeight = -PXTexture->Height; // Top-down DIB 
    bitmapInfo[0].bmiHeader.biPlanes = 1;
    bitmapInfo[0].bmiHeader.biBitCount = 24;// PXColorFormatBitsPerPixel(pxIconCreateInfo->IconImage->Format);
    bitmapInfo[0].bmiHeader.biCompression = BI_RGB;

    bitmapInfo[1].bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo[1].bmiHeader.biWidth = PXTexture->Width;
    bitmapInfo[1].bmiHeader.biHeight = -PXTexture->Height; // Top-down DIB
    bitmapInfo[1].bmiHeader.biPlanes = 1;
    bitmapInfo[1].bmiHeader.biBitCount = 1; // Monochrome bitmap 
    bitmapInfo[1].bmiHeader.biCompression = BI_RGB;

    for(PXSize i = 0; i < 2; ++i)
    {
        bitmapHandle[i] = CreateDIBSection(NULL, &bitmapInfo[i], DIB_RGB_COLORS, &bitmapData[i], NULL, 0);
    }

    BYTE* pixelDataBGR = (BYTE*)bitmapData[0];
    BYTE* pxMaskAND = (BYTE*)bitmapData[1];
    BYTE* pxMaskXOR = &bitmapData[(PXTexture->Width * PXTexture->Height) / 8];
  
    const PXSize maskXOROffset = (PXTexture->Height - 1) * ((PXTexture->Width + 7) / 8) + ((PXTexture->Width - 1) / 8) + 1;

    for(PXSize y = 0; y < PXTexture->Height; y++)
    {
        for(PXSize x = 0; x < PXTexture->Width; x++)
        {
            const PXSize indexInsret = (x + y * PXTexture->Width) * 3;

            const PXSize sourceX = x;// +pxIconCreateInfo->OffsetX;
            const PXSize sourceY = y;// + pxIconCreateInfo->OffsetY;
            const PXSize indexSource = PXTexturePixelPosition(PXTexture, sourceX, sourceY);
            char* pixelDataSource = (char*)PXTexture->PixelData;
            const PXColorRGBAI8 PXREF source = (PXColorRGBAI8*)&pixelDataSource[indexSource];

            BYTE PXREF insert = &pixelDataBGR[indexInsret];


            insert[0] = source->Blue;
            insert[1] = source->Green;
            insert[2] = source->Red;
            // There is a *2 because the mask index is only used every 2nd line??
            const PXSize maskIndex = (y * ((PXTexture->Width + 7) / 8) * 2) + (x / 8);
            const PXBool isTransparent = source->Alpha == 0;
            const PXI8U bitData = 1 << (7 - (x % 8));

            if(isTransparent)
            {
                pxMaskAND[maskIndex] &= ~bitData;
                pxMaskXOR[maskIndex] |= bitData;
            }
            else
            {
                pxMaskAND[maskIndex] |= bitData;
                pxMaskXOR[maskIndex] &= ~bitData;
            }
        }
    }
    

    if(0) // Use binaryicon
    {
        pxIcon->Info.Handle.IconHandle = CreateIcon
        (
            PXNull, 
            PXTexture->Width, 
            PXTexture->Height, 
            1, 
            1, 
            pxMaskAND, 
            pxMaskXOR
        );
    }
    else
    {
        // memset(pxMaskAND, 0x40, 0x1000);

        ICONINFO iconInfo;
        iconInfo.fIcon = TRUE;
        iconInfo.xHotspot = 0;
        iconInfo.yHotspot = 0;
        iconInfo.hbmMask = bitmapHandle[1]; // mask pxMaskXOR is following
        iconInfo.hbmColor = bitmapHandle[0];

        pxIcon->Info.Handle.IconHandle = CreateIconIndirect(&iconInfo);

        // iconHandle = CreateIcon(PXNull, pxIconCreateInfo->Width, pxIconCreateInfo->Height, 1, 1, pxMaskAND, pxMaskXOR);
    }
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawWindowProperty(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXGUIProperty PXREF pxWindowPropertyInfoList, const PXSize amount)
{
    PXWindow PXREF pxWindowParrent = (PXWindow*)pxWindow->Info.Hierarchy.Parrent;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXGUIProperty PXREF pxGUIProperty = &pxWindowPropertyInfoList[i];

        if(!pxWindow)
        {
            continue;
        }

        switch(pxGUIProperty->Property)
        {
            case PXUIElementPropertyTextContent:
            {
#if 0
                PXUIElementTextInfo PXREF pxUIElementTextInfo = &pxWindowUpdateInfo->Data.Text;

                if(!pxWindow)
                {
                    return PXActionInvalid;
                }

#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNativDraw,
                    "Update-Text",
                    "(0x%p) %s",
                    (int)pxWindow->ID,
                    pxUIElementTextInfo->Content
                );
#endif

                PXWindowTextSet(pxGUIManager, pxWindow, pxUIElementTextInfo->Content);
#endif


#if OSWindows



                //   SendMessageA(pxWindow->ID, PBM_SETPOS, stepsConverted, 0);


#if 0
                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                PXWindowSizeInfo pxWindowSizeInfo;

                PXWindowID pxWindowID = pxWindowUpdateInfo->WindowReference ? pxWindowUpdateInfo->WindowReference->ID : PXNull;

                PXWindowSizeGet(pxWindowID, &pxWindowSizeInfo);

                pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Height;

                PXUIElementPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);

                const PXBool succes54s = MoveWindow
                (
                    pxWindow->ID,
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
            case PXUIElementPropertyAllign:
            {
                // PXUIElementTextInfo PXREF pxUIElementTextInfo = &pxWindowUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows


                HDC xx = GetDC(pxWindow->Info.Handle.WindowHandle);

                LONG ww = SetWindowLongA(pxWindow->Info.Handle.WindowHandle, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

                // const UINT allign = SetAlign(xx, TA_CENTER);
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
                PXGUIProperty sizeFetchInfo;
                PXClear(PXGUIProperty, &sizeFetchInfo);
                //sizeFetchInfo.WindowCurrent = pxGUIProperty->WindowParent;
                sizeFetchInfo.Property = PXUIElementPropertySize;
                sizeFetchInfo.UpdateType = PXWindowPropertyUpdateTypeRead;

                PXNativDrawWindowProperty(pxNativDraw, pxWindowParrent, &sizeFetchInfo, 1);

                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                //PXWindowSizeInfo pxWindowSizeInfo;
                //PXWindowID pxWindowID = pxWindow->Parent ? pxWindow->Parent->ID : PXNull;

                //PXWindowSizeGet(PXNull, &pxWindowSizeInfo); // TODO: Problematic

                pxUIElementPositionCalulcateInfo.WindowWidth = sizeFetchInfo.Size.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = sizeFetchInfo.Size.Height;

                PXUIElementPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);



                // If the window is an actual window, we will have bad allignment if we use MoveWindow
                //if(pxWindow->Type != PXUIElementTypeWindow)

                if(1)
                {
                    const PXBool moveSuccess = MoveWindow
                    (
                        pxWindow->Info.Handle.WindowHandle,
                        pxUIElementPositionCalulcateInfo.Size.X,
                        pxUIElementPositionCalulcateInfo.Size.Y,
                        pxUIElementPositionCalulcateInfo.Size.Width,
                        pxUIElementPositionCalulcateInfo.Size.Height,
                        PXTrue
                    );
                    const PXResult moveResult = PXErrorCurrent(moveSuccess);
                }
#if 0
                else
                {
                    const BOOL result = SetWindowPos
                    (
                        pxWindow->ID,
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
#if 0
                    PXUIElementSceneRenderInfo PXREF pxUIElementSceneRenderInfo = &pxGUIProperty->SceneRender;
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
#endif

                }


#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNativDrawText,
                    "Update-Size",
                    "X:%4i, Y:%4i, W:%4i, H:%4i\n\n\n\n",
                    (int)pxUIElementPositionCalulcateInfo.Size.X,
                    (int)pxUIElementPositionCalulcateInfo.Size.Y,
                    (int)pxUIElementPositionCalulcateInfo.Size.Width,
                    (int)pxUIElementPositionCalulcateInfo.Size.Height
                );
#endif

                break;
            }
            case PXUIElementPropertyProgressbarPercentage:
            {
#if 0
                PXUIElementProgressBarInfo PXREF progressBar = &pxWindowUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                PXI32U stepsConverted = progressBar->Percentage * 100;
                SendMessageA(pxWindow->Info.Handle.WindowHandle, PBM_SETPOS, stepsConverted, 0);
#endif
#endif

                break;
            }
            case PXUIElementPropertyProgressbarBarColor:
            {
#if 0
                PXUIElementProgressBarInfo PXREF progressBar = &pxWindowUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                COLORREF color = RGB(progressBar->BarColor.Red, progressBar->BarColor.Green, progressBar->BarColor.Blue);
                SendMessageA(pxWindow->Info.Handle.WindowHandle, PBM_SETBARCOLOR, 0, color);
#endif
#endif

                break;
            }
            case PXUIElementPropertyVisibility:
            {
#if OSUnix
#elif OSWindows

                PXBool show = pxGUIProperty->Show;
                int showID = show ? SW_SHOW : SW_HIDE;

                const PXBool isWindowValid = IsWindow(pxWindow->Info.Handle.WindowHandle);

                if(!isWindowValid)
                {
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXNativDrawText,
                        "Visibility",
                        "%20s (0x%p), Invalid",
                        "",//pxWindow->NameData,
                        pxWindow->Info.Handle.WindowHandle
                    );

                    break;
                }

                if(show)
                {
                    pxWindow->Info.Behaviour |= PXResourceInfoRender;
                }
                else
                {
                    pxWindow->Info.Behaviour &= ~PXResourceInfoRender;
                }




                // EnableWindow(pxWindow->ID, show);

                ShowWindow(pxWindow->Info.Handle.WindowHandle, showID);
                BOOL res = ShowWindow(pxWindow->Info.Handle.WindowHandle, showID); // Twice to prevent some errors

                //  HWND parrent = GetParent(pxWindow->ID);

                //  HWND grandParrent = GetParent(parrent);

                //  UpdateWindow(pxWindow->ID);
                //  UpdateWindow(parrent);
                //  UpdateWindow(grandParrent);

                //  UpdateWindow(sourceObject);
                // FlashWindow(sourceObject, 1);

#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNativDraw,
                    "Visibility",
                    "Page %20s, (0x%p), Mode:%s Status:%s",
                    "",//pxWindow->NameData,
                    pxWindow->Info.Handle.WindowHandle,
                    show ? "Show" : "Hide",
                    res ? "OK" : "FAIL"
                );
#endif


                // Own solution
                {
                  //  PXWindowChildListEnumerate(pxGUIManager, pxWindow, show);



                }


                const BOOL success = EnumChildWindows
                (
                    pxWindow->Info.Handle.WindowHandle,
                    PXNativeWindowListUpdateEnum,
                    &showID
                );
#endif

                break;
            }
            case PXUIElementPropertyComboBoxAdd:
            {
#if 0
                PXUIElementComboBoxInfo PXREF pxUIElementComboBoxInfo = &pxGUIProperty->Data.ComboBox;

#if OSUnix
#elif OSWindows

                for(size_t i = 0; i < pxUIElementComboBoxInfo->DataListAmount; i++)
                {
                    char* name = pxUIElementComboBoxInfo->DataList[i];

                    ComboBox_AddString(pxWindow->Info.Handle.WindowHandle, name);
                }

                ComboBox_SetCurSel(pxWindow->Info.Handle.WindowHandle, 0);
#endif
#endif

                break;
            }
            case PXUIElementPropertyItemAdd:
            {
#if 0

                PXUIElementTreeViewItemInfo PXREF pxUIElementTreeViewItemInfo = &pxGUIProperty->Data.TreeViewItem;

#if OSUnix
#elif OSWindows

                TVINSERTSTRUCT item;
                PXClear(TVINSERTSTRUCT, &item);

                const char text[] = "[N/A]";

                item.item.pszText = text;
                item.item.cchTextMax = sizeof(text);
                item.hInsertAfter = TVI_ROOT;
                item.item.mask = TVIF_TEXT;

                TreeView_InsertItem(pxWindow->Info.Handle.WindowHandle, &item);

#endif
#endif

                break;
            }
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawTextSet(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const char PXREF text, const PXSize textLength)
{
    if(!(pxNativDraw && pxWindow && text && textLength))
    {
        return PXActionRefusedArgumentNull;
    }


    PXActionResult result = PXActionInvalid;

#if OSUnix
    // Will BadAlloc, BadWindow
    const int resultID = XStoreName(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowHandle, text);
    result = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    const PXBool success = SetWindowTextA(pxWindow->Info.Handle.WindowHandle, text);
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
            PXNativDrawText,
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
        PXNativDraw,
        "Window-Text",
        "Set: %s on (0x%p)",
        text,
        pxWindow
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXNativDrawTextGet(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const char PXREF text, const PXSize textLength)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    pxActionResult = PXActionRefusedNotImplemented;
#elif OSWindows

    const int size = GetWindowTextA
    (
        pxWindow->Info.Handle.WindowHandle,
        (LPSTR)text,
        textLength
    );

#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}



PXResult PXAPI PXNativDrawWindowIsEnabled(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXFalse;
#elif OSWindows
    const BOOL success = IsWindowEnabled(pxWindow->Info.Handle.WindowHandle); // Windows 2000, User32.dll, winuser.h
    const PXResult pxActionResult = PXErrorCurrent(success);

    return pxActionResult;
#else
    return PXFalse;
#endif
}

PXResult PXAPI PXNativDrawWindowXYWH(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const PXBool doWrite)
{
    if(doWrite)
    {
#if OSUnix
        const int resultID = XMoveWindow(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowHandle, position->X, position->Y);
        const PXResult pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop
        // Note:
        // MoveWindow() is a bad function. SetWindowPos() seems to be better in every case.

        const UINT flags = SWP_NOZORDER | SWP_NOOWNERZORDER; // SWP_NOSIZE
        const BOOL success = SetWindowPos
        (
            pxWindow->Info.Handle.WindowHandle, 
            PXNull,
            pxRectangleXYWHI32->X,
            pxRectangleXYWHI32->Y,
            pxRectangleXYWHI32->Width,
            pxRectangleXYWHI32->Height,
            flags
        ); // Windows 2000, User32.dll
        const PXResult pxActionResult = PXErrorCurrent(success);

        return pxActionResult;

#else
        return PXActionRefusedNotSupportedByLibrary;
#endif
    }
    else
    {
        pxRectangleXYWHI32->X = 0;
        pxRectangleXYWHI32->Y = 0;
        pxRectangleXYWHI32->Width = 0;
        pxRectangleXYWHI32->Height = 0;

#if OSUnix

        // Get the parent window 
        Display PXREF displayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;

        Window windowRoot;
        Window windowParent;
        Window* windowChildren;
        unsigned int* windowChildrenAmount = 0;

        XQueryTree
        (
            displayHandle,
            pxWindow->Info.Handle.WindowHandle,
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

        PXAssert(pxWindow->Info.Handle.WindowHandle, "Err");

        //const PXBool result = GetWindowRect(pxWindow->ID, &rect); // Windows 2000, User32.dll, winuser.h
        const BOOL resultGetID = GetClientRect(pxWindow->Info.Handle.WindowHandle, &rect); // Windows 2000, User32.dll, winuser.h
        const PXResult resultGet = PXErrorCurrent(resultGetID);

        if(!resultGetID)
        {
            return resultGet;
        }

        PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&rect, pxRectangleXYWHI32);
#endif


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXNativDrawText,
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
    }
}

PXResult PXAPI PXNativDrawWindowResize(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int width, const int height)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    const int resultID = 0;//XResizeWindow(Display *display, Window w, width, height);
    pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop
    const UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER;
    const PXBool success = SetWindowPos
    (
        pxWindow->Info.Handle.WindowHandle,
        PXNull, 
        0, 
        0,
        width, 
        height,
        flags
    ); // Windows 2000, User32.dll
    pxActionResult = PXErrorCurrent(success);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXResult PXAPI PXNativDrawWindowMoveAndResize(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height)
{
    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    const int resultID = 0;//XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned int width, unsigned int height);
    pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop
    const PXBool success = MoveWindow(pxWindow->Info.Handle.WindowHandle, x, y, width, height, PXTrue); // Windows 2000, User32.dll, winuser.h
    pxActionResult = PXErrorCurrent(success);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXResult PXAPI PXNativDrawIcon(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, const int x, const int y, const int size)
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

PXResult PXAPI PXNativDrawCursorPosition(PXNativDraw PXREF pxNativDraw, PXVector2I32S PXREF position, const PXBool isWrite)
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

    const PXResult result = PXErrorCurrent(success);

    return result;

#else
    return PXActionRefusedTypeNotSupported;
#endif
}

PXResult PXAPI PXNativDrawCursorCollisionCheck(PXNativDraw PXREF pxNativDraw)
{
    const int cursorSize = 16;
    PXVector2I32S cursorPosition;

    PXNativDrawCursorPosition(pxNativDraw, &cursorPosition, 0);

    PXDictionary PXREF pxDictionaryUI = &pxNativDraw->ResourceManager->GUIElementLookup;

    for(size_t i = 0; i < pxDictionaryUI->List.EntryAmountUsed; i++)
    {
        PXVector2I32S windowPosition;

        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(pxDictionaryUI, i, &pxDictionaryEntry);

        PXWindow PXREF pxWindow = *(PXWindow**)pxDictionaryEntry.Value;

        #if OSWindows
        RECT winRec;

        GetWindowRect(pxWindow->Info.Handle.WindowHandle, &winRec);
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
        const PXI32U valueBefore = pxWindow->Info.Behaviour;

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
            const BOOL xx = RedrawWindow(pxWindow->Info.Handle.WindowHandle, PXNull, PXNull, RDW_INVALIDATE);

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

PXResult PXAPI PXNativDrawWindowEventPoll(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
    if(!(pxNativDraw && pxWindow))
    {
        return PXActionRefusedArgumentNull;
    }

    if(!pxWindow->Info.Handle.WindowHandle)
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

        for(PXI32U i = 0; i < amountOfEventsPending; ++i)
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
    const HWND windowHandle = pxWindow->Info.Handle.WindowHandle;

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

PXResult PXAPI PXNativDrawWindowIDValid(PXNativDraw PXREF pxNativDraw, const PXWindowHandle pxNativDrawWindowHandle)
{
    if(!(pxNativDraw && pxNativDrawWindowHandle))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix

    // This function does not exist, we emulate this behaviour
    // by just fetching attributes. if we fail, the ID is invalid.
    PXWindow pxWindow;

    const PXResult fetchResult = PXNativDrawWindowFetch(pxNativDraw, &pxWindow, pxNativDrawWindowHandle);

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

PXResult PXAPI PXNativDrawWindowFetch(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const PXWindowHandle pxNativDrawWindowHandle)
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

PXResult PXAPI PXNativDrawFrameBufferCreate(PXNativDraw PXREF pxNativDraw, PXFrameBuffer PXREF pxFrameBuffer, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo)
{
    PXAssert(pxFrameBufferCreateInfo->WindowHandle, "Err");

    // Get if not exists
    if(!pxFrameBufferCreateInfo->WindowDeviceContext)
    {
        pxFrameBufferCreateInfo->WindowDeviceContext = GetDC(pxFrameBufferCreateInfo->WindowHandle);
    }

    HDC memoryDeviceContext = CreateCompatibleDC(pxFrameBufferCreateInfo->WindowDeviceContext);
    HBITMAP framebuffer = CreateCompatibleBitmap
    (
        memoryDeviceContext,
        pxFrameBufferCreateInfo->Width,
        pxFrameBufferCreateInfo->Height
    );

    pxFrameBuffer->GDI.MemoryDeviceContext = memoryDeviceContext;
    pxFrameBuffer->GDI.FrameBufferTexture = framebuffer;
    pxFrameBuffer->Width = pxFrameBufferCreateInfo->Width;
    pxFrameBuffer->Height = pxFrameBufferCreateInfo->Height;

    SelectObject(memoryDeviceContext, framebuffer);

    return PXActionSuccessful;
}

void PXAPI PXNativDrawScreenSizeGet(PXRectangleXYWHI32 PXREF pxRectangleXYWH)
{
#if OSUnix

    // To direct eqivilant, emulate with X11.
    Display PXREF displayHandle = XOpenDisplay(NULL);

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

PXResult PXAPI PXNativDrawClear(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
        "Clear",
        "PXID:%i",
        pxWindow->Info.ID
    );
#endif

#if OSUnix
    const int resultID = XClearWindow(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowHandle);
#elif OSWindows
    // Does this exists?

    //GetUpdateRect();
#endif

    return PXActionSuccessful;
}


GLuint fontBase;
GLYPHMETRICSFLOAT* gmf = 0; // Unicode range

PXResult PXAPI PXNativDrawA
(
    PXNativDraw PXREF pxNativDraw,
    PXWindow PXREF pxWindow,
    int x,
    int y,
    int width,
    int height,
    const char* text,
    PXSize textSize,
    const PXI32U behaviour
)
{
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXGraphic PXREF pxGraphic = PXGraphicInstantiateGET();
    PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;

    PXWindowBrush PXREF brushText = pxGUITheme->TextWhite;

    if(!(text && textSize))
    {
       // return PXActionRefusedArgumentNull;
    }


    if(!text)
    {
        text = "[NO TEXT]";
        textSize = 10;
    }

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {

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
                pxWindow->Info.Handle.WindowHandle,
                pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
                xMX,
                yMX,
                text,
                textSize
            );
#elif OSWindows

            const PXRectangleXYWHI32 pxRectangleXYWHI32 = { x, y, width, height };

            RECT rectangle;
            PXRectangleXYWHI32ToLTRBI32(&pxRectangleXYWHI32, (PXRectangleLTRBI32*)&rectangle);

            UINT format = DT_SINGLELINE | DT_NOCLIP;
            format |= PXFlagIsSet(behaviour, PXWindowAllignTop) * DT_TOP;
            format |= PXFlagIsSet(behaviour, PXWindowAllignLeft) * DT_LEFT;
            format |= PXFlagIsSet(behaviour, PXWindowAllignRight) * DT_RIGHT;
            format |= PXFlagIsSet(behaviour, PXWindowAllignBottom) * DT_BOTTOM;

            if(PXFlagIsSet(behaviour, PXWindowAllignCenter))
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

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            SelectObject(pxWindow->DeviceContextHandle, pxGUITheme->FontTitle->Info.Handle.BrushHandle);

            if(!gmf)
            {
                int glyphsIndexFirst = 0;
                int glyphsAmount = 65536;// Full Unicode range
                float maximumChordalDeviation = 0;
                float extrusion = 0; // Add to Z axis, 0=No extrusion

                gmf = calloc(glyphsAmount, sizeof(GLYPHMETRICSFLOAT));
                fontBase = pxOpenGL->Binding.GenLists(glyphsAmount);

                BOOL is = wglUseFontOutlinesW
                (
                    pxWindow->DeviceContextHandle, 
                    glyphsIndexFirst,
                    glyphsAmount, 
                    fontBase, 
                    maximumChordalDeviation, 
                    extrusion,
                    WGL_FONT_POLYGONS, 
                    gmf
                ); // Ungodly slow!!
            }



            float offset = 0.005;
            float scaling = 0.040f;
            float x = -1 + offset*4;
            float y = -1 + 0.1;
            float xSpacer = 0.5;
            float xScale = scaling * 1;
            float yScale = scaling * pxWindow->Position.Form.Width / (PXF32)pxWindow->Position.Form.Height; // pxWindow->Position.Form.Height / (PXF32)pxWindow->Position.Form.Width;

            
            //pxOpenGL->Binding.Scalef(0.6, 0.6, 0.1);
            pxOpenGL->Binding.LineWidth(2),
               // pxOpenGL->Binding.Translatef(-1, -1, 0.0f); // Shadow offset

            pxOpenGL->Binding.PushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
            pxOpenGL->Binding.MatrixMode(GL_MODELVIEW);
            pxOpenGL->Binding.PushMatrix();

            
            pxOpenGL->Binding.Translatef(x, y, 0.0f); // Shadow offset
            pxOpenGL->Binding.Scalef(xScale, yScale, 1.0);
            pxOpenGL->Binding.Color3f(0.2,0.2,0.2);

            for(int i = 0; text[i] != L'\0'; ++i) 
            {
                pxOpenGL->Binding.PushMatrix();
                pxOpenGL->Binding.Translatef(-1 + (i* xSpacer) + gmf[text[i]].gmfCellIncX, 0.0f, 0.0f);
                pxOpenGL->Binding.CallList(fontBase + text[i]);
                pxOpenGL->Binding.PopMatrix();
            }

            pxOpenGL->Binding.LoadIdentity();
            pxOpenGL->Binding.Translatef(x + scaling * offset, y+ scaling * offset, 0.0f); // Main text
            pxOpenGL->Binding.Scalef(xScale, yScale, 1.0);
            pxOpenGL->Binding.Color3f(brushText->ColorDate.Red, brushText->ColorDate.Green, brushText->ColorDate.Blue);
            for(int i = 0; text[i] != L'\0'; ++i) 
            {
                pxOpenGL->Binding.PushMatrix();
                pxOpenGL->Binding.Translatef(-1 + (i* xSpacer) + gmf[text[i]].gmfCellIncX, 0.0f, 0.0f);
                pxOpenGL->Binding.CallList(fontBase + text[i]);
                pxOpenGL->Binding.PopMatrix();
            }

            pxOpenGL->Binding.Translatef(0, 0, 0);
            pxOpenGL->Binding.PopMatrix();
            pxOpenGL->Binding.PopAttrib();
            pxOpenGL->Binding.Flush();

            break;
        }

        default:
            break;
    }







#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
        "Text",
        "PXID:%-4i X:%-4i Y:%-4i %2i %s",
        pxWindow->Info.ID,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        textSize,
        text
    );
#endif
}

PXResult PXAPI PXNativDrawW(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const wchar_t PXREF text, const PXSize textSize)
{
#if OSUnix
    // For UNICODE
    // const int resultID = XDrawString16(Display * display, Drawable d, GC gc, int x, int y, XChar2b * string, int length);

#elif OSWindows

    RECT rectangle;
    UINT format;

    const int nextHeight = DrawTextExW
    (
        pxWindow->DeviceContextHandle,
        text,
        textSize,
        &rectangle,
        format,
        PXNull
    ); // Windows 2000, User32.dll, winuser.h
    const PXBool success = 0 != nextHeight;
    const PXResult result = PXErrorCurrent(success);

    return result;

#else
    return NotSupport;
#endif
}

PXResult PXAPI PXNativDrawPoint(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y)
{
#if OSUnix
    const int resultID = 0;//XDrawPoint(Display *display, Drawable d, GC gc, int x, int y);
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXNativDrawPoints(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height)
{
#if OSUnix
    const int resultID = 0;//XDrawPoints(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXNativDrawLine(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x1, const int y1, const int x2, const int y2)
{
#if OSUnix
    const resultID = 0;//XDrawLine(Display *display, Drawable d, GC gc, x1, y1, x2, y2);
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows

    POINT point;

    // Move to a start point, no drawing happens here
    const BOOL successMove = MoveToEx(pxWindow->DeviceContextHandle, x1, y1, &point);

    // use the previous step to draw a line from there to here
    const BOOL successDraw = LineTo(pxWindow->DeviceContextHandle, x2, y2);

#else
    return PXNotSupport;
#endif
}

PXResult PXAPI PXNativDrawLines(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height)
{
#if OSUnix
    XPoint points;
    int npoints = 0;
    int mode = 0;

    const resultID = XDrawLines
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxWindow->Info.Handle.WindowHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        &points,
        npoints,
        mode
    );
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    const BOOL success = PolylineTo(pxWindow->DeviceContextHandle, 0, 0);
#else
    return PXNotSupport;
#endif
}

PXResult PXAPI PXNativDrawRectangle(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height, PXWindowBrush PXREF pxWindowBrush)
{
    PXGraphic* pxGraphic = PXGraphicInstantiateGET();
    PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;

  //  PXGUI

    PXGUITheme* pxGUITheme = PXGUIThemeGet();
    PXWindowBrush* brushMainPrimary = pxGUITheme->MainPrimary;
    PXWindowBrush* brushMainSecoundary = pxGUITheme->MainSecoundary;

    PXResult pxResult = PXActionInvalid;

    PXRectangleLTRBI32 pxRectangleLTRBI32;
    PXRectangleXYWHI32 pxRectangleXYWHI32 = { x, y, width, height };
    PXRectangleXYWHI32ToLTRBI32(&pxRectangleXYWHI32, &pxRectangleLTRBI32);

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
#if OSUnix
            const int resultID = XFillRectangle
            (
                pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
                pxWindow->Info.Handle.WindowHandle,
                pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
                x,
                y,
                width,
                height
            );
            pxResult = PXActionSuccessful;// PXWindowErrorFromXSystem(resultID);
#elif OSWindows

            RECT* rect = (RECT*)&pxRectangleLTRBI32;

            const int resultID = FillRect
            (
                pxWindow->DeviceContextHandle,
                rect,
                pxWindowBrush->Info.Handle.BrushHandle // TODO: !!!
            ); // Windows 2000, User32.dll, winuser.h 
            pxResult = PXErrorCurrent(0 != resultID);
#endif

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNativDrawText,
                "Rectangle",
                "<PXID:%-4i> L:%-4i T:%-4i R:%-4i B:%-4i",
                pxWindow->Info.ID,
                rect->left,
                rect->top,
                rect->right,
                rect->bottom
            );
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            PXRectangleVertexF32 pxRectangleVertexF32 = 
            {
                -1+x,
                -1+y,
                -1+(width / (PXF32)pxWindow->Position.Form.Width) *2,
                -1+ (height / (PXF32)pxWindow->Position.Form.Height)*2
            };

            PXGUIProperty pxGUIProperty;
            pxGUIProperty.Property = PXUIElementPropertySize;
            PXGUIPropertyFetch(pxWindow, &pxGUIProperty, 1, PXFalse);

          //  PXRectangleXYWHI32ToVertex(&pxRectangleXYWHI32, &pxGUIProperty.Size, &pxRectangleVertexF32);

            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            pxOpenGL->Binding.Enable(GL_LINE_SMOOTH);
            pxOpenGL->Binding.Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);


            // Draw gradient background
            pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            pxOpenGL->Binding.Begin(GL_QUADS);
            pxOpenGL->Binding.Color3bv(&brushMainPrimary->ColorDate);
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.BX, pxRectangleVertexF32.BY); // +1.0f - offset, 1.0f
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.AX, pxRectangleVertexF32.BY); // -1.0f + offset, 1.0f
            pxOpenGL->Binding.Color3bv(&brushMainSecoundary->ColorDate);   
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.AX, pxRectangleVertexF32.AY); // -1.0f + offset, -1.0f
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.BX, pxRectangleVertexF32.AY); // +1.0f - offset, -1.0f
            pxOpenGL->Binding.End();

            pxOpenGL->Binding.LineWidth(1);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXNativDrawText,
                "Rectangle",
                "<PXID:%-4i> X1:%-4i Y1:%-4i X2:%-4i Y2:%-4i",
                pxWindow->Info.ID,
                pxRectangleVertexF32.AX,
                pxRectangleVertexF32.AY,
                pxRectangleVertexF32.BX,
                pxRectangleVertexF32.BY
            );
#endif

            break;
        }
        default:
            return PXActionInvalid;
    }

    return pxResult;
}

PXResult PXAPI PXNativDrawRectangleRounded(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height)
{
#if OSUnix

    // does not exist?

#elif OSWindows


    const BOOL success = RoundRect
    (
        pxWindow->DeviceContextHandle,
        0,
        0,
        0,
        0,
        0,
        0
    );
#endif
}

PXResult PXAPI PXNativDrawIconLoad(PXNativDraw PXREF pxGUIManager, PXIcon PXREF pxIcon, const char* iconName)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDrawText,
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

PXResult PXAPI PXNativDrawIconDraw(PXNativDraw PXREF pxGUIManager, PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, const int x, const int y, const int width, const int height)
{
    if(!(pxGUIManager && pxWindow && pxIcon))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows && 0
    const BOOL result = DrawIconEx
    (
        pxWindow->DeviceContextHandle,
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

PXResult PXAPI PXNativDrawEventConsumer(PXNativDraw PXREF pxNativDraw, PXWindowEvent PXREF pxWindowEvent)
{
    // Invoke
    PXWindow PXREF pxWindow = pxWindowEvent->UIElementReference;

    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeElementClick:
        {
            PXLogPrint
            (
                PXLoggingEvent,
                PXNativDrawText,
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
                PXLoggingEvent,
                PXNativDrawText,
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
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXNativDrawText,
                "Event-Move",
                ""
            );
#endif

            break;
        }
        case PXWindowEventTypeElementResize:
        {
            PXWindowEventResize PXREF pxWindowEventResize = &pxWindowEvent->Resize;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXNativDrawText,
                "Window-Resize",
                "<%ix%i>",
                pxWindowEventResize->Width,
                pxWindowEventResize->Height
            );
#endif

            PXNativeDrawChildrenReDraw(pxNativDraw, pxWindow);

            if(pxWindow)
            {
                pxWindow->Position.Form.Width = pxWindowEventResize->Width;      
                pxWindow->Position.Form.Height = pxWindowEventResize->Height;
                //pxWindow->HasSizeChanged = PXYes;

                if(pxWindow->Info.Hierarchy.ChildFirstborn)
                {
                    PXWindow PXREF pxWindowSub = (PXWindow*)pxWindow->Info.Hierarchy.ChildFirstborn;
                    
                    // Will trigger WM_SIZE, this is recusive
                    InvalidateRect(pxWindowSub->Info.Handle.WindowHandle, NULL, TRUE);
                   // ShowWindow(pxWindowSub->Info.Handle.WindowHandle, SW_HIDE);
                }

                PXWindow* windowCursor = (PXWindow*)pxWindow->Info.Hierarchy.Sibling->Yourself;

                while(windowCursor)
                {
                    InvalidateRect(windowCursor->Info.Handle.WindowHandle, NULL, TRUE);
                    //ShowWindow(windowCursor->Info.Handle.WindowHandle, SW_HIDE);

                    windowCursor = (PXWindow*)windowCursor->Info.Hierarchy.Sibling->Yourself;
                }
            }


            // Does not work here, as this is not updated for now!
           //PXNativeDrawRefreshSizeAllChildren(pxNativDraw, pxWindowEvent->UIElementReference);



            // PXWindowhSizeRefresAll(pxGUIManager, pxWindowEvent->UIElementReference);

   

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
        PXWindow PXREF pxWindow = pxWindowEvent->UIElementReference;

        if(pxWindow->InteractCallBack)
        {
            pxWindow->InteractCallBack(pxWindow->InteractOwner, pxWindowEvent);
        }
    }

    if(pxWindowEvent->UIElementSender)
    {
        PXWindow PXREF pxWindow = pxWindowEvent->UIElementSender;

        if(pxWindow->InteractCallBack)
        {
            pxWindow->InteractCallBack(pxWindow->InteractOwner, pxWindowEvent);
        }
    }

    return PXActionSuccessful;
}


#if OSUnix
void PXNativDrawEventTranslator(PXNativDraw PXREF pxNativDraw, const XEvent PXREF xEventData)
{
    PXGUIManager* pxGUIManager = pxNativDraw->GUISystem;

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
            const KeySym keySym = XKeycodeToKeysym(pxGUIManager->DisplayCurrent.DisplayHandle, keyCode, 0);
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

            PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);

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

            PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);

            break;
        }
        case MotionNotify:
        {
            // printf("[Event] MotionNotify \n");

            //  pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
            //  PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);

            break;
        }
        case EnterNotify:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusEnter;
            PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
            break;
        }
        case LeaveNotify:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusLeave;
            PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
            break;
        }
        case FocusIn:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusEnter;
            // PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
            break;
        }
        case FocusOut:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementFocusLeave;
            // PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
            break;
        }
        case KeymapNotify:
        {
            printf("[Event] KeymapNotify \n");

            break;
        }
        case Expose:
        {
            XExposeEvent PXREF xExposeEventData = &xEventData->xexpose;
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

            PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);

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
            const XGenericEventCookie PXREF cookie = &xEventData->xcookie; // Make Copy
            const int result = XGetEventData(pxGUIManager->DisplayCurrent.DisplayHandle, &cookie);
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

                            PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
                        }
                    }
                }
            }
            else
            {
                printf("[Event] GenericEvent %i\n", cookie->evtype);
            }

            XFreeEventData(pxGUIManager->DisplayCurrent.DisplayHandle, &cookie);

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
PXBool PXAPI PXWindowRender(PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXNativDraw PXREF pxNativDraw = PXNativDrawInstantance();

    PAINTSTRUCT paintStruct;
    PXWindow* pxWindow = PXNull;
    const HWND windowHandle = pxWindowDrawInfo->hwnd;

    if(pxNativDraw->ResourceManager)
    {
        PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &windowHandle, &pxWindow);
    }

    if(!pxWindow)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXNativDrawText,
            PXDrawText,
            "HANDLE:%p, was not found! Not registered?",
            windowHandle
        );
#endif

        return PXFalse; // Window not found in engine
    } 

    if(!pxWindow->DrawFunction)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXNativDrawText,
            PXDrawText,
            "<PXID:%i> Name:%s, has no draw function. Please check!",
            pxWindow->Info.ID,
            pxWindow->NameContent
        );
#endif

        return PXFalse; // Window has no function to draw
    }

    const PXBool shallDraw = pxWindow->DrawFunction && (PXResourceInfoRender & pxWindow->Info.Behaviour);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXNativDrawText,
        PXDrawText,
        "<PXID:%i> Draw:%3s, Name:%s",
        pxWindow->Info.ID,
        shallDraw ? "Yes" : " No",
        pxWindow->NameContent
    );
#endif

    if(!shallDraw)
    {
        return PXFalse; // Window is not set to draw
    }

    // Update raw size of window.. again? 
    RECT rect;
    GetClientRect(pxWindow->Info.Handle.WindowHandle, &rect);

    PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&rect, &pxWindow->Position.Form);

   // PXNativeDrawChildrenReDraw(pxNativDraw, pxWindow);

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
            // Required to be able to use GDI calls
            pxWindowDrawInfo->hDC = BeginPaint(windowHandle, &paintStruct); // This HDC is ONLY valid for GDI calls!
            paintStruct.fErase = 1;
            pxWindowDrawInfo->bErase = paintStruct.fErase;
            //pxWindow->DeviceContextHandle = pxWindowDrawInfo->hDC;

           // PXGUIDrawRectangle3D(pxWindow, 20, 20, 100, 100);


            //PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&paintStruct.rcPaint, &pxWindow->Position.Form);     
            //pxWindow->Position.Form.Width = 200;
            //pxWindow->Position.Form.Height = 200;

   

            pxWindow->DrawFunction(pxWindow, pxWindowDrawInfo);

            // Finalize GDI calls
            const BOOL endSuccess = EndPaint(windowHandle, &paintStruct);

            //SwapBuffers(pxWindowDrawInfo->hDC);

            return TRUE; // We did a custom draw, so return true to mark this as handled
        }
        case PXGraphicSystemOpenGL:
        {
            // Although we dont use any GDI calls, as we draw a 
            // physical window, the WM_PAINT message expects to handle 
            // this handshake. It revalidates the region, can prevent 
            // multible triggers to this event. Less lag?
            const HDC hdc = BeginPaint(windowHandle, &paintStruct);
            const BOOL endSuccess = EndPaint(windowHandle, &paintStruct);

            //PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&paintStruct.rcPaint, &pxWindow->Position.Form);

            //PXGraphic* pxGraphic = PXGraphicInstantiateGET();
           // PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;            
            BOOL result = FALSE;

            result = wglMakeCurrent(pxWindow->DeviceContextHandle, pxWindow->RenderContext); // Bind
            glViewport(0, 0, pxWindow->Position.Form.Width, pxWindow->Position.Form.Height);

            pxWindow->DrawFunction(pxWindow, pxWindowDrawInfo);

            result = wglMakeCurrent(pxWindow->DeviceContextHandle, 0); // un-bind

            result = SwapBuffers(pxWindow->DeviceContextHandle);

            PXNativeDrawChildrenReDraw(pxNativDraw, pxWindow);

            return TRUE; // We did a custom draw, so return true to mark this as handled
        }
        default:
        {
            PXAssert(PXFalse, "CANT");
            break;
        }
    }

    return PXFalse;
}

LRESULT CALLBACK PXNativDrawEventTranslator(const HWND windowID, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    PXNativDraw PXREF pxNativDraw = PXNativDrawInstantance();
    PXWindow* pxWindowParrent = PXNull;

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowHandle = windowID;
    pxWindowEvent.EventID = eventID;
    pxWindowEvent.ParamW = wParam;
    pxWindowEvent.ParamL = lParam;

    if(pxNativDraw->ResourceManager)
    {
        PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &windowID, &pxWindowEvent.UIElementReference);  
    }

    if(pxWindowEvent.UIElementReference)
    {
        pxWindowParrent = (PXWindow*)pxWindowEvent.UIElementReference->Info.Hierarchy.Parrent->Yourself;
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
            pxWindow.Info.Handle.WindowHandle = windowID;

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
            NMHDR PXREF notificationCode = (NMHDR*)lParam;
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
                        PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &sourceObject, &pxTreeViewContainer);

                        // Fetch treeviewitem object
                        PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &itemHandle, &pxTreeViewItem);
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
                    PXWindow* pxWindow = PXNull;

                    PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &sourceObject, &pxWindow);

                    PXWindowExtendedBehaviourTab PXREF pxWindowExtendedBehaviourTab = (PXWindowExtendedBehaviourTab*)pxWindow->ExtendedData;

                    // Hide current page
                    const PXBool isValidIndex = pxWindowExtendedBehaviourTab->TABPageAmount >= (pageID + 1);

                    BOOL success = 0;

                    for(PXSize i = 0; i < pxWindowExtendedBehaviourTab->TABPageAmount; i++)
                    {
                        PXWindow PXREF pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

                        PXGUIProperty pxWindowUpdateInfo;
                        PXClear(PXGUIProperty, &pxWindowUpdateInfo);
                        pxWindowUpdateInfo.Show = PXFalse;
                        //pxWindowUpdateInfo.WindowCurrent = pxWindowTABPage;
                        pxWindowUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXNativDrawWindowProperty(pxNativDraw, pxWindowTABPage, &pxWindowUpdateInfo, 1);
                    }

                    // Show new page
                    if(isValidIndex)
                    {
                        PXWindow PXREF pxWindowTABPageSelected = &pxWindowExtendedBehaviourTab->TABPageList[pageID];

                        PXLogPrint
                        (
                            PXLoggingInfo,
                            PXNativDrawText,
                            "Event",
                            "Tab Select <0x%p>, %15s ID:<%i>",
                            sourceObject,
                            "", //pxUIElementEEE->NameData,
                            pageID
                        );

                        PXGUIProperty pxWindowUpdateInfo;
                        PXClear(PXGUIProperty, &pxWindowUpdateInfo);
                        pxWindowUpdateInfo.Show = PXTrue;
                        //pxWindowUpdateInfo.WindowCurrent = pxWindowTABPageSelected;
                        pxWindowUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXNativDrawWindowProperty(pxNativDraw, pxWindowTABPageSelected, &pxWindowUpdateInfo, 1);
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

            PXWindowDrawInfo pxWindowDrawInfo;
            PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);
            pxWindowDrawInfo.hwnd = windowID;

            const PXBool isOK = PXWindowRender(&pxWindowDrawInfo);

            if(isOK)
            {
                return TRUE;
            }

            break;
        }
        case WM_DRAWITEM:
        {
            const HWND identifier = (HWND)wParam;
            DRAWITEMSTRUCT PXREF drawItemInfo = (DRAWITEMSTRUCT*)lParam;

            PXWindowDrawInfo pxWindowDrawInfo;
            PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);
            pxWindowDrawInfo.hwnd = drawItemInfo->hwndItem;
            pxWindowDrawInfo.hDC = drawItemInfo->hDC;

            PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&drawItemInfo->rcItem, &pxWindowDrawInfo.RectangleXYWH);

            const PXBool isOK = PXWindowRender(&pxWindowDrawInfo);

            if(isOK)
            {
                return TRUE;
            }

            break;
        }
        case WM_PRINTCLIENT:
        {
            PXWindowDrawInfo pxWindowDrawInfo;
            PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);
            pxWindowDrawInfo.hwnd = windowID;
            pxWindowDrawInfo.hDC = (HDC)wParam;
            //  pxWindowDrawInfo.rcDirty = &rc;
            pxWindowDrawInfo.bErase = TRUE;

            RECT rc;

            GetClientRect(windowID, &rc);

            const PXBool isOK = PXWindowRender(&pxWindowDrawInfo);

            if(isOK)
            {
                return TRUE;
            }

            break;
        }
        case WM_POWERBROADCAST:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                "Window",
                "EVENT",
                "WM_POWERBROADCAST"
            );
#endif

            // Notifies applications that a power-management event has occurred.
            switch(wParam)
            {
                case PBT_APMQUERYSUSPEND:
                {
                    break;
                }
                //case PBTF_APMRESUMEFROMFAILURE:
                case PBT_APMQUERYSTANDBY: // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMQUERYSUSPENDFAILED:
                {
                    break;
                }
                case PBT_APMQUERYSTANDBYFAILED:  // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMSUSPEND: // System is suspending operation.
                {
                    break;
                }
                case PBT_APMSTANDBY:  // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMRESUMECRITICAL:
                {
                    break;
                }
                case PBT_APMRESUMESUSPEND: // Operation is resuming by user input from a low-power state.
                {
                    break;
                }
                case PBT_APMRESUMESTANDBY:  // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMBATTERYLOW:
                {
                    break;
                }
                case PBT_APMPOWERSTATUSCHANGE:  // Power status has changed.
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingWarning,
                        "Window",
                        "Power",
                        "Status changed!"
                    );
#endif

                    /*
                    pxWindowEvent.Type = PXWindowEventTypeElementResize;
                    pxWindowEvent.Resize.Width = LOWORD(lParam);
                    pxWindowEvent.Resize.Height = HIWORD(lParam);

                    PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);
                    */
                    break;
                }
                case PBT_APMOEMEVENT:
                {
                    break;
                }
                case PBT_APMRESUMEAUTOMATIC: // Operation is resuming automatically from a low-power state.
                {
                    break;
                }
                case PBT_POWERSETTINGCHANGE: // A power setting change event has been received. 
                {
                    POWERBROADCAST_SETTING PXREF powerBroadCastSetting = (POWERBROADCAST_SETTING*)lParam;

                    break;
                }
            }

            break;
        }

        case WM_DEVICECHANGE:
        {
            // Triggers in any case
            UINT eventID = (UINT)wParam;

            switch(eventID)
            {
                case DBT_DEVNODES_CHANGED: // A device has been added to or removed from the system.
                {
                    // We get no additional info??
                    break;
                }
                case DBT_QUERYCHANGECONFIG: // Permission is requested to change the current configuration(dock or undock).
                {
                    break;
                }
                case DBT_CONFIGCHANGED: // The current configuration has changed, due to a dock or undock.
                {
                    break;
                }
                case DBT_CONFIGCHANGECANCELED: // A request to change the current configuration(dock or undock) has been canceled.
                {
                    break;
                }
                case DBT_DEVICEARRIVAL: // 	A device or piece of media has been inserted and is now available.
                {
                    break;
                }
                case DBT_DEVICEQUERYREMOVE: // 	Permission is requested to remove a device or piece of media.Any application can deny this request and cancel the removal.
                {
                    break;
                }
                case DBT_DEVICEQUERYREMOVEFAILED: // A request to remove a device or piece of media has been canceled.
                {
                    break;
                }
                case DBT_DEVICEREMOVEPENDING: //A device or piece of media is about to be removed.Cannot be denied.
                {
                    break;
                }
                case DBT_DEVICEREMOVECOMPLETE: // A device or piece of media has been removed.
                {
                    break;
                }
                case DBT_DEVICETYPESPECIFIC: // A device - specific event has occurred.
                {
                    break;
                }
                case DBT_CUSTOMEVENT: // A custom event has occurred.
                {
                    break;
                }
                case DBT_USERDEFINED: // The meaning of this message is user - defined.
                {
                    break;
                }
                default:
                {
                    break;
                }
            }


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "Name:DEVICECHANGE, ID:%4.4x", 
                eventID
            );
#endif

            break;
        }

        case WM_INPUT_DEVICE_CHANGE: 
        {
            // WARNING! 
            // This event is only triggerd when we have registerd a raw device!

            const UINT eventID = (UINT)wParam;

            PXDeviceInputUpdateInfo pxDeviceInputUpdateInfo;
            PXClear(PXDeviceInputUpdateInfo, &pxDeviceInputUpdateInfo);
            pxDeviceInputUpdateInfo.DeviceHandle = (HANDLE)lParam;

            switch(eventID)
            {
                case GIDC_ARRIVAL:
                {
                    pxDeviceInputUpdateInfo.Type = PXDeviceInputUpdateTypeArrival;
                    break;
                }
                case GIDC_REMOVAL:
                {
                    pxDeviceInputUpdateInfo.Type = PXDeviceInputUpdateTypeRemoval;
                    break;
                }
                default:
                {
                    pxDeviceInputUpdateInfo.Type = PXDeviceInputUpdateTypeInvalid;
                    break;
                }
            }

            PXDeviceInputUpdate(&pxDeviceInputUpdateInfo);

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
            pxWindowEvent.InputMouseMove.Position.X = GET_X_LPARAM(lParam);
            pxWindowEvent.InputMouseMove.Position.Y = GET_Y_LPARAM(lParam);

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
            //  PXWindow* pxWindow = PXNull;

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
                            pxWindowEvent.InputMouseMove.Axis.X = 0;
                            pxWindowEvent.InputMouseMove.Axis.Y = 0;
                            pxWindowEvent.InputMouseMove.Delta.X = rawInput.data.mouse.lLastX;
                            pxWindowEvent.InputMouseMove.Delta.Y = rawInput.data.mouse.lLastY;

                            PXCursorPositionGet
                            (
                                windowID,
                                &pxWindowEvent.InputMouseMove.Axis                            
                            );

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
            const PXI32U buttonType = wParam; // IDOK;
            const HANDLE handle = (HANDLE)lParam;

            PXWindow PXREF pxWindow = PXNull;

            if(pxNativDraw)
            {
                PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &handle, &pxWindow);
            }

            if(!pxWindow) // if we did not find the object, stop
            {
                break;
            }

            if(!(pxWindow->Info.Behaviour & PXResourceInfoActive))
            {
                // ShowWindow(pxWindow->ID, SW_HIDE);
            }

            pxWindowEvent.Type = PXWindowEventTypeElementClick;
            pxWindowEvent.UIElementSender = pxWindow;

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


#if 0
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
void PXAPI PXWindowChildListEnumerate(PXGUIManager PXREF pxGUIManager, PXWindow PXREF parent, PXBool visible)
{
#if 0
    for(PXSize i = 0; i < pxGUIManager->ResourceManager->GUIElementLookup.List.EntryAmountUsed; i++)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXWindow* childElement = PXNull;

        PXDictionaryIndex(&pxGUIManager->ResourceManager->GUIElementLookup, i, &pxDictionaryEntry);

        childElement = *(PXWindow**)pxDictionaryEntry.Value;

        PXWindow* parrent = (PXWindow*)childElement->Info.Hierarchy.Parrent;

        if(parrent)
        {
            if(parrent->Info.Handle.WindowHandle == parent->Info.Handle.WindowHandle)
            {
                // found child.

                PXWindowPropertyInfo pxWindowUpdateInfo;
                PXClear(PXWindowPropertyInfo, &pxWindowUpdateInfo);

                pxWindowUpdateInfo.WindowCurrent = childElement;
                pxWindowUpdateInfo.Show = visible;
                pxWindowUpdateInfo.Property = PXUIElementPropertyVisibility;

               // PXWindowUpdate(pxGUIManager, &pxWindowUpdateInfo, 1);
            }
        }
    }
#endif
}

BOOL CALLBACK PXNativeWindowListUpdateEnum(const HWND windowHandle, LPARAM lParam)
{
    // Fetch reference

#if 1
    PXLogPrint
    (
        PXLoggingInfo,
        "Windows",
        "Window",
        "Enum: %p",
        windowHandle
    );
#endif

    // Call recursive
    const BOOL success = EnumChildWindows
    (
        windowHandle,
        PXNativeWindowListUpdateEnum,
        lParam
    );

    return TRUE;
}

BOOL CALLBACK PXNativeWindowChildrenUpdateEnum(const HWND windowHandle, LPARAM lParam)
{
 //   const BOOL result = InvalidateRect(windowHandle, NULL, TRUE);

    SendMessageA(windowHandle, WM_PAINT, NULL, NULL);


    return TRUE;
}
#endif

PXResult PXAPI PXNativeWindowListUpdate(const PXWindowHandle pxWindowHandle)
{
#if 1
    PXLogPrint
    (
        PXLoggingInfo,
        "Windows",
        "Window ",
        "list update"
    );
#endif

#if OSUnix
#elif OSWindows

    const BOOL success = EnumChildWindows
    (
        pxWindowHandle,
        PXNativeWindowListUpdateEnum,
        0
    );

    

#endif

    return PXActionSuccessful;
}

BOOL CALLBACK PXNativeDrawRefreshSizeAllChildrenEEEEE(HWND windowHandle, PXNativDraw PXREF pxNativDraw)
{
    PXWindow* pxWindow = 0;

    // Fetch window
    {
        PXDictionary* lookup = &pxNativDraw->ResourceManager->GUIElementLookup;

        PXDictionaryEntryFind(lookup, &windowHandle, &pxWindow);
    }


    PXGUIProperty pxGUIProperty;
    PXClear(PXGUIProperty, &pxGUIProperty);
   // pxWindowSizeInfo.WindowParentREF = ;
    //pxGUIProperty.WindowCurrent = pxWindow;
    pxGUIProperty.Property = PXUIElementPropertySizeParent;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);
    
    PXGUIPropertyFetch(pxWindow, &pxGUIProperty, 1, PXFalse);

    pxUIElementPositionCalulcateInfo.WindowWidth = pxGUIProperty.Size.Width;
    pxUIElementPositionCalulcateInfo.WindowHeight = pxGUIProperty.Size.Height;

    if(pxUIElementPositionCalulcateInfo.WindowWidth == 0)
    {
        pxUIElementPositionCalulcateInfo.WindowWidth = 400;
        pxUIElementPositionCalulcateInfo.WindowHeight = 500;
    }

    PXUIElementPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);

    PXNativDrawWindowXYWH(pxNativDraw, pxWindow, &pxUIElementPositionCalulcateInfo.Size, PXTrue);

    PXLogPrint
    (
        PXLoggingWarning,
        "EEEE",
        "",
        ""
    );    
}


PXResult PXAPI PXNativeDrawChildrenReDraw(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXActionSuccessful;
    }

    const BOOL success = EnumChildWindows
    (
        pxWindow->Info.Handle.WindowHandle,
        PXNativeWindowChildrenUpdateEnum,
        pxNativDraw
    );

    return PXActionSuccessful;
}

void PXAPI PXNativeDrawRefreshSizeAllChildren(PXNativDraw PXREF pxNativDraw, PXWindow PXREF window)
{
    const BOOL success = EnumChildWindows
    (
        window->Info.Handle.WindowHandle,
        PXNativeDrawRefreshSizeAllChildrenEEEEE,
        pxNativDraw
    );


}

void PXAPI PXNativeDrawMouseTrack(PXWindow PXREF window)
{
#if OSUnix
#elif OSWindows
    TRACKMOUSEEVENT trackMouseEvent;
    trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
    trackMouseEvent.hwndTrack = window->Info.Handle.WindowHandle;
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

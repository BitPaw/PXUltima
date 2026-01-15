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
#include <PX/Math/PXCollision.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Input/PXInput.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

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
        return PXResultOK;
    }


    return PXResultOK;
}
#endif

PXResult PXAPI PXNativDrawWindowPrintHirachy(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, int depth)
{
#if 0
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

    for(PXHierarchicalNode* pxHierarchicalNode = pxWindow->Info.Hierarchy.Sibling; pxHierarchicalNode; pxHierarchicalNode = pxHierarchicalNode->ChildFirstborn)
    {
        PXWindow* sibling = (PXWindow*)pxHierarchicalNode;

        if(!sibling)
        {
            continue;
        }

        PXNativDrawWindowPrintHirachy(pxNativDraw, sibling, depth + 1);
    }
#endif
    return PXResultOK;
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

            return PXResultOK;
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

    return PXResultOK;
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

        return PXResultInvalid;
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

    return PXResultOK;

#elif OSWindows

    // Does not exists?

    return PXResultOK;

#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXNativDrawWindowProperty(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXGUIProperty PXREF pxWindowPropertyInfoList, const PXSize amount)
{
    PXWindow PXREF pxWindowParrent = 0;// (PXWindow*)pxWindow->Info.Hierarchy.Parrent;

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
                    return PXResultInvalid;
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

                HDC hdc = PXWindowDCGet(pxWindow);
                LONG ww = SetWindowLongA(hdc, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

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

                PXWindowPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXWindowPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                //PXWindowSizeInfo pxWindowSizeInfo;
                //PXWindowID pxWindowID = pxWindow->Parent ? pxWindow->Parent->ID : PXNull;

                //PXWindowSizeGet(PXNull, &pxWindowSizeInfo); // TODO: Problematic

                pxUIElementPositionCalulcateInfo.WindowWidth = sizeFetchInfo.Size.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = sizeFetchInfo.Size.Height;

                PXWindowPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);



                // If the window is an actual window, we will have bad allignment if we use MoveWindow
                //if(pxWindow->Type != PXUIElementTypeWindow)

#if 0
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
#endif
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
               // if(PXUIElementTypeRenderFrame == pxWindow->Type)
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


                const PXBool isWindowValid = PXWindowIsValid(pxWindow);
               

                if(show)
                {
                    PXWindowShow(pxWindow);

                   // pxWindow->Info.Behaviour |= PXECSInfoRender;
                }
                else
                {
                    PXWindowHide(pxWindow);

                    //pxWindow->Info.Behaviour &= ~PXECSInfoRender;
                }



              


  
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

                /*
                const BOOL success = EnumChildWindows
                (
                    pxWindow->Info.Handle.WindowHandle,
                    PXNativeWindowListUpdateEnum,
                    &showID
                );*/
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

    return PXResultOK;
}

PXResult PXAPI PXNativDrawWindowXYWH(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const PXBool doWrite)
{
#if 0
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


#elif OSWindows && 0
        RECT rect;

        PXAssert(pxWindow->Info.Handle.WindowHandle, "Err");

        //const PXBool result = GetWindowRect(pxWindow->ID, &rect); // Windows 2000, User32.dll, winuser.h
        const BOOL resultGetID = GetClientRect(pxWindow->Info.Handle.WindowHandle, &rect); // Windows 2000, User32.dll, winuser.h
        const PXResult resultGet = PXErrorCurrent(resultGetID);

        if(!resultGetID)
        {
            return resultGet;
        }
        PXWindowRectangleXYWH(pxWindow, pxRectangleXYWHI32);*/

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
#endif
}

PXResult PXAPI PXNativDrawWindowResize(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int width, const int height)
{
    PXResult pxActionResult = PXResultInvalid;

#if 0

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

#endif

    return pxActionResult;
}

PXResult PXAPI PXNativDrawWindowMoveAndResize(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height)
{
    PXResult pxActionResult = PXResultInvalid;

#if OSUnix && 0
    const int resultID = 0;//XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned int width, unsigned int height);
    pxActionResult = PXWindowErrorFromXSystem(resultID);
#elif PXOSWindowsDestop && 0
    const PXBool success = MoveWindow(pxWindow->Info.Handle.WindowHandle, x, y, width, height, PXTrue); // Windows 2000, User32.dll, winuser.h
    pxActionResult = PXErrorCurrent(success);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
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
#if 0
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
#endif
    return PXResultOK;
}

PXNativDraw* PXAPI PXNativDrawInstantance(void)
{
    return &_internalNativDraw;
}

PXResult PXAPI PXNativDrawWindowIDValid(PXNativDraw PXREF pxNativDraw, const PXWindowHandle pxNativDrawWindowHandle)
{
    if(!(pxNativDraw && pxNativDrawWindowHandle))
    {
        return PXResultRefusedParameterNull;
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

    return PXResultOK;

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

    return PXResultOK;

#elif OSWindows

    return PXActionRefusedNotImplemented;

#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
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
                    if(PXECSInfoRender & windowCurrent->Info.Flags)
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
    /*
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

    const PXBool shallDraw = pxWindow->DrawFunction && (PXECSInfoRender & pxWindow->Info.Behaviour);

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
    

    PXWindowRectangleLTRB();

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

    */

    return PXFalse;
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

    return PXResultOK;
}

BOOL CALLBACK PXNativeDrawRefreshSizeAllChildrenEEEEE(HWND windowHandle, PXNativDraw PXREF pxNativDraw)
{
#if 0
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

    PXWindowPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXWindowPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);
    
    PXGUIPropertyFetch(pxWindow, &pxGUIProperty, 1, PXFalse);

    pxUIElementPositionCalulcateInfo.WindowWidth = pxGUIProperty.Size.Width;
    pxUIElementPositionCalulcateInfo.WindowHeight = pxGUIProperty.Size.Height;

    if(pxUIElementPositionCalulcateInfo.WindowWidth == 0)
    {
        pxUIElementPositionCalulcateInfo.WindowWidth = 400;
        pxUIElementPositionCalulcateInfo.WindowHeight = 500;
    }

    PXWindowPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);

    PXNativDrawWindowXYWH(pxNativDraw, pxWindow, &pxUIElementPositionCalulcateInfo.Size, PXTrue);

    PXLogPrint
    (
        PXLoggingWarning,
        "EEEE",
        "",
        ""
    );    
#endif
}


PXResult PXAPI PXNativeDrawChildrenReDraw(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow)
{
#if 0
    if(!pxWindow)
    {
        return PXResultOK;
    }

    const BOOL success = EnumChildWindows
    (
        pxWindow->Info.Handle.WindowHandle,
        PXNativeWindowChildrenUpdateEnum,
        pxNativDraw
    );
#endif

    return PXResultOK;
}

void PXAPI PXNativeDrawRefreshSizeAllChildren(PXNativDraw PXREF pxNativDraw, PXWindow PXREF window)
{
#if 0
    const BOOL success = EnumChildWindows
    (
        window->Info.Handle.WindowHandle,
        PXNativeDrawRefreshSizeAllChildrenEEEEE,
        pxNativDraw
    );
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

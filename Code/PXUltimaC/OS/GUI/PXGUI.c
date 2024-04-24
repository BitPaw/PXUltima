#include "PXGUI.h"

#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Hardware/PXMonitor.h>
#include <OS/Async/PXAwait.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Console/PXConsole.h>
#include <Engine/PXEngine.h>

#if OSUnix

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xmd.h>

#define DefautPositionX 00000000000000
#define DefautPositionY 00000000000000

#define MouseButtonLeft 1
#define MouseButtonMiddle 2
#define MouseButtonRight 3
#define MouseScrollUp 4
#define MouseScrollDown 5

#elif OSWindows
#include <windowsx.h>
#include <WinUser.h>
#include <wtypes.h>
//#include <hidusage.h>
#include <Dbt.h>

#include <dwmapi.h> // Set tilebar color

#define Windows10DarkModeID 20 // DWMWA_USE_IMMERSIVE_DARK_MODE

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Comctl32.lib")

#define DefautPositionX CW_USEDEFAULT
#define DefautPositionY CW_USEDEFAULT

#endif

PXWindow* currentWindow = 0;

#if OSUnix
//#include <gtk/gtk.h>
#elif OSWindows
#include <Windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <Richedit.h>
#include <WindowsX.h>

//#include <ShObjIdl.h>

#if WindowsEnableModernColorSceme

// use this define to disable manifest dependency
#define _PFD_DISABLE_MANIFEST 1

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "Comctl32.lib")

#endif
#endif






#if OSUnix

#define CursorIconNormalID 1
#define CursorIconIBeamID 2
#define CursorIconWaitID 3
#define CursorIconCrossID 4
#define CursorIconUpID 5
#define CursorIconSizeID 6
#define CursorIconIconID 7
#define CursorIconResizeClockwiseCounterID 8
#define CursorIconResizeClockwiseID 10
#define CursorIconResizeHorizontalID 11
#define CursorIconResizeVerticalID 12
#define CursorIconResizeAllID 13
#define CursorIconCursorID 14
#define CursorIconNoAllowedID 15
#define CursorIconHandID 16
#define CursorIconAppStartingID 17

#elif OSWindows

#include <winuser.rh> // MISSING
#include <Engine/PXEngine.h>
#define CursorIconNormalID OCR_NORMAL
#define CursorIconIBeamID OCR_IBEAM
#define CursorIconWaitID OCR_WAIT
#define CursorIconCrossID OCR_CROSS
#define CursorIconUpID OCR_UP
#define CursorIconSizeID OCR_SIZE               // OBSOLETE: use OCR_SIZEALL
#define CursorIconIconID OCR_ICON               // OBSOLETE: use OCR_NORMAL
#define CursorIconResizeClockwiseCounterID OCR_SIZENWSE
#define CursorIconResizeClockwiseID OCR_SIZENESW
#define CursorIconResizeHorizontalID OCR_SIZEWE
#define CursorIconResizeVerticalID OCR_SIZENS
#define CursorIconResizeAllID OCR_SIZEALL
#define CursorIconCursorID OCR_ICOCUR             // OBSOLETE: use OIC_WINLOGO
#define CursorIconNoAllowedID OCR_NO

#if defined(WindowsAtleast2000)
#define CursorIconHandID OCR_HAND
#else
#define CursorIconHandID 32649
#endif

#if defined(WindowsAtleastNT)
#define CursorIconAppStartingID OCR_APPSTARTING
#else
#define CursorIconAppStartingID 32650
#endif

#endif



void PXAPI PXWindowEventConsumer(PXGUISystem* const pxGUISystem, PXWindowEvent* const pxWindowEvent)
{
    // Invoke

    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeClick:
        {
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event-Click",
                "Sender (%0xp)",
                pxWindowEvent->UIElementSender->ID
            );

            break;
        }
        case PXWindowEventTypeSelect:
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
        case PXWindowEventTypeDestroy:
        {

           // window->IsRunning = PXFalse;

            break;
        }

        case PXWindowEventTypeMove:
        {
#if PXLogEnable
            PXLoggingEventData pxLoggingEventData;
            PXClear(PXLoggingEventData, &pxLoggingEventData);
            pxLoggingEventData.ModuleSource = "Window";
            pxLoggingEventData.ModuleAction = "Event";
            pxLoggingEventData.PrintFormat = "Moved";
            pxLoggingEventData.Type = PXLoggingInfo;

            PXLogPrintInvoke(&pxLoggingEventData);
#endif

            break;
        }
        case PXWindowEventTypeResize:
        {
            PXWindowEventResize* const pxWindowEventResize = &pxWindowEvent->Resize;

#if PXLogEnable
            PXLoggingEventData pxLoggingEventData;
            PXClear(PXLoggingEventData, &pxLoggingEventData);
            pxLoggingEventData.ModuleSource = "Window";
            pxLoggingEventData.ModuleAction = "Event";
            pxLoggingEventData.PrintFormat = "Size changed to <%ix%i>";
            pxLoggingEventData.Type = PXLoggingInfo;

            PXLogPrintInvoke(&pxLoggingEventData, pxWindowEventResize->Width, pxWindowEventResize->Height);
#endif

           // PXGUIElementhSizeRefresAll(pxGUISystem, pxWindowEvent->UIElementReference);


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
        PXUIElement* const pxUIElement = pxWindowEvent->UIElementReference;

        if(pxUIElement->InteractCallBack)
        {
            pxUIElement->InteractCallBack(pxUIElement->InteractOwner, pxWindowEvent);
        }
    }

    if(pxWindowEvent->UIElementSender)
    {
        PXUIElement* const pxUIElement = pxWindowEvent->UIElementSender;

        if(pxUIElement->InteractCallBack)
        {
            pxUIElement->InteractCallBack(pxUIElement->InteractOwner, pxWindowEvent);
        }
    }
}


#if OSUnix
void PXWindowEventHandler(PXWindow* const pxWindow, const XEvent* const event)
{
    switch(event->type)
    {
        case KeyPress:
        case KeyRelease:
        {
            const XKeyEvent* keyEvent = &event->xkey;
            const unsigned int keyCode = keyEvent->keycode;
            const PXBool release = event->type == KeyRelease;
            const KeySym keySym = XKeycodeToKeysym(pxWindow->DisplayCurrent, keyCode, 0);
            const char* keyName = XKeysymToString(keySym);

            PXVirtualKey keyBoardKey = PXVirtualKeyFromID(keySym);

            PXKeyBoardKeyInfo keyBoardKeyInfo;

            keyBoardKeyInfo.Key = keyBoardKey;
            keyBoardKeyInfo.Mode = release ? PXKeyPressStateUp : PXKeyPressStateDown;
            keyBoardKeyInfo.Repeat = 0;
            keyBoardKeyInfo.ScanCode = keySym;
            keyBoardKeyInfo.SpecialKey = 0;
            keyBoardKeyInfo.KontextCode = 0;
            keyBoardKeyInfo.PreState = 0;
            keyBoardKeyInfo.GapState = 0;

            PXFunctionInvoke(pxWindow->KeyBoardKeyCallBack, pxWindow->EventReceiver, pxWindow, &keyBoardKeyInfo);

            if(release)
            {
                printf("[Event] Key-Release %2i %2i %s\n", keyCode, keySym, keyName);
            }
            else
            {
                printf("[Event] Key-Press %2i %2i %s\n", keyCode, keySym, keyName);
            }

            break;
        }
        case ButtonRelease:
        case ButtonPress:
        {
            const XButtonEvent* buttonEvent = &event->xbutton;
            const unsigned int buttonID = buttonEvent->button;
            const PXKeyPressState buttonState = event->type == ButtonPress ? PXKeyPressStateDown : PXKeyPressStateUp;
            PXMouseButton mouseButton = PXMouseButtonInvalid;

            switch(buttonID)
            {
                case MouseButtonLeft:
                    PXFunctionInvoke(pxWindow->MouseClickCallBack, pxWindow->EventReceiver, pxWindow, MouseButtonLeft, buttonState);
                    break;

                case MouseButtonMiddle:
                    PXFunctionInvoke(pxWindow->MouseClickCallBack, pxWindow->EventReceiver, pxWindow, MouseButtonMiddle, buttonState);
                    break;

                case MouseButtonRight:
                    PXFunctionInvoke(pxWindow->MouseClickCallBack, pxWindow->EventReceiver, pxWindow, MouseButtonRight, buttonState);
                    break;

                case MouseScrollUp:
                    PXFunctionInvoke(pxWindow->MouseScrollCallBack, pxWindow->EventReceiver, pxWindow, PXMouseScrollDirectionUp);
                    break;

                case MouseScrollDown:
                    PXFunctionInvoke(pxWindow->MouseScrollCallBack, pxWindow->EventReceiver, pxWindow, PXMouseScrollDirectionDown);
                    break;

            }
        }
        case MotionNotify:
        {
            printf("[Event] MotionNotify \n");

            break;
        }
        case EnterNotify:
        {
            PXFunctionInvoke(pxWindow->MouseEnterCallBack, pxWindow->EventReceiver, pxWindow);
            break;
        }
        case LeaveNotify:
        {
            PXFunctionInvoke(pxWindow->MouseLeaveCallBack, pxWindow->EventReceiver, pxWindow);
            break;
        }
        case FocusIn:
        {
            PXFunctionInvoke(pxWindow->FocusEnterCallBack, pxWindow->EventReceiver, pxWindow);
            break;
        }
        case FocusOut:
        {
            PXFunctionInvoke(pxWindow->FocusLeaveCallBack, pxWindow->EventReceiver, pxWindow);
            break;
        }
        case KeymapNotify:
        {
            printf("[Event] KeymapNotify \n");

            break;
        }
        case Expose:
        {
            printf("[Event] Expose \n");

            /*
              XWindowAttributes gwa;

                        XGetWindowAttributes(window.Dis, PXWindowID, &gwa);
                        glViewport(0, 0, gwa.width, gwa.height);


                        glClearColor(1.0, 1.0, 1.0, 1.0);
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                        glBegin(GL_POLYGON);
                        glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
                        glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
                        glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
                        glEnd();

                        // Flush drawing command buffer to make drawing happen as soon as possible.
                        //glFlush();

                        window.FrameBufferSwap();*/

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
            const XResizeRequestEvent resizeRequestEvent = event->xresizerequest;
            const int width = resizeRequestEvent.width;
            const int height = resizeRequestEvent.height;

            // glViewport(0,0, width, height);

            PXFunctionInvoke(pxWindow->WindowSizeChangedCallBack, pxWindow->EventReceiver, pxWindow);

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
            XGenericEventCookie cookie = event->xcookie; // Make Copy

            const int result = XGetEventData(pxWindow->DisplayCurrent, &cookie);
            const unsigned char sucessful = result != 0 && cookie.data;

            if(sucessful)
            {
                switch(cookie.evtype)
                {
                    case XI_RawMotion:
                    {
                        XIRawEvent* re = (XIRawEvent*)cookie.data;

                        if(re->valuators.mask_len)
                        {
                            PXMouse* mouse = &pxWindow->MouseCurrentInput;
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


                            mouse->Delta[0] = mouse->Position[0] - xpos;
                            mouse->Delta[1] = mouse->Position[1] - ypos;

                            mouse->Position[0] = xpos;
                            mouse->Position[1] = ypos;

                            printf("[Event] RawMotion %5.4lf %5.4lf\n", xpos, ypos);

                            PXFunctionInvoke(pxWindow->MouseMoveCallBack, pxWindow->EventReceiver, pxWindow, mouse);

                            //printf("[Event] RawMotion %5.4lf %5.4lf\n", window.MouseDeltaX, window.MouseDeltaY);

                            //PXFunctionInvoke(window.MouseMoveCallBack, window.MousePositionX, window.MousePositionY, window.MouseDeltaX, window.MouseDeltaY);
                        }
                    }
                }
            }
            else
            {
                printf("[Event] GenericEvent %i\n", cookie.evtype);
            }

            XFreeEventData(pxWindow->DisplayCurrent, &cookie);

            break;
        }
        default:
        {
            printf("[Event] default: unkown event \n");

            break;
        }
    }
}
#elif PXOSWindowsDestop
LRESULT CALLBACK PXWindowEventHandler(const HWND windowID, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    PXGUISystem* pxGUISystem = PXNull;

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowHandle = windowID;
    pxWindowEvent.EventID = eventID;
    pxWindowEvent.ParamW = wParam;
    pxWindowEvent.ParamL = lParam;

    if(PXGUISystemGlobalReference)
    {
        pxGUISystem = PXGUISystemGlobalReference;

        PXDictionaryFindEntry(&pxGUISystem->UIElementLookUp, &windowID, &pxWindowEvent.UIElementReference);
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
            break;
        case WM_CREATE: // Gets called inside the "CreateWindow" function.
        {
            // Do nothíng here, as it's too soon to regard the window as 'created'

            return DefWindowProc(windowID, eventID, wParam, lParam);
        }
        case WM_DESTROY:
        {
            pxWindowEvent.Type = PXWindowEventTypeDestroy;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);
  
            break;
        }
        case WM_MOVE:
        {
            pxWindowEvent.Type = PXWindowEventTypeMove;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_SIZE:
        {
            pxWindowEvent.Type = PXWindowEventTypeResize;
            pxWindowEvent.Resize.Width = LOWORD(lParam);
            pxWindowEvent.Resize.Height = HIWORD(lParam);

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }   
        case WM_CLOSE:
        {
            pxWindowEvent.Type = PXWindowEventTypeClose;
            pxWindowEvent.Close.CommitToClose = PXTrue;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

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
                case TVN_SELCHANGED:
                {
                    // NOTE:
                    // On treeview item selection, "wParam" will might always be NULL.
                    // It's a copy of the item "lParam" parameter on creation.

                    PXUIElement* pxTreeViewContainer = PXNull;
                    PXUIElement* pxTreeViewItem = PXNull;

                    if(pxGUISystem)
                    {    
                        const HWND itemHandle = TreeView_GetSelection(sourceObject); // Event does not give us the handle, fetch manually.

                        // Fetch treeview object
                        PXDictionaryFindEntry(&pxGUISystem->UIElementLookUp, &sourceObject, &pxTreeViewContainer);

                        // Fetch treeviewitem object
                        PXDictionaryFindEntry(&pxGUISystem->UIElementLookUp, &itemHandle, &pxTreeViewItem);
                    }                  

                    pxWindowEvent.UIElementReference = pxTreeViewContainer;
                    pxWindowEvent.UIElementSender = pxTreeViewItem;

                    pxWindowEvent.Type = PXWindowEventTypeSelect;
                    pxWindowEvent.Select.UIElementSelected = pxTreeViewItem;

                    PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);           

                    break;
                }
                case TCN_SELCHANGE:
                {
                    const int pageID = TabCtrl_GetCurSel(sourceObject);

#if 1
                    PXUIElement* const pxUIElement = PXNull;

                    PXDictionaryFindEntry(&pxGUISystem->UIElementLookUp, &sourceObject, &pxUIElement);

                    // Hide current page
                    const PXBool isValidIndex = pxUIElement->ListEESize >= (pageID+1);

                    BOOL success = 0;

                    for(PXSize i = 0; i < pxUIElement->ListEESize; i++)
                    {
                        PXUIElement* const pxUIElementEEE = pxUIElement->ListEEData[i];

                        PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
                        PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);
                        pxGUIElementUpdateInfo.Show = PXFalse;
                        pxGUIElementUpdateInfo.UIElement = pxUIElementEEE;
                        pxGUIElementUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXGUIElementUpdate(pxGUISystem, &pxGUIElementUpdateInfo, 1);
                    }

                    // Show new page
                    if(isValidIndex)
                    {
                        PXUIElement* const pxUIElementEEE = pxUIElement->ListEEData[pageID];

                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "GUI",
                            "Event",
                            "Tab Select <0x%p>, %15s ID:<%i>",
                            sourceObject,
                            pxUIElementEEE->NameData,
                            pageID
                        );

                        PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
                        PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);
                        pxGUIElementUpdateInfo.Show = PXTrue;
                        pxGUIElementUpdateInfo.UIElement = pxUIElementEEE;
                        pxGUIElementUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXGUIElementUpdate(pxGUISystem, &pxGUIElementUpdateInfo, 1);                     
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

        case WM_PAINT:
        {
            PXUIElement* const pxUIElement = PXNull;

            PXDictionaryFindEntry(&pxGUISystem->UIElementLookUp, &windowID, &pxUIElement);

            if(pxUIElement)
            {
                if(!pxUIElement->IsEnabled)
                {
                  //  return 0;
                }              
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

        case WM_PAINT:
        {
            break;
        }
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
        case WM_DEVMODECHANGE: return WindowEventDeviceModeChange;
        case WM_ACTIVATEAPP: return WindowEventActivateApp;
        case WM_FONTCHANGE: return WindowEventFontChange;
        case WM_TIMECHANGE: return WindowEventTimeChange;
        case WM_CANCELMODE: return WindowEventCancelMode;
        case WM_MOUSEACTIVATE: return WindowEventMouseActivate;
        case WM_CHILDACTIVATE: return WindowEventChildActivate;
        case WM_QUEUESYNC: return WindowEventQueueSync;
        case WM_GETMINMAXINFO: return WindowEventSizeChange;
        case WM_PAINTICON: return WindowEventIconPaint;
        case WM_ICONERASEBKGND: return WindowEventIconBackgroundErase;
        case WM_NEXTDLGCTL: return WindowEventDialogControlNext;
        case WM_SPOOLERSTATUS: return WindowEventSPOOLERSTATUS;
        case WM_DRAWITEM: return WindowEventItemDraw;
        case WM_MEASUREITEM: return WindowEventItemMeasure;
        case WM_DELETEITEM: return WindowEventItemDelete;
        case WM_VKEYTOITEM: return WindowEventVKEYTOITEM;
        case WM_CHARTOITEM: return WindowEventCHARTOITEM;
        case WM_SETFONT: return WindowEventFontSet;
        case WM_GETFONT: return WindowEventFontGet;
        case WM_SETHOTKEY: return WindowEventSETHOTKEY;
        case WM_GETHOTKEY: return WindowEventGETHOTKEY;
        case WM_QUERYDRAGICON: return WindowEventQUERYDRAGICON;
        case WM_COMPAREITEM: return WindowEventCOMPAREITEM;
        case WM_GETOBJECT: return WindowEventGETOBJECT;
        case WM_COMPACTING: return WindowEventCOMPACTING;
        case WM_COMMNOTIFY: return WindowEventCOMMNOTIFY;   /* no longer suported */
        case WM_WINDOWPOSCHANGING: return WindowEventWINDOWPOSCHANGING;
        case WM_WINDOWPOSCHANGED: return WindowEventWINDOWPOSCHANGED;
        case WM_POWER: return WindowEventPOWER;
        case WM_COPYDATA: return WindowEventCOPYDATA;
        case WM_CANCELJOURNAL: return WindowEventCANCELJOURNAL;

        case WM_INPUTLANGCHANGEREQUEST: return WindowEventINPUTLANGCHANGEREQUEST;
        case WM_INPUTLANGCHANGE: return WindowEventINPUTLANGCHANGE;
        case WM_TCARD: return WindowEventTCARD;
        case WM_HELP: return WindowEventHELP;
        case WM_USERCHANGED: return WindowEventUSERCHANGED;
        case WM_NOTIFYFORMAT: return WindowEventNOTIFYFORMAT;
        case WM_CONTEXTMENU: return WindowEventCONTEXTMENU;
        case WM_STYLECHANGING: return WindowEventSTYLECHANGING;
        case WM_STYLECHANGED: return WindowEventSTYLECHANGED;
        case WM_DISPLAYCHANGE: return WindowEventDISPLAYCHANGE;
        case WM_GETICON: return WindowEventGETICON;
        case WM_SETICON: return WindowEventSETICON;
        case WM_NCCREATE: return WindowEventNCCREATE;
        case WM_NCDESTROY: return WindowEventNCDESTROY;
        case WM_NCCALCSIZE: return WindowEventNCCALCSIZE;
        case WM_NCHITTEST: return WindowEventNCHITTEST;
        case WM_NCPAINT: return WindowEventNCPAINT;
        case WM_NCACTIVATE: return WindowEventNCACTIVATE;
        case WM_GETDLGCODE: return WindowEventGETDLGCODE;
        case WM_SYNCPAINT: return WindowEventSYNCPAINT;
        case WM_NCMOUSEMOVE: return WindowEventNCMOUSEMOVE;
        case WM_NCLBUTTONDOWN: return WindowEventNCLBUTTONDOWN;
        case WM_NCLBUTTONUP: return WindowEventNCLBUTTONUP;
        case WM_NCLBUTTONDBLCLK: return WindowEventNCLBUTTONDBLCLK;
        case WM_NCRBUTTONDOWN: return WindowEventNCRBUTTONDOWN;
        case WM_NCRBUTTONUP: return WindowEventNCRBUTTONUP;
        case WM_NCRBUTTONDBLCLK: return WindowEventNCRBUTTONDBLCLK;
        case WM_NCMBUTTONDOWN: return WindowEventNCMBUTTONDOWN;
        case WM_NCMBUTTONUP: return WindowEventNCMBUTTONUP;
        case WM_NCMBUTTONDBLCLK: return WindowEventNCMBUTTONDBLCLK;
        case WM_NCXBUTTONDOWN: return WindowEventNCXBUTTONDOWN;
        case WM_NCXBUTTONUP: return WindowEventNCXBUTTONUP;
        case WM_NCXBUTTONDBLCLK: return WindowEventNCXBUTTONDBLCLK;
        case WM_INPUT_DEVICE_CHANGE: return WindowEventINPUT_DEVICE_CHANGE;
            // ?? case WM_KEYFIRST: return WindowEventKEYFIRST;

        case WM_CHAR: return WindowEventCHAR;
        case WM_DEADCHAR: return WindowEventDEADCHAR;
        case WM_SYSKEYDOWN: return WindowEventSYSKEYDOWN;
        case WM_SYSKEYUP: return WindowEventSYSKEYUP;
        case WM_SYSCHAR: return WindowEventSYSCHAR;
        case WM_SYSDEADCHAR: return WindowEventSYSDEADCHAR;
            // case WM_UNICHAR: return WindowEventUNICHAR;
                 //case WM_KEYLAST: return WindowEventKEYLAST;
                    // case UNICODE_NOCHAR: return WindowEventXXXXXXXXXXXXXXX;

        case WM_IME_STARTCOMPOSITION: return WindowEventIME_STARTCOMPOSITION;
        case WM_IME_ENDCOMPOSITION: return WindowEventIME_ENDCOMPOSITION;
        case WM_IME_COMPOSITION: return WindowEventIME_COMPOSITION;
            //case WM_IME_KEYLAST: return WindowEventIME_KEYLAST;
        case WM_INITDIALOG: return WindowEventINITDIALOG;

        case WM_SYSCOMMAND: return WindowEventSYSCOMMAND;
        case WM_TIMER: return WindowEventTIMER;
        case WM_HSCROLL: return WindowEventHSCROLL;
        case WM_VSCROLL: return WindowEventVSCROLL;
        case WM_INITMENU: return WindowEventINITMENU;
        case WM_INITMENUPOPUP: return WindowEventINITMENUPOPUP;
            //  case WM_GESTURE: return WindowEventGESTURE;
           //   case WM_GESTURENOTIFY: return WindowEventGESTURENOTIFY;
        case WM_MENUSELECT: return WindowEventMENUSELECT;
        case WM_MENUCHAR: return WindowEventMENUCHAR;
        case WM_ENTERIDLE: return WindowEventENTERIDLE;
        case WM_MENURBUTTONUP: return WindowEventMENURBUTTONUP;
        case WM_MENUDRAG: return WindowEventMENUDRAG;
        case WM_MENUGETOBJECT: return WindowEventMENUGETOBJECT;
        case WM_UNINITMENUPOPUP: return WindowEventUNINITMENUPOPUP;
        case WM_MENUCOMMAND: return WindowEventMENUCOMMAND;
        case WM_CHANGEUISTATE: return WindowEventCHANGEUISTATE;
        case WM_UPDATEUISTATE: return WindowEventUPDATEUISTATE;
        case WM_QUERYUISTATE: return WindowEventQUERYUISTATE;
        case WM_CTLCOLORMSGBOX: return WindowEventCTLCOLORMSGBOX;
        case WM_CTLCOLOREDIT: return WindowEventCTLCOLOREDIT;
        case WM_CTLCOLORLISTBOX: return WindowEventCTLCOLORLISTBOX;
        case WM_CTLCOLORBTN: return WindowEventCTLCOLORBTN;
        case WM_CTLCOLORDLG: return WindowEventCTLCOLORDLG;
        case WM_CTLCOLORSCROLLBAR: return WindowEventCTLCOLORSCROLLBAR;

        case WM_CTLCOLORSTATIC: return WindowEventCTLCOLORSTATIC;
            //  case MN_GETHMENU: return WindowEventGETHMENU;
                  //case WM_MOUSEFIRST: return WindowEventMOUSEFIRST;
        case WM_MOUSEMOVE: return WindowEventMOUSEMOVE;

#endif

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

#if 1
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
          //  PXUIElement* pxUIElement = PXNull;

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
                "Event",
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
                    const PXBool sucessful = result == 0;

                    if(sucessful)
                    {
#if UseRawMouseData
                        if(rawInput.header.dwType == RIM_TYPEMOUSE)
                        {
                            pxWindowEvent.Type = PXWindowEventTypeMove;
                            pxWindowEvent.InputMouseMove.AxisX = 0;
                            pxWindowEvent.InputMouseMove.AxisY = 0;
                            pxWindowEvent.InputMouseMove.DeltaX = rawInput.data.mouse.lLastX;
                            pxWindowEvent.InputMouseMove.DeltaY = rawInput.data.mouse.lLastY; 
   
                            PXWindowCursorPositionInWindowGet(windowID, &pxWindowEvent.InputMouseMove.AxisX, &pxWindowEvent.InputMouseMove.AxisY);
                                                  
                            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);


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
            PXInt32U buttonType = wParam; // IDOK;
            HANDLE handle = (HANDLE)lParam;

            PXUIElement* const pxUIElement = PXNull;

            if(pxGUISystem)
            {
                PXDictionaryFindEntry(&pxGUISystem->UIElementLookUp, &handle, &pxUIElement);
            }

            if(!pxUIElement->IsEnabled)
            {
               // ShowWindow(pxUIElement->ID, SW_HIDE);
            }

            pxWindowEvent.Type = PXWindowEventTypeClick;
            pxWindowEvent.UIElementSender = pxUIElement;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        } 
        case WM_LBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_LBUTTONUP: 
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_LBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_RBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_RBUTTONUP: 
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_RBUTTONDBLCLK: 
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_MBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_MBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case WM_MBUTTONDBLCLK: 
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

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

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);


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
            //buttonInfo.ReservedDontUse = (characterInfo & 0b00011110000000000000000000000000) >> 25; //	Reserviert; nicht verwenden.
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
        case WM_DEVICECHANGE: return WindowEventDEVICECHANGE;
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
        case WM_MOUSEHOVER: return WindowEventMOUSEHOVER;
        case WM_MOUSELEAVE: return WindowEventMOUSELEAVE;
        case WM_NCMOUSEHOVER: return WindowEventNCMOUSEHOVER;
        case WM_NCMOUSELEAVE: return WindowEventNCMOUSELEAVE;
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

void PXAPI PXGUIElementChildListEnumerate(PXGUISystem* const pxGUISystem, PXUIElement* const parent, PXBool visible)
{
    for(size_t i = 0; i < pxGUISystem->UIElementLookUp.EntryAmountCurrent; i++)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXUIElement* childElement = PXNull;

        PXDictionaryIndex(&pxGUISystem->UIElementLookUp, i, &pxDictionaryEntry);

        childElement = *(PXUIElement**)pxDictionaryEntry.Value;

        if(childElement->Parent)
        {
            if(childElement->Parent->ID == parent->ID)
            {
                // found child.

                PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
                PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);

                pxGUIElementUpdateInfo.UIElement = childElement;
                pxGUIElementUpdateInfo.Show = visible;
                pxGUIElementUpdateInfo.Property = PXUIElementPropertyVisibility;

                PXGUIElementUpdate(pxGUISystem, &pxGUIElementUpdateInfo, 1);
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

PXThreadResult PXOSAPI PXWindowMessageLoop(PXUIElement* const pxUIElement)
{
#if 0
    while(pxUIElement->IsRunning)
    {
        PXWindowUpdate(pxWindow);
        PXThreadYieldToOtherThreads();
    }
#endif

    return PXActionSuccessful;
}


PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem)
{
    if(PXGUISystemGlobalReference)
    {
        return PXActionRefuedObjectAlreadyInizialized;
    }

    PXGUISystemGlobalReference = pxGUISystem;


    INITCOMMONCONTROLSEX initCommonControls;
    initCommonControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    initCommonControls.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&initCommonControls);

#if OSUnix

    // Make this thread safe
    {
        const int result = XInitThreads();
    }


    // XCloseDisplay();

#elif OSWindows



#endif


    PXDictionaryConstruct(&pxGUISystem->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);




    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem)
{

    PXDictionaryDestruct(&pxGUISystem->UIElementLookUp);

    PXGUISystemGlobalReference = PXNull;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIElementCreate(PXGUISystem* const pxGUISystem, PXGUIElementCreateInfo* const pxGUIElementCreateInfo, const PXSize amount)
{
    if(!(pxGUISystem && pxGUIElementCreateInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    //assert();

    PXUIElement* pxUIElement = *pxGUIElementCreateInfo->UIElement;

    if(!pxUIElement)
    {
        PXNewZerod(PXUIElement, &pxUIElement);
        *pxGUIElementCreateInfo->UIElement = pxUIElement;
    }

    pxUIElement = *pxGUIElementCreateInfo->UIElement;
    pxUIElement->Type = pxGUIElementCreateInfo->Type;
    pxUIElement->InteractCallBack = pxGUIElementCreateInfo->InteractCallBack;
    pxUIElement->InteractOwner = pxGUIElementCreateInfo->InteractOwner;
    pxUIElement->Parent = pxGUIElementCreateInfo->UIElementParent;
    pxUIElement->IsEnabled = PXTrue;

    PXCopy(PXUIElementPosition, &pxGUIElementCreateInfo->Position, &pxUIElement->Position);



  

#if 1
    const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

    //const char* format = PXEngineCreateTypeToString(pxEngineResourceCreateInfo->CreateType);
    if(PXUIElementTypeTreeViewItem == pxGUIElementCreateInfo->Type)
    {
      // PXUIElement* const uiElementSource



        switch(pxGUIElementCreateInfo->Data.TreeViewItem.OwningObjectType)
        {
            case PXFileResourceTypeEmpty:
            {
                pxUIElement->NameSize = PXTextPrintA(pxUIElement->NameData, 128, "<%s>", pxGUIElementCreateInfo->Name);
                break;
            }
            case PXFileResourceTypeUI:
            {
                PXUIElement* const uiElementSource = (PXUIElement*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                const char* uiElementTypeName = PXUIElementTypeToString(uiElementSource->Type);
                const char* name = uiElementSource->NameData;

                if(name[0] == '\0')
                {
                    name = "**Unnamed**";
                }

                pxUIElement->NameSize = PXTextPrintA(pxUIElement->NameData, 128, "[%s] %s", uiElementTypeName, name);

                break;
            }
            case PXFileResourceTypeModel:
            {
                PXModel* const pxModel = (PXModel*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                pxUIElement->NameSize = PXTextPrintA
                (
                    pxUIElement->NameData,
                    128,
                    "[Model] %s ID:%i",              
                    pxModel->ResourceID.Name,
                    pxModel->ResourceID.PXID
                );

                break;
            }
            case PXFileResourceTypeRenderShader:
            {
                PXShaderProgram* const pxShaderProgram = (PXShaderProgram*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                pxUIElement->NameSize = PXTextPrintA
                (
                    pxUIElement->NameData,
                    128,
                    "[Shader] %s ID:%i",
                    pxShaderProgram->ResourceID.Name,
                    pxShaderProgram->ResourceID.PXID
                );

                break;
            }
            case PXFileResourceTypeImage:
            {
                PXImage* const pxImage = (PXImage*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                pxUIElement->NameSize = PXTextPrintA
                (
                    pxUIElement->NameData,
                    128, 
                    "[Image] %ix%i",
                    pxImage->Width, 
                    pxImage->Height
                );

                break;
            }
            default:
            { 
                pxUIElement->NameSize = PXTextPrintA(pxUIElement->NameData, 128, "ERROR");

                break;
            }
        }      
    }
    else
    {
        const char* name = pxGUIElementCreateInfo->Name;

        if(!name)
        {
            name = "**Unnamed**";
        }

        pxUIElement->NameSize = PXTextPrintA(pxUIElement->NameData, 128, "%s", name);
    }
#endif


    // Resize
    
        PXGUIElementUpdateInfo sizeInfoAA;
        PXClear(PXGUIElementUpdateInfo, &sizeInfoAA);
        sizeInfoAA.UIElement = *pxGUIElementCreateInfo->UIElement;
        sizeInfoAA.WindowReference = pxGUIElementCreateInfo->UIElementWindow;
        sizeInfoAA.Property = PXUIElementPropertySizeParent;

        PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
        PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

       // PXWindowSizeInfo pxWindowSizeInfo;

       // PXWindowID windowID = pxGUIElementCreateInfo->UIElementWindow ? pxGUIElementCreateInfo->UIElementWindow->ID : PXNull;

      //  PXWindowSizeGet(windowID, &pxWindowSizeInfo);

        PXGUIElementFetch(pxGUISystem, &sizeInfoAA, 1);

        pxUIElementPositionCalulcateInfo.WindowWidth = sizeInfoAA.Data.Size.Width;
        pxUIElementPositionCalulcateInfo.WindowHeight = sizeInfoAA.Data.Size.Height;

        PXUIElementPositionCalculcate(pxUIElement, &pxUIElementPositionCalulcateInfo);
    

#if OSUnix


    if(pxGUISystem->AreOSUIElementsDefined) // 
    {
        // Try to create the correct UI Element with the library "GTK"
        // 
        // TODO: ...
    }
    else
    {
        // UI Elements not defined, we need it ourself!

        switch(pxUIElement->Type)
        {
            case PXUIElementTypeWindow:
            {
                PXGUIElementCreateWindowInfo* const pxGUIElementCreateWindowInfo = &pxGUIElementCreateInfo->Window;

                Display* display = PXNull;

                // Create display
                {
                    display = XOpenDisplay(PXNull);   // X11/Xlib.h Create Window
                    const PXBool successful = PXNull != display;

                    if(!successful)
                    {
                        return PXActionFailedInitialization; // printf("\n\tcannot connect to X server\n\n");
                    }
                
                    pxGUIElementCreateWindowInfo->WindowDisplay = display;

                    const XID windowRoot = DefaultRootWindow(display); // Make windows root
                    const PXBool successful = windowRoot != 0;

                    pxGUIElementCreateWindowInfo->WindowHandle = windowRoot;
          
                }

                const int attributeList[] =
                {
                    GLX_RGBA,
                    GLX_DEPTH_SIZE,
                    24,
                    GLX_DOUBLEBUFFER,
                    None
                };

                const XVisualInfo* const visualInfo = glXChooseVisual(display, 0, attributeList);

                {
                    const PXBool successful = visualInfo != 0;

                    if(!successful)
                    {
                        return PXActionSuccessful; // no appropriate visual found
                    }
                }

                // Create colormapping
                Colormap colormap = XCreateColormap
                (
                    pxGUIElementCreateWindowInfo->WindowDisplay,
                    pxGUIElementCreateWindowInfo->WindowHandle,
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



                // Create window
                {
                    int borderWidth = 0;

                    const XID windowhandle = XCreateWindow
                    (
                        pxGUIElementCreateWindowInfo->WindowDisplay,
                        pxGUIElementCreateWindowInfo->WindowRootHandle,
                        pxGUIElementCreateWindowInfo->X,
                        pxGUIElementCreateWindowInfo->Y,
                        pxGUIElementCreateWindowInfo->Width,
                        pxGUIElementCreateWindowInfo->Height,
                        borderWidth,
                        visualInfo->depth,
                        InputOutput,
                        visualInfo->visual,
                        CWColormap | CWEventMask,
                        &setWindowAttributes
                    );
                    const PXBool sucessful = PXNull != windowhandle;

                    pxGUIElementCreateWindowInfo->WindowHandle = windowhandle;
                }


                // Attach to render engine
                XMapWindow(pxGUIElementCreateWindowInfo->WindowDisplay, pxGUIElementCreateWindowInfo->WindowHandle);


                break;
            }
            default:
                break;
        }
    }





  

    

    // Set Title
    {
  

        XStoreName(pxGUIElementCreateWindowInfo->DisplayCurrent, pxWindow->ID, pxWindow->Title.TextA);
    }

#if 0 // Grab means literally Drag%Drop grab. This is not mouse motion
    //bool   ret    = false;
    XID cursor = XCreateFontCursor(display, XC_crosshair);
    int    root = DefaultRootWindow(display);

    const int grabResult = XGrabPointer
    (
        display,
        root,
        0,
        ButtonMotionMask,
        GrabModeAsync,
        GrabModeAsync,
        root,
        None,
        CurrentTime
    );
#endif


    // Raw mouse movement
    XIEventMask eventmask;
    const PXSize maskLength = (XI_LASTEVENT + 7) / 8;
    unsigned char mask[maskLength];

    PXClearList(char, mask, maskLength);
    PXClear(XIEventMask, &eventmask);

    XISetMask(mask, XI_RawMotion);
    //XISetMask(mask, XI_RawButtonPress);
    //XISetMask(mask, XI_RawKeyPress);

    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = maskLength;
    eventmask.mask = mask;


    XISelectEvents(pxWindow->DisplayCurrent, pxWindow->WindowRoot, &eventmask, 1u);
    XFlush(pxWindow->DisplayCurrent);










  
  


#elif OSWindows

  //  pxUIElementCreateData->CreationSkip = PXFalse;


    if(pxGUIElementCreateInfo->StyleFlagList & PXGUIElementStyleFlagVisible)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_VISIBLE;
    }

    if(pxGUIElementCreateInfo->StyleFlagList & PXGUIElementStyleFlagBorder)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_BORDER;
    }

    if(pxGUIElementCreateInfo->UIElementParent)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_CHILD;
    }

    if(pxGUIElementCreateInfo->BehaviourFlagList & PXGUIElementBehaviourCanBeTabbed)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_TABSTOP;
    }


    switch(pxUIElement->Type)
    {
        case PXUIElementTypePanel:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;

            pxGUIElementCreateInfo->WindowsStyleFlags |= SS_BLACKFRAME;

            break;
        }
        case PXUIElementTypeText:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;

            PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementCreateInfo->Data.Text;

            pxGUIElementCreateInfo->WindowsTextContent = pxUIElementTextInfo->Content;

            switch(pxUIElementTextInfo->Allign)
            {
                case PXUIElementTextAllignLeft:
                    pxGUIElementCreateInfo->WindowsStyleFlags |= SS_LEFT;
                    break;

                case PXUIElementTextAllignRight:
                    pxGUIElementCreateInfo->WindowsStyleFlags |= SS_RIGHT;
                    break;

                default:
                case PXUIElementTextAllignCenter:
                    pxGUIElementCreateInfo->WindowsStyleFlags |= SS_CENTER;
                    break;
            }

            PXBool hasParenet = 0;// pxGUIElementCreateInfo->UIElementParent;

            if(hasParenet)
            {
                pxGUIElementCreateInfo->AvoidCreation = PXUIElementTypeButton == pxUIElement->Parent->Type;

                if(pxGUIElementCreateInfo->AvoidCreation)
                {
                    pxUIElement->Type = PXUIElementTypeButtonText;
                    pxUIElement->ID = 0;
                    //return;
                }
            }

          //  return;

            break;
        }
        case PXUIElementTypeButton:
        {// BS_USERBUTTON 
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            // BS_DEFPUSHBUTTON 


            pxGUIElementCreateInfo->WindowsTextContent = pxGUIElementCreateInfo->Data.Button.TextInfo.Content;

       


            break;
        }
        case PXUIElementTypeImage:
        {
            // pxUIElementCreateData->WindowsClassName = WC_IMAGE;
             //pxUIElementCreateData->WindowsStyleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeDropDown:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_COMBOBOX;
            pxGUIElementCreateInfo->WindowsStyleFlags |= CBS_HASSTRINGS | CBS_DROPDOWNLIST | WS_OVERLAPPED;
            break;
        }
        case PXUIElementTypeListBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_LISTBOX;
            break;
        }
        case PXUIElementTypeTextEdit:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_EDIT;
            pxGUIElementCreateInfo->WindowsStyleFlags |= ES_MULTILINE;

            pxGUIElementCreateInfo->WindowsTextContent = pxGUIElementCreateInfo->Data.Button.TextInfo.Content;

            break;
        }
        case PXUIElementTypeRichEdit:
        {
            pxGUIElementCreateInfo->WindowsClassName = "RICHEDIT_CLASS";
            break;
        }
        case PXUIElementTypeScrollBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_SCROLLBAR;
            break;
        }
        case PXUIElementTypeTrackBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = TRACKBAR_CLASS;
            pxGUIElementCreateInfo->WindowsStyleFlags |= TBS_AUTOTICKS | TBS_TOOLTIPS;
            break;
        }
        case PXUIElementTypeStatusBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = STATUSCLASSNAMEA;
            break;
        }
        case PXUIElementTypeUpDown:
        {
            pxGUIElementCreateInfo->WindowsClassName = UPDOWN_CLASS;
            break;
        }
        case PXUIElementTypeProgressBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = PROGRESS_CLASS;
            break;
        }
        case PXUIElementTypeHotKey:
        {
            pxGUIElementCreateInfo->WindowsClassName = HOTKEY_CLASS;
            break;
        }
        case PXUIElementTypeCalender:
        {
            pxGUIElementCreateInfo->WindowsClassName = MONTHCAL_CLASS;
            break;
        }
        case PXUIElementTypeToolTip:
        {
            pxGUIElementCreateInfo->WindowsClassName = TOOLTIPS_CLASS;
            break;
        }
        case PXUIElementTypeAnimate:
        {
            pxGUIElementCreateInfo->WindowsClassName = ANIMATE_CLASS;
            break;
        }
        case PXUIElementTypeDatePicker:
        {
            pxGUIElementCreateInfo->WindowsClassName = DATETIMEPICK_CLASS;
            break;
        }
        case PXUIElementTypeGroupBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_GROUPBOX;
            break;
        }
        case PXUIElementTypeRadioButton:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_RADIOBUTTON;
            break;
        }
        case PXUIElementTypeGroupRadioButton:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_AUTORADIOBUTTON;
            break;
        }
        case PXUIElementTypeTreeView:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_TREEVIEW;
            pxGUIElementCreateInfo->WindowsStyleFlags |=
                TVIF_TEXT |
                TVIF_IMAGE |
                TVIF_SELECTEDIMAGE |
                TVIF_PARAM | // Required to get the a selected item, otherwise its just NULL.
                TVS_HASBUTTONS |
                TVS_HASLINES |
                TVS_LINESATROOT;
            break;
        }
        case PXUIElementTypeIPInput:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_IPADDRESS;
            break;
        }
        case PXUIElementTypeLink:
        {
            pxGUIElementCreateInfo->WindowsClassName = "SysLink"; // WC_LINK
            break;
        }
        case PXUIElementTypeHeader:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_HEADER;
            break;
        }
        case PXUIElementTypeFontSelector:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_NATIVEFONTCTL;
            // NFS_USEFONTASSOC
            break;
        }
        case PXUIElementTypePageScroll:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_PAGESCROLLER;
            break;
        }
        case PXUIElementTypeTabControll:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_TABCONTROL;
            pxGUIElementCreateInfo->WindowsStyleFlags |= WS_CLIPSIBLINGS | TCS_BUTTONS;
            break;
        }
        case PXUIElementTypeToggle:
        {
            pxGUIElementCreateInfo->WindowsClassName = 00000000000000000000;
            pxGUIElementCreateInfo->WindowsStyleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeColorPicker:
        {
            pxGUIElementCreateInfo->WindowsClassName = 00000000000000000000;
            pxGUIElementCreateInfo->WindowsStyleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeSlider:
        {
            pxGUIElementCreateInfo->WindowsClassName = 00000000000000000000;
            pxGUIElementCreateInfo->WindowsStyleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeCheckBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_CHECKBOX;

            //CheckDlgButton(uiCheckBox->ID, 1, BST_CHECKED); // BST_UNCHECKED

            break;
        }
        case PXUIElementTypeComboBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_COMBOBOXEX;
            break;
        }
        case PXUIElementTypeRenderFrame:
        {
            // HBRUSH hbrBackground = CreateSolidBrush(RGB(38, 38, 38));
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->WindowsStyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
            break;
        }
        case PXUIElementTypeWindow:
        {
            PXGUIElementCreateWindowInfo* const windowInfo = &pxGUIElementCreateInfo->Data.Window;

            pxGUIElementCreateInfo->WindowsWindowsStyleFlagsExtended = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME | WS_EX_CONTEXTHELP;
            pxGUIElementCreateInfo->WindowsClassName = "PXWindow";
            pxGUIElementCreateInfo->WindowsStyleFlags |=                
                WS_OVERLAPPEDWINDOW |
                CS_OWNDC | 
                CS_HREDRAW |
                CS_VREDRAW;


            PXWindowID windowID = 0;
  
            DWORD dwStyle = 0;
            HWND hWndParent = 0;
            HMENU hMenu = 0;
            void* lpParam = 0;

           // HICON       hIcon = 
           // HCURSOR     hCursor = pxWindow->CursorID;
            // HBRUSH      hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); //(HBRUSH)GetStockObject(COLOR_BACKGROUND);

          //  HBRUSH hbrBackground = 

            /*
            // Cursor setup
            {
               // const HCURSOR cursorID = LoadCursor(hInstance, IDC_ARROW);
                //pxWindow->CursorID = cursorID;

                if(pxWindow->Mode == PXWindowModeNormal)
                {
                    const int xx = WS_OVERLAPPEDWINDOW;

                    dwStyle |= WS_OVERLAPPEDWINDOW;// | WS_VISIBLE;
                }
            }
            */

            // Registering of class
            {
                WNDCLASSA wndclass;

                PXClear(WNDCLASSA, &wndclass);

                wndclass.style = 0; // Can't apply
                wndclass.lpfnWndProc = PXWindowEventHandler;
                wndclass.cbClsExtra = 0;
                wndclass.cbWndExtra = 0;
                wndclass.hInstance = GetModuleHandleA(NULL);
                wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
                wndclass.hCursor = PXNull;
                wndclass.hbrBackground = CreateSolidBrush(RGB(windowInfo->BackGroundColor.Red, windowInfo->BackGroundColor.Green, windowInfo->BackGroundColor.Blue));
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = "PXWindow";

                
                const ATOM classID = RegisterClassA(&wndclass);
                const PXBool success = 0 != classID;

                if(!success)
                {
                    const DWORD errorID = GetLastError();
                    const PXActionResult result = PXWindowsHandleErrorFromID(errorID);
                }     

                pxGUIElementCreateInfo->WindowsClassName = (char*)classID;
            }

            // Calc size
            {
                const PXBool isDefaultSize = 1;// windowInfo->Width == 0 && windowInfo->Height == 0;

                if(isDefaultSize)
                {
                    PXInt32S screenWidth = 0;
                    PXInt32S screenHeight = 0;

                    PXMonitorGetSize(&screenWidth, &screenHeight);

                    pxUIElementPositionCalulcateInfo.X = screenWidth * 0.125f;
                    pxUIElementPositionCalulcateInfo.Y = screenHeight * 0.125f;
                    pxUIElementPositionCalulcateInfo.Width = screenWidth * 0.75f;
                    pxUIElementPositionCalulcateInfo.Height = screenHeight * 0.75f;
                }
            }

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            pxGUIElementCreateInfo->AvoidCreation = PXTrue;
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }



    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
     // pxUIElement->Type == PXUIElementTypeText&&;
    

    if(!pxGUIElementCreateInfo->AvoidCreation)
    {
        HINSTANCE hInstance = PXNull;
        PXWindowID windowID = pxGUIElementCreateInfo->UIElementWindow ? pxGUIElementCreateInfo->UIElementWindow->ID : PXNull;

        if(pxGUIElementCreateInfo->UIElementWindow)
        {
            hInstance = (HINSTANCE)GetWindowLongPtr(pxGUIElementCreateInfo->UIElementWindow->ID, GWLP_HINSTANCE); 
            //windowID = pxGUIElementCreateInfo->UIElementWindow->ID;
        }
        else
        {
            hInstance = GetModuleHandle(NULL);
        }

       // const HINSTANCE hInstance = GetModuleHandle(NULL);

        pxUIElement->ID = CreateWindowExA // Windows 2000, User32.dll, winuser.h
        (
            pxGUIElementCreateInfo->WindowsWindowsStyleFlagsExtended,
            pxGUIElementCreateInfo->WindowsClassName,
            pxGUIElementCreateInfo->WindowsTextContent, // Text content
            pxGUIElementCreateInfo->WindowsStyleFlags,
            pxUIElementPositionCalulcateInfo.X,
            pxUIElementPositionCalulcateInfo.Y,
            pxUIElementPositionCalulcateInfo.Width,
            pxUIElementPositionCalulcateInfo.Height,
            windowID,
            PXNull,  // No menu.
            hInstance,
            NULL // Pointer not needed.
        );
        const PXBool success = PXNull != pxUIElement->ID;


        if(!success)
        {
            const PXActionResult pxActionResult = PXErrorCurrent();

#if PXLogEnable
            const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

            PXLogPrint
            (
                PXLoggingError,
                "GUI",
                "Element-Create",
                "Failed: X:%4i, Y:%4i, W:%4i, H:%4i, (%s)",
                (int)pxUIElementPositionCalulcateInfo.X,
                (int)pxUIElementPositionCalulcateInfo.Y,
                (int)pxUIElementPositionCalulcateInfo.Width,
                (int)pxUIElementPositionCalulcateInfo.Height,
                uielementName
            );
#endif

            return pxActionResult;
        }

        PXDictionaryAdd(&pxGUISystem->UIElementLookUp, &pxUIElement->ID, pxUIElement);

#if PXLogEnable
        const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

        PXLogPrint
        (
            PXLoggingInfo,
            "GUI",
            "Element-Create",
            "X:%4i Y:%4i W:%4i H:%4i 0x%p %s",          
            (int)pxUIElementPositionCalulcateInfo.X,
            (int)pxUIElementPositionCalulcateInfo.Y,
            (int)pxUIElementPositionCalulcateInfo.Width,
            (int)pxUIElementPositionCalulcateInfo.Height,         
            pxUIElement->ID,
            uielementName
        );
#endif


#if 0
        if(pxUIElement->ID && pxGUIElementCreateInfo->UIElementParent)
        {
            PXUIElement* parent = pxGUIElementCreateInfo->UIElementParent;

            HDWP resA = BeginDeferWindowPos(1);

            if(PXUIElementTypeButtonText == pxUIElement->Type)
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    parent->ID,
                    pxUIElement->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
                EndDeferWindowPos(resB);
            }
            else
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    pxUIElement->ID,
                    parent->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
                EndDeferWindowPos(resB);
            }

         
        }
#endif



    }




#if 0

    char buffer[255];
    PXClearList(char, buffer, 255);

    PXSize* size = buffer;
    *size = 255;

    COLORREF colorAA = RGB(255, 0, 0);

    HDC xx = GetDC(pxUIElement->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORSTATIC, xx, pxUIElement->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));


    Pager_SetBkColor(pxUIElement->ID, &colorAA);
#endif


    // SetTextColor(xx, colorAA);


     //LRESULT ww = SendMessageA(pxUIElement->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR


   //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);


    // SendMessageA(pxUIElement->ID, RB_SETTEXTCOLOR, 0, &colorAA);




#endif






   





    //-------------------------------------------------------------------------
    // POST-Update
    //-------------------------------------------------------------------------
    switch(pxUIElement->Type)
    {
        case PXUIElementTypePanel:
        {


            break;
        }
        case PXUIElementTypeText:
        {
            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;

            pxUIElementUpdateInfo[1].UIElement = pxUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyTextAllign;

           // PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeButtonText: // Local override for OSStyle buttons
        {
            PXUIElement* pxButton = pxUIElement->Parent;

            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxButton;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;
            pxUIElementUpdateInfo[0].Data.Text.Content = pxGUIElementCreateInfo->Data.Text.Content;

           // PXCopy(PXUIElementTextInfo, &pxUIElement->TextInfo, &pxButton->TextInfo);

            PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 1);

            break;
        }
        case PXUIElementTypeButton:
        {
            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElement = pxUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;


         //  PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            PXUIElementTreeViewItemInfo* const pxUIElementTreeViewItemInfo = &pxGUIElementCreateInfo->Data.TreeViewItem;
            // Create ui item for a tree view

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

           // item.item.iImage = 1;


            const char text[] = "[N/A]";

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                item.hParent = pxUIElementTreeViewItemInfo->ItemParent->ID;
            }

            if(pxUIElementTreeViewItemInfo->TextDataOverride)
            {
                item.item.pszText = pxUIElementTreeViewItemInfo->TextDataOverride;
                item.item.cchTextMax = pxUIElementTreeViewItemInfo->TextSizeOverride;
            }
            else
            {
                item.item.pszText = pxUIElement->NameData;
                item.item.cchTextMax = pxUIElement->NameSize;
            }



            if(pxUIElementTreeViewItemInfo->OwningObjectType == PXFileResourceTypeUI)
            {
                PXUIElement* const uiElement = (PXUIElement*)pxUIElementTreeViewItemInfo->OwningObject;

                switch(uiElement->Type)
                {
                    // case PXUIElementTypeRenderFrame:


                    case PXUIElementTypeWindow:                item.item.iImage = 0;                    break;
                    case PXUIElementTypeButton:                item.item.iImage = 1;                    break;
                    case PXUIElementTypeTextEdit:              item.item.iImage = 2;                    break;
                    case PXUIElementTypeRenderFrame:           item.item.iImage = 3;                    break;
                    case PXUIElementTypeText:                  item.item.iImage = 5;                    break;
                    case PXUIElementTypeTreeView:              item.item.iImage = 4;                    break;
                    case PXUIElementTypePanel:                 item.item.iImage = 6;                    break;

                        // case PXUIElementTypeWindow:                    item.item.iImage = 1;                    break;
                        // case PXUIElementTypeWindow:                    item.item.iImage = 1;                    break;



                    default:
                        item.item.iImage = -1;
                        break;
                }
            }         

            item.item.iSelectedImage = item.item.iImage;

            switch(pxUIElementTreeViewItemInfo->InsertMode)
            {
                case PXUIElementTreeViewItemInsertModeROOT:
                    item.hInsertAfter = TVI_ROOT;
                    break;

                case PXUIElementTreeViewItemInsertModeFIRST:
                    item.hInsertAfter = TVI_FIRST;
                    break;

                case PXUIElementTreeViewItemInsertModeLAST:
                    item.hInsertAfter = TVI_LAST;
                    break;

                case PXUIElementTreeViewItemInsertModeSORT:
                    item.hInsertAfter = TVI_SORT;
                    break;
            }

            item.item.mask = TVIF_TEXT | TVIF_IMAGE;

            HTREEITEM itemID = TreeView_InsertItem(pxUIElementTreeViewItemInfo->TreeView->ID, &item);

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                TreeView_Expand
                (
                    pxUIElementTreeViewItemInfo->TreeView->ID,
                    pxUIElementTreeViewItemInfo->ItemParent->ID,
                    TVE_EXPAND
                );
            }

            pxUIElement->ID = itemID;
            pxUIElementTreeViewItemInfo->ItemHandle = itemID;

            break;
        }
        case PXUIElementTypeImage:
        {

            break;
        }
        case PXUIElementTypeDropDown:
        {

            break;
        }
        case PXUIElementTypeListBox:
        {

            break;
        }
        case PXUIElementTypeTextEdit:
        {




            break;
        }
        case PXUIElementTypeRichEdit:
        {


            // EM_SETBKGNDCOLOR(pxUIElement->ID, RGB(10, 10, 10));
            // EM_SETTEXTCOLOR(pxUIElement->ID, RGB(200, 200, 200));

            break;
        }
        case PXUIElementTypeScrollBar:
        {

            break;
        }
        case PXUIElementTypeTrackBar:
        {
            HDC dc = GetDC(pxUIElement->ID);


            SetTextColor(dc, RGB(255, 0, 0));
            SetBkColor(dc, RGB(0, 0, 255));


            SendMessageA(pxUIElement->ID, TBM_SETTIPSIDE, TBTS_RIGHT, PXNull);


           // SendMessageA(pxUIElement->ID, SET_BACKGROUND_COLOR, RGB(30, 30, 30), RGB(30, 30, 30));

           // SET_BACKGROUND_COLOR;

           // TreeView_SetBkColor(pxUIElement->ID, RGB(30, 30, 30));

         //   TreeView_SetTextColor(pxUIElement->ID, RGB(200, 200, 200));


            break;
        }
        case PXUIElementTypeStatusBar:
        {

            break;
        }
        case PXUIElementTypeUpDown:
        {

            break;
        }
        case PXUIElementTypeProgressBar:
        {
            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElement = pxUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;

            PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);
            break;
        }
        case PXUIElementTypeHotKey:
        {

            break;
        }
        case PXUIElementTypeCalender:
        {

            break;
        }
        case PXUIElementTypeToolTip:
        {

            break;
        }
        case PXUIElementTypeAnimate:
        {

            break;
        }
        case PXUIElementTypeDatePicker:
        {

            break;
        }
        case PXUIElementTypeGroupBox:
        {

            break;
        }
        case PXUIElementTypeRadioButton:
        {

            break;
        }
        case PXUIElementTypeGroupRadioButton:
        {

            break;
        }
        case PXUIElementTypeTreeView:
        {
            int sizeX = 16;
            int sizeY = 16;
            int amount = 11;

            TreeView_SetBkColor(pxUIElement->ID, RGB(30, 30, 30));

            TreeView_SetTextColor(pxUIElement->ID, RGB(200, 200, 200));


            // Add icons
            HIMAGELIST imageListHandle = ImageList_Create
            (
                sizeX,
                sizeY,
                ILC_COLOR,
                amount,
                amount
            );

            
            HMODULE currentModule = GetModuleHandle(NULL);


            const char* pathList[11] =
            {
                "Texture/Window.bmp",
                "Texture/Button.bmp",              
                "Texture/Edit.bmp",
                "Texture/FrameBuffer.bmp",
                "Texture/TreeView.bmp",
                "Texture/Text.bmp",
                "Texture/Panel.bmp",
                "Texture/Folder.bmp",
                "Texture/H.bmp",
                "Texture/HPP.bmp",
                "Texture/SquareBlue.bmp"
            };


            for(size_t i = 0; i < amount; i++)
            {
               // HBITMAP bitmapHandle = LoadBitmapA(PXNull, OBM_CLOSE);

                const char* filePath = pathList[i];

                HBITMAP bitmapHandle = LoadImageA
                (
                    PXNull,
                    filePath,
                    IMAGE_BITMAP,
                    sizeX,
                    sizeY,
                    LR_LOADFROMFILE
                );

                int addedID = ImageList_Add(imageListHandle, bitmapHandle, PXNull);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "ImageList-Add",
                    "New icon %i",
                    addedID
                );
            }

            TreeView_SetImageList(pxUIElement->ID, imageListHandle, TVSIL_NORMAL);

            break;
        }
        case PXUIElementTypeIPInput:
        {

            break;
        }
        case PXUIElementTypeLink:
        {

            break;
        }
        case PXUIElementTypeHeader:
        {

            break;
        }
        case PXUIElementTypeFontSelector:
        {

            break;
        }
        case PXUIElementTypePageScroll:
        {

            break;
        }
        case PXUIElementTypeTabControll:
        {
            HDC pDC = GetDC(pxUIElement->ID);
            //SetBkMode(pDC, TRANSPARENT);
           SetBkColor(pDC, RGB(255, 0, 0));
           SetTextColor(pDC, RGB(0, 0, 255));

            PXUIElementTabPageInfo* const pxUIElementTabPageInfo = &pxGUIElementCreateInfo->Data.TabPage;



            pxUIElement->ListEESize = pxGUIElementCreateInfo->Data.TabPage.TabPageSingleInfoAmount;
            PXNewList(PXUIElement*, pxGUIElementCreateInfo->Data.TabPage.TabPageSingleInfoAmount, &pxUIElement->ListEEData, PXNull);


            for(PXSize i = 0; i < pxUIElementTabPageInfo->TabPageSingleInfoAmount; ++i)
            {
                PXUIElementTabPageSingleInfo* const pxUIElementTabPageSingleInfo = &pxUIElementTabPageInfo->TabPageSingleInfoList[i];

                char buffer[64];                
                PXTextPrintA(buffer, 64, "TabPage-%i-%s", i, pxUIElementTabPageSingleInfo->PageName);

                // Create a panel for each page, to contain all elements, so that we can hide and show all at once
                PXGUIElementCreateInfo pxGUIElementCreateInfoPanel;
                PXClear(PXGUIElementCreateInfo, &pxGUIElementCreateInfoPanel);
                pxGUIElementCreateInfoPanel.Type = PXUIElementTypePanel;
                pxGUIElementCreateInfoPanel.Name = buffer;
                pxGUIElementCreateInfoPanel.UIElement = pxUIElementTabPageSingleInfo->UIElement;
                pxGUIElementCreateInfoPanel.UIElementWindow = pxGUIElementCreateInfo->UIElementWindow;
                pxGUIElementCreateInfoPanel.UIElementParent = pxUIElement;
                pxGUIElementCreateInfoPanel.BehaviourFlags = PXUIElementDecorative;
                pxGUIElementCreateInfoPanel.StyleFlagList = PXGUIElementStyleDefault;
                pxGUIElementCreateInfoPanel.Position.FlagListKeep = PXUIElementAllignLeft;
                pxGUIElementCreateInfoPanel.Position.MarginLeft = 0.005;
                pxGUIElementCreateInfoPanel.Position.MarginTop = 0.1;
                pxGUIElementCreateInfoPanel.Position.MarginRight = 0.005;
                pxGUIElementCreateInfoPanel.Position.MarginBottom = 0.02;
                pxGUIElementCreateInfoPanel.Data.Text.Content = buffer;

                PXGUIElementCreate(pxGUISystem, &pxGUIElementCreateInfoPanel, 1);


                pxUIElement->ListEEData[i] = *pxUIElementTabPageSingleInfo->UIElement;


                // Add tabs for each day of the week. 
                TCITEM tie;
                tie.mask = TCIF_TEXT | TCIF_IMAGE;
                tie.iImage = pxUIElementTabPageSingleInfo->ImageID;
                tie.pszText = pxUIElementTabPageSingleInfo->PageName;

                auto x = TabCtrl_InsertItem(pxUIElement->ID, i, &tie);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "TabControl-Add",
                    "Page added %s",
                    pxUIElementTabPageSingleInfo->PageName
                );
            }        

            SendMessage(pxUIElement->ID, TCM_SETCURFOCUS, 0, 0);

            break;
        }
        case PXUIElementTypeToggle:
        {

            break;
        }
        case PXUIElementTypeColorPicker:
        {

            break;
        }
        case PXUIElementTypeSlider:
        {

            break;
        }
        case PXUIElementTypeCheckBox:
        {


            break;
        }
        case PXUIElementTypeComboBox:
        {

            break;
        }
        case PXUIElementTypeRenderFrame:
        {
#if 1
            PXUIElementSceneRenderInfo* const pxUIElementSceneRenderInfo = &pxGUIElementCreateInfo->Data.SceneRender;
            PXEngine* const pxEngine = pxUIElementSceneRenderInfo->Engine;
            PXEngineStartInfo* const pxEngineStartInfo = pxUIElementSceneRenderInfo->StartInfo;

            pxEngineStartInfo->Mode = PXGraphicInitializeModeOSGUIElement;
            pxEngineStartInfo->Width = pxUIElement->Position.Width;
            pxEngineStartInfo->Height = pxUIElement->Position.Height;
            pxEngineStartInfo->UIElement = pxUIElement;
            pxEngineStartInfo->Name = "UIElement-RenderFrame";
            pxEngineStartInfo->UseMouseInput = 1;

            PXEngineStart(pxEngine, pxEngineStartInfo);

#endif

            break;
        }

        case PXUIElementTypeWindow:
        {
            PXGUIElementCreateWindowInfo* const pxGUIElementCreateWindowInfo = &pxGUIElementCreateInfo->Data.Window;

            PXWindowTitleBarColorSet(pxUIElement->ID);
            UpdateWindow(pxUIElement->ID);

            // ShowWindow(pxWindow->ID, SW_NORMAL)

          


#if 0
#if OSUnix

            int numberOfDevices = 0;

            const XDeviceInfo* deviceInfoList = XListInputDevices(display, &numberOfDevices);

            for(int i = 0; i < numberOfDevices; ++i)
            {
                const XDeviceInfo& xDeviceInfo = deviceInfoList[i];

                printf
                (
                    "[Device] ID:%i Name:%s Use:%i\n",
                    xDeviceInfo.id,
                    xDeviceInfo.name,
                    xDeviceInfo.use
                );

                // XOpenDevice(display, i); // Cannot open mouse or keyboard??  invalid call

                // XEventClass eventList[8]={0,0,0,0,0,0,0,0};
               //  int listSize = xDeviceInfo.num_classes;

               //  int resultE = XSelectExtensionEvent(display, PXWindowID, eventList, listSize);

                // printf("");
            }
#endif



            if(info)
            {
                const PXActionResult actionResult = PXThreadRun(&window->MessageThread, "PXWindowMessageLoop", PXWindowMessageLoop, window);
                const PXBool sucessful = PXActionSuccessful == actionResult;

                if(!sucessful)
                {
                    return; // TODO: return something?
                }

                PXBool expected = PXTrue;
                PXAwaitInfo pxAwaitInfo;
                pxAwaitInfo.DataTarget = &window->IsRunning;
                pxAwaitInfo.DataExpect = &expected;
                pxAwaitInfo.DataSize = sizeof(PXBool);

                PXAwaitChange(&pxAwaitInfo);
            }

#endif

            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIElementUpdate(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount)
{
    for(size_t i = 0; i < amount; ++i)
    {
        PXGUIElementUpdateInfo* const pxGUIElementUpdateInfo = &pxGUIElementUpdateInfoList[i];
        PXUIElement* const pxUIElement = pxGUIElementUpdateInfo->UIElement;

        switch(pxGUIElementUpdateInfo->Property)
        {
            case PXUIElementPropertyTextContent:
            {
                PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementUpdateInfo->Data.Text;

                if(!pxUIElement)
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
                    (int)pxUIElement->ID,
                    pxUIElementTextInfo->Content
                );
#endif        

#if OSUnix
                XStoreName(pxGUIElementCreateWindowInfo->DisplayCurrent, pxWindow->ID, pxWindow->Title.TextA);
#elif OSWindows
                const PXBool success = SetWindowTextA(pxUIElement->ID, pxUIElementTextInfo->Content);


                //   SendMessageA(pxUIElement->ID, PBM_SETPOS, stepsConverted, 0);


#if 0
                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                PXWindowSizeInfo pxWindowSizeInfo;

                PXWindowID pxWindowID = pxGUIElementUpdateInfo->WindowReference ? pxGUIElementUpdateInfo->WindowReference->ID : PXNull;

                PXWindowSizeGet(pxWindowID, &pxWindowSizeInfo);

                pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Height;

                PXUIElementPositionCalculcate(pxUIElement, &pxUIElementPositionCalulcateInfo);

                const PXBool succes54s = MoveWindow
                (
                    pxUIElement->ID,
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
                PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementUpdateInfo->Data.Text;

                HDC xx = GetDC(pxUIElement->ID);



                LONG ww = SetWindowLongA(pxUIElement->ID, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

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

                break;
            }
            case PXUIElementPropertySize:
            {
                PXGUIElementUpdateInfo sizeFetchInfo;
                PXClear(PXGUIElementUpdateInfo, &sizeFetchInfo);
                sizeFetchInfo.UIElement = pxGUIElementUpdateInfoList->WindowReference;
                sizeFetchInfo.Property = PXUIElementPropertySize;

                PXGUIElementFetch(pxGUISystem, &sizeFetchInfo, 1);

                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                //PXWindowSizeInfo pxWindowSizeInfo;
                //PXWindowID pxWindowID = pxUIElement->Parent ? pxUIElement->Parent->ID : PXNull;             

                //PXWindowSizeGet(PXNull, &pxWindowSizeInfo); // TODO: Problematic

                pxUIElementPositionCalulcateInfo.WindowWidth = sizeFetchInfo.Data.Size.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = sizeFetchInfo.Data.Size.Height;

                PXUIElementPositionCalculcate(pxUIElement, &pxUIElementPositionCalulcateInfo);

                

                // If the window is an actual window, we will have bad allignment if we use MoveWindow
                //if(pxUIElement->Type != PXUIElementTypeWindow)

                if(0)
                {
                    const PXBool success = MoveWindow
                    (
                        pxUIElement->ID,
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
                        pxUIElement->ID,
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
                if(PXUIElementTypeRenderFrame == pxUIElement->Type)
                {
                    PXUIElementSceneRenderInfo* const pxUIElementSceneRenderInfo = &pxGUIElementUpdateInfo->Data.SceneRender;
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
                PXUIElementProgressBarInfo* const progressBar = &pxGUIElementUpdateInfo->Data.Text;

                PXInt32U stepsConverted = progressBar->Percentage * 100;
                SendMessageA(pxUIElement->ID, PBM_SETPOS, stepsConverted, 0);

                break;
            }
            case PXUIElementPropertyProgressbarBarColor:
            {
                PXUIElementProgressBarInfo* const progressBar = &pxGUIElementUpdateInfo->Data.Text;

                COLORREF color = RGB(progressBar->BarColor.Red, progressBar->BarColor.Green, progressBar->BarColor.Blue);
                SendMessageA(pxUIElement->ID, PBM_SETBARCOLOR, 0, color);

                break;
            }
            case PXUIElementPropertyVisibility:
            {
                PXBool show = pxGUIElementUpdateInfo->Show;
                int showID = show ? SW_SHOW : SW_HIDE;

                const PXBool isWindowValid = IsWindow(pxUIElement->ID);

                if(!isWindowValid)
                {
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "GUI",
                        "Visibility",
                        "%20s (0x%p), Invalid",
                        pxUIElement->NameData,
                        pxUIElement->ID
                    );

                    break;
                }

                pxUIElement->IsEnabled = show;
                // EnableWindow(pxUIElement->ID, show);

                ShowWindow(pxUIElement->ID, showID);
                BOOL res = ShowWindow(pxUIElement->ID, showID); // Twice to prevent some errors

                //  HWND parrent = GetParent(pxUIElement->ID);

                //  HWND grandParrent = GetParent(parrent);

                //  UpdateWindow(pxUIElement->ID);
                 //  UpdateWindow(parrent);
                 //  UpdateWindow(grandParrent);

                         //  UpdateWindow(sourceObject);
                          // FlashWindow(sourceObject, 1);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "Visibility",
                    "Page %20s, (0x%p), Mode:%s Status:%s",
                    pxUIElement->NameData,
                    pxUIElement->ID,
                    show ? "Show" : "Hide",
                    res ? "OK" : "FAIL"
                );


                // Own solution
                {
                    PXGUIElementChildListEnumerate(pxGUISystem, pxUIElement, show);



                }


                const BOOL success = EnumChildWindows
                (
                    pxUIElement->ID,
                    PXWindowEnumChildProc,
                    &showID
                );

                break;
            }
            case PXUIElementPropertyComboBoxAdd:
            {
                PXUIElementComboBoxInfo* const pxUIElementComboBoxInfo = &pxGUIElementUpdateInfo->Data.ComboBox;

                for(size_t i = 0; i < pxUIElementComboBoxInfo->DataListAmount; i++)
                {
                    char* name = pxUIElementComboBoxInfo->DataList[i];

                    ComboBox_AddString(pxUIElement->ID, name);
                }

                ComboBox_SetCurSel(pxUIElement->ID, 0);

                break;
            }
            case PXUIElementPropertyItemAdd:
            {
                PXUIElementItemInfo* pxUIElementItemInfo = &pxGUIElementUpdateInfo->Data.TreeViewItem;

                TVINSERTSTRUCT item;
                PXClear(TVINSERTSTRUCT, &item);

                const char text[] = "[N/A]";

                item.item.pszText = text;
                item.item.cchTextMax = sizeof(text);
                item.hInsertAfter = TVI_ROOT;
                item.item.mask = TVIF_TEXT;

                TreeView_InsertItem(pxUIElement->ID, &item);

                break;
            }
        }
    } 

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIElementFetch(PXGUISystem* const pxGUISystem, PXGUIElementUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXGUIElementUpdateInfo* const pxGUIElementUpdateInfo = &pxGUIElementUpdateInfoList[i];
        PXUIElement* const pxUIElement = pxGUIElementUpdateInfo->UIElement;

        switch(pxGUIElementUpdateInfo->Property)
        {
            case PXUIElementPropertySizeParent:
            {
                PXWindowSizeInfo* pxWindowSizeInfo = &pxGUIElementUpdateInfo->Data.Size;

               // const PXBool hasParent = pxUIElement ? pxUIElement->Parent : PXFalse;

                const PXBool hasParent = pxGUIElementUpdateInfoList->WindowReference;

                if(!hasParent) // Special behaviour, if ID is null, get the screensize
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;

                    PXMonitorGetSize(&pxWindowSizeInfo->Width, &pxWindowSizeInfo->Height);

                    return PXActionSuccessful;
                }

                PXGUIElementUpdateInfo pxGUIElementUpdateInfoSub;
                PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfoSub);
                pxGUIElementUpdateInfoSub.Property = PXUIElementPropertySize;
                pxGUIElementUpdateInfoSub.UIElement = pxGUIElementUpdateInfoList->WindowReference;

                PXGUIElementFetch(pxGUISystem, &pxGUIElementUpdateInfoSub, 1);

                PXCopy(PXWindowSizeInfo, &pxGUIElementUpdateInfoSub.Data.Size, &pxGUIElementUpdateInfo->Data.Size);

                break;
            }
            case PXUIElementPropertySize:
            {
                PXWindowSizeInfo* pxWindowSizeInfo = &pxGUIElementUpdateInfo->Data.Size;

#if OSUnix
#elif PXOSWindowsDestop

                RECT rect;

                //const PXBool result = GetWindowRect(pxUIElement->ID, &rect); // Windows 2000, User32.dll, winuser.h
                const PXBool result = GetClientRect(pxUIElement->ID, &rect); // Windows 2000, User32.dll, winuser.h

                if(!result)
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = 0;
                    pxWindowSizeInfo->Height = 0;

                    return PXActionRefusedObjectNotFound;
                }

                // Get Last Error

                pxWindowSizeInfo->X = rect.left;
                pxWindowSizeInfo->Y = rect.top;
                pxWindowSizeInfo->Width = rect.right - rect.left;
                pxWindowSizeInfo->Height = rect.bottom - rect.top;
#endif

                break;
            }

            default:
                break;
        }
    }



    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIElementRelease(PXUIElement* const pxUIElement)
{
#if 0

#if OSUnix
    glXMakeCurrent(window->DisplayCurrent, None, NULL);
    //    glXDestroyContext(DisplayCurrent, PXOpenGLConextID);
    XDestroyWindow(window->DisplayCurrent, window->ID);
    XCloseDisplay(window->DisplayCurrent);

    window->ID = PXNull;

#elif PXOSWindowsDestop

    // Minimizes but does not destroy the specified window.
    CloseWindow(window->ID); // Windows 2000, User32.dll, winuser.h

    // Finally destroy the window
    DestroyWindow(window->ID); // Windows 2000, User32.dll, winuser.h

    window->ID = PXNull;

#else
    return; // Not supported on this OS
#endif

#endif

    return PXActionSuccessful;
}

void PXAPI PXGUIElementhSizeRefresAll(PXGUISystem* const pxGUISystem)
{
    if(!(pxGUISystem))
    {
        return;
    }

    PXDictionary* const uiElementLookup = &pxGUISystem->UIElementLookUp;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

    for(PXSize i = 0; i < uiElementLookup->EntryAmountCurrent; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXUIElement* uiElement = PXNull;

        PXDictionaryIndex(uiElementLookup, i, &pxDictionaryEntry);

        uiElement = *(PXUIElement**)pxDictionaryEntry.Value;

        if(uiElement->Type == PXUIElementTypeWindow)
        {
            continue;
        }

        PXGUIElementUpdateInfo pxGUIElementUpdateInfo;
        PXClear(PXGUIElementUpdateInfo, &pxGUIElementUpdateInfo);
        pxGUIElementUpdateInfo.UIElement = uiElement;
        pxGUIElementUpdateInfo.WindowReference = PXNull;
        pxGUIElementUpdateInfo.Property = PXUIElementPropertySizeParent;

        PXGUIElementUpdate(pxGUISystem, &pxGUIElementUpdateInfo, 1);
    }
}

PXActionResult PXAPI PXWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Window",
        "PixelSystem",
        "Setting info"
    );
#endif

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    if (!pxWindowPixelSystemInfo->HandleDeviceContext) // If we dont have a prefered GPU
    {
        pxWindowPixelSystemInfo->HandleDeviceContext = GetDC(pxWindowPixelSystemInfo->HandleWindow); // Get the "default" device
    }

    const WORD  nVersion = 1;
    DWORD dwFlags = 
        //PFD_DRAW_TO_WINDOW |
        //PFD_DIRECT3D_ACCELERATED | 
        PFD_DOUBLEBUFFER | 
        0;



    dwFlags |= pxWindowPixelSystemInfo->OpenGL * PFD_SUPPORT_OPENGL;
    dwFlags |= pxWindowPixelSystemInfo->DirectX * PFD_SUPPORT_DIRECTDRAW;
    dwFlags |= pxWindowPixelSystemInfo->GDI * PFD_SUPPORT_GDI;


    const BYTE  iPixelType = PFD_TYPE_RGBA; // The kind of framebuffer. RGBA or palette.
    const BYTE  cColorBits = pxWindowPixelSystemInfo->BitPerPixel;   // Colordepth of the framebuffer.
    const BYTE  cRedBits = 0;
    const BYTE  cRedShift = 0;
    const BYTE  cGreenBits = 0;
    const BYTE  cGreenShift = 0;
    const BYTE  cBlueBits = 0;
    const BYTE  cBlueShift = 0;
    const BYTE  cAlphaBits = 0;
    const BYTE  cAlphaShift = 0;
    const BYTE  cAccumBits = 0;
    const BYTE  cAccumRedBits = 0;
    const BYTE  cAccumGreenBits = 0;
    const BYTE  cAccumBlueBits = 0;
    const BYTE  cAccumAlphaBits = 0;
    const BYTE  cDepthBits = 24; // Number of bits for the depthbuffer
    const BYTE  cStencilBits = 8;  // Number of bits for the stencilbuffer
    const BYTE  cAuxBuffers = 0;  // Number of Aux buffers in the framebuffer.
    const BYTE  iLayerType = PFD_MAIN_PLANE;
    const BYTE  bReserved = 0;
    const DWORD dwLayerMask = 0;
    const DWORD dwVisibleMask = 0;
    const DWORD dwDamageMask = 0;
    const PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        nVersion,
        dwFlags,
        iPixelType,
        cColorBits,
        cRedBits,cRedShift,
        cGreenBits, cGreenShift,
        cBlueBits, cBlueShift,
        cAlphaBits, cAlphaShift,
        cAccumBits,
        cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits,
        cDepthBits,
        cStencilBits,
        cAuxBuffers,
        iLayerType,
        bReserved,
        dwLayerMask, dwVisibleMask, dwDamageMask
    };
    const int letWindowsChooseThisPixelFormat = ChoosePixelFormat(pxWindowPixelSystemInfo->HandleDeviceContext, &pfd);
    const PXBool successul = SetPixelFormat(pxWindowPixelSystemInfo->HandleDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

    if(!successul)
    {
        const PXActionResult pxActionResult = PXErrorCurrent();

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "Window",
            "PixelSystem",
            "Setting failed"
        );
#endif
    }
       
    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif  
}

void PXAPI PXWindowUpdate(PXUIElement* const pxUIElement)
{
    if(!pxUIElement)
    {
        return;
    }

    if(!pxUIElement->ID)
    {
        return;
    }

   // pxWindow->MouseCurrentInput.ButtonsDelta = 0; // Reset mouse data

#if OSUnix
    XEvent windowEvent;

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




    XLockDisplay(pxWindow->DisplayCurrent);

    XNextEvent(pxWindow->DisplayCurrent, &windowEvent);

    XUnlockDisplay(pxWindow->DisplayCurrent);

    PXWindowEventHandler(pxWindow, &windowEvent);

#elif PXOSWindowsDestop     
  
    for (;;)
    {
        MSG message;

        const PXBool peekResult = PeekMessage(&message, pxUIElement->ID, 0, 0, PM_NOREMOVE); // Windows 2000, User32.dll, winuser.h

        if (!peekResult)
        {
            break; // Stop, no more messages
        }

        const PXBool messageResult = GetMessage(&message, pxUIElement->ID, 0, 0); // Windows 2000, User32.dll, winuser.h

        if (!messageResult)
        {
            continue; // Skip, message fetch failed
        }

        const BOOL translateResult = TranslateMessage(&message); // Windows 2000, User32.dll, winuser.h
        const LRESULT dispatchResult = DispatchMessage(&message); // Windows 2000, User32.dll, winuser.h
    }
#endif

}

PXProcessThreadID PXAPI PXWindowThreadProcessID(const PXWindowID windowID)
{
#if OSUnix
    return -1;
#elif PXOSWindowsDestop
    return GetWindowThreadProcessId(windowID, PXNull);
#else
    return -1;
#endif
}

PXWindowID PXAPI PXWindowFindViaTitle(const PXText* const windowTitle)
{
    switch (windowTitle->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix
            return PXNull;
#elif PXOSWindowsDestop
            return FindWindowA(0, windowTitle->TextA); // Windows 2000, User32.dll, winuser.h
#else
            return PXFalse; // Not supported by OS
#endif
        }
        case TextFormatUNICODE:
        {
#if OSUnix
            return PXNull;
#elif PXOSWindowsDestop
            return FindWindowW(0, windowTitle->TextW); // Windows 2000, User32.dll, winuser.h
#else
            return PXNull; // Not supported by OS
#endif
        }
    }

    return PXNull;
}

void PXAPI PXWindowIconCorner()
{
}

void PXAPI PXWindowIconTaskBar()
{
}

PXWindow* PXAPI PXWindowLookupFind(const PXWindowID PXWindowID)
{
    return currentWindow;
}

void PXAPI PXWindowLookupRemove(const PXWindow* window)
{
    currentWindow = 0;
}

PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindowID pxWindowID)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;
#elif OSWindows

    PXLibrary pyLibrary;

    // Open lib
    {
        const PXActionResult libOpenResult = PXLibraryOpenA(&pyLibrary, "DWMAPI.DLL");

        if (PXActionSuccessful != libOpenResult)
        {
            return PXActionRefusedNotSupported;
        }
    }


    typedef HRESULT(WINAPI* PXDwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, _In_reads_bytes_(cbAttribute) LPCVOID pvAttribute, DWORD cbAttribute);

    PXDwmSetWindowAttribute pxDwmSetWindowAttribute;

    PXBool hasFunction = PXLibraryGetSymbolA(&pyLibrary, &pxDwmSetWindowAttribute, "DwmSetWindowAttribute");

    if (!hasFunction)
    {
        PXLibraryClose(&pyLibrary);

        return PXActionRefusedNotSupported;        
    }

    const BOOL useDarkMode = PXTrue;
    const BOOL setAttributeSuccess = SUCCEEDED(pxDwmSetWindowAttribute(pxWindowID, Windows10DarkModeID, &useDarkMode, sizeof(BOOL))); // Windows Vista, Dwmapi.dll;Uxtheme.dll, dwmapi.h

    PXLibraryClose(&pyLibrary);

    if (!setAttributeSuccess)
    {
        return PXActionCancelled;
    }

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXWindowMouseMovementEnable(const PXWindowID pxWindow)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
#if PXOSWindowsDestop && WindowsAtleastXP
    RAWINPUTDEVICE rawInputDeviceList[2];
    rawInputDeviceList[0].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    rawInputDeviceList[0].usUsage = 0x03;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rawInputDeviceList[0].hwndTarget = pxWindow;

    rawInputDeviceList[1].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    rawInputDeviceList[1].usUsage = 0x02;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rawInputDeviceList[1].hwndTarget = pxWindow;

    // WARNING
    // RegisterRawInputDevices should not be used from a library!
    // As it may interfere with any raw input processing logic already present in applications that load it.
    const PXBool regsiterResultID = RegisterRawInputDevices(rawInputDeviceList, 2, sizeof(RAWINPUTDEVICE)); // Windows XP, User32.dll, winuser.h

    if(!regsiterResultID)
    {
        const PXActionResult regsiterResult = PXErrorCurrent();

        PXLogPrint
        (
            PXLoggingError,
            "GUI",
            "Input-Mouse",
            "Failed to registerd device for <0x%p>",
            pxWindow
        );


        return regsiterResult;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "GUI", 
        "Input-Mouse", 
        "Registerd device for <0x%p>",
        pxWindow
    );

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif

    return PXActionRefusedNotSupported;;
    
#endif
}

/*
PXActionResult PXAPI PXWindowSizeSet(const PXWindowID pxWindow, PXWindowSizeInfo* const pxWindowSizeInfo)
{
#if OSUnix
#elif PXOSWindowsDestop
    RECT rect;

    rect.left = pxWindowSizeInfo->X;
    rect.top = pxWindowSizeInfo->Y;
    rect.right = pxWindowSizeInfo->X + pxWindowSizeInfo->Width;
    rect.bottom = pxWindowSizeInfo->Y + pxWindowSizeInfo->Height;

    DWORD style = 0;
    DWORD exStyle = 0;

    //AdjustWindowRectEx();

    const PXBool result = AdjustWindowRectEx(&rect, style, FALSE, exStyle); // Windows 2000, User32.dll, winuser.h
    const PXBool success = result != 0;

    // Get Last Error

    return PXActionSuccessful;
#endif
}
*/

PXActionResult PXAPI PXWindowPosition(const PXWindowID pxWindowID, PXInt32S* x, PXInt32S* y)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
    RECT rectangle;
    //const PXBool success = GetWindowRect(window->ID, &rectangle); // Windows 2000, User32.dll, winuser.h

    const PXBool success = GetWindowRect(pxWindowID, &rectangle);

    if (!success)
    {
        *x = -1;
        *y = -1;

        return PXErrorCurrent();
    }

  //  window->X = rectangle.left;
  //  window->Y = rectangle.top;
 //   window->Width = rectangle.right - rectangle.left;
 //   window->Height = rectangle.bottom - rectangle.top;

    return PXActionSuccessful;

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXWindowCursorCaptureMode(const PXWindowID pxWindowID, const PXWindowCursorMode cursorMode)
{
    PXInt32S horizontal = 0;
    PXInt32S vertical = 0;

#if OSUnix
#elif PXOSWindowsDestop

    PXMonitorGetSize(&horizontal, &vertical);

    switch(cursorMode)
    {
        case PXWindowCursorShow:
        {
            //printf("[Cursor] Show\n");

            while(ShowCursor(1) < 0);

            const PXBool clipResult = ClipCursor(NULL);
            const HCURSOR cursorSet = SetCursor(pxWindowID);

            break;
        }
        case PXWindowCursorLock:
        {
           // printf("[Cursor] Lock\n");

            // Capture cursor
            {
                RECT clipRect;
                GetClientRect(pxWindowID, &clipRect);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.left);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.right);
                ClipCursor(&clipRect);
            }

            break;
        }
        case PXWindowCursorLockAndHide:
        {
            //printf("[Cursor] Lock and hide\n");

            while(ShowCursor(0) >= 0);

            {
                RECT clipRect;
                GetClientRect(pxWindowID, &clipRect);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.left);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.right);

                int xOff = (clipRect.right - clipRect.left) / 2;
                int yoFf = (clipRect.bottom - clipRect.top) / 2;

                clipRect.left += xOff;
                clipRect.top += yoFf;
                clipRect.right -= xOff;
                clipRect.bottom -= yoFf;

                unsigned char sucessClip = ClipCursor(&clipRect);
            }


            CURSORINFO cursorInfo;
            cursorInfo.cbSize = sizeof(CURSORINFO);

            const unsigned char sucessfulInfoGet = GetCursorInfo(&cursorInfo);

            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

            CONSOLE_CURSOR_INFO lpCursor;
            lpCursor.bVisible = 0;
            lpCursor.dwSize = sizeof(CONSOLE_CURSOR_INFO);
            unsigned char y = SetConsoleCursorInfo(console, &lpCursor);

            break;
        }
    }

    //window->CursorModeCurrent = cursorMode;
#endif
}

PXBool PXAPI PXWindowFrameBufferSwap(const PXWindowID pxWindowID)
{
#if OSUnix
    glXSwapBuffers(window->DisplayCurrent, window->ID);
    return PXFalse;

#elif OSWindows

    const PXBool result = 0;// SwapBuffers(window->HandleDeviceContext);

   return result;

#else
    return PXFalse;
#endif
}

PXBool PXAPI PXWindowInteractable(const PXWindowID pxWindowID)
{
#if 0
    switch (window->CursorModeCurrent)
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
}

PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y)
{
    PXInt32S xPos = 0;
    PXInt32S yPos = 0;
    const PXBool sucessfulA = PXWindowCursorPositionInDestopGet(pxWindowID, &xPos, &yPos);

#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    POINT point;
    point.x = xPos;
    point.y = yPos;

    const PXBool sucessful = ScreenToClient(pxWindowID, &point);  // are now relative to hwnd's client area

    if(sucessful)
    {
        *x = point.x;
        *y = point.y;
    }
    else
    {
        *x = 0;
        *y = 0;
    }

    return sucessful;
#endif
}

PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXWindowID pxWindowID, PXInt32S* const x, PXInt32S* const y)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop

#if WindowsAtleastVista
	POINT point;
	point.x = 0;
	point.y = 0;

	const PXBool successful = GetPhysicalCursorPos(&point); // Windows Vista, User32.dll, winuser.h

	if (successful)
	{
		*x = point.x;
		*y = point.y;
	}
	else
	{
		*x = 0;
		*y = 0;
	}

	return successful;
#else

	const PXSize mouseMovePointSize = sizeof(MOUSEMOVEPOINT);
	const int nVirtualWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const int nVirtualHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	const int nVirtualLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
	const int nVirtualTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
	const int mode = GMMP_USE_DISPLAY_POINTS;
	MOUSEMOVEPOINT mp_in;
	MOUSEMOVEPOINT mp_out[64];

	PXMemoryClear(&mp_in, mouseMovePointSize);

	mp_in.x = 0x0000FFFF;//Ensure that this number will pass through.
	mp_in.y = 0x0000FFFF;
	const int numberOfPoints = GetMouseMovePointsEx(mouseMovePointSize, &mp_in, &mp_out, 64, mode); // Windows 2000, User32.dll, winuser.h
	const PXBool success = numberOfPoints > 0;

	if (!success)
	{
		return PXFalse;
	}

	/*
	for (PXSize i = 0; i < (PXSize)numberOfPoints; ++i)
	{
		switch (mode)
		{
		case GMMP_USE_DISPLAY_POINTS:
		{

			if (mp_out[i].x > 32767)
				mp_out[i].x -= 65536;
			if (mp_out[i].y > 32767)
				mp_out[i].y -= 65536;
			break;
		}
		case GMMP_USE_HIGH_RESOLUTION_POINTS:
		{
			mp_out[i].x = ((mp_out[i].x * (nVirtualWidth - 1)) - (nVirtualLeft * 65536)) / nVirtualWidth;
			mp_out[i].y = ((mp_out[i].y * (nVirtualHeight - 1)) - (nVirtualTop * 65536)) / nVirtualHeight;
			break;
		}
		}

		*x = mp_out[i].x;
		*y = mp_out[i].y;

		break;
	}*/

	return PXTrue;

#endif


#endif
}

PXBool PXAPI PXWindowIsInFocus(const PXWindowID pxWindowID)
{
#if OSUnix
    return PXFalse;
#elif PXOSWindowsDestop
    const HWND windowIDInFocus = GetForegroundWindow(); // Windows 2000, User32.dll,
    const PXBool isInFocus = pxWindowID == windowIDInFocus;

    return isInFocus;
#endif
}

PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon)
{
    switch (cursorIcon)
    {
        default:
        case PXCursorIconInvalid:
            return -1;

        case PXCursorIconNormal:
            return CursorIconNormalID;

        case PXCursorIconIBeam:
            return CursorIconIBeamID;

        case PXCursorIconWait:
            return CursorIconWaitID;

        case PXCursorIconCross:
            return CursorIconCrossID;

        case PXCursorIconUp:
            return CursorIconUpID;

        case PXCursorIconHand:
            return CursorIconHandID;

        case PXCursorIconNotAllowed:
            return CursorIconNoAllowedID;

        case PXCursorIconAppStarting:
            return CursorIconAppStartingID;

        case PXCursorIconResizeHorizontal:
            return CursorIconResizeHorizontalID;

        case PXCursorIconResizeVertical:
            return CursorIconResizeVerticalID;

        case PXCursorIconResizeClockwise:
            return CursorIconResizeClockwiseID;

        case PXCursorIconResizeClockwiseCounter:
            return CursorIconResizeClockwiseCounterID;

        case PXCursorIconResizeAll:
            return CursorIconResizeAllID;
    }
}
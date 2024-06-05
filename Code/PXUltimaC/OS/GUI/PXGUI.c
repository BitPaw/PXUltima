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
                pxWindowEvent->UIElementSender->Info.ID
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
        case PXWindowEventTypeResize:
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
        PXGUIElement* const pxGUIElement = pxWindowEvent->UIElementReference;

        if(pxGUIElement->InteractCallBack)
        {
            pxGUIElement->InteractCallBack(pxGUIElement->InteractOwner, pxWindowEvent);
        }
    }

    if(pxWindowEvent->UIElementSender)
    {
        PXGUIElement* const pxGUIElement = pxWindowEvent->UIElementSender;

        if(pxGUIElement->InteractCallBack)
        {
            pxGUIElement->InteractCallBack(pxGUIElement->InteractOwner, pxWindowEvent);
        }
    }
}


#if OSUnix
void PXWindowEventHandler(PXGUIElement* const pxWindow, const XEvent* const xEventData)
{
    PXGUISystem* pxGUISystem = PXGUISystemGlobalReference;

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.EventData = xEventData;
    pxWindowEvent.UIElementReference = pxWindow;

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

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

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

                default:
                    pxWindowEvent.InputMouseButton.Button = PXMouseButtonInvalid;
                    break;
            }

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case MotionNotify:
        {
           // printf("[Event] MotionNotify \n");

          //  pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
          //  PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

            break;
        }
        case EnterNotify:
        {
            pxWindowEvent.Type = PXWindowEventTypeFocusEnter;
            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case LeaveNotify:
        {
            pxWindowEvent.Type = PXWindowEventTypeFocusLeave;
            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case FocusIn:
        {
            pxWindowEvent.Type = PXWindowEventTypeFocusLeave;
            // PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);
            break;
        }
        case FocusOut:
        {
            pxWindowEvent.Type = PXWindowEventTypeFocusLeave;
            // PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);
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
            const XResizeRequestEvent* resizeRequestEvent = &xEventData->xresizerequest;
            const int width = resizeRequestEvent->width;
            const int height = resizeRequestEvent->height;

            pxWindowEvent.Type = PXWindowEventTypeResize;
            pxWindowEvent.Resize.Width = width;
            pxWindowEvent.Resize.Height = height;

            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);

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

                            PXWindowEventConsumer(pxGUISystem, &pxWindowEvent);
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

        PXDictionaryFindEntry(&pxGUISystem->ResourceManager->GUIElementLookup, &windowID, &pxWindowEvent.UIElementReference);
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

                    PXGUIElement* pxTreeViewContainer = PXNull;
                    PXGUIElement* pxTreeViewItem = PXNull;

                    if(pxGUISystem)
                    {
                        const HWND itemHandle = TreeView_GetSelection(sourceObject); // Event does not give us the handle, fetch manually.

                        // Fetch treeview object
                        PXDictionaryFindEntry(&pxGUISystem->ResourceManager->GUIElementLookup, &sourceObject, &pxTreeViewContainer);

                        // Fetch treeviewitem object
                        PXDictionaryFindEntry(&pxGUISystem->ResourceManager->GUIElementLookup, &itemHandle, &pxTreeViewItem);
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
                    PXGUIElement* const pxGUIElement = PXNull;

                    PXDictionaryFindEntry(&pxGUISystem->ResourceManager->GUIElementLookup, &sourceObject, &pxGUIElement);

                    // Hide current page
                    const PXBool isValidIndex = pxGUIElement->ListEESize >= (pageID+1);

                    BOOL success = 0;

                    for(PXSize i = 0; i < pxGUIElement->ListEESize; i++)
                    {
                        PXGUIElement* const pxUIElementEEE = pxGUIElement->ListEEData[i];

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
                        PXGUIElement* const pxUIElementEEE = pxGUIElement->ListEEData[pageID];

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
            PXGUIElement* const pxGUIElement = PXNull;

            PXDictionaryFindEntry(&pxGUISystem->ResourceManager->GUIElementLookup, &windowID, &pxGUIElement);

            if(pxGUIElement)
            {
              //  if(!pxGUIElement->IsEnabled)
               // {
                  //  return 0;
              //  }
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
          //  PXGUIElement* pxGUIElement = PXNull;

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
            const PXInt32U buttonType = wParam; // IDOK;
            const HANDLE handle = (HANDLE)lParam;

            PXGUIElement* const pxGUIElement = PXNull;

            if(pxGUISystem)
            {
                PXDictionaryFindEntry(&pxGUISystem->ResourceManager->GUIElementLookup, &handle, &pxGUIElement);
            }

            if(!(pxGUIElement->Info.Flags & PXEngineResourceInfoEnabled))
            {
               // ShowWindow(pxGUIElement->ID, SW_HIDE);
            }

            pxWindowEvent.Type = PXWindowEventTypeClick;
            pxWindowEvent.UIElementSender = pxGUIElement;

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

void PXAPI PXGUIElementChildListEnumerate(PXGUISystem* const pxGUISystem, PXGUIElement* const parent, PXBool visible)
{
    for(size_t i = 0; i < pxGUISystem->ResourceManager->GUIElementLookup.EntryAmountCurrent; i++)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXGUIElement* childElement = PXNull;

        PXDictionaryIndex(&pxGUISystem->ResourceManager->GUIElementLookup, i, &pxDictionaryEntry);

        childElement = *(PXGUIElement**)pxDictionaryEntry.Value;

        if(childElement->Parent)
        {
            if(childElement->Parent->Info.WindowID == parent->Info.WindowID)
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

PXThreadResult PXOSAPI PXWindowMessageLoop(PXGUIElement* const pxGUIElement)
{
#if 0
    while(pxGUIElement->IsRunning)
    {
        PXWindowUpdate(pxWindow);
        PXThreadYieldToOtherThreads();
    }
#endif

    return PXActionSuccessful;
}

PXBool PXAPI PXGUIElementIsEnabled(const PXWindowID pxUIElementID)
{
#if OSUnix
    return PXFalse;
#elif OSWindows
    return IsWindowEnabled(pxUIElementID); // Windows 2000, User32.dll, winuser.h
#else
    return PXFalse;
#endif
}

PXBool PXAPI PXGUIElementFind(const PXWindowID pxUIElementID, PXGUIElement* const pxGUIElement)
{
    return PXFalse;
}

PXActionResult PXAPI PXGUIElementDelete(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
	const int resultID = XDestroyWindow(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.WindowID);
	result = PXGUIElementErrorFromXSystem(resultID);
#elif OSWindows
	const PXBool success = DestroyWindow(pxGUIElement->Info.WindowID);
    result = PXWindowsErrorCurrent(success);
#else
    result PXActionRefusedNotSupportedByLibrary;
#endif

    if(PXActionSuccessful == result)
    {
        pxGUIElement->Info.WindowID = 0;
    }

    return result;
}

PXActionResult PXAPI PXGUIElementTextSet(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, char* text)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    // Will BadAlloc, BadWindow
    const int resultID = XStoreName(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.WindowID, text);
    result = PXGUIElementErrorFromXSystem(resultID);
#elif OSWindows
    const PXBool success = SetWindowTextA(pxGUIElement->Info.WindowID, text);
    result = PXWindowsErrorCurrent(success);
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
            pxGUIElement
        );
#endif

        return PXFalse;
    }

#if PXLogEnable
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


    return PXTrue;
}

PXBool PXAPI PXGUIElementValueFetch(PXGUIElement* const pxUIElementList, const PXSize dataListAmount, const PXUIElementProperty pxUIElementProperty, void* const dataList)
{
    for(size_t i = 0; i < dataListAmount; ++i)
    {
        PXGUIElement* const pxGUIElement = &pxUIElementList[i];

        switch(pxUIElementProperty)
        {
            case PXUIElementPropertySliderPercentage:
            {
                float* target = &((float*)dataList)[i];

#if OSUnix

#elif OSWindows
                *target = SendMessageA(pxGUIElement->Info.ID, TBM_GETPOS, 0, 0) / 100.f;
#endif

                break;
            }

            default:
                break;
        }
    }

    return PXTrue;
}

PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem)
{
    if(PXGUISystemGlobalReference)
    {
        return PXActionRefusedAlreadyInizialized;
    }

    PXGUISystemGlobalReference = pxGUISystem;

#if OSUnix

    // Make this thread safe
    {
        // This is only needed if we access the X-System with different threads
        // reason is timing collisions where zwo X-Calls are handled at the same time
        // This will leed to problems.
        // On systems where threads are not supported, this will always return NULL
        const int result = XInitThreads();
    }

    // Connect to X-System display server
    {
        PXDisplay* const pxDisplay = &pxGUISystem->DisplayCurrent;

        // open a connection to the x-server. NULL here uses the default display.
        pxDisplay->DisplayHandle = XOpenDisplay(PXNull);   // X11/Xlib.h,  Create Window
        const PXBool successful = PXNull != pxDisplay->DisplayHandle;

        if(!successful)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "GUI",
                "X-System",
                "Failed to open X server. XOpenDisplay() failed."
            );
#endif

            return PXActionFailedInitialization; // printf("\n\tcannot connect to X server\n\n");
        }


        pxDisplay->Data = XDisplayString(pxDisplay->DisplayHandle);
        pxDisplay->Name = XDisplayName(pxDisplay->DisplayHandle); // if NULL, this is the atempted name what XOpen would use

        pxDisplay->ProtocolVersion = XProtocolVersion(pxDisplay->DisplayHandle); // for X11, it is 11
        pxDisplay->ProtocolRevision = XProtocolRevision(pxDisplay->DisplayHandle);

        pxDisplay->ServerVendor = XServerVendor(pxDisplay->DisplayHandle);
        pxDisplay->VendorRelease = XVendorRelease(pxDisplay->DisplayHandle);

        pxDisplay->ScreenDefaultID = DefaultScreen(pxDisplay->DisplayHandle);

        pxDisplay->ScreenListAmount = XScreenCount(pxDisplay->DisplayHandle);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "GUI",
            "X-System",
            "Successfly opened display (0x%p)\n"
            "%10s: %s, Data: %s\n"
            "%10s: %i.%i\n"
            "%10s: %s (Relase %i)\n"
            "%10s: %i, main:%i",
            pxDisplay->DisplayHandle,
            "Name", pxDisplay->Name, pxDisplay->Data,
            "Protocol", pxDisplay->ProtocolVersion, pxDisplay->ProtocolRevision,
            "Server", pxDisplay->ServerVendor, pxDisplay->VendorRelease,
            "Screens", pxDisplay->ScreenListAmount, pxDisplay->ScreenDefaultID
        );
#endif

        // Get default values

        for(size_t screenID = 0; screenID < pxDisplay->ScreenListAmount; ++screenID)
        {
            PXDisplayScreen* pxDisplayScreen = &pxDisplay->DisplayScreenList[screenID];

            pxDisplayScreen->Width = XDisplayWidth(pxDisplay->DisplayHandle, screenID);
            pxDisplayScreen->Height = XDisplayHeight(pxDisplay->DisplayHandle, screenID);
            pxDisplayScreen->Cells = XDisplayCells(pxDisplay->DisplayHandle, screenID);
            pxDisplayScreen->Planes = XDisplayPlanes(pxDisplay->DisplayHandle, screenID);
            pxDisplayScreen->WidthMM = XDisplayWidthMM(pxDisplay->DisplayHandle, screenID);
            pxDisplayScreen->HeightMM = XDisplayHeightMM(pxDisplay->DisplayHandle, screenID);

        }

        pxDisplay->WindowRootHandle = XDefaultRootWindow(pxDisplay->DisplayHandle); // Make windows root
    }









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
			PXGUIElementDrawFunction(GUISystem, PXGUIElement);


			PXGUIElementDrawRectangleFill();



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


















#elif OSWindows

    INITCOMMONCONTROLSEX initCommonControls;
    initCommonControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    initCommonControls.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&initCommonControls);

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem)
{
#if OSUnix
	const int resultID = XCloseDisplay(pxGUISystem->DisplayCurrent.DisplayHandle);
#elif OSWindows

#else
	return PXnursupported;
#endif

    PXGUISystemGlobalReference = PXNull;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIElementCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount)
{
    if(!(pxGUISystem && pxResourceCreateInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    //assert();

    PXGUIElementCreateInfo* pxGUIElementCreateInfo = &pxResourceCreateInfo->UIElement;
    PXGUIElement* pxGUIElement = *(PXGUIElement**)pxResourceCreateInfo->ObjectReference;

    pxGUIElement->Type = pxGUIElementCreateInfo->Type;
    pxGUIElement->InteractCallBack = pxGUIElementCreateInfo->InteractCallBack;
    pxGUIElement->InteractOwner = pxGUIElementCreateInfo->InteractOwner;
    pxGUIElement->Parent = pxGUIElementCreateInfo->UIElementParent;

    PXCopy(PXUIElementPosition, &pxGUIElementCreateInfo->Position, &pxGUIElement->Position);





#if 0
    const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

    //const char* format = PXEngineCreateTypeToString(pxEngineResourceCreateInfo->CreateType);
    if(PXUIElementTypeTreeViewItem == pxGUIElementCreateInfo->Type)
    {
      // PXGUIElement* const uiElementSource



        switch(pxGUIElementCreateInfo->Data.TreeViewItem.OwningObjectType)
        {
            case PXFileResourceTypeEmpty:
            {
                pxGUIElement->NameSize = PXTextPrintA(pxGUIElement->NameData, 128, "<%s>", pxGUIElementCreateInfo->Name);
                break;
            }
            case PXFileResourceTypeUI:
            {
                PXGUIElement* const uiElementSource = (PXGUIElement*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                const char* uiElementTypeName = PXUIElementTypeToString(uiElementSource->Type);
                const char* name = uiElementSource->NameData;

                if(name[0] == '\0')
                {
                    name = "**Unnamed**";
                }

                pxGUIElement->NameSize = PXTextPrintA(pxGUIElement->NameData, 128, "[%s] %s", uiElementTypeName, name);

                break;
            }
            case PXResourceTypeModel:
            {
                PXModel* const pxModel = (PXModel*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                pxGUIElement->NameSize = PXTextPrintA
                (
                    pxGUIElement->NameData,
                    128,
                    "[Model] %s ID:%i",
                    "---",//pxModel->Info.Name,
                    pxModel->Info.ID
                );

                break;
            }
            case PXFileResourceTypeRenderShader:
            {
                PXShaderProgram* const pxShaderProgram = (PXShaderProgram*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                pxGUIElement->NameSize = PXTextPrintA
                (
                    pxGUIElement->NameData,
                    128,
                    "[Shader] %s ID:%i",
                    "---",// pxShaderProgram->ResourceID.Name,
                    pxShaderProgram->Info.ID
                );

                break;
            }
            case PXResourceTypeImage:
            {
                PXImage* const pxImage = (PXImage*)pxGUIElementCreateInfo->Data.TreeViewItem.OwningObject;

                pxGUIElement->NameSize = PXTextPrintA
                (
                    pxGUIElement->NameData,
                    128,
                    "[Image] %ix%i",
                    pxImage->Width,
                    pxImage->Height
                );

                break;
            }
            default:
            {
                pxGUIElement->NameSize = PXTextPrintA(pxGUIElement->NameData, 128, "ERROR");

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

        pxGUIElement->NameSize = PXTextPrintA(pxGUIElement->NameData, 128, "%s", name);
    }
#endif


    // Resize

        PXGUIElementUpdateInfo sizeInfoAA;
        PXClear(PXGUIElementUpdateInfo, &sizeInfoAA);
        //sizeInfoAA.UIElement = *pxGUIElementCreateInfo->UIElement;
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

        PXUIElementPositionCalculcate(pxGUIElement, &pxUIElementPositionCalulcateInfo);


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

        switch(pxGUIElement->Type)
        {
            case PXUIElementTypeWindow:
            {
                PXGUIElementCreateWindowInfo* const pxGUIElementCreateWindowInfo = &pxGUIElementCreateInfo->Data.Window;

                pxGUIElementCreateWindowInfo->UIElementReference = pxGUIElement;

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



                // Create window
                {
                    int borderWidth = 0;

                    pxGUIElement->Info.WindowID = XCreateWindow
                    (
                        pxGUISystem->DisplayCurrent.DisplayHandle,
                        pxGUISystem->DisplayCurrent.WindowRootHandle,
                        pxUIElementPositionCalulcateInfo.X,
                        pxUIElementPositionCalulcateInfo.Y,
                        pxUIElementPositionCalulcateInfo.Width,
                        pxUIElementPositionCalulcateInfo.Height,
                        borderWidth,
                        visualInfo->depth,
                        InputOutput,
                        visualInfo->visual,
                        CWColormap | CWEventMask,
                        &setWindowAttributes
                    );
                    const PXBool sucessful = PXNull != pxGUIElement->Info.WindowID;

                    if(!sucessful)
                    {
#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingError,
                            "X-System",
                            "Window-Create",
                            "Failed!"
                        );
#endif
                        return PXActionFailedCreate;
                    }

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "X-System",
                        "Window-Create",
                        "X:%4i, Y:%4i, W:%4i, H:%4i ID:%i",
                        pxUIElementPositionCalulcateInfo.X,
                        pxUIElementPositionCalulcateInfo.Y,
                        pxUIElementPositionCalulcateInfo.Width,
                        pxUIElementPositionCalulcateInfo.Height,
                        pxGUIElement->Info.WindowID
                    );
#endif
                }

                // Attach to render engine
                {
                    const int mapResultID = XMapWindow(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.WindowID);
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
                    const PXBool setTextSuccess = PXGUIElementTextSet(pxGUISystem, pxGUIElement, pxGUIElementCreateWindowInfo->Title);
                }

                break;
            }
            default:
                break;
        }
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
    PXClear(XIEventMask, &eventmask);

    const PXSize maskLength = (XI_LASTEVENT + 7) / 8;
    char mask[maskLength];

    PXClearList(char, mask, maskLength);


    XISetMask(mask, XI_RawMotion);
    //XISetMask(mask, XI_RawButtonPress);
    //XISetMask(mask, XI_RawKeyPress);

    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = maskLength;
    eventmask.mask = mask;


    const int selectResultID = XISelectEvents(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUISystem->DisplayCurrent.WindowRootHandle, &eventmask, 1u);
    const PXBool success = PXNull != selectResultID;

    if(!success)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "X-System",
            "Event-Select",
            "ID:%i Failed",
            pxGUIElement->Info.WindowID
        );
#endif
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "X-System",
            "Event-Select",
            "ID:%i OK",
            pxGUIElement->Info.WindowID
        );
#endif
    }



    const int flushResultID = XFlush(pxGUISystem->DisplayCurrent.DisplayHandle);




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


    switch(pxGUIElement->Type)
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
                pxGUIElementCreateInfo->AvoidCreation = PXUIElementTypeButton == pxGUIElement->Parent->Type;

                if(pxGUIElementCreateInfo->AvoidCreation)
                {
                    pxGUIElement->Type = PXUIElementTypeButtonText;
                    pxGUIElement->Info.WindowID = PXNull;
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
     // pxGUIElement->Type == PXUIElementTypeText&&;


    if(!pxGUIElementCreateInfo->AvoidCreation)
    {
        HINSTANCE hInstance = PXNull;
        PXWindowID windowID = pxGUIElementCreateInfo->UIElementWindow ? pxGUIElementCreateInfo->UIElementWindow->Info.WindowID : PXNull;

        if(pxGUIElementCreateInfo->UIElementWindow)
        {
            hInstance = (HINSTANCE)GetWindowLongPtr(pxGUIElementCreateInfo->UIElementWindow->Info.WindowID, GWLP_HINSTANCE);
            //windowID = pxGUIElementCreateInfo->UIElementWindow->ID;
        }
        else
        {
            hInstance = GetModuleHandle(NULL);
        }

       // const HINSTANCE hInstance = GetModuleHandle(NULL);

        pxGUIElement->Info.WindowID = CreateWindowExA // Windows 2000, User32.dll, winuser.h
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
        const PXBool success = PXNull != pxGUIElement->Info.WindowID;


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

        PXDictionaryAdd(&pxGUISystem->ResourceManager->GUIElementLookup, &pxGUIElement->Info.WindowID, pxGUIElement);

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
            pxGUIElement->Info.WindowID,
            uielementName
        );
#endif


#if 0
        if(pxGUIElement->ID && pxGUIElementCreateInfo->UIElementParent)
        {
            PXGUIElement* parent = pxGUIElementCreateInfo->UIElementParent;

            HDWP resA = BeginDeferWindowPos(1);

            if(PXUIElementTypeButtonText == pxGUIElement->Type)
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    parent->ID,
                    pxGUIElement->ID,
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
                    pxGUIElement->ID,
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

    HDC xx = GetDC(pxGUIElement->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORSTATIC, xx, pxGUIElement->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));


    Pager_SetBkColor(pxGUIElement->ID, &colorAA);
#endif


    // SetTextColor(xx, colorAA);


     //LRESULT ww = SendMessageA(pxGUIElement->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR


   //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);


    // SendMessageA(pxGUIElement->ID, RB_SETTEXTCOLOR, 0, &colorAA);




#endif












    //-------------------------------------------------------------------------
    // POST-Update
    //-------------------------------------------------------------------------
    switch(pxGUIElement->Type)
    {
        case PXUIElementTypePanel:
        {


            break;
        }
        case PXUIElementTypeText:
        {
            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxGUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;

            pxUIElementUpdateInfo[1].UIElement = pxGUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyTextAllign;

           // PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeButtonText: // Local override for OSStyle buttons
        {
            PXGUIElement* pxButton = pxGUIElement->Parent;

            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxButton;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;
            pxUIElementUpdateInfo[0].Data.Text.Content = pxGUIElementCreateInfo->Data.Text.Content;

           // PXCopy(PXUIElementTextInfo, &pxGUIElement->TextInfo, &pxButton->TextInfo);

            PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 1);

            break;
        }
        case PXUIElementTypeButton:
        {
            PXGUIElementUpdateInfo pxUIElementUpdateInfo[2];
            PXClearList(PXGUIElementUpdateInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxGUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElement = pxGUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;


         //  PXGUIElementUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            PXUIElementTreeViewItemInfo* const pxUIElementTreeViewItemInfo = &pxGUIElementCreateInfo->Data.TreeViewItem;
            // Create ui item for a tree view

#if OSUnix
#elif OSWindows

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

           // item.item.iImage = 1;


            const char text[] = "[N/A]";

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                item.hParent = pxUIElementTreeViewItemInfo->ItemParent->Info.WindowID;
            }

            if(pxUIElementTreeViewItemInfo->TextDataOverride)
            {
                item.item.pszText = pxUIElementTreeViewItemInfo->TextDataOverride;
                item.item.cchTextMax = pxUIElementTreeViewItemInfo->TextSizeOverride;
            }
            else
            {
              //  item.item.pszText = pxGUIElement->NameData;
              //  item.item.cchTextMax = pxGUIElement->NameSize;
            }



            if(PXResourceTypeGUIElement == pxUIElementTreeViewItemInfo->OwningObjectType)
            {
                PXGUIElement* const uiElement = (PXGUIElement*)pxUIElementTreeViewItemInfo->OwningObject;

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

            HTREEITEM itemID = TreeView_InsertItem(pxUIElementTreeViewItemInfo->TreeView->Info.WindowID, &item);

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                TreeView_Expand
                (
                    pxUIElementTreeViewItemInfo->TreeView->Info.WindowID,
                    pxUIElementTreeViewItemInfo->ItemParent->Info.WindowID,
                    TVE_EXPAND
                );
            }

            pxGUIElement->Info.WindowID = itemID;
            pxUIElementTreeViewItemInfo->ItemHandle = itemID;
#endif

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


            // EM_SETBKGNDCOLOR(pxGUIElement->ID, RGB(10, 10, 10));
            // EM_SETTEXTCOLOR(pxGUIElement->ID, RGB(200, 200, 200));

            break;
        }
        case PXUIElementTypeScrollBar:
        {

            break;
        }
        case PXUIElementTypeTrackBar:
        {
#if OSUnix
#elif OSWindows

            HDC dc = GetDC(pxGUIElement->Info.WindowID);


            SetTextColor(dc, RGB(255, 0, 0));
            SetBkColor(dc, RGB(0, 0, 255));


            SendMessageA(pxGUIElement->Info.WindowID, TBM_SETTIPSIDE, TBTS_RIGHT, PXNull);


           // SendMessageA(pxGUIElement->ID, SET_BACKGROUND_COLOR, RGB(30, 30, 30), RGB(30, 30, 30));

           // SET_BACKGROUND_COLOR;

           // TreeView_SetBkColor(pxGUIElement->ID, RGB(30, 30, 30));

         //   TreeView_SetTextColor(pxGUIElement->ID, RGB(200, 200, 200));
#endif

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

            pxUIElementUpdateInfo[0].UIElement = pxGUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElement = pxGUIElement;
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
#if OSUnix
#elif OSWindows

            int sizeX = 16;
            int sizeY = 16;
            int amount = 11;

            TreeView_SetBkColor(pxGUIElement->Info.WindowID, RGB(30, 30, 30));

            TreeView_SetTextColor(pxGUIElement->Info.WindowID, RGB(200, 200, 200));


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

            TreeView_SetImageList(pxGUIElement->Info.WindowID, imageListHandle, TVSIL_NORMAL);
#endif

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
#if OSUnix
#elif OSWindows
            HDC pDC = GetDC(pxGUIElement->Info.WindowID);
            //SetBkMode(pDC, TRANSPARENT);
            SetBkColor(pDC, RGB(255, 0, 0));
            SetTextColor(pDC, RGB(0, 0, 255));

            PXUIElementTabPageInfo* const pxUIElementTabPageInfo = &pxGUIElementCreateInfo->Data.TabPage;



            pxGUIElement->ListEESize = pxGUIElementCreateInfo->Data.TabPage.TabPageSingleInfoAmount;
            PXNewList(PXGUIElement*, pxGUIElementCreateInfo->Data.TabPage.TabPageSingleInfoAmount, &pxGUIElement->ListEEData, PXNull);


            for(PXSize i = 0; i < pxUIElementTabPageInfo->TabPageSingleInfoAmount; ++i)
            {
                PXUIElementTabPageSingleInfo* const pxUIElementTabPageSingleInfo = &pxUIElementTabPageInfo->TabPageSingleInfoList[i];

                char buffer[64];
                PXTextPrintA(buffer, 64, "TabPage-%i-%s", i, pxUIElementTabPageSingleInfo->PageName);

                // Create a panel for each page, to contain all elements, so that we can hide and show all at once
                PXResourceCreateInfo pxResourceCreateInfo;
                PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
                pxResourceCreateInfo.Type = PXResourceTypeGUIElement;
                pxResourceCreateInfo.ObjectReference = pxUIElementTabPageSingleInfo->UIElement;
                pxResourceCreateInfo.UIElement.Type = PXUIElementTypePanel;
                pxResourceCreateInfo.UIElement.Name = buffer;
                pxResourceCreateInfo.UIElement.UIElementWindow = pxGUIElementCreateInfo->UIElementWindow;
                pxResourceCreateInfo.UIElement.UIElementParent = pxGUIElement;
                pxResourceCreateInfo.UIElement.BehaviourFlags = PXUIElementDecorative;
                pxResourceCreateInfo.UIElement.StyleFlagList = PXGUIElementStyleDefault;
                pxResourceCreateInfo.UIElement.Position.FlagListKeep = PXUIElementAllignLeft;
                pxResourceCreateInfo.UIElement.Position.MarginLeft = 0.005;
                pxResourceCreateInfo.UIElement.Position.MarginTop = 0.1;
                pxResourceCreateInfo.UIElement.Position.MarginRight = 0.005;
                pxResourceCreateInfo.UIElement.Position.MarginBottom = 0.02;
                pxResourceCreateInfo.UIElement.Data.Text.Content = buffer;

                PXResourceManagerAdd(pxGUISystem->ResourceManager,&pxResourceCreateInfo, 1);

                PXGUIElementCreate(pxGUISystem, &pxResourceCreateInfo, 1);


                pxGUIElement->ListEEData[i] = *pxUIElementTabPageSingleInfo->UIElement;


                // Add tabs for each day of the week.
                TCITEM tie;
                tie.mask = TCIF_TEXT | TCIF_IMAGE;
                tie.iImage = pxUIElementTabPageSingleInfo->ImageID;
                tie.pszText = pxUIElementTabPageSingleInfo->PageName;

                auto x = TabCtrl_InsertItem(pxGUIElement->Info.WindowID, i, &tie);

                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "TabControl-Add",
                    "Page added %s",
                    pxUIElementTabPageSingleInfo->PageName
                );
            }

            SendMessage(pxGUIElement->Info.WindowID, TCM_SETCURFOCUS, 0, 0);

#endif

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
            pxEngineStartInfo->Width = pxGUIElement->Position.Width;
            pxEngineStartInfo->Height = pxGUIElement->Position.Height;
            pxEngineStartInfo->UIElement = pxGUIElement;
            pxEngineStartInfo->Name = "UIElement-RenderFrame";
            pxEngineStartInfo->UseMouseInput = 1;

            PXEngineStart(pxEngine, pxEngineStartInfo);

#endif

            break;
        }

        case PXUIElementTypeWindow:
        {
            PXGUIElementCreateWindowInfo* const pxGUIElementCreateWindowInfo = &pxGUIElementCreateInfo->Data.Window;

            PXWindowTitleBarColorSet(pxGUIElement->Info.WindowID);

            // UpdateWindow(pxGUIElement->Info.WindowID);

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
        PXGUIElement* const pxGUIElement = pxGUIElementUpdateInfo->UIElement;

        if(!pxGUIElement)
        {
            continue;
        }

        switch(pxGUIElementUpdateInfo->Property)
        {
            case PXUIElementPropertyTextContent:
            {
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

                PXGUIElementTextSet(pxGUISystem, pxGUIElement, pxUIElementTextInfo->Content);

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
                PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows


                HDC xx = GetDC(pxGUIElement->Info.WindowID);

                LONG ww = SetWindowLongA(pxGUIElement->Info.WindowID, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

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
                PXGUIElementUpdateInfo sizeFetchInfo;
                PXClear(PXGUIElementUpdateInfo, &sizeFetchInfo);
                sizeFetchInfo.UIElement = pxGUIElementUpdateInfoList->WindowReference;
                sizeFetchInfo.Property = PXUIElementPropertySize;

                PXGUIElementFetch(pxGUISystem, &sizeFetchInfo, 1);

                PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                //PXWindowSizeInfo pxWindowSizeInfo;
                //PXWindowID pxWindowID = pxGUIElement->Parent ? pxGUIElement->Parent->ID : PXNull;

                //PXWindowSizeGet(PXNull, &pxWindowSizeInfo); // TODO: Problematic

                pxUIElementPositionCalulcateInfo.WindowWidth = sizeFetchInfo.Data.Size.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = sizeFetchInfo.Data.Size.Height;

                PXUIElementPositionCalculcate(pxGUIElement, &pxUIElementPositionCalulcateInfo);



                // If the window is an actual window, we will have bad allignment if we use MoveWindow
                //if(pxGUIElement->Type != PXUIElementTypeWindow)

                if(0)
                {
                    const PXBool success = MoveWindow
                    (
                        pxGUIElement->Info.WindowID,
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
                if(PXUIElementTypeRenderFrame == pxGUIElement->Type)
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

#if OSUnix
#elif OSWindows

                PXInt32U stepsConverted = progressBar->Percentage * 100;
                SendMessageA(pxGUIElement->Info.WindowID, PBM_SETPOS, stepsConverted, 0);
#endif

                break;
            }
            case PXUIElementPropertyProgressbarBarColor:
            {
                PXUIElementProgressBarInfo* const progressBar = &pxGUIElementUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                COLORREF color = RGB(progressBar->BarColor.Red, progressBar->BarColor.Green, progressBar->BarColor.Blue);
                SendMessageA(pxGUIElement->Info.WindowID, PBM_SETBARCOLOR, 0, color);
#endif

                break;
            }
            case PXUIElementPropertyVisibility:
            {
#if OSUnix
#elif OSWindows

                PXBool show = pxGUIElementUpdateInfo->Show;
                int showID = show ? SW_SHOW : SW_HIDE;

                const PXBool isWindowValid = IsWindow(pxGUIElement->Info.WindowID);

                if(!isWindowValid)
                {
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "GUI",
                        "Visibility",
                        "%20s (0x%p), Invalid",
                        "",//pxGUIElement->NameData,
                        pxGUIElement->Info.WindowID
                    );

                    break;
                }

                if(show)
                {
                    pxGUIElement->Info.Flags |= PXEngineResourceInfoVisble;
                }
                else
                {
                    pxGUIElement->Info.Flags &= ~PXEngineResourceInfoVisble;
                }




                // EnableWindow(pxGUIElement->ID, show);

                ShowWindow(pxGUIElement->Info.WindowID, showID);
                BOOL res = ShowWindow(pxGUIElement->Info.WindowID, showID); // Twice to prevent some errors

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
                    pxGUIElement->Info.WindowID,
                    show ? "Show" : "Hide",
                    res ? "OK" : "FAIL"
                );
#endif


                // Own solution
                {
                    PXGUIElementChildListEnumerate(pxGUISystem, pxGUIElement, show);



                }


                const BOOL success = EnumChildWindows
                (
                    pxGUIElement->Info.WindowID,
                    PXWindowEnumChildProc,
                    &showID
                );
#endif

                break;
            }
            case PXUIElementPropertyComboBoxAdd:
            {
                PXUIElementComboBoxInfo* const pxUIElementComboBoxInfo = &pxGUIElementUpdateInfo->Data.ComboBox;

#if OSUnix
#elif OSWindows

                for(size_t i = 0; i < pxUIElementComboBoxInfo->DataListAmount; i++)
                {
                    char* name = pxUIElementComboBoxInfo->DataList[i];

                    ComboBox_AddString(pxGUIElement->Info.WindowID, name);
                }

                ComboBox_SetCurSel(pxGUIElement->Info.WindowID, 0);
#endif

                break;
            }
            case PXUIElementPropertyItemAdd:
            {
                PXUIElementItemInfo* pxUIElementItemInfo = &pxGUIElementUpdateInfo->Data.TreeViewItem;

#if OSUnix
#elif OSWindows

                TVINSERTSTRUCT item;
                PXClear(TVINSERTSTRUCT, &item);

                const char text[] = "[N/A]";

                item.item.pszText = text;
                item.item.cchTextMax = sizeof(text);
                item.hInsertAfter = TVI_ROOT;
                item.item.mask = TVIF_TEXT;

                TreeView_InsertItem(pxGUIElement->Info.WindowID, &item);

#endif


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
        PXGUIElement* const pxGUIElement = pxGUIElementUpdateInfo->UIElement;

        switch(pxGUIElementUpdateInfo->Property)
        {
            case PXUIElementPropertySizeParent:
            {
                PXWindowSizeInfo* pxWindowSizeInfo = &pxGUIElementUpdateInfo->Data.Size;

               // const PXBool hasParent = pxGUIElement ? pxGUIElement->Parent : PXFalse;

                const PXBool hasParent = PXNull != pxGUIElementUpdateInfoList->WindowReference;

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
               // ScreenCount();

               // ScreenOfDisplay();

                // if "display" is null, DefaultScreenOfDisplay will SEGFAULT

                if(pxGUISystem->DisplayCurrent.DisplayHandle)
                {
                    Screen* const xScreen = DefaultScreenOfDisplay(pxGUISystem->DisplayCurrent.DisplayHandle); // X11

                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = WidthOfScreen(xScreen);
                    pxWindowSizeInfo->Height = HeightOfScreen(xScreen);
                }
                else
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = 800;
                    pxWindowSizeInfo->Height = 600;
                }


#elif PXOSWindowsDestop

                RECT rect;

                //const PXBool result = GetWindowRect(pxGUIElement->ID, &rect); // Windows 2000, User32.dll, winuser.h
                const PXBool result = GetClientRect(pxGUIElement->Info.WindowID, &rect); // Windows 2000, User32.dll, winuser.h

                if(!result)
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = 0;
                    pxWindowSizeInfo->Height = 0;

                    return PXActionRefusedNotFound;
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

PXActionResult PXAPI PXGUIElementRelease(PXGUIElement* const pxGUIElement)
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

    PXDictionary* const uiElementLookup = &pxGUISystem->ResourceManager->GUIElementLookup;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

    for(PXSize i = 0; i < uiElementLookup->EntryAmountCurrent; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXGUIElement* uiElement = PXNull;

        PXDictionaryIndex(uiElementLookup, i, &pxDictionaryEntry);

        uiElement = *(PXGUIElement**)pxDictionaryEntry.Value;

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

void PXAPI PXWindowUpdate(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement)
{
    if(!pxGUIElement)
    {
        return;
    }

    if(!pxGUIElement->Info.WindowID)
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




    XLockDisplay(pxGUISystem->DisplayCurrent.DisplayHandle);

    XNextEvent(pxGUISystem->DisplayCurrent.DisplayHandle, &windowEvent);

    XUnlockDisplay(pxGUISystem->DisplayCurrent.DisplayHandle);

    PXWindowEventHandler(pxGUIElement, &windowEvent);

#elif PXOSWindowsDestop

    for (;;)
    {
        MSG message;

        const PXBool peekResult = PeekMessage(&message, pxGUIElement->Info.WindowID, 0, 0, PM_NOREMOVE); // Windows 2000, User32.dll, winuser.h

        if (!peekResult)
        {
            break; // Stop, no more messages
        }

        const PXBool messageResult = GetMessage(&message, pxGUIElement->Info.WindowID, 0, 0); // Windows 2000, User32.dll, winuser.h

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

PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindowID pxWindowID)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    PXLibrary pyLibrary;

    // Open lib
    {
        const PXActionResult libOpenResult = PXLibraryOpenA(&pyLibrary, "DWMAPI.DLL");

        if (PXActionSuccessful != libOpenResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    typedef HRESULT(WINAPI* PXDwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, _In_reads_bytes_(cbAttribute) LPCVOID pvAttribute, DWORD cbAttribute);

    PXDwmSetWindowAttribute pxDwmSetWindowAttribute;

    PXBool hasFunction = PXLibraryGetSymbolA(&pyLibrary, &pxDwmSetWindowAttribute, "DwmSetWindowAttribute");

    if (!hasFunction)
    {
        PXLibraryClose(&pyLibrary);

        return PXActionRefusedNotSupportedByOperatingSystem;
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

    return PXActionRefusedNotSupportedByOperatingSystem;

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
   // glXSwapBuffers(window->DisplayCurrent, window->ID);
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

PXActionResult PXAPI PXGUIElementDrawText(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, PXText* const pxText)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
            return PXGUIElementDrawTextA(pxGUISystem, pxGUIElement, pxText->TextA, pxText->SizeUsed);

        case TextFormatUNICODE:
            return PXGUIElementDrawTextA(pxGUISystem, pxGUIElement, pxText->TextW, pxText->SizeUsed);

        default:
            return TextFormatInvalid;
    }
}

PXActionResult PXAPI PXGUIElementDrawTextA(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const char* const text, const PXSize textSize)
{
#if OSUnix
    // For ANSI and UTF-8 strings
    //const int resultID = XDrawString(pxGUISystem->DisplayCurrent.DisplayHandle, Drawable d, GC gc, int x, int y, char* string, int length);

#elif OSWindows

    RECT rectangle;
    UINT format;

    DRAWTEXTPARAMS drawinfo;
    PXClear(DRAWTEXTPARAMS, &drawinfo);

    const int nextHeight = DrawTextExA(pxGUIElement->DeviceContextHandle, text, textSize, &rectangle, format, &drawinfo); // Windows 2000, User32.dll, winuser.h
    const PXBool success = 0 != nextHeight;
    const PXActionResult result = PXWindowsErrorCurrent(success);

    return result;

#else
    return NotSupport;
#endif
}

PXActionResult PXAPI PXGUIElementDrawTextW(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const wchar_t* const text, const PXSize textSize)
{
#if OSUnix
    // For UNICODE
   // const int resultID = XDrawString16(Display * display, Drawable d, GC gc, int x, int y, XChar2b * string, int length);

#elif OSWindows

    RECT rectangle;
    UINT format;

    DRAWTEXTPARAMS drawinfo;
    PXClear(DRAWTEXTPARAMS, &drawinfo);

    const int nextHeight = DrawTextW(pxGUIElement->DeviceContextHandle, text, textSize, &rectangle, format, &drawinfo); // Windows 2000, User32.dll, winuser.h
    const PXBool success = 0 != nextHeight;
    const PXActionResult result = PXWindowsErrorCurrent(success);

    return result;

#else
    return NotSupport;
#endif
}

PXActionResult PXAPI PXGUIElementDrawPoint(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y)
{
#if OSUnix
    const int resultID = 0;//XDrawPoint(Display *display, Drawable d, GC gc, int x, int y);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
	return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXGUIElementDrawPoints(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix
    const int resultID = 0;//XDrawPoints(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
	return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXActionResult PXAPI PXGUIElementDrawLine(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x1, const int y1, const int x2, const int y2)
{
#if OSUnix
	const resultID = 0;//XDrawLine(Display *display, Drawable d, GC gc, x1, y1, x2, y2);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
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

PXActionResult PXAPI PXGUIElementDrawLines(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix
    const resultID = 0;//XDrawLines(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
    return result;
#elif OSWindows
	const BOOL success = PolylineTo(pxGUIElement->DeviceContextHandle, 0,  0);
#else
	return PXNotSupport;
#endif
}

PXActionResult PXAPI PXGUIElementDrawRectangle(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix
    const resultID = 0;//XDrawRectangle(Display *display, Drawable d, GC gc, int x, int y, unsigned int width, unsigned int height);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
    return result;
#elif OSWindows
	const BOOL success = Rectangle(pxGUIElement->DeviceContextHandle, 0, 0, 0, 0);
#else
	return PXNotSupport;
#endif
}

PXActionResult PXAPI PXGUIElementDrawRectangleRounded(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height)
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

PXActionResult PXAPI PXGUIElementMove(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y)
{
#if OSUnix
    const int resultID = 0;//XMoveWindow(pxGUISystem->DisplayHandle, Window w, x, y);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
    return result;
#elif PXOSWindowsDestop
    const PXBool success = MoveWindow(pxGUIElement->Info.WindowID, 0, 0, 0, 0, PXTrue); // Windows 2000, User32.dll

    return PXFalse;
#endif
}

PXActionResult PXAPI PXGUIElementResize(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int width, const int height)
{
#if OSUnix
	const int resultID = 0;//XResizeWindow(Display *display, Window w, width, height);
	const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);

    return result;
#elif PXOSWindowsDestop
    const PXBool success = MoveWindow(pxGUIElement->Info.WindowID, 0, 0, width, height, PXTrue); // Windows 2000, User32.dll

    return PXFalse;
#else
	return PXNotsupported;
#endif
}

PXActionResult PXAPI PXGUIElementMoveAndResize(PXGUISystem* const pxGUISystem, PXGUIElement* const pxGUIElement, const int x, const int y, const int width, const int height)
{
#if OSUnix
    const int resultID = 0;//XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned int width, unsigned int height);
    const PXActionResult result = PXGUIElementErrorFromXSystem(resultID);
    return PXFalse;
#elif PXOSWindowsDestop
	const PXBool success = MoveWindow(pxGUIElement->Info.WindowID, 0, 0, width, height, PXTrue); // Windows 2000, User32.dll, winuser.h

    return PXFalse;
#else
	return NotSupprort;
#endif
}

#if OSUnix
PXActionResult PXAPI PXGUIElementErrorFromXSystem(const int xSysstemErrorID)
{
	switch(xSysstemErrorID)
	{
		case BadValue: return PXActionInvalid; // input is not valid
		case BadWindow: return PXActionInvalid; // object id invalid

 // BadAlloc, BadColor, BadCursor, BadMatch, BadPixmap, BadValue, and BadWindow

		default:
			return PXActionInvalid;
	}
}
#endif

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

#include "PXWindow.h"

#if PXWindowUSE

#include <stdio.h>

#include <OS/Memory/PXMemory.h>
#include <OS/Hardware/PXMonitor.h>
#include <OS/Async/PXAwait.h>
#include <Media/PXText.h>
#include <OS/Graphic/PXGraphic.h>

#include <Math/PXMath.h>

#if OSUnix

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

#define DefautPositionX CW_USEDEFAULT
#define DefautPositionY CW_USEDEFAULT

#endif

PXWindow* currentWindow = 0;

#if OSWindows

typedef enum PXWindowEventType_
{
    WindowEventInvalid,
    WindowEventNoMessage,
    WindowEventCreate,
    WindowEventDestroy,
    WindowEventMove,
    WindowEventSize,
    WindowEventActivate,
    WindowEventRedrawSet,
    WindowEventFocusSet,
    WindowEventFocusKill,
    WindowEventEnable,
    WindowEventTextSet,
    WindowEventTextGet,
    WindowEventTextGetLength,
    WindowEventPaint,
    WindowEventClose,
    WindowEventSessionQuerryEnd,
    WindowEventSessionEnd,
    WindowEventQuerryOpen,
    WindowEventQuit,
    WindowEventBackgroundErase,
    WindowEventSystemColorChange,
    WindowEventShowWindow,
    WindowEventIconChange,
    WindowEventSettingChange,
    WindowEventDeviceModeChange,
    WindowEventActivateApp,
    WindowEventFontChange,
    WindowEventTimeChange,
    WindowEventCancelMode,
    WindowEventCursorSet,
    WindowEventMouseActivate,
    WindowEventChildActivate,
    WindowEventQueueSync,
    WindowEventSizeChange,
    WindowEventIconPaint,
    WindowEventIconBackgroundErase,
    WindowEventDialogControlNext,
    WindowEventSPOOLERSTATUS,
    WindowEventItemDraw,
    WindowEventItemMeasure,
    WindowEventItemDelete,
    WindowEventVKEYTOITEM,
    WindowEventCHARTOITEM,
    WindowEventFontSet,
    WindowEventFontGet,
    WindowEventSETHOTKEY,
    WindowEventGETHOTKEY,
    WindowEventQUERYDRAGICON,
    WindowEventCOMPAREITEM,
    WindowEventGETOBJECT,
    WindowEventCOMPACTING,
    WindowEventCOMMNOTIFY,
    WindowEventWINDOWPOSCHANGING,
    WindowEventWINDOWPOSCHANGED,
    WindowEventPOWER,
    WindowEventCOPYDATA,
    WindowEventCANCELJOURNAL,
    WindowEventNOTIFY,
    WindowEventINPUTLANGCHANGEREQUEST,
    WindowEventINPUTLANGCHANGE,
    WindowEventTCARD,
    WindowEventHELP,
    WindowEventUSERCHANGED,
    WindowEventNOTIFYFORMAT,
    WindowEventCONTEXTMENU,
    WindowEventSTYLECHANGING,
    WindowEventSTYLECHANGED,
    WindowEventDISPLAYCHANGE,
    WindowEventGETICON,
    WindowEventSETICON,
    WindowEventNCCREATE,
    WindowEventNCDESTROY,
    WindowEventNCCALCSIZE,
    WindowEventNCHITTEST,
    WindowEventNCPAINT,
    WindowEventNCACTIVATE,
    WindowEventGETDLGCODE,
    WindowEventSYNCPAINT,
    WindowEventNCMOUSEMOVE,
    WindowEventNCLBUTTONDOWN,
    WindowEventNCLBUTTONUP,
    WindowEventNCLBUTTONDBLCLK,
    WindowEventNCRBUTTONDOWN,
    WindowEventNCRBUTTONUP,
    WindowEventNCRBUTTONDBLCLK,
    WindowEventNCMBUTTONDOWN,
    WindowEventNCMBUTTONUP,
    WindowEventNCMBUTTONDBLCLK,
    WindowEventNCXBUTTONDOWN,
    WindowEventNCXBUTTONUP,
    WindowEventNCXBUTTONDBLCLK,
    WindowEventINPUT_DEVICE_CHANGE,
    WindowEventINPUT,
    WindowEventKEYFIRST,
    WindowEventKEYDOWN,
    WindowEventKEYUP,
    WindowEventCHAR,
    WindowEventDEADCHAR,
        WindowEventSYSKEYDOWN,
        WindowEventSYSKEYUP,
        WindowEventSYSCHAR,
        WindowEventSYSDEADCHAR,
        WindowEventUNICHAR,
        WindowEventKEYLAST,
    //UNICODE_NOCHAR,
        WindowEventIME_STARTCOMPOSITION,
        WindowEventIME_ENDCOMPOSITION,
        WindowEventIME_COMPOSITION,
        WindowEventIME_KEYLAST,
        WindowEventINITDIALOG,
        WindowEventCOMMAND,
        WindowEventSYSCOMMAND,
        WindowEventTIMER,
        WindowEventHSCROLL,
        WindowEventVSCROLL,
        WindowEventINITMENU,
        WindowEventINITMENUPOPUP,
        WindowEventGESTURE,
        WindowEventGESTURENOTIFY,
        WindowEventMENUSELECT,
        WindowEventMENUCHAR,
        WindowEventENTERIDLE,
        WindowEventMENURBUTTONUP,
        WindowEventMENUDRAG,
        WindowEventMENUGETOBJECT,
        WindowEventUNINITMENUPOPUP,
        WindowEventMENUCOMMAND,
        WindowEventCHANGEUISTATE,
        WindowEventUPDATEUISTATE,
        WindowEventQUERYUISTATE,
        WindowEventCTLCOLORMSGBOX,
        WindowEventCTLCOLOREDIT,
        WindowEventCTLCOLORLISTBOX,
        WindowEventCTLCOLORBTN,
        WindowEventCTLCOLORDLG,
        WindowEventCTLCOLORSCROLLBAR,
        WindowEventCTLCOLORSTATIC,
        WindowEventGETHMENU,
        WindowEventMOUSEFIRST,
        WindowEventMOUSEMOVE,
        WindowEventLBUTTONDOWN,
        WindowEventLBUTTONUP,
        WindowEventLBUTTONDBLCLK,
        WindowEventRBUTTONDOWN,
        WindowEventRBUTTONUP,
        WindowEventRBUTTONDBLCLK,
        WindowEventMBUTTONDOWN,
        WindowEventMBUTTONUP,
        WindowEventMBUTTONDBLCLK,
        WindowEventMOUSEWHEEL,
        WindowEventXBUTTONDOWN,
        WindowEventXBUTTONUP,
        WindowEventXBUTTONDBLCLK,
        WindowEventMOUSEHWHEEL,
        WindowEventMOUSELAST,
        WindowEventPARENTNOTIFY,
        WindowEventENTERMENULOOP,
        WindowEventEXITMENULOOP,
        WindowEventNEXTMENU,
        WindowEventSIZING,
        WindowEventCAPTURECHANGED,
        WindowEventMOVING,
        WindowEventPOWERBROADCAST,
        WindowEventDEVICECHANGE,
        WindowEventMDICREATE,
        WindowEventMDIDESTROY,
        WindowEventMDIACTIVATE,
        WindowEventMDIRESTORE,
        WindowEventMDINEXT,
        WindowEventMDIMAXIMIZE,
        WindowEventMDITILE,
        WindowEventMDICASCADE,
        WindowEventMDIICONARRANGE,
        WindowEventMDIGETACTIVE,
        WindowEventMDISETMENU,
        WindowEventENTERSIZEMOVE,
        WindowEventEXITSIZEMOVE,
        WindowEventDROPFILES,
        WindowEventMDIREFRESHMENU,
        WindowEventPOINTERDEVICECHANGE,
        WindowEventPOINTERDEVICEINRANGE,
        WindowEventPOINTERDEVICEOUTOFRANGE,
        WindowEventTOUCH,
        WindowEventNCPOINTERUPDATE,
        WindowEventNCPOINTERDOWN,
        WindowEventNCPOINTERUP,
        WindowEventPOINTERUPDATE,
        WindowEventPOINTERDOWN,
        WindowEventPOINTERUP,
        WindowEventPOINTERENTER,
        WindowEventPOINTERLEAVE,
        WindowEventPOINTERACTIVATE,
        WindowEventPOINTERCAPTURECHANGED,
        WindowEventTOUCHHITTESTING,
        WindowEventPOINTERWHEEL,
        WindowEventPOINTERHWHEEL,
        WindowEventPOINTERROUTEDTO,
        WindowEventPOINTERROUTEDAWAY,
        WindowEventPOINTERROUTEDRELEASED,
        WindowEventIME_SETCONTEXT,
        WindowEventIME_NOTIFY,
        WindowEventIME_CONTROL,
        WindowEventIME_COMPOSITIONFULL,
        WindowEventIME_SELECT,
        WindowEventIME_CHAR,
        WindowEventIME_REQUEST,
        WindowEventIME_KEYDOWN,
        WindowEventIME_KEYUP,
        WindowEventMOUSEHOVER,
        WindowEventMOUSELEAVE,
        WindowEventNCMOUSEHOVER,
        WindowEventNCMOUSELEAVE,
        WindowEventWTSSESSION_CHANGE,
        WindowEventTABLET_FIRST,
        WindowEventTABLET_LAST,
        WindowEventDPICHANGED,
        WindowEventDPICHANGED_BEFOREPARENT,
        WindowEventDPICHANGED_AFTERPARENT,
        WindowEventGETDPISCALEDSIZE,
        WindowEventCUT,
        WindowEventCOPY,
        WindowEventPASTE,
        WindowEventCLEAR,
        WindowEventUNDO,
        WindowEventRENDERFORMAT,
        WindowEventRENDERALLFORMATS,
        WindowEventDESTROYCLIPBOARD,
        WindowEventDRAWCLIPBOARD,
        WindowEventPAINTCLIPBOARD,
        WindowEventVSCROLLCLIPBOARD,
        WindowEventSIZECLIPBOARD,
        WindowEventASKCBFORMATNAME,
        WindowEventCHANGECBCHAIN,
        WindowEventHSCROLLCLIPBOARD,
        WindowEventQUERYNEWPALETTE,
        WindowEventPALETTEISCHANGING,
        WindowEventPALETTECHANGED,
        WindowEventHOTKEY,
        WindowEventPRINT,
        WindowEventPRINTCLIENT,
        WindowEventAPPCOMMAND,
        WindowEventTHEMECHANGED,
        WindowEventCLIPBOARDUPDATE,
        WindowEventDWMCOMPOSITIONCHANGED,
        WindowEventDWMNCRENDERINGCHANGED,
        WindowEventDWMCOLORIZATIONCOLORCHANGED,
        WindowEventDWMWINDOWMAXIMIZEDCHANGE,
        WindowEventDWMSENDICONICTHUMBNAIL,
        WindowEventDWMSENDICONICLIVEPREVIEWBITMAP,
        WindowEventGETTITLEBARINFOEX,
        WindowEventHANDHELDFIRST,
        WindowEventHANDHELDLAST,
        WindowEventAFXFIRST,
        WindowEventAFXLAST,
        WindowEventPENWINFIRST,
        WindowEventPENWINLAST,
        WindowEventAPP
}
PXWindowEventType;

PXWindowEventType ToWindowEventType(const unsigned int windowEventID)
{
    switch(windowEventID)
    {
        case WM_NULL: return WindowEventNoMessage;
        case WM_CREATE: return WindowEventCreate;
        case WM_DESTROY: return WindowEventDestroy;
        case WM_MOVE: return WindowEventMove;
        case WM_SIZE: return WindowEventSize;
        case WM_ACTIVATE: return WindowEventActivate;
        case WM_SETFOCUS: return WindowEventFocusSet;
        case WM_KILLFOCUS: return WindowEventFocusKill;
        case WM_ENABLE: return WindowEventEnable;
        case WM_SETREDRAW: return WindowEventRedrawSet;
        case WM_SETTEXT: return WindowEventTextSet;
        case WM_GETTEXT: return WindowEventTextGet;
        case WM_GETTEXTLENGTH: return WindowEventTextGetLength;
        case WM_PAINT: return WindowEventPaint;
        case WM_CLOSE: return WindowEventClose;
        case WM_QUERYENDSESSION: return WindowEventSessionQuerryEnd;
        case WM_QUERYOPEN: return WindowEventQuerryOpen;
        case WM_ENDSESSION: return WindowEventSessionEnd;
        case WM_QUIT: return WindowEventQuit;
        case WM_ERASEBKGND: return WindowEventBackgroundErase;
        case WM_SYSCOLORCHANGE: return WindowEventSystemColorChange;
        case WM_SHOWWINDOW: return WindowEventShowWindow;
        case WM_WININICHANGE: return WindowEventIconChange;
            //case WM_SETTINGCHANGE                WM_WININICHANGE
        case WM_DEVMODECHANGE: return WindowEventDeviceModeChange;
        case WM_ACTIVATEAPP: return WindowEventActivateApp;
        case WM_FONTCHANGE: return WindowEventFontChange;
        case WM_TIMECHANGE: return WindowEventTimeChange;
        case WM_CANCELMODE: return WindowEventCancelMode;
        case WM_SETCURSOR: return WindowEventCursorSet;
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
        case WM_NOTIFY: return WindowEventNOTIFY;
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
        case WM_INPUT: return WindowEventINPUT;
            // ?? case WM_KEYFIRST: return WindowEventKEYFIRST;
        case WM_KEYDOWN: return WindowEventKEYDOWN;
        case WM_KEYUP: return WindowEventKEYUP;
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
        case WM_COMMAND: return WindowEventCOMMAND;
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

        case WM_LBUTTONDOWN: return WindowEventLBUTTONDOWN;
        case WM_LBUTTONUP: return WindowEventLBUTTONUP;
        case WM_LBUTTONDBLCLK: return WindowEventLBUTTONDBLCLK;
        case WM_RBUTTONDOWN: return WindowEventRBUTTONDOWN;
        case WM_RBUTTONUP: return WindowEventRBUTTONUP;

        case WM_RBUTTONDBLCLK: return WindowEventRBUTTONDBLCLK;
        case WM_MBUTTONDOWN: return WindowEventMBUTTONDOWN;
        case WM_MBUTTONUP: return WindowEventMBUTTONUP;
        case WM_MBUTTONDBLCLK: return WindowEventMBUTTONDBLCLK;
        case WM_MOUSEWHEEL: return WindowEventMOUSEWHEEL;

        case WM_XBUTTONDOWN: return WindowEventXBUTTONDOWN;
        case WM_XBUTTONUP: return WindowEventXBUTTONUP;
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

        default: return WindowEventInvalid;
}
}

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
LRESULT CALLBACK PXWindowEventHandler(const HWND windowsID, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    const PXWindowEventType windowEventType = ToWindowEventType(eventID);
    PXWindow* const window = PXWindowLookupFind(windowsID);

    if(!window)
    {
        return DefWindowProc(windowsID, eventID, wParam, lParam);
    }

    switch(windowEventType)
    {
        case WindowEventNoMessage:
            break;
        case WindowEventCreate: // Gets called inside the "CreateWindow" function.
        {
            // Do nothíng here, as it's too soon to regard the window as 'created'

            return DefWindowProc(windowsID, eventID, wParam, lParam);
        }
        case WindowEventDestroy:
        {
            window->IsRunning = PXFalse;
            break;
        }
        case WindowEventMove:
            break;
        case WindowEventSize:
        {
#if 1
            window->Width = LOWORD(lParam);
            window->Height = HIWORD(lParam);

            window->HasSizeChanged = PXYes;

            PXFunctionInvoke(window->WindowSizeChangedCallBack, window->EventReceiver, window);
#endif

            break;
        }
        case WindowEventActivate:
            break;
        case WindowEventRedrawSet:
            break;
        case WindowEventFocusSet:
            break;
        case WindowEventFocusKill:
            break;
        case WindowEventEnable:
            break;
        case WindowEventTextSet:
            break;
        case WindowEventTextGet:
            break;
        case WindowEventTextGetLength:
            break;
        case WindowEventPaint:
        {
            break;
        }      
        case WindowEventClose:
        {
            PXBool closeWindow = 0;

            PXFunctionInvoke(window->WindowClosingCallBack, window->EventReceiver, window, &closeWindow);

            if(closeWindow)
            {
                PXFunctionInvoke(window->WindowClosedCallBack, window->EventReceiver, window);

                const LRESULT result = DefWindowProc(windowsID, WM_CLOSE, wParam, lParam);

                window->IsRunning = PXFalse;

                return result;
            }

            break;
        }
        case WindowEventSessionQuerryEnd:
            break;
        case WindowEventSessionEnd:
            break;
        case WindowEventQuerryOpen:
            break;
        case WindowEventQuit:
            break;
        case WindowEventBackgroundErase:
            break;
        case WindowEventSystemColorChange:
            break;
        case WindowEventShowWindow:
            break;
        case WindowEventIconChange:
            break;
        case WindowEventSettingChange:
            break;
        case WindowEventDeviceModeChange:
            break;
        case WindowEventActivateApp:
            break;
        case WindowEventFontChange:
            break;
        case WindowEventTimeChange:
            break;
        case WindowEventCancelMode:
            break;
        case WindowEventCursorSet:
        {
            const HWND windowsHandle = (HWND)wParam;
            const WORD hitTestResult = LOWORD(lParam);
            const WORD sourceMessage = HIWORD(lParam);
            const unsigned char showCursor = !(window->CursorModeCurrent == PXWindowCursorInvisible || window->CursorModeCurrent == PXWindowCursorLockAndHide);

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
        case WindowEventMouseActivate:
            break;
        case WindowEventChildActivate:
            break;
        case WindowEventQueueSync:
            break;

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
        case WindowEventIconPaint:
            break;
        case WindowEventIconBackgroundErase:
            break;
        case WindowEventDialogControlNext:
            break;
        case WindowEventSPOOLERSTATUS:
            break;
        case WindowEventItemDraw:
            break;
        case WindowEventItemMeasure:
            break;
        case WindowEventItemDelete:
            break;
        case WindowEventVKEYTOITEM:
            break;
        case WindowEventCHARTOITEM:
            break;
        case WindowEventFontSet:
            break;
        case WindowEventFontGet:
            break;
        case WindowEventSETHOTKEY:
            break;
        case WindowEventGETHOTKEY:
            break;
        case WindowEventQUERYDRAGICON:
            break;
        case WindowEventCOMPAREITEM:
            break;
        case WindowEventGETOBJECT:
            break;
        case WindowEventCOMPACTING:
            break;
        case WindowEventCOMMNOTIFY:
            break;
        case WindowEventWINDOWPOSCHANGING:
            break;
        case WindowEventWINDOWPOSCHANGED:
            break;
        case WindowEventPOWER:
            break;
        case WindowEventCOPYDATA:
            break;
        case WindowEventCANCELJOURNAL:
            break;
        case WindowEventNOTIFY:
            break;
        case WindowEventINPUTLANGCHANGEREQUEST:
            break;
        case WindowEventINPUTLANGCHANGE:
            break;
        case WindowEventTCARD:
            break;
        case WindowEventHELP:
            break;
        case WindowEventUSERCHANGED:
            break;
        case WindowEventNOTIFYFORMAT:
            break;
        case WindowEventCONTEXTMENU:
            break;
        case WindowEventSTYLECHANGING:
            break;
        case WindowEventSTYLECHANGED:
            break;
        case WindowEventDISPLAYCHANGE:
            break;
        case WindowEventGETICON:
            break;
        case WindowEventSETICON:
            break;
        case WindowEventNCCREATE:
            break;
        case WindowEventNCDESTROY:
            break;
        case WindowEventNCCALCSIZE:
            break;
        case WindowEventNCHITTEST:
            break;
        case WindowEventNCPAINT:
            break;
        case WindowEventNCACTIVATE:
            break;
        case WindowEventGETDLGCODE:
            break;
        case WindowEventSYNCPAINT:
            break;
        case WindowEventNCMOUSEMOVE:
            break;
        case WindowEventNCLBUTTONDOWN:
            break;
        case WindowEventNCLBUTTONUP:
            break;
        case WindowEventNCLBUTTONDBLCLK:
            break;
        case WindowEventNCRBUTTONDOWN:
            break;
        case WindowEventNCRBUTTONUP:
            break;
        case WindowEventNCRBUTTONDBLCLK:
            break;
        case WindowEventNCMBUTTONDOWN:
            break;
        case WindowEventNCMBUTTONUP:
            break;
        case WindowEventNCMBUTTONDBLCLK:
            break;
        case WindowEventNCXBUTTONDOWN:
            break;
        case WindowEventNCXBUTTONUP:
            break;
        case WindowEventNCXBUTTONDBLCLK:
            break;
        case WindowEventINPUT_DEVICE_CHANGE:
            break;
        case WindowEventINPUT:
        {
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

                    const UINT result = GetRawInputData(handle, uiCommand, &rawInput, &rawInputSize, sizeof(RAWINPUTHEADER));
                    const PXBool sucessful = result != -1;

                    if(sucessful)
                    {
#if UseRawMouseData
                        if(rawInput.header.dwType == RIM_TYPEMOUSE)
                        {
                            PXInt32S positionX = 0;
                            PXInt32S positionY = 0;
                           
                            PXInt32S deltaX = rawInput.data.mouse.lLastX;
                            PXInt32S deltaY = rawInput.data.mouse.lLastY;

                            PXWindowCursorPositionInWindowGet(window, &positionX, &positionY);

                            PXWindowTriggerOnMouseMoveEvent(window, positionX, positionY, deltaX, deltaY);

                            

                            // Wheel data needs to be pointer casted to interpret an unsigned short as a short, with no conversion
                            // otherwise it'll overflow when going negative.
                            // Didn't happen before some minor changes in the code, doesn't seem to go away
                            // so it's going to have to be like this.
                            // if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
                            //     input.mouse.wheel = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
                        }
#endif
                    }

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

        case WindowEventKEYFIRST:
            break;

        case WindowEventKEYDOWN:
        case WindowEventKEYUP:
        {
            PXKeyPressState mode = PXKeyPressStateInvalid;

            switch(eventID)
            {
                case WM_KEYUP:
                {
                    mode = PXKeyPressStateUp;
                    break;
                }
                case WM_KEYDOWN:
                {
                    mode = PXKeyPressStateDown;
                    break;
                }
            }

            const PXSize character = wParam;
            const PXSize characterInfo = lParam;
            const PXVirtualKey virtualKey = PXVirtualKeyFromID(character);

            PXKeyBoardKeyPressedSet(&window->KeyBoardCurrentInput, virtualKey, mode == PXKeyPressStateUp);

            PXKeyBoardKeyInfo buttonInfo;
            buttonInfo.KeyID = character;
            buttonInfo.Key = virtualKey;
            buttonInfo.Mode = mode;

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

            PXWindowTriggerOnKeyBoardKeyEvent(window, &buttonInfo);

            break;
        }

        case WindowEventCHAR:
            break;
        case WindowEventDEADCHAR:
            break;
        case WindowEventSYSKEYDOWN:
            break;
        case WindowEventSYSKEYUP:
            break;
        case WindowEventSYSCHAR:
            break;
        case WindowEventSYSDEADCHAR:
            break;
        case WindowEventUNICHAR:
            break;
        case WindowEventKEYLAST:
            break;
        case WindowEventIME_STARTCOMPOSITION:
            break;
        case WindowEventIME_ENDCOMPOSITION:
            break;
        case WindowEventIME_COMPOSITION:
            break;
        case WindowEventIME_KEYLAST:
            break;
        case WindowEventINITDIALOG:
            break;
        case WindowEventCOMMAND:
            break;
        case WindowEventSYSCOMMAND:
            break;
        case WindowEventTIMER:
            break;
        case WindowEventHSCROLL:
            break;
        case WindowEventVSCROLL:
            break;
        case WindowEventINITMENU:
            break;
        case WindowEventINITMENUPOPUP:
            break;
        case WindowEventGESTURE:
            break;
        case WindowEventGESTURENOTIFY:
            break;
        case WindowEventMENUSELECT:
            break;
        case WindowEventMENUCHAR:
            break;
        case WindowEventENTERIDLE:
            break;
        case WindowEventMENURBUTTONUP:
            break;
        case WindowEventMENUDRAG:
            break;
        case WindowEventMENUGETOBJECT:
            break;
        case WindowEventUNINITMENUPOPUP:
            break;
        case WindowEventMENUCOMMAND:
            break;
        case WindowEventCHANGEUISTATE:
            break;
        case WindowEventUPDATEUISTATE:
            break;
        case WindowEventQUERYUISTATE:
            break;
        case WindowEventCTLCOLORMSGBOX:
            break;
        case WindowEventCTLCOLOREDIT:
            break;
        case WindowEventCTLCOLORLISTBOX:
            break;
        case WindowEventCTLCOLORBTN:
            break;
        case WindowEventCTLCOLORDLG:
            break;
        case WindowEventCTLCOLORSCROLLBAR:
            break;
        case WindowEventCTLCOLORSTATIC:
            break;
        case WindowEventGETHMENU:
            break;
        //case WindowEventMOUSEFIRST:
        //    break;
        //case WindowEventMOUSEMOVE:
        //    break;
        case WindowEventLBUTTONDOWN:
            PXWindowTriggerOnMouseClickEvent(window, PXMouseButtonLeft, PXKeyPressStateDown);
            break;

        case WindowEventLBUTTONUP:
            PXWindowTriggerOnMouseClickEvent(window, PXMouseButtonLeft, PXKeyPressStateUp);
            break;

        case WindowEventLBUTTONDBLCLK:
            PXWindowTriggerOnMouseClickDoubleEvent(window, PXMouseButtonLeft);
            break;

        case WindowEventRBUTTONDOWN:
            PXWindowTriggerOnMouseClickEvent(window, PXMouseButtonRight, PXKeyPressStateDown);
            break;

        case WindowEventRBUTTONUP:
            PXWindowTriggerOnMouseClickEvent(window, PXMouseButtonRight, PXKeyPressStateUp);
            break;

        case WindowEventRBUTTONDBLCLK:
            PXWindowTriggerOnMouseClickDoubleEvent(window, PXMouseButtonRight);
            break;

        case WindowEventMBUTTONDOWN:
            PXWindowTriggerOnMouseClickEvent(window, PXMouseButtonMiddle, PXKeyPressStateDown);
            break;

        case WindowEventMBUTTONUP:
            PXWindowTriggerOnMouseClickEvent(window, PXMouseButtonMiddle, PXKeyPressStateUp);
            break;

        case WindowEventMBUTTONDBLCLK:
            PXWindowTriggerOnMouseClickDoubleEvent(window, PXMouseButtonMiddle);
            break;

        case WindowEventMOUSEWHEEL:
            break;

        case WindowEventXBUTTONUP:
        case WindowEventXBUTTONDOWN:
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
                switch(windowEventType)
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


                PXWindowTriggerOnMouseClickEvent(window, mouseButton, buttonState);
            }

            break;
        }

        case WindowEventXBUTTONDBLCLK:
            break;
        case WindowEventMOUSEHWHEEL:
            break;
        case WindowEventMOUSELAST:
            break;
        case WindowEventPARENTNOTIFY:
            break;
        case WindowEventENTERMENULOOP:
            break;
        case WindowEventEXITMENULOOP:
            break;
        case WindowEventNEXTMENU:
            break;
        case WindowEventSIZING:
            break;
        case WindowEventCAPTURECHANGED:
            break;
        case WindowEventMOVING:
            break;
        case WindowEventPOWERBROADCAST:
            break;
        case WindowEventDEVICECHANGE:
            break;
        case WindowEventMDICREATE:
            break;
        case WindowEventMDIDESTROY:
            break;
        case WindowEventMDIACTIVATE:
            break;
        case WindowEventMDIRESTORE:
            break;
        case WindowEventMDINEXT:
            break;
        case WindowEventMDIMAXIMIZE:
            break;
        case WindowEventMDITILE:
            break;
        case WindowEventMDICASCADE:
            break;
        case WindowEventMDIICONARRANGE:
            break;
        case WindowEventMDIGETACTIVE:
            break;
        case WindowEventMDISETMENU:
            break;
        case WindowEventENTERSIZEMOVE:
            break;
        case WindowEventEXITSIZEMOVE:
            break;
        case WindowEventDROPFILES:
            break;
        case WindowEventMDIREFRESHMENU:
            break;
        case WindowEventPOINTERDEVICECHANGE:
            break;
        case WindowEventPOINTERDEVICEINRANGE:
            break;
        case WindowEventPOINTERDEVICEOUTOFRANGE:
            break;
        case WindowEventTOUCH:
            break;
        case WindowEventNCPOINTERUPDATE:
            break;
        case WindowEventNCPOINTERDOWN:
            break;
        case WindowEventNCPOINTERUP:
            break;
        case WindowEventPOINTERUPDATE:
            break;
        case WindowEventPOINTERDOWN:
            break;
        case WindowEventPOINTERUP:
            break;
        case WindowEventPOINTERENTER:
            break;
        case WindowEventPOINTERLEAVE:
            break;
        case WindowEventPOINTERACTIVATE:
            break;
        case WindowEventPOINTERCAPTURECHANGED:
            break;
        case WindowEventTOUCHHITTESTING:
            break;
        case WindowEventPOINTERWHEEL:
            break;
        case WindowEventPOINTERHWHEEL:
            break;
        case WindowEventPOINTERROUTEDTO:
            break;
        case WindowEventPOINTERROUTEDAWAY:
            break;
        case WindowEventPOINTERROUTEDRELEASED:
            break;
        case WindowEventIME_SETCONTEXT:
            break;
        case WindowEventIME_NOTIFY:
            break;
        case WindowEventIME_CONTROL:
            break;
        case WindowEventIME_COMPOSITIONFULL:
            break;
        case WindowEventIME_SELECT:
            break;
        case WindowEventIME_CHAR:
            break;
        case WindowEventIME_REQUEST:
            break;
        case WindowEventIME_KEYDOWN:
            break;
        case WindowEventIME_KEYUP:
            break;
        case WindowEventMOUSEHOVER:
            break;
        case WindowEventMOUSELEAVE:
            break;
        case WindowEventNCMOUSEHOVER:
            break;
        case WindowEventNCMOUSELEAVE:
            break;
        case WindowEventWTSSESSION_CHANGE:
            break;
        case WindowEventTABLET_FIRST:
            break;
        case WindowEventTABLET_LAST:
            break;
        case WindowEventDPICHANGED:
            break;
        case WindowEventDPICHANGED_BEFOREPARENT:
            break;
        case WindowEventDPICHANGED_AFTERPARENT:
            break;
        case WindowEventGETDPISCALEDSIZE:
            break;
        case WindowEventCUT:
        {
           // printf("[#][Event] CUT\n");
            break;
        }

        case WindowEventCOPY:
        {
           // printf("[#][Event] Copy\n");
            break;
        }
        case WindowEventPASTE:
        {
            //printf("[#][Event] Paste\n");
            break;
        }
        case WindowEventCLEAR:
        {
            //printf("[#][Event] Clear\n");
            break;
        }
        case WindowEventUNDO:
        {
            //printf("[#][Event] Undo\n");
            break;
        }
        case WindowEventRENDERFORMAT:
            break;
        case WindowEventRENDERALLFORMATS:
            break;
        case WindowEventDESTROYCLIPBOARD:
            break;
        case WindowEventDRAWCLIPBOARD:
            break;
        case WindowEventPAINTCLIPBOARD:
            break;
        case WindowEventVSCROLLCLIPBOARD:
            break;
        case WindowEventSIZECLIPBOARD:
            break;
        case WindowEventASKCBFORMATNAME:
            break;
        case WindowEventCHANGECBCHAIN:
            break;
        case WindowEventHSCROLLCLIPBOARD:
            break;
        case WindowEventQUERYNEWPALETTE:
            break;
        case WindowEventPALETTEISCHANGING:
            break;
        case WindowEventPALETTECHANGED:
            break;
        case WindowEventHOTKEY:
            break;
        case WindowEventPRINT:
            break;
        case WindowEventPRINTCLIENT:
            break;
        case WindowEventAPPCOMMAND:
            break;
        case WindowEventTHEMECHANGED:
            break;
        case WindowEventCLIPBOARDUPDATE:
            break;
        case WindowEventDWMCOMPOSITIONCHANGED:
            break;
        case WindowEventDWMNCRENDERINGCHANGED:
            break;
        case WindowEventDWMCOLORIZATIONCOLORCHANGED:
            break;
        case WindowEventDWMWINDOWMAXIMIZEDCHANGE:
            break;
        case WindowEventDWMSENDICONICTHUMBNAIL:
            break;
        case WindowEventDWMSENDICONICLIVEPREVIEWBITMAP:
            break;
        case WindowEventGETTITLEBARINFOEX:
            break;
        case WindowEventHANDHELDFIRST:
            break;
        case WindowEventHANDHELDLAST:
            break;
        case WindowEventAFXFIRST:
            break;
        case WindowEventAFXLAST:
            break;
        case WindowEventPENWINFIRST:
            break;
        case WindowEventPENWINLAST:
            break;
        case WindowEventAPP:
            break;

        case WindowEventInvalid:
        default:
            break;
    }

    // If nothing had grabbed the event by now, let the default funcion handle it.
    return DefWindowProc(windowsID, eventID, wParam, lParam);
}
#endif

PXThreadResult PXOSAPI PXWindowCreateThread(PXWindow* const window)
{
    window->IsRunning = 0;
    window->CursorModeCurrent = PXWindowCursorShow;

#if OSUnix

    // Make this thread safe
    {
        const int result = XInitThreads();
    }

    // Create display
    {
        const Display* const display = XOpenDisplay(0);   // Create Window
        const PXBool successful = display != 0;

        if (!successful)
        {
            return PXThreadSucessful; // printf("\n\tcannot connect to X server\n\n");
        }

        window->DisplayCurrent = display;
    }

    // Get root window
    {
        const XID windowRoot = DefaultRootWindow(window->DisplayCurrent); // Make windows root
        const PXBool successful = windowRoot != 0;

        window->WindowRoot = successful ? windowRoot : 0;
    }


    const int attributeList[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

    const XVisualInfo* const visualInfo = glXChooseVisual(window->DisplayCurrent, 0, attributeList);

    {
        const PXBool successful = visualInfo != 0;

        if (!successful)
        {
            return PXThreadSucessful; // no appropriate visual found
        }
    }

    // Create colormapping
    Colormap colormap = XCreateColormap
    (
        window->DisplayCurrent,
        window->WindowRoot,
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

        PXWindow cccc = *window;

        const XID PXWindowID = XCreateWindow
        (
            window->DisplayCurrent,
            window->WindowRoot,
            window->X,
            window->Y,
            window->Width,
            window->Height,
            borderWidth,
            visualInfo->depth,
            InputOutput,
            visualInfo->visual,
            CWColormap | CWEventMask,
            &setWindowAttributes
        );
        const PXBool sucessful = PXWindowID;

        printf("[i][Window] Create <%i x %i> \n", window->Width, window->Height);

        window->ID = sucessful ? PXWindowID : 0;
    }

    // Giving the graphic system window context
    window->GraphicInstance.AttachedWindow = window;

    // Set Title
    {
        XMapWindow(window->DisplayCurrent, window->ID);

        switch(window->Title.Format)
        {
            case TextFormatASCII:
            case TextFormatUTF8:
            {
                 XStoreName(window->DisplayCurrent, window->ID, window->Title.TextA);

                break;
            }
            case TextFormatUNICODE:
            {
                //char windowTitleA[256];

                //PXTextCopyWA(window->Title, 256, windowTitleA, 256);

                XStoreName(window->DisplayCurrent, window->ID, window->Title.TextW);

                break;
            }
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
    const PXSize maskLength = (XI_LASTEVENT + 7) / 8;
    unsigned char mask[maskLength];

    PXMemoryClear(mask, sizeof(mask));
    PXMemoryClear(&eventmask, sizeof(XIEventMask));

    XISetMask(mask, XI_RawMotion);
    //XISetMask(mask, XI_RawButtonPress);
    //XISetMask(mask, XI_RawKeyPress);

    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = maskLength;
    eventmask.mask = mask;


    XISelectEvents(window->DisplayCurrent, window->WindowRoot, &eventmask, 1u);
    XFlush(window->DisplayCurrent);


#elif PXOSWindowsDestop

    PXWindowID windowID = 0;
    const DWORD windowStyle = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME | WS_EX_CONTEXTHELP;
    DWORD dwStyle = 0;
    HWND hWndParent = 0;
    HMENU hMenu = 0;
    void* lpParam = 0;

    UINT        style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    WNDPROC     lpfnWndProc = PXWindowEventHandler;
    int         cbClsExtra = 0; // The number of extra bytes to allocate following the window-class structure.
    int         cbWndExtra = 0;
    HINSTANCE hInstance = GetModuleHandle(NULL);
    HICON       hIcon = LoadIcon(NULL, IDI_APPLICATION);
    HCURSOR     hCursor = window->CursorID;
   // HBRUSH      hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); //(HBRUSH)GetStockObject(COLOR_BACKGROUND);
    
    HBRUSH hbrBackground = CreateSolidBrush(RGB(38, 38, 38));

    // Cursor setup
    {
        const HCURSOR cursorID = LoadCursor(hInstance, IDC_ARROW);
        window->CursorID = cursorID;

        if (window->Mode == PXWindowModeNormal)
        {
            const int xx = WS_OVERLAPPEDWINDOW;

            dwStyle |= WS_OVERLAPPEDWINDOW;// | WS_VISIBLE;
        }
    }

    switch (window->Title.Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            char* windowClassName = 0;

            // Registering of class
            {
                WNDCLASSA wndclass;

                PXMemoryClear(&wndclass, sizeof(WNDCLASSA));

                wndclass.style = style;
                wndclass.lpfnWndProc = lpfnWndProc;
                wndclass.cbClsExtra = cbClsExtra;
                wndclass.cbWndExtra = cbWndExtra;
                wndclass.hInstance = hInstance;
                wndclass.hIcon = hIcon;
                wndclass.hCursor = hCursor;
                wndclass.hbrBackground = hbrBackground;
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = "PXUltima_WindowCreate";

                const WORD classID = RegisterClassA(&wndclass);

                windowClassName = (char*)classID;
            }

            windowID = CreateWindowExA // Windows 2000, User32.dll, winuser.h
            (
                windowStyle,
                windowClassName,
                window->Title.TextA,
                dwStyle,
                window->X,
                window->Y,
                window->Width,
                window->Height,
                hWndParent,
                hMenu,
                hInstance,
                lpParam
            );

            break;
        }
        case TextFormatUNICODE:
        {
            wchar_t* windowClassName = 0;

            // Registering of class
            {
                WNDCLASSW wndclass;

                PXMemoryClear(&wndclass, sizeof(WNDCLASSW));

                wndclass.style = style;
                wndclass.lpfnWndProc = lpfnWndProc;
                wndclass.cbClsExtra = cbClsExtra;
                wndclass.cbWndExtra = cbWndExtra;
                wndclass.hInstance = hInstance;
                wndclass.hIcon = hIcon;
                wndclass.hCursor = hCursor;
                wndclass.hbrBackground = hbrBackground;
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = L"PXUltima_WindowCreate";

                const WORD classID = RegisterClassW(&wndclass);

                windowClassName = (wchar_t*)classID;
            }

            windowID = CreateWindowExW // Windows 2000, User32.dll, winuser.h
            (
                windowStyle,
                windowClassName,
                window->Title.TextW,
                dwStyle,
                window->X,
                window->Y,
                window->Width,
                window->Height,
                hWndParent,
                hMenu,
                hInstance,
                lpParam
            );

            break;
        }
        default:
        {
            char* windowClassName = 0;

            // Registering of class
            {
                WNDCLASS wndclass;

                PXMemoryClear(&wndclass, sizeof(WNDCLASS));

                wndclass.style = style;
                wndclass.lpfnWndProc = lpfnWndProc;
                wndclass.cbClsExtra = cbClsExtra;
                wndclass.cbWndExtra = cbWndExtra;
                wndclass.hInstance = hInstance;
                wndclass.hIcon = hIcon;
                wndclass.hCursor = hCursor;
                wndclass.hbrBackground = hbrBackground;
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = "PXUltima_WindowCreate";

                const WORD classID = RegisterClass(&wndclass);

                windowClassName = (char*)classID;
            }

            windowID = CreateWindowEx // Windows 2000, User32.dll, winuser.h
            (
                windowStyle,
                windowClassName,
                PXNull,
                dwStyle,
                window->X,
                window->Y,
                window->Width,
                window->Height,
                hWndParent,
                hMenu,
                hInstance,
                lpParam
            );
            break;
        }
    }

    {
        if (!windowID)
        {
            const PXActionResult windowsCreateResult = PXErrorCurrent();

            // Handle error?

            return PXThreadActionFailed;
        }

        window->ID = windowID;
    }
#else

// OS does not support window creation or it is not implemented

#endif

    //---<POST-Update>----------------
    PXWindowPosition(window, PXNull, PXNull);
    PXWindowCursorPositionInWindowGet(window, &window->MouseCurrentInput.Position[0], &window->MouseCurrentInput.Position[1]);
    PXWindowTitleBarColorSet(window);
    PXWindowLookupAdd(window);
    //--------------------------

#if PXOSWindowsDestop
    UpdateWindow(windowID);

    if (window->Mode == PXWindowModeNormal)
    {
#if 0 // Use animation

        const BOOL animationResult = AnimateWindow(windowID, 10000, AW_ACTIVATE | AW_HOR_POSITIVE);

        if (!animationResult)
        {
            PXActionResult pxActionResult = PXErrorCurrent();

            printf("ERR\n");
        }
#else
        ShowWindow(windowID, SW_NORMAL);
#endif  
    }
#endif

    window->IsRunning = 1;

    PXFunctionInvoke(window->WindowCreatedCallBack, window->EventReceiver, window);
      

#if OSUnix
#if 0
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
#elif PXOSWindowsDestop
    // Register input device
    {
        // We're configuring just one RAWINPUTDEVICE, the mouse, so it's a single-element array (a pointer).
        RAWINPUTDEVICE rid;

        rid.usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
        rid.usUsage = 0x02;// HID_USAGE_GENERIC_MOUSE;
        rid.dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
        rid.hwndTarget = windowID;

        const PXBool result = RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));

        if (!result)
        {
            //printf("Err\n");

            // TODO: Handle error

        }

        // RegisterRawInputDevices should not be used from a library, as it may interfere with any raw input processing logic already present in applications that load it.
    }
#endif

    if (window->MessageThread.ThreadID != 0)
    {
        while (window->IsRunning)
        {
            PXWindowUpdate(window);
        }
    }

    return PXThreadSucessful;
}

PXActionResult PXAPI PXWindowPixelSystemSet(PXWindow* const window)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    if (!window->HandleDeviceContext) // If we dont have a prefered GPU
    {
        window->HandleDeviceContext = GetDC(window->ID); // Get the "default" device
    }

    const WORD  nVersion = 1;
    const DWORD dwFlags = 
        //PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        //PFD_SUPPORT_DIRECTDRAW | 
        //PFD_DIRECT3D_ACCELERATED | 
        PFD_DOUBLEBUFFER | 
        0;
    const BYTE  iPixelType = PFD_TYPE_RGBA; // The kind of framebuffer. RGBA or palette.
    const BYTE  cColorBits = 32;   // Colordepth of the framebuffer.
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
    const int letWindowsChooseThisPixelFormat = ChoosePixelFormat(window->HandleDeviceContext, &pfd);
    const PXBool sucessul = SetPixelFormat(window->HandleDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

    PXActionOnErrorFetchAndReturn(!sucessul);
       
    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif  
}

void PXAPI PXWindowUpdate(PXWindow* const pxWindow)
{
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




    XLockDisplay(window->DisplayCurrent);

    XNextEvent(window->DisplayCurrent, &windowEvent);

    XUnlockDisplay(window->DisplayCurrent);

    PXWindowEventHandler(window, &windowEvent);

#elif PXOSWindowsDestop     
  
    while (1)
    {
        MSG message;

        const PXBool peekResult = PeekMessage(&message, pxWindow->ID, 0, 0, PM_NOREMOVE); // Windows 2000, User32.dll, winuser.h

        if (!peekResult)
        {
            break; // Stop, no more messages
        }

        const PXBool messageResult = GetMessage(&message, pxWindow->ID, 0, 0); // Windows 2000, User32.dll, winuser.h

        if (!messageResult)
        {
            continue; // Skip, message fetch failed
        }

        const BOOL translateResult = TranslateMessage(&message); // Windows 2000, User32.dll, winuser.h
        const LRESULT dispatchResult = DispatchMessage(&message); // Windows 2000, User32.dll, winuser.h
    }

#endif
}

void PXAPI PXWindowConstruct(PXWindow* const window)
{
   // PXClear(PXWindow, window);
    window->Title.SizeAllocated = 256;
    window->Title.TextA = window->TitleBuffer;
    window->Mode = PXWindowModeNormal;
}

float PXAPI PXWindowScreenRatio(const PXWindow* const window)
{
    return (float)window->Width / (float)window->Height;
}

void PXAPI PXWindowCreateA(PXWindow* const window, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height, const char* const title, const PXBool async)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, title, PXTextUnkownLength);

    PXWindowCreate(window, x, y, width, height, &pxText, async);
}

void PXAPI PXWindowCreate(PXWindow* const window, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height, const PXText* const title, const PXBool async)
{
    PXTextCopy(title, &window->Title);

    window->X = x;
    window->Y = y;
    window->Width = width;
    window->Height = height;

    {
        const PXBool isDefaultSize = width == -1 && height == -1;

        if (isDefaultSize)
        {
            PXInt32S screenWidth = 0;
            PXInt32S screenHeight = 0;

            PXMonitorGetSize(&screenWidth, &screenHeight);

            window->X = screenWidth * 0.125f;
            window->Y = screenHeight * 0.125f;
            window->Width = screenWidth * 0.75f;
            window->Height = screenHeight * 0.75f;
        }
    }

    if (async)
    {
        const PXActionResult actionResult = PXThreadRun(&window->MessageThread, PXWindowCreateThread, window);
        const PXBool sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return; // TODO: return something?
        }

        PXAwaitChangeCU(&window->IsRunning);
    }
    else
    {
        PXWindowCreateThread(window);
    }
}

void PXAPI PXWindowCreateHidden(PXWindow* const window, const PXInt32S width, const PXInt32S height, const PXBool async)
{
    window->Mode = PXWindowModeHidden;

    PXWindowCreate(window, 0, 0, width, height, 0, async);
}

void PXAPI PXWindowDestruct(PXWindow* const window)
{
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

PXBool PXAPI PXWindowTitleSet(PXWindow* const window, const PXText* const title)
{
    // PXTextCopy(title, &window->Title); Useless?

    switch (title->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix
            return 0;
#elif PXOSWindowsDestop
            const PXBool success = SetWindowTextA(window->ID, window->Title.TextA); // Windows 2000, User32.dll, winuser.h

            // could get extended error

            return success;
#else
            return PXFalse; // Not supported by OS
#endif
        }
        case TextFormatUNICODE:
        {
#if OSUnix
            return 0;
#elif PXOSWindowsDestop

            const PXBool success = SetWindowTextW(window->ID, window->Title.TextW); // Windows 2000, User32.dll, winuser.h

            // could get extended error

            return success;
#else
            return PXFalse; // Not supported by OS
#endif
        }
    }

    return PXNull;
}

PXSize PXAPI PXWindowTitleGet(const PXWindow* const window, PXText* const title)
{
    switch (title->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix
            return 0;
#elif PXOSWindowsDestop
            const int result = GetWindowTextA(window->ID, title->TextA, title->SizeAllocated); // Windows 2000, User32.dll, winuser.h
            const PXBool success = result > 0;

            title->SizeUsed = 0;

            // could get extended error
            if (success)
            {
                title->SizeUsed = result;
            }

            return result;
#else
            return PXFalse; // Not supported by OS
#endif
        }
        case TextFormatUNICODE:
        {
#if OSUnix
            return 0;
#elif PXOSWindowsDestop
            const int result = GetWindowTextW(window->ID, title->TextW, title->SizeAllocated); // Windows 2000, User32.dll, winuser.h
            const PXBool success = result > 0;

            title->SizeUsed = 0;

            if (success)
            {
                title->SizeUsed = result;
            }

            return result;
#else
            return PXFalse; // Not supported by OS
#endif
        }
        }

    return PXNull;
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

void PXAPI PXWindowLookupAdd(PXWindow* const window)
{
    currentWindow = window;
}

PXWindow* PXAPI PXWindowLookupFind(const PXWindowID PXWindowID)
{
    return currentWindow;
}

void PXAPI PXWindowLookupRemove(const PXWindow* window)
{
    currentWindow = 0;
}

PXActionResult PXAPI PXWindowTitleBarColorSet(const PXWindow* const pxWindow)
{
#if OSUnix
    return PXActionNotSupportedByOperatingSystem;
#elif WindowsAtleast10

    const BOOL useDarkMode = PXTrue;
    const BOOL setAttributeSuccess = SUCCEEDED(DwmSetWindowAttribute(pxWindow->ID, Windows10DarkModeID, &useDarkMode, sizeof(BOOL))); // Windows Vista, Dwmapi.dll;Uxtheme.dll, dwmapi.h

    if (!setAttributeSuccess)
    {
        return PXActionCancelled;
    }

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXWindowSize(const PXWindow* const pxWindow, PXInt32S* const x, PXInt32S* const y, PXInt32S* const width, PXInt32S* const height)
{
#if OSUnix
#elif PXOSWindowsDestop
    RECT rect;

    const unsigned char result = GetWindowRect(pxWindow->ID, &rect); // Windows 2000, User32.dll, winuser.h
    const PXBool success = result != 0;

    // Get Last Error

    *x = rect.left;
    *y = rect.top;
    *width = rect.right - *x;
    *height = rect.bottom - *y;
#endif
}

void PXAPI PXWindowSizeChange(PXWindow* const pxWindow, const PXInt32S x, const PXInt32S y, const PXInt32S width, const PXInt32S height)
{
#if OSUnix
#elif PXOSWindowsDestop
    RECT rect;

    rect.left = x;
    rect.top = y;
    rect.right = width + x;
    rect.bottom = height + y;

    DWORD style = 0;
    DWORD exStyle = 0;

    //AdjustWindowRectEx();

    const unsigned char result = AdjustWindowRectEx(&rect, style, FALSE, exStyle); // Windows 2000, User32.dll, winuser.h
    const unsigned char success = result != 0;

    // Get Last Error
#endif
}

PXActionResult PXAPI PXWindowPosition(PXWindow* window, PXInt32S* x, PXInt32S* y)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop
    RECT rectangle;
    const PXBool success = GetWindowRect(window->ID, &rectangle); // Windows 2000, User32.dll, winuser.h

    if (!success)
    {
        *x = -1;
        *y = -1;

        return PXErrorCurrent();
    }

    window->X = rectangle.left;
    window->Y = rectangle.top;
    window->Width = rectangle.right - rectangle.left;
    window->Height = rectangle.bottom - rectangle.top;

    return PXActionSuccessful;

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXWindowMove(PXWindow* const pxWindow, const PXInt32S x, const PXInt32S y)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop

   // const PXBool result = MoveWindow(pxWindow->ID, );

    return PXActionRefusedNotImplemented;

#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXWindowPositonCenterScreen(PXWindow* window)
{
}

void PXAPI PXWindowCursor(PXWindow* window)
{
}

void PXAPI PXWindowCursorTexture()
{
}

void PXAPI PXWindowCursorCaptureMode(PXWindow* window, const PXWindowCursorMode cursorMode)
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
            const HCURSOR cursorSet = SetCursor(window->CursorID);

            break;
        }
        case PXWindowCursorLock:
        {
           // printf("[Cursor] Lock\n");

            // Capture cursor
            {
                RECT clipRect;
                GetClientRect(window->ID, &clipRect);
                ClientToScreen(window->ID, (POINT*)&clipRect.left);
                ClientToScreen(window->ID, (POINT*)&clipRect.right);
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
                GetClientRect(window->ID, &clipRect);
                ClientToScreen(window->ID, (POINT*)&clipRect.left);
                ClientToScreen(window->ID, (POINT*)&clipRect.right);

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

    window->CursorModeCurrent = cursorMode;
#endif
}

PXBool PXAPI PXWindowFrameBufferSwap(const PXWindow* const window)
{
#if OSUnix
    glXSwapBuffers(window->DisplayCurrent, window->ID);
    return PXFalse;

#elif OSWindows

   const PXBool result = SwapBuffers(window->HandleDeviceContext);

   return result;

#else
    return PXFalse;
#endif
}

PXBool PXAPI PXWindowInteractable(PXWindow* window)
{
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
}

PXBool PXAPI PXWindowCursorPositionInWindowGet(PXWindow* const window, PXInt32S* const x, PXInt32S* const y)
{
    PXInt32S xPos = 0;
    PXInt32S yPos = 0;
    const PXBool sucessfulA = PXWindowCursorPositionInDestopGet(window, &xPos, &yPos);

#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    POINT point;
    point.x = xPos;
    point.y = yPos;

    const PXBool sucessful = ScreenToClient(window->ID, &point);  // are now relative to hwnd's client area

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

PXBool PXAPI PXWindowCursorPositionInDestopGet(PXWindow* const window, PXInt32S* const x, PXInt32S* const y)
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

PXBool PXAPI PXWindowIsInFocus(const PXWindow* const window)
{
#if OSUnix
    return PXFalse;
#elif PXOSWindowsDestop
    const HWND windowIDInFocus = GetForegroundWindow(); // Windows 2000, User32.dll,
    const PXBool isInFocus = window->ID == windowIDInFocus;

    return isInFocus;
#endif
}

void PXAPI PXWindowTriggerOnMouseScrollEvent(const PXWindow* window, const PXMouse* mouse)
{

}

void PXAPI PXWindowTriggerOnMouseClickEvent(PXWindow* const window, const PXMouseButton mouseButton, const PXKeyPressState buttonState)
{
    PXMouse* const mouse = &window->MouseCurrentInput;

    switch (buttonState)
    {
        case PXKeyPressStateDown:
        {
            switch (mouseButton)
            {
                case PXMouseButtonLeft:
                {
                    mouse->Buttons |= ButtonLeft;
                    break;
                }
                case PXMouseButtonMiddle:
                {
                    mouse->Buttons |= ButtonMiddle;
                    break;
                }
                case PXMouseButtonRight:
                {
                    mouse->Buttons |= ButtonRight;
                    break;
                }
                case PXMouseButtonSpecialA:
                {
                    mouse->Buttons |= ButtonCustomA;
                    break;
                }
                case PXMouseButtonSpecialB:
                {
                    mouse->Buttons |= ButtonCustomB;
                    break;
                }
            }
            break;
        }
        case PXKeyPressStateUp:
        {
            switch (mouseButton)
            {
                case PXMouseButtonLeft:
                {
                    mouse->Buttons &= ~ButtonLeft;
                    break;
                }
                case PXMouseButtonMiddle:
                {
                    mouse->Buttons &= ~ButtonMiddle;
                    break;
                }
                case PXMouseButtonRight:
                {
                    mouse->Buttons &= ~ButtonRight;
                    break;
                }
                case PXMouseButtonSpecialA:
                {
                    mouse->Buttons &= ~ButtonCustomA;
                    break;
                }
                case PXMouseButtonSpecialB:
                {
                    mouse->Buttons &= ~ButtonCustomB;
                    break;
                }
            }
            break;
        }

        default:
            break;
    }

#if 0
    const char* buttonStateText = 0;
    const char* mouseButtonText = 0;

    switch(buttonState)
    {
        case PXKeyPressStateInvalid:
            buttonStateText = "Invalid";
            break;

        case PXKeyPressStateDown:
            buttonStateText = "Down";
            break;

        case PXKeyPressStateHold:
            buttonStateText = "Hold";
            break;

        case PXKeyPressStateUp:
            buttonStateText = "Release";
            break;
    }

    switch(mouseButton)
    {
        case PXMouseButtonInvalid:
            mouseButtonText = "Invalid";
            break;

        case PXMouseButtonLeft:
            mouseButtonText = "Left";
            break;

        case PXMouseButtonMiddle:
            mouseButtonText = "Middle";
            break;

        case PXMouseButtonRight:
            mouseButtonText = "Right";
            break;

        case PXMouseButtonSpecialA:
            mouseButtonText = "Special A";
            break;

        case PXMouseButtonSpecialB:
            mouseButtonText = "Special B";
            break;

        case PXMouseButtonSpecialC:
            mouseButtonText = "Special C";
            break;

        case PXMouseButtonSpecialD:
            mouseButtonText = "Special D";
            break;

        case PXMouseButtonSpecialE:
            mouseButtonText = "Special E";
            break;
    }

    printf("[#][Event][Mouse] Button:%-10s State:%-10s\n", mouseButtonText, buttonStateText);

#endif

    PXFunctionInvoke(window->MouseClickCallBack, window->EventReceiver, window, mouseButton, buttonState);
}

void PXAPI PXWindowTriggerOnMouseClickDoubleEvent(const PXWindow* window, const PXMouseButton mouseButton)
{
    PXFunctionInvoke(window->MouseClickDoubleCallBack, window->EventReceiver, window, mouseButton);
}

#define UseOSDelta 0

void PXAPI PXWindowTriggerOnMouseMoveEvent(PXWindow* const window, const PXInt32S positionX, const PXInt32S positionY, const PXInt32S deltaX, const PXInt32S deltaY)
{
    PXMouse* const mouse = &window->MouseCurrentInput;

    const PXInt32S mousePositionOld[2] =
    {
        mouse->Position[0],
        mouse->Position[1]
    };
    const PXInt32S mousePositionNew[2] =
    {
#if UseOSDelta
        mousePositionOld + deltaX,
        mousePositionOld - deltaY
#else
        PXMathLimit(positionX, 0, window->Width),
        window->Height - PXMathLimit(positionY, 0, window->Height)
#endif
    };

    const PXInt32S mousePositionDeltaNew[2] = 
    {
        #if UseOSDelta
        mousePositionNew[0] - mousePositionOld[0],
        mousePositionNew[1] - mousePositionOld[1]
        #else
        deltaX,
        deltaY
        #endif
    };

    const PXBool hasDelta = (mousePositionDeltaNew[0] != 0 && mousePositionDeltaNew[1] != 0) || 1;

    if (hasDelta)
    {
        //mouse->Delta[0] = mousePositionDeltaNew[0];
        //mouse->Delta[1] = mousePositionDeltaNew[1];
        mouse->Delta[0] += deltaX;
        mouse->Delta[1] += deltaY;
        mouse->DeltaNormalisized[0] = (mousePositionDeltaNew[0] / ((float)window->Width / 2.0f)) - 1.0f;
        mouse->DeltaNormalisized[1] = (mousePositionDeltaNew[1] / ((float)window->Height / 2.0f)) - 1.0f;
        mouse->Position[0] = mousePositionNew[0];
        mouse->Position[1] = mousePositionNew[1];
        mouse->PositionNormalisized[0] = (mousePositionNew[0] / ((float)window->Width / 2.0f)) - 1.0f;
        mouse->PositionNormalisized[1] = (mousePositionNew[1] / ((float)window->Height / 2.0f)) - 1.0f;
    }
    else
    {
        mouse->Delta[0] = 0;
        mouse->Delta[1] = 0;
    }

    PXFunctionInvoke(window->MouseMoveCallBack, window->EventReceiver, window, mouse);
}

void PXAPI PXWindowTriggerOnMouseEnterEvent(const PXWindow* window, const PXMouse* mouse)
{
}

void PXAPI PXWindowTriggerOnMouseLeaveEvent(const PXWindow* window, const PXMouse* mouse)
{
}

void PXAPI PXWindowTriggerOnKeyBoardKeyEvent(PXWindow* const window, const PXKeyBoardKeyInfo* const keyBoardKeyInfo)
{
    // printf("[#][Event][Key] ID:%-3i Name:%-3i State:%i\n", keyBoardKeyInfo->KeyID, keyBoardKeyInfo->Key, keyBoardKeyInfo->Mode);

    PXKeyBoard* const keyBoard = &window->KeyBoardCurrentInput;

    PXInt32U mask = 0;
    PXInt32U data = 0;

    if (keyBoardKeyInfo->Mode == PXKeyPressStateDown)
    {
        switch (keyBoardKeyInfo->Key)
        {
            case KeyA: keyBoard->Letters |= KeyBoardIDLetterA; break;
            case KeyB: keyBoard->Letters |= KeyBoardIDLetterB; break;
            case KeyC: keyBoard->Letters |= KeyBoardIDLetterC; break;
            case KeyD: keyBoard->Letters |= KeyBoardIDLetterD; break;
            case KeyE: keyBoard->Letters |= KeyBoardIDLetterE; break;
            case KeyF: keyBoard->Letters |= KeyBoardIDLetterF; break;
            case KeyG: keyBoard->Letters |= KeyBoardIDLetterG; break;
            case KeyH: keyBoard->Letters |= KeyBoardIDLetterH; break;
            case KeyI: keyBoard->Letters |= KeyBoardIDLetterI; break;
            case KeyJ: keyBoard->Letters |= KeyBoardIDLetterJ; break;
            case KeyK: keyBoard->Letters |= KeyBoardIDLetterK; break;
            case KeyL: keyBoard->Letters |= KeyBoardIDLetterL; break;
            case KeyM: keyBoard->Letters |= KeyBoardIDLetterM; break;
            case KeyN: keyBoard->Letters |= KeyBoardIDLetterN; break;
            case KeyO: keyBoard->Letters |= KeyBoardIDLetterO; break;
            case KeyP: keyBoard->Letters |= KeyBoardIDLetterP; break;
            case KeyQ: keyBoard->Letters |= KeyBoardIDLetterQ; break;
            case KeyR: keyBoard->Letters |= KeyBoardIDLetterR; break;
            case KeyS: keyBoard->Letters |= KeyBoardIDLetterS; break;
            case KeyT: keyBoard->Letters |= KeyBoardIDLetterT; break;
            case KeyU: keyBoard->Letters |= KeyBoardIDLetterU; break;
            case KeyV: keyBoard->Letters |= KeyBoardIDLetterV; break;
            case KeyW: keyBoard->Letters |= KeyBoardIDLetterW; break;
            case KeyX: keyBoard->Letters |= KeyBoardIDLetterX; break;
            case KeyY: keyBoard->Letters |= KeyBoardIDLetterY; break;
            case KeyZ: keyBoard->Letters |= KeyBoardIDLetterZ; break;
            case KeySpace: keyBoard->Letters |= KeyBoardIDSpace; break;
            case KeyApostrophe: keyBoard->Letters |= KeyBoardIDAPOSTROPHE; break;
            case KeyComma: keyBoard->Letters |= KeyBoardIDComma; break;
            case KeyGraveAccent: keyBoard->Letters |= KeyBoardIDGRAVE_ACCENT; break;
            case KeySemicolon: keyBoard->Letters |= KeyBoardIDSemicolon; break;
            case KeyPeriod: keyBoard->Letters |= KeyBoardIDDECIMAL; break;


            case KeyEscape: keyBoard->Commands |= KeyBoardIDCommandEscape; break;
            case KeyEnter: keyBoard->Commands |= KeyBoardIDCommandEnter; break;
            case KeyTab: keyBoard->Commands |= KeyBoardIDCommandTab; break;
            //case : keyBoard->Commands |= KeyBoardIDCommandShift; break;
            case KeyBackspace: keyBoard->Commands |= KeyBoardIDBACKSPACE; break;
            case KeyInsert: keyBoard->Commands |= KeyBoardIDINSERT; break;
            case KeyDelete: keyBoard->Commands |= KeyBoardIDDELETE; break;
            case KeyRight: keyBoard->Commands |= KeyBoardIDRIGHT; break;
            case KeyLeft: keyBoard->Commands |= KeyBoardIDLEFT; break;
            case KeyDown: keyBoard->Commands |= KeyBoardIDDOWN; break;
            case KeyUp: keyBoard->Commands |= KeyBoardIDUP; break;
            case KeyPageUp: keyBoard->Commands |= KeyBoardIDPAGE_UP; break;
            case KeyPageDown: keyBoard->Commands |= KeyBoardIDPAGE_DOWN; break;
            case KeyHome: keyBoard->Commands |= KeyBoardIDHOME; break;
            case KeyEnd: keyBoard->Commands |= KeyBoardIDEND; break;
            case KeyCapsLock: keyBoard->Commands |= KeyBoardIDCAPS_LOCK; break;
            case KeyScrollLock: keyBoard->Commands |= KeyBoardIDSCROLL_LOCK; break;
            case KeyNumLock: keyBoard->Commands |= KeyBoardIDNUM_LOCK; break;
            case KeyPrintScreen: keyBoard->Commands |= KeyBoardIDPRINT_SCREEN; break;
            case KeyPause: keyBoard->Commands |= KeyBoardIDPAUSE; break;
            case KeyPadEnter: keyBoard->Commands |= KeyBoardIDPadENTER; break;
            case KeyShiftLeft: keyBoard->Commands |= KeyBoardIDShiftLeft; break;
            case KeyShiftRight: keyBoard->Commands |= KeyBoardIDShiftRight; break;
            case KeyControlLeft: keyBoard->Commands |= KeyBoardIDCONTROLLEFT; break;
            case KeyAltLeft: keyBoard->Commands |= KeyBoardIDALTLEFT; break;
            //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDSUPERLEFT; break;
            case KeyControlRight: keyBoard->Commands |= KeyBoardIDCONTROLRIGHT; break;
            case KeyAltRight: keyBoard->Commands |= KeyBoardIDALTRIGHT; break;
            //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDSUPERRIGHT; break;
            //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDMENU; break;
            //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDWORLD_1; break;
            //case xxxxxxxxxxxxx: keyBoard->Commands |= KeyBoardIDWORLD_2; break;


                // Numbers
            case Key0: keyBoard->Numbers |= KeyBoardIDNumber0; break;
            case Key1: keyBoard->Numbers |= KeyBoardIDNumber1; break;
            case Key2: keyBoard->Numbers |= KeyBoardIDNumber2; break;
            case Key3: keyBoard->Numbers |= KeyBoardIDNumber3; break;
            case Key4: keyBoard->Numbers |= KeyBoardIDNumber4; break;
            case Key5: keyBoard->Numbers |= KeyBoardIDNumber5; break;
            case Key6: keyBoard->Numbers |= KeyBoardIDNumber6; break;
            case Key7: keyBoard->Numbers |= KeyBoardIDNumber7; break;
            case Key8: keyBoard->Numbers |= KeyBoardIDNumber8; break;
            case Key9: keyBoard->Numbers |= KeyBoardIDNumber9; break;
            case KeyPad0: keyBoard->Numbers |= KeyBoardIDNumberBlock0; break;
            case KeyPad1: keyBoard->Numbers |= KeyBoardIDNumberBlock1; break;
            case KeyPad2: keyBoard->Numbers |= KeyBoardIDNumberBlock2; break;
            case KeyPad3: keyBoard->Numbers |= KeyBoardIDNumberBlock3; break;
            case KeyPad4: keyBoard->Numbers |= KeyBoardIDNumberBlock4; break;
            case KeyPad5: keyBoard->Numbers |= KeyBoardIDNumberBlock5; break;
            case KeyPad6: keyBoard->Numbers |= KeyBoardIDNumberBlock6; break;
            case KeyPad7: keyBoard->Numbers |= KeyBoardIDNumberBlock7; break;
            case KeyPad8: keyBoard->Numbers |= KeyBoardIDNumberBlock8; break;
            case KeyPad9: keyBoard->Numbers |= KeyBoardIDNumberBlock9; break;
            case KeyMinus: keyBoard->Numbers |= KeyBoardIDNumberKeyMinus; break;
           // case KeyPeriod: keyBoard->Numbers |= KeyBoardIDNumberKeyPeriod; break;
            case KeySlash: keyBoard->Numbers |= KeyBoardIDNumberKeySlash; break;
            case KeyEqual: keyBoard->Numbers |= KeyBoardIDNumberKeyEqual; break;
            case KeyBrackedLeft: keyBoard->Numbers |= KeyBoardIDNumberKeyLEFT_BRACKET; break;
            case KeyBackSlash: keyBoard->Numbers |= KeyBoardIDNumberKeyBACKSLASH; break;
            case KeyBrackedRight: keyBoard->Numbers |= KeyBoardIDNumberKeyRIGHT_BRACKET; break;
            case KeyPadDivide: keyBoard->Numbers |= KeyBoardIDNumberKeyPadDIVIDE; break;
            case KeyPadMultiply: keyBoard->Numbers |= KeyBoardIDNumberKeyPadMULTIPLY; break;
            case KeyPadSubtract: keyBoard->Numbers |= KeyBoardIDNumberKeyPadSUBTRACT; break;
            case KeyPadAdd: keyBoard->Numbers |= KeyBoardIDNumberKeyPadADD; break;
            case KeyPadEqual: keyBoard->Numbers |= KeyBoardIDNumberKeyPadEQUAL; break;

                // Function key

            case KeyF1: keyBoard->Actions |= KeyBoardIDF01; break;
            case KeyF2: keyBoard->Actions |= KeyBoardIDF02; break;
            case KeyF3: keyBoard->Actions |= KeyBoardIDF03; break;
            case KeyF4: keyBoard->Actions |= KeyBoardIDF04; break;
            case KeyF5: keyBoard->Actions |= KeyBoardIDF05; break;
            case KeyF6: keyBoard->Actions |= KeyBoardIDF06; break;
            case KeyF7: keyBoard->Actions |= KeyBoardIDF07; break;
            case KeyF8: keyBoard->Actions |= KeyBoardIDF08; break;
            case KeyF9: keyBoard->Actions |= KeyBoardIDF09; break;
            case KeyF10: keyBoard->Actions |= KeyBoardIDF10; break;
            case KeyF11: keyBoard->Actions |= KeyBoardIDF11; break;
            case KeyF12: keyBoard->Actions |= KeyBoardIDF12; break;
            case KeyF13: keyBoard->Actions |= KeyBoardIDF13; break;
            case KeyF14: keyBoard->Actions |= KeyBoardIDF14; break;
            case KeyF15: keyBoard->Actions |= KeyBoardIDF15; break;
            case KeyF16: keyBoard->Actions |= KeyBoardIDF16; break;
            case KeyF17: keyBoard->Actions |= KeyBoardIDF17; break;
            case KeyF18: keyBoard->Actions |= KeyBoardIDF18; break;
            case KeyF19: keyBoard->Actions |= KeyBoardIDF19; break;
            case KeyF20: keyBoard->Actions |= KeyBoardIDF20; break;
            case KeyF21: keyBoard->Actions |= KeyBoardIDF21; break;
            case KeyF22: keyBoard->Actions |= KeyBoardIDF22; break;
            case KeyF23: keyBoard->Actions |= KeyBoardIDF23; break;
            case KeyF24: keyBoard->Actions |= KeyBoardIDF24; break;
            case KeyF25: keyBoard->Actions |= KeyBoardIDF25; break;

            default:
                break;
        }
    }
    else
    {
        switch (keyBoardKeyInfo->Key)
        {
            case KeyA: keyBoard->Letters &= ~KeyBoardIDLetterA; break;
            case KeyB: keyBoard->Letters &= ~KeyBoardIDLetterB; break;
            case KeyC: keyBoard->Letters &= ~KeyBoardIDLetterC; break;
            case KeyD: keyBoard->Letters &= ~KeyBoardIDLetterD; break;
            case KeyE: keyBoard->Letters &= ~KeyBoardIDLetterE; break;
            case KeyF: keyBoard->Letters &= ~KeyBoardIDLetterF; break;
            case KeyG: keyBoard->Letters &= ~KeyBoardIDLetterG; break;
            case KeyH: keyBoard->Letters &= ~KeyBoardIDLetterH; break;
            case KeyI: keyBoard->Letters &= ~KeyBoardIDLetterI; break;
            case KeyJ: keyBoard->Letters &= ~KeyBoardIDLetterJ; break;
            case KeyK: keyBoard->Letters &= ~KeyBoardIDLetterK; break;
            case KeyL: keyBoard->Letters &= ~KeyBoardIDLetterL; break;
            case KeyM: keyBoard->Letters &= ~KeyBoardIDLetterM; break;
            case KeyN: keyBoard->Letters &= ~KeyBoardIDLetterN; break;
            case KeyO: keyBoard->Letters &= ~KeyBoardIDLetterO; break;
            case KeyP: keyBoard->Letters &= ~KeyBoardIDLetterP; break;
            case KeyQ: keyBoard->Letters &= ~KeyBoardIDLetterQ; break;
            case KeyR: keyBoard->Letters &= ~KeyBoardIDLetterR; break;
            case KeyS: keyBoard->Letters &= ~KeyBoardIDLetterS; break;
            case KeyT: keyBoard->Letters &= ~KeyBoardIDLetterT; break;
            case KeyU: keyBoard->Letters &= ~KeyBoardIDLetterU; break;
            case KeyV: keyBoard->Letters &= ~KeyBoardIDLetterV; break;
            case KeyW: keyBoard->Letters &= ~KeyBoardIDLetterW; break;
            case KeyX: keyBoard->Letters &= ~KeyBoardIDLetterX; break;
            case KeyY: keyBoard->Letters &= ~KeyBoardIDLetterY; break;
            case KeyZ: keyBoard->Letters &= ~KeyBoardIDLetterZ; break;
            case KeySpace: keyBoard->Letters &= ~KeyBoardIDSpace; break;
            case KeyApostrophe: keyBoard->Letters &= ~KeyBoardIDAPOSTROPHE; break;
            case KeyComma: keyBoard->Letters &= ~KeyBoardIDComma; break;
            case KeyGraveAccent: keyBoard->Letters &= ~KeyBoardIDGRAVE_ACCENT; break;
            case KeySemicolon: keyBoard->Letters &= ~KeyBoardIDSemicolon; break;
            case KeyPeriod: keyBoard->Letters &= ~KeyBoardIDDECIMAL; break;


            case KeyEscape: keyBoard->Commands &= ~KeyBoardIDCommandEscape; break;
            case KeyEnter: keyBoard->Commands &= ~KeyBoardIDCommandEnter; break;
            case KeyTab: keyBoard->Commands &= ~KeyBoardIDCommandTab; break;
                //case : keyBoard->Commands &= ~KeyBoardIDCommandShift; break;
            case KeyBackspace: keyBoard->Commands &= ~KeyBoardIDBACKSPACE; break;
            case KeyInsert: keyBoard->Commands &= ~KeyBoardIDINSERT; break;
            case KeyDelete: keyBoard->Commands &= ~KeyBoardIDDELETE; break;
            case KeyRight: keyBoard->Commands &= ~KeyBoardIDRIGHT; break;
            case KeyLeft: keyBoard->Commands &= ~KeyBoardIDLEFT; break;
            case KeyDown: keyBoard->Commands &= ~KeyBoardIDDOWN; break;
            case KeyUp: keyBoard->Commands &= ~KeyBoardIDUP; break;
            case KeyPageUp: keyBoard->Commands &= ~KeyBoardIDPAGE_UP; break;
            case KeyPageDown: keyBoard->Commands &= ~KeyBoardIDPAGE_DOWN; break;
            case KeyHome: keyBoard->Commands &= ~KeyBoardIDHOME; break;
            case KeyEnd: keyBoard->Commands &= ~KeyBoardIDEND; break;
            case KeyCapsLock: keyBoard->Commands &= ~KeyBoardIDCAPS_LOCK; break;
            case KeyScrollLock: keyBoard->Commands &= ~KeyBoardIDSCROLL_LOCK; break;
            case KeyNumLock: keyBoard->Commands &= ~KeyBoardIDNUM_LOCK; break;
            case KeyPrintScreen: keyBoard->Commands &= ~KeyBoardIDPRINT_SCREEN; break;
            case KeyPause: keyBoard->Commands &= ~KeyBoardIDPAUSE; break;
            case KeyPadEnter: keyBoard->Commands &= ~KeyBoardIDPadENTER; break;
            case KeyShiftLeft: keyBoard->Commands &= ~KeyBoardIDShiftLeft; break;
            case KeyShiftRight: keyBoard->Commands &= ~KeyBoardIDShiftRight; break;
            case KeyControlLeft: keyBoard->Commands &= ~KeyBoardIDCONTROLLEFT; break;
            case KeyAltLeft: keyBoard->Commands &= ~KeyBoardIDALTLEFT; break;
                //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDSUPERLEFT; break;
            case KeyControlRight: keyBoard->Commands &= ~KeyBoardIDCONTROLRIGHT; break;
            case KeyAltRight: keyBoard->Commands &= ~KeyBoardIDALTRIGHT; break;
                //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDSUPERRIGHT; break;
                //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDMENU; break;
                //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDWORLD_1; break;
                //case xxxxxxxxxxxxx: keyBoard->Commands &= ~KeyBoardIDWORLD_2; break;


                    // Numbers
            case Key0: keyBoard->Numbers &= ~KeyBoardIDNumber0; break;
            case Key1: keyBoard->Numbers &= ~KeyBoardIDNumber1; break;
            case Key2: keyBoard->Numbers &= ~KeyBoardIDNumber2; break;
            case Key3: keyBoard->Numbers &= ~KeyBoardIDNumber3; break;
            case Key4: keyBoard->Numbers &= ~KeyBoardIDNumber4; break;
            case Key5: keyBoard->Numbers &= ~KeyBoardIDNumber5; break;
            case Key6: keyBoard->Numbers &= ~KeyBoardIDNumber6; break;
            case Key7: keyBoard->Numbers &= ~KeyBoardIDNumber7; break;
            case Key8: keyBoard->Numbers &= ~KeyBoardIDNumber8; break;
            case Key9: keyBoard->Numbers &= ~KeyBoardIDNumber9; break;
            case KeyPad0: keyBoard->Numbers &= ~KeyBoardIDNumberBlock0; break;
            case KeyPad1: keyBoard->Numbers &= ~KeyBoardIDNumberBlock1; break;
            case KeyPad2: keyBoard->Numbers &= ~KeyBoardIDNumberBlock2; break;
            case KeyPad3: keyBoard->Numbers &= ~KeyBoardIDNumberBlock3; break;
            case KeyPad4: keyBoard->Numbers &= ~KeyBoardIDNumberBlock4; break;
            case KeyPad5: keyBoard->Numbers &= ~KeyBoardIDNumberBlock5; break;
            case KeyPad6: keyBoard->Numbers &= ~KeyBoardIDNumberBlock6; break;
            case KeyPad7: keyBoard->Numbers &= ~KeyBoardIDNumberBlock7; break;
            case KeyPad8: keyBoard->Numbers &= ~KeyBoardIDNumberBlock8; break;
            case KeyPad9: keyBoard->Numbers &= ~KeyBoardIDNumberBlock9; break;
            case KeyMinus: keyBoard->Numbers &= ~KeyBoardIDNumberKeyMinus; break;
                // case KeyPeriod: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPeriod; break;
            case KeySlash: keyBoard->Numbers &= ~KeyBoardIDNumberKeySlash; break;
            case KeyEqual: keyBoard->Numbers &= ~KeyBoardIDNumberKeyEqual; break;
            case KeyBrackedLeft: keyBoard->Numbers &= ~KeyBoardIDNumberKeyLEFT_BRACKET; break;
            case KeyBackSlash: keyBoard->Numbers &= ~KeyBoardIDNumberKeyBACKSLASH; break;
            case KeyBrackedRight: keyBoard->Numbers &= ~KeyBoardIDNumberKeyRIGHT_BRACKET; break;
            case KeyPadDivide: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadDIVIDE; break;
            case KeyPadMultiply: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadMULTIPLY; break;
            case KeyPadSubtract: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadSUBTRACT; break;
            case KeyPadAdd: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadADD; break;
            case KeyPadEqual: keyBoard->Numbers &= ~KeyBoardIDNumberKeyPadEQUAL; break;

                // Function key

            case KeyF1: keyBoard->Actions &= ~KeyBoardIDF01; break;
            case KeyF2: keyBoard->Actions &= ~KeyBoardIDF02; break;
            case KeyF3: keyBoard->Actions &= ~KeyBoardIDF03; break;
            case KeyF4: keyBoard->Actions &= ~KeyBoardIDF04; break;
            case KeyF5: keyBoard->Actions &= ~KeyBoardIDF05; break;
            case KeyF6: keyBoard->Actions &= ~KeyBoardIDF06; break;
            case KeyF7: keyBoard->Actions &= ~KeyBoardIDF07; break;
            case KeyF8: keyBoard->Actions &= ~KeyBoardIDF08; break;
            case KeyF9: keyBoard->Actions &= ~KeyBoardIDF09; break;
            case KeyF10: keyBoard->Actions &= ~KeyBoardIDF10; break;
            case KeyF11: keyBoard->Actions &= ~KeyBoardIDF11; break;
            case KeyF12: keyBoard->Actions &= ~KeyBoardIDF12; break;
            case KeyF13: keyBoard->Actions &= ~KeyBoardIDF13; break;
            case KeyF14: keyBoard->Actions &= ~KeyBoardIDF14; break;
            case KeyF15: keyBoard->Actions &= ~KeyBoardIDF15; break;
            case KeyF16: keyBoard->Actions &= ~KeyBoardIDF16; break;
            case KeyF17: keyBoard->Actions &= ~KeyBoardIDF17; break;
            case KeyF18: keyBoard->Actions &= ~KeyBoardIDF18; break;
            case KeyF19: keyBoard->Actions &= ~KeyBoardIDF19; break;
            case KeyF20: keyBoard->Actions &= ~KeyBoardIDF20; break;
            case KeyF21: keyBoard->Actions &= ~KeyBoardIDF21; break;
            case KeyF22: keyBoard->Actions &= ~KeyBoardIDF22; break;
            case KeyF23: keyBoard->Actions &= ~KeyBoardIDF23; break;
            case KeyF24: keyBoard->Actions &= ~KeyBoardIDF24; break;
            case KeyF25: keyBoard->Actions &= ~KeyBoardIDF25; break;

            default:
                break;
        }
    }

    PXFunctionInvoke(window->KeyBoardKeyCallBack, window->EventReceiver, window, keyBoardKeyInfo);
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
#endif

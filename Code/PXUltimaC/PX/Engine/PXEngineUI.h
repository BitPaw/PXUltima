#pragma once

#ifndef PXEngineUIIncluded
#define PXEngineUIIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXRectangle.h>
#include <PX/Type/PXMouse.h>
#include <PX/Type/PXKeyboard.h>
#include <PX/Type/PXColor.h>
#include <PX/Type/PXIcon.h>

#if OSUnix
#include <X11/Xlib.h>
#elif OSWindows
// ---
#endif


typedef struct PXEngineUI_ PXEngineUI;
typedef struct PXEngineUIEvent_ PXEngineUIEvent;

typedef struct PXEngineUITheme_
{
    PXBrush* BrushMainPrimary;
    PXBrush* BrushMainSecoundary;

    PXBrush* BrushDarkBackground;
    PXBrush* BrushTextWhite;

    PXFont* FontTitle;
    PXFont* FontContent;
}
PXEngineUITheme;

typedef struct PXEngineUIInfo_
{
    PXECSCreateInfo Info;

    PXText Path;

    PXSize MonitorListAmount;
    PXMonitor* MonitorList;

    PXSize DisplayListAmount;
    PXDisplay* DisplayList;

    // To create main window
    PXWindow* WindowRoot;
    //PXText WindowRootName;
}
PXEngineUIInfo;


PXPublic PXResult PXAPI PXEngineUILoadFromFile(PXEngineUI PXREF pxUI, PXText PXREF pxFilePath);

PXPublic PXResult PXAPI PXEngineUICreate(PXEngineUI** pxUIREF, PXEngineUIInfo PXREF pxUIInfo);
PXPublic PXResult PXAPI PXEngineUIRelease(PXEngineUI PXREF pxUI);
PXPublic PXResult PXAPI PXEngineUIUpdate(PXEngineUI PXREF pxEngineUI, PXEngineUpdateInfo PXREF pxEngineUpdateInfo);


PXPublic PXWindow* PXAPI PXEngineUIWindowRootGet(const PXEngineUI PXREF pxEngineUI);
PXPublic PXResult PXAPI PXEngineUIWindowRootSet(PXEngineUI PXREF pxEngineUI, PXWindow PXREF pxWindow);




typedef enum PXEngineUIElementAllign_
{
    PXEngineUIElementAllignInvalid,
    PXEngineUIElementAllignLeft,
    PXEngineUIElementAllignRight,
    PXEngineUIElementAllignCenter
}
PXEngineUIElementAllign;

typedef enum PXEngineUIDockSide_
{
    PXWindowDockSideNone = 0,
    PXWindowDockSideCenter,
    PXWindowDockSideLeft,
    PXWindowDockSideTop,
    PXWindowDockSideRight,
    PXWindowDockSideBottom
}
PXEngineUIDockSide;

typedef enum PXEngineUILayoutMode_
{
    PXLayoutNone,
    PXLayoutVertical,
    PXLayoutHorizontal,
    PXLayoutGrid
}
PXEngineUILayoutMode;


typedef enum PXEngineUIHoverState_
{
    PXEngineUIHoverStateInvalid,
    PXEngineUIHoverStateNotBeeingHovered, // Not beeing hovered
    PXEngineUIHoverStateHovered, // IS beeing hovered
    PXEngineUIHoverStateHoveredButOverlapped // User hovers over this object but its been blocked by other object
}
PXEngineUIHoverState;

typedef enum PXWindowResizeCause_
{
    PXWindowResizeCauseUnkown,
    PXWindowResizeCauseMAXHIDE, // Message is sent to all pop - up windows when some other window is maximized.
    PXWindowResizeCauseMAXIMIZED, // The window has been maximized.
    PXWindowResizeCauseMAXSHOW, // Message is sent to all pop - up windows when some other window has been restored to its former size.
    PXWindowResizeCauseMINIMIZED, // The window has been minimized.
    PXWindowResizeCauseRESTORED // The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
}
PXWindowResizeCause;

























typedef struct PXWindowEventCloseData_
{
    PXBool CommitToClose;
}
PXWindowEventCloseData;

typedef struct PXWindowEventResizeData_
{
    PXI16S Width;
    PXI16S Height;
    PXWindowResizeCause Cause;
}
PXWindowEventResizeData;

typedef struct PXWindowEventMouseWheelData_
{
    PXI32S UpDownDelta;

    PXBool IsPressed_LBUTTON;
    PXBool IsPressed_RBUTTON;
    PXBool IsPressed_SHIFT;
    PXBool IsPressed_CONTROL;
    PXBool IsPressed_MBUTTON;
    PXBool IsPressed_XBUTTON1;
    PXBool IsPressed_XBUTTON2;
}
PXWindowEventMouseWheelData;

typedef struct PXWindowEventMouseButtonData_
{
    PXKeyPressState PressState;
    PXMouseButton Button;

    PXVector2I32S Position;
}
PXWindowEventMouseButtonData;

typedef struct PXWindowEventSelectData_
{
    PXWindow* UIElementSelected;
}
PXWindowEventSelectData;

typedef struct PXWindowEventMouseMoveData_
{
    PXVector2F32 Axis;
    PXVector2I32S Delta;
    PXVector2I32S Position;

    PXBool IsPressed_LBUTTON;
    PXBool IsPressed_RBUTTON;
    PXBool IsPressed_SHIFT;
    PXBool IsPressed_CONTROL;
    PXBool IsPressed_MBUTTON;
    PXBool IsPressed_XBUTTON1;
    PXBool IsPressed_XBUTTON2;

    PXBool IsOwnedByProcess;
    //PXWindowHandle WindowHandleHovered;
    PXWindow* WindowHovered;
}
PXWindowEventMouseMoveData;

typedef struct PXWindowEventInputKeyboard_
{
    PXKeyPressState PressState;
    PXVirtualKey VirtualKey;

    union
    {
        char CharacterA;
        wchar_t CharacterW;
    };

    PXI16U CharacterID;
}
PXWindowEventKeyboardData;


typedef struct PXEngineUIDrawInfo_
{
    //-------------------------
    // Set by engine
    PXWindow* Window;

    PXEngine* Engine;
    PXEngineRender* Render;
    PXScene* Scene;
    PXCamera* Camera;
    PXEngineUITheme* UITheme;

    //_____________________________________________________
    // Set by imediate rendering
    PXBool IsCursorInWindow;
    PXVector2I32S CursorPositionLocal;
    PXVector2I32S CursorPositionGlobal;
    PXRectangleXYWHI32 ClientArea; // Where we render into
    //-----------------------------------------------------



    void* Owner; // User defined payload
    PXECSDrawFunction DrawCall;

#if OSUnix
    int ScreenIDHandle;
    Display* DisplayHandle;
    Drawable WindowIDHandle;
    GC GraphicContntainerHandle;
#elif OSWindows
#endif

    //PXText Content;

    PXI32U Behaviour;

    // Color
    PXBrush* Brush;
    PXColorRGB_U8 Color;

    // Rectangle
    PXBool Rounded;

    // Scene
    PXF32 AspectRatio;

    // Text
    PXText* Text;
    PXF32 Size;
    PXF32 OffsetX;
}
PXEngineUIDrawInfo;









typedef enum PXWindowEventType_
{
    PXWindowEventTypeInvalid,

    PXWindowEventTypeElementMove,
    PXWindowEventTypeElementResize,
    PXWindowEventTypeElementDestroy,
    PXWindowEventTypeElementClose,
    PXWindowEventTypeElementCreate,
    PXWindowEventTypeElementSelect,
    PXWindowEventTypeElementClick,
    PXWindowEventTypeElementFocusEnter,
    PXWindowEventTypeElementFocusLeave,

    PXWindowEventTypeInputMouseButton,
    PXWindowEventTypeInputMouseMove,
    PXWindowEventTypeInputKeyboard,


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

typedef struct PXEngineUIEvent_ PXEngineUIEvent;

typedef void (PXAPI* PXWindowEventFunction)(PXEngineUIEvent PXREF pxWindowEvent);

typedef struct PXEngineUIEvent_
{
    // Calls are generated in the OS-API layer.
    // Wrappers are required, this is an internal callback for translation.
    PXWindowEventFunction CallBack;

    union
    {
        PXWindowEventCloseData Close;
        PXWindowEventResizeData Resize;
        // Mouse
        PXWindowEventMouseButtonData MouseButton;
        PXWindowEventMouseMoveData MouseMove;
        PXWindowEventMouseWheelData MouseWheel;

        PXWindowEventKeyboardData Keyboard;
        PXWindowEventSelectData Select;
        PXEngineUIDrawInfo Draw;
    };

    //-----------------------------
    // Original data
    //-----------------------------
#if OSUnix
    XEvent* EventData;
    Display* XDisplayHandle;
#elif OSWindows
    HWND WindowHandle;
    UINT EventID;
    WPARAM ParamW;
    LPARAM ParamL;

    PXBool DoReturnValue;
    LRESULT ReturnValue;
#endif

    void* Owner; // Custom type, user defined payload.

    //-----------------------------
    // Translated Data
    //-----------------------------
    PXWindowEventType Type;

    PXWindow* WindowSender; // Source of the event

    PXMouse* DeviceMouse;
    PXKeyBoard* DeviceKeyboard;
}
PXEngineUIEvent;
















typedef enum PXUIContentPack_ {
    PACK_COLUMN,
    PACK_ROW,
    PACK_GRID
} PackMethod;

typedef enum {
    SIZE_PERCENT,
    SIZE_FIXED,
    SIZE_AUTO
} SizeRule;

typedef enum {
    ALIGN_START,
    ALIGN_CENTER,
    ALIGN_END
} Alignment;

typedef enum {
    WIDGET_CONTAINER,
    WIDGET_BUTTON,
    WIDGET_SLIDER,
    WIDGET_INPUT_FIELD
} WidgetType;


typedef struct PXEngineUIWindow_ UIWindow;

typedef struct PXEngineUIWindow_ 
{
    const char* title;
    WidgetType type;

    PackMethod pack_method;
    SizeRule size_rule_width;
    SizeRule size_rule_height;
    float requested_width;
    float requested_height;
    Alignment alignment;

    PXRectangleXYWHF32 computed_rect;
    PXRectangleXYWHF32 content_rect;

    float value;
    char text[260];

    PXBool is_focused;
    PXBool is_hovered;
    PXBool is_selected;
    PXBool is_enabled;
    PXBool is_popped_out;
    PXBool is_dragging;
    PXBool accepts_children;

    float floating_x;
    float floating_y;

    UIWindow** children;
    int child_count;
    UIWindow* parent;

    //PXWindow* Window; ??
    PXColorRGBA_F32 ColorBackGround;
    PXBool DoRenderingChildren;
    const char* title_text;    // NULL = uses text placeholder
    unsigned int icon_texture; // 0 = no texture, draws color placeholder
    int button_count;          // e.g., 1 for just [X], 3 for [ _ ][ [] ][ X ]
    int is_window_focused;     // Uses a different theme color if active
    int TitleBarHeight;    // legacy; used as fallback min height
} 
UIWindow;

typedef struct UIState 
{
    int mouse_x;
    int mouse_y;
    int drag_start_x;
    int drag_start_y;
    PXBool mouse_pressed;

    UIWindow* hovered_window;
    UIWindow* focused_window;
    UIWindow* active_dragging_window;
    UIWindow* potential_drop_target;

    UIWindow* resizing_splitter_parent;
    int resizing_splitter_index;
    PXBool is_resizing_splitter;

    UIWindow* selected_windows;
    int selected_count;

    PXBool debug_show_hitboxes;
    float global_zoom;

    PXBool menu_active;
    float menu_x;
    float menu_y;
    struct UIWindow* menu_target;
} UIState;



PXPublic void PXAPI PXEngineUIWindowAdd(PXEngineUI PXREF pxEngineUI, UIWindow PXREF pxWindow);




PXPublic PXResult PXAPI PXEngineUIWindowResolve(PXEngineUI PXREF pxUI, PXECSInfo** pxECSInfo, PXText PXREF pxTextName);

PXPrivate PXResult PXAPI PXEngineUICreateContext(PXEngineUI PXREF pxUI);

PXPublic void PXAPI PXEngineUIScreenSizeGet(PXRectangleXYWHI32 PXREF pxRectangleXYWH);


PXPublic PXResult PXAPI PXEngineUICursorPosition(PXVector2I32S PXREF position, const PXBool isWrite);






PXPublic PXSize PXAPI PXEngineUIEventPendingAmount(void);
PXPublic PXResult PXAPI PXEngineUIEventPoll(void);

// Events to capture.
// The OS spawns some events and we will call them here.
// The user may call these functions for fake input

PXPublic PXResult PXAPI PXEngineUIOnWindowCreate(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_CREATE
PXPublic PXResult PXAPI PXEngineUIOnWindowSizeChanged(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);// WM_SIZE
PXPublic PXResult PXAPI PXEngineUIOnWindowSizeChanging(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_SIZING:
PXPublic PXResult PXAPI PXEngineUIOnWindowMoved(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnWindowClosing(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnWindowClosed(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);

PXPublic PXResult PXAPI PXEngineUIOnMouseMoved(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_MOUSEMOVE
PXPublic PXResult PXAPI PXEngineUIOnMouseEnter(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnMouseClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnMouseLeave(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonRightDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_RBUTTONDOWN
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonRightUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_RBUTTONUP
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonRightDoubleClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); 
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonLeftDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_LBUTTONDOWN
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonLeftUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_LBUTTONUP
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonLeftDoubleClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonMiddleDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonMiddleUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); 
PXPublic PXResult PXAPI PXEngineUIOnMouseButtonMiddleDoubleClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);

PXPublic PXResult PXAPI PXEngineUIOnKeyboardKeyUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnKeyboardKeyDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);

PXPublic PXResult PXAPI PXEngineUIOnKeyboardInput(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent);
PXPublic PXResult PXAPI PXEngineUIOnDraw(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent); // WM_PAINT


//---------------------------------------------------------
// WINDOW-EVENT

// This function consumes events first before any other
// listener revices this. Because of this, this event handler
// shall only handle low-level events and not be too much in the way
#if 0
PXPublic PXResult PXAPI PXWindowEventConsumer(PXWindowEvent PXREF pxWindowEvent);
#endif
//---------------------------------------------------------


// Internal draw?
PXPublic void PXAPI PXEngineUIDrawPanel(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo);












//PXPrivate void PXMathCircle(PXColorCircle PXREF pxColorCircle);

// UNSORTERD
PXPublic PXResult PXAPI PXWindowTabListSwapPage(PXWindow PXREF pxWindow);


#if OSUnix
PXInternal int PXAPI PXEngineUICallBackHandler(Display PXREF displayHandle, XErrorEvent PXREF xErrorEvent);
PXPublic PXEngineUI* PXAPI PXEngineUIDisplayInstance(void);

PXPublic Display* PXAPI PXEngineUIDisplayGet(PXEngineUI PXREF pxUI);

PXPublic int PXAPI PXEngineUIScreenIDDefaultGet(PXEngineUI PXREF pxUI);
#elif OSWindows
// ??
#endif





PXPublic PXResult PXAPI PXEngineUIDrawRectangle2D(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxWindowDrawRectangleInfo);

PXPublic PXResult PXAPI PXEngineUIDrawRectangle3D_GL(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXEngineUIDrawRectangle3D_GDI(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxWindowDrawRectangleInfo);


PXPublic PXResult PXAPI PXTextDrawInfoRowNext(PXEngineUIDrawInfo PXREF pxTextDrawInfo);




PXPublic PXResult PXAPI PXEngineUIDrawText
(
    PXWindow PXREF pxWindow,
    PXEngineUIDrawInfo PXREF pxTextDrawInfo
);
PXPrivate PXResult PXAPI PXWindowDrawTextNative
(
    PXWindow PXREF pxWindow,
    PXEngineUIDrawInfo PXREF pxTextDrawInfo
);
PXPrivate PXResult PXAPI PXWindowDrawTextGLFF
(
    PXWindow PXREF pxWindow,
    PXEngineUIDrawInfo PXREF pxTextDrawInfo
);

PXPublic PXResult PXAPI PXWindowDrawPoint(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y);
PXPublic PXResult PXAPI PXWindowDrawPoints(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);
PXPublic PXResult PXAPI PXWindowDrawLine(PXWindow PXREF pxWindow, const PXI32S x1, const PXI32S y1, const PXI32S x2, const PXI32S y2);
PXPublic PXResult PXAPI PXWindowDrawLines(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);

PXPublic PXResult PXAPI PXWindowDrawHelloWorldTriangle(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);

PXPublic PXResult PXAPI PXWindowDrawRect(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);
PXPublic PXResult PXAPI PXWindowDrawRect2(PXWindow PXREF pxWindow, const PXRectangleXYWHI32 PXREF pxRectangle);

PXPublic PXResult PXAPI PXWindowDrawPanel
(
    PXWindow PXREF pxWindow,
    PXRectangleXYWHI32 PXREF rectangle,
    PXRectangleLTRBF32 PXREF margin
);

PXPublic void PXAPI PXWindowDrawRectangleGL(PXRectangleXYWHI32 PXREF rectangle, PXColorRGBA_F32 PXREF pxColor);
PXPublic void PXAPI PXWindowDrawRectangleGL2
(
    const PXF32 x,
    const PXF32 y,
    const PXF32 w,
    const PXF32 h,
    const PXF32 r,
    const PXF32 g,
    const PXF32 b
);
PXPublic void PXAPI PXWindowDrawRectangleWireGL2
(
    const PXF32 x,
    const PXF32 y,
    const PXF32 w,
    const PXF32 h,
    const PXF32 r,
    const PXF32 g,
    const PXF32 b,
    const PXF32 thickness
);
PXPublic void PXAPI PXWindowDrawRectangleWireGL(PXRectangleXYWHI32 PXREF rectangle, PXF32 r, PXF32 g, PXF32 b, PXF32 thickness);

PXPublic PXResult PXAPI PXWindowDrawRectangle(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxWindowDrawRectangleInfo);

PXPublic PXResult PXAPI PXEngineUIDrawIcon_GDI(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, PXIcon PXREF pxIcon);
PXPublic PXResult PXAPI PXEngineUIDrawIcon_GL(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, PXIcon PXREF pxIcon);









// NEW LAXPUT
PXPublic PXBool UI_PointInRect(int px, int py, PXRectangleXYWHF32 PXREF rect);

PXPublic void UI_FindPotentialDropTarget(PXEngineUI PXREF pxEngineUI, UIWindow* root, int mx, int my, UIState* state);
PXPublic void UI_CheckSplitterHover(PXEngineUI PXREF pxEngineUI, UIWindow* win, int mx, int my, UIState* state);
PXPublic void UI_RemoveChild(PXEngineUI PXREF pxEngineUI, UIWindow* parent, UIWindow* child);
PXPublic void UI_AddChild(PXEngineUI PXREF pxEngineUI, UIWindow* parent, UIWindow* child);

PXPublic void UI_InitState(PXEngineUI PXREF pxEngineUI, UIState* state);
PXPublic void UI_ComputeLayout(PXEngineUI PXREF pxEngineUI, UIWindow* win, float x, float y, float w, float h, UIState* state);
PXPublic void UI_HandleMouseMove(PXEngineUI PXREF pxEngineUI, UIWindow PXREF g_root_workspace, UIWindow* root, int mx, int my, UIState* state);
PXPublic void UI_HandleMouseClick(PXEngineUI PXREF pxEngineUI, UIWindow PXREF g_root_workspace, UIWindow* root, int mx, int my, PXBool is_down, PXBool ctrl_pressed, UIState* state);
PXPublic void UI_RenderWindow(PXEngineUI PXREF pxEngineUI, UIWindow* win, UIState* state);




#endif
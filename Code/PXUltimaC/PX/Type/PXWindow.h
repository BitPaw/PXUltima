#pragma once

#ifndef PXWindowIncluded
#define PXWindowIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXFont.h>
#include <PX/Type/PXMouse.h>
#include <PX/Type/PXKeyboard.h>
#include <PX/Type/PXRectangle.h>
#include <PX/Type/PXIcon.h>
#include <PX/Type/PXCursor.h>
#include <PX/Type/PXColor.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>

#if OSUnix
#include <X11/Xlib.h> // Display*
#endif


typedef struct PXOpenGLContext_
{
#if OSUnix
    GLXContext Data; // <GL/glx.h>
#elif OSWindows
    HGLRC Data; // <windef.h>
#endif
}
PXOpenGLContext;



// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXWindow_ PXWindow;
typedef struct PXWindowEvent_ PXWindowEvent;













//-----------------------------------------------------
// UI-Element
//-----------------------------------------------------

// Ancering will stick the given edge to a side.
// Offset will be interpretet 0=NoSpace, 1=???
// Goal: Scale the object with screensize
#define PXWindowAncerParent           0b00000000000000000000000000001111
#define PXWindowAncerParentLeft       0b00000000000000000000000000000001
#define PXWindowAncerParentTop        0b00000000000000000000000000000010
#define PXWindowAncerParentRight      0b00000000000000000000000000000100
#define PXWindowAncerParentBottom     0b00000000000000000000000000001000

// Let siblings calulate their offset themself.
// Goal: Group multible objects together that belong together
#define PXWindowAncerSibling          0b00000000000000000000000011110000
#define PXWindowAncerSiblingLeft      0b00000000000000000000000000010000
#define PXWindowAncerSiblingTop       0b00000000000000000000000000100000
#define PXWindowAncerSiblingRight     0b00000000000000000000000001000000
#define PXWindowAncerSiblingBottom    0b00000000000000000000000010000000

#define PXWindowKeepFlags             0b00000000000000000000111100000000
#define PXWindowKeepPositionX         0b00000000000000000000000100000000
#define PXWindowKeepPositionY         0b00000000000000000000001000000000
#define PXWindowKeepWidth             0b00000000000000000000010000000000
#define PXWindowKeepHeight            0b00000000000000000000100000000000

// Allign content inside a element
#define PXWindowAllignFlags                 0b00000000000000011111000000000000
#define PXWindowAllignLeft                  0b00000000000000000001000000000000
#define PXWindowAllignTop                   0b00000000000000000010000000000000
#define PXWindowAllignRight                 0b00000000000000000100000000000000
#define PXWindowAllignBottom                0b00000000000000001000000000000000
#define PXWindowAllignCenter                0b00000000000000010000000000000000

#define PXWindowAllignIgnoreParentMargin    0b00000000100000000000000000000000

// Interaction
#define PXWindowBehaviourInteractMask       0b11111111000000000000000000000000
#define PXWindowBehaviourHoverable          0b00000001000000000000000000000000
#define PXWindowBehaviourIsBeingHovered     0b00000010000000000000000000000000
#define PXWindowBehaviourSelectable         0b00000100000000000000000000000000
#define PXWindowBehaviourIsBeeingSelected   0b00001000000000000000000000000000
#define PXWindowBehaviourBorder             0b00010000000000000000000000000000
#define PXWindowBehaviourScrollBarHor       0b00100000000000000000000000000000
#define PXWindowBehaviourScrollBarVer       0b01000000000000000000000000000000

#define PXWindowBehaviourDefaultKeepAspect   PXWindowKeepWidth | PXWindowKeepHeight
#define PXWindowBehaviourDefaultInputNormal  PXWindowBehaviourSelectable | PXWindowBehaviourHoverable
#define PXWindowBehaviourDefaultText         PXWindowKeepHeight | PXWindowAllignCenter
#define PXWindowBehaviourDefaultBuffer       PXWindowBehaviourDefaultKeepAspect

#endif


typedef enum PXUIHoverState_
{
    PXUIHoverStateInvalid,
    PXUIHoverStateNotBeeingHovered, // Not beeing hovered
    PXUIHoverStateHovered, // IS beeing hovered
    PXUIHoverStateHoveredButOverlapped // User hovers over this object but its been blocked by other object
}
PXUIHoverState;

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




typedef enum PXWindowPropertyType_
{
    PXWindowPropertyTypeInvalid,
    PXWindowPropertyTypeTextContent,
    PXWindowPropertyTypeAllign,
    PXWindowPropertyTypeTextColor,
    PXWindowPropertyTypeSize,
    PXWindowPropertyTypeSizeParent,

    PXWindowPropertyTypeSliderPercentage,

    PXWindowPropertyTypeProgressbarPercentage,
    PXWindowPropertyTypeProgressbarBarColor,

    PXWindowPropertyTypeBackGroundColor,

    PXWindowPropertyTypeVisibility,

    PXWindowPropertyTypeComboBoxAdd,


    PXWindowPropertyTypeItemAdd,
    PXWindowPropertyTypeItemDelete,
    PXWindowPropertyTypeItemUpdate,

    // Tree view

    PXWindowPropertyTypeTreeView_CreateDragImage,
    PXWindowPropertyTypeTreeView_DeleteAllItems,
    PXWindowPropertyTypeTreeView_DeleteItem,
    PXWindowPropertyTypeTreeView_EditLabel,
    PXWindowPropertyTypeTreeView_EndEditLabelNow,
    PXWindowPropertyTypeTreeView_EnsureVisible,
    PXWindowPropertyTypeTreeView_Expand,
    PXWindowPropertyTypeTreeView_GetBkColor,
    PXWindowPropertyTypeTreeView_GetCheckState,
    PXWindowPropertyTypeTreeView_GetChild,
    PXWindowPropertyTypeTreeView_GetCount,
    PXWindowPropertyTypeTreeView_GetDropHilight,
    PXWindowPropertyTypeTreeView_GetEditControl,
    PXWindowPropertyTypeTreeView_GetExtendedStyle,
    PXWindowPropertyTypeTreeView_GetFirstVisible,
    PXWindowPropertyTypeTreeView_GetImageList,
    PXWindowPropertyTypeTreeView_GetIndent,
    PXWindowPropertyTypeTreeView_GetInsertMarkColor,
    PXWindowPropertyTypeTreeView_GetISearchString,
    PXWindowPropertyTypeTreeView_GetItem,
    PXWindowPropertyTypeTreeView_GetItemHeight,
    PXWindowPropertyTypeTreeView_GetItemPartRect,
    PXWindowPropertyTypeTreeView_GetItemRect,
    PXWindowPropertyTypeTreeView_GetItemState,
    PXWindowPropertyTypeTreeView_GetLastVisible,
    PXWindowPropertyTypeTreeView_GetLineColor,
    PXWindowPropertyTypeTreeView_GetNextItem,
    PXWindowPropertyTypeTreeView_GetNextSelected,
    PXWindowPropertyTypeTreeView_GetNextSibling,
    PXWindowPropertyTypeTreeView_GetNextVisible,
    PXWindowPropertyTypeTreeView_GetParent,
    PXWindowPropertyTypeTreeView_GetPrevSibling,
    PXWindowPropertyTypeTreeView_GetPrevVisible,
    PXWindowPropertyTypeTreeView_GetRoot,
    PXWindowPropertyTypeTreeView_GetScrollTime,
    PXWindowPropertyTypeTreeView_GetSelectedCount,
    PXWindowPropertyTypeTreeView_GetSelection,
    PXWindowPropertyTypeTreeView_GetTextColor,
    PXWindowPropertyTypeTreeView_GetToolTips,
    PXWindowPropertyTypeTreeView_GetUnicodeFormat,
    PXWindowPropertyTypeTreeView_GetVisibleCount,
    PXWindowPropertyTypeTreeView_HitTest,
    PXWindowPropertyTypeTreeView_InsertItem,
    PXWindowPropertyTypeTreeView_MapAccIDToHTREEITEM,
    PXWindowPropertyTypeTreeView_MapHTREEITEMtoAccID,
    PXWindowPropertyTypeTreeView_Select,
    PXWindowPropertyTypeTreeView_SelectDropTarget,
    PXWindowPropertyTypeTreeView_SelectItem,
    PXWindowPropertyTypeTreeView_SelectSetFirstVisible,
    PXWindowPropertyTypeTreeView_SetAutoScrollInfo,
    PXWindowPropertyTypeTreeView_SetBkColor,
    PXWindowPropertyTypeTreeView_SetBorder,
    PXWindowPropertyTypeTreeView_SetCheckState,
    PXWindowPropertyTypeTreeView_SetExtendedStyle,
    PXWindowPropertyTypeTreeView_SetHot,
    PXWindowPropertyTypeTreeView_SetImageList,
    PXWindowPropertyTypeTreeView_SetIndent,
    PXWindowPropertyTypeTreeView_SetInsertMark,
    PXWindowPropertyTypeTreeView_SetInsertMarkColor,
    PXWindowPropertyTypeTreeView_SetItem,
    PXWindowPropertyTypeTreeView_SetItemHeight,
    PXWindowPropertyTypeTreeView_SetItemState,
    PXWindowPropertyTypeTreeView_SetLineColor,
    PXWindowPropertyTypeTreeView_SetScrollTime,
    PXWindowPropertyTypeTreeView_SetTextColor,
    PXWindowPropertyTypeTreeView_SetToolTips,
    PXWindowPropertyTypeTreeView_SetUnicodeFormat,
    PXWindowPropertyTypeTreeView_ShowInfoTip,
    PXWindowPropertyTypeTreeView_SortChildren,
    PXWindowPropertyTypeTreeView_SortChildrenCB,

}
PXWindowPropertyType;

typedef enum PXUIElementTreeViewItemInsertMode_
{
    PXUIElementTreeViewItemInsertModeROOT,
    PXUIElementTreeViewItemInsertModeFIRST,
    PXUIElementTreeViewItemInsertModeLAST,
    PXUIElementTreeViewItemInsertModeSORT
}
PXUIElementTreeViewItemInsertMode;

typedef enum PXUIElementType_
{
    PXUIElementTypeInvalid,
    PXUIElementTypeCustom,
    PXUIElementTypeWindow, // General window to draw into
    PXUIElementTypePanel, // static generic element
    PXUIElementTypeText, // text, letters
    PXUIElementTypeButton, // Button to click
    PXUIElementTypeImage,
    PXUIElementTypeDropDown,
    PXUIElementTypeListBox,
    PXUIElementTypeTextEdit,
    PXUIElementTypeRichEdit,
    PXUIElementTypeScrollBar,
    PXUIElementTypeTrackBar,
    PXUIElementTypeStatusBar,
    PXUIElementTypeUpDown,
    PXUIElementTypeProgressBar,
    PXUIElementTypeHotKey,
    PXUIElementTypeCalender,
    PXUIElementTypeToolTip,
    PXUIElementTypeAnimate,
    PXUIElementTypeDatePicker,
    PXUIElementTypeGroupBox,
    PXUIElementTypeRadioButton,
    PXUIElementTypeGroupRadioButton,
    PXUIElementTypeTreeView,
    PXUIElementTypeTreeViewItem,
    PXUIElementTypeFileDirectyView,
    PXUIElementTypeFileDirectyViewEntry,
    PXUIElementTypeIPInput,
    PXUIElementTypeLink,
    PXUIElementTypeHeader, // Object insode another for header info
    PXUIElementTypeFontSelector,
    PXUIElementTypePageScroll,
    PXUIElementTypeTabControll,
    PXUIElementTypeTabPage,
    PXUIElementTypeToggle,
    PXUIElementTypeCheckBox,
    PXUIElementTypeComboBox,
    PXUIElementTypeColorPicker,
    PXUIElementTypeSlider,
    PXUIElementTypeImageList,
    PXUIElementTypeRenderFrame,

    PXUIElementTypeMenuStrip, // Topline of a window that contains a selectable list of things

    PXUIElementTypeScene,
    PXUIElementTypeCode,
    PXUIElementTypeHexEditor,
    PXUIElementTypeGraphBehaviour,
    PXUIElementTypeGraphTime,
    PXUIElementTypeSoundPlayerMixer,
    PXUIElementTypeVideoCutter,
    PXUIElementTypeDataBaseManager,
    PXUIElementTypeNetworkTester,
    PXUIElementTypeInputView,
    PXUIElementTypeHardwareInfo,

    PXUIElementTypeFileManager,    // TreeView of filesystem with utility
    PXUIElementTypeResourceManger, // TreeView of a list of resource entrys
    PXUIElementTypeResourceEntry,  // Panel with name and icons of type and propertys
    PXUIElementTypeResourceInfo    // Resource info
}
PXUIElementType;

typedef enum PXUIElementAllign_
{
    PXUIElementAllignInvalid,
    PXUIElementAllignLeft,
    PXUIElementAllignRight,
    PXUIElementAllignCenter
}
PXUIElementAllign;















typedef enum PXWindowDockSide_
{
    PXWindowDockSideNone = 0,
    PXWindowDockSideCenter,
    PXWindowDockSideLeft,
    PXWindowDockSideTop,
    PXWindowDockSideRight,
    PXWindowDockSideBottom
}
PXWindowDockSide;


typedef enum PXLayoutMode_
{
    PXLayoutNone,
    PXLayoutVertical,
    PXLayoutHorizontal,
    PXLayoutGrid
}
PXLayoutMode;

typedef struct PXWindowLayout_
{
    //-----------------------------------------------------
    // Rectangles, calculated by engine
    PXRectangleXYWHI32 SpaceDraw; // Total drawable space
    PXRectangleXYWHI32 SpaceBox; // Total drawable space
    PXRectangleXYWHI32 SpaceContent; // Space where we render stuff into
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Settings, provided by user
    PXRectangleLTRBI32 Padding;    // (inside pane) l,t,w,h used as padding (or 4 floats if you prefer)
    PXRectangleLTRBI32 Margin;     // same idea
    PXI8U BorderWidth;
    //-----------------------------------------------------

    PXBool AutoWidth;
    PXBool AutoHeight;

    PXLayoutMode LayoutMode;
    int SpacingX;
    int SpacingY;
    int GridColumns;
}
PXWindowLayout;

typedef struct PXWindowState_
{
    // Hold same as "Visible", but with more behaviour
    PXBool DoRenderingSelf;
    PXBool DoRenderingChildren;

    PXBool Hover;
    PXBool Active;
    PXBool Dirty;              // needs redraw
    PXBool IsInFocus;

    PXWindowDockSide  DockSide;     // Docking behaviour
    PXBool Floating;   // if TRUE, excluded from docking layout
}
PXWindowState;


typedef void (PXAPI* PXWindowEventFunction)(PXWindowEvent PXREF pxWindowEvent);

typedef struct PXWindowEventList_
{
    PXECSInfo* CallBackOwner; // Object to reley back
    PXECSDrawFunction CallBackDraw; // Windows:WM_PAINT event
    PXWindowEventFunction CallBackEvent; // Different I/O Events
}
PXWindowEventList;


typedef struct PXDrawInfo_
{
    //-------------------------
    // Set by engine
    PXWindow* Window;

    PXEngine* Engine;
    PXRenderSystem* Rendrer;
    PXScene* Scene;
    PXCamera* Camera;
    PXUITheme* UITheme;
    //-------------------------

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

    // Input
    PXVector2I32S CursorPosition;


    // Color
    PXBrush* Brush;
    PXColorRGBI8 Color;

    // Rectangle
    PXBool Rounded;

    // Scene
    PXF32 AspectRatio;

    // Text
    PXText* Text;
    PXF32 Size;
    PXF32 OffsetX;
}
PXDrawInfo;






#if OSWindows
typedef struct BoxInsets
{
    int l, t, r, b;
}
BoxInsets;


typedef struct DockLayoutConfig {
    int defaultBandW;     // default LEFT/RIGHT thickness
    int defaultBandH;     // default TOP/BOTTOM thickness
    int defaultSpacing;   // default spacing between siblings
    BoxInsets padLeft;    // padding inside LEFT band area
    BoxInsets padRight;   // padding inside RIGHT band area
    BoxInsets padTop;     // padding inside TOP band area
    BoxInsets padBottom;  // padding inside BOTTOM band area
    BoxInsets padCenter;  // padding inside CENTER area
    PXBool centerVertical;  // TRUE => center stacks vertically; FALSE => horizontally
} DockLayoutConfig;





void SubSegment(RECT* m, RECT* r, PXWindowDockSide side);

// For allocation convenience
typedef struct Paneref
{
    HWND h;
    PXWindow* p;
}
Paneref;

typedef struct DockOverlay
{
    BOOL     active;
    PXWindowDockSide side;
    RECT     rect;        // rectangle of preview area
}
DockOverlay;

static DockOverlay gOverlay = { 0 };







void LayoutDockedAA(PXWindow PXREF pxWindow);
void LayoutDockedGOOD(PXWindow PXREF pxWindow);
DockLayoutConfig DockDefaultConfig(void);
void InitDockContainerStyles(PXWindow PXREF pxWindow);
void InitDockChildStyles(PXWindow PXREF pxWindow);
int clamp_nonneg(int v);
int CountVisiblePanes(HWND hParent);
int CollectSide(HWND hParent, PXWindowDockSide side, Paneref* buf, int cap);
void DistributeAxisWithBox(Paneref* arr, int n, int availableAxis, int spacing, BOOL vertical, int* outSlotSizes /* n */);
void ApplyStackWithBox(HWND hParent, HDWP* phdwp, RECT area, BOOL vertical, Paneref* arr, int n, int spacing);
RECT InsetRectBy(RECT r, BoxInsets in);
void LayoutDockedEx(PXWindow PXREF hMain, const DockLayoutConfig* pCfg);


BOOL PXAPI PXWindowScreenPtInMainClient(PXWindow PXREF pxWindow, PXVector2I32S PXREF pxVector2I32S);
PXWindowDockSide PXAPI ChooseDockSide(PXWindow PXREF pxWindow, PXVector2I32S PXREF pxVector2I32S);
RECT PXAPI OverlayRectForSide(PXWindow PXREF pxWindow, PXWindowDockSide side);
void PXAPI PXWindowShowDockOverlay(PXWindow PXREF pxWindow, PXWindowDockSide side);
void PXAPI PXWindowHideDockOverlay(PXWindow PXREF pxWindow);
void PXAPI PXWindowTearOffToFloating(PXWindow* p, POINT startScreen);
void PXAPI PXWindowDockBackToChild(PXWindow* p, PXWindowDockSide side);
void PXAPI PXWindowDrawAlphaRect(HDC hdc, RECT rc, COLORREF color, BYTE alpha);
void PXAPI PXWindowDrawOverlay(HDC hdc, const DockOverlay* ov);
void PXAPI PXWindowDrawScene(void);

void PXAPI PXWindowPaintPattern(HWND hwnd, HDC hdc, RECT* rect, PXColorRGBI8 PXREF color);
void PXAPI PXWindowPaintMain(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);


#endif




#if 1






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




typedef struct PXWindowEvent_
{
    // Calls are generated in the OS-API layer.
    // Wrappers are required, this is an internal callback for translation.
    PXWindowEventFunction CallBack;

    union
    {
        PXWindowEventCloseData Close;
        PXWindowEventResizeData Resize;
        PXWindowEventMouseButtonData MouseButton;
        PXWindowEventMouseMoveData MouseMove;
        PXWindowEventKeyboardData Keyboard;
        PXWindowEventSelectData Select;
        PXDrawInfo Draw;
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
PXWindowEvent;


typedef struct PXWindowPixelSystemInfo_
{
    PXI8U BitPerPixel; // 32=8Bit Default

    PXBool OpenGL;
    PXBool DirectX;
    PXBool GDI;
}
PXWindowPixelSystemInfo;



#define PXScollbarBehaviourVertical     (1<<0)
#define PXScollbarBehaviourHorrizontal  (1<<1)
#define PXScollbarBehaviourBoth         PXScollbarBehaviourHorrizontal | PXScollbarBehaviourVertical

typedef struct PXScollbar_
{
    PXECSInfo Info;
}
PXScollbar;














typedef struct PXWindowCreateInfo_ PXWindowCreateInfo;

typedef PXResult(PXAPI* PXWindowCreateHelper)(PXWindowCreateInfo PXREF pxWindowCreateInfo);

typedef PXResult(PXAPI* PXWindowCreateFunction)(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);





typedef enum PXUIElementInteractType_
{
    PXUIElementInteractTypeInvalid,
    PXUIElementInteractTypeSelect,
    PXUIElementInteractTypeClick,
    PXUIElementInteractTypeMouseEnter,
    PXUIElementInteractTypeMouseLeave
}
PXUIElementInteractType;


typedef struct PXUIElementFrameBufferInfo_
{
    struct PXTexture_* TextureReference;

    PXI32U Width;
    PXI32U Height;
    PXI32U BufferID;
    PXI32U RenderID;
}
PXUIElementFrameBufferInfo;

typedef struct PXUIElementImageInfo_
{
    struct PXTexture_* TextureReference;
}
PXUIElementImageInfo;



typedef struct PXUIElementItemInfo_
{
    char* TextData;
    PXSize TextSize;
}
PXUIElementItemInfo;

typedef struct PXUIElementSceneRenderInfo_
{
    struct PXEngine_* Engine;
    struct PXEngineStartInfo_* StartInfo;
}
PXUIElementSceneRenderInfo;



typedef struct PXUIElementProgressBarInfo_
{
    PXF32 Percentage;
    PXColorRGBI8 BarColor;
}
PXUIElementProgressBarInfo;


#if 0
typedef struct PXUIElementPosition_
{
    PXRectangleLTRBF32 Margin;
    PXRectangleLTRBF32 Padding;
    PXRectangleXYWHI32 Form;

    // PXF32 Left;
    // PXF32 Top;
    // PXF32 Right;
    // PXF32 Bottom;

     // PXI16U FlagListFormat; // Unused
 //   PXI8U FlagListAncer;
     // PXI8U FlagListKeep;
}
PXUIElementPosition;
#endif






typedef struct PXWindowHandle_
{
#if OSUnix
    Window Data; // X11
#elif OSWindows
    union
    {
        HWND Data;
        HMENU DataM;
    };
#endif
}
PXWindowHandle;

typedef struct PXWindowPixelBuffer_
{
#if OSUnix
    GC Data;
#elif OSWindows
    HDC Data; // Used by windows for direct context for rendering
#endif
}
PXWindowPixelBuffer;


typedef struct PXWindowCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindow* WindowParent;
    PXText WindowParentName; // Set by config, needs to be translated into actual handle!

    PXDisplay* DisplayCurrent;

    // Positions
    PXWindowLayout Layout;
    PXWindowState State;

    // Style


    PXColorRGBI8 BackGroundColor;


   // BOOL isChild;


    PXBool DoOpenGL;




    // Setings
    PXBool UsePhysical; // Dont create a HANDLE for the window. Nested rendering
    PXBool UseFrameBuffer; // Dont render into a frame but into a framebuffer
    PXBool Invisible; // Hide the window, we still want to create it.


    PXBool Simple;
    PXBool MaximizeOnStart;


    PXWindow* UIElementReference;

    PXWindowEventList EventList;

    //PXI32U FlagList;

    PXUIElementType Type;
    PXI32U Behaviour;
    PXI32U Setting;
    PXColorRGBAF* ColorTintReference;


#if OSUnix
    Display* DisplayHandle;
    int ScreenHandleNumber;
#elif OSWindows
    HDC DeviceContextHandle;
    HINSTANCE InstanceHandle;

    DWORD StyleFlagsExtended;
    DWORD StyleFlags;
#endif

    PXText WindowClassName;
    PXText WindowText;


    PXWindowCreateFunction CreatePre; // Function is called before the physical creation
    PXWindowCreateFunction CreatePost; // Function is called after physical creation


    //PXI8U FLags;

    // PXWindowMenuItemList MenuItem;
     //PXUIElementSceneRenderInfo SceneRender;

        // REMOVE??
    struct PXEngine_* Engine;
    struct PXRenderSystem_* Renderer;
}
PXWindowCreateInfo;


#if 0
typedef struct PXWindowPositionCalulcateInfo_
{
    // Input
    PXF32 WindowWidth;
    PXF32 WindowHeight;

    // Result
    PXI32U DepthCounter;

    PXRectangleLTRBF32 Padding;
    PXRectangleLTRBF32 Margin;

    PXF32 AA;
    PXF32 BA;
    PXF32 BB;
    PXF32 AB;

    PXRectangleXYWHI32 Size;
}
PXWindowPositionCalulcateInfo;
#endif


//PXPublic PXResult PXAPI PXWindowPositionCalculcate(PXWindow PXREF pxWindow, PXWindowPositionCalulcateInfo PXREF pxUIElementPositionCalulcateInfo);


PXPublic const char* PXAPI PXWindowTypeToString(const PXUIElementType pxUIElementType);

PXPublic void PXAPI PXWindowRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

#if OSUnix

#elif OSWindows
PXPublic HDC PXAPI PXWindowDCGet(PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowFromHandle(const HWND windowHandle);
PXPrivate void PXAPI PXWindowStoreSelf(PXWindow PXREF pxWindow, const HWND windowHandle);
#endif


PXPublic PXOpenGLContext PXAPI PXWindowGLContextGet(const PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowGLContextSet(PXWindow PXREF pxWindow, const PXOpenGLContext pxOpenGLContext);

PXPublic PXWindowHandle PXAPI PXWindowHandleGet(const PXWindow PXREF pxWindow);





// Use for a seperate window that needs to be merged into a main one.
// Given a spesific window we can try to absorb the contens and underlieing elemetns and move them into your own space.
// Objects shall not be created or destroyed, simply the ownership of those objects should be transphered. (can we do that?)
//PXPublic PXResult PXAPI PXWindowAbsorb(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);

// Use for draging a window outside it own borders to spawn a new one.
//PXPublic PXResult PXAPI PXWindowEmit(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);


//---------------------------------------------------------
// WINDOW-EVENT

// This function consumes events first before any other
// listener revices this. Because of this, this event handler
// shall only handle low-level events and not be too much in the way
PXPublic PXResult PXAPI PXWindowEventConsumer(PXWindowEvent PXREF pxWindowEvent);


PXPublic void PXAPI PXWindowEventMouseEnter(PXWindowEvent PXREF pxWindowEvent);
PXPublic void PXAPI PXWindowEventMouseLeave(PXWindowEvent PXREF pxWindowEvent);
PXPublic void PXAPI PXWindowEventMouseClick(PXWindowEvent PXREF pxWindowEvent);
PXPublic void PXAPI PXWindowEventMouseMove(PXWindowEvent PXREF pxWindowEvent);

PXPublic void PXAPI PXWindowEventRender(PXWindowEvent PXREF pxWindowEvent);

PXPublic void PXAPI PXWindowEventKeyboardInput(PXWindowEvent PXREF pxWindowEvent);

PXPublic void PXAPI PXWindowEventResize(PXWindowEvent PXREF pxWindowEvent);
PXPublic void PXAPI PXWindowEventMoved(PXWindowEvent PXREF pxWindowEvent);
//---------------------------------------------------------


PXPublic PXWindow* PXAPI PXWindowRootGet(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsRoot(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowOpenGLEnable(PXWindow PXREF pxWindow);

PXPublic const char* PXWindowDockSideToString(const PXWindowDockSide pxWindowDockSide);


PXPublic PXResult PXAPI PXWindowEventPendingAmount(PXSize PXREF amount);
PXPublic PXResult PXAPI PXWindowEventPoll(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowBufferSwap(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowCreate(PXWindow** pxWindowREF, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPublic PXResult PXAPI PXWindowCreatePhysical(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPrivate PXResult PXAPI PXWindowCreatePhysicalEventListenerAdd(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPrivate PXResult PXAPI PXWindowCreatePhysicalEventDragAndDropAdd(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPrivate PXResult PXAPI PXWindowCreatePhysicalSpawn(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPublic PXResult PXAPI PXWindowCreateFrameBuffer(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPublic PXResult PXAPI PXWindowDestroy(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowDraw(PXWindow PXREF pxWindow);



//---------------------------------------------------------
// Hirachy
//---------------------------------------------------------
PXPublic PXSize PXAPI PXWindowSiblingAmount(const PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowSiblingGet(PXWindow PXREF pxWindow, const PXSize index);


PXPublic PXSize PXAPI PXWindowChildAmount(const PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowChildGet(PXWindow PXREF pxWindow, const PXSize index);


PXPublic PXResult PXAPI PXWindowChildAdd(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild);
PXPublic PXResult PXAPI PXWindowChildRemove(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild);

PXPublic PXResult PXAPI PXWindowParentSet(PXWindow PXREF pxWindow, const PXWindow PXREF pxWindowParent);
PXPublic PXWindow* PXAPI PXWindowParentGet(const PXWindow PXREF pxWindow);

// Same function as PXWindowParentGet with added default screen if we dont have any.
// if parrent NULL, get default window (desktop).
#if OSUnix
PXPublic Window PXAPI PXWindowParentHandleGet(const PXWindow PXREF pxWindow, const Display PXREF xDisplay);
#endif

PXPublic PXWindow* PXAPI PXWindowGetViaName(PXText PXREF pxText);

PXPublic PXResult PXAPI PXWindowHitReset(PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowHitTest(PXWindow PXREF pxWindow, float x, float y);

PXPublic PXBool PXAPI PXWindowDoRenderingSelf(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowDoRenderingChildren(const PXWindow PXREF pxWindow);

PXPublic void PXAPI PXWindowLayoutCompute(PXWindow PXREF pxWindow);
//---------------------------------------------------------


PXPublic PXResult PXAPI PXWindowFindViaTitle(PXWindow PXREF pxWindow, const PXText PXREF pxText);

PXPublic PXResult PXAPI PXWindowDragStart(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowPixelSystemSet(PXWindow PXREF pxWindow, PXWindowPixelSystemInfo PXREF pxWindowPixelSystemInfo);

// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindow PXREF pxWindow);

// If a window owns an actual physical handle.
PXPublic PXBool PXAPI PXWindowIsPhysical(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowVisibilitySet
(
    PXWindow PXREF pxWindow,
    const PXBool doRenderingSelf,
    const PXBool doRenderingChildren
);

PXPublic PXResult PXAPI PXWindowShow(const PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowHide(const PXWindow PXREF pxWindow);

// aka mark as dirty
PXPublic PXResult PXAPI PXWindowRedraw(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowRedrawEnable(PXWindow PXREF pxWindow, const PXBool enable);

PXPublic PXResult PXAPI PXWindowDragAndDropBegin(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowDragAndDropEnd(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowDrawBegin(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowDrawEnd(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowScrollbarUpdate(PXWindow PXREF pxWindow, PXScollbar PXREF pxScollbar);

PXPublic PXResult PXAPI PXWindowTitleBarColor(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowForegroundSet(PXWindow PXREF pxWindow, PXColorRGBI8 PXREF pxColorRGBI8);
PXPublic PXResult PXAPI PXWindowBackgroundSet(PXWindow PXREF pxWindow, PXColorRGBI8 PXREF pxColorRGBI8);
PXPublic PXResult PXAPI PXWindowFontSet(PXWindow PXREF pxWindow, PXFont PXREF pxFont);
PXPublic PXResult PXAPI PXWindowBrushSet(PXWindow PXREF pxWindow, PXBrush PXREF pxBrush, const PXI8U mode);
PXPublic PXResult PXAPI PXWindowTextGet(PXWindow PXREF pxWindow, PXText PXREF pxText);
PXPublic PXResult PXAPI PXWindowTextSet(PXWindow PXREF pxWindow, PXText PXREF pxText);
PXPublic PXResult PXAPI PXWindowStyleUpdate(PXWindow PXREF pxWindow);
//---------------------------------------------------------


//---------------------------------------------------------
// Properties
//---------------------------------------------------------
PXPublic PXResult PXAPI PXWindowSpaceDrawGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);
PXPublic PXResult PXAPI PXWindowSpaceBoxGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);
PXPublic PXResult PXAPI PXWindowSpaceContentGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);

PXPublic PXResult PXAPI PXWindowDrawFunctionSet(PXWindow PXREF pxWindow, const PXECSDrawFunction pxECSDrawFunction, void* owner);


PXPublic PXF32 PXAPI PXWindowAspectRatio(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsValid(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsEnabled(const PXWindow PXREF pxWindow);
//---------------------------------------------------------


//---------------------------------------------------------
// DRAW
//---------------------------------------------------------
// glViewPort
PXPublic PXResult PXAPI PXWindowViewPortSet(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, const PXBool normalized);


PXPublic PXResult PXAPI PXWindowDrawRectangle2D(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXWindowDrawRectangle3D(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXWindowDrawClear(PXWindow PXREF pxWindow);


PXPublic PXResult PXAPI PXTextDrawInfoRowNext(PXDrawInfo PXREF pxTextDrawInfo);


PXPublic PXResult PXAPI PXWindowDrawText
(
    PXWindow PXREF pxWindow,
    PXDrawInfo PXREF pxTextDrawInfo
);
PXPrivate PXResult PXAPI PXWindowDrawTextNative
(
    PXWindow PXREF pxWindow,
    PXDrawInfo PXREF pxTextDrawInfo
);
PXPrivate PXResult PXAPI PXWindowDrawTextGLFF
(
    PXWindow PXREF pxWindow,
    PXDrawInfo PXREF pxTextDrawInfo
);

PXPublic PXResult PXAPI PXWindowDrawPoint(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y);
PXPublic PXResult PXAPI PXWindowDrawPoints(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);
PXPublic PXResult PXAPI PXWindowDrawLine(PXWindow PXREF pxWindow, const PXI32S x1, const PXI32S y1, const PXI32S x2, const PXI32S y2);
PXPublic PXResult PXAPI PXWindowDrawLines(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);

PXPublic PXResult PXAPI PXWindowDrawHelloWorldTriangle(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);

PXPublic PXResult PXAPI PXWindowDrawRect(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);
PXPublic PXResult PXAPI PXWindowDrawRect2(PXWindow PXREF pxWindow, const PXRectangleXYWHI32 PXREF pxRectangle);


PXPublic PXResult PXAPI PXWindowDrawRectangle(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXWindowDrawIcon(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo, PXIcon PXREF pxIcon);

PXPublic PXResult PXAPI PXWindowMouseTrack(PXWindow PXREF window);
PXPublic PXResult PXAPI PXWindowMouseMovementEnable(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowCursorCaptureMode(PXWindow PXREF pxWindow, const PXWindowCursorMode cursorMode);
PXPublic PXResult PXAPI PXWindowCursorPositionGet(PXWindow PXREF pxWindow, PXVector2I32S PXREF position);
//---------------------------------------------------------






// Event functions
#if OSUnix

typedef void(PXAPI* PXWindowEventHandlerFunction)(PXWindowEvent PXREF pxWindowEvent);

PXPrivate void PXAPI PXWindowEvent_X_KeyAction(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ButtonAction(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_MotionNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_EnterNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_LeaveNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_FocusIn(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_FocusOut(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_KeymapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_Expose(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_GraphicsExpose(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_NoExpose(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_VisibilityNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_CreateNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_DestroyNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_UnmapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_MapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_MapRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ReparentNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ConfigureNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ConfigureRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_GravityNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ResizeRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_CirculateNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_CirculateRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_PropertyNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_SelectionClear(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_SelectionRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_SelectionNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ColormapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_ClientMessage(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_MappingNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate void PXAPI PXWindowEvent_X_GenericEvent(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
#elif OSWindows

typedef LRESULT(PXAPI* PXWindowEventHandlerFunction)(PXWindowEvent PXREF pxWindowEvent);

PXPrivate LRESULT PXAPI PXWindowEvent_WM_EMPTY(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCCREATE(PXWindow PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DESTROY(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SIZE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CLOSE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SETCURSOR(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NOTIFY(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ERASEBKGND(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_PAINT(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT_DEVICE_CHANGE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEMOVE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSELEAVE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCHITTEST(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DISPLAYCHANGE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CTLCOLOR(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVING(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ENTERSIZEMOVE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_EXITSIZEMOVE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_POWERBROADCAST(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DEVICECHANGE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUTLANGCHANGE(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_COMMAND(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_KEY(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDOWN(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONUP(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDBLCLK(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDOWN(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONUP(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDBLCLK(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDOWN(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONUP(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDBLCLK(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEWHEEL(PXWindowEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_XBUTTON( PXWindowEvent PXREF pxWindowEvent);

#endif













PXPublic PXResult PXAPI PXWindowDisplayListFetch();

//---------------------------------------------------------
// Property
//---------------------------------------------------------
PXPublic PXResult PXAPI PXWindowXYWH(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const PXBool doWrite);
PXPublic PXResult PXAPI PXWindowResize(PXWindow PXREF pxWindow, const int width, const int height);
PXPublic PXResult PXAPI PXWindowMoveAndResize(PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
//---------------------------------------------------------


// Collsison
PXPublic PXResult PXAPI PXWindowCursorCollisionCheck();


/*

PXPublic PXResult PXAPI PXAllPlanes(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXBlackPixel(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWhitePixel(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXConnectionNumber(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultColormap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultDepth(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListDepths(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultRootWindow(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultScreenOfDisplay(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXScreenOfDisplay(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultVisual(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayCells(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayPlanes(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXExtendedMaxRequestSize(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXMaxRequestSize(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLastKnownRequestProcessed(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXNextRequest(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXProtocolVersion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXProtocolRevision(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQLength(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXRootWindow(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXScreenCount(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXServerVendor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXVendorRelease(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListPixmapFormats(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXTextureByteOrder(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXBitmapUnit(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXBitmapBitOrder(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXBitmapPad(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayHeight(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayHeightMM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayWidth(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayWidthMM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXBlackPixelOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWhitePixelOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCellsOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultColormapOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultDepthOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultGCOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultVisualOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDoesBackingStore(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDoesSaveUnders(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXScreenNumberOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXEventMaskOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWidthOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXHeightOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWidthMMOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXHeightMMOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXMaxCmapsOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXMinCmapsOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPlanesOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXRootWindowOfScreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXInitThreads(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAddConnectionWatch(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXRemoveConnectionWatch(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXProcessInternalConnection(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXIernalConnectionNumbers(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXVisualIDFromVisual(PXNativDrawText PXREF pxNativDraw);

PXPublic PXResult PXAPI PXQueryTree(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWindowAttributes(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetGeometry(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXTranslateCoordinates(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryPointer(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXIernAtom(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXIernAtoms(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetAtomName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetAtomNames(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWindowProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListProperties(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetSelectionOwner(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreatePixmap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateFontCursor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateGlyphCursor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreatePixmapCursor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestCursor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateColormap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCopyColormapAndFree(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLookupColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXParseColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsLookupColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsAllocColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocNamedColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsAllocNamedColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocColorCells(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocColorPlanes(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsStoreColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsStoreColors(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryColor(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryColors(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCCCOfColormap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetCCCOfColormap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsDefaultCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsDisplayOfCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsVisualOfCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsScreenNumberOfCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsScreenWhitePointOfCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsClientWhitePointOfCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetWhitePoint(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetCompressionProc(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetWhiteAdjustProc(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCreateCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsFreeCCC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsConvertColors(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStatus(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStatus(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryBlack(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryBlue(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryGreen(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryRed(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryWhite(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMaxC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMaxL(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMaxLC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMinL(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMaxC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMaxL(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMaxLC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMinL(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxV(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxVC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxVSamples(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMinV(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsAddColorSpace(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsFormatOfPrefix(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsPrefixOfFormat(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsParseStringProc(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXConversionProc(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXConversionProc(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXcmsAddFunctionSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStatus(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetGCValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFlushGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestSize(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestTile(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestStipple(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLoadFont(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryFont(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLoadQueryFont(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetFontProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListFonts(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListFontsWithInfo(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWidth(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWidth16(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXInitImage(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetImage(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetSubImage(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListInstalledColormaps(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetFontPath(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXEventsQueued(PXNativDrawText PXREF pxNativDraw);

PXPublic PXResult PXAPI PXCheckWindowEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCheckMaskEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCheckTypedEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCheckTypedWindowEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSendEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetMotionEvents(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetErrorHandler(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGrabPointer(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGrabKeyboard(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetPointerMapping(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetPointerMapping(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetKeyboardMapping(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXNewModifiermap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXInsertModifiermapEntry(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDeleteModifiermapEntry(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetModifierMapping(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetModifierMapping(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSupportsLocale(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetLocaleModifiers(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXVaCreateNestedList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXOpenOM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCloseOM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetOMValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetOMValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayOfOM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLocaleOfOM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateOC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDestroyOC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXOMOfOC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetOCValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetOCValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateFontSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFontsOfFontSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXBaseFontNameListOfFontSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLocaleOfFontSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFreeFontSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDirectionalDependentDrawing(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXContextualDrawing(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXContextDependentDrawing(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXExtentsOfFontSet(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbTextEscapement(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcTextEscapement(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbTextExtents(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcTextExtents(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbTextPerCharExtents(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcTextPerCharExtents(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbDrawText(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcDrawText(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbDrawString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcDrawString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbDrawImageString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcDrawImageString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXOpenIM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCloseIM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetIMValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetIMValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDisplayOfIM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLocaleOfIM(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXRegisterIMInstantiateCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXIMInstantiateCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXUnregisterIMInstantiateCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDestroyCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateIC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDestroyIC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetICFocus(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXUnsetICFocus(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbResetIC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcResetIC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXIMOfIC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetICValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetICValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPreeditStateNotifyCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCallbackPrototype(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGeometryCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDestroyCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStringConversionCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPreeditStartCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPreeditDoneCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPreeditDrawCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPreeditCaretCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStatusStartCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStatusDoneCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStatusDrawCallback(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFilterEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbLookupString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcLookupString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXIconifyWindow(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWithdrawWindow(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXReconfigureWMWindow(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbTextListToTextProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcTextListToTextProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbTextPropertyToTextList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcTextPropertyToTextList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXwcFreeStringList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDefaultString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStringListToTextProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXTextPropertyToStringList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFreeStringList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetTextProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetTextProperty(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFetchName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMIconName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMIconName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetIconName(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocWMHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocSizeHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMNormalHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMNormalHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMSizeHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMSizeHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocClassHint(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetClassHint(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetTransientForHint(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMProtocols(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMProtocols(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMColormapWindows(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMColormapWindows(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocIconSize(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetIconSizes(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXmbSetWMProperties(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMProperties(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetCommand(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetWMClientMachine(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetWMClientMachine(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSetRGBColormaps(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetRGBColormaps(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmUniqueQuark(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmStringToQuark(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmQuarkToString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmStringToQuarkList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmInitialize(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmGetFileDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmPutFileDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXResourceManagerString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXScreenResourceString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmGetStringDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmLocaleOfDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmDestroyDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmSetDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmGetDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmCombineFileDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmCombineDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmMergeDatabases(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmGetResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmQGetResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmQGetSearchResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmQGetSearchResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmPutResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmQPutResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmPutStringResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmQPutStringResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmPutLineResource(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmEnumerateDatabase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXrmParseCommand(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLookupKeysym(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXKeycodeToKeysym(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXKeysymToKeycode(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXConvertCase(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXStringToKeysym(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXKeysymToString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLookupString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXpermalloc(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXParseGeometry(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWMGeometry(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateRegion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPolygonRegion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXEmptyRegion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXEqualRegion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXPointInRegion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXRectInRegion(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFetchBytes(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFetchBuffer(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXMatchVisualInfo(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateImage(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetPixel(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSubImage(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXReadBitmapFile(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXReadBitmapFileData(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXWriteBitmapFile(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreatePixmapFromBitmapData(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateBitmapFromData(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXSaveContext(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFindContext(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDeleteContext(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXQueryExtension(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXListExtensions(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXInitExtension(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAddExtension(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetCloseDisplay(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetCreateGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetCopyGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetFreeGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetCreateFont(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetFreeFont(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetWireToEvent(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetEventToWire(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetWireToError(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetError(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetErrorString(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetPrintErrorValues(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetFlushGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXESetCopyGC(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXEHeadOfExtensionList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFindOnExtensionList(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXReply(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocScratch(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXAllocTemp(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXFreeTemp(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetNormalHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetZoomHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetSizeHints(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetStandardColormap(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGeometry(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXGetDefault(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDraw(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXDrawFilled(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXCreateAssocTable(PXNativDrawText PXREF pxNativDraw);
PXPublic PXResult PXAPI PXLookUpAssoc(PXNativDrawText PXREF pxNativDraw);


*/

#endif

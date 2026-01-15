#pragma once

#ifndef PXWindowIncluded
#define PXWindowIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Font/PXFont.h>
#include <PX/Engine/ECS/Component/Mouse/PXMouse.h>
#include <PX/Engine/ECS/Component/Keyboard/PXKeyboard.h>
#include <PX/Engine/ECS/Component/Rectangle/PXRectangle.h>
#include <PX/Engine/ECS/Resource/Icon/PXIcon.h>
#include <PX/Engine/ECS/Resource/Cursor/PXCursor.h>
#include <PX/Engine/ECS/Resource/Brush/PXBrush.h>
#include <PX/Media/PXColor.h>




// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXWindow_ PXWindow;
typedef struct PXWindowEvent_ PXWindowEvent;


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


typedef void (PXAPI* PXWindowEventFunction)(void PXREF owner, PXWindowEvent PXREF pxWindowEvent);

typedef struct PXWindowEventList_
{
    PXECSInfo* CallBackOwner; // Object to reley back
    PXECSDrawFunction CallBackDraw; // Windows:WM_PAINT event
    PXWindowEventFunction CallBackEvent; // Different I/O Events
}
PXWindowEventList;


typedef struct PXWindowDrawInfo_
{
    PXRectangleXYWHI32 RectangleXYWH;

#if OSUnix
    int ScreenIDHandle;
    Display* DisplayHandle;
    Drawable WindowIDHandle;
    GC GraphicContntainerHandle;
#elif OSWindows
#endif

    //PXText Content;

    PXI32U Behaviour;

    PXBrush* Brush;
    PXBool Rounded;
}
PXWindowDrawInfo;





PXPublic const char* PXWindowDockSideToString(const PXWindowDockSide pxWindowDockSide);

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
    BOOL centerVertical;  // TRUE => center stacks vertically; FALSE => horizontally
} DockLayoutConfig;


RECT PXWindowRectOf(HWND h);
PXWindow* PaneFromHwnd(HWND h);


void SubSegment(RECT* m, RECT* r, PXWindowDockSide side);


// For allocation convenience
typedef struct Paneref { HWND h; PXWindow* p; } Paneref;



typedef struct DockOverlay {
    BOOL     active;
    PXWindowDockSide side;
    RECT     rect;        // rectangle of preview area
} DockOverlay;

static DockOverlay gOverlay = { 0 };


PXPublic PXWindow* PXAPI PXWindowRootGet(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsRoot(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowOpenGLEnable(PXWindow PXREF pxWindow);


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
// End of file

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

void PXAPI PXWindowPaintPane(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
void PXAPI PXWindowPaintPattern(HWND hwnd, HDC hdc, RECT* rect, PXColorRGBI8 PXREF color);
void PXAPI PXWindowPaintMain(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);




#if 1

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




typedef enum PXUIElementProperty_
{
    PXUIElementPropertyInvalid,
    PXUIElementPropertyTextContent,
    PXUIElementPropertyAllign,
    PXUIElementPropertyTextColor,
    PXUIElementPropertySize,
    PXUIElementPropertySizeParent,

    PXUIElementPropertySliderPercentage,

    PXUIElementPropertyProgressbarPercentage,
    PXUIElementPropertyProgressbarBarColor,

    PXUIElementPropertyBackGroundColor,

    PXUIElementPropertyVisibility,

    PXUIElementPropertyComboBoxAdd,


    PXUIElementPropertyItemAdd,
    PXUIElementPropertyItemDelete,
    PXUIElementPropertyItemUpdate,

    // Tree view

    PXUIElementPropertyTreeView_CreateDragImage,
    PXUIElementPropertyTreeView_DeleteAllItems,
    PXUIElementPropertyTreeView_DeleteItem,
    PXUIElementPropertyTreeView_EditLabel,
    PXUIElementPropertyTreeView_EndEditLabelNow,
    PXUIElementPropertyTreeView_EnsureVisible,
    PXUIElementPropertyTreeView_Expand,
    PXUIElementPropertyTreeView_GetBkColor,
    PXUIElementPropertyTreeView_GetCheckState,
    PXUIElementPropertyTreeView_GetChild,
    PXUIElementPropertyTreeView_GetCount,
    PXUIElementPropertyTreeView_GetDropHilight,
    PXUIElementPropertyTreeView_GetEditControl,
    PXUIElementPropertyTreeView_GetExtendedStyle,
    PXUIElementPropertyTreeView_GetFirstVisible,
    PXUIElementPropertyTreeView_GetImageList,
    PXUIElementPropertyTreeView_GetIndent,
    PXUIElementPropertyTreeView_GetInsertMarkColor,
    PXUIElementPropertyTreeView_GetISearchString,
    PXUIElementPropertyTreeView_GetItem,
    PXUIElementPropertyTreeView_GetItemHeight,
    PXUIElementPropertyTreeView_GetItemPartRect,
    PXUIElementPropertyTreeView_GetItemRect,
    PXUIElementPropertyTreeView_GetItemState,
    PXUIElementPropertyTreeView_GetLastVisible,
    PXUIElementPropertyTreeView_GetLineColor,
    PXUIElementPropertyTreeView_GetNextItem,
    PXUIElementPropertyTreeView_GetNextSelected,
    PXUIElementPropertyTreeView_GetNextSibling,
    PXUIElementPropertyTreeView_GetNextVisible,
    PXUIElementPropertyTreeView_GetParent,
    PXUIElementPropertyTreeView_GetPrevSibling,
    PXUIElementPropertyTreeView_GetPrevVisible,
    PXUIElementPropertyTreeView_GetRoot,
    PXUIElementPropertyTreeView_GetScrollTime,
    PXUIElementPropertyTreeView_GetSelectedCount,
    PXUIElementPropertyTreeView_GetSelection,
    PXUIElementPropertyTreeView_GetTextColor,
    PXUIElementPropertyTreeView_GetToolTips,
    PXUIElementPropertyTreeView_GetUnicodeFormat,
    PXUIElementPropertyTreeView_GetVisibleCount,
    PXUIElementPropertyTreeView_HitTest,
    PXUIElementPropertyTreeView_InsertItem,
    PXUIElementPropertyTreeView_MapAccIDToHTREEITEM,
    PXUIElementPropertyTreeView_MapHTREEITEMtoAccID,
    PXUIElementPropertyTreeView_Select,
    PXUIElementPropertyTreeView_SelectDropTarget,
    PXUIElementPropertyTreeView_SelectItem,
    PXUIElementPropertyTreeView_SelectSetFirstVisible,
    PXUIElementPropertyTreeView_SetAutoScrollInfo,
    PXUIElementPropertyTreeView_SetBkColor,
    PXUIElementPropertyTreeView_SetBorder,
    PXUIElementPropertyTreeView_SetCheckState,
    PXUIElementPropertyTreeView_SetExtendedStyle,
    PXUIElementPropertyTreeView_SetHot,
    PXUIElementPropertyTreeView_SetImageList,
    PXUIElementPropertyTreeView_SetIndent,
    PXUIElementPropertyTreeView_SetInsertMark,
    PXUIElementPropertyTreeView_SetInsertMarkColor,
    PXUIElementPropertyTreeView_SetItem,
    PXUIElementPropertyTreeView_SetItemHeight,
    PXUIElementPropertyTreeView_SetItemState,
    PXUIElementPropertyTreeView_SetLineColor,
    PXUIElementPropertyTreeView_SetScrollTime,
    PXUIElementPropertyTreeView_SetTextColor,
    PXUIElementPropertyTreeView_SetToolTips,
    PXUIElementPropertyTreeView_SetUnicodeFormat,
    PXUIElementPropertyTreeView_ShowInfoTip,
    PXUIElementPropertyTreeView_SortChildren,
    PXUIElementPropertyTreeView_SortChildrenCB,

}
PXUIElementProperty;

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





typedef struct PXWindowEventClose_
{
    PXBool CommitToClose;
}
PXWindowEventClose;

typedef struct PXWindowEventResize_
{
    PXI16S Width;
    PXI16S Height;
    PXWindowResizeCause Cause;
}
PXWindowEventResize;

typedef struct PXWindowEventInputMouseButton_
{
    PXKeyPressState PressState;
    PXMouseButton Button;
}
PXWindowEventInputMouseButton;

typedef struct PXWindowEventSelect_
{
    PXWindow* UIElementSelected;
}
PXWindowEventSelect;

typedef struct PXWindowEventInputMouseMove_
{
    PXVector2F32 Axis;
    PXVector2I32S Delta;
    PXVector2I32S Position;
}
PXWindowEventInputMouseMove;

typedef struct PXWindowEventInputKeyboard_
{
    PXKeyPressState PressState;
    PXVirtualKey VirtualKey;

    union MyUnion
    {
        char CharacterA;
        wchar_t CharacterW;
    };  

    PXI16U CharacterID;
}
PXWindowEventInputKeyboard;




typedef struct PXWindowEvent_
{
    union
    {
        PXWindowEventClose Close;
        PXWindowEventResize Resize;
        PXWindowEventInputMouseButton InputMouseButton;
        PXWindowEventInputMouseMove InputMouseMove;
        PXWindowEventInputKeyboard InputKeyboard;
        PXWindowEventSelect Select;
    };

    //-----------------------------
    // Original data
    //-----------------------------
#if OSUnix
    XEvent* EventData;
#elif OSWindows
    HWND WindowHandle;
    UINT EventID;
    WPARAM ParamW;
    LPARAM ParamL;
#endif

    //-----------------------------
    // Translated Data
    //-----------------------------
    PXWindowEventType Type;

    //PXWindow* UIElementReference;
    PXWindow* WindowSender;
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












typedef struct PXWindowCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindow* WindowParent;
    PXDisplay* DisplayCurrent;

    // Positions
    PXRectangleXYWHI32 Size;

    // Style
    PXI8U BorderWidth;
    PXI8U Border;

    PXColorRGBI8 BackGroundColor;

    PXWindowDockSide  dockSide;
   // BOOL isChild;
    PXBool      floating;   // if TRUE, excluded from docking layout


    // Setings
    PXBool Simple;
    PXBool AvoidCreation;
    PXBool Invisible; // Hide the window, we still want to create it.
    PXBool IsVirtual; // Avoid window from beeing created, we use this to make framebuffer windows?
    PXBool CreateMessageThread; // Run events in another thread
    PXBool MaximizeOnStart;


    PXWindow* UIElementReference;

 



    PXWindowEventList EventList;

   

    //PXI32U FlagList;

    PXUIElementType Type;
    PXI32U Behaviour;
    PXI32U Setting;
    PXColorRGBAF* ColorTintReference;




    PXUIElementPosition Position;

#if OSUnix
#elif OSWindows
    HDC DeviceContextHandle;
    HINSTANCE InstanceHandle;

    DWORD StyleFlagsExtended;
    DWORD StyleFlags;

    PXText WindowText;
    PXText WindowClassName;
#endif

    PXWindowCreateFunction CreatePre; // Function is called before the physical creation
    PXWindowCreateFunction CreatePost; // Function is called after physical creation

#define PXWindowCreateVirtual   1
#define PXWindowCreatePhysical  2

    PXI8U FLags;

    // PXWindowMenuItemList MenuItem;
     //PXUIElementSceneRenderInfo SceneRender;
}
PXWindowCreateInfo;





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


PXPublic PXResult PXAPI PXWindowPositionCalculcate(PXWindow PXREF pxWindow, PXWindowPositionCalulcateInfo PXREF pxUIElementPositionCalulcateInfo);


PXPublic const char* PXAPI PXWindowTypeToString(const PXUIElementType pxUIElementType);


PXPublic PXResult PXAPI PXWindowRegisterToECS();

PXPublic HDC PXAPI PXWindowDCGet(PXWindow PXREF pxWindow);
PXPublic HWND PXAPI PXWindowHandleGet(PXWindow PXREF pxWindow);


// This function consumes events first before any other 
// listener revices this. Because of this, this event handler 
// shall only handle low-level events and not be too much in the way
PXPublic PXResult PXAPI PXWindowEventConsumer(PXWindowEvent PXREF pxWindowEvent);

PXPublic PXResult PXAPI PXWindowEventPendingAmount(PXSize PXREF amount);
PXPublic PXResult PXAPI PXWindowEventPoll(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowBufferSwap(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowCreate(PXWindow** pxWindowREF, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPublic PXResult PXAPI PXWindowDestroy(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowFindViaTitle(PXWindow PXREF pxWindow, const PXText PXREF pxText);

PXPublic PXResult PXAPI PXWindowDragStart(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowPixelSystemSet(PXWindow PXREF pxWindow, PXWindowPixelSystemInfo PXREF pxWindowPixelSystemInfo);

// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowShow(const PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowHide(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowRedraw(const PXWindow PXREF pxWindow);

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

PXPublic PXResult PXAPI PXWindowRectangleXYWH(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32);

PXPublic PXResult PXAPI PXWindowRectangleLTRB(PXWindow PXREF pxWindow, PXRectangleLTRBI32 PXREF pxRectangleLTRBI32);

PXPublic PXBool PXAPI PXWindowIsValid(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsEnabled(const PXWindow PXREF pxWindow);


// DRAW
PXPublic PXResult PXAPI PXWindowDrawRectangle2D(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXWindowDrawRectangle3D(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXWindowDrawClear(PXWindow PXREF pxWindow);


typedef struct PXTextDrawInfo_
{
    PXWindowDrawInfo* WindowDrawInfo;
    PXText* Text;

    float X;
    float Y;

    PXI32U Behaviour;

    float Size;
    float OffsetX;
}
PXTextDrawInfo;

PXPublic PXResult PXAPI PXWindowDrawText
(
    PXWindow PXREF pxWindow,
    PXTextDrawInfo PXREF pxTextDrawInfo
);
PXPublic PXResult PXAPI PXWindowDrawPoint(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y);
PXPublic PXResult PXAPI PXWindowDrawPoints(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);
PXPublic PXResult PXAPI PXWindowDrawLine(PXWindow PXREF pxWindow, const PXI32S x1, const PXI32S y1, const PXI32S x2, const PXI32S y2);
PXPublic PXResult PXAPI PXWindowDrawLines(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height);


PXPublic PXResult PXAPI PXWindowDrawRectangle(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawRectangleInfo);
PXPublic PXResult PXAPI PXWindowDrawIcon(PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, const int x, const int y, const int width, const int height);


PXPublic PXResult PXAPI PXWindowMouseTrack(PXWindow PXREF window);
PXPublic PXResult PXAPI PXWindowMouseMovementEnable(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowCursorCaptureMode(PXWindow PXREF pxWindow, const PXWindowCursorMode cursorMode);
PXPublic PXResult PXAPI PXWindowCursorPositionGet(PXWindow PXREF pxWindow, PXVector2I32S PXREF position);

#endif
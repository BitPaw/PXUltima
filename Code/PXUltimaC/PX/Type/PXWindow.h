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
#include <PX/Type/PXColor.h>
#include <PX/Engine/PXEngineUI.h>

#if OSUnix
#include <X11/Xlib.h> // Display*
#endif

// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXWindow_ PXWindow;
typedef struct PXOpenGLContext_ PXOpenGLContext;


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

typedef enum PXEngineUIElementTreeViewItemInsertMode_
{
    PXEngineUIElementTreeViewItemInsertModeROOT,
    PXEngineUIElementTreeViewItemInsertModeFIRST,
    PXEngineUIElementTreeViewItemInsertModeLAST,
    PXEngineUIElementTreeViewItemInsertModeSORT
}
PXEngineUIElementTreeViewItemInsertMode;

typedef enum PXEngineUIElementType_
{
    PXEngineUIElementTypeInvalid,
    PXEngineUIElementTypeCustom,
    PXEngineUIElementTypeWindow, // General window to draw into
    PXEngineUIElementTypePanel, // static generic element
    PXEngineUIElementTypeText, // text, letters
    PXEngineUIElementTypeButton, // Button to click
    PXEngineUIElementTypeImage,
    PXEngineUIElementTypeDropDown,
    PXEngineUIElementTypeListBox,
    PXEngineUIElementTypeTextEdit,
    PXEngineUIElementTypeRichEdit,
    PXEngineUIElementTypeScrollBar,
    PXEngineUIElementTypeTrackBar,
    PXEngineUIElementTypeStatusBar,
    PXEngineUIElementTypeUpDown,
    PXEngineUIElementTypeProgressBar,
    PXEngineUIElementTypeHotKey,
    PXEngineUIElementTypeCalender,
    PXEngineUIElementTypeToolTip,
    PXEngineUIElementTypeAnimate,
    PXEngineUIElementTypeDatePicker,
    PXEngineUIElementTypeGroupBox,
    PXEngineUIElementTypeRadioButton,
    PXEngineUIElementTypeGroupRadioButton,
    PXEngineUIElementTypeTreeView,
    PXEngineUIElementTypeTreeViewItem,
    PXEngineUIElementTypeFileDirectyView,
    PXEngineUIElementTypeFileDirectyViewEntry,
    PXEngineUIElementTypeIPInput,
    PXEngineUIElementTypeLink,
    PXEngineUIElementTypeHeader, // Object insode another for header info
    PXEngineUIElementTypeFontSelector,
    PXEngineUIElementTypePageScroll,
    PXEngineUIElementTypeTabControll,
    PXEngineUIElementTypeTabPage,
    PXEngineUIElementTypeToggle,
    PXEngineUIElementTypeCheckBox,
    PXEngineUIElementTypeComboBox,
    PXEngineUIElementTypeColorPicker,
    PXEngineUIElementTypeSlider,
    PXEngineUIElementTypeImageList,
    PXEngineUIElementTypeRenderFrame,

    PXEngineUIElementTypeMenuStrip, // Topline of a window that contains a selectable list of things

    PXEngineUIElementTypeScene,
    PXEngineUIElementTypeCode,
    PXEngineUIElementTypeHexEditor,
    PXEngineUIElementTypeGraphBehaviour,
    PXEngineUIElementTypeGraphTime,
    PXEngineUIElementTypeSoundPlayerMixer,
    PXEngineUIElementTypeVideoCutter,
    PXEngineUIElementTypeDataBaseManager,
    PXEngineUIElementTypeNetworkTester,
    PXEngineUIElementTypeInputView,
    PXEngineUIElementTypeHardwareInfo,

    PXEngineUIElementTypeFileManager,    // TreeView of filesystem with utility
    PXEngineUIElementTypeResourceManger, // TreeView of a list of resource entrys
    PXEngineUIElementTypeResourceEntry,  // Panel with name and icons of type and propertys
    PXEngineUIElementTypeResourceInfo    // Resource info
}
PXEngineUIElementType;


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

    PXEngineUILayoutMode LayoutMode;
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

    PXEngineUIDockSide  DockSide;     // Docking behaviour
    PXBool Floating;   // if TRUE, excluded from docking layout
}
PXWindowState;




typedef struct PXWindowEventList_
{
    PXECSInfo* CallBackOwner; // Object to reley back
    PXECSDrawFunction CallBackDraw; // Windows:WM_PAINT event
    PXWindowEventFunction CallBackEvent; // Different I/O Events
}
PXWindowEventList;



//---------------------------------------------------------
// Window stuff
//---------------------------------------------------------
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
//---------------------------------------------------------



//---------------------------------------------------------
// Window create
//---------------------------------------------------------
typedef struct PXWindowCreateInfo_ PXWindowCreateInfo;

typedef PXResult(PXAPI* PXWindowCreateHelper)(PXWindowCreateInfo PXREF pxWindowCreateInfo);
typedef PXResult(PXAPI* PXWindowCreateFunction)(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

typedef struct PXWindowCreateInfo_
{
    PXECSCreateInfo Info;
     
    PXText WindowParentName; // Set by config, needs to be translated into actual handle!

    PXDisplay* DisplayCurrent;
    PXWindow* WindowParent;

    //_____________________________________________________
    // Positions
    PXWindowLayout Layout;
    PXWindowState State;
    //_____________________________________________________

    // Style


    PXColorRGB_U8 BackGroundColor;


    // BOOL isChild;


    PXBool DoOpenGL;




    // Setings

    // Don't create a HANDLE for the window. 
    // Nested rendering without OS overhead.
    PXBool SkipHandleCreation; 

    // Windows: Will attach the WM_CHILD flag.
    PXBool DoSpawnInParent;


    PXBool UseFrameBuffer; // Dont render into a frame but into a framebuffer
    PXBool Invisible; // Hide the window, we still want to create it.


    PXBool Simple;
    PXBool MaximizeOnStart;


    PXWindow* UIElementReference;

    PXWindowEventList EventList;

    //PXI32U FlagList;

    PXEngineUIElementType Type;
    PXI32U Behaviour;
    PXI32U Setting;
    PXColorRGBA_F32* ColorTintReference;


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
     //PXEngineUIElementSceneRenderInfo SceneRender;

    // Reference to hold so OS level events can get the 
    // required handle for the UI system.
    PXEngineUI* EngineUI;
}
PXWindowCreateInfo;
//---------------------------------------------------------




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


typedef enum PXEngineUIElementInteractType_
{
    PXEngineUIElementInteractTypeInvalid,
    PXEngineUIElementInteractTypeSelect,
    PXEngineUIElementInteractTypeClick,
    PXEngineUIElementInteractTypeMouseEnter,
    PXEngineUIElementInteractTypeMouseLeave
}
PXEngineUIElementInteractType;

typedef struct PXEngineUIElementFrameBufferInfo_
{
    struct PXTexture_* TextureReference;

    PXI32U Width;
    PXI32U Height;
    PXI32U BufferID;
    PXI32U RenderID;
}
PXEngineUIElementFrameBufferInfo;

typedef struct PXEngineUIElementImageInfo_
{
    struct PXTexture_* TextureReference;
}
PXEngineUIElementImageInfo;

typedef struct PXEngineUIElementItemInfo_
{
    char* TextData;
    PXSize TextSize;
}
PXEngineUIElementItemInfo;

typedef struct PXEngineUIElementSceneRenderInfo_
{
    struct PXEngine_* Engine;
    struct PXEngineStartInfo_* StartInfo;
}
PXEngineUIElementSceneRenderInfo;

typedef struct PXEngineUIElementProgressBarInfo_
{
    PXF32 Percentage;
    PXColorRGB_U8 BarColor;
}
PXEngineUIElementProgressBarInfo;


#if 0
typedef struct PXEngineUIElementPosition_
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
PXEngineUIElementPosition;
#endif

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


PXPublic const char* PXAPI PXWindowTypeToString(const PXEngineUIElementType pxUIElementType);

PXPublic void PXAPI PXWindowRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);


//---------------------------------------------------------
#if OSUnix

#elif OSWindows
PXPublic HDC PXAPI PXWindowDCGet(PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowFromHandle(const HWND windowHandle);
PXPrivate void PXAPI PXWindowStoreSelf(PXWindow PXREF pxWindow, const HWND windowHandle);
#endif
//---------------------------------------------------------


//---------------------------------------------------------
// OpenGL
PXPublic PXOpenGLContext PXAPI PXWindowGLContextGet(const PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowGLContextSet(PXWindow PXREF pxWindow, const PXOpenGLContext pxOpenGLContext);
PXPublic PXResult PXAPI PXWindowOpenGLEnable(PXWindow PXREF pxWindow);
//---------------------------------------------------------



PXPublic PXWindowHandle PXAPI PXWindowHandleGet(const PXWindow PXREF pxWindow);

// [WARNING] This function can return a window handle that is 
// owned by another thread or even process. 
PXPublic PXWindowHandle PXAPI PXWindowHandleGetAtCursorPosition();
PXPublic PXWindowHandle PXAPI PXWindowHandleGetAtPosition(const int x, const int y);

PXPublic PXBool PXAPI PXWindowOwnedByThisProcess(PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowHandleOwnedByThisProcess(const PXWindowHandle pxWindowHandle);
PXPublic PXF32 PXAPI PXWindowDPIScaleFactorGet(const PXWindowHandle pxWindowHandle);

// Use for a seperate window that needs to be merged into a main one.
// Given a spesific window we can try to absorb the contens and underlieing elemetns and move them into your own space.
// Objects shall not be created or destroyed, simply the ownership of those objects should be transphered. (can we do that?)
//PXPublic PXResult PXAPI PXWindowAbsorb(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);

// Use for draging a window outside it own borders to spawn a new one.
//PXPublic PXResult PXAPI PXWindowEmit(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);




//---------------------------------------------------------
// Hirachy
PXPublic PXWindow* PXAPI PXWindowRootGet(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsRoot(const PXWindow PXREF pxWindow);

PXPublic PXSize PXAPI PXWindowSiblingAmount(const PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowSiblingGet(PXWindow PXREF pxWindow, const PXSize index);

PXPublic PXSize PXAPI PXWindowChildAmount(const PXWindow PXREF pxWindow);
PXPublic PXWindow* PXAPI PXWindowChildGet(PXWindow PXREF pxWindow, const PXSize index);

PXPublic PXResult PXAPI PXWindowChildAdd(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild);
PXPublic PXResult PXAPI PXWindowChildRemove(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild);

PXPublic PXResult PXAPI PXWindowParentSet(PXWindow PXREF pxWindow, const PXWindow PXREF pxWindowParent);
PXPublic PXWindow* PXAPI PXWindowParentGet(const PXWindow PXREF pxWindow);
//---------------------------------------------------------

//---------------------------------------------------------
// Docking
PXPublic const char* PXWindowDockSideToString(const PXEngineUIDockSide pxWindowDockSide);
//---------------------------------------------------------

//---------------------------------------------------------
// Drag'n'Drop

//---------------------------------------------------------



//---------------------------------------------------------
PXPublic PXResult PXAPI PXWindowBufferSwap(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowCreate(PXWindow** pxWindowREF, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPublic PXResult PXAPI PXWindowCreatePhysical(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPrivate PXResult PXAPI PXWindowCreatePhysicalEventListenerAdd(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPrivate PXResult PXAPI PXWindowCreatePhysicalEventDragAndDropAdd(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPrivate PXResult PXAPI PXWindowCreatePhysicalSpawn(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPublic PXResult PXAPI PXWindowCreateFrameBuffer(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);

PXPublic PXResult PXAPI PXWindowDestroy(PXWindow PXREF pxWindow);
//---------------------------------------------------------



//---------------------------------------------------------
// Hirachy
//---------------------------------------------------------

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

PXPublic PXBool PXAPI PXWindowIsMinimized(const PXWindow PXREF pxWindow);

PXPublic PXBool PXAPI PXWindowIsSurfaceValid(const PXWindow PXREF pxWindow);



PXPublic PXBool PXAPI PXWindowClientRectangleGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);

PXPublic PXResult PXAPI PXWindowVisibilitySet
(
    PXWindow PXREF pxWindow,
    const PXBool doRenderingSelf,
    const PXBool doRenderingChildren
);

PXPublic PXResult PXAPI PXWindowShow(const PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowHide(const PXWindow PXREF pxWindow);

// aka mark as dirty
#if 0
PXPublic PXResult PXAPI PXWindowRedraw(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowRedrawEnable(PXWindow PXREF pxWindow, const PXBool enable);
#endif

PXPublic PXResult PXAPI PXWindowDragAndDropBegin(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowDragAndDropEnd(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowDrawBegin(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowDrawEnd(PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowScrollbarUpdate(PXWindow PXREF pxWindow, PXScollbar PXREF pxScollbar);

PXPublic PXResult PXAPI PXWindowTitleBarColor(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowForegroundSet(PXWindow PXREF pxWindow, PXColorRGB_U8 PXREF PXColorRGB_U8);
PXPublic PXResult PXAPI PXWindowBackgroundSet(PXWindow PXREF pxWindow, PXColorRGB_U8 PXREF PXColorRGB_U8);
PXPublic PXResult PXAPI PXWindowFontSet(PXWindow PXREF pxWindow, PXFont PXREF pxFont);
PXPublic PXResult PXAPI PXWindowBrushSet(PXWindow PXREF pxWindow, PXBrush PXREF pxBrush, const PXI8U mode);
PXPublic PXResult PXAPI PXWindowTextGet(PXWindow PXREF pxWindow, PXText PXREF pxText);
PXPublic PXResult PXAPI PXWindowTextSet(PXWindow PXREF pxWindow, PXText PXREF pxText);
PXPublic PXResult PXAPI PXWindowStyleUpdate(PXWindow PXREF pxWindow);
//---------------------------------------------------------


//---------------------------------------------------------
// Properties
//---------------------------------------------------------
PXPublic UIWindow* PXAPI PXWindowUIDataGet(const PXWindow PXREF pxWindow);
PXPublic UIState* PXAPI PXWindowUIStateGet(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowClientArea(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);
PXPublic PXResult PXAPI PXWindowCursorPosition(const PXWindow PXREF pxWindow, PXVector2I32S PXREF positionGlobal, PXVector2I32S PXREF positionLocal);

PXPublic PXResult PXAPI PXWindowSpaceDrawGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);
PXPublic PXResult PXAPI PXWindowSpaceBoxGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);
PXPublic PXResult PXAPI PXWindowSpaceContentGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle);

PXPublic PXResult PXAPI PXWindowDrawFunctionSet(PXWindow PXREF pxWindow, const PXECSDrawFunction pxECSDrawFunction, void* owner);


PXPublic PXF32 PXAPI PXWindowAspectRatio(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsValid(const PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowIsEnabled(const PXWindow PXREF pxWindow);

PXPublic PXResult PXAPI PXWindowResize(PXWindow PXREF pxWindow, const int width, const int height);
PXPublic PXResult PXAPI PXWindowMoveAndResize(PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
//---------------------------------------------------------


//---------------------------------------------------------
// DRAW
//---------------------------------------------------------
// glViewPort
PXPublic PXResult PXAPI PXWindowViewPortSet(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, const PXBool normalized);


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

typedef LRESULT(PXAPI* PXWindowEventHandlerFunction)(PXEngineUIEvent PXREF pxWindowEvent);

PXPrivate LRESULT PXAPI PXWindowEvent_WM_EMPTY(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCCREATE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DESTROY(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SIZE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CLOSE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SETCURSOR(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NOTIFY(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ERASEBKGND(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_PAINT(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_GETMINMAXINFO(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DEVMODECHANGE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT_DEVICE_CHANGE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEMOVE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSELEAVE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCHITTEST(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DISPLAYCHANGE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CTLCOLOR(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVING(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ENTERSIZEMOVE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_EXITSIZEMOVE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_POWERBROADCAST(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DEVICECHANGE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUTLANGCHANGE(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_COMMAND(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_GETICON(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DPICHANGED(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_KEY(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDOWN(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONUP(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDBLCLK(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDOWN(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONUP(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDBLCLK(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDOWN(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONUP(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDBLCLK(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEWHEEL(PXEngineUIEvent PXREF pxWindowEvent);
PXPrivate LRESULT PXAPI PXWindowEvent_WM_XBUTTON(PXEngineUIEvent PXREF pxWindowEvent);

#endif


PXPublic PXResult PXAPI PXWindowDisplayListFetch();


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
































#if 0


//---------------------------------------------------------
// DO BE REMOVED!!
//---------------------------------------------------------
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

void SubSegment(RECT* m, RECT* r, PXWindowDockSide side);
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

void PXAPI PXWindowPaintPattern(HWND hwnd, HDC hdc, RECT* rect, PXColorRGB_U8 PXREF color);
void PXAPI PXWindowPaintMain(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif











#endif
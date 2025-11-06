#pragma once

#ifndef PXGUIIncluded
#define PXGUIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXGUITheme_
{
    PXWindowBrush* MainPrimary;
    PXWindowBrush* MainSecoundary;

    PXWindowBrush* TextWhite;

    PXFont* FontTitle;
    PXFont* FontContent;
}
PXGUITheme;


// Manager to spawn and handle all physical and virtual windows.
// Buttons, Labels and all.
typedef struct PXGUIManager_
{
    PXGUITheme ThemeDefault;
    PXGUITheme* ThemeSelected;

    PXNativDraw NativDraw;
}
PXGUIManager;


// Info about a window property like position or text content
typedef struct PXGUIProperty_
{
    // List of all current windows overall, in this system
    // List of all current windows locally, in this program.

    //PXWindow* WindowCurrent;
    //PXWindow* WindowParent;
    PXUIElementProperty Property;
    //PXWindowCreateInfoData Data;

    union
    {
        PXRectangleXYWHI32 Size;
    };

    PXBool Show;
    PXWindowPropertyUpdateType UpdateType;
}
PXGUIProperty;


// UI Element, button, text and everything else
typedef struct PXGUIElement_
{
    int x;
}
PXGUIElement;


typedef struct PXColorCircleVertex_
{
    PXF32 X;
    PXF32 Y;
    PXI8U Red;
    PXI8U Green;
    PXI8U Blue;
}
PXColorCircleVertex;

typedef struct PXColorCircle_
{
    int StartX;
    int StartY;
    int Size;
    int Precision;
    PXColorCircleVertex* VertexList;

    // Triangle
    PXColorCircleVertex VertexListTriangle[3];

    PXColorHSV ColorSelected;
}
PXColorCircle;


PXPublic PXGUIManager* PXAPI PXGUIInitialize(void);
PXPublic PXResult PXAPI PXGUIShutdown(void);

PXPublic PXGUITheme* PXAPI PXGUIThemeGet(void);


// Create a window
PXPrivate PXResult PXAPI PXGUICreatePhysical(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPrivate PXResult PXAPI PXGUICreateVirtual(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPublic PXResult PXAPI PXGUIWindowPixelSystemSet(PXWindowPixelSystemInfo PXREF pxWindowPixelSystemInfo);

// Create any GUI element
PXPublic PXResult PXAPI PXGUICreate(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowParent, PXWindowCreateInfo PXREF pxWindowCreateInfo);
PXPublic PXResult PXAPI PXGUIDestroy(PXWindow PXREF pxWindow);


// Change any property of a window. This has to be used for all changes!
PXPublic PXResult PXAPI PXGUIPropertyFetch(PXWindow PXREF pxWindow, PXGUIProperty PXREF pxGUIPropertyList, const PXSize amount, const PXBool doWrite);
PXPublic PXResult PXAPI PXGUIPropertyTitleBarColor(PXWindow PXREF pxWindow);


// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXGUIWindowIsInFocus(const PXWindowHandle pxWindowID);


//---------------------------------------------------------
// Find
PXPublic PXResult PXAPI PXGUIFindViaTitleA(PXWindow PXREF pxWindow, const char* text);
PXPublic PXResult PXAPI PXGUIFindViaTitleW(PXWindow PXREF pxWindow, const wchar_t* text);
//---------------------------------------------------------


//---------------------------------------------------------
// Display/Monitors
//---------------------------------------------------------
#if OSUnix
PXPrivate PXResult PXAPI PXWindowErrorFromXSystem(const int xSysstemErrorID);
#elif PXOSWindowsDestop
PXPrivate void PXAPI PXGUIChildEnumerate(PXGUIManager PXREF pxGUIManager, PXWindow PXREF parent, PXBool visible);
PXPrivate BOOL CALLBACK PXGUIChildEnum(const HWND hwnd, const LPARAM lParam);
#endif

PXPublic PXResult PXAPI PXGUIMonitorListRefresh();
PXPublic PXResult PXAPI PXGUIWindowListRefresh();
//---------------------------------------------------------



//---------------------------------------------------------
// Icons
PXPublic PXResult PXAPI PXGUIIconGetSystem(PXIcon PXREF pxIcon, const int iconID);
PXPublic PXResult PXAPI PXGUIIconGetViaFilePath(PXIcon PXREF pxIcon, const char* fileExtension);
//---------------------------------------------------------

//---------------------------------------------------------
// Cursor
PXPublic PXResult PXAPI PXGUIMouseMovementEnable(const PXWindowHandle pxWindow);
PXPrivate PXI32U PXAPI PXGUICursorIconToID(const PXCursorIcon cursorIcon);
PXPublic void PXAPI PXGUICursorCaptureMode(const PXWindowHandle pxWindowID, const PXWindowCursorMode cursorMode);
PXPublic PXResult PXAPI PXCursorPositionGet(const PXWindowHandle pxWindowID, PXVector2I32S PXREF position);
//---------------------------------------------------------

//---------------------------------------------------------
// Drag
PXPublic PXResult PXAPI PXGUIDragStart(PXWindow PXREF pxWindow);
//---------------------------------------------------------



PXPrivate void PXMathCircle(PXColorCircle PXREF pxColorCircle);

// UNSORTERD
PXPublic PXResult PXAPI PXWindowTabListSwapPage(PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowStyleUpdate(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXWindowReDrawEnable(const PXWindowHandle pxWindowID, const PXBool enable);
PXPublic PXBool PXAPI PXWindowValueFetch
(
    PXWindow PXREF pxUIElementList, //
    const PXSize dataListAmount,
    const PXUIElementProperty pxUIElementProperty,
    void PXREF dataList // The given data
);







//---------------------------------------------------------
// Default rendering functions
//---------------------------------------------------------
PXPrivate PXResult PXAPI PXGUIDrawRectangle2D(PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
PXPrivate PXResult PXAPI PXGUIDrawRectangle3D(PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
PXPrivate PXResult PXAPI PXGUIDrawHeader(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawFooter(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawView(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawViewTitle(PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const char PXREF title, const PXSize titleSize);
PXPrivate PXResult PXAPI PXGUIDrawViewEntry(PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const char PXREF title, const PXSize titleSize);
PXPrivate PXResult PXAPI PXGUIDrawResourceInfo(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawTabList(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawFailback(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawText(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawButton(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawComboBox(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawColorPicker(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawHexView(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawFileDirectoryView(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawCode(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawHexEditor(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawGraphBehaviour(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawGraphTime(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawSoundPlayerMixer(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawVideoCutter(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawDataBase(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawNetwork(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawInputView(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawHardwareInfo(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);
//---------------------------------------------------------

#endif


/*
// Acts as as a mediator and container.
// Uses differtent
typedef struct PXGUIManager_
{
    PXLibrary LibraryWindowsUser32DLL;

    PXBool AreOSUIElementsDefined;

    PXResourceManager* ResourceManager;

    PXDisplay DisplayCurrent;

    // private 

    PXNativDraw NativDraw;
}
PXGUIManager;*/






// TODO: Global Variable, bad but needed for SYNC with OS. Stupid design
//PXPrivate PXGUIManager* PXGUIManagerGlobalReference = PXNull;










#ifndef PXGUIIncludedd
#define PXGUIIncludedd

#include <PX/Engine/PXResource.h>

typedef struct PXGUITheme_
{
    PXWindowBrush* BrushBackgroundDark;
    PXWindowBrush* BrushWhite;

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


typedef struct PXGUIProperty_
{
    int x;

    // List of all current windows overall, in this system

    // List of all current windows locally, in this program.

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


PXPublic PXGUISystem* PXAPI PXGUIInitialize(void);
PXPublic PXResult PXAPI PXGUIShutdown(void);


// Create a window
PXPrivate PXResult PXAPI  PXGUICreatePhysical(PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo);
PXPrivate PXResult PXAPI  PXGUICreateVirtual(PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo);

PXPublic PXResult PXAPI PXGUICreate(PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo);
PXPublic PXResult PXAPI PXGUIDestroy(PXWindow* const pxWindow);


// Change the property of a window
PXPublic PXResult PXAPI PXGUIPropertyChange(PXWindow* const pxWindow, PXGUIProperty* const pxGUIProperty);
PXPublic PXResult PXAPI PXGUIPropertyTitleBarColor(PXWindow* const pxWindow);

#if OSUnix
PXPrivate PXResult PXAPI  PXWindowErrorFromXSystem(const int xSysstemErrorID);
#elif PXOSWindowsDestop
PXPrivate void PXAPI PXGUIChildEnumerate(PXGUISystem* const pxGUISystem, PXWindow* const parent, PXBool visible);
PXPrivate BOOL CALLBACK PXGUIChildEnum(const HWND hwnd, const LPARAM lParam);
#endif


// Direct I/O functions
PXPublic PXResult PXAPI PXGUIFindViaTitleA(PXWindow* const pxWindow, const char* text);
PXPublic PXResult PXAPI PXGUIFindViaTitleW(PXWindow* const pxWindow, const wchar_t* text);

PXPublic PXResult PXAPI PXGUIIconGetSystem(PXIcon* const pxIcon, const int iconID);
PXPublic PXResult PXAPI PXGUIIconGetViaFilePath(PXIcon* const pxIcon, const char* fileExtension);

PXPublic PXResult PXAPI PXGUIDragStart(PXWindow* const pxWindow);

PXPrivate void PXMathCircle(PXColorCircle* const pxColorCircle);


//---------------------------------------------------------
// Default rendering functions
//---------------------------------------------------------
PXPrivate PXResult PXAPI PXGUIDrawRectangle2D(PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
PXPrivate PXResult PXAPI PXGUIDrawRectangle3D(PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
PXPrivate PXResult PXAPI PXGUIDrawHeader(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawFooter(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawViewTitle(PXWindow* const pxWindow, PXIcon* const pxIcon, PXRectangleXYWHI32* const pxRectangleXYWHI32, const char* const title, const PXSize titleSize);
PXPrivate PXResult PXAPI PXGUIDrawViewEntry(PXWindow* const pxWindow, PXIcon* const pxIcon, PXRectangleXYWHI32* const pxRectangleXYWHI32, const char* const title, const PXSize titleSize);
PXPrivate PXResult PXAPI PXGUIDrawResourceInfo(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawTabList(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawFailback(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawText(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawButton(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawComboBox(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawColorPicker(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawHexView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawFileDirectoryView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawCode(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawHexEditor(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawGraphBehaviour(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawGraphTime(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawSoundPlayerMixer(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawVideoCutter(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawDataBase(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawNetwork(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawInputView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
PXPrivate PXResult PXAPI PXGUIDrawHardwareInfo(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo);
//---------------------------------------------------------

#endif














#ifndef PXGUIIncluded
#define PXGUIIncluded

#include <PX/Media/PXType.h>
#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/OS/Hardware/PXMouse.h>
#include <PX/OS/Hardware/PXKeyBoard.h>
#include <PX/OS/Async/PXProcess.h>
#include <PX/OS/Async/PXThread.h>
#include <PX/OS/File/PXFile.h>


// Acts as as a mediator and container.
// Uses differtent
typedef struct PXGUISystem_
{
    PXLibrary LibraryWindowsUser32DLL;

    PXBool AreOSUIElementsDefined;

    PXResourceManager* ResourceManager;

    PXDisplay DisplayCurrent;

    // private 

    PXNativDraw NativDraw;
}
PXGUISystem;






// TODO: Global Variable, bad but needed for SYNC with OS. Stupid design
PXPrivate PXGUISystem* PXGUISystemGlobalReference = PXNull;

//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------

//---------------------------------------------------------



//---------------------------------------------------------
// Window utility functions
//---------------------------------------------------------
PXPublic PXResult PXAPI PXWindowCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount);
PXPublic PXResult PXAPI PXWindowFetch(PXGUISystem* const pxGUISystem, PXWindowPropertyInfo* const pxWindowUpdateInfoList, const PXSize amount);
//---------------------------------------------------------



//---------------------------------------------------------
// Window propertys - Getter / Setter
//---------------------------------------------------------
PXPublic PXResult PXAPI PXWindowStyleUpdate(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow);


PXPublic PXResult PXAPI PXWindowReDrawEnable(const PXWindowHandle pxWindowID, const PXBool enable);

PXPublic PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXWindowHandle pxWindowID, PXI32S* const x, PXI32S* const y);
PXPublic PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXWindowHandle pxWindowID, PXI32S* const x, PXI32S* const y);

PXPublic PXBool PXAPI PXWindowValueFetch
(
    PXWindow* const pxUIElementList, //
    const PXSize dataListAmount,
    const PXUIElementProperty pxUIElementProperty,
    void* const dataList // The given data
);





// QueryDisplayConfig












PXPublic PXResult PXAPI PXWindowTabListSwapPage(PXWindow* const pxWindow);





PXPublic PXResult PXAPI PXGUIIconGetSystem(PXIcon* const pxIcon, const int iconID);
PXPublic PXResult PXAPI PXGUIIconGetViaFilePath(PXIcon* const pxIcon, const char* fileExtension);




PXPublic void PXAPI PXWindowhSizeRefresAll(PXGUISystem* const pxGUISystem);




PXPublic PXResult PXAPI PXGUIWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo);







PXPublic PXResult PXAPI PXWindowMouseMovementEnable(const PXWindowHandle pxWindow);



PXPublic void PXAPI PXWindowCursorCaptureMode(const PXWindowHandle pxWindowID, const PXWindowCursorMode cursorMode);





// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXWindowHandle pxWindowID);

PXPrivate PXI32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);



#endif

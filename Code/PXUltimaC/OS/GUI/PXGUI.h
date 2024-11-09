#ifndef PXGUIINCLUDE
#define PXGUIINCLUDE

#include <Media/PXResource.h>
#include <Container/Dictionary/PXDictionary.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Hardware/PXMouse.h>
#include <OS/Hardware/PXKeyBoard.h>
#include <OS/Async/PXProcess.h>
#include <OS/Async/PXThread.h>
#include <OS/File/PXFile.h>




// Manages library calls to the operating system window manager
typedef struct PXGUISystem_
{
    PXLibrary LibraryWindowsUser32DLL;

    PXBool AreOSUIElementsDefined;

    PXResourceManager* ResourceManager;

    PXDisplay DisplayCurrent;

    // private
    PXWindowBrush* BrushBackgroundDark;
    PXWindowBrush* BrushTextWhite;

    PXFont* FontTitle;
    PXFont* FontContent;
}
PXGUISystem;






// TODO: Global Variable, bad but needed for SYNC with OS. Stupid design
PXPrivate PXGUISystem* PXGUISystemGlobalReference = PXNull;


//---------------------------------------------------------
// GUI system functions
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem);
PXPublic PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem);

#if OSUnix
PXPublic PXActionResult PXAPI PXWindowErrorFromXSystem(const int xSysstemErrorID);
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------
PXPublic void PXAPI PXWindowEventUpdate(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);

PXPublic void PXAPI PXWindowEventConsumer(PXGUISystem* const pxGUISystem, PXWindowEvent* const pxWindowEvent);

#if OSUnix
PXPrivate void PXWindowEventHandler(PXWindow* const pxWindow, const XEvent* const xEventData);
#elif PXOSWindowsDestop
PXPublic LRESULT CALLBACK PXWindowEventHandler(const HWND PXWindowsID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);

PXPrivate void PXAPI PXWindowChildListEnumerate(PXGUISystem* const pxGUISystem, PXWindow* const parent, PXBool visible);
PXPrivate BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam);
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Window utility functions
//---------------------------------------------------------

PXPublic PXActionResult PXAPI PXWindowCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount);
PXPublic PXActionResult PXAPI PXWindowUpdate(PXGUISystem* const pxGUISystem, PXWindowUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount);
PXPublic PXActionResult PXAPI PXWindowFetch(PXGUISystem* const pxGUISystem, PXWindowUpdateInfo* const pxGUIElementUpdateInfoList, const PXSize amount);
PXPublic PXActionResult PXAPI PXWindowRelease(PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXWindowDelete(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);


//---------------------------------------------------------



//---------------------------------------------------------
// Window propertys - Getter / Setter
//---------------------------------------------------------
PXPublic PXBool PXAPI PXWindowIsEnabled(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXWindowMove(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXWindowResize(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int width, const int height);
PXPublic PXActionResult PXAPI PXWindowMoveAndResize(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);

PXPublic PXActionResult PXAPI PXWindowTextSet(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, char* text);
PXPublic PXActionResult PXAPI PXWindowTextGet(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, char* text);
PXPublic PXActionResult PXAPI PXWindowStyleUpdate(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXWindowTitleBarColorSet(const PXNativDrawWindowHandle pxWindowID);


PXPublic PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXNativDrawWindowHandle pxWindowID, PXInt32S* const x, PXInt32S* const y);
PXPublic PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXNativDrawWindowHandle pxWindowID, PXInt32S* const x, PXInt32S* const y);

PXPublic PXBool PXAPI PXWindowValueFetch
(
    PXWindow* const pxUIElementList, //
    const PXSize dataListAmount,
    const PXUIElementProperty pxUIElementProperty,
    void* const dataList // The given data
);



PXPublic void PXAPI PXGUIScreenGetSize(PXInt32S* const width, PXInt32S* const height);

// QueryDisplayConfig













//---------------------------------------------------------
// Window Drag & Drop
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXWindowDragStart(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
//---------------------------------------------------------


//---------------------------------------------------------
// Default rendering functions
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXWindowDrawCustomTabList(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomFailback(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomText(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomButton(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomComboBox(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomColorPicker(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomHexView(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawFileDirectoryView(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
//---------------------------------------------------------



PXPublic PXActionResult PXAPI PXGUIIconGetSystem(PXIcon* const pxIcon, const int iconID);
PXPublic PXActionResult PXAPI PXGUIIconGetViaFilePath(PXIcon* const pxIcon, const char* fileExtension);




PXPublic void PXAPI PXWindowhSizeRefresAll(PXGUISystem* const pxGUISystem);




PXPublic PXActionResult PXAPI PXWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo);



PXPublic PXNativDrawWindowHandle PXAPI PXWindowFindViaTitle(const PXText* const windowTitle);







PXPublic PXActionResult PXAPI PXWindowMouseMovementEnable(const PXNativDrawWindowHandle pxWindow);

PXPublic PXActionResult PXAPI PXWindowPosition(const PXNativDrawWindowHandle pxWindowID, PXInt32S* x, PXInt32S* y);


PXPublic void PXAPI PXWindowCursorCaptureMode(const PXNativDrawWindowHandle pxWindowID, const PXWindowCursorMode cursorMode);





// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXNativDrawWindowHandle pxWindowID);

PXPrivate PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);



#endif

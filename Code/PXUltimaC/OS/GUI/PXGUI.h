#ifndef PXGUIINCLUDE
#define PXGUIINCLUDE

#include <Media/PXType.h>
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

    PXNativDraw NativDraw;
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
#if OSUnix
#elif PXOSWindowsDestop
PXPrivate void PXAPI PXWindowChildListEnumerate(PXGUISystem* const pxGUISystem, PXWindow* const parent, PXBool visible);
PXPrivate BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam);
#endif
//---------------------------------------------------------



//---------------------------------------------------------
// Window utility functions
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXWindowCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount);
PXPublic PXActionResult PXAPI PXWindowFetch(PXGUISystem* const pxGUISystem, PXWindowPropertyInfo* const pxGUIElementUpdateInfoList, const PXSize amount);
PXPublic PXActionResult PXAPI PXWindowRelease(PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXWindowDelete(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
//---------------------------------------------------------



//---------------------------------------------------------
// Window propertys - Getter / Setter
//---------------------------------------------------------
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





// QueryDisplayConfig













//---------------------------------------------------------
// Window Drag & Drop
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXWindowDragStart(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
//---------------------------------------------------------


//---------------------------------------------------------
// Default rendering functions
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXWindowDrawCustomRectangle3D(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXWindowDrawCustomHeader(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomFooter(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomResourceView(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
PXPublic PXActionResult PXAPI PXWindowDrawCustomResourceInfo(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo);
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



PXPublic void PXAPI PXWindowCursorCaptureMode(const PXNativDrawWindowHandle pxWindowID, const PXWindowCursorMode cursorMode);





// Checks if the current window is the one in focus.
PXPublic PXBool PXAPI PXWindowIsInFocus(const PXNativDrawWindowHandle pxWindowID);

PXPrivate PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon);



#endif

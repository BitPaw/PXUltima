#pragma once

#ifndef PXNativDrawIncluded
#define PXNativDrawIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Math/PXVector.h>

#define PXGUIDrawModeFront (1<<0)
#define PXGUIDrawModeBack  (1<<1)

typedef struct PXGUIProperty_ PXGUIProperty_;

#if OSUnix
// Fetch, translate and display a error from the native draw system. Only used for linux X11 for now
PXPrivate PXResult PXAPI PXNativeDrawErrorFetch(const PXBool condition);
#endif

//------------------------
// Debug
//------------------------
PXPublic PXResult PXAPI PXNativDrawWindowPrintHirachy(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, int depth);
//------------------------

PXPublic PXResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw PXREF pxNativDraw);

// Connect to a display server and fetch data
// name=NULL for the default display
PXPublic PXResult PXAPI PXNativDrawDisplayOpen(PXNativDraw PXREF pxNativDraw, PXDisplay PXREF pxDisplay, const char PXREF displayName);


PXPublic PXResult PXAPI PXNativDrawWindowCreate(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo);


// Events
PXPublic PXResult PXAPI PXNativDrawWindowEventPendingAmount(PXNativDraw PXREF pxNativDraw, PXSize PXREF amount);


// Buffer Swap
PXPublic PXResult PXAPI PXNativDrawWindowBufferSwap(PXNativDraw PXREF pxNativDraw, const PXWindow PXREF pxWindow);


// Use for a seperate window that needs to be merged into a main one.
// Given a spesific window we can try to absorb the contens and underlieing elemetns and move them into your own space.
// Objects shall not be created or destroyed, simply the ownership of those objects should be transphered. (can we do that?)
PXPublic PXResult PXAPI PXWindowAbsorb(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);

// Use for draging a window outside it own borders to spawn a new one.
PXPublic PXResult PXAPI PXWindowEmit(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow);


//---------------------------------------------------------
// Main
//---------------------------------------------------------
PXPublic PXNativDraw* PXAPI PXNativDrawInstantance(void);
PXPublic PXResult PXAPI PXNativDrawWindowEventPoll(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXNativDrawWindowIDValid(PXNativDraw PXREF pxNativDraw, const PXWindowHandle pxNativDrawWindowHandle);
PXPublic PXResult PXAPI PXNativDrawWindowFetch(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const PXWindowHandle pxNativDrawWindowHandle);

PXPublic PXResult PXAPI PXNativDrawFrameBufferCreate(PXNativDraw PXREF pxNativDraw, PXFrameBuffer PXREF pxFrameBuffer, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo);


PXPublic void PXAPI PXNativDrawScreenSizeGet(PXRectangleXYWHI32 PXREF pxRectangleXYWH);

//---------------------------------------------------------
// Drag'n'Drop
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawClear(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXNativDrawBegin(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXNativDrawEnd(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawBrushCreate(PXNativDraw PXREF pxNativDraw, PXWindowBrush PXREF pxWindowBrush, PXColorRGBI8 PXREF pxColorRGBI8);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawColorSetBrush(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXWindowBrush PXREF pxWindowBrush, const char mode);
PXPublic PXResult PXAPI PXNativDrawSetV3(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXColorRGBI8 PXREF pxColorRGBI8, const char mode);
PXPublic PXResult PXAPI PXNativDrawSetRGB(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, char red, char green, char blue, const char mode);
//---------------------------------------------------------


//---------------------------------------------------------
// Font/Text
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawFontListFetch(PXNativDraw PXREF pxNativDraw);
PXPublic PXResult PXAPI PXNativDrawFontLoadA(PXNativDraw PXREF pxNativDraw, PXFont PXREF pxFont, const char PXREF name, const PXSize nameLength);
PXPublic PXResult PXAPI PXNativDrawFontRelease(PXNativDraw PXREF pxNativDraw, PXFont PXREF pxFont);
PXPublic PXResult PXAPI PXNativDrawFontSelect(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXFont PXREF pxFont);
//---------------------------------------------------------



//---------------------------------------------------------
// Scrollbar
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawScrollbarUpdate(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXScollbar PXREF pxScollbar);
//---------------------------------------------------------


//---------------------------------------------------------
// Icons
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawIconFromImage(PXNativDraw PXREF pxNativDraw, PXIcon PXREF pxIcon, PXTexture PXREF PXTexture);
PXPublic PXResult PXAPI PXNativDrawIconFromAtlas(PXNativDraw PXREF pxNativDraw, PXIcon PXREF pxIcon, PXTexture PXREF PXTexture);
//---------------------------------------------------------




//---------------------------------------------------------
// Property
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawWindowProperty(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXGUIProperty PXREF pxWindowPropertyInfoList, const PXSize amount);

PXPublic PXResult PXAPI PXNativDrawTextSet(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const char PXREF text, const PXSize textLength);
PXPublic PXResult PXAPI PXNativDrawTextGet(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const char PXREF text, const PXSize textLength);
PXPublic PXResult PXAPI PXNativDrawWindowIsEnabled(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXNativDrawWindowXYWH(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32, const PXBool doWrite);
PXPublic PXResult PXAPI PXNativDrawWindowResize(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawWindowMoveAndResize(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
//---------------------------------------------------------

//---------------------------------------------------------
// Icon
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawIcon(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, const int x, const int y, const int size);

//---------------------------------------------------------
// Cursor
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawCursorPosition(PXNativDraw PXREF pxNativDraw, PXVector2I32S PXREF position, const PXBool isWrite);

// Collsison
PXPublic PXResult PXAPI PXNativDrawCursorCollisionCheck(PXNativDraw PXREF pxNativDraw);



//PXPublic PXResult PXAPI PXWindowIconCorner();
//PXPublic PXResult PXAPI PXWindowIconTaskBar();


// Draw text into a given window
// Example: Text for a button
//PXPublic PXResult PXAPI PXWindowDrawText(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow, PXText PXREF pxText);
PXPublic PXResult PXAPI PXNativDrawA
(
    PXNativDraw PXREF pxNativDraw,
    PXWindow PXREF pxWindow,
    int x,
    int y,
    int width,
    int height,
    const char PXREF text,
    const PXSize textSize,
    const PXI32U behaviour
);
PXPublic PXResult PXAPI PXNativDrawW(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const wchar_t PXREF text, const PXSize textSize);
PXPublic PXResult PXAPI PXNativDrawPoint(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y);
PXPublic PXResult PXAPI PXNativDrawPoints(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawLine(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x1, const int y1, const int x2, const int y2);
PXPublic PXResult PXAPI PXNativDrawLines(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawRectangle(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height, PXWindowBrush PXREF pxWindowBrush);
PXPublic PXResult PXAPI PXNativDrawRectangleRounded(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawIconLoad(PXNativDraw PXREF pxNativDraw, PXIcon PXREF pxIcon, const char* iconName);
PXPublic PXResult PXAPI PXNativDrawIconDraw(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, const int x, const int y, const int width, const int height);
//---------------------------------------------------------




//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------

// This function consumes events first before any other 
// listener revices this. Because of this, this event handler 
// shall only handle low-level events and not be too much in the way
PXPublic PXResult PXAPI PXNativDrawEventConsumer(PXNativDraw PXREF pxNativDraw, PXWindowEvent PXREF pxWindowEvent);

#if OSUnix
PXPublic void PXNativDrawEventTranslator(PXNativDraw PXREF pxNativDraw, const XEvent PXREF xEventData);
#elif PXOSWindowsDestop
PXPublic LRESULT CALLBACK PXNativDrawEventTranslator(const HWND windowID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);
#endif
//---------------------------------------------------------


//---------------------------------------------------------
// 
//---------------------------------------------------------
#if OSUnix
//----
#elif PXOSWindowsDestop
PXPrivate void PXAPI PXWindowChildListEnumerate(PXGUIManager PXREF pxGUIManager, PXWindow PXREF parent, PXBool visible);
PXPrivate BOOL CALLBACK PXNativeWindowListUpdateEnum(const HWND windowHandle, LPARAM lParam);
PXPrivate BOOL CALLBACK PXNativeWindowChildrenUpdateEnum(const HWND windowHandle, LPARAM lParam);
#endif

PXPublic PXResult PXAPI PXNativeWindowListUpdate(const PXWindowHandle pxWindowHandle);
//---------------------------------------------------------



PXPublic PXResult PXAPI PXNativeDrawChildrenReDraw(PXNativDraw PXREF pxNativDraw, PXWindow PXREF pxWindow);


PXPublic void PXAPI PXNativeDrawRefreshSizeAllChildren(PXNativDraw PXREF pxNativDraw, PXWindow PXREF window);


PXPublic void PXAPI PXNativeDrawMouseTrack(PXWindow PXREF window);


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
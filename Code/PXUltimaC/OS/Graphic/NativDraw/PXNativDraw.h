#ifndef PXNativDrawINCLUDE
#define PXNativDrawINCLUDE

#include <Media/PXResource.h>

#define PXGUIDrawModeFront (1<<0)
#define PXGUIDrawModeBack  (1<<1)

#if OSUnix
// Fetch, translate and display a error from the native draw system. Only used for linux X11 for now
PXPrivate PXActionResult PXAPI PXNativeDrawErrorFetch(const PXBool condition);
#endif

//------------------------
// Debug
//------------------------
PXPublic PXActionResult PXAPI PXNativDrawWindowPrintHirachy(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, int depth);
//------------------------

PXPublic PXActionResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw* const pxNativDraw);

// Connect to a display server and fetch data
// name=NULL for the default display
PXPublic PXActionResult PXAPI PXNativDrawDisplayOpen(PXNativDraw* const pxNativDraw, PXDisplay* const pxDisplay, const char* const displayName);


PXPublic PXActionResult PXAPI PXNativDrawWindowCreate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo);


// Events
PXPublic PXActionResult PXAPI PXNativDrawWindowEventPendingAmount(PXNativDraw* const pxNativDraw, PXSize* const amount);


// Buffer Swap
PXPublic PXActionResult PXAPI PXNativDrawWindowBufferSwap(PXNativDraw* const pxNativDraw, const PXWindow* const pxWindow);


// Use for a seperate window that needs to be merged into a main one.
// Given a spesific window we can try to absorb the contens and underlieing elemetns and move them into your own space.
// Objects shall not be created or destroyed, simply the ownership of those objects should be transphered. (can we do that?)
PXPublic PXActionResult PXAPI PXWindowAbsorb(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);

// Use for draging a window outside it own borders to spawn a new one.
PXPublic PXActionResult PXAPI PXWindowEmit(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);


//---------------------------------------------------------
// Main
//---------------------------------------------------------
PXPublic PXNativDraw* PXAPI PXNativDrawInstantance(void);
PXPublic PXActionResult PXAPI PXNativDrawWindowEventPoll(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXNativDrawWindowIDValid(PXNativDraw* const pxNativDraw, const PXNativDrawWindowHandle pxNativDrawWindowHandle);
PXPublic PXActionResult PXAPI PXNativDrawWindowFetch(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const PXNativDrawWindowHandle pxNativDrawWindowHandle);


PXPublic void PXAPI PXNativDrawScreenSizeGet(PXRectangleXYWHI32* const pxRectangleXYWH);

//---------------------------------------------------------
// Drag'n'Drop
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawClear(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXNativDrawBegin(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXNativDrawEnd(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawBrushCreate(PXNativDraw* const pxNativDraw, PXWindowBrush* const pxWindowBrush, PXColorRGBI8* const pxColorRGBI8);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawColorSetBrush(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, PXWindowBrush* const pxGUIElementBrush, const char mode);
PXPublic PXActionResult PXAPI PXNativDrawSetV3(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, PXColorRGBI8* const pxColorRGBI8, const char mode);
PXPublic PXActionResult PXAPI PXNativDrawSetRGB(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, char red, char green, char blue, const char mode);
//---------------------------------------------------------


//---------------------------------------------------------
// Font/Text
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawFontListFetch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXNativDrawFontLoadA(PXNativDraw* const pxNativDraw, PXFont* const pxFont, const char* const name, const PXSize nameLength);
PXPublic PXActionResult PXAPI PXNativDrawFontRelease(PXNativDraw* const pxNativDraw, PXFont* const pxFont);
PXPublic PXActionResult PXAPI PXNativDrawFontSelect(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXFont* const pxFont);
//---------------------------------------------------------



//---------------------------------------------------------
// Scrollbar
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawScrollbarUpdate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXScollbar* const pxScollbar);
//---------------------------------------------------------


//---------------------------------------------------------
// Icons
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawIconFromImage(PXNativDraw* const pxNativDraw, PXIcon* const pxIcon, PXImage* const pxImage);
PXPublic PXActionResult PXAPI PXNativDrawIconFromAtlas(PXNativDraw* const pxNativDraw, PXIcon* const pxIcon, PXImage* const pxImage);
//---------------------------------------------------------




//---------------------------------------------------------
// Property
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawWindowProperty(PXNativDraw* const pxNativDraw, PXWindowPropertyInfo* const pxWindowPropertyInfoList, const PXSize amount);

PXPublic PXActionResult PXAPI PXNativDrawTextSet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const char* const text, const PXSize textLength);
PXPublic PXActionResult PXAPI PXNativDrawTextGet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const char* const text, const PXSize textLength);
PXPublic PXActionResult PXAPI PXNativDrawWindowIsEnabled(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow);
PXPublic PXActionResult PXAPI PXNativDrawWindowXYWH(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXRectangleXYWHI32* const pxRectangleXYWHI32, const PXBool doWrite);
PXPublic PXActionResult PXAPI PXNativDrawWindowResize(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawWindowMoveAndResize(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
//---------------------------------------------------------

//---------------------------------------------------------
// Icon
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawIcon(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXIcon* const pxIcon, const int x, const int y, const int size);

//---------------------------------------------------------
// Cursor
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawCursorPosition(PXNativDraw* const pxNativDraw, PXVector2I* const position, const PXBool isWrite);

// Collsison
PXPublic PXActionResult PXAPI PXNativDrawCursorCollisionCheck(PXNativDraw* const pxNativDraw);



//PXPublic PXActionResult PXAPI PXWindowIconCorner();
//PXPublic PXActionResult PXAPI PXWindowIconTaskBar();


// Draw text into a given window
// Example: Text for a button
//PXPublic PXActionResult PXAPI PXWindowDrawText(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXText* const pxText);
PXPublic PXActionResult PXAPI PXNativDrawTextA
(
    PXNativDraw* const pxNativDraw,
    PXWindow* const pxGUIElement,
    int x,
    int y,
    int width,
    int height,
    const char* const text,
    const PXSize textSize,
    const PXInt32U behaviour
);
PXPublic PXActionResult PXAPI PXNativDrawTextW(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const wchar_t* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXNativDrawPoint(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXNativDrawPoints(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawLine(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x1, const int y1, const int x2, const int y2);
PXPublic PXActionResult PXAPI PXNativDrawLines(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawRectangle(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawRectangleRounded(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawIconLoad(PXNativDraw* const pxNativDraw, PXIcon* const pxIcon, const char* iconName);
PXPublic PXActionResult PXAPI PXNativDrawIconDraw(PXNativDraw* const pxNativDraw, PXWindow* const pxGUIElement, PXIcon* const pxIcon, const int x, const int y, const int width, const int height);
//---------------------------------------------------------




//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------

// This function consumes events first before any other 
// listener revices this. Because of this, this event handler 
// shall only handle low-level events and not be too much in the way
PXPublic PXActionResult PXAPI PXNativDrawEventConsumer(PXNativDraw* const pxNativDraw, PXWindowEvent* const pxWindowEvent);

#if OSUnix
PXPublic void PXNativDrawEventTranslator(PXNativDraw* const pxNativDraw, const XEvent* const xEventData);
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
PXPrivate void PXAPI PXWindowChildListEnumerate(PXGUISystem* const pxGUISystem, PXWindow* const parent, PXBool visible);
PXPrivate BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam);
#endif
//---------------------------------------------------------


PXPublic void PXAPI PXNativeDrawRefreshSizeAllChildren(PXNativDraw* const pxNativDraw, PXWindow* const window);


PXPublic void PXAPI PXNativeDrawMouseTrack(PXWindow* const window);


/*

PXPublic PXActionResult PXAPI PXAllPlanes(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBlackPixel(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWhitePixel(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConnectionNumber(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultDepth(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListDepths(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultRootWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultScreenOfDisplay(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenOfDisplay(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultVisual(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayCells(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayPlanes(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXExtendedMaxRequestSize(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMaxRequestSize(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLastKnownRequestProcessed(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXNextRequest(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXProtocolVersion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXProtocolRevision(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQLength(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRootWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenCount(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXServerVendor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXVendorRelease(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListPixmapFormats(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXImageByteOrder(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBitmapUnit(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBitmapBitOrder(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBitmapPad(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayHeight(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayHeightMM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayWidth(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayWidthMM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBlackPixelOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWhitePixelOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCellsOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultColormapOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultDepthOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultGCOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultVisualOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDoesBackingStore(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDoesSaveUnders(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenNumberOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEventMaskOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWidthOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXHeightOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWidthMMOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXHeightMMOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMaxCmapsOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMinCmapsOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPlanesOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRootWindowOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInitThreads(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAddConnectionWatch(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRemoveConnectionWatch(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXProcessInternalConnection(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInternalConnectionNumbers(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXVisualIDFromVisual(PXNativDrawText* const pxNativDraw);

PXPublic PXActionResult PXAPI PXQueryTree(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWindowAttributes(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTranslateCoordinates(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryPointer(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInternAtom(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInternAtoms(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetAtomName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetAtomNames(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWindowProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListProperties(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetSelectionOwner(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreatePixmap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateFontCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateGlyphCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreatePixmapCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCopyColormapAndFree(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookupColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXParseColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsLookupColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAllocColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocNamedColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAllocNamedColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocColorCells(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocColorPlanes(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsStoreColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsStoreColors(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryColor(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryColors(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCCCOfColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetCCCOfColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsDefaultCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsDisplayOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsVisualOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsScreenNumberOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsScreenWhitePointOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsClientWhitePointOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetWhitePoint(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetCompressionProc(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetWhiteAdjustProc(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCreateCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsFreeCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsConvertColors(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatus(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatus(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryBlack(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryBlue(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryGreen(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryRed(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryWhite(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMaxC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMaxL(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMaxLC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMinL(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMaxC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMaxL(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMaxLC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMinL(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxV(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxVC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxVSamples(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMinV(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAddColorSpace(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsFormatOfPrefix(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsPrefixOfFormat(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsParseStringProc(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConversionProc(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConversionProc(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAddFunctionSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatus(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetGCValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFlushGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestSize(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestTile(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestStipple(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLoadFont(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryFont(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLoadQueryFont(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetFontProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListFonts(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListFontsWithInfo(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTextWidth(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTextWidth16(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInitImage(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetImage(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetSubImage(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListInstalledColormaps(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetFontPath(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEventsQueued(PXNativDrawText* const pxNativDraw);

PXPublic PXActionResult PXAPI PXCheckWindowEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCheckMaskEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCheckTypedEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCheckTypedWindowEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSendEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetMotionEvents(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetErrorHandler(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGrabPointer(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGrabKeyboard(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetPointerMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetPointerMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetKeyboardMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXNewModifiermap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInsertModifiermapEntry(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDeleteModifiermapEntry(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetModifierMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetModifierMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSupportsLocale(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetLocaleModifiers(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXVaCreateNestedList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXOpenOM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCloseOM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetOMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetOMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayOfOM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLocaleOfOM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateOC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyOC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXOMOfOC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetOCValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetOCValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFontsOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBaseFontNameListOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLocaleOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFreeFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDirectionalDependentDrawing(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXContextualDrawing(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXContextDependentDrawing(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXExtentsOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextEscapement(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextEscapement(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextPerCharExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextPerCharExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbDrawText(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcDrawText(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbDrawString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcDrawString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbDrawImageString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcDrawImageString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXOpenIM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCloseIM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetIMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetIMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayOfIM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLocaleOfIM(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRegisterIMInstantiateCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXIMInstantiateCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXUnregisterIMInstantiateCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateIC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyIC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetICFocus(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXUnsetICFocus(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbResetIC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcResetIC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXIMOfIC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetICValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetICValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditStateNotifyCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCallbackPrototype(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGeometryCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStringConversionCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditStartCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditDoneCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditDrawCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditCaretCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatusStartCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatusDoneCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatusDrawCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFilterEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbLookupString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcLookupString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXIconifyWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWithdrawWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReconfigureWMWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextListToTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextListToTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextPropertyToTextList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextPropertyToTextList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcFreeStringList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStringListToTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTextPropertyToStringList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFreeStringList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFetchName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMIconName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMIconName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetIconName(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocWMHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMNormalHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMNormalHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocClassHint(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetClassHint(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetTransientForHint(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMProtocols(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMProtocols(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMColormapWindows(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMColormapWindows(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocIconSize(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetIconSizes(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbSetWMProperties(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMProperties(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetCommand(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMClientMachine(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMClientMachine(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetRGBColormaps(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetRGBColormaps(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmUniqueQuark(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmStringToQuark(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQuarkToString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmStringToQuarkList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmInitialize(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetFileDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutFileDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXResourceManagerString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenResourceString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetStringDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmLocaleOfDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmDestroyDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmSetDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmCombineFileDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmCombineDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmMergeDatabases(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQGetResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQGetSearchResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQGetSearchResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQPutResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutStringResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQPutStringResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutLineResource(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmEnumerateDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmParseCommand(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookupKeysym(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXKeycodeToKeysym(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXKeysymToKeycode(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConvertCase(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStringToKeysym(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXKeysymToString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookupString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXpermalloc(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXParseGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWMGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPolygonRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEmptyRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEqualRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPointInRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRectInRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFetchBytes(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFetchBuffer(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMatchVisualInfo(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateImage(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetPixel(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSubImage(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReadBitmapFile(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReadBitmapFileData(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWriteBitmapFile(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreatePixmapFromBitmapData(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateBitmapFromData(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSaveContext(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFindContext(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDeleteContext(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryExtension(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListExtensions(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInitExtension(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAddExtension(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCloseDisplay(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCreateGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCopyGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetFreeGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCreateFont(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetFreeFont(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetWireToEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetEventToWire(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetWireToError(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetError(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetErrorString(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetPrintErrorValues(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetFlushGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCopyGC(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEHeadOfExtensionList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFindOnExtensionList(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReply(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocScratch(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocTemp(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFreeTemp(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetNormalHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetZoomHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetStandardColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetDefault(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDraw(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDrawFilled(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateAssocTable(PXNativDrawText* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookUpAssoc(PXNativDrawText* const pxNativDraw);


*/




#endif
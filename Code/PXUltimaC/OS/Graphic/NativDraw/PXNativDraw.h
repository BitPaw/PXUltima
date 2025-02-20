#ifndef PXNativDrawINCLUDE
#define PXNativDrawINCLUDE

#include <Media/PXResource.h>

#define PXGUIDrawModeFront (1<<0)
#define PXGUIDrawModeBack  (1<<1)

#if OSUnix
// Fetch, translate and display a error from the native draw system. Only used for linux X11 for now
PXPrivate PXActionResult PXAPI PXNativeDrawErrorFetch(const PXBool condition);
#endif

PXPublic PXActionResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw* const pxNativDraw);

// Connect to a display server and fetch data
// name=NULL for the default display
PXPublic PXActionResult PXAPI PXNativDrawDisplayOpen(PXNativDraw* const pxNativDraw, PXDisplay* const pxDisplay, const char* const displayName);


PXPublic PXActionResult PXAPI PXNativDrawWindowCreate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo);


PXPublic PXActionResult PXAPI PXNativDrawRectangleParent(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXRectangleXYWHI32* const pxRectangleXYWHI32);


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
PXPublic PXActionResult PXAPI PXNativDrawWindowPosition(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXVector2I* const position, const PXBool doWrite);
PXPublic PXActionResult PXAPI PXNativDrawWindowMove(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y);
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



PXPrivate void PXAPI PXNativeDrawMouseTrack(PXWindow* const window);




PXPublic PXActionResult PXAPI PXAllPlanes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBlackPixel(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWhitePixel(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConnectionNumber(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultDepth(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListDepths(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultRootWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultScreenOfDisplay(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenOfDisplay(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultVisual(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayCells(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayPlanes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXExtendedMaxRequestSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMaxRequestSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLastKnownRequestProcessed(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXNextRequest(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXProtocolVersion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXProtocolRevision(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQLength(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRootWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenCount(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXServerVendor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXVendorRelease(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListPixmapFormats(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXImageByteOrder(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBitmapUnit(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBitmapBitOrder(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBitmapPad(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayHeight(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayHeightMM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayWidth(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayWidthMM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBlackPixelOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWhitePixelOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCellsOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultColormapOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultDepthOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultGCOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultVisualOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDoesBackingStore(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDoesSaveUnders(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenNumberOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEventMaskOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWidthOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXHeightOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWidthMMOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXHeightMMOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMaxCmapsOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMinCmapsOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPlanesOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRootWindowOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInitThreads(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAddConnectionWatch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRemoveConnectionWatch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXProcessInternalConnection(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInternalConnectionNumbers(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXVisualIDFromVisual(PXNativDraw* const pxNativDraw);

PXPublic PXActionResult PXAPI PXQueryTree(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWindowAttributes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTranslateCoordinates(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryPointer(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInternAtom(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInternAtoms(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetAtomName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetAtomNames(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWindowProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListProperties(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetSelectionOwner(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreatePixmap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateFontCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateGlyphCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreatePixmapCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCopyColormapAndFree(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookupColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXParseColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsLookupColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAllocColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocNamedColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAllocNamedColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocColorCells(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocColorPlanes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsStoreColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsStoreColors(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryColors(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCCCOfColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetCCCOfColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsDefaultCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsDisplayOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsVisualOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsScreenNumberOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsScreenWhitePointOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsClientWhitePointOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetWhitePoint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetCompressionProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsSetWhiteAdjustProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCreateCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsFreeCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsConvertColors(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryBlack(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryBlue(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryGreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryRed(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsQueryWhite(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMaxC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMaxL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMaxLC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELabQueryMinL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMaxC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMaxL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMaxLC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsCIELuvQueryMinL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxV(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxVC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMaxVSamples(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsTekHVCQueryMinV(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAddColorSpace(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsFormatOfPrefix(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsPrefixOfFormat(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsParseStringProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConversionProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConversionProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXcmsAddFunctionSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetGCValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFlushGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestTile(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryBestStipple(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLoadFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLoadQueryFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetFontProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListFonts(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListFontsWithInfo(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTextWidth(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTextWidth16(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInitImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetSubImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListInstalledColormaps(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetFontPath(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEventsQueued(PXNativDraw* const pxNativDraw);

PXPublic PXActionResult PXAPI PXCheckWindowEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCheckMaskEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCheckTypedEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCheckTypedWindowEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSendEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetMotionEvents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetErrorHandler(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGrabPointer(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGrabKeyboard(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetPointerMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetPointerMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetKeyboardMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXNewModifiermap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInsertModifiermapEntry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDeleteModifiermapEntry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetModifierMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetModifierMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSupportsLocale(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetLocaleModifiers(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXVaCreateNestedList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXOpenOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCloseOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetOMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetOMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayOfOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLocaleOfOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateOC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyOC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXOMOfOC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetOCValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetOCValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFontsOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXBaseFontNameListOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLocaleOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFreeFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDirectionalDependentDrawing(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXContextualDrawing(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXContextDependentDrawing(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXExtentsOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextEscapement(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextEscapement(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextPerCharExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextPerCharExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbDrawText(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcDrawText(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbDrawString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcDrawString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbDrawImageString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcDrawImageString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXOpenIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCloseIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetIMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetIMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDisplayOfIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLocaleOfIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRegisterIMInstantiateCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXIMInstantiateCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXUnregisterIMInstantiateCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetICFocus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXUnsetICFocus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbResetIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcResetIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXIMOfIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetICValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetICValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditStateNotifyCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCallbackPrototype(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGeometryCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDestroyCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStringConversionCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditStartCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditDoneCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditDrawCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPreeditCaretCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatusStartCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatusDoneCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStatusDrawCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFilterEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbLookupString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcLookupString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXIconifyWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWithdrawWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReconfigureWMWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextListToTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextListToTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbTextPropertyToTextList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcTextPropertyToTextList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXwcFreeStringList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDefaultString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStringListToTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXTextPropertyToStringList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFreeStringList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFetchName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMIconName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMIconName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetIconName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocWMHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMNormalHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMNormalHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocClassHint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetClassHint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetTransientForHint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMProtocols(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMProtocols(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMColormapWindows(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMColormapWindows(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocIconSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetIconSizes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXmbSetWMProperties(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMProperties(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetCommand(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetWMClientMachine(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetWMClientMachine(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSetRGBColormaps(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetRGBColormaps(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmUniqueQuark(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmStringToQuark(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQuarkToString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmStringToQuarkList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmInitialize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetFileDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutFileDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXResourceManagerString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXScreenResourceString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetStringDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmLocaleOfDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmDestroyDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmSetDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmCombineFileDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmCombineDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmMergeDatabases(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmGetResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQGetResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQGetSearchResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQGetSearchResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQPutResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutStringResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmQPutStringResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmPutLineResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmEnumerateDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXrmParseCommand(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookupKeysym(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXKeycodeToKeysym(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXKeysymToKeycode(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXConvertCase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXStringToKeysym(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXKeysymToString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookupString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXpermalloc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXParseGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWMGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPolygonRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEmptyRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEqualRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXPointInRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXRectInRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFetchBytes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFetchBuffer(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXMatchVisualInfo(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetPixel(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSubImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReadBitmapFile(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReadBitmapFileData(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXWriteBitmapFile(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreatePixmapFromBitmapData(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateBitmapFromData(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXSaveContext(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFindContext(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDeleteContext(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXQueryExtension(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXListExtensions(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXInitExtension(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAddExtension(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCloseDisplay(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCreateGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCopyGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetFreeGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCreateFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetFreeFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetWireToEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetEventToWire(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetWireToError(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetError(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetErrorString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetPrintErrorValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetFlushGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXESetCopyGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXEHeadOfExtensionList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFindOnExtensionList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXReply(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocScratch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXAllocTemp(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXFreeTemp(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetNormalHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetZoomHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetStandardColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXGetDefault(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDraw(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXDrawFilled(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXCreateAssocTable(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXLookUpAssoc(PXNativDraw* const pxNativDraw);







#endif
#ifndef PXNativDrawINCLUDE
#define PXNativDrawINCLUDE

#include <Media/PXResource.h>

#define PXGUIDrawModeFront (1<<0)
#define PXGUIDrawModeBack  (1<<1)

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


//---------------------------------------------------------
// Drag'n'Drop
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawClear(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXNativDrawBegin(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
PXPublic PXActionResult PXAPI PXNativDrawEnd(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawColorSetBrush(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowBrush* const pxGUIElementBrush, const char mode);
PXPublic PXActionResult PXAPI PXNativDrawSetV3(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXColorRGBI8* const pxColorRGBI8, const char mode);
PXPublic PXActionResult PXAPI PXNativDrawSetRGB(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, char red, char green, char blue, const char mode);
//---------------------------------------------------------


//---------------------------------------------------------
// Font/Text
//---------------------------------------------------------
PXPublic PXActionResult PXAPI PXNativDrawFontListFetch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PXNativDrawFontLoadA(PXNativDraw* const pxNativDraw, PXFont* const pxFont, const char* const name, const PXSize nameLength);
PXPublic PXActionResult PXAPI PXNativDrawFontRelease(PXNativDraw* const pxNativDraw, PXFont* const pxFont);
PXPublic PXActionResult PXAPI PXNativDrawFontSet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXFont* const pxFont);
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
PXPublic PXActionResult PXAPI PXNativDrawTextA(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const char* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXNativDrawTextW(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const wchar_t* const text, const PXSize textSize);
PXPublic PXActionResult PXAPI PXNativDrawPoint(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y);
PXPublic PXActionResult PXAPI PXNativDrawPoints(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawLine(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x1, const int y1, const int x2, const int y2);
PXPublic PXActionResult PXAPI PXNativDrawLines(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawRectangle(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawRectangleRounded(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height);
PXPublic PXActionResult PXAPI PXNativDrawLoad(PXGUISystem* const pxGUISystem, PXIcon* const pxIcon, const char* iconName);
PXPublic PXActionResult PXAPI PXNativDrawIcon(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXIcon* const pxIcon, const int x, const int y, const int width, const int height);
//---------------------------------------------------------




//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------

// This function consumes events first before any other 
// listener revices this. Because of this, this event handler 
// shall only handle low-level events and not be too much in the way
PXPublic PXActionResult PXAPI PXNativDrawEventConsumer(PXNativDraw* const pxNativDraw, PXWindowEvent* const pxWindowEvent);

#if OSUnix
PXPublic void PXNativDrawEventReceiver(PXWindow* const pxWindow, const XEvent* const xEventData);
#elif PXOSWindowsDestop
PXPublic LRESULT CALLBACK PXNativDrawEventReceiver(const HWND windowID, const UINT eventID, const WPARAM wParam, const LPARAM lParam);
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




PXPublic PXActionResult PXAPI XAllPlanes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XBlackPixel(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWhitePixel(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XConnectionNumber(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultDepth(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListDepths(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultRootWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultScreenOfDisplay(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XScreenOfDisplay(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultVisual(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayCells(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayPlanes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XExtendedMaxRequestSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XMaxRequestSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLastKnownRequestProcessed(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XNextRequest(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XProtocolVersion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XProtocolRevision(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQLength(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XRootWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XScreenCount(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XServerVendor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XVendorRelease(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListPixmapFormats(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XImageByteOrder(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XBitmapUnit(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XBitmapBitOrder(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XBitmapPad(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayHeight(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayHeightMM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayWidth(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayWidthMM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XBlackPixelOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWhitePixelOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCellsOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultColormapOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultDepthOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultGCOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultVisualOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDoesBackingStore(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDoesSaveUnders(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XScreenNumberOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XEventMaskOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWidthOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XHeightOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWidthMMOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XHeightMMOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XMaxCmapsOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XMinCmapsOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XPlanesOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XRootWindowOfScreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInitThreads(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAddConnectionWatch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XRemoveConnectionWatch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XProcessInternalConnection(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInternalConnectionNumbers(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XVisualIDFromVisual(PXNativDraw* const pxNativDraw);

PXPublic PXActionResult PXAPI XQueryTree(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWindowAttributes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XTranslateCoordinates(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryPointer(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInternAtom(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInternAtoms(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetAtomName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetAtomNames(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWindowProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListProperties(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetSelectionOwner(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreatePixmap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateFontCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateGlyphCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreatePixmapCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryBestCursor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCopyColormapAndFree(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLookupColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XParseColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsLookupColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsAllocColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocNamedColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsAllocNamedColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocColorCells(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocColorPlanes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsStoreColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsStoreColors(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryColor(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryColors(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCCCOfColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsSetCCCOfColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsDefaultCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsDisplayOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsVisualOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsScreenNumberOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsScreenWhitePointOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsClientWhitePointOfCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsSetWhitePoint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsSetCompressionProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsSetWhiteAdjustProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCreateCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsFreeCCC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsConvertColors(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI Status(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI Status(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryBlack(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryBlue(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryGreen(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryRed(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsQueryWhite(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELabQueryMaxC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELabQueryMaxL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELabQueryMaxLC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELabQueryMinL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELuvQueryMaxC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELuvQueryMaxL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELuvQueryMaxLC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsCIELuvQueryMinL(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsTekHVCQueryMaxC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsTekHVCQueryMaxV(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsTekHVCQueryMaxVC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsTekHVCQueryMaxVSamples(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsTekHVCQueryMinV(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsAddColorSpace(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsFormatOfPrefix(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsPrefixOfFormat(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsParseStringProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI ConversionProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI ConversionProc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XcmsAddFunctionSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI Status(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetGCValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFlushGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryBestSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryBestTile(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryBestStipple(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLoadFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLoadQueryFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetFontProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListFonts(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListFontsWithInfo(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XTextWidth(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XTextWidth16(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInitImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetSubImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListInstalledColormaps(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetFontPath(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XEventsQueued(PXNativDraw* const pxNativDraw);

PXPublic PXActionResult PXAPI XCheckWindowEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCheckMaskEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCheckTypedEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCheckTypedWindowEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSendEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetMotionEvents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetErrorHandler(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGrabPointer(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGrabKeyboard(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetPointerMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetPointerMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetKeyboardMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XNewModifiermap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInsertModifiermapEntry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDeleteModifiermapEntry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetModifierMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetModifierMapping(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSupportsLocale(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetLocaleModifiers(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XVaCreateNestedList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XOpenOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCloseOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetOMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetOMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayOfOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLocaleOfOM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateOC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDestroyOC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XOMOfOC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetOCValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetOCValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFontsOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XBaseFontNameListOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLocaleOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFreeFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDirectionalDependentDrawing(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XContextualDrawing(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XContextDependentDrawing(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XExtentsOfFontSet(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbTextEscapement(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcTextEscapement(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbTextExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcTextExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbTextPerCharExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcTextPerCharExtents(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbDrawText(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcDrawText(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbDrawString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcDrawString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbDrawImageString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcDrawImageString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XOpenIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCloseIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetIMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetIMValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDisplayOfIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLocaleOfIM(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XRegisterIMInstantiateCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI IMInstantiateCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XUnregisterIMInstantiateCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI DestroyCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDestroyIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetICFocus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XUnsetICFocus(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbResetIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcResetIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XIMOfIC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetICValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetICValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PreeditStateNotifyCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI CallbackPrototype(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI GeometryCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI DestroyCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI StringConversionCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PreeditStartCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PreeditDoneCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PreeditDrawCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI PreeditCaretCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI StatusStartCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI StatusDoneCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI StatusDrawCallback(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFilterEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbLookupString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcLookupString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XIconifyWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWithdrawWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XReconfigureWMWindow(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbTextListToTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcTextListToTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbTextPropertyToTextList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcTextPropertyToTextList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XwcFreeStringList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDefaultString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XStringListToTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XTextPropertyToStringList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFreeStringList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetTextProperty(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFetchName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMIconName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMIconName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetIconName(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocWMHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMNormalHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMNormalHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocClassHint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetClassHint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetTransientForHint(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMProtocols(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMProtocols(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMColormapWindows(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMColormapWindows(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocIconSize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetIconSizes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XmbSetWMProperties(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMProperties(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetCommand(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetWMClientMachine(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetWMClientMachine(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSetRGBColormaps(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetRGBColormaps(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmUniqueQuark(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmStringToQuark(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmQuarkToString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmStringToQuarkList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmInitialize(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmGetFileDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmPutFileDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XResourceManagerString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XScreenResourceString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmGetStringDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmLocaleOfDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmDestroyDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmSetDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmGetDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmCombineFileDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmCombineDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmMergeDatabases(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmGetResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmQGetResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmQGetSearchResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmQGetSearchResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmPutResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmQPutResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmPutStringResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmQPutStringResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmPutLineResource(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmEnumerateDatabase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XrmParseCommand(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLookupKeysym(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XKeycodeToKeysym(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XKeysymToKeycode(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XConvertCase(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XStringToKeysym(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XKeysymToString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLookupString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI Xpermalloc(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XParseGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWMGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XPolygonRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XEmptyRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XEqualRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XPointInRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XRectInRegion(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFetchBytes(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFetchBuffer(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XMatchVisualInfo(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetPixel(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSubImage(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XReadBitmapFile(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XReadBitmapFileData(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XWriteBitmapFile(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreatePixmapFromBitmapData(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateBitmapFromData(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XSaveContext(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFindContext(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDeleteContext(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XQueryExtension(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XListExtensions(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XInitExtension(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAddExtension(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetCloseDisplay(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetCreateGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetCopyGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetFreeGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetCreateFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetFreeFont(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetWireToEvent(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetEventToWire(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetWireToError(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetError(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetErrorString(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetPrintErrorValues(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetFlushGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XESetCopyGC(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XEHeadOfExtensionList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFindOnExtensionList(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XReply(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocScratch(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XAllocTemp(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XFreeTemp(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetNormalHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetZoomHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetSizeHints(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetStandardColormap(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGeometry(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XGetDefault(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDraw(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XDrawFilled(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XCreateAssocTable(PXNativDraw* const pxNativDraw);
PXPublic PXActionResult PXAPI XLookUpAssoc(PXNativDraw* const pxNativDraw);







#endif
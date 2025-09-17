#ifndef PXNativDrawIncluded
#define PXNativDrawIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Math/PXVector.h>

#define PXGUIDrawModeFront (1<<0)
#define PXGUIDrawModeBack  (1<<1)

#if OSUnix
// Fetch, translate and display a error from the native draw system. Only used for linux X11 for now
PXPrivate PXResult PXAPI  PXNativeDrawErrorFetch(const PXBool condition);
#endif

//------------------------
// Debug
//------------------------
PXPublic PXResult PXAPI PXNativDrawWindowPrintHirachy(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, int depth);
//------------------------

PXPublic PXResult PXAPI PXNativDrawDisplayListFetch(PXNativDraw* const pxNativDraw);

// Connect to a display server and fetch data
// name=NULL for the default display
PXPublic PXResult PXAPI PXNativDrawDisplayOpen(PXNativDraw* const pxNativDraw, PXDisplay* const pxDisplay, const char* const displayName);


PXPublic PXResult PXAPI PXNativDrawWindowCreate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo);


// Events
PXPublic PXResult PXAPI PXNativDrawWindowEventPendingAmount(PXNativDraw* const pxNativDraw, PXSize* const amount);


// Buffer Swap
PXPublic PXResult PXAPI PXNativDrawWindowBufferSwap(PXNativDraw* const pxNativDraw, const PXWindow* const pxWindow);


// Use for a seperate window that needs to be merged into a main one.
// Given a spesific window we can try to absorb the contens and underlieing elemetns and move them into your own space.
// Objects shall not be created or destroyed, simply the ownership of those objects should be transphered. (can we do that?)
PXPublic PXResult PXAPI PXWindowAbsorb(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow);

// Use for draging a window outside it own borders to spawn a new one.
PXPublic PXResult PXAPI PXWindowEmit(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow);


//---------------------------------------------------------
// Main
//---------------------------------------------------------
PXPublic PXNativDraw* PXAPI PXNativDrawInstantance(void);
PXPublic PXResult PXAPI PXNativDrawWindowEventPoll(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow);
PXPublic PXResult PXAPI PXNativDrawWindowIDValid(PXNativDraw* const pxNativDraw, const PXWindowHandle pxNativDrawWindowHandle);
PXPublic PXResult PXAPI PXNativDrawWindowFetch(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const PXWindowHandle pxNativDrawWindowHandle);


PXPublic void PXAPI PXNativDrawScreenSizeGet(PXRectangleXYWHI32* const pxRectangleXYWH);

//---------------------------------------------------------
// Drag'n'Drop
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawClear(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow);
PXPublic PXResult PXAPI PXNativDrawBegin(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow);
PXPublic PXResult PXAPI PXNativDrawEnd(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawBrushCreate(PXNativDraw* const pxNativDraw, PXWindowBrush* const pxWindowBrush, PXColorRGBI8* const pxColorRGBI8);
//---------------------------------------------------------


//---------------------------------------------------------
// Color for drawing
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawColorSetBrush(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXWindowBrush* const pxWindowBrush, const char mode);
PXPublic PXResult PXAPI PXNativDrawSetV3(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXColorRGBI8* const pxColorRGBI8, const char mode);
PXPublic PXResult PXAPI PXNativDrawSetRGB(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, char red, char green, char blue, const char mode);
//---------------------------------------------------------


//---------------------------------------------------------
// Font/Text
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawFontListFetch(PXNativDraw* const pxNativDraw);
PXPublic PXResult PXAPI PXNativDrawFontLoadA(PXNativDraw* const pxNativDraw, PXFont* const pxFont, const char* const name, const PXSize nameLength);
PXPublic PXResult PXAPI PXNativDrawFontRelease(PXNativDraw* const pxNativDraw, PXFont* const pxFont);
PXPublic PXResult PXAPI PXNativDrawFontSelect(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXFont* const pxFont);
//---------------------------------------------------------



//---------------------------------------------------------
// Scrollbar
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawScrollbarUpdate(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXScollbar* const pxScollbar);
//---------------------------------------------------------


//---------------------------------------------------------
// Icons
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawIconFromImage(PXNativDraw* const pxNativDraw, PXIcon* const pxIcon, PXTexture* const PXTexture);
PXPublic PXResult PXAPI PXNativDrawIconFromAtlas(PXNativDraw* const pxNativDraw, PXIcon* const pxIcon, PXTexture* const PXTexture);
//---------------------------------------------------------




//---------------------------------------------------------
// Property
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawWindowProperty(PXNativDraw* const pxNativDraw, PXWindowPropertyInfo* const pxWindowPropertyInfoList, const PXSize amount);

PXPublic PXResult PXAPI PXNativDrawTextSet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const char* const text, const PXSize textLength);
PXPublic PXResult PXAPI PXNativDrawTextGet(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const char* const text, const PXSize textLength);
PXPublic PXResult PXAPI PXNativDrawWindowIsEnabled(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow);
PXPublic PXResult PXAPI PXNativDrawWindowXYWH(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXRectangleXYWHI32* const pxRectangleXYWHI32, const PXBool doWrite);
PXPublic PXResult PXAPI PXNativDrawWindowResize(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawWindowMoveAndResize(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
//---------------------------------------------------------

//---------------------------------------------------------
// Icon
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawIcon(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXIcon* const pxIcon, const int x, const int y, const int size);

//---------------------------------------------------------
// Cursor
//---------------------------------------------------------
PXPublic PXResult PXAPI PXNativDrawCursorPosition(PXNativDraw* const pxNativDraw, PXVector2I32S* const position, const PXBool isWrite);

// Collsison
PXPublic PXResult PXAPI PXNativDrawCursorCollisionCheck(PXNativDraw* const pxNativDraw);



//PXPublic PXResult PXAPI PXWindowIconCorner();
//PXPublic PXResult PXAPI PXWindowIconTaskBar();


// Draw text into a given window
// Example: Text for a button
//PXPublic PXResult PXAPI PXWindowDrawText(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow, PXText* const pxText);
PXPublic PXResult PXAPI PXNativDrawA
(
    PXNativDraw* const pxNativDraw,
    PXWindow* const pxWindow,
    int x,
    int y,
    int width,
    int height,
    const char* const text,
    const PXSize textSize,
    const PXI32U behaviour
);
PXPublic PXResult PXAPI PXNativDrawW(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const wchar_t* const text, const PXSize textSize);
PXPublic PXResult PXAPI PXNativDrawPoint(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y);
PXPublic PXResult PXAPI PXNativDrawPoints(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawLine(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x1, const int y1, const int x2, const int y2);
PXPublic PXResult PXAPI PXNativDrawLines(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawRectangle(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height, PXWindowBrush* const pxWindowBrush);
PXPublic PXResult PXAPI PXNativDrawRectangleRounded(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, const int x, const int y, const int width, const int height);
PXPublic PXResult PXAPI PXNativDrawIconLoad(PXNativDraw* const pxNativDraw, PXIcon* const pxIcon, const char* iconName);
PXPublic PXResult PXAPI PXNativDrawIconDraw(PXNativDraw* const pxNativDraw, PXWindow* const pxWindow, PXIcon* const pxIcon, const int x, const int y, const int width, const int height);
//---------------------------------------------------------




//---------------------------------------------------------
// Window event functions
//---------------------------------------------------------

// This function consumes events first before any other 
// listener revices this. Because of this, this event handler 
// shall only handle low-level events and not be too much in the way
PXPublic PXResult PXAPI PXNativDrawEventConsumer(PXNativDraw* const pxNativDraw, PXWindowEvent* const pxWindowEvent);

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

PXPublic PXResult PXAPI PXAllPlanes(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXBlackPixel(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWhitePixel(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXConnectionNumber(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultDepth(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListDepths(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultRootWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultScreenOfDisplay(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXScreenOfDisplay(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultVisual(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayCells(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayPlanes(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXExtendedMaxRequestSize(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXMaxRequestSize(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLastKnownRequestProcessed(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXNextRequest(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXProtocolVersion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXProtocolRevision(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQLength(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXRootWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXScreenCount(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXServerVendor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXVendorRelease(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListPixmapFormats(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXTextureByteOrder(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXBitmapUnit(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXBitmapBitOrder(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXBitmapPad(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayHeight(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayHeightMM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayWidth(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayWidthMM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXBlackPixelOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWhitePixelOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCellsOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultColormapOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultDepthOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultGCOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultVisualOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDoesBackingStore(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDoesSaveUnders(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXScreenNumberOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXEventMaskOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWidthOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXHeightOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWidthMMOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXHeightMMOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXMaxCmapsOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXMinCmapsOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPlanesOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXRootWindowOfScreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXInitThreads(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAddConnectionWatch(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXRemoveConnectionWatch(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXProcessInternalConnection(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXIernalConnectionNumbers(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXVisualIDFromVisual(PXNativDrawText* const pxNativDraw);

PXPublic PXResult PXAPI PXQueryTree(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWindowAttributes(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXTranslateCoordinates(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryPointer(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXIernAtom(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXIernAtoms(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetAtomName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetAtomNames(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWindowProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListProperties(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetSelectionOwner(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreatePixmap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateFontCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateGlyphCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreatePixmapCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestCursor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCopyColormapAndFree(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLookupColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXParseColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsLookupColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsAllocColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocNamedColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsAllocNamedColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocColorCells(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocColorPlanes(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsStoreColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsStoreColors(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryColor(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryColors(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCCCOfColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetCCCOfColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsDefaultCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsDisplayOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsVisualOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsScreenNumberOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsScreenWhitePointOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsClientWhitePointOfCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetWhitePoint(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetCompressionProc(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsSetWhiteAdjustProc(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCreateCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsFreeCCC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsConvertColors(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStatus(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStatus(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryBlack(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryBlue(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryGreen(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryRed(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsQueryWhite(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMaxC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMaxL(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMaxLC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELabQueryMinL(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMaxC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMaxL(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMaxLC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsCIELuvQueryMinL(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxV(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxVC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMaxVSamples(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsTekHVCQueryMinV(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsAddColorSpace(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsFormatOfPrefix(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsPrefixOfFormat(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsParseStringProc(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXConversionProc(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXConversionProc(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXcmsAddFunctionSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStatus(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetGCValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFlushGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestSize(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestTile(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryBestStipple(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLoadFont(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryFont(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLoadQueryFont(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetFontProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListFonts(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListFontsWithInfo(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWidth(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWidth16(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXInitImage(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetImage(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetSubImage(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListInstalledColormaps(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetFontPath(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXEventsQueued(PXNativDrawText* const pxNativDraw);

PXPublic PXResult PXAPI PXCheckWindowEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCheckMaskEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCheckTypedEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCheckTypedWindowEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSendEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetMotionEvents(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetErrorHandler(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGrabPointer(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGrabKeyboard(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetPointerMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetPointerMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetKeyboardMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXNewModifiermap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXInsertModifiermapEntry(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDeleteModifiermapEntry(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetModifierMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetModifierMapping(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSupportsLocale(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetLocaleModifiers(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXVaCreateNestedList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXOpenOM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCloseOM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetOMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetOMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayOfOM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLocaleOfOM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateOC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDestroyOC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXOMOfOC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetOCValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetOCValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFontsOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXBaseFontNameListOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLocaleOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFreeFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDirectionalDependentDrawing(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXContextualDrawing(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXContextDependentDrawing(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXExtentsOfFontSet(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbTextEscapement(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcTextEscapement(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbTextExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcTextExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbTextPerCharExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcTextPerCharExtents(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbDrawText(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcDrawText(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbDrawString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcDrawString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbDrawImageString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcDrawImageString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXOpenIM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCloseIM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetIMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetIMValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDisplayOfIM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLocaleOfIM(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXRegisterIMInstantiateCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXIMInstantiateCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXUnregisterIMInstantiateCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDestroyCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateIC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDestroyIC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetICFocus(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXUnsetICFocus(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbResetIC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcResetIC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXIMOfIC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetICValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetICValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPreeditStateNotifyCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCallbackPrototype(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGeometryCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDestroyCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStringConversionCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPreeditStartCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPreeditDoneCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPreeditDrawCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPreeditCaretCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStatusStartCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStatusDoneCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStatusDrawCallback(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFilterEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbLookupString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcLookupString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXIconifyWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWithdrawWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXReconfigureWMWindow(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbTextListToTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcTextListToTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbTextPropertyToTextList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcTextPropertyToTextList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXwcFreeStringList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDefaultString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStringListToTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXTextPropertyToStringList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFreeStringList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetTextProperty(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFetchName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMIconName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMIconName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetIconName(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocWMHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMNormalHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMNormalHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocClassHint(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetClassHint(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetTransientForHint(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMProtocols(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMProtocols(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMColormapWindows(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMColormapWindows(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocIconSize(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetIconSizes(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXmbSetWMProperties(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMProperties(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetCommand(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetWMClientMachine(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetWMClientMachine(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSetRGBColormaps(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetRGBColormaps(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmUniqueQuark(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmStringToQuark(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmQuarkToString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmStringToQuarkList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmInitialize(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmGetFileDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmPutFileDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXResourceManagerString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXScreenResourceString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmGetStringDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmLocaleOfDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmDestroyDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmSetDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmGetDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmCombineFileDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmCombineDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmMergeDatabases(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmGetResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmQGetResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmQGetSearchResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmQGetSearchResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmPutResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmQPutResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmPutStringResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmQPutStringResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmPutLineResource(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmEnumerateDatabase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXrmParseCommand(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLookupKeysym(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXKeycodeToKeysym(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXKeysymToKeycode(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXConvertCase(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXStringToKeysym(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXKeysymToString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLookupString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXpermalloc(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXParseGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWMGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPolygonRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXEmptyRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXEqualRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXPointInRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXRectInRegion(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFetchBytes(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFetchBuffer(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXMatchVisualInfo(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateImage(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetPixel(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSubImage(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXReadBitmapFile(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXReadBitmapFileData(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXWriteBitmapFile(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreatePixmapFromBitmapData(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateBitmapFromData(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXSaveContext(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFindContext(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDeleteContext(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXQueryExtension(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXListExtensions(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXInitExtension(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAddExtension(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetCloseDisplay(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetCreateGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetCopyGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetFreeGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetCreateFont(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetFreeFont(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetWireToEvent(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetEventToWire(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetWireToError(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetError(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetErrorString(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetPrintErrorValues(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetFlushGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXESetCopyGC(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXEHeadOfExtensionList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFindOnExtensionList(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXReply(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocScratch(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXAllocTemp(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXFreeTemp(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetNormalHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetZoomHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetSizeHints(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetStandardColormap(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGeometry(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXGetDefault(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDraw(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXDrawFilled(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXCreateAssocTable(PXNativDrawText* const pxNativDraw);
PXPublic PXResult PXAPI PXLookUpAssoc(PXNativDrawText* const pxNativDraw);


*/




#endif
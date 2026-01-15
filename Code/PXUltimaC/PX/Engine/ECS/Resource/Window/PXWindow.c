#include "PXWindow.h"
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Font/PXFont.h>
#include <PX/Engine/ECS/Resource/FrameBuffer/PXFrameBuffer.h>
#include <PX/OS/PXOS.h>
#include <PX/Engine/ECS/Resource/Brush/PXBrush.h>
#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Input/PXInput.h>

#if OSUnix
#elif OSWindows
#include <Windows.h>
#include <CommCtrl.h>
#include <wingdi.h>
#include <windowsx.h>
#include <Dbt.h>
#include <uxtheme.h>   // optional, not required
#include <dwmapi.h>    // optional, not required

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "opengl32.lib")

const char WindowsLibraryDWMAPI[] = "DWMAPI.DLL";
const char WindowsLibraryDWMAPISET[] = "DwmSetWindowAttribute";

#endif


#include <gl/GLU.h>
const char PXTextFailsafe[] = "<missing text>";
const PXI8U PXTextFailsafeLength = sizeof(PXTextFailsafe);

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <gl/GL.h>


ATOM _pxEngineBaseClassID = 0;
HKL _keyBoardLayout = PXNull; // Selected keyboardlayout in taskbar
wchar_t _keyBoardLayoutName[KL_NAMELENGTH];

// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXWindow_
{
    PXECSInfo Info;

    PXGraphicSystem GraphicSystem; // The window can be drawn by different systems.

    // Used as the render sink target
    PXFrameBuffer* FrameBuffer;

#if OSWindows

    union 
    {
        HMENU MenuHandle;
        HWND WindowHandle;
    };

   // HWND ownerMain; // Parant
    PXWindow* WindowParent;

    HDC DeviceContextHandle;
    HGLRC RenderContext;

    PXBuffer InputBuffer; // Windows want a buffer like this

#endif


    PXWindowEventList EventList;


    PXUIElementPosition Position;

    //---<State-Info>------------------------
   // PXWindowBrush* BrushFront;
   // PXWindowBrush* BrushBackground;
    PXFont* FontForText;
    PXIcon* Icon; // Icon to be rendered

    //PXColorRGBAF* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
    PXUIHoverState Hover;
    PXI32U FlagsList;
    //---------------------------------------


    //---[Extended data]---------------------
    PXUIElementType Type;
    void* ExtendedData; // Extra data that will be allocated seperatly for additional use
    //---------------------------------------

    PXColorRGBI8 BackGroundColor;

    PXWindowDockSide  dockSide;
    BOOL      floating;   // if TRUE, excluded from docking layout
  
    int       headerH;    // legacy; used as fallback min height

    // ---- Layout extensions (optional; defaults apply if 0) ----
    int       prefW;      // preferred band width for LEFT/RIGHT
    int       prefH;      // preferred band height for TOP/BOTTOM

    int       fixedLong;  // fixed size along stack axis (px). For LEFT/RIGHT: height; TOP/BOTTOM: width; CENTER: height.
    double    weight;     // flexible share weight along stack axis (<=0 => 1.0)

    int       minW;       // minimum width of the final rect given to the child (px)
    int       minH;       // minimum height of the final rect given to the child (px)

    // Margins around pane (outside the pane)
    int       marginL, marginT, marginR, marginB;

    // Padding inside pane (applied to rect before MoveWindow)
    int       padL, padT, padR, padB;

    // Optional per-pane spacing override among siblings; 0 => use container default
    int       spacing;
}
PXWindow;


const char PXWindowTextText[] = "Window";
const char PXRectangleText[] = "Rectangle";
const char PXRectangle2DText[] = "Rectangle2D";
const char PXRectangle3DText[] = "Rectangle3D";

const char PXWindowClassName[] = "PX_Window_Main";
const wchar_t PXWindowClassNameW[] = L"PX_Window_Main";
const PXECSRegisterInfoStatic PXWindowRegisterInfoStatic =
{
    {sizeof(PXWindowTextText), sizeof(PXWindowTextText), PXWindowTextText, TextFormatASCII},
    sizeof(PXWindow),
    __alignof(PXWindow),
    PXECSTypeResource,
    PXWindowCreate,
    PXWindowDestroy,
    PXNull
};
PXECSRegisterInfoDynamic PXWindowRegisterInfoDynamic;

PXFont* _lastFont = PXNull;



#if 0
static const wchar_t* kPanePropName = L"__PanePtr__";

static void PaneAttach(HWND hChild, PXWindow* p) {
    p->WindowHandle = hChild; // store HWND in pane for convenience
    SetPropW(hChild, kPanePropName, (HANDLE)p);
}

static Pane* PaneFromHwnd(HWND hChild) {
    return (Pane*)GetPropW(hChild, kPanePropName);
}
#else

#endif

PXWindow* PXAPI PXWindowRootGet(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXNull;
    }

#if 0 // Search

    HWND rootOwner = GetAncestor(pxWindow->WindowHandle, GA_PARENT);

    if(!rootOwner)
    {
        return PXNull;
    }

    PXWindow* pxWindowRoot = (PXWindow*)GetWindowLongPtr(rootOwner, GWLP_USERDATA);

    return pxWindowRoot;
#else

    PXWindow* root = pxWindow->WindowParent;

    if(!root)
    {
        return pxWindow;
    }

    while(root->WindowParent)
    {
        root = root->WindowParent;
    }

    return root;

#endif

 
}

PXBool PXAPI PXWindowIsRoot(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXFalse;
    }
  
    LONG style = GetWindowLong(pxWindow->WindowHandle, GWL_STYLE);

    // A top-level window has no WS_CHILD and no parent 
    if(!(style & WS_CHILD) && GetParent(pxWindow->WindowHandle) == NULL)
        return TRUE;
    return FALSE;
    
}

PXResult PXAPI PXWindowOpenGLEnable(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;
    BOOL isOK = PXFalse;
    PXWindow* windowParent = pxWindow->WindowParent;

    pxWindow->GraphicSystem = PXGraphicSystemOpenGL;

    PXWindowPixelSystemInfo pxWindowPixelSystemInfo;
    PXClear(PXWindowPixelSystemInfo, &pxWindowPixelSystemInfo);
    pxWindowPixelSystemInfo.OpenGL = PXTrue;
    pxWindowPixelSystemInfo.GDI= PXTrue;
    pxWindowPixelSystemInfo.DirectX = PXTrue;
    pxWindowPixelSystemInfo.BitPerPixel = 32;

    PXWindowPixelSystemSet(pxWindow, &pxWindowPixelSystemInfo);

    PXRectangleXYWHI32 pxRectangleXYWHI32;
    PXWindowRectangleXYWH(pxWindow, &pxRectangleXYWHI32);

    HGLRC glContextNew = wglCreateContext(pxWindow->DeviceContextHandle);
    pxResult = PXErrorCurrent(glContextNew);

    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "OpenGL",
        "Context-Create:<%s>",
        glContextNew != PXNull ? "OK" : "FAILURE"
    );

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    // If we have a parent, share objects between parent
    if(windowParent)
    {
        HGLRC glContextParent = windowParent->RenderContext;

        isOK = wglShareLists(glContextParent, glContextNew);
        pxResult = PXErrorCurrent(isOK);

        PXLogPrint
        (
            isOK ? PXLoggingInfo : PXLoggingError,
            PXWindowTextText,
            "OpenGL",
            "Context-Share:<%s>",
            isOK ? "OK" : "FAILURE"
        );
    }

    pxWindow->RenderContext = glContextNew;

    isOK = wglMakeCurrent(pxWindow->DeviceContextHandle, glContextNew);

    glViewport(0, 0, pxRectangleXYWHI32.Width, pxRectangleXYWHI32.Height); 

    //isOK = wglMakeCurrent(pxWindow->DeviceContextHandle, 0);

    return PXResultOK;
}

static void LayoutDockedAA(PXWindow PXREF pxWindow)
{
    RECT rc = PXWindowRectOf(pxWindow->WindowHandle);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;
    const int bandW = 260;  // left/right band width
    const int bandH = 180;  // top band height

    for(HWND child = GetWindow(pxWindow->WindowHandle, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT))
    {
        PXWindow* p = PaneFromHwnd(child);
        if(!p)
            continue;

        RECT r = { 0 };
        switch(p->dockSide) {
            case PXWindowDockSideLeft:
                r.left = leftX;
                r.right = leftX + bandW;
                r.top = rc.top;
                r.bottom = rc.bottom;
                leftX += bandW;
                break;
            case PXWindowDockSideRight:
                r.left = rightX - bandW;
                r.right = rightX;
                r.top = rc.top;
                r.bottom = rc.bottom;
                rightX -= bandW;
                break;
            case PXWindowDockSideTop:
                r.left = rc.left;
                r.right = rc.right;
                r.top = topY;
                r.bottom = topY + bandH;
                topY += bandH;
                break;
            case PXWindowDockSideBottom:
                r.left = rc.left;
                r.right = rc.right;
                r.top = bottomY - bandH;
                r.bottom = bottomY;
                bottomY -= bandH;
                break;

            default:
                r = rc;
                break;
        }
        MoveWindow(child, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
    }
}

void LayoutDockedGOOD(PXWindow PXREF pxWindow)
{
    RECT rc = PXWindowRectOf(pxWindow->WindowHandle);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;

    const int bandW = 260;  // left/right band width (could be per-pane)
    const int bandH = 180;  // top/bottom band height (could be per-pane)

    // PASS 1: LEFT/RIGHT
    for(HWND child = GetWindow(pxWindow->WindowHandle, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
        PXWindow* p = PaneFromHwnd(child);
        if(!p) continue;

        RECT r;
        switch(p->dockSide) {
            case PXWindowDockSideLeft:
                r.left = leftX; r.right = leftX + bandW;
                r.top = rc.top; r.bottom = rc.bottom; // full height
                leftX += bandW;
                MoveWindow(child, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
                break;
            case PXWindowDockSideRight:
                r.left = rightX - bandW; r.right = rightX;
                r.top = rc.top; r.bottom = rc.bottom; // full height
                rightX -= bandW;
                MoveWindow(child, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
                break;
            default:
                break;
        }
    }

    // PASS 2: TOP/BOTTOM — use remaining width [leftX, rightX]
    for(HWND child = GetWindow(pxWindow->WindowHandle, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
        PXWindow* p = PaneFromHwnd(child);
        if(!p) continue;

        RECT r;
        switch(p->dockSide) {
            case PXWindowDockSideTop:
                r.left = leftX; r.right = rightX;
                r.top = topY;   r.bottom = topY + bandH;
                topY += bandH;
                MoveWindow(child, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
                break;
            case PXWindowDockSideBottom:
                r.left = leftX; r.right = rightX;
                r.top = bottomY - bandH; r.bottom = bottomY;
                bottomY -= bandH;
                MoveWindow(child, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
                break;
            default:
                break;
        }
    }

    // PASS 3: CENTER (or default)
    for(HWND child = GetWindow(pxWindow->WindowHandle, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
        PXWindow* p = PaneFromHwnd(child);
        if(!p) continue;

        if(p->dockSide == PXWindowDockSideNone /*DOCK_FILL  or your default/none enum */) {
            RECT r = { leftX, topY, rightX, bottomY };
            MoveWindow(child, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
        }
    }
}

// -----------------------------
// Types from your current model
// -----------------------------

// Defaults
static DockLayoutConfig DockDefaultConfig(void) 
{
    DockLayoutConfig cfg;
    cfg.defaultBandW = 260;
    cfg.defaultBandH = 180;
    cfg.defaultSpacing = 8;
    cfg.padLeft = (BoxInsets){ 8, 8, 8, 8 };
    cfg.padRight = (BoxInsets){ 8, 8, 8, 8 };
    cfg.padTop = (BoxInsets){ 8, 8, 8, 8 };
    cfg.padBottom = (BoxInsets){ 8, 8, 8, 8 };
    cfg.padCenter = (BoxInsets){ 8, 8, 8, 8 };
    cfg.centerVertical = TRUE;
    return cfg;
}

// -----------------------------
// Styles to reduce overdraw
// -----------------------------
void InitDockContainerStyles(PXWindow PXREF pxWindow)
{
    HWND hwnd = pxWindow->WindowHandle;

    LONG_PTR st = GetWindowLongPtr(hwnd, GWL_STYLE);
    st |= WS_CLIPCHILDREN;           // parent won't draw under children
    SetWindowLongPtr(hwnd, GWL_STYLE, st);

    SetWindowPos
    (
        hwnd,
        NULL, 
        0,
        0, 
        0, 
        0,
        SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE
    );
}

static void InitDockChildStyles(PXWindow PXREF pxWindow)
{
    HWND hwnd = pxWindow->WindowHandle;
    LONG_PTR st = GetWindowLongPtr(hwnd, GWL_STYLE);
    st |= WS_CLIPSIBLINGS;           // siblings avoid overdrawing each other
    SetWindowLongPtr(hwnd, GWL_STYLE, st);
}

// -----------------------------
// Utility
// -----------------------------
static int clamp_nonneg(int v) { return v < 0 ? 0 : v; }
static int CountVisiblePanes(HWND hParent)
{
    int total = 0;
    for(HWND child = GetWindow(hParent, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
        if(!IsWindowVisible(child)) continue;
        PXWindow* p = PaneFromHwnd(child);
        if(!p || p->floating) continue;
        ++total;
    }
    return total;
}



static int CollectSide(HWND hParent, PXWindowDockSide side, Paneref* buf, int cap)
{
    int n = 0;
    for(HWND child = GetWindow(hParent, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
        if(!IsWindowVisible(child)) continue;
        PXWindow* p = PaneFromHwnd(child);
        if(!p || p->floating) continue;
        if(p->dockSide != side) continue;
        if(n < cap) { buf[n].h = child; buf[n].p = p; }
        ++n;
    }
    return n;
}

// -----------------------------
// Axis distribution with margin & padding
// -----------------------------
// Distributes space along the stack axis among panes.
// vertical = TRUE: axis = Y (heights vary), width = area width.
// vertical = FALSE: axis = X (widths vary), height = area height.
//
// We compute each pane's SLOT size (the space it consumes along the axis).
// SLOT size includes: marginBefore + content + marginAfter + paddingBefore + paddingAfter.
// The rectangle given to the child will then be SLOT rect inset by margins and padding.
static void DistributeAxisWithBox(Paneref* arr, int n, int availableAxis, int spacing,
                                  BOOL vertical, int* outSlotSizes /* n */)
{
    int gaps = (n > 1 ? spacing * (n - 1) : 0);

    // Sum fixed segment (fixedLong + margins + padding) and total weight
    int totalFixed = 0;
    double totalWeight = 0.0;

    for(int i = 0; i < n; ++i) {
        PXWindow* p = arr[i].p;
        int mBefore = vertical ? p->marginT : p->marginL;
        int mAfter = vertical ? p->marginB : p->marginR;
        int padBefore = vertical ? p->padT : p->padL;
        int padAfter = vertical ? p->padB : p->padR;
        int axisExtra = mBefore + mAfter + padBefore + padAfter;

        if(p->fixedLong > 0) {
            totalFixed += p->fixedLong + axisExtra;
        }
        else {
            totalWeight += (p->weight > 0.0 ? p->weight : 1.0);
            // axisExtra will be added per pane to slot later
        }
    }

    int remaining = availableAxis - totalFixed - gaps;
    if(remaining < 0) remaining = 0;

    // Assign slots: content from weights for flexible panes, plus margins/padding
    for(int i = 0; i < n; ++i) {
        PXWindow* p = arr[i].p;
        int mBefore = vertical ? p->marginT : p->marginL;
        int mAfter = vertical ? p->marginB : p->marginR;
        int padBefore = vertical ? p->padT : p->padL;
        int padAfter = vertical ? p->padB : p->padR;
        int axisExtra = mBefore + mAfter + padBefore + padAfter;

        int content = 0;
        if(p->fixedLong > 0) {
            content = p->fixedLong;
        }
        else {
            double w = (p->weight > 0.0 ? p->weight : 1.0);
            content = (totalWeight > 0.0) ? (int)floor((w / totalWeight) * (double)remaining + 0.5) : 0;
        }

        // Min constraint along axis applies to content
        if(vertical) {
            int minH = (p->minH > 0 ? p->minH : (p->headerH > 0 ? p->headerH : 0));
            if(content < minH) content = minH;
        }
        else {
            int minW = (p->minW > 0 ? p->minW : 0);
            if(content < minW) content = minW;
        }

        outSlotSizes[i] = clamp_nonneg(content + axisExtra);
    }

    // Fix rounding to consume exactly availableAxis
    int sum = gaps;
    for(int i = 0; i < n; ++i) sum += outSlotSizes[i];
    int diff = availableAxis - sum;
    if(diff != 0 && n > 0) {
        // Adjust the last flexible pane's slot if possible; otherwise the last pane
        for(int i = n - 1; i >= 0; --i) {
            PXWindow* p = arr[i].p;
            if(p->fixedLong <= 0) {
                outSlotSizes[i] = clamp_nonneg(outSlotSizes[i] + diff);
                return;
            }
        }
        outSlotSizes[n - 1] = clamp_nonneg(outSlotSizes[n - 1] + diff);
    }
}

// Apply a stack inside 'area' with margins/padding.
static void ApplyStackWithBox(HWND hParent, HDWP* phdwp, RECT area, BOOL vertical,
                              Paneref* arr, int n, int spacing)
{
    const UINT SWP_FLAGS = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;
    int axisLen = vertical ? (area.bottom - area.top) : (area.right - area.left);
    if(axisLen < 0) axisLen = 0;

    // Compute SLOT sizes (including margins/padding)
    int* slotSizes = (int*)HeapAlloc(GetProcessHeap(), 0, sizeof(int) * (n > 0 ? n : 1));
    if(!slotSizes) return;

    DistributeAxisWithBox(arr, n, axisLen, spacing, vertical, slotSizes);

    // Cursor along axis
    int cursor = vertical ? area.top : area.left;

    for(int i = 0; i < n; ++i) {
        PXWindow* p = arr[i].p;

        // Effective spacing for this pane (allow per-pane override)
        int sp = (p->spacing > 0 ? p->spacing : spacing);

        // Margins/padding
        int mL = p->marginL, mT = p->marginT, mR = p->marginR, mB = p->marginB;
        int padL = p->padL, padT = p->padT, padR = p->padR, padB = p->padB;

        // SLOT rect (includes margins and padding)
        int x = area.left, y = area.top, w = area.right - area.left, h = area.bottom - area.top;
        if(vertical) {
            y = cursor;
            h = slotSizes[i];
        }
        else {
            x = cursor;
            w = slotSizes[i];
        }

        // Apply margins to SLOT (outside gaps)
        x += mL; y += mT; w -= (mL + mR); h -= (mT + mB);
        w = clamp_nonneg(w); h = clamp_nonneg(h);

        // Apply padding (inner inset into the rect we pass to child)
        x += padL; y += padT; w -= (padL + padR); h -= (padT + padB);
        w = clamp_nonneg(w); h = clamp_nonneg(h);

        // Final min constraints (ensure the rect is not smaller than minW/minH)
        if(p->minW > 0 && w < p->minW) w = p->minW;
        if(p->minH > 0 && h < p->minH) h = p->minH;

        if(*phdwp) {
            *phdwp = DeferWindowPos(*phdwp, arr[i].h, NULL, x, y, w, h, SWP_FLAGS);
        }
        else {
            MoveWindow(arr[i].h, x, y, w, h, TRUE);
        }

        // Advance cursor by SLOT size + spacing (spacing is outside panes)
        cursor += slotSizes[i] + (i + 1 < n ? sp : 0);
    }

    HeapFree(GetProcessHeap(), 0, slotSizes);
}

// Inset a rect by BoxInsets (band-level padding)
static RECT InsetRectBy(RECT r, BoxInsets in)
{
    r.left += in.l;
    r.top += in.t;
    r.right -= in.r;
    r.bottom -= in.b;
    if(r.right < r.left)   r.right = r.left;
    if(r.bottom < r.top)   r.bottom = r.top;
    return r;
}

// -----------------------------
// Main layout (multi-pass)
// -----------------------------
void LayoutDockedEx(PXWindow PXREF pxWindow, const DockLayoutConfig* pCfg)
{
    if(!(pxWindow && pCfg))
    {
        return;
    }

    DockLayoutConfig cfg = pCfg ? *pCfg : DockDefaultConfig();

    RECT rc = PXWindowRectOf(pxWindow->WindowHandle);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;

    int total = CountVisiblePanes(pxWindow->WindowHandle);
    if(total <= 0) return;

    HDWP hdwp = BeginDeferWindowPos(total);
    const int spacingDefault = cfg.defaultSpacing;

    Paneref* buf = (Paneref*)HeapAlloc(GetProcessHeap(), 0, sizeof(Paneref) * (total > 0 ? total : 1));
    if(!buf) 
    {
        if(hdwp)
            EndDeferWindowPos(hdwp); return; }

    // -----------------------------
    // PASS 1: LEFT band(s)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->WindowHandle, PXWindowDockSideLeft, buf, total);
        if(n > 0) {
            int bandW = cfg.defaultBandW;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefW > bandW) bandW = buf[i].p->prefW;

            RECT band = (RECT){ leftX, topY, leftX + bandW, bottomY };
            // Band-level padding:
            band = InsetRectBy(band, cfg.padLeft);

            leftX += bandW; // consume thickness regardless of inner padding

            ApplyStackWithBox(pxWindow->WindowHandle, &hdwp, band, TRUE /* vertical stack */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 1: RIGHT band(s)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->WindowHandle, PXWindowDockSideRight, buf, total);
        if(n > 0) {
            int bandW = cfg.defaultBandW;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefW > bandW) bandW = buf[i].p->prefW;

            RECT band = (RECT){ rightX - bandW, topY, rightX, bottomY };
            band = InsetRectBy(band, cfg.padRight);

            rightX -= bandW;

            ApplyStackWithBox(pxWindow->WindowHandle, &hdwp, band, TRUE /* vertical stack */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 2: TOP band(s) — use remaining width
    // -----------------------------
    {
        int n = CollectSide(pxWindow->WindowHandle, PXWindowDockSideTop, buf, total);
        if(n > 0) {
            int bandH = cfg.defaultBandH;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefH > bandH) bandH = buf[i].p->prefH;

            RECT band = (RECT){ leftX, topY, rightX, topY + bandH };
            band = InsetRectBy(band, cfg.padTop);

            topY += bandH;

            ApplyStackWithBox(pxWindow->WindowHandle, &hdwp, band, FALSE /* horizontal row */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 2: BOTTOM band(s)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->WindowHandle, PXWindowDockSideBottom, buf, total);
        if(n > 0) {
            int bandH = cfg.defaultBandH;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefH > bandH) bandH = buf[i].p->prefH;

            RECT band = (RECT){ leftX, bottomY - bandH, rightX, bottomY };
            band = InsetRectBy(band, cfg.padBottom);

            bottomY -= bandH;

            ApplyStackWithBox(pxWindow->WindowHandle, &hdwp, band, FALSE /* horizontal row */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 3: CENTER / FILL (DOCK_NONE)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->WindowHandle, PXWindowDockSideNone, buf, total);
        RECT remain = (RECT){ leftX, topY, rightX, bottomY };
        remain = InsetRectBy(remain, cfg.padCenter);

        if(n > 0) {
            ApplyStackWithBox(pxWindow->WindowHandle, &hdwp, remain, cfg.centerVertical /* orientation */, buf, n, spacingDefault);
        }
    }

    HeapFree(GetProcessHeap(), 0, buf);
    if(hdwp)
        EndDeferWindowPos(hdwp);

    PXWindowEventPoll(pxWindow);

   // InvalidateRect(pxWindow->WindowHandle, 0, 0);
   // UpdateWindow(pxWindow->WindowHandle);
   // ShowWindow(pxWindow->WindowHandle, SW_MINIMIZE);
   // ShowWindow(pxWindow->WindowHandle, SW_SHOW);
}

BOOL PXAPI PXWindowScreenPtInMainClient(PXWindow PXREF pxWindow, PXVector2I32S PXREF pxVector2I32S)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    if(!pxWindowRoot)
        return FALSE;

    RECT rc; GetClientRect(pxWindowRoot->WindowHandle, &rc);

    POINT tl = { rc.left, rc.top },
        br = { rc.right, rc.bottom };

    ClientToScreen(pxWindowRoot->WindowHandle, &tl);
    ClientToScreen(pxWindowRoot->WindowHandle, &br);

    RECT scr = { tl.x, tl.y, br.x, br.y };

    POINT point = *(POINT*)pxVector2I32S;

    return PtInRect(&scr, point);
}

PXWindowDockSide PXAPI ChooseDockSide(PXWindow PXREF pxWindow, PXVector2I32S PXREF pxVector2I32S)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    RECT rectangle; 
    GetClientRect(pxWindowRoot->WindowHandle, &rectangle);
    POINT topLeft = { rectangle.left, rectangle.top };
    POINT botomRight = { rectangle.right, rectangle.bottom };

    ClientToScreen(pxWindowRoot->WindowHandle, &topLeft);
    ClientToScreen(pxWindowRoot->WindowHandle, &botomRight);

#if 0

    int marginX = (botomRight.x - topLefe.x) / 4;
    int marginY = (botomRight.y - topLefe.y) / 4;

    int limitDockSideTop = topLefe.y + marginY;
    int limitDockSideBottom = botomRight.y - marginY;
    int limitDockSideLeft = topLefe.x + marginX;
    int limitDockSideRight =  botomRight.x - marginX;

    PXBool dockSideTop = (pxVector2I32S->Y < limitDockSideTop);
    PXBool dockSideBottom = (pxVector2I32S->Y > limitDockSideBottom);
    PXBool dockSideLeft = (pxVector2I32S->X < limitDockSideLeft);
    PXBool dockSideRight = (pxVector2I32S->X > limitDockSideRight);
#else

    int marginX = (topLeft.x + botomRight.x) / 2;
    int marginY = (topLeft.y + botomRight.y) / 2;

    if(pxVector2I32S->Y < marginY)
        return PXWindowDockSideTop;

    int deadOffsetY = marginX / 2;
    int deadOffsetX = marginY / 2;

    int limitDockSideTop = deadOffsetY;
    int limitDockSideBottom = deadOffsetX;
    int limitDockSideLeft = marginY + deadOffsetY;
    int limitDockSideRight = marginX + deadOffsetX;


    PXBool dockSideTop = (pxVector2I32S->Y < limitDockSideTop);
    PXBool dockSideLeft = (pxVector2I32S->X < limitDockSideBottom);
    PXBool dockSideBottom = (pxVector2I32S->Y > limitDockSideLeft);
    PXBool dockSideRight = (pxVector2I32S->X > limitDockSideRight);



#endif


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Dock-PX",
        "\n"
        "%20s : %i, %i, M:%i,%i\n"
        "%20s : %i, %i < %i\n"
        "%20s : %i, %i > %i\n"
        "%20s : %i, %i < %i\n"
        "%20s : %i, %i > %i",
        "Position", pxVector2I32S->X, pxVector2I32S->Y, marginX, marginY,
        "Top", dockSideTop, pxVector2I32S->Y, limitDockSideTop,
        "Bottom", dockSideBottom, pxVector2I32S->Y, limitDockSideBottom,
        "Left", dockSideLeft, pxVector2I32S->X, limitDockSideLeft,
        "Right", dockSideRight, pxVector2I32S->X, limitDockSideRight
    );
#endif

    if(dockSideBottom)
        return PXWindowDockSideBottom;

    if(dockSideRight)
        return PXWindowDockSideRight;

    if(dockSideLeft)
        return PXWindowDockSideLeft;

    if(dockSideTop)
        return PXWindowDockSideTop;

    return PXWindowDockSideNone;
}

/* ---- Overlay helpers --------------------------------------------------- */

RECT PXAPI OverlayRectForSide(PXWindow PXREF pxWindow, PXWindowDockSide side)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    RECT m = PXWindowRectOf(pxWindowRoot->WindowHandle);
    POINT tl = { m.left, m.top }, br = { m.right, m.bottom };
    // Convert to screen for DockSide calc, but overlay is drawn in client coords.
    // The rect we return is in MAIN CLIENT COORDS (used in PaintMain).

    RECT r = { 0 };

    SubSegment(&m, &r, side);

    return r;
}

void PXAPI PXWindowShowDockOverlay(PXWindow PXREF pxWindow, PXWindowDockSide side)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    if(!pxWindowRoot)
        return;

    gOverlay.active = TRUE;
    gOverlay.side = side;
    gOverlay.rect = OverlayRectForSide(pxWindow, side);
    InvalidateRect(pxWindowRoot->WindowHandle, NULL, FALSE);
}

void PXAPI PXWindowHideDockOverlay(PXWindow PXREF pxWindow)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    if(!pxWindowRoot)
        return;

    if(gOverlay.active) {
        gOverlay.active = FALSE;
        InvalidateRect(pxWindowRoot->WindowHandle, NULL, FALSE);
    }
}

void PXAPI PXWindowTearOffToFloating(PXWindow* pxWindow, POINT startScreen)
{
    if(pxWindow->floating)
        return;

    PXBool reactGL = pxWindow->RenderContext > 0;

    PXWindowDestroy(pxWindow);

    PXWindowCreateInfo pxWindowCreateInfo;
    PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
    pxWindowCreateInfo.floating = TRUE;
    pxWindowCreateInfo.dockSide = PXWindowDockSideNone;
    pxWindowCreateInfo.WindowParent = PXNull;
   //pxWindowCreateInfo.isChild = FALSE;

    PXWindowCreate(&pxWindow, &pxWindowCreateInfo);

    if(reactGL)
    {
        PXWindowOpenGLEnable(pxWindow);
    }

    RECT wr; GetWindowRect(pxWindow->WindowHandle, &wr);
    int w = wr.right - wr.left, h = wr.bottom - wr.top;
    MoveWindow(pxWindow->WindowHandle, startScreen.x - w / 2, startScreen.y - pxWindow->headerH / 2, w, h, TRUE);
    ShowWindow(pxWindow->WindowHandle, SW_SHOWNORMAL);

    ReleaseCapture();
    SendMessageW(pxWindow->WindowHandle, WM_SYSCOMMAND, SC_MOVE | 0x0002, 0); // start system drag
}

void PXAPI PXWindowDockBackToChild(PXWindow* pxWindow, PXWindowDockSide side)
{
    if(!pxWindow->floating) 
        return;

    PXBool reactGL = pxWindow->RenderContext > 0;

    PXWindowDestroy(pxWindow);

    PXWindowCreateInfo pxWindowCreateInfo;
    PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
    pxWindowCreateInfo.WindowParent = pxWindow->WindowParent;
    pxWindowCreateInfo.floating = FALSE;
    pxWindowCreateInfo.dockSide = side;
    //pxWindowCreateInfo.isChild = PXTrue;

    PXWindowCreate(&pxWindow, &pxWindowCreateInfo);

    if(reactGL)
    {
        PXWindowOpenGLEnable(pxWindow);
    }

    LayoutDockedEx(pxWindow, NULL);
}

/* ---- Painting ---------------------------------------------------------- */

void PXAPI PXWindowDrawAlphaRect(HDC hdc, RECT rc, COLORREF color, BYTE alpha) {
    int w = rc.right - rc.left, h = rc.bottom - rc.top;
    if(w <= 0 || h <= 0) return;

    HDC mem = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, w, h);
    HGDIOBJ oldBmp = SelectObject(mem, bmp);

    HBRUSH br = CreateSolidBrush(color);
    RECT fill = { 0,0,w,h };
    FillRect(mem, &fill, br);
    DeleteObject(br);

    BLENDFUNCTION bf = { AC_SRC_OVER, 0, alpha, 0 };
    AlphaBlend(hdc, rc.left, rc.top, w, h, mem, 0, 0, w, h, bf);

    SelectObject(mem, oldBmp);
    DeleteObject(bmp);
    DeleteDC(mem);
}

void PXAPI PXWindowDrawOverlay(HDC hdc, const DockOverlay* ov)
{
    if(!ov->active) return;

    RECT rScreen = ov->rect;
    // Adorner_ShowAt(&gAdorner, rScreen);

    // return;

     // Translucent aqua block
    PXWindowDrawAlphaRect(hdc, ov->rect, RGB(80, 170, 200), 120);
    //Rectangle(hdc, ov->rect.left, ov->rect.top, ov->rect.right, ov->rect.bottom);

    // Triangle arrow pointing into the block
    POINT tri[3];
    const int m = 16; // arrow size/padding
    switch(ov->side) {
        case PXWindowDockSideLeft:
            tri[0].x = ov->rect.right - m; tri[0].y = (ov->rect.top + ov->rect.bottom) / 2;
            tri[1].x = ov->rect.right - m - 18; tri[1].y = tri[0].y - 12;
            tri[2].x = ov->rect.right - m - 18; tri[2].y = tri[0].y + 12;
            break;
        case PXWindowDockSideRight:
            tri[0].x = ov->rect.left + m; tri[0].y = (ov->rect.top + ov->rect.bottom) / 2;
            tri[1].x = ov->rect.left + m + 18; tri[1].y = tri[0].y - 12;
            tri[2].x = ov->rect.left + m + 18; tri[2].y = tri[0].y + 12;
            break;
        case PXWindowDockSideTop:
            tri[0].x = (ov->rect.left + ov->rect.right) / 2; tri[0].y = ov->rect.bottom - m;
            tri[1].x = tri[0].x - 12; tri[1].y = ov->rect.bottom - m - 18;
            tri[2].x = tri[0].x + 12; tri[2].y = ov->rect.bottom - m - 18;
            break;
        case PXWindowDockSideBottom:
            tri[0].x = (ov->rect.left + ov->rect.right) / 2; tri[0].y = ov->rect.top + m;
            tri[1].x = tri[0].x - 12; tri[1].y = ov->rect.top + m + 18;
            tri[2].x = tri[0].x + 12; tri[2].y = ov->rect.top + m + 18;
            break;

        default: return;
    }
    HBRUSH triBr = CreateSolidBrush(RGB(250, 250, 0));
    HGDIOBJ oldBr = SelectObject(hdc, triBr);
    HPEN triPen = CreatePen(PS_SOLID, 2, RGB(0xFF, 0, 0));
    HGDIOBJ oldPen = SelectObject(hdc, triPen);
    Polygon(hdc, tri, 3);
    SelectObject(hdc, oldPen); DeleteObject(triPen);
    SelectObject(hdc, oldBr);  DeleteObject(triBr);

    // Border around the block
    HPEN pen = CreatePen(PS_DASH, 3, RGB(0, 120, 200));
    HGDIOBJ old = SelectObject(hdc, pen);
    // SetPolyFillMode(hdc, );
    // Rectangle(hdc, ov->rect.left, ov->rect.top, ov->rect.right, ov->rect.bottom);
    SelectObject(hdc, old);
    DeleteObject(pen);
}




// Call this once per frame to draw a spinning triangle.
// Requirements: a current OpenGL context (wglMakeCurrent),
// a valid viewport/projection, and double buffering enabled.
void PXAPI PXWindowDrawScene(void)
{
    // --- Timing (simple delta based on GetTickCount) ---
    static DWORD lastTick = 0;
    static float angleDeg = 0.0f; // spin angle in degrees
    DWORD now = GetTickCount();
    if(lastTick == 0) lastTick = now;
    float dt = (float)(now - lastTick) * 0.001f; // seconds
    lastTick = now;

    // Spin speed (degrees per second)
    const float spinSpeed = 90.0f; // 90°/sec
    angleDeg += spinSpeed * dt;
    if(angleDeg > 360.0f) angleDeg -= 360.0f;

    // --- Clear background ---
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark theme
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- Fixed-function state ---
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);     // optional; useful if you add 3D later
    glShadeModel(GL_SMOOTH);

    // --- Model transform: center & rotate ---
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // If you use a pixel-space ortho (glOrtho(0,w,0,h)), translate to center:
    // Change cx, cy to your desired center (e.g., window center).
    // Remove this translate if you're in NDC or a camera space already.
    float cx = 0.0f, cy = 0.0f; // center at origin for NDC/perspective setups
    glTranslatef(cx, cy, 0.0f);
    glRotatef(angleDeg, 0.0f, 0.0f, 1.0f); // rotate around Z for 2D spin

    // --- Triangle geometry (in local coordinates) ---
    // Size: radius ~0.5 in NDC or adjust for pixel-space
    const float r = 0.5f;
    // Equilateral triangle around origin
    const float x0 = 0.0f, y0 = r;                 // top
    const float x1 = -0.4330127f * r, y1 = -0.5f * r; // bottom-left  (-sqrt(3)/4, -1/4)
    const float x2 = 0.4330127f * r, y2 = -0.5f * r; // bottom-right  (+sqrt(3)/4, -1/4)

    // Colors (dark themed gradient-like via per-vertex colors)
    const float mb_r = 0.098f, mb_g = 0.098f, mb_b = 0.439f; // midnight blue
    const float cy_r = 0.0f, cy_g = 1.0f, cy_b = 1.0f;   // cyan
    const float gray_r = 0.6f, gray_g = 0.7f, gray_b = 0.8f; // accent

    glBegin(GL_TRIANGLES);
    glColor3f(mb_r, mb_g, mb_b); glVertex2f(x0, y0);
    glColor3f(cy_r, cy_g, cy_b); glVertex2f(x1, y1);
    glColor3f(gray_r, gray_g, gray_b); glVertex2f(x2, y2);
    glEnd();

    // If using immediate animation loop outside WM_PAINT, swap here.
    // Otherwise swap in your WM_PAINT handler after calling DrawScene().
    // SwapBuffers(hdc); // <-- call in your message loop/paint path
}


PXResult PXAPI PXWindowDrawGDI(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXColorRGBI8* color = &pxWindow->BackGroundColor;

    HWND windowHandle = pxWindow->WindowHandle;
    HDC hdc = pxWindow->DeviceContextHandle;

    RECT r;
    GetClientRect(windowHandle, &r);

    SetBkColor(hdc, RGB(32, 32, 32));
    PXWindowPaintPattern
    (
        windowHandle, 
        hdc,
        &r, 
        color
    );


    RECT hdr = r;
    hdr.bottom = hdr.top + pxWindow->headerH;

    PXColorRGBI8 pxColorRGBI8 = { 60, 60, 60 };
    PXWindowPaintPattern(windowHandle, hdc, &hdr, &pxColorRGBI8);

    SetBkMode(hdc, TRANSPARENT);
    //SetBkColor(hdc, RGB(32, 32, 32));
    SetTextColor(hdc, RGB(232, 232, 232));
    SetTextAlign(hdc, TA_LEFT);

    char buffer[128];
    int amount = sprintf_s(buffer, 128, "Cool window - 0x%8.8x", hdc);

    TextOutA(hdc, hdr.left + 5, hdr.top + 5, buffer, amount);
    SetBkMode(hdc, OPAQUE);

    HPEN pen = CreatePen(PS_SOLID, 3, RGB(75,75, 75));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, r.left, hdr.bottom, NULL);
    LineTo(hdc, r.right, hdr.bottom);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawGL(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXColorRGBI8* color = &pxWindow->BackGroundColor;

#if 0

    // glEnable(GL_DEPTH_TEST);
    glClearColor(0.06f, 0.07f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);
    // Bottom (midnight blue)
    glColor3f(0.098f, 0.098f, 0.439f);
    glVertex2f(-1, -1); // bottom-left
    glVertex2f(1, -1); // bottom-right

    // Top (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex2f(1, 1); // top-right
    glVertex2f(-1, 1); // top-left
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.8, -0.8);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.8, -0.8);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(0, 0.8);
    glEnd();

#else

    int w = pxWindowDrawInfo->RectangleXYWH.Width;
    int h = pxWindowDrawInfo->RectangleXYWH.Height;
    glViewport(0, 0, w, h);
    // 2D orthographic projection (pixel coordinates)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    // ----------------------------- 
    // // Background rectangle 
    // // -----------------------------
    glColor3ub(GetRValue(color->Red), GetGValue(color->Green), GetBValue(color->Blue));

    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(w, 0);
    glVertex2i(w, h);
    glVertex2i(0, h);
    glEnd();
    // ----------------------------- 
    // Header rectangle 
    // ----------------------------- 
    int headerH = pxWindow->headerH;
    glColor3ub(50, 50, 50);
    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(w, 0);
    glVertex2i(w, headerH);
    glVertex2i(0, headerH);
    glEnd();
    // -----------------------------
    // Text (using bitmap font lists)
    //----------------------------- 
    char buffer[128];
    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

    int amount = sprintf(buffer, "Cool window - 0x%8.8x", pxWindow->DeviceContextHandle);
    glColor3ub(232, 232, 232);
    glRasterPos2i(5, 5 + 12);
    // y + font height 

    //PXTextDraw(, &pxText);

    //for(int i = 0; i < amount; i++)
     //   glCallList(pxWindow->fontBase + (unsigned char)buffer[i]);

    // ----------------------------- 
    // Separator line 
    // ----------------------------- 
    glColor3ub(64, 64, 64);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2i(0, headerH);
    glVertex2i(w, headerH);
    glEnd();
#endif

    return PXResultOK;
}

void PXAPI PXWindowPaintPane(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = PXGUIThemeGet();
    const HDC hdc = PXWindowDCGet(pxWindow);

    PXECSDrawFunction drawFunciton = PXNull;

    // PreRender
    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
            drawFunciton = PXWindowDrawGDI;

#if OSWindows
            RECT rect;
            GetClientRect(pxWindow->WindowHandle, &rect);
            FillRect(hdc, &rect, pxGUITheme->BrushDarkBackground->BrushHandle);
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            drawFunciton = PXWindowDrawGL;

            BOOL openglInstance = wglMakeCurrent(hdc, pxWindow->RenderContext);

            if(!openglInstance)
                return;

            const float r = 0x20 / (float)0xFF;
            const float g = 0x20 / (float)0xFF;
            const float b = 0x20 / (float)0xFF;

            const GLbitfield clearFlags = 
                GL_COLOR_BUFFER_BIT |
                GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT |
                GL_ACCUM_BUFFER_BIT;

            glClearColor(r, g, b, 1.0f);
            glClear(clearFlags); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho
            (
                pxWindowDrawInfo->RectangleXYWH.X,
                pxWindowDrawInfo->RectangleXYWH.X + pxWindowDrawInfo->RectangleXYWH.Width,
                pxWindowDrawInfo->RectangleXYWH.Y + pxWindowDrawInfo->RectangleXYWH.Height,
                pxWindowDrawInfo->RectangleXYWH.Y,
                -1.0f,
                +1.0f
            );

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            //glDrawBuffer(GL_BACK);
            //glEnableClientState(GL_COLOR_ARRAY);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            break;
        }
        default:
            return;
    }

    PXECSInfo* pxECSInfo = pxWindow->EventList.CallBackOwner;
    PXECSDrawFunction pxECSDrawFunction = pxWindow->EventList.CallBackDraw;

    if(pxECSDrawFunction && pxECSInfo)
    {
        drawFunciton = pxECSDrawFunction;

        PXResult pxResult = drawFunciton(pxECSInfo, pxWindowDrawInfo);
    }
    else
    {
        drawFunciton(pxWindow, pxWindowDrawInfo);
    }  

    // Post render update
    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
            GdiFlush();
            break;
        }
        case PXGraphicSystemOpenGL:
        {
            glFinish();

            SwapBuffers(hdc);

            GLenum err = glGetError();
            if(err != GL_NO_ERROR)
                printf("OpenGL error: %d\n", err);

            err = glGetError();
            if(err != GL_NO_ERROR)
                printf("OpenGL error: %d\n", err);


            BOOL openglInstance = wglMakeCurrent(hdc, PXNull);

            break;
        }
    }

#if 0
    if(pxWindow->isopengltest)
    {       
        DrawScene();
        return;
    }
#endif
}

void PXAPI PXWindowPaintPattern(HWND hwnd, HDC hdc, RECT* rect, PXColorRGBI8 PXREF color)
{
    COLORREF pattern = RGB(color->Red + 20, color->Green + 20, color->Blue + 20);
    COLORREF background = RGB(color->Red, color->Green, color->Blue);

    HBRUSH hatch = CreateHatchBrush(HS_DIAGCROSS, pattern); // cyan lines
    HBRUSH hatsolid = CreateSolidBrush(background);

    //  FillRect(hdc, &ps.rcPaint, hatsolid);

    HGDIOBJ oldBr = SelectObject(hdc, hatch);
    HGDIOBJ oldPen = SelectObject(hdc, GetStockObject(NULL_PEN));

    if(0)
    {
        SetBkColor(hdc, TRANSPARENT);
    }
    else
    {
        SetBkColor(hdc, background);    // dark background
    }

    SetTextColor(hdc, pattern); // hatch lines (cyan)

    Rectangle(hdc, rect->left, rect->top, rect->right, rect->bottom);

    DeleteObject(hatch);
    DeleteObject(hatsolid);
}

void PXAPI PXWindowPaintMain(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
            RECT rectangle;
            GetClientRect(pxWindow->WindowHandle, &rectangle);

            PXColorRGBI8 color = { 28 , 28 , 28 };

            PXWindowPaintPattern
            (
                pxWindow->WindowHandle,
                pxWindow->DeviceContextHandle,
                &rectangle,
                &color
            );

            // FillRect(hdc, &ps.rcPaint, hatch);

#if 0
            HBRUSH bg = CreateSolidBrush(RGB(40, 40, 60));
            FillRect(pxWindow->DeviceContextHandle, &rectangle, bg);
            DeleteObject(bg);
            // Draw overlay if active
            DrawOverlay(pxWindow->DeviceContextHandle, &gOverlay);
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            const float r = 0x20 / (float)0xFF;
            const float g = 0x20 / (float)0xFF;
            const float b = 0x20 / (float)0xFF;

            glClearColor(r, g, b, 1.0f);
            glClear(GL_ALL_ATTRIB_BITS);

            PXWindowBufferSwap(pxWindow);
            break;
        }
    }
}

#if OSWindows
LRESULT CALLBACK PXWindowEventCallBack(const HWND windowHandle, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();

    PXWindow* pxWindowParrent = PXNull;
    PXWindow* pxWindow = (PXWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowHandle = windowHandle;
    pxWindowEvent.EventID = eventID;
    pxWindowEvent.ParamW = wParam;
    pxWindowEvent.ParamL = lParam;
    pxWindowEvent.WindowSender = pxWindow;

#if PXLogEnable && 0
    int pxID = pxWindowEvent.WindowSender ? pxWindowEvent.WindowSender->Info.ID : -1;

    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Event",
        "PXID:<%4i>, Event:<%i>",
        pxID,
        eventID
    );
#endif

    switch(eventID)
    {
        default:
        case WM_NULL:
        {
            break;
        }
        case WM_CREATE: // Gets called inside the "CreateWindow" function.
        {
            // Do nothíng here, as it's too soon to regard the window as 'created'
            break;
        }
        case WM_NCCREATE: 
        {
            CREATESTRUCTA* cs = (CREATESTRUCTA*)lParam;
            pxWindow = (PXWindow*)cs->lpCreateParams;

            SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)pxWindow);

            pxWindow->WindowHandle = windowHandle;

            return TRUE; 
        }
        case WM_DESTROY:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementDestroy;

            PXWindowEventConsumer(&pxWindowEvent);

#if 0
            if(hwnd == gMain)
                PostQuitMessage(0);
            return 0;
#endif

            break;
        }
        case WM_MOVE:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementMove;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_SIZE:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementResize;
            pxWindowEvent.Resize.Width = LOWORD(lParam);
            pxWindowEvent.Resize.Height = HIWORD(lParam);

            switch(wParam)
            {
                case SIZE_MAXHIDE: // (4) Message is sent to all pop - up windows when some other window is maximized.
                    pxWindowEvent.Resize.Cause = PXWindowResizeCauseMAXHIDE;
                    break;
                
                case SIZE_MAXIMIZED: // (2), The window has been maximized.
                    pxWindowEvent.Resize.Cause = PXWindowResizeCauseMAXIMIZED;
                    break;                
                case SIZE_MAXSHOW: // (3), Message is sent to all pop - up windows when some other window has been restored to its former size.
                    pxWindowEvent.Resize.Cause = PXWindowResizeCauseMAXSHOW;
                    break;

                case SIZE_MINIMIZED: // (1) The window has been minimized.
                    pxWindowEvent.Resize.Cause = PXWindowResizeCauseMINIMIZED;
                    break;

                case SIZE_RESTORED: // (0) The window
                    pxWindowEvent.Resize.Cause = PXWindowResizeCauseRESTORED;
                    break;

                default:
                    pxWindowEvent.Resize.Cause = PXWindowResizeCauseUnkown;
                    break;
            }

      

            PXWindowEventConsumer(&pxWindowEvent);

            DockLayoutConfig cfg = DockDefaultConfig();
            LayoutDockedEx(pxWindow, &cfg);

            if(!pxWindow)
            {
                LayoutDockedEx(pxWindow, NULL);
                InvalidateRect(windowHandle, NULL, TRUE);
                UpdateWindow(windowHandle);
            }
            else
            {
                int w = LOWORD(lParam), h = HIWORD(lParam);
                if(h == 0) h = 1;

                if(pxWindow->RenderContext)
                {
                    wglMakeCurrent(pxWindow->DeviceContextHandle, pxWindow->RenderContext);
                    glViewport(0, 0, w, h);
                    //wglMakeCurrent(pxWindow->DeviceContextHandle, 0);
                }

                 //LayoutDockedEx(windowID, NULL);
                 //InvalidateRect(windowID, NULL, FALSE);
            }
            return 0;

            break;
        }
        case WM_CLOSE:
        {
            pxWindowEvent.Type = PXWindowEventTypeElementClose;
            pxWindowEvent.Close.CommitToClose = PXTrue;

            PXWindowEventConsumer(&pxWindowEvent);

            if(!pxWindowEvent.Close.CommitToClose)
            {
                return PXFalse;
            }

            break;
        }
        case WM_SETCURSOR:
        {
            const HWND windowsHandle = (HWND)wParam;
            const WORD hitTestResult = LOWORD(lParam);
            const WORD sourceMessage = HIWORD(lParam);
            const PXBool showCursor = PXTrue;//!(window->CursorModeCurrent == PXWindowCursorInvisible || window->CursorModeCurrent == PXWindowCursorLockAndHide);

            if(showCursor)
            {
                while(ShowCursor(1) < 0);
            }
            else
            {
                while(ShowCursor(0) >= 0);
            }

            return 1; // prevent further processing
        }
        case WM_WINDOWPOSCHANGING:
        {
            break;
        }
        case WM_WINDOWPOSCHANGED:
        {
            break;
        }
        case WM_ACTIVATE:
        {
            switch(wParam)
            {
                case WA_INACTIVE:
                {
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case WM_SYSCOMMAND:
        {
            switch(0xFFF0 & wParam)
            {
                case SC_MINIMIZE: // User requested minimize
                {

                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case WM_NOTIFY:
        {
            const HWND windowsHandle = (HWND)wParam;
            NMHDR PXREF notificationCode = (NMHDR*)lParam;
            HWND sourceObject = notificationCode->hwndFrom;

            switch(notificationCode->code)
            {
                case LVN_BEGINDRAG:
                {
                    //  if(!bDragging)
                    //  break;

                    // p.x = LOWORD(lParam);
                    //  p.y = HIWORD(lParam);

                    //ClientToScreen(hWndMain, &p);
                    // ImageList_DragMove(p.x, p.y);

                    break;
                }
                case TVN_SELCHANGED:
                {
                    /*
                    // NOTE:
                    // On treeview item selection, "wParam" will might always be NULL.
                    // It's a copy of the item "lParam" parameter on creation.

                    PXWindow* pxTreeViewContainer = PXNull;
                    PXWindow* pxTreeViewItem = PXNull;

                    if(pxNativDraw)
                    {
                        const HTREEITEM treeItemHandle = TreeView_GetSelection(sourceObject); // Event does not give us the handle, fetch manually.
                        const HWND itemHandle = (HWND)treeItemHandle;

                        // Fetch treeview object
                        PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &sourceObject, &pxTreeViewContainer);

                        // Fetch treeviewitem object
                        PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &itemHandle, &pxTreeViewItem);
                    }

                    pxWindowEvent.UIElementReference = pxTreeViewContainer;
                    pxWindowEvent.WindowSender = pxTreeViewItem;

                    pxWindowEvent.Type = PXWindowEventTypeElementSelect;
                    pxWindowEvent.Select.UIElementSelected = pxTreeViewItem;

                    PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);
                    */
                    break;
                }
                case TCN_SELCHANGE:
                {
                    /*

                    // Setup data

                    const int pageID = TabCtrl_GetCurSel(sourceObject);

#if 1
                    PXWindow* pxWindow = PXNull;

                    PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &sourceObject, &pxWindow);

                    PXWindowExtendedBehaviourTab PXREF pxWindowExtendedBehaviourTab = (PXWindowExtendedBehaviourTab*)pxWindow->ExtendedData;

                    // Hide current page
                    const PXBool isValidIndex = pxWindowExtendedBehaviourTab->TABPageAmount >= (pageID + 1);

                    BOOL success = 0;

                    for(PXSize i = 0; i < pxWindowExtendedBehaviourTab->TABPageAmount; i++)
                    {
                        PXWindow PXREF pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

                        PXGUIProperty pxWindowUpdateInfo;
                        PXClear(PXGUIProperty, &pxWindowUpdateInfo);
                        pxWindowUpdateInfo.Show = PXFalse;
                        //pxWindowUpdateInfo.WindowCurrent = pxWindowTABPage;
                        pxWindowUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXNativDrawWindowProperty(pxNativDraw, pxWindowTABPage, &pxWindowUpdateInfo, 1);
                    }

                    // Show new page
                    if(isValidIndex)
                    {
                        PXWindow PXREF pxWindowTABPageSelected = &pxWindowExtendedBehaviourTab->TABPageList[pageID];

                        PXLogPrint
                        (
                            PXLoggingInfo,
                            PXWindowTextText,
                            "Event",
                            "Tab Select <0x%p>, %15s ID:<%i>",
                            sourceObject,
                            "", //pxUIElementEEE->NameData,
                            pageID
                        );

                        PXGUIProperty pxWindowUpdateInfo;
                        PXClear(PXGUIProperty, &pxWindowUpdateInfo);
                        pxWindowUpdateInfo.Show = PXTrue;
                        //pxWindowUpdateInfo.WindowCurrent = pxWindowTABPageSelected;
                        pxWindowUpdateInfo.Property = PXUIElementPropertyVisibility;

                        PXNativDrawWindowProperty(pxNativDraw, pxWindowTABPageSelected, &pxWindowUpdateInfo, 1);
                    }

#endif
*/
                    break;
                }
                default:
                {
#if 0
                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "Windows",
                        "Event",
                        "Notify <0x%p>, ID:<%i>",
                        sourceObject,
                        notificationCode->code
                    );
#endif

                    break;
                }
            }

            break;
        }

        case WM_ERASEBKGND:
#if 1
            return TRUE; // Signal we handled erasing. 0 would mean we didn't
#else
            break;
#endif

        case WM_PAINT:
        {
            PXWindowDrawInfo pxWindowDrawInfo;
            PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);

            PAINTSTRUCT painStruct;
            HDC hdcInvalid = BeginPaint(windowHandle, &painStruct);
 
            PXWindowRectangleXYWH(pxWindow, &pxWindowDrawInfo.RectangleXYWH);
            pxWindow->Position.Form = pxWindowDrawInfo.RectangleXYWH;

#if 1
            if(pxWindow->WindowParent)
            {
                PXWindowPaintPane(pxWindow, &pxWindowDrawInfo);
            }
            else
            {
                PXWindowPaintMain(pxWindow, &pxWindowDrawInfo);
            }
#endif

            BOOL isOK = EndPaint(windowHandle, &painStruct);

            return 0; // Signal we processed it
        }
#if 0
        case WM_DRAWITEM:
        {
            const HWND identifier = (HWND)wParam;
            DRAWITEMSTRUCT PXREF drawItemInfo = (DRAWITEMSTRUCT*)lParam;

#if 0
            PXWindowDrawInfo pxWindowDrawInfo;
            PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);
            pxWindowDrawInfo.hwnd = drawItemInfo->hwndItem;
            pxWindowDrawInfo.hDC = drawItemInfo->hDC;

            PXRectangleLTRBI32ToXYWHI32((PXRectangleLTRBI32*)&drawItemInfo->rcItem, &pxWindowDrawInfo.RectangleXYWH);

            const PXBool isOK = PXWindowRender(&pxWindowDrawInfo);

            if(isOK)
            {
                return TRUE;
            }
#endif

            return 0;
        }
#endif

#if 0
        case WM_PRINTCLIENT:
        {
#if 0
            PXWindowDrawInfo pxWindowDrawInfo;
            PXClear(PXWindowDrawInfo, &pxWindowDrawInfo);
            pxWindowDrawInfo.hwnd = windowID;
            pxWindowDrawInfo.hDC = (HDC)wParam;
            //  pxWindowDrawInfo.rcDirty = &rc;
            pxWindowDrawInfo.bErase = TRUE;

            RECT rc;

            GetClientRect(windowID, &rc);

            const PXBool isOK = PXWindowRender(&pxWindowDrawInfo);

            if(isOK)
            {
                return TRUE;
            }
#endif

            return 0;
        }
#endif

        case WM_NCHITTEST:
        {
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);

            if(pxWindow && pxWindow->floating) {
                POINT s = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                POINT c = s; 
                ScreenToClient(windowHandle, &c);
                RECT rc; 
                GetClientRect(windowHandle, &rc);
                const int b = 6;
                if(c.y < b)
                    return HTTOP;
                if(c.y > rc.bottom - b) 
                    return HTBOTTOM;
                if(c.x < b) 
                    return HTLEFT;
                if(c.x > rc.right - b) 
                    return HTRIGHT;
                if(c.y < pxWindow->headerH)
                    return HTCAPTION;
               
                return HTCLIENT;
            }

            break;
        }
        case WM_MOVING:
        {
            PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);
#if 1
            if(pxWindow && pxWindow->floating && pxWindowRoot) {
                // lParam is LPRECT of proposed window rect during the drag
                RECT* pr = (RECT*)lParam;
                PXVector2I32S center;
                center.X = (pr->left + pr->right) / 2;
                center.Y = (pr->top + pr->bottom) / 2;

                if(PXWindowScreenPtInMainClient(pxWindow, &center)) 
                {
                    PXWindowDockSide side = ChooseDockSide(pxWindow, &center);
                   

#if PXLogEnable
                    const char* name = PXWindowDockSideToString(side);

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXWindowTextText,
                        "Event",
                        "PXID:<%4i>, DOC:%s",
                        pxWindow->Info.ID,
                        name
                    );
#endif

                    PXWindowShowDockOverlay(pxWindow, side);
                }
                else {
                    PXWindowHideDockOverlay(pxWindow);
                }
            }
#endif
            return 0;
        }
        case WM_ENTERSIZEMOVE:
            return 0;

        case WM_EXITSIZEMOVE:
        {
            if(pxWindow && pxWindow->floating)
            {
                PXWindowHideDockOverlay(pxWindow);
                RECT wr; 
                GetWindowRect(windowHandle, &wr);
               
                PXVector2I32S center;
                center.X = (wr.left + wr.right) / 2;
                center.Y = (wr.top + wr.bottom) / 2;

                if(PXWindowScreenPtInMainClient(pxWindow, &center)) 
                {
                    PXWindowDockSide side = ChooseDockSide(pxWindow, &center);
                    PXWindowDockBackToChild(pxWindow, side);
                }
            }

            return 0;
        }
        case WM_POWERBROADCAST:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "EVENT",
                "WM_POWERBROADCAST"
            );
#endif

            // Notifies applications that a power-management event has occurred.
            switch(wParam)
            {
                case PBT_APMQUERYSUSPEND:
                {
                    break;
                }
                //case PBTF_APMRESUMEFROMFAILURE:
                case PBT_APMQUERYSTANDBY: // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMQUERYSUSPENDFAILED:
                {
                    break;
                }
                case PBT_APMQUERYSTANDBYFAILED:  // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMSUSPEND: // System is suspending operation.
                {
                    break;
                }
                case PBT_APMSTANDBY:  // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMRESUMECRITICAL:
                {
                    break;
                }
                case PBT_APMRESUMESUSPEND: // Operation is resuming by user input from a low-power state.
                {
                    break;
                }
                case PBT_APMRESUMESTANDBY:  // NOTE: Documentation says this might not trigger
                {
                    break;
                }
                case PBT_APMBATTERYLOW:
                {
                    break;
                }
                case PBT_APMPOWERSTATUSCHANGE:  // Power status has changed.
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingWarning,
                        "Window",
                        "Power",
                        "Status changed!"
                    );
#endif

                    /*
                    pxWindowEvent.Type = PXWindowEventTypeElementResize;
                    pxWindowEvent.Resize.Width = LOWORD(lParam);
                    pxWindowEvent.Resize.Height = HIWORD(lParam);

                    PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);
                    */
                    break;
                }
                case PBT_APMOEMEVENT:
                {
                    break;
                }
                case PBT_APMRESUMEAUTOMATIC: // Operation is resuming automatically from a low-power state.
                {
                    break;
                }
                case PBT_POWERSETTINGCHANGE: // A power setting change event has been received. 
                {
                    POWERBROADCAST_SETTING PXREF powerBroadCastSetting = (POWERBROADCAST_SETTING*)lParam;

                    break;
                }
            }

            break;
        }
        case WM_DEVICECHANGE:
        {
            // Triggers in any case
            UINT eventID = (UINT)wParam;

            switch(eventID)
            {
                case DBT_DEVNODES_CHANGED: // A device has been added to or removed from the system.
                {
                    // We get no additional info??
                    break;
                }
                case DBT_QUERYCHANGECONFIG: // Permission is requested to change the current configuration(dock or undock).
                {
                    break;
                }
                case DBT_CONFIGCHANGED: // The current configuration has changed, due to a dock or undock.
                {
                    break;
                }
                case DBT_CONFIGCHANGECANCELED: // A request to change the current configuration(dock or undock) has been canceled.
                {
                    break;
                }
                case DBT_DEVICEARRIVAL: // 	A device or piece of media has been inserted and is now available.
                {
                    break;
                }
                case DBT_DEVICEQUERYREMOVE: // 	Permission is requested to remove a device or piece of media.Any application can deny this request and cancel the removal.
                {
                    break;
                }
                case DBT_DEVICEQUERYREMOVEFAILED: // A request to remove a device or piece of media has been canceled.
                {
                    break;
                }
                case DBT_DEVICEREMOVEPENDING: //A device or piece of media is about to be removed.Cannot be denied.
                {
                    break;
                }
                case DBT_DEVICEREMOVECOMPLETE: // A device or piece of media has been removed.
                {
                    break;
                }
                case DBT_DEVICETYPESPECIFIC: // A device - specific event has occurred.
                {
                    break;
                }
                case DBT_CUSTOMEVENT: // A custom event has occurred.
                {
                    break;
                }
                case DBT_USERDEFINED: // The meaning of this message is user - defined.
                {
                    break;
                }
                default:
                {
                    break;
                }
            }


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "Name:DEVICECHANGE, ID:%4.4x",
                eventID
            );
#endif

            break;
        }
        case WM_INPUT_DEVICE_CHANGE:
        {
            // WARNING! 
            // This event is only triggerd when we have registerd a raw device!

            const UINT eventID = (UINT)wParam;

            PXDeviceInputUpdateInfo pxDeviceInputUpdateInfo;
            PXClear(PXDeviceInputUpdateInfo, &pxDeviceInputUpdateInfo);
            pxDeviceInputUpdateInfo.DeviceHandle = (HANDLE)lParam;

            switch(eventID)
            {
                case GIDC_ARRIVAL:
                {
                    pxDeviceInputUpdateInfo.Type = PXDeviceInputUpdateTypeArrival;
                    break;
                }
                case GIDC_REMOVAL:
                {
                    pxDeviceInputUpdateInfo.Type = PXDeviceInputUpdateTypeRemoval;
                    break;
                }
                default:
                {
                    pxDeviceInputUpdateInfo.Type = PXDeviceInputUpdateTypeInvalid;
                    break;
                }
            }

            PXDeviceInputUpdate(&pxDeviceInputUpdateInfo);

            break;
        }
#if 0
        case WM_ACTIVATE:
            break;
        case WM_SETREDRAW:
            break;
        case WM_SETFOCUS:
            break;
        case WM_KILLFOCUS:
            break;
        case WM_ENABLE:
            break;
        case WM_SETTEXT:
            break;
        case WM_GETTEXT:
            break;
        case WM_GETTEXTLENGTH:
            break;

        case WM_QUERYENDSESSION:
            break;
        case WM_ENDSESSION:
            break;
        case WM_QUERYOPEN:
            break;
        case WM_QUIT:
            break;
        case WM_ERASEBKGND:
            break;
        case WM_SYSCOLORCHANGE:
            break;
        case WM_SHOWWINDOW:
            break;
        case WM_WININICHANGE:
            break;
            //  case WM_SETTINGCHANGE:
            //     break;
        case WM_DEVMODECHANGE:
            return WindowEventDeviceModeChange;
        case WM_ACTIVATEAPP:
            return WindowEventActivateApp;
        case WM_FONTCHANGE:
            return WindowEventFontChange;
        case WM_TIMECHANGE:
            return WindowEventTimeChange;
        case WM_CANCELMODE:
            return WindowEventCancelMode;
        case WM_MOUSEACTIVATE:
            return WindowEventMouseActivate;
        case WM_CHILDACTIVATE:
            return WindowEventChildActivate;
        case WM_QUEUESYNC:
            return WindowEventQueueSync;
        case WM_GETMINMAXINFO:
            return WindowEventSizeChange;
        case WM_PAINTICON:
            return WindowEventIconPaint;
        case WM_ICONERASEBKGND:
            return WindowEventIconBackgroundErase;
        case WM_NEXTDLGCTL:
            return WindowEventDialogControlNext;
        case WM_SPOOLERSTATUS:
            return WindowEventSPOOLERSTATUS;
        case WM_DRAWITEM:
            return WindowEventItemDraw;
        case WM_MEASUREITEM:
            return WindowEventItemMeasure;
        case WM_DELETEITEM:
            return WindowEventItemDelete;
        case WM_VKEYTOITEM:
            return WindowEventVKEYTOITEM;
        case WM_CHARTOITEM:
            return WindowEventCHARTOITEM;
        case WM_SETFONT:
            return WindowEventFontSet;
        case WM_GETFONT:
            return WindowEventFontGet;
        case WM_SETHOTKEY:
            return WindowEventSETHOTKEY;
        case WM_GETHOTKEY:
            return WindowEventGETHOTKEY;
        case WM_QUERYDRAGICON:
            return WindowEventQUERYDRAGICON;
        case WM_COMPAREITEM:
            return WindowEventCOMPAREITEM;
        case WM_GETOBJECT:
            return WindowEventGETOBJECT;
        case WM_COMPACTING:
            return WindowEventCOMPACTING;
        case WM_COMMNOTIFY:
            return WindowEventCOMMNOTIFY;   /* no longer suported */
        case WM_WINDOWPOSCHANGING:
            return WindowEventWINDOWPOSCHANGING;
        case WM_WINDOWPOSCHANGED:
            return WindowEventWINDOWPOSCHANGED;
        case WM_POWER:
            return WindowEventPOWER;
        case WM_COPYDATA:
            return WindowEventCOPYDATA;
        case WM_CANCELJOURNAL:
            return WindowEventCANCELJOURNAL;

        case WM_INPUTLANGCHANGEREQUEST:
            return WindowEventINPUTLANGCHANGEREQUEST;
        case WM_INPUTLANGCHANGE:
            return WindowEventINPUTLANGCHANGE;
        case WM_TCARD:
            return WindowEventTCARD;
        case WM_HELP:
            return WindowEventHELP;
        case WM_USERCHANGED:
            return WindowEventUSERCHANGED;
        case WM_NOTIFYFORMAT:
            return WindowEventNOTIFYFORMAT;
        case WM_CONTEXTMENU:
            return WindowEventCONTEXTMENU;
        case WM_STYLECHANGING:
            return WindowEventSTYLECHANGING;
        case WM_STYLECHANGED:
            return WindowEventSTYLECHANGED;
        case WM_DISPLAYCHANGE:
            return WindowEventDISPLAYCHANGE;
        case WM_GETICON:
            return WindowEventGETICON;
        case WM_SETICON:
            return WindowEventSETICON;
        case WM_NCCREATE:
            return WindowEventNCCREATE;
        case WM_NCDESTROY:
            return WindowEventNCDESTROY;
        case WM_NCCALCSIZE:
            return WindowEventNCCALCSIZE;
        case WM_NCHITTEST:
            return WindowEventNCHITTEST;
        case WM_NCPAINT:
            return WindowEventNCPAINT;
        case WM_NCACTIVATE:
            return WindowEventNCACTIVATE;
        case WM_GETDLGCODE:
            return WindowEventGETDLGCODE;
        case WM_SYNCPAINT:
            return WindowEventSYNCPAINT;
        case WM_NCMOUSEMOVE:
            return WindowEventNCMOUSEMOVE;
        case WM_NCLBUTTONDOWN:
            return WindowEventNCLBUTTONDOWN;
        case WM_NCLBUTTONUP:
            return WindowEventNCLBUTTONUP;
        case WM_NCLBUTTONDBLCLK:
            return WindowEventNCLBUTTONDBLCLK;
        case WM_NCRBUTTONDOWN:
            return WindowEventNCRBUTTONDOWN;
        case WM_NCRBUTTONUP:
            return WindowEventNCRBUTTONUP;
        case WM_NCRBUTTONDBLCLK:
            return WindowEventNCRBUTTONDBLCLK;
        case WM_NCMBUTTONDOWN:
            return WindowEventNCMBUTTONDOWN;
        case WM_NCMBUTTONUP:
            return WindowEventNCMBUTTONUP;
        case WM_NCMBUTTONDBLCLK:
            return WindowEventNCMBUTTONDBLCLK;
        case WM_NCXBUTTONDOWN:
            return WindowEventNCXBUTTONDOWN;
        case WM_NCXBUTTONUP:
            return WindowEventNCXBUTTONUP;
        case WM_NCXBUTTONDBLCLK:
            return WindowEventNCXBUTTONDBLCLK;
        case WM_INPUT_DEVICE_CHANGE:
            return WindowEventINPUT_DEVICE_CHANGE;
            // ?? case WM_KEYFIRST: return WindowEventKEYFIRST;

        case WM_CHAR:
            return WindowEventCHAR;
        case WM_DEADCHAR:
            return WindowEventDEADCHAR;
        case WM_SYSKEYDOWN:
            return WindowEventSYSKEYDOWN;
        case WM_SYSKEYUP:
            return WindowEventSYSKEYUP;
        case WM_SYSCHAR:
            return WindowEventSYSCHAR;
        case WM_SYSDEADCHAR:
            return WindowEventSYSDEADCHAR;
            // case WM_UNICHAR: return WindowEventUNICHAR;
            //case WM_KEYLAST: return WindowEventKEYLAST;
            // case UNICODE_NOCHAR: return WindowEventXXXXXXXXXXXXXXX;

        case WM_IME_STARTCOMPOSITION:
            return WindowEventIME_STARTCOMPOSITION;
        case WM_IME_ENDCOMPOSITION:
            return WindowEventIME_ENDCOMPOSITION;
        case WM_IME_COMPOSITION:
            return WindowEventIME_COMPOSITION;
            //case WM_IME_KEYLAST: return WindowEventIME_KEYLAST;
        case WM_INITDIALOG:
            return WindowEventINITDIALOG;


        case WM_TIMER:
            return WindowEventTIMER;
        case WM_HSCROLL:
            return WindowEventHSCROLL;
        case WM_VSCROLL:
            return WindowEventVSCROLL;
        case WM_INITMENU:
            return WindowEventINITMENU;
        case WM_INITMENUPOPUP:
            return WindowEventINITMENUPOPUP;
            //  case WM_GESTURE: return WindowEventGESTURE;
            //   case WM_GESTURENOTIFY: return WindowEventGESTURENOTIFY;
        case WM_MENUSELECT:
            return WindowEventMENUSELECT;
        case WM_MENUCHAR:
            return WindowEventMENUCHAR;
        case WM_ENTERIDLE:
            return WindowEventENTERIDLE;
        case WM_MENURBUTTONUP:
            return WindowEventMENURBUTTONUP;
        case WM_MENUDRAG:
            return WindowEventMENUDRAG;
        case WM_MENUGETOBJECT:
            return WindowEventMENUGETOBJECT;
        case WM_UNINITMENUPOPUP:
            return WindowEventUNINITMENUPOPUP;
        case WM_MENUCOMMAND:
            return WindowEventMENUCOMMAND;
        case WM_CHANGEUISTATE:
            return WindowEventCHANGEUISTATE;
        case WM_UPDATEUISTATE:
            return WindowEventUPDATEUISTATE;
        case WM_QUERYUISTATE:
            return WindowEventQUERYUISTATE;
        case WM_CTLCOLORMSGBOX:
            return WindowEventCTLCOLORMSGBOX;
        case WM_CTLCOLOREDIT:
            return WindowEventCTLCOLOREDIT;
        case WM_CTLCOLORLISTBOX:
            return WindowEventCTLCOLORLISTBOX;
        case WM_CTLCOLORBTN:
            return WindowEventCTLCOLORBTN;
        case WM_CTLCOLORDLG:
            return WindowEventCTLCOLORDLG;
        case WM_CTLCOLORSCROLLBAR:
            return WindowEventCTLCOLORSCROLLBAR;

        case WM_CTLCOLORSTATIC:
            return WindowEventCTLCOLORSTATIC;
            //  case MN_GETHMENU: return WindowEventGETHMENU;
            //case WM_MOUSEFIRST: return WindowEventMOUSEFIRST;


#endif
        case WM_MOUSEMOVE:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
            pxWindowEvent.InputMouseMove.Position.X = GET_X_LPARAM(lParam); // Windows 2000, windowsx.h 
            pxWindowEvent.InputMouseMove.Position.Y = GET_Y_LPARAM(lParam); // Windows 2000, windowsx.h 

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLORDLG:
        case WM_CTLCOLORSCROLLBAR:
        case WM_CTLCOLORSTATIC:
        {
            // HWND windowFocusedHandle = (HWND)GetFocus();;
            HWND windowHandleNow = (HWND)lParam;
            HDC hdc = (HDC)wParam;

#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                "Window",
                "Event",
                "Color Edit (0x%p) -> (0x%p)",
                windowID,
                windowHandleNow
            );
#endif
            HBRUSH brushHandle = NULL;

            if(pxGUITheme->BrushMainPrimary && 0)
            {
                brushHandle = pxGUITheme->BrushDarkBackground->BrushHandle;
            }
            else
            {
                brushHandle = (HBRUSH)GetStockObject(WHITE_BRUSH);
            }        

            return (LRESULT)brushHandle;
        }
        case WM_INPUT:
        {
            const HRAWINPUT rawInput = (HRAWINPUT)lParam;

#if 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWindowTextText,
                "Event-Input",
                "PXID:%4i, Event:<%i>",
                pxWindow->Info.ID,
                eventID
            );
#endif

#if 1
            UINT size = 0;
            GetRawInputData
            (
                rawInput, 
                RID_INPUT,
                NULL,
                &size,
                sizeof(RAWINPUTHEADER)
            );

            PXBufferEnsure(&pxWindow->InputBuffer, size);

            GetRawInputData
            (
                rawInput,
                RID_INPUT,
                pxWindow->InputBuffer.Adress,
                &size,
                sizeof(RAWINPUTHEADER)
            );

            RAWINPUT* raw = (RAWINPUT*)pxWindow->InputBuffer.Adress;

            switch(raw->header.dwType)
            {
                case RIM_TYPEKEYBOARD:
                {
                    // raw->data.keyboard.Flags == RI_KEY_MAKE)

                    UINT wVirtKey = raw->data.keyboard.VKey;
                    UINT wScanCode = raw->data.keyboard.MakeCode;

                    pxWindowEvent.Type = PXWindowEventTypeInputKeyboard;
                    pxWindowEvent.InputKeyboard.VirtualKey = wVirtKey;
             
                    PXBool released = (raw->data.keyboard.Flags & RI_KEY_BREAK) != 0;

                    if(released)
                    {
                        pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateDown;
                    }
                    else
                    {
                        pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateUp;
                    }

                    // Used to reduce some wierd key interactions?
                    if(raw->data.keyboard.Flags & RI_KEY_E0) 
                        wScanCode |= 0xE000;

                    BYTE keyboardState[256]; // Documentation demands 256.
                    BOOL succ = GetKeyboardState(keyboardState);

                    HKL keyBoardLayoutNEW = GetKeyboardLayout(0);
                    PXBool isDifferent = _keyBoardLayout != keyBoardLayoutNEW;

                    // Never is different??
                    if(isDifferent)
                    {
                        _keyBoardLayout = keyBoardLayoutNEW;

#if 0
                        GetKeyboardLayoutNameW(_keyBoardLayoutName);

                        PXLogPrint
                        (
                            PXLoggingInfo,
                            PXWindowTextText,
                            "Event",
                            "Keyboardlayout changed to <%ls>",
                            _keyBoardLayoutName
                        );
#endif
                    }

                    // ToDo: why size of 8?
                    WCHAR buffer[8];
                    int count = ToUnicodeEx
                    (
                        wVirtKey,
                        wScanCode,
                        keyboardState,
                        buffer, 
                        8,
                        0, 
                        _keyBoardLayout
                    );

                    pxWindowEvent.InputKeyboard.CharacterW = buffer[0];

                    break;
                }
                case RIM_TYPEMOUSE:
                {
                    pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
                    pxWindowEvent.InputMouseMove.Axis.X = 0;
                    pxWindowEvent.InputMouseMove.Axis.Y = 0;
                    pxWindowEvent.InputMouseMove.Delta.X = raw->data.mouse.lLastX;
                    pxWindowEvent.InputMouseMove.Delta.Y = raw->data.mouse.lLastY;
#if 0
                    PXWindow* root = PXWindowRootGet(pxWindow);

                    PXWindowCursorPositionGet
                    (
                        root,
                        &pxWindowEvent.InputMouseMove.Position
                    );
#endif
                    break;
                }
                default:
                {
                    break;
                }
            }

            PXWindowEventConsumer(&pxWindowEvent);

#else
            // MISSING
            const PXSize inputCode = GET_RAWINPUT_CODE_WPARAM(wParam);
         
            const UINT uiCommand = RID_INPUT; // RID_HEADER

            switch(inputCode)
            {
                case RIM_INPUT: // Input occurred while the application was in the foreground.
                {
                    RAWINPUT rawInput;
                    UINT rawInputSize = sizeof(RAWINPUT); // Can't be 'const' !

#if WindowsAtleastXP
                    const UINT result = GetRawInputData // Windows XP, User32.dll, winuser.h
                    (
                        handle,
                        uiCommand,
                        &rawInput,
                        &rawInputSize,
                        sizeof(RAWINPUTHEADER)
                    );
                    const PXBool sucessful = result != -1;

                    if(sucessful)
                    {
#if UseRawMouseData
                        if(RIM_TYPEMOUSE == rawInput.header.dwType)
                        {
                            pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
                            pxWindowEvent.InputMouseMove.Axis.X = 0;
                            pxWindowEvent.InputMouseMove.Axis.Y = 0;
                            pxWindowEvent.InputMouseMove.Delta.X = rawInput.data.mouse.lLastX;
                            pxWindowEvent.InputMouseMove.Delta.Y = rawInput.data.mouse.lLastY;

                            PXWindowCursorPositionGet
                            (
                                pxWindowEvent.WindowSender,
                                &pxWindowEvent.InputMouseMove.Position
                            );

                            PXWindowEventConsumer(&pxWindowEvent);


                            //PXWindowTriggerOnMouseMoveEvent(window, positionX, positionY, deltaX, deltaY);



                            // Wheel data needs to be pointer casted to interpret an unsigned short as a short, with no conversion
                            // otherwise it'll overflow when going negative.
                            // Didn't happen before some minor changes in the code, doesn't seem to go away
                            // so it's going to have to be like this.
                            // if (raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
                            //     input.mouse.wheel = (*(short*)&raw->data.mouse.usButtonData) / WHEEL_DELTA;
                        }
#endif


                    }
#endif





                    break;
                }
                case RIM_INPUTSINK: // Input occurred while the application was not in the foreground.
                {
                    break;
                }

                default:
                    break;
            }
#endif
            break;
        }
        case WM_INPUTLANGCHANGE: // unrelayable!
        {
            _keyBoardLayout = (HKL)lParam; // new layout

            GetKeyboardLayoutNameW(_keyBoardLayoutName);

#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                PXWindowTextText,
                "Event",
                "Keyboardlayout changed to <%s>",
                _keyBoardLayoutName
            );
#endif

            break;
        }
        case WM_COMMAND:
        {
            const PXI32U buttonType = wParam; // IDOK;
            const HANDLE handle = (HANDLE)lParam;

            /*
            PXWindow PXREF pxWindow = PXNull;

            if(pxNativDraw)
            {
                PXDictionaryEntryFind(&pxNativDraw->ResourceManager->GUIElementLookup, &handle, &pxWindow);
            }

            if(!pxWindow) // if we did not find the object, stop
            {
                break;
            }

            if(!(pxWindow->Info.Behaviour & PXECSInfoActive))
            {
                // ShowWindow(pxWindow->ID, SW_HIDE);
            }

            pxWindowEvent.Type = PXWindowEventTypeElementClick;
            pxWindowEvent.WindowSender = pxWindow;

            PXNativDrawEventConsumer(pxNativDraw, &pxWindowEvent);
            */
            break;
        }
        case WM_LBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXWindowEventConsumer(&pxWindowEvent);

            if(pxWindow && !pxWindow->floating) {
                POINT c = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
                if(c.y < pxWindow->headerH) {
                    POINT s = c; ClientToScreen(windowHandle, &s);
                    PXWindowTearOffToFloating(pxWindow, s);
                    return 0;
                }
            }
            return 0;

           // break;
        }
        case WM_LBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_LBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonLeft;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_RBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_RBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_RBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonRight;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_MBUTTONDOWN:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDown;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_MBUTTONUP:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateUp;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_MBUTTONDBLCLK:
        {
            pxWindowEvent.Type = PXWindowEventTypeInputMouseButton;
            pxWindowEvent.InputMouseButton.Button = PXMouseButtonMiddle;
            pxWindowEvent.InputMouseButton.PressState = PXKeyPressStateDoubleClick;

            PXWindowEventConsumer(&pxWindowEvent);

            break;
        }
        case WM_MOUSEWHEEL:
        {

            break;
        }
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            const PXSize characterInfo = lParam;

            pxWindowEvent.Type = PXWindowEventTypeInputKeyboard;
            pxWindowEvent.InputKeyboard.CharacterID = wParam;
            pxWindowEvent.InputKeyboard.VirtualKey = PXVirtualKeyFromID(wParam);

            switch(eventID)
            {
                case WM_KEYUP:
                {
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateUp;
                    break;
                }
                case WM_KEYDOWN:
                {
                    pxWindowEvent.InputKeyboard.PressState = PXKeyPressStateDown;
                    break;
                }
            }

            PXWindowEventConsumer(&pxWindowEvent);


            /*
            PXKeyBoardKeyInfo buttonInfo;
            buttonInfo.KeyID = character;
            buttonInfo.Key = virtualKey;
            buttonInfo.Mode = mode;
            */

            // TODO: on system 32 Bit error
            /*
            buttonInfo.Repeat = (characterInfo & 0b00000000000000001111111111111111); // Die Wiederholungsanzahl für die aktuelle Meldung.Der Wert gibt an, wie oft die Tastatureingabe automatisch angezeigt wird, wenn der Benutzer den Schlüssel hält.Die Wiederholungsanzahl ist immer 1 für eine WM _ KEYUP - Nachricht.
            buttonInfo.ScanCode = (characterInfo & 0b00000000111111110000000000000000) >> 16; // Der Scancode.Der Wert hängt vom OEM ab.
            buttonInfo.SpecialKey = (characterInfo & 0b00000001000000000000000000000000) >> 24; // Gibt an, ob es sich bei der Taste um eine erweiterte Taste handelt, z.B.die rechte ALT - und STRG - Taste, die auf einer erweiterten Tastatur mit 101 oder 102 Tasten angezeigt werden.Der Wert ist 1, wenn es sich um einen erweiterten Schlüssel handelt.andernfalls ist es 0.
            //buttonInfo.ReservedDontUse = (characterInfo & 0b00011110000000000000000000000000) >> 25; //    Reserviert; nicht verwenden.
            buttonInfo.KontextCode = (characterInfo & 0b00100000000000000000000000000000) >> 29; // Der Kontextcode.Der Wert ist für eine WM _ KEYUP - Nachricht immer 0.
            buttonInfo.PreState = (characterInfo & 0b01000000000000000000000000000000) >> 30; // Der vorherige Schlüsselzustand.Der Wert ist immer 1 für eine WM _ KEYUP - Nachricht.
            buttonInfo.GapState = (characterInfo & 0b10000000000000000000000000000000) >> 31; // Der Übergangszustand.Der Wert ist immer 1 für eine WM _ KEYUP - Nachricht.
            */

            //  PXWindowTriggerOnKeyBoardKeyEvent(window, &buttonInfo);

            break;
        }
        case WM_XBUTTONDOWN: // Fall though
        case WM_XBUTTONUP:
        {
            PXMouseButton mouseButton = PXMouseButtonInvalid;
            PXKeyPressState buttonState = PXKeyPressStateInvalid;

            const WORD releaseID = HIWORD(wParam);
            // const WORD xxxxx = LOWORD(wParam);
            // const WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
            // const WORD fwButton = GET_XBUTTON_WPARAM(wParam);
            // const int xPos = GET_X_LPARAM(lParam);
            // const int yPos = GET_Y_LPARAM(lParam);

            {
                switch(eventID)
                {

                    case WindowEventXBUTTONUP:
                    {
                        buttonState = PXKeyPressStateUp;
                        break;
                    }

                    case WindowEventXBUTTONDOWN:
                    {
                        buttonState = PXKeyPressStateDown;
                        break;
                    }
                }


                switch(releaseID)
                {
                    case XBUTTON1:
                        mouseButton = PXMouseButtonSpecialA;
                        break;

                    case XBUTTON2:
                        mouseButton = PXMouseButtonSpecialB;
                        break;
                }

                /*
                        switch(fwKeys)
                        {
                            case MK_XBUTTON1:
                                mouseButton = MouseButtonSpecialA;
                                break;

                            case MK_XBUTTON2:
                                mouseButton = MouseButtonSpecialB;
                                break;

                            default:
                                mouseButton = MouseButtonInvalid;
                                break;
                        }
                */


                // PXWindowTriggerOnMouseClickEvent(window, mouseButton, buttonState);
            }

            break;
        }
        // Track non client area
        case WM_NCMOUSEMOVE:
        {
            const int xPos = GET_X_LPARAM(lParam);
            const int yPos = GET_Y_LPARAM(lParam);

#if 0
            PXLogPrint
            (
                PXLoggingInfo,
                "Windows",
                "Event",
                "MouseMove <%ix%i>",
                xPos,
                yPos
            );
#endif

            //   PXNativeDrawMouseTrack(pxWindowEvent.UIElementReference);

            break;
        }

        case WM_MOUSEHOVER: // Never trigger?
        {
#if 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWindowTextText,
                "Event",
                "WM_MOUSEHOVER"
            );
#endif
            //  PXNativeDrawMouseTrack(pxWindowEvent.UIElementReference);

            break;
        }
        case WM_MOUSELEAVE:  // Never trigger?
        {
#if 1
            PXLogPrint
            (
                PXLoggingInfo,
                PXWindowTextText,
                "Event",
                "WM_MOUSELEAVE"
            );
#endif

            // PXNativeDrawMouseTrack(pxWindowEvent.UIElementReference);

            break;
        }


        /*

        case WM_XBUTTONDBLCLK: return WindowEventXBUTTONDBLCLK;
            //   case WM_MOUSEHWHEEL: return WindowEventMOUSEHWHEEL;
                   //case WM_MOUSELAST: return WindowEventMOUSELAST;
        case WM_PARENTNOTIFY: return WindowEventPARENTNOTIFY;
        case WM_ENTERMENULOOP: return WindowEventENTERMENULOOP;
        case WM_EXITMENULOOP: return WindowEventEXITMENULOOP;
        case WM_NEXTMENU: return WindowEventNEXTMENU;
        case WM_SIZING: return WindowEventSIZING;
        case WM_CAPTURECHANGED: return WindowEventCAPTURECHANGED;
        case WM_MOVING: return WindowEventMOVING;
        case WM_POWERBROADCAST: return WindowEventPOWERBROADCAST;

        case WM_MDICREATE: return WindowEventMDICREATE;
        case WM_MDIDESTROY: return WindowEventMDIDESTROY;
        case WM_MDIACTIVATE: return WindowEventMDIACTIVATE;
        case WM_MDIRESTORE: return WindowEventMDIRESTORE;
        case WM_MDINEXT: return WindowEventMDINEXT;

        case WM_MDIMAXIMIZE: return WindowEventMDIMAXIMIZE;
        case WM_MDITILE: return WindowEventMDITILE;
        case WM_MDICASCADE: return WindowEventMDICASCADE;
        case WM_MDIICONARRANGE: return WindowEventMDIICONARRANGE;
        case WM_MDIGETACTIVE: return WindowEventMDIGETACTIVE;
        case WM_MDISETMENU: return WindowEventMDISETMENU;
        case WM_ENTERSIZEMOVE: return WindowEventENTERSIZEMOVE;
        case WM_EXITSIZEMOVE: return WindowEventEXITSIZEMOVE;
        case WM_DROPFILES: return WindowEventDROPFILES;
        case WM_MDIREFRESHMENU: return WindowEventMDIREFRESHMENU;
            //case WM_POINTERDEVICECHANGE: return WindowEventPOINTERDEVICECHANGE;
        //case WM_POINTERDEVICEINRANGE: return WindowEventPOINTERDEVICEINRANGE;
        //case WM_POINTERDEVICEOUTOFRANGE: return WindowEventPOINTERDEVICEOUTOFRANGE;
        //  case WM_TOUCH: return WindowEventTOUCH;
            //case WM_NCPOINTERUPDATE: return WindowEventNCPOINTERUPDATE;
        //case WM_NCPOINTERDOWN: return WindowEventNCPOINTERDOWN;
        //case WM_NCPOINTERUP: return WindowEventNCPOINTERUP;
        //case WM_POINTERUPDATE: return WindowEventPOINTERUPDATE;
        //case WM_POINTERDOWN: return WindowEventPOINTERDOWN;
        //case WM_POINTERUP: return WindowEventPOINTERUP;
        //case WM_POINTERENTER: return WindowEventPOINTERENTER;
        //case WM_POINTERLEAVE: return WindowEventPOINTERLEAVE;
        //case WM_POINTERACTIVATE: return WindowEventPOINTERACTIVATE;
        //case WM_POINTERCAPTURECHANGED: return WindowEventPOINTERCAPTURECHANGED;
        //case WM_TOUCHHITTESTING: return WindowEventTOUCHHITTESTING;
        //case WM_POINTERWHEEL: return WindowEventPOINTERWHEEL;
        //case WM_POINTERHWHEEL: return WindowEventPOINTERHWHEEL;
            //case WM_POINTERROUTEDTO: return WindowEventPOINTERROUTEDTO;
            //case WM_POINTERROUTEDAWAY: return WindowEventPOINTERROUTEDAWAY;
        //case WM_POINTERROUTEDRELEASED: return WindowEventPOINTERROUTEDRELEASED;
        case WM_IME_SETCONTEXT: return WindowEventIME_SETCONTEXT;
        case WM_IME_NOTIFY: return WindowEventIME_NOTIFY;
        case WM_IME_CONTROL: return WindowEventIME_CONTROL;
        case WM_IME_COMPOSITIONFULL: return WindowEventIME_COMPOSITIONFULL;
        case WM_IME_SELECT: return WindowEventIME_SELECT;
        case WM_IME_CHAR: return WindowEventIME_CHAR;
        case WM_IME_REQUEST: return WindowEventIME_REQUEST;
        case WM_IME_KEYDOWN: return WindowEventIME_KEYDOWN;
        case WM_IME_KEYUP: return WindowEventIME_KEYUP;


            //  case WM_WTSSESSION_CHANGE: return WindowEventWTSSESSION_CHANGE;
            //  case WM_TABLET_FIRST: return WindowEventTABLET_FIRST;
            //  case WM_TABLET_LAST: return WindowEventTABLET_LAST;
                  //case WM_DPICHANGED: return WindowEventDPICHANGED;
                  //case WM_DPICHANGED_BEFOREPARENT: return WindowEventDPICHANGED_BEFOREPARENT;
                  //case WM_DPICHANGED_AFTERPARENT: return WindowEventDPICHANGED_AFTERPARENT;
                  // case WM_GETDPISCALEDSIZE: return WindowEventGETDPISCALEDSIZE;
        case WM_CUT: return WindowEventCUT;
        case WM_COPY: return WindowEventCOPY;
        case WM_PASTE: return WindowEventPASTE;
        case WM_CLEAR: return WindowEventCLEAR;
        case WM_UNDO: return WindowEventUNDO;
        case WM_RENDERFORMAT: return WindowEventRENDERFORMAT;
        case WM_RENDERALLFORMATS: return WindowEventRENDERALLFORMATS;
        case WM_DESTROYCLIPBOARD: return WindowEventDESTROYCLIPBOARD;
        case WM_DRAWCLIPBOARD: return WindowEventDRAWCLIPBOARD;
        case WM_PAINTCLIPBOARD: return WindowEventPAINTCLIPBOARD;
        case WM_VSCROLLCLIPBOARD: return WindowEventVSCROLLCLIPBOARD;
        case WM_SIZECLIPBOARD: return WindowEventSIZECLIPBOARD;
        case WM_ASKCBFORMATNAME: return WindowEventASKCBFORMATNAME;
        case WM_CHANGECBCHAIN: return WindowEventCHANGECBCHAIN;
        case WM_HSCROLLCLIPBOARD: return WindowEventHSCROLLCLIPBOARD;
        case WM_QUERYNEWPALETTE: return WindowEventQUERYNEWPALETTE;
        case WM_PALETTEISCHANGING: return WindowEventPALETTEISCHANGING;
        case WM_PALETTECHANGED: return WindowEventPALETTECHANGED;
        case WM_HOTKEY: return WindowEventHOTKEY;
        case WM_PRINT: return WindowEventPRINT;
        case WM_PRINTCLIENT: return WindowEventPRINTCLIENT;
        case WM_APPCOMMAND: return WindowEventAPPCOMMAND;
        case WM_THEMECHANGED: return WindowEventTHEMECHANGED;
            //  case WM_CLIPBOARDUPDATE: return WindowEventCLIPBOARDUPDATE;
            //  case WM_DWMCOMPOSITIONCHANGED: return WindowEventDWMCOMPOSITIONCHANGED;
           //   case WM_DWMNCRENDERINGCHANGED: return WindowEventDWMNCRENDERINGCHANGED;
           //   case WM_DWMCOLORIZATIONCOLORCHANGED: return WindowEventDWMCOLORIZATIONCOLORCHANGED;
          //    case WM_DWMWINDOWMAXIMIZEDCHANGE: return WindowEventDWMWINDOWMAXIMIZEDCHANGE;
          //    case WM_DWMSENDICONICTHUMBNAIL: return WindowEventDWMSENDICONICTHUMBNAIL;
           //   case WM_DWMSENDICONICLIVEPREVIEWBITMAP: return WindowEventDWMSENDICONICLIVEPREVIEWBITMAP;
           //   case WM_GETTITLEBARINFOEX: return WindowEventGETTITLEBARINFOEX;
        case WM_HANDHELDFIRST: return WindowEventHANDHELDFIRST;
        case WM_HANDHELDLAST: return WindowEventHANDHELDLAST;
        case WM_AFXFIRST: return WindowEventAFXFIRST;
        case WM_AFXLAST: return WindowEventAFXLAST;
        case WM_PENWINFIRST: return WindowEventPENWINFIRST;
        case WM_PENWINLAST: return WindowEventPENWINLAST;
        case WM_APP: return WindowEventAPP;




        */

        case WM_DISPLAYCHANGE:
        {
            // Some display got plugged in, out or some setting changed
            // We need to update our monitors/displays



            break;
        }











#if 0 // Not useable for resize event. Its not what it seems
        case WindowEventSizeChange:
        {
            //wParam is unused
            const MINMAXINFO* minmaxInfo = (MINMAXINFO*)lParam;
            const LONG width = minmaxInfo->ptMaxSize.x;
            const LONG height = minmaxInfo->ptMaxSize.y;

            break;
        }
#endif

    }

    // If nothing had grabbed the event by now, let the default funcion handle it.
    return DefWindowProc(windowHandle, eventID, wParam, lParam);
}

#endif

const char* PXAPI PXWindowTypeToString(const PXUIElementType pxUIElementType)
{
    switch(pxUIElementType)
    {
        case PXUIElementTypeCustom:            return "Custom";
        case PXUIElementTypeWindow:            return "Window";
        case PXUIElementTypePanel:            return "Panel";
        case PXUIElementTypeText:            return "Text";
        case PXUIElementTypeButton:               return "Button";
        case PXUIElementTypeImage:                return "Image";
        case PXUIElementTypeDropDown:             return "DropDown";
        case PXUIElementTypeListBox:              return "ListBox";
        case PXUIElementTypeTextEdit:             return "TextEdit";
        case PXUIElementTypeRichEdit:             return "RichText";
        case PXUIElementTypeScrollBar:            return "ScrollBar";
        case PXUIElementTypeTrackBar:             return "TrackBar";
        case PXUIElementTypeStatusBar:            return "StatusBar";
        case PXUIElementTypeUpDown:               return "UpDown";
        case PXUIElementTypeProgressBar:          return "ProgressBar";
        case PXUIElementTypeHotKey:               return "HotKey";
        case PXUIElementTypeCalender:             return "Calender";
        case PXUIElementTypeToolTip:              return "ToolTip";
        case PXUIElementTypeAnimate:              return "Animate";
        case PXUIElementTypeDatePicker:           return "DatePicker";
        case PXUIElementTypeGroupBox:             return "GroupBox";
        case PXUIElementTypeRadioButton:          return "RadioButton";
        case PXUIElementTypeGroupRadioButton:     return "GroupRadioButton";
        case PXUIElementTypeTreeView:             return "TreeView";
        case PXUIElementTypeTreeViewItem:         return "TreeViewItem";
        case PXUIElementTypeFileDirectyView:      return "FileDirectyView";
        case PXUIElementTypeFileDirectyViewEntry: return "FileDirectyViewEntry";
        case PXUIElementTypeIPInput:              return "IPInput";
        case PXUIElementTypeLink:                 return "Link";
        case PXUIElementTypeHeader:               return "Header";
        case PXUIElementTypeFontSelector:         return "FontSelector";
        case PXUIElementTypePageScroll:           return "PageScroll";
        case PXUIElementTypeTabControll:          return "TabControll";
        case PXUIElementTypeTabPage:              return "TAB-Page";
        case PXUIElementTypeToggle:               return "Toggle";
        case PXUIElementTypeCheckBox:             return "CheckBox";
        case PXUIElementTypeComboBox:             return "ComboBox";
        case PXUIElementTypeColorPicker:          return "ColorPicker";
        case PXUIElementTypeSlider:               return "Slider";
        case PXUIElementTypeRenderFrame:          return "FrameRender";
        case PXUIElementTypeMenuStrip:            return "MenuStrip";
        case PXUIElementTypeResourceManger:       return "ResourceManger";
        case PXUIElementTypeResourceEntry:        return "ResourceEntry";

        default:
            return PXNull;
    }
}

PXResult PXAPI PXWindowRegisterToECS()
{
    PXECSRegister(&PXWindowRegisterInfoStatic, &PXWindowRegisterInfoDynamic);

    return PXResultOK;
}

HDC PXAPI PXWindowDCGet(PXWindow PXREF pxWindow)
{
    if(pxWindow->DeviceContextHandle)
    {
        return pxWindow->DeviceContextHandle;
    }

    pxWindow->DeviceContextHandle = GetDC(pxWindow->WindowHandle);

    PXAssert(pxWindow->DeviceContextHandle, "The device context can only be NULL if the window handle is invalid");

    return pxWindow->DeviceContextHandle;
}

HWND PXAPI PXWindowHandleGet(PXWindow PXREF pxWindow)
{
    return pxWindow->WindowHandle;
}

PXResult PXAPI PXWindowEventConsumer(PXWindowEvent PXREF pxWindowEvent)
{
    // Invoke
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXECSInfo* pxECSInfo = (PXECSInfo*)pxWindow;

    switch(pxWindowEvent->Type)
    {
        case PXWindowEventTypeElementClick:
        {
            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "Event-Click",
                "Sender (%0xp)",
                pxWindowEvent->WindowSender->Info.ID
            );

            break;
        }
        case PXWindowEventTypeElementSelect:
        {
            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "Event-Select",
                "ID:<%i>"
            );
            break;
        }
        case PXWindowEventTypeElementDestroy:
        {

            // window->IsRunning = PXFalse;

            break;
        }

        case PXWindowEventTypeElementMove:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "Event-Move",
                ""
            );
#endif

            break;
        }
        case PXWindowEventTypeElementResize:
        {
            PXWindowEventResize PXREF pxWindowEventResize = &pxWindowEvent->Resize;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "Window-Resize",
                "<%ix%i>",
                pxWindowEventResize->Width,
                pxWindowEventResize->Height
            );
#endif
            /*

            PXNativeDrawChildrenReDraw(pxNativDraw, pxWindow);

            if(pxWindow)
            {
                pxWindow->Position.Form.Width = pxWindowEventResize->Width;
                pxWindow->Position.Form.Height = pxWindowEventResize->Height;
                //pxWindow->HasSizeChanged = PXYes;

                if(pxWindow->Info.Hierarchy.ChildFirstborn)
                {
                    PXWindow PXREF pxWindowSub = (PXWindow*)pxWindow->Info.Hierarchy.ChildFirstborn;

                    // Will trigger WM_SIZE, this is recusive
                    InvalidateRect(pxWindowSub->Info.Handle.WindowHandle, NULL, TRUE);
                    // ShowWindow(pxWindowSub->Info.Handle.WindowHandle, SW_HIDE);
                }

                PXWindow* windowCursor = (PXWindow*)pxWindow->Info.Hierarchy.Sibling->Yourself;

                while(windowCursor)
                {
                    const BOOL result = InvalidateRect(windowCursor->Info.Handle.WindowHandle, NULL, TRUE);
                    //ShowWindow(windowCursor->Info.Handle.WindowHandle, SW_HIDE);

                    PXHierarchicalNode* pxHierarchicalNode = windowCursor->Info.Hierarchy.Sibling;

                    if(!pxHierarchicalNode)
                    {
                        break;
                    }

                    windowCursor = (PXWindow*)pxHierarchicalNode->Yourself;
                }
            }
            */

            // Does not work here, as this is not updated for now!
           //PXNativeDrawRefreshSizeAllChildren(pxNativDraw, pxWindowEvent->UIElementReference);



            // PXWindowhSizeRefresAll(pxGUIManager, pxWindowEvent->UIElementReference);



            //  PXFunctionInvoke(window->WindowSizeChangedCallBack, window->EventReceiver, window);

            break;
        }
        case PXWindowEventTypeInputMouseButton:
        {
            break;
        }
        case PXWindowEventTypeInputMouseMove:
        {
            PXWindowEventInputMouseMove* inputMouseMove = &pxWindowEvent->InputMouseMove;

#if 0
            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "Event",
                "PXID:<%i>, MouseMove, Px:%4i Py:%4i Dx:%4i Dy:%4i",
                pxECSInfo->ID,
                inputMouseMove->Position.X,
                inputMouseMove->Position.Y,
                inputMouseMove->Delta.X,
                inputMouseMove->Delta.Y
            );
#endif

            break;
        }
        case PXWindowEventTypeInputKeyboard:
        {
            PXWindowEventInputKeyboard* inputKeyboard = &pxWindowEvent->InputKeyboard;

            const char* buttonState = PXKeyPressStateToString(inputKeyboard->PressState);

            PXLogPrint
            (
                PXLoggingEvent,
                PXWindowTextText,
                "Event",
                "PXID:<%i>, KeyBoard, VKey:%-3i (%lc), %s",
                pxECSInfo->ID,
                inputKeyboard->VirtualKey,
                inputKeyboard->CharacterW,
                buttonState
            );

            break;
        }
        default:
        {
            return PXResultRefusedParameterInvalid;
        }
    }
    
    if(pxWindow)
    {
        PXECSInfo* callBackOwner = pxWindow->EventList.CallBackOwner;
        PXWindowEventFunction pxWindowEventFunction = pxWindow->EventList.CallBackEvent;

        if(pxWindowEventFunction)
        {
            pxWindowEventFunction(callBackOwner, pxWindowEvent);
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXWindowEventPendingAmount(PXSize PXREF amount)
{
#if OSUnix

    * amount = XPending(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle); // X11

    return PXResultOK;

#elif OSWindows

    const DWORD inputAmount = GetQueueStatus(QS_ALLINPUT); // Windows 2000, User32.dll

    *amount = inputAmount;

    return PXResultOK;

#else
    * amount = 0;

    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXWindowEventPoll(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxWindow->WindowHandle)
    {
        return PXResultRefusedParameterInvalid;
    }

    // pxWindow->MouseCurrentInput.ButtonsDelta = 0; // Reset mouse data

#if OSUnix
    XEvent windowEvent;

    assert(pxNativDraw->GUISystem);
    assert(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle);

    Display* displayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;

    /*
        Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;
        unsigned int mask_return;
        /*
         * We need:
         *     child_return - the active window under the cursor
         *     win_{x,y}_return - pointer coordinate with respect to root window
         * /
        int retval = XQueryPointer
        (
            display,
            root_window,
            &root_return,
            &child_return,
            &root_x_return,
            &root_y_return,
            &win_x_return,
            &win_y_return,
            &mask_return
        );*/



        // Multithread lock
    XLockDisplay(displayHandle);

    // poll all events
    for(;;)
    {
        // Check if we have events
        const int amountOfEventsPending = XPending(displayHandle);

        if(0 == amountOfEventsPending)
        {
            // Done
            break;
        }

        for(PXI32U i = 0; i < amountOfEventsPending; ++i)
        {
            // Fetch event form the display
            XNextEvent(displayHandle, &windowEvent);

            // Translate event
            PXNativDrawEventTranslator(pxNativDraw, &windowEvent);
        }
    }

    // Multithread unlock
    XUnlockDisplay(displayHandle);

#elif PXOSWindowsDestop
    const HWND windowHandle = pxWindow->WindowHandle;

    MSG message;
    BOOL isOK;
    LRESULT dispatchResult;

    for(;;)
    {
        isOK = PeekMessage(&message, PXNull, 0, 0, PM_NOREMOVE); // Windows 2000, User32.dll, winuser.h

        if(!isOK)
        {
            break; // Stop, no more messages
        }

        isOK = GetMessage(&message, PXNull, 0, 0); // Windows 2000, User32.dll, winuser.h

        if(!isOK)
        {
            continue; // Skip, message fetch failed
        }

        isOK = TranslateMessage(&message); // Windows 2000, User32.dll, winuser.h
        dispatchResult = DispatchMessage(&message); // Windows 2000, User32.dll, winuser.h
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowBufferSwap(const PXWindow PXREF pxWindow)
{
    PXResult pxActionResult = PXResultInvalid;

#if OSUnix
    glXSwapBuffers(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->WindowHandle);
    pxActionResult = PXResultOK; // TODO: Do we have error checking?

#elif OSWindows
    const BOOL result = SwapBuffers(pxWindow->DeviceContextHandle);
    pxActionResult = PXErrorCurrent(result);
#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDraw,
        "BufferSwap",
        "%i",
        pxActionResult
    );
#endif

    return pxActionResult;
}

PXResult PXAPI PXWindowDestroy(PXWindow PXREF pxWindow)
{
    PXResult result = PXResultInvalid;

    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    // Step 1) Disconnect OpenGL possible connection
    if(pxWindow->RenderContext)
    {
#if OSUnix
        glXMakeCurrent(window->DisplayCurrent, None, NULL);
        //glXDestroyContext(DisplayCurrent, PXOpenGLConextID);
#elif OSWindows
        wglMakeCurrent(pxWindow->DeviceContextHandle, PXNull);
        wglDeleteContext(pxWindow->RenderContext);
#endif

        pxWindow->RenderContext = PXNull;
    }

#if OSUnix
    const int resultID = XDestroyWindow(_pxGUIManager.DisplayCurrent.DisplayHandle, pxWindow->WindowHandle);
    result = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    const BOOL success = DestroyWindow(pxWindow->WindowHandle);
    result = PXErrorCurrent(success);
#else
    result PXActionRefusedNotSupportedByLibrary;
#endif

    if(PXResultOK != result)
    {
        return result;
    }

    // Step 4, reset handles
    pxWindow->WindowHandle = PXNull;
    pxWindow->DeviceContextHandle = PXNull;

    return result;
}

PXResult PXAPI PXWindowFindViaTitle(PXWindow PXREF pxWindow, const PXText PXREF pxText)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    HWND windowHandle = PXNull;

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            windowHandle = FindWindowA(0, pxText->A); // Windows 2000, User32.dll, winuser.h
            break;
        }
        case TextFormatUNICODE:
        {
            windowHandle = FindWindowW(0, pxText->W); // Windows 2000, User32.dll, winuser.h
            break;
        }
        default:
            return PXResultRefusedParameterInvalid;
    }

    return PXResultOK;
}

PXResult PXAPI PXWindowDragStart(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows
    HWND hwndParent = PXNull;
    HWND hwndFrom = PXNull;
    UINT fmt = 0;
    ULONG_PTR data = 0;
    HCURSOR hcur = PXNull;

    const DWORD aa = DragObject(pxWindow->WindowHandle, hwndFrom, fmt, data, hcur);
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

    //const BOOL res = DragDetect();

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXWindowPixelSystemSet(PXWindow PXREF pxWindow, PXWindowPixelSystemInfo PXREF pxWindowPixelSystemInfo)
{
    if(!(pxWindow && pxWindowPixelSystemInfo))
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "PixelSystem",
        "Setting..."
    );
#endif

#if OSUnix
    // TODO: implement?

    return PXResultOK;

#elif OSWindows

    const WORD  nVersion = 1;
    DWORD dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_DIRECT3D_ACCELERATED |
        PFD_GENERIC_ACCELERATED |
        PFD_DOUBLEBUFFER |
        0;

    dwFlags |= pxWindowPixelSystemInfo->OpenGL * PFD_SUPPORT_OPENGL;
    dwFlags |= pxWindowPixelSystemInfo->DirectX * PFD_SUPPORT_DIRECTDRAW;
    dwFlags |= pxWindowPixelSystemInfo->GDI * PFD_SUPPORT_GDI;


    const BYTE  iPixelType = PFD_TYPE_RGBA; // The kind of framebuffer. RGBA or palette.
    const BYTE  cColorBits = pxWindowPixelSystemInfo->BitPerPixel;   // Colordepth of the framebuffer.
    const BYTE  cRedBits = 0;
    const BYTE  cRedShift = 0;
    const BYTE  cGreenBits = 0;
    const BYTE  cGreenShift = 0;
    const BYTE  cBlueBits = 0;
    const BYTE  cBlueShift = 0;
    const BYTE  cAlphaBits = 0;
    const BYTE  cAlphaShift = 0;
    const BYTE  cAccumBits = 0;
    const BYTE  cAccumRedBits = 0;
    const BYTE  cAccumGreenBits = 0;
    const BYTE  cAccumBlueBits = 0;
    const BYTE  cAccumAlphaBits = 0;
    const BYTE  cDepthBits = 24; // Number of bits for the depthbuffer
    const BYTE  cStencilBits = 8;  // Number of bits for the stencilbuffer
    const BYTE  cAuxBuffers = 0;  // Number of Aux buffers in the framebuffer.
    const BYTE  iLayerType = PFD_MAIN_PLANE;
    const BYTE  bReserved = 0;
    const DWORD dwLayerMask = 0;
    const DWORD dwVisibleMask = 0;
    const DWORD dwDamageMask = 0;
    const PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        nVersion,
        dwFlags,
        iPixelType,
        cColorBits,
        cRedBits,cRedShift,
        cGreenBits, cGreenShift,
        cBlueBits, cBlueShift,
        cAlphaBits, cAlphaShift,
        cAccumBits,
        cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits,
        cDepthBits,
        cStencilBits,
        cAuxBuffers,
        iLayerType,
        bReserved,
        dwLayerMask, dwVisibleMask, dwDamageMask
    };

    const HDC windowDeviceHandle = PXWindowDCGet(pxWindow);
    const int pixelFormatIndex = ChoosePixelFormat(windowDeviceHandle, &pixelFormatDescriptor);
    const BOOL successul = SetPixelFormat(windowDeviceHandle, pixelFormatIndex, &pixelFormatDescriptor);
    const PXResult pxActionResult = PXErrorCurrent(successul);

    if(PXResultOK != pxActionResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXWindowTextText,
            "PixelSystem",
            "Setting failed"
        );
#endif

        return pxActionResult;
    }

    return pxActionResult;

#else
    return PXActionRefusedNotSupported;
#endif
}

PXBool PXAPI PXWindowIsInFocus(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    return PXFalse;
#elif PXOSWindowsDestop
    const HWND windowIDInFocus = GetForegroundWindow(); // Windows 2000, User32.dll,
    const PXBool isInFocus = pxWindow->WindowHandle == windowIDInFocus;

    return isInFocus;
#endif
}

PXResult PXAPI PXWindowShow(const PXWindow PXREF pxWindow)
{    
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    // EnableWindow(pxWindow->ID, show);
    HWND windowHandle = pxWindow->WindowHandle;


    ShowWindow(windowHandle, SW_SHOW);
    BOOL res = ShowWindow(windowHandle, SW_SHOW); // Twice to prevent some errors


    return PXResultOK;
}

PXResult PXAPI PXWindowHide(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXWindowRedraw(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    InvalidateRect(pxWindow->WindowHandle, 0, PXFalse);
    UpdateWindow(pxWindow->WindowHandle);

    return PXResultOK;
}

PXResult PXAPI PXWindowRedrawEnable(PXWindow PXREF pxWindow, const PXBool enable)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows

    const LRESULT result = SendMessage(pxWindow->WindowHandle, WM_SETREDRAW, enable, 0);

    if(enable)
    {
        const BOOL result = InvalidateRect(pxWindow->WindowHandle, NULL, TRUE); // forces the window to redraw.
    }

#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowDragAndDropBegin(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    // Gegister drag&Drop
    IDropTarget dropTarget;

    const HRESULT dragResult = RegisterDragDrop(pxWindow->WindowHandle, &dropTarget); // Windows 2000, Ole32.dll, ole2.h

    return PXResultOK;
}

PXResult PXAPI PXWindowDragAndDropEnd(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawBegin(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

#elif OSWindows
    PAINTSTRUCT paintSturct;
    PXClear(PAINTSTRUCT, &paintSturct);

    const HDC hdc = BeginPaint(pxWindow->WindowHandle, &paintSturct);
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXWindowDrawEnd(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

#elif OSWindows
    PAINTSTRUCT paintSturct;
    PXClear(PAINTSTRUCT, &paintSturct);

    const PXBool end = EndPaint(pxWindow->WindowHandle, &paintSturct);
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXWindowScrollbarUpdate(PXWindow PXREF pxWindow, PXScollbar PXREF pxScollbar)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows

    UINT flags = 0;

    if(PXScollbarBehaviourVertical & pxScollbar->Info.Behaviour)
    {
        flags |= SB_VERT;
    }

    if(PXScollbarBehaviourHorrizontal & pxScollbar->Info.Behaviour)
    {
        flags |= SB_HORZ;
    }

    const BOOL enable = EnableScrollBar
    (
        pxWindow->WindowHandle,
        flags,
        ESB_ENABLE_BOTH
    );

    const BOOL show = ShowScrollBar
    (
        pxWindow->WindowHandle,
        flags,
        TRUE
    );

    SCROLLINFO scrollInfo;
    PXClear(SCROLLINFO, &scrollInfo);
    scrollInfo.cbSize = sizeof(SCROLLINFO);
    scrollInfo.nMin = 0;
    scrollInfo.nMax = 5;
    scrollInfo.nPos = SIF_POS | SIF_RANGE;

    int xx = SetScrollInfo
    (
        pxWindow->WindowHandle,
        flags,
        &scrollInfo,
        TRUE
    );

#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Scrollbar",
        "-"

    );
#endif

    return PXResultOK;
}


PXLibrary pxLibrary;
typedef HRESULT(WINAPI* PXDwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, _In_reads_bytes_(cbAttribute) LPCVOID pvAttribute, DWORD cbAttribute);
PXDwmSetWindowAttribute pxDwmSetWindowAttribute = PXNull;

PXResult PXAPI PXWindowTitleBarColor(PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
       
    // If not loaded already
    if(!pxDwmSetWindowAttribute)
    {
        // Open lib
        {
            PXText pxText;
            PXTextFromAdressA(&pxText, WindowsLibraryDWMAPI, sizeof(WindowsLibraryDWMAPI), sizeof(WindowsLibraryDWMAPI));
            const PXResult libOpenResult = PXLibraryOpen(&pxLibrary, &pxText);

            if(PXResultOK != libOpenResult)
            {
                return libOpenResult;
            }
        }

        const PXBool hasFunction = PXLibraryGetSymbolA(&pxLibrary, &pxDwmSetWindowAttribute, WindowsLibraryDWMAPISET, PXTrue);

        if(!hasFunction)
        {
            PXLibraryClose(&pxLibrary);

            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }
    

    if(!pxDwmSetWindowAttribute)
    {
        return PXActionRefusedNotSupportedByOperatingSystem;
    }


    const HWND windowHandle = pxWindow->WindowHandle;
    const BOOL useDarkMode = PXTrue;
    const HRESULT resultID = pxDwmSetWindowAttribute
    (
        windowHandle, 
        DWMWA_USE_IMMERSIVE_DARK_MODE, 
        &useDarkMode, 
        sizeof(BOOL)
    ); // Windows Vista, Dwmapi.dll;Uxtheme.dll, dwmapi.h
    const PXResult setResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != setResult)
    {
        return setResult;
    }

    ShowWindow(windowHandle, SW_HIDE);
    ShowWindow(windowHandle, SW_SHOW);

    return setResult;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXWindowForegroundSet(PXWindow PXREF pxWindow, PXColorRGBI8 PXREF pxColorRGBI8)
{
#if OSUnix
    const PXI32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
    const int resultID = XSetForeground
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        color
    );
#elif OSWindows
    const COLORREF colorNew = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
    const COLORREF colorPrevious = SetTextColor(pxWindow->DeviceContextHandle, colorNew);
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowBackgroundSet(PXWindow PXREF pxWindow, PXColorRGBI8 PXREF pxColorRGBI8)
{
    PXResult pxActionResult = PXResultInvalid;

#if OSUnix
    const PXI32U color = PXColorI32FromBGR(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
    const int resultID = XSetBackground
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        color
    );
#elif OSWindows

    const int mode = SetBkMode(pxWindow->DeviceContextHandle, OPAQUE);

    const COLORREF colorNew = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);
    const COLORREF colorPrevious = SetBkColor(pxWindow->DeviceContextHandle, colorNew);
    const PXBool successful = CLR_INVALID != colorPrevious;

    if(!successful)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXWindowTextText,
            "Color-Set",
            "Failed set backgroundcolor"
        );
#endif

        return PXResultRefusedParameterInvalid;
    }

#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowFontSet(PXWindow PXREF pxWindow, PXFont PXREF pxFont)
{
    if(!(pxWindow && pxFont))
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Font-Select",
        "PXID:%-4i %p %s",
        pxFont->Info.ID,
        pxFont->FontHandle,
        pxFont->Name

    );
#endif

#if OSUnix
    const int resultID = XSetFont
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        pxFont->Info.Handle.FontHandle->fid
    );
#elif OSWindows

    if(!pxFont->FontHandle)
    {
        return PXResultRefusedParameterInvalid;
    }

    const HFONT fontHandleOld = (HFONT)SelectObject(pxWindow->DeviceContextHandle, pxFont->FontHandle);


#endif

    _lastFont = pxFont;

    return PXResultOK;
}

PXResult PXAPI PXWindowBrushSet(PXWindow PXREF pxWindow, PXBrush PXREF pxBrush, const PXI8U mode)
{
    if(!(pxWindow && pxBrush))
    {
        return PXResultRefusedParameterNull;
    }

    PXColorRGBI8* colorRef = PXNull;

    if(PXWindowBrushBehaviourColorEmbeded & pxBrush->Info.Behaviour)
    {
        colorRef = &pxBrush->ColorDate;
    }
    else
    {
        colorRef = pxBrush->ColorReference;
    }

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Brush-Set",
        "%2.2X%2.2X%2.2X",
        colorRef->Red,
        colorRef->Green,
        colorRef->Blue
    );
#endif

    if(mode == PXGUIDrawModeFront)
    {
        PXWindowForegroundSet(pxWindow, colorRef);
    }
    else
    {
        PXWindowBackgroundSet(pxWindow, colorRef);
    }

    return PXResultOK;
}

PXResult PXAPI PXWindowTextGet(PXWindow PXREF pxWindow, PXText PXREF pxText)
{
    PXResult pxActionResult = PXResultInvalid;

#if OSUnix
    pxActionResult = PXActionRefusedNotImplemented;
#elif OSWindows

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            pxText->SizeUsed = GetWindowTextA
            (
                pxWindow->WindowHandle,
                pxText->A,
                pxText->SizeAllocated
            );
            break;
        }
        case TextFormatUNICODE:
        {
            pxText->SizeUsed = GetWindowTextW
            (
                pxWindow->WindowHandle,
                pxText->W,
                pxText->SizeAllocated
            );

            break;
        }

        default:
            break;
    }

#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXResult PXAPI PXWindowTextSet(PXWindow PXREF pxWindow, PXText PXREF pxText)
{
    PXResult pxActionResult = PXResultInvalid;

#if OSUnix
    pxActionResult = PXActionRefusedNotImplemented;
#elif OSWindows

    BOOL result = FALSE;

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            result = SetWindowTextA
            (
                pxWindow->WindowHandle,
                pxText->A
            );

            break;
        }
        case TextFormatUNICODE:
        {
            result = SetWindowTextW
            (
                pxWindow->WindowHandle,
                pxText->W
            );
            break;
        }

        default:
            break;
    }

#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXResult PXAPI PXWindowStyleUpdate(PXWindow PXREF pxWindow)
{
    PXResult result = PXResultInvalid;

#if OSUnix
    result = PXActionRefusedNotImplemented;
#elif OSWindows
    SetWindowLongPtr(pxWindow->WindowHandle, GWL_STYLE, WS_SYSMENU); //3d argument=style
    const PXBool setSuccess = SetWindowPos
    (
        pxWindow->WindowHandle,
        HWND_TOPMOST,
        100,
        100,
        100,
        100,
        SWP_SHOWWINDOW
    );
#else
    result = PXActionRefusedNotSupportedByLibrary;
#endif

    return result;
}

PXResult PXAPI PXWindowRectangleXYWH(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangleXYWHI32)
{
#if 0
    *pxRectangleXYWHI32 = pxWindow->Position.Form;
#else
    RECT rectangle;
    BOOL result = GetClientRect(pxWindow->WindowHandle, &rectangle);

    pxRectangleXYWHI32->X = rectangle.left;
    pxRectangleXYWHI32->Y = rectangle.top;
    pxRectangleXYWHI32->Width = rectangle.right;
    pxRectangleXYWHI32->Height = rectangle.bottom;
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowRectangleLTRB(PXWindow PXREF pxWindow, PXRectangleLTRBI32 PXREF pxRectangleLTRBI32)
{
    RECT rectangle;
    BOOL result = GetClientRect(pxWindow->WindowHandle, &rectangle);

    return PXResultOK;
}

PXBool PXAPI PXWindowIsValid(const PXWindow PXREF pxWindow)
{
    const PXBool isWindowValid = IsWindow(pxWindow->WindowHandle);

    if(!isWindowValid)
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXWindowTextText,
            "Valid",
            "%20s (0x%p), Invalid",
            "",//pxWindow->NameData,
            pxWindow->WindowHandle
        );
    }

    return PXTrue;
}

PXBool PXAPI PXWindowIsEnabled(const PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXFalse;
#elif OSWindows
    const BOOL success = IsWindowEnabled(pxWindow->WindowHandle); // Windows 2000, User32.dll, winuser.h
    const PXResult pxActionResult = PXErrorCurrent(success);

    return pxActionResult;
#else
    return PXFalse;
#endif
}

PXResult PXAPI PXWindowDrawRectangle2D(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawRectangleInfo)
{
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        PXRectangle2DText,
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        pxWindowDrawRectangleInfo->RectangleXYWH.X,
        pxWindowDrawRectangleInfo->RectangleXYWH.Y,
        pxWindowDrawRectangleInfo->RectangleXYWH.Width,
        pxWindowDrawRectangleInfo->RectangleXYWH.Height
    );
#endif

    const PXBool isHovered = (PXECSInfoSelected & pxWindow->Info.Behaviour) > 0;

    //  const BOOL bbbbb = SelectObject(pxWindow->DeviceContextHandle, GetStockObject(GRAY_BRUSH));

    // const COLORREF color = RGB(255, 0, 200);
    // const HBRUSH brushAA = CreateSolidBrush(color);

    PXBrush* pxWindowBrush = PXNull;

    if(isHovered)
    {
        pxWindowBrush = pxGUITheme->BrushTextWhite;
    }
    else
    {
        pxWindowBrush = pxGUITheme->BrushMainPrimary;
    }

    PXWindowDrawInfo pxWindowDrawRectangleInfoSub;
    PXClear(PXWindowDrawInfo, &pxWindowDrawRectangleInfoSub);
    pxWindowDrawRectangleInfoSub.RectangleXYWH = pxWindowDrawRectangleInfo->RectangleXYWH;
    pxWindowDrawRectangleInfoSub.Brush = pxWindowBrush;
    pxWindowDrawRectangleInfoSub.Rounded = PXFalse;

    PXResult pxResult = PXWindowDrawRectangle(pxWindow, &pxWindowDrawRectangleInfoSub);

    return pxResult;
}

PXResult PXAPI PXWindowDrawRectangle3D(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawRectangleInfo)
{
    //PXAssert(width > 0, "Width is 0");
      //PXAssert(height > 0, "height is 0");

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        PXRectangle3DText,
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        pxWindowDrawRectangleInfo->RectangleXYWH.X,
        pxWindowDrawRectangleInfo->RectangleXYWH.Y,
        pxWindowDrawRectangleInfo->RectangleXYWH.Width,
        pxWindowDrawRectangleInfo->RectangleXYWH.Height
    );
#endif

    const PXBool isHovered = (PXECSInfoSelected & pxWindow->Info.Behaviour) > 0;
    PXGUITheme* pxGUITheme = PXGUIThemeGet();
    PXBrush* brushFront = pxGUITheme->BrushTextWhite;
    PXBrush* brushBackground = pxGUITheme->BrushMainPrimary;


    const PXF32 colorTint = isHovered * 0.4f;
    const PXF32 highFactor = 1.35f;
    const PXF32 lowFactor = 0.65f;


    PXColorRGBI8 highColor;
    highColor.Red = colorTint + highFactor * brushBackground->ColorDate.Red;
    highColor.Green = colorTint + highFactor * brushBackground->ColorDate.Green;
    highColor.Blue = colorTint + highFactor * brushBackground->ColorDate.Blue;

    PXColorRGBI8 lowColor;
    lowColor.Red = colorTint + lowFactor * brushBackground->ColorDate.Red;
    lowColor.Green = colorTint + lowFactor * brushBackground->ColorDate.Green;
    lowColor.Blue = colorTint + lowFactor * brushBackground->ColorDate.Blue;

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
            PXWindowBrushSet
            (
                pxWindow,
                brushBackground,
                PXGUIDrawModeBack
            );
            PXWindowDrawRectangle
            (
                pxWindow,
                pxWindowDrawRectangleInfo
            );


#if OSWindows

            const COLORREF highColorWIN = RGB
            (
                highColor.Red,
                highColor.Green,
                highColor.Blue
            );
            const COLORREF lowColorWIN = RGB
            (
                lowColor.Red,
                lowColor.Green,
                lowColor.Blue
            );

            const DWORD penStyle = PS_ENDCAP_SQUARE | PS_GEOMETRIC | PS_SOLID | PS_JOIN_MITER;
            const DWORD penSize = 2;
            const PXI16U borderInwardOffset = penSize - 1;

            if(penSize == 0)
            {
                return PXResultOK;
            }

            const HPEN penLight = CreatePen(penStyle, penSize, highColorWIN);
            const HPEN penDark = CreatePen(penStyle, penSize, lowColorWIN);

            HDC hdc = PXWindowDCGet(pxWindow);

            PXI32S x = pxWindowDrawRectangleInfo->RectangleXYWH.X;
            PXI32S y = pxWindowDrawRectangleInfo->RectangleXYWH.Y;
            PXI32S width = pxWindowDrawRectangleInfo->RectangleXYWH.Width;
            PXI32S height = pxWindowDrawRectangleInfo->RectangleXYWH.Height;

            // Bottom, right, dark line
            MoveToEx(hdc, x + width - borderInwardOffset, y + borderInwardOffset, PXNull);
            SelectObject(hdc, penDark); // Windows 2000, Gdi32.dll, wingdi.h, 
            LineTo(hdc, x + width - borderInwardOffset, y + height - borderInwardOffset);
            LineTo(hdc, x + borderInwardOffset, y + height - borderInwardOffset);

            SelectObject(hdc, penLight);
            LineTo(hdc, x + borderInwardOffset, y + borderInwardOffset);
            LineTo(hdc, width - borderInwardOffset, y + borderInwardOffset);

            DeleteObject(penLight);
            DeleteObject(penDark);
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {

            float diff = 0.1;
            float primeColor = 0.7f;// -diff / 2 + (cosf(time * 20)) * diff;

            // Optional shake effect
            float offset = 0;

            if(1)
            {
                //offset = PXMathSinusRADF32(xt * 200.8f) * 0.01f;
                float x = offset;

               // xt += 0.01;
            }

            PXGraphic* pxGraphic = PXGraphicInstantiateGET();
            PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;

            //glEnable(GL_BLEND);
            //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            pxOpenGL->Binding.Enable(GL_LINE_SMOOTH);
            pxOpenGL->Binding.Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);


            /*
            // Draw gradient background
            pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            pxOpenGL->Binding.Begin(GL_QUADS);
            pxOpenGL->Binding.Color3fv(colorBottom);
            pxOpenGL->Binding.Vertex2f(-1.0f+ offset, -1.0f);
            pxOpenGL->Binding.Vertex2f(+1.0f - offset, -1.0f);
            pxOpenGL->Binding.Color3fv(colorTop);
            pxOpenGL->Binding.Vertex2f(+1.0f - offset, 1.0f);
            pxOpenGL->Binding.Vertex2f(-1.0f + offset, 1.0f);
            pxOpenGL->Binding.End();
            */

            pxOpenGL->Binding.LineWidth(1);

#if 0
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glBegin(GL_QUADS);
            glColor3fv(borderDark); glVertex2f(x, y);
            glColor3fv(borderLight); glVertex2f(x + width, y);
            glColor3fv(borderDark); glVertex2f(x + width, y + height);
            glColor3fv(borderLight); glVertex2f(x, y + height);
            glEnd();
#else
            
            
            // Draw border (light top-left, dark bottom-right)

            PXI32S x = pxWindowDrawRectangleInfo->RectangleXYWH.X;
            PXI32S y = pxWindowDrawRectangleInfo->RectangleXYWH.Y;
            PXI32S width = pxWindowDrawRectangleInfo->RectangleXYWH.Width;
            PXI32S height = pxWindowDrawRectangleInfo->RectangleXYWH.Height;


            if(0)
            {
                pxOpenGL->Binding.Begin(GL_LINES);

                pxOpenGL->Binding.Color3bv(&highColor);

                pxOpenGL->Binding.Vertex2f(-1.0f, -1.0f);
                pxOpenGL->Binding.Vertex2f(+1.0f, -1.0f); // Top
                pxOpenGL->Binding.Vertex2f(-1.0f, +1.0f);
                pxOpenGL->Binding.Vertex2f(+1.0f, +1.0f); // Left


                pxOpenGL->Binding.Color3bv(&lowColor);
                pxOpenGL->Binding.Vertex2f(x + width, y);
                pxOpenGL->Binding.Vertex2f(x + width, y + height); // Right
                pxOpenGL->Binding.Vertex2f(x, y + height);
                pxOpenGL->Binding.Vertex2f(x + width, y + height); // Bottom 

                pxOpenGL->Binding.End();
            }


#endif

            pxOpenGL->Binding.Flush();

            break;
        }

        default:
            break;
    }

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawClear(PXWindow PXREF pxWindow)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Clear",
        "PXID:%i",
        pxWindow->Info.ID
    );
#endif

#if OSUnix
    const int resultID = XClearWindow(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowHandle);
#elif OSWindows
    // Does this exists?

    //GetUpdateRect();
#endif

    return PXResultOK;
}

PXResult PXAPI PXTextDraw(PXTextDrawInfo PXREF pxTextDrawInfo, PXOpenGL PXREF pxOpenGL, PXFont PXREF pxFont, PXText PXREF pxText)
{
    GLYPHMETRICSFLOAT* glythList = pxFont->GlyphMetricsFloat;
    PXResult pxResult = PXResultOK;

    glPushMatrix(); // We need to store the current matrix to use a new one
    //glScalef(0.7, -0.7, 1);

    pxOpenGL->Binding.Translatef
    (
        0.0,
        pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Height,
        0.0f
    );

    /*
    glOrtho
    (
        pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.X,
        pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Width,
        pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Height,
        pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Y,
        -1,
        1
    );*/

    //glLoadIdentity();

    glScalef(50, -50, 1.0);

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            for(PXSize i = 0; i < pxText->SizeUsed; ++i)
            {
                const char character = pxText->A[i];
                GLYPHMETRICSFLOAT* glyth = &glythList[character];

                if(0 == character)
                {
                    break;
                }

                // Movement is additive!             
                pxOpenGL->Binding.CallList(pxFont->FontBase + character);
                pxOpenGL->Binding.Translatef
                (
                    pxTextDrawInfo->OffsetX * glyth->gmfCellIncX,
                    0.0f,
                    0.0f
                );
            }

            break;
        }
        case TextFormatUNICODE:
        {
            for(PXSize i = 0; i < pxText->SizeUsed; ++i)
            {
                const wchar_t character = pxText->W[i];

                if(0 == character)
                {
                    break;
                }

                GLYPHMETRICSFLOAT* glyth = &glythList[character];

                pxOpenGL->Binding.Translatef(pxTextDrawInfo->OffsetX * glyth->gmfCellIncX, 0.0f, 0.0f);
                pxOpenGL->Binding.CallList(pxFont->FontBase + character);
            }

            break;
        }
        default:
        {
            pxResult = PXResultRefusedParameterInvalid;
            break;
        }
    }

    glPopMatrix();

    return pxResult;
}

float hyCol = 0;

PXResult PXAPI PXWindowDrawText
(
    PXWindow PXREF pxWindow,
    PXTextDrawInfo PXREF pxTextDrawInfo
)
{
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXGraphic PXREF pxGraphic = PXGraphicInstantiateGET();

    if(!(pxWindow && pxTextDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;
    PXBrush PXREF brushText = pxGUITheme->BrushTextWhite;
    

#if 1
 


   // float desiredSize = pxTextDrawInfo->Size * 100; // pixels
   // float aspectRatio = width / height;

  //  float xScale = desiredSize / aspectRatio;
    //float yScale = desiredSize;
   
    // Y scale defines the actual text size
    //float yScale = desiredSize * (1.0f / height) * 2.0f;

    // X scale must be corrected by aspect ratio
    //float xScale = yScale / aspectRatio;

  //  glScalef(xScale, yScale, 1.0);

   // glScalef(desiredSize, desiredSize, 1.0);

#endif

    PXText pxTextFailsafe;
    PXTextFromAdressA(&pxTextFailsafe, PXTextFailsafe, PXTextFailsafeLength, PXTextFailsafeLength);

    PXText* targetText = pxTextDrawInfo->Text;

    if(!targetText)
    {
        targetText = &pxTextFailsafe;
    }

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
#if OSUnix

            PXRectangleXYWHI32 pxRectangleXYWHI32;

            PXNativDrawRectangleParent(pxNativDraw, pxWindow, &pxRectangleXYWHI32);

            int xMX = x;
            int yMX = y;


            if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignLeft))
            {
                xMX = pxRectangleXYWHI32.X + x;
            }

            if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignRight))
            {
                xMX = width - (pxRectangleXYWHI32.X + x);
            }


            if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignTop))
            {
                yMX = pxRectangleXYWHI32.Y + y;
            }

            if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignBottom))
            {
                yMX = height - (pxRectangleXYWHI32.Y + y);
            }

            if(PXFlagIsSet(pxWindow->Info.Behaviour, PXWindowAllignCenter))
            {
                yMX = (height / 2) - (pxRectangleXYWHI32.Y + y);
            }


            // For ANSI and UTF-8 strings
            const int resultID = XDrawString
            (
                pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
                pxWindow->Info.Handle.WindowHandle,
                pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
                xMX,
                yMX,
                text,
                textSize
            );
#elif OSWindows

            UINT format = DT_SINGLELINE | DT_NOCLIP;
            format |= PXFlagIsSet(pxTextDrawInfo->Behaviour, PXWindowAllignTop) * DT_TOP;
            format |= PXFlagIsSet(pxTextDrawInfo->Behaviour, PXWindowAllignLeft) * DT_LEFT;
            format |= PXFlagIsSet(pxTextDrawInfo->Behaviour, PXWindowAllignRight) * DT_RIGHT;
            format |= PXFlagIsSet(pxTextDrawInfo->Behaviour, PXWindowAllignBottom) * DT_BOTTOM;

            if(PXFlagIsSet(pxTextDrawInfo->Behaviour, PXWindowAllignCenter))
            {
                format |= DT_VCENTER | DT_CENTER;
            }

            RECT* rectangle = (RECT*)&pxTextDrawInfo->WindowDrawInfo->RectangleXYWH;

            switch(targetText->Format)
            {
                case TextFormatASCII:
                {
                    const int nextHeightBBB = DrawTextExA
                    (
                        pxWindow->DeviceContextHandle,
                        targetText->A,
                        targetText->SizeUsed,
                        rectangle,
                        format,
                        PXNull
                    ); // Windows 2000, User32.dll, winuser.h
                    break;
                }
                case TextFormatUNICODE:
                {
                    const int nextHeightBBB = DrawTextExW
                    (
                        pxWindow->DeviceContextHandle,
                        targetText->W,
                        targetText->SizeUsed,
                        rectangle,
                        format,
                        PXNull
                    ); // Windows 2000, User32.dll, winuser.h
                    break;
                }
                default:
                    break;
            }       
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            PXFont* fontTitle = pxGUITheme->FontTitle;

            PXWindowFontSet(pxWindow, fontTitle);

            if(fontTitle)
            {
                if(!fontTitle->IsOK)
                {
                    int glyphsIndexFirst = 0;
                    int glyphsAmount = 200;// Full Unicode range
                    float maximumChordalDeviation = 0;
                    float extrusion = 0; // Add to Z axis, 0=No extrusion

                    fontTitle->GlyphMetricsFloat = PXMemoryHeapCallocT(GLYPHMETRICSFLOAT, glyphsAmount);
                    fontTitle->FontBase = pxOpenGL->Binding.GenLists(glyphsAmount);

                    fontTitle->IsOK = wglUseFontOutlinesW
                    (
                        pxWindow->DeviceContextHandle,
                        glyphsIndexFirst,
                        glyphsAmount,
                        fontTitle->FontBase,
                        maximumChordalDeviation,
                        extrusion,
                        WGL_FONT_POLYGONS,
                        fontTitle->GlyphMetricsFloat
                    ); // Ungodly slow!!
                    PXResult pxResult = PXErrorCurrent(fontTitle->IsOK);

                    if(!fontTitle->IsOK)
                    {
                        break;
                    }
                }
            }

            float offset = 0.02;
            //float scaling = 0.20f;
            //float x = pxTextDrawInfo->X;
            //float y = pxTextDrawInfo->Y;
            //float xSpacer = 3.5;
            
            if(0 == pxTextDrawInfo->Size)
            {
                pxTextDrawInfo->Size = 1;
            }

            //pxOpenGL->Binding.Scalef(0.6, 0.6, 0.1);
            pxOpenGL->Binding.LineWidth(2),
                // pxOpenGL->Binding.Translatef(-1, -1, 0.0f); // Shadow offset

            pxOpenGL->Binding.PushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT);
            //pxOpenGL->Binding.MatrixMode(GL_MODELVIEW);
            //pxOpenGL->Binding.PushMatrix();

           // pxOpenGL->Binding.Translatef(x, y, 0.0f); // Shadow offset
           // pxOpenGL->Binding.Scalef(xScale, yScale, 1.0);
            pxOpenGL->Binding.Color3f(0.2, 0.2, 0.2);
                       
           // PXTextDraw(pxOpenGL, fontTitle, targetText);

            //pxOpenGL->Binding.LoadIdentity();
            /*
            pxOpenGL->Binding.Translatef
            (
                pxTextDrawInfo->X + offset,
                pxTextDrawInfo->Y + offset,
                0.0f
            ); // Main text
            */
           // pxOpenGL->Binding.Scalef(xScale, yScale, 1.0);
            pxOpenGL->Binding.Color3f
            (
                brushText->ColorDate.Red - pxTextDrawInfo->X,
                brushText->ColorDate.Green, 
                brushText->ColorDate.Blue
            );

            hyCol += 0.0008;

            pxOpenGL->Binding.Color3f(PXMathSinusRADF32(hyCol)+1.3, 0.2, 0.2);


            PXTextDraw(pxTextDrawInfo,pxOpenGL, fontTitle, targetText);

            //pxOpenGL->Binding.Translatef(0, 0, 0);
            //pxOpenGL->Binding.PopMatrix();
            pxOpenGL->Binding.PopAttrib();
            //pxOpenGL->Binding.Flush();

            break;
        }

        default:
            break;
    }

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Text",
        "PXID:%-4i X:%-4i Y:%-4i %2i %s",
        pxWindow->Info.ID,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        targetText->SizeUsed,
        targetText->A
    );
#endif
}

PXResult PXAPI PXWindowDrawPoint(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y)
{
#if OSUnix
    const int resultID = 0;//XDrawPoint(Display *display, Drawable d, GC gc, int x, int y);
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXWindowDrawPoints(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
#if OSUnix
    const int resultID = 0;//XDrawPoints(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    return PXActionRefusedNotImplemented;
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXWindowDrawLine(PXWindow PXREF pxWindow, const PXI32S x1, const PXI32S y1, const PXI32S x2, const PXI32S y2)
{
#if OSUnix
    const resultID = 0;//XDrawLine(Display *display, Drawable d, GC gc, x1, y1, x2, y2);
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows

    POINT point;

    // Move to a start point, no drawing happens here
    const BOOL successMove = MoveToEx(pxWindow->DeviceContextHandle, x1, y1, &point);

    // use the previous step to draw a line from there to here
    const BOOL successDraw = LineTo(pxWindow->DeviceContextHandle, x2, y2);

#else
    return PXNotSupport;
#endif
}

PXResult PXAPI PXWindowDrawLines(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
#if OSUnix
    XPoint points;
    int npoints = 0;
    int mode = 0;

    const resultID = XDrawLines
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxWindow->Info.Handle.WindowHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        &points,
        npoints,
        mode
    );
    const PXResult result = PXWindowErrorFromXSystem(resultID);
    return result;
#elif OSWindows
    const BOOL success = PolylineTo(pxWindow->DeviceContextHandle, 0, 0);
#else
    return PXNotSupport;
#endif
}

PXResult PXAPI PXWindowDrawRectangle(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawRectangleInfo)
{
    PXGraphic* pxGraphic = PXGraphicInstantiateGET();
    PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;

    //  PXGUI

    PXGUITheme* pxGUITheme = PXGUIThemeGet();
    PXBrush* brushMainPrimary = pxGUITheme->BrushMainPrimary;
    PXBrush* brushMainSecoundary = pxGUITheme->BrushMainSecoundary;

    PXResult pxResult = PXResultInvalid;

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
#if OSUnix
            const int resultID = XFillRectangle
            (
                pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
                pxWindow->Info.Handle.WindowHandle,
                pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
                x,
                y,
                width,
                height
            );
            pxResult = PXResultOK;// PXWindowErrorFromXSystem(resultID);
#elif OSWindows

            PXResult pxResult;

            RECT* rect = (RECT*)&pxWindowDrawRectangleInfo->RectangleXYWH;

            if(pxWindowDrawRectangleInfo->Rounded)
            {
                const BOOL success = RoundRect
                (
                    pxWindow->DeviceContextHandle,
                    rect->left,
                    rect->top,
                    rect->right,
                    rect->bottom,
                    3,
                    3
                );
                pxResult = PXErrorCurrent(success);
            }
            else
            {
                const int resultID = FillRect
                (
                    pxWindow->DeviceContextHandle,
                    rect,
                    pxWindowDrawRectangleInfo->Brush->BrushHandle // TODO: !!!
                ); // Windows 2000, User32.dll, winuser.h 
                pxResult = PXErrorCurrent(0 != resultID);
            }
#endif

            // const BOOL success = 1; Rectangle(pxWindow->DeviceContextHandle, x, y, width, height);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXWindowTextText,
                PXRectangleText,
                "<PXID:%-4i> L:%-4i T:%-4i R:%-4i B:%-4i",
                pxWindow->Info.ID,
                pxWindowDrawRectangleInfo->RectangleXYWH.X,
                pxWindowDrawRectangleInfo->RectangleXYWH.Y,
                pxWindowDrawRectangleInfo->RectangleXYWH.Width,
                pxWindowDrawRectangleInfo->RectangleXYWH.Height
            );
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            PXRectangleVertexF32 pxRectangleVertexF32 =
            {
                -1 + pxWindowDrawRectangleInfo->RectangleXYWH.X,
                -1 + pxWindowDrawRectangleInfo->RectangleXYWH.Y,
                -1 + (pxWindowDrawRectangleInfo->RectangleXYWH.Width / (PXF32)pxWindow->Position.Form.Width) * 2,
                -1 + (pxWindowDrawRectangleInfo->RectangleXYWH.Height / (PXF32)pxWindow->Position.Form.Height) * 2
            };

            PXGUIProperty pxGUIProperty;
            pxGUIProperty.Property = PXUIElementPropertySize;
            PXGUIPropertyFetch(pxWindow, &pxGUIProperty, 1, PXFalse);

            //  PXRectangleXYWHI32ToVertex(&pxRectangleXYWHI32, &pxGUIProperty.Size, &pxRectangleVertexF32);

              //glEnable(GL_BLEND);
              //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            pxOpenGL->Binding.Enable(GL_LINE_SMOOTH);
            pxOpenGL->Binding.Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);


            // Draw gradient background
            pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            pxOpenGL->Binding.Begin(GL_QUADS);
            pxOpenGL->Binding.Color3bv(&brushMainPrimary->ColorDate);
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.BX, pxRectangleVertexF32.BY); // +1.0f - offset, 1.0f
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.AX, pxRectangleVertexF32.BY); // -1.0f + offset, 1.0f
            pxOpenGL->Binding.Color3bv(&brushMainSecoundary->ColorDate);
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.AX, pxRectangleVertexF32.AY); // -1.0f + offset, -1.0f
            pxOpenGL->Binding.Vertex2f(pxRectangleVertexF32.BX, pxRectangleVertexF32.AY); // +1.0f - offset, -1.0f
            pxOpenGL->Binding.End();

            pxOpenGL->Binding.LineWidth(1);


#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWindowTextText,
                PXRectangleText,
                "<PXID:%-4i> X1:%-4i Y1:%-4i X2:%-4i Y2:%-4i",
                pxWindow->Info.ID,
                pxRectangleVertexF32.AX,
                pxRectangleVertexF32.AY,
                pxRectangleVertexF32.BX,
                pxRectangleVertexF32.BY
            );
#endif

            break;
        }
        default:
            return PXResultInvalid;
    }

    return pxResult;
}

PXResult PXAPI PXWindowDrawIcon(PXWindow PXREF pxWindow, PXIcon PXREF pxIcon, const int x, const int y, const int width, const int height)
{
    if(!(pxWindow && pxIcon))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    const BOOL result = DrawIconEx
    (
        pxWindow->DeviceContextHandle,
        x,
        y,
        pxIcon->IconHandle,
        width,
        height,
        0,
        0,
        DI_NORMAL
    );

    return PXResultOK;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXWindowMouseTrack(PXWindow PXREF window)
{
#if OSUnix
#elif OSWindows
    TRACKMOUSEEVENT trackMouseEvent;
    trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
    trackMouseEvent.hwndTrack = window->WindowHandle;
    trackMouseEvent.dwHoverTime = HOVER_DEFAULT;

    const PXBool rs = TrackMouseEvent(&trackMouseEvent);
#endif

    return PXResultOK;
}


#include <dinput.h>


LPDIRECTINPUT8 g_di = NULL;
LPDIRECTINPUTDEVICE8 g_wheel = NULL;

BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* inst, VOID* ctx) {
    if(inst->dwDevType & DI8DEVTYPE_DRIVING) {
        IDirectInput8_CreateDevice(g_di, &inst->guidInstance, &g_wheel, NULL);
        return DIENUM_STOP;
    }
    return DIENUM_CONTINUE;
}

void InitDirectInput(HWND hwnd) {
    DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                       &IID_IDirectInput8, (void**)&g_di, NULL);

    IDirectInput8_EnumDevices(g_di, DI8DEVCLASS_GAMECTRL,
                              EnumDevicesCallback, NULL, DIEDFL_ATTACHEDONLY);

    if(g_wheel) {
        IDirectInputDevice8_SetDataFormat(g_wheel, &c_dfDIJoystick2);
        IDirectInputDevice8_SetCooperativeLevel(g_wheel, hwnd,
                                                DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        IDirectInputDevice8_Acquire(g_wheel);
    }
}



PXResult PXAPI PXWindowMouseMovementEnable(PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
#if PXOSWindowsDestop && WindowsAtleastXP
    RAWINPUTDEVICE rawInputDeviceList[2];

    // Keyboard
    // rawInputDeviceList[0].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    // rawInputDeviceList[0].usUsage = 0x03;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[0].usUsagePage = 0x01;
    rawInputDeviceList[0].usUsage = 0x06;
    rawInputDeviceList[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rawInputDeviceList[0].hwndTarget = pxWindow->WindowHandle;

    // Mouse
    // rawInputDeviceList[1].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    // rawInputDeviceList[1].usUsage = 0x02;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[1].usUsagePage = 0x01;
    rawInputDeviceList[1].usUsage = 0x02;
    rawInputDeviceList[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rawInputDeviceList[1].hwndTarget = pxWindow->WindowHandle;

    // WARNING
    // RegisterRawInputDevices should not be used from a library!
    // As it may interfere with any raw input processing logic already present in applications that load it.
    const BOOL regsiterRawImputSuccess = RegisterRawInputDevices(rawInputDeviceList, 2, sizeof(RAWINPUTDEVICE)); // Windows XP, User32.dll, winuser.h
    const PXResult pxActionResult = PXErrorCurrent(regsiterRawImputSuccess);

    if(PXResultOK != pxActionResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXWindowTextText,
            "Input",
            "Failed to registerd device for <0x%p>",
            pxWindow
        );
#endif

        return pxActionResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Input",
        "Registerd device for <0x%p>",
        pxWindow
    );
#endif










    return PXResultOK;

#else
    return PXActionRefusedNotSupported;
#endif

    return PXActionRefusedNotSupportedByOperatingSystem;

#endif
}



PXResult PXAPI PXWindowCursorCaptureMode(PXWindow PXREF pxWindow, const PXWindowCursorMode cursorMode)
{
#if OSUnix
#elif PXOSWindowsDestop

    switch(cursorMode)
    {
        case PXWindowCursorShow:
        {
            //printf("[Cursor] Show\n");

            while(ShowCursor(1) < 0);

            //const PXBool clipResult = ClipCursor(NULL);
            //const HCURSOR cursorSet = SetCursor(pxWindowID);

            break;
        }
        case PXWindowCursorLock:
        {
            // printf("[Cursor] Lock\n");

            // Capture cursor
            {
                RECT clipRect;
                GetClientRect(pxWindow->WindowHandle, &clipRect);
                ClientToScreen(pxWindow->WindowHandle, (POINT*)&clipRect.left);
                ClientToScreen(pxWindow->WindowHandle, (POINT*)&clipRect.right);
                ClipCursor(&clipRect);
            }

            break;
        }
        case PXWindowCursorLockAndHide:
        {
            //printf("[Cursor] Lock and hide\n");

            while(ShowCursor(0) >= 0);

            {
                RECT clipRect;
                GetClientRect(pxWindow->WindowHandle, &clipRect);
                ClientToScreen(pxWindow->WindowHandle, (POINT*)&clipRect.left);
                ClientToScreen(pxWindow->WindowHandle, (POINT*)&clipRect.right);

                int xOff = (clipRect.right - clipRect.left) / 2;
                int yoFf = (clipRect.bottom - clipRect.top) / 2;

                clipRect.left += xOff;
                clipRect.top += yoFf;
                clipRect.right -= xOff;
                clipRect.bottom -= yoFf;

                unsigned char sucessClip = ClipCursor(&clipRect);
            }


            CURSORINFO cursorInfo;
            cursorInfo.cbSize = sizeof(CURSORINFO);

            const unsigned char sucessfulInfoGet = GetCursorInfo(&cursorInfo);

            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

            CONSOLE_CURSOR_INFO lpCursor;
            lpCursor.bVisible = 0;
            lpCursor.dwSize = sizeof(CONSOLE_CURSOR_INFO);
            unsigned char y = SetConsoleCursorInfo(console, &lpCursor);

            break;
        }
    }

    //window->CursorModeCurrent = cursorMode;
#endif
}

PXResult PXAPI PXWindowCursorPositionGet(PXWindow PXREF pxWindow, PXVector2I32S PXREF position)
{
    PXResult pxResult = PXResultInvalid;

    if(pxWindow->WindowHandle)
    {
        // We have a target window, 

#if OSUnix


#elif PXOSWindowsDestop
        POINT* point = (POINT*)position; // Illegal cast

        const BOOL sucessful = ScreenToClient(pxWindow->WindowHandle, point);  // are now relative to hwnd's client area
        pxResult = PXErrorCurrent(sucessful);
#endif
    }
    else
    {
#if OSUnix


#elif PXOSWindowsDestop

#if WindowsAtleastVista
        POINT* point = (POINT*)position; // Illegal cast

        const BOOL successful = GetPhysicalCursorPos(point); // Windows Vista, User32.dll, winuser.h
        pxResult = PXErrorCurrent(successful);
#else

        const PXSize mouseMovePointSize = sizeof(MOUSEMOVEPOINT);
        const int nVirtualWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
        const int nVirtualHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
        const int nVirtualLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
        const int nVirtualTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
        const int mode = GMMP_USE_DISPLAY_POINTS;
        MOUSEMOVEPOINT mp_in;
        MOUSEMOVEPOINT mp_out[64];

        PXMemoryClear(&mp_in, mouseMovePointSize);

        mp_in.x = 0x0000FFFF;//Ensure that this number will pass through.
        mp_in.y = 0x0000FFFF;
        const int numberOfPoints = GetMouseMovePointsEx(mouseMovePointSize, &mp_in, &mp_out, 64, mode); // Windows 2000, User32.dll, winuser.h
        const PXBool success = numberOfPoints > 0;

        if(!success)
        {
            return PXFalse;
        }

        /*
        for (PXSize i = 0; i < (PXSize)numberOfPoints; ++i)
        {
            switch (mode)
            {
            case GMMP_USE_DISPLAY_POINTS:
            {

                if (mp_out[i].x > 32767)
                    mp_out[i].x -= 65536;
                if (mp_out[i].y > 32767)
                    mp_out[i].y -= 65536;
                break;
            }
            case GMMP_USE_HIGH_RESOLUTION_POINTS:
            {
                mp_out[i].x = ((mp_out[i].x * (nVirtualWidth - 1)) - (nVirtualLeft * 65536)) / nVirtualWidth;
                mp_out[i].y = ((mp_out[i].y * (nVirtualHeight - 1)) - (nVirtualTop * 65536)) / nVirtualHeight;
                break;
            }
            }

            *x = mp_out[i].x;
            *y = mp_out[i].y;

            break;
        }*/

        return PXTrue;

#endif

#endif
    }

    return pxResult;
}

PXResult PXAPI PXWindowText(PXWindow PXREF pxWindow, PXText PXREF pxText, const PXBool doWrite)
{
    PXResult pxActionResult = PXResultInvalid;

#if OSUnix
    pxActionResult = PXActionRefusedNotImplemented;
#elif OSWindows

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            if(doWrite)
            {
                BOOL result = SetWindowTextA
                (
                    pxWindow->WindowHandle,
                    pxText->A
                );
            }
            else
            {
                pxText->SizeUsed = GetWindowTextA
                (
                    pxWindow->WindowHandle,
                    pxText->A,
                    pxText->SizeAllocated
                );
            }

            break;
        }
        case TextFormatUNICODE:
        {
            if(doWrite)
            {
                BOOL result = SetWindowTextW
                (
                    pxWindow->WindowHandle,
                    pxText->W
                );
            }
            else
            {
                pxText->SizeUsed = GetWindowTextW
                (
                    pxWindow->WindowHandle,
                    pxText->W,
                    pxText->SizeAllocated
                );
            }

            break;
        }

        default:
            break;
    }

#else
    pxActionResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxActionResult;
}

PXResult PXAPI PXWindowCreate(PXWindow** pxWindowREF, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    /*
        PXWindowCreateInfo pxWindowCreateInfo;
    PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
    pxWindowCreateInfo.StyleFlags = 
        asChild ? (WS_CHILD | WS_VISIBLE | CS_OWNDC | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
        : (WS_OVERLAPPEDWINDOW);
     // p->ownerMain; // parent for child; owner for top-level

    pxWindowCreateInfo.WindowParent = pxWindowParant;
    pxWindowCreateInfo.WindowClassName.A = PXWindowClassName;
    pxWindowCreateInfo.Position.Form.X = CW_USEDEFAULT;
    pxWindowCreateInfo.Position.Form.Y = CW_USEDEFAULT;
    pxWindowCreateInfo.Position.Form.Width = 320;
    pxWindowCreateInfo.Position.Form.Height = 320;
    
    */

    PXResult result = PXResultInvalid;

    if(!(pxWindowREF && pxWindowCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXWindow* pxWindow = *pxWindowREF;
    PXWindow* pxWindowParent = pxWindowCreateInfo->WindowParent;

    char name[32];

    if(!pxWindow)
    {
        pxWindowCreateInfo->Info.Static = &PXWindowRegisterInfoStatic;
        pxWindowCreateInfo->Info.Dynamic = &PXWindowRegisterInfoDynamic;
        PXResult pxResult = PXECSCreate(pxWindowREF, pxWindowCreateInfo);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }

        pxWindow = *pxWindowREF;
        pxWindow->WindowParent = pxWindowParent;

        if(PXGraphicSystemInvalid == pxWindow->GraphicSystem)
        {
            pxWindow->GraphicSystem = PXGraphicSystemNative;
        }

        // These settings are only valid on startup, so we set them here ONCE
        pxWindow->Type = pxWindowCreateInfo->Type;
        pxWindow->EventList = pxWindowCreateInfo->EventList;
        pxWindow->Info.Behaviour |= pxWindowCreateInfo->Behaviour;
        pxWindow->Info.Setting |= pxWindowCreateInfo->Setting;
        pxWindow->BackGroundColor = pxWindowCreateInfo->BackGroundColor;
        pxWindow->headerH = 28;

        PXTextFromAdressA(&pxWindowCreateInfo->WindowClassName, PXWindowClassName, sizeof(PXWindowClassName), sizeof(PXWindowClassName));    
    }

    // Add title if not set
    if(!pxWindowCreateInfo->WindowText.SizeUsed)
    {
        PXTextFromAdressA(&pxWindowCreateInfo->WindowText, name, 0, sizeof(name));
        PXTextPrint(&pxWindowCreateInfo->WindowText, "PX-Window_%i", pxWindow->Info.ID);
    }


    pxWindow->floating = pxWindowCreateInfo->floating;
    pxWindow->dockSide = pxWindowCreateInfo->dockSide;
    //pxWindow->Child

    if(!pxWindow->EventList.CallBackOwner && pxWindow->EventList.CallBackDraw)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXWindowTextText,
            "Create",
            "PXID:%i, %s, Custom draw set but no event callback owner! Invalid call!",
            pxWindow->Info.ID,
            pxWindowCreateInfo->Info.Static->NameOfType.A
        );
#endif
    }

    pxWindowCreateInfo->StyleFlagsExtended |= WS_EX_WINDOWEDGE | WS_EX_COMPOSITED;

    if(pxWindowParent)
    {
        // If we want to put this as a subwindow, we need the handle of the parranet
        pxWindowCreateInfo->InstanceHandle = (HINSTANCE)GetWindowLongPtr
        (
            pxWindowParent->WindowHandle,
            GWLP_HINSTANCE
        );

        pxWindowCreateInfo->StyleFlags |=
            WS_CHILD |
            CS_OWNDC |
            WS_CLIPSIBLINGS |
            WS_CLIPCHILDREN;
    }
    else
    {
        // Get the current module context
        pxWindowCreateInfo->InstanceHandle = GetModuleHandle(NULL);

        pxWindowCreateInfo->StyleFlags |= WS_OVERLAPPEDWINDOW;
    }


    // Precalculate and preo all variables.


    // Normally, we want to create a physical window from the OS.
    // But.. if we want to spawn our own buttons and elements, we can also create a "fake" one.
    // This decreses the amount of "managing" code and lets the UI scale way better.
    // For this fake window, we just create a framebuffer
    const PXBool createVirtualWindow = pxWindowCreateInfo->IsVirtual;

    if(createVirtualWindow)
    {
        PXFrameBufferCreateInfo pxFrameBufferCreateInfo;
        PXClear(PXFrameBufferCreateInfo, &pxFrameBufferCreateInfo);
        pxFrameBufferCreateInfo.System = PXGraphicSystemNative;
        pxFrameBufferCreateInfo.Width = pxWindowCreateInfo->Size.Width;
        pxFrameBufferCreateInfo.Height = pxWindowCreateInfo->Size.Height;

        const PXResult pxResult = PXFrameBufferCreate(&pxWindow->FrameBuffer, &pxFrameBufferCreateInfo);

        return pxResult;
    }

    // Override parent ONLY when we set one
    //if(pxWindowParent)
    {
      //  pxWindow->Info.Hierarchy.Parrent = &pxWindowParent->Info.Hierarchy; // Set parrent

        // Check if the parrent is itself?
        //pxWindowParent->Info.Hierarchy.Yourself = pxWindowParent;
    }

 
    PXCopy(PXUIElementPosition, &pxWindowCreateInfo->Position, &pxWindow->Position);


    // Resize

    PXGUIProperty pxWindowSizeInfo;
    PXClear(PXGUIProperty, &pxWindowSizeInfo);
    //sizeInfoAA.UIElement = *pxWindowCreateInfo->UIElement;
    //pxWindowSizeInfo.WindowParent = pxWindowParent; // TODO: Is this a parent??
    pxWindowSizeInfo.Property = PXUIElementPropertySizeParent;

    PXWindowPositionCalulcateInfo pxWindowPositionInfo;
    PXClear(PXWindowPositionCalulcateInfo, &pxWindowPositionInfo);

    // PXWindowSizeInfo pxWindowSizeInfo;

    // PXWindowID windowID = pxWindowCreateInfo->UIElementWindow ? pxWindowCreateInfo->UIElementWindow->ID : PXNull;

    //  PXWindowSizeGet(windowID, &pxWindowSizeInfo);

    PXGUIPropertyFetch(pxWindow, &pxWindowSizeInfo, 1, 0);

    pxWindowPositionInfo.WindowWidth = pxWindowSizeInfo.Size.Width;
    pxWindowPositionInfo.WindowHeight = pxWindowSizeInfo.Size.Height;

    if(pxWindowPositionInfo.WindowWidth == 0)
    {
        pxWindowPositionInfo.WindowWidth = 400;
        pxWindowPositionInfo.WindowHeight = 500;
    }

    PXWindowPositionCalculcate(pxWindow, &pxWindowPositionInfo);

    if(pxWindowParent)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXWindowTextText,
            "Create",
            "Attempting to create window with parent..\n"
            "%10s - [PX-ID:%i], X:%4i Y:%4i W:%4i H:%4i, Name:%s\n"
            "%10s - [PX-ID:%i], X:%4i Y:%4i W:%4i H:%4i, Name:%s",
            "Self",
            pxWindow->Info.ID,
            (int)pxWindowPositionInfo.Size.X,
            (int)pxWindowPositionInfo.Size.Y,
            (int)pxWindowPositionInfo.Size.Width,
            (int)pxWindowPositionInfo.Size.Height,
            pxWindowCreateInfo->WindowText.A,
            "Parent",
            pxWindowParent->Info.ID,
            pxWindowParent->Position.Form.X,
            pxWindowParent->Position.Form.Y,
            pxWindowParent->Position.Form.Width,
            pxWindowParent->Position.Form.Height,
            "---"
        );
#endif
    }
    else
    {
        // Calc size
        {
            const PXBool isDefaultSize = 1;// windowInfo->Width == 0 && windowInfo->Height == 0;

            if(isDefaultSize)
            {
                PXRectangleXYWHI32 pxRectangleXYWH;

                PXNativDrawScreenSizeGet(&pxRectangleXYWH);

                pxWindowPositionInfo.Size.X = pxRectangleXYWH.Width * 0.125f;
                pxWindowPositionInfo.Size.Y = pxRectangleXYWH.Height * 0.125f;
                pxWindowPositionInfo.Size.Width = pxRectangleXYWH.Width * 0.75f;
                pxWindowPositionInfo.Size.Height = pxRectangleXYWH.Height * 0.75f;
            }
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXWindowTextText,
            "Create",
            "Attempting to create global window..\n"
            "%10s PX-ID:%i, X:%4i Y:%4i W:%4i H:%4i, Name:%s (%i)",
            "",
            pxWindow->Info.ID,
            (int)pxWindowPositionInfo.Size.X,
            (int)pxWindowPositionInfo.Size.Y,
            (int)pxWindowPositionInfo.Size.Width,
            (int)pxWindowPositionInfo.Size.Height,
            pxWindowCreateInfo->WindowText.A,
            pxWindowCreateInfo->WindowText.SizeUsed
        );
#endif
    }

    //-----------------------------------------------------
    // Setup flags and draw function callbacks
    //-----------------------------------------------------
#if OSWindows
    pxWindowCreateInfo->StyleFlags |= WS_CLIPCHILDREN | SS_NOTIFY; // WS_CLIPSIBLINGS

    if(pxWindowCreateInfo->Invisible)
    {
        pxWindowCreateInfo->StyleFlags &= ~WS_VISIBLE;
    }
    else
    {
        pxWindowCreateInfo->StyleFlags |= WS_VISIBLE;
    }

    if(PXWindowBehaviourBorder & pxWindowCreateInfo->Setting)
    {
        pxWindowCreateInfo->StyleFlags |= WS_BORDER;
    }

    if(pxWindowParent)
    {
        pxWindowCreateInfo->StyleFlags |= WS_CHILD;
    }

    if(PXWindowBehaviourSelectable & pxWindowCreateInfo->Setting)
    {
        pxWindowCreateInfo->StyleFlags |= WS_TABSTOP;
    }
#endif

    // Default
#if OSWindows
    if(!pxWindowCreateInfo->WindowClassName.A)
    {
        pxWindowCreateInfo->WindowClassName.A = WC_STATIC;
    }
#endif

    // TODO: wanky ovverride
    //pxWindowCreateInfo->WindowsStyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;


    /*

    pxWindowCreateInfo->WindowClassName.A = WC_IPADDRESS;
    pxWindowCreateInfo->WindowClassName.A = "SysLink"; // WC_LINK
#if OSWindows
    // pxUIElementCreateData->WindowsClassName = WC_IMAGE;
//pxUIElementCreateData->WindowsStyleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
    pxWindowCreateInfo->WindowClassName.A = WC_LISTBOX;
#endif

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_EDIT;
    pxWindowCreateInfo->WindowsStyleFlags |= ES_MULTILINE;
    pxWindowCreateInfo->WindowText.A = pxWindowCreateInfo->Button.TextInfo.Content;
#endif
    pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawText;

    pxWindowCreateInfo->WindowClassName.A = "RICHEDIT_CLASS";

    pxWindowCreateInfo->WindowClassName.A = WC_SCROLLBAR;

    pxWindowCreateInfo->WindowClassName.A = TRACKBAR_CLASS;
    pxWindowCreateInfo->WindowsStyleFlags |= TBS_AUTOTICKS | TBS_TOOLTIPS;

    pxWindowCreateInfo->WindowClassName.A = STATUSCLASSNAMEA;
    //pxWindowCreateInfo->WindowsStyleFlags |= SBARS_SIZEGRIP;


    pxWindowCreateInfo->WindowClassName.A = WC_STATIC;

    pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawFooter;

    pxWindow->Info.Behaviour &= ~PXECSInfoUseByMask;
    pxWindow->Info.Behaviour |= PXECSInfoUseByEngine;


    pxWindowCreateInfo->WindowClassName.A = UPDOWN_CLASS;
    pxWindowCreateInfo->WindowClassName.A = HOTKEY_CLASS;
    pxWindowCreateInfo->WindowClassName.A = ANIMATE_CLASS;




                //PXWindowCreateWindowInfo PXREF windowInfo = &pxWindowCreateInfo->Data.Window;

#if OSWindows
            pxWindowCreateInfo->WindowClassName.A = PXWindowName;
            pxWindowCreateInfo->WindowClassName.SizeUsed = PXWindowNameLength;


            pxWindowCreateInfo->WindowsWindowsStyleFlagsExtended =
                WS_EX_APPWINDOW |
                WS_EX_DLGMODALFRAME |
                WS_EX_CONTEXTHELP;

            pxWindowCreateInfo->WindowsStyleFlags |=
                WS_OVERLAPPEDWINDOW |
                CS_OWNDC |
                CS_HREDRAW |
                CS_VREDRAW;

            // PXWindowHandle windowID = 0;

            DWORD dwStyle = 0;
            HWND hWndParent = 0;
            HMENU hMenu = 0;
            void* lpParam = 0;

            // HICON       hIcon =
            // HCURSOR     hCursor = pxWindow->CursorID;
            // HBRUSH      hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); //(HBRUSH)GetStockObject(COLOR_BACKGROUND);

            //  HBRUSH hbrBackground =

            /*
            // Cursor setup
            {
               // const HCURSOR cursorID = LoadCursor(hInstance, IDC_ARROW);
                //pxWindow->CursorID = cursorID;

                if(pxWindow->Mode == PXWindowModeNormal)
                {
                    const int xx = WS_OVERLAPPEDWINDOW;

                    dwStyle |= WS_OVERLAPPEDWINDOW;// | WS_VISIBLE;
                }
            }
            * /

            // Registering of class


#endif
    */

    if(!pxWindowParent)
    {
        if(!_pxEngineBaseClassID)
        {
            WNDCLASSA wndclass;

            PXClear(WNDCLASSA, &wndclass);

            COLORREF color = RGB(0x20, 0x20, 0x20);// RGB(windowInfo->BackGroundColor.Red, windowInfo->BackGroundColor.Green, windowInfo->BackGroundColor.Blue)

            wndclass.style = CS_DBLCLKS | CS_OWNDC;; // Can't apply
            wndclass.lpfnWndProc = PXWindowEventCallBack;
            wndclass.cbClsExtra = 0;
            wndclass.cbWndExtra = 0;
            wndclass.hInstance = pxWindowCreateInfo->InstanceHandle;
            wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
            wndclass.hCursor = PXNull;
            wndclass.hbrBackground = CreateSolidBrush(color);
            wndclass.lpszMenuName = 0;
            // wndclass.lpszClassName = pxWindowCreateInfo->WindowClassName.A;
            wndclass.lpszClassName = PXWindowClassName;

            _pxEngineBaseClassID = RegisterClassA(&wndclass);
            const PXResult result = PXErrorCurrent(0 != _pxEngineBaseClassID);
        }       

        pxWindowCreateInfo->WindowClassName.A = (char*)_pxEngineBaseClassID; 
        pxWindowCreateInfo->StyleFlags |= WS_OVERLAPPEDWINDOW;
    }


    // If we hav
    {
        pxWindow->Info.Behaviour &= ~PXECSInfoUseByMask;
        pxWindow->Info.Behaviour |= PXECSInfoUseByEngine;

#if OSWindows
        if(pxWindow->EventList.CallBackDraw)
        {
            DWORD magicID = 0;

            switch(pxWindow->Type)
            {
                case PXUIElementTypeButton:
                    magicID = BS_OWNERDRAW;
                    break;

                case PXUIElementTypeRenderFrame:
                case PXUIElementTypeColorPicker:
                case PXUIElementTypePanel:
                default: // TODO: problem with default value, we cant detect if we have the wrong enum type
                    magicID = SS_OWNERDRAW | SS_BLACKRECT;
                    break;
            }

            pxWindowCreateInfo->StyleFlags |= magicID;
        }
#endif // OSWindows
    }

#if OSUnix

    pxWindowCreateInfo->Simple = PXTrue;
    pxWindowCreateInfo->X = pxUIElementPositionCalulcateInfo.X;
    pxWindowCreateInfo->Y = pxUIElementPositionCalulcateInfo.Y;
    pxWindowCreateInfo->Width = pxUIElementPositionCalulcateInfo.Width;
    pxWindowCreateInfo->Height = pxUIElementPositionCalulcateInfo.Height;
    pxWindowCreateInfo->BorderWidth = 1;
    pxWindowCreateInfo->Border = 1;

    PXNativDrawWindowCreate(PXNativDrawInstantance(), pxWindowCurrent, pxWindowCreateInfo);


#if 0 // Grab means literally Drag%Drop grab. This is not mouse motion
    //bool   ret    = false;
    XID cursor = XCreateFontCursor(display, XC_crosshair);
    int    root = DefaultRootWindow(display);

    const int grabResult = XGrabPointer
    (
        display,
        root,
        0,
        ButtonMotionMask,
        GrabModeAsync,
        GrabModeAsync,
        root,
        None,
        CurrentTime
    );
#endif


    // Raw mouse movement
    XIEventMask eventmask;
    PXClear(XIEventMask, &eventmask);

    const PXSize maskLength = (XI_LASTEVENT + 7) / 8;
    char mask[maskLength];

    PXClearList(char, mask, maskLength);
    XISetMask(mask, XI_RawMotion);
    XISetMask(mask, XI_RawButtonPress);
    XISetMask(mask, XI_RawKeyPress);

    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = maskLength;
    eventmask.mask = mask;


    const int selectResultID = XISelectEvents(_pxGUIManager.DisplayCurrent.DisplayHandle, _pxGUIManager.DisplayCurrent.WindowRootHandle, &eventmask, 1u);
    const PXBool success = PXNull != selectResultID;

    if(!success)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "X-System",
            "Event-Select",
            "Failed: Display:<%p>, Root:<%i>, Mask:%8x",
            _pxGUIManager.DisplayCurrent.DisplayHandle,
            _pxGUIManager.DisplayCurrent.WindowRootHandle,
            mask
        );
#endif
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "X-System",
            "Event-Select",
            "ID:%i OK",
            pxWindowCurrent->Info.Handle.WindowHandle
        );
#endif
    }


#elif OSWindows

    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
    // pxWindow->Type == PXUIElementTypeText&&;

    if(!pxWindowCreateInfo->AvoidCreation)
    {
        PXHierarchicalNode* pxHierarchicalNode = 0;// pxWindow->Info.Hierarchy.Parrent;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXWindowTextText,
            "Window",
            "PXID:%i, Creating...",
            pxWindow->Info.ID
        );
#endif

#if OSUnix
        Display* displayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;
        const int screenHandle = DefaultScreen(displayHandle);

        // if we dont have a parent, we need to use the root window
        if(!pxWindowParentHandle)
        {
            pxWindowParentHandle = RootWindow(displayHandle, screenHandle);
        }

#if 0
        const int attributeList[] =
        {
            GLX_RGBA,
            GLX_DEPTH_SIZE,
            24,
            GLX_DOUBLEBUFFER,
            None
        };

        const XVisualInfo PXREF visualInfo = glXChooseVisual(displayHandle, screenHandle, attributeList);

        {
            const PXBool successful = visualInfo != 0;

            if(!successful)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "X-System",
                    "Visual",
                    "Choosing failed"
                );
#endif

                return PXResultOK; // no appropriate visual found
            }


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "X-System",
                "Visual",
                "OK, (%p)",
                visualInfo
            );
#endif

        }


        // default color map?

        // Create colormapping
        Colormap colormap = XCreateColormap
        (
            displayHandle,
            pxWindowParentHandle,
            visualInfo->visual,
            AllocNone
        );
#endif

        // Create window
        {
            if(pxWindowCreateInfo->Simple)
            {
                pxWindow->Info.Handle.WindowHandle = XCreateSimpleWindow
                (
                    displayHandle,
                    pxWindowParentHandle,
                    pxWindowCreateInfo->X,
                    pxWindowCreateInfo->Y,
                    pxWindowCreateInfo->Width,
                    pxWindowCreateInfo->Height,
                    pxWindowCreateInfo->BorderWidth,
                    WhitePixel(displayHandle, screenHandle),
                    BlackPixel(displayHandle, screenHandle)
                );

                if(pxWindow->Info.Handle.WindowHandle)
                {
                    // Create events
                  //  XSelectInput(displayHandle, pxWindow->Info.Handle.WindowHandle, inputEventsToListen);
                }
            }
            else
            {
                pxWindow->Info.Handle.WindowHandle = XCreateWindow
                (
                    displayHandle,
                    pxWindowParentHandle,
                    pxWindowCreateInfo->X,
                    pxWindowCreateInfo->Y,
                    pxWindowCreateInfo->Width,
                    pxWindowCreateInfo->Height,
                    pxWindowCreateInfo->BorderWidth,
                    0, //visualInfo->depth,
                    0,//InputOutput,
                    0,//visualInfo->visual,
                    CWColormap | CWEventMask,
                    0//&setWindowAttributes
                );
            }

            const PXBool successful = PXNull != pxWindow->Info.Handle.WindowHandle;

            if(!successful)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "X-System",
                    "Window-Create",
                    "Failed: ID:<%i>",
                    pxWindow->Info.ID
                );
#endif

                return PXResultInvalid;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "X-System",
                "Window-Create",
                "OK, ID:%i, X11-ID:<0x%8.8x>\n"
                "%15s : %i,%i\n"
                "%15s : %ix%i\n"
                "%15s : %i\n"
                "%15s : %i",
                pxWindow->Info.ID,
                pxWindow->Info.Handle.WindowHandle,
                "Position", (int)pxWindowCreateInfo->X, (int)pxWindowCreateInfo->Y,
                "Size", (int)pxWindowCreateInfo->Width, (int)pxWindowCreateInfo->Height,
                "BorderWidth", pxWindowCreateInfo->BorderWidth,
                "Depth", 0//visualInfo->depth
            );
#endif
        }

        // const int flushResultID = XFlush(displayHandle);


         // Add callbacks
        const unsigned long inputEventsToListen =
            KeyPressMask |
            //    KeyReleaseMask |
            //    ButtonPressMask |
             //   ButtonReleaseMask |
             //   EnterWindowMask |
             //   LeaveWindowMask |
              //  PointerMotionMask |
              //  PointerMotionHintMask |
             //   Button1MotionMask |
             //   Button2MotionMask |
             //   Button3MotionMask |
             //   Button4MotionMask |
             //   Button5MotionMask |
             //   ButtonMotionMask |
              //  KeymapStateMask |
            ExposureMask |
            //  VisibilityChangeMask |
            //  StructureNotifyMask |
            //  ResizeRedirectMask |
            //  SubstructureNotifyMask |
             // SubstructureRedirectMask |
             // FocusChangeMask |
            //  PropertyChangeMask |
            //  ColormapChangeMask |
             // OwnerGrabButtonMask |
              // XI_RawMotion |
            0;


        //XSetWindowAttributes setWindowAttributes;
        //setWindowAttributes.cursor = ;
       // setWindowAttributes.colormap = colormap;
       // setWindowAttributes.event_mask = inputEventsToListen;


        XSelectInput(displayHandle, pxWindow->Info.Handle.WindowHandle, inputEventsToListen);


        // Attach to render engine
        {
            const int mapResultID = XMapWindow(displayHandle, pxWindow->Info.Handle.WindowHandle);

            // Linux doc states, mapwindow shall be ignored?
#if 0
            const PXBool success = 0 == mapResultID; // Success

            if(!success)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "X-System",
                    "Window-Mapping",
                    "Failed, Display:<%p>, X11-ID:<0x%8.8x>",
                    displayHandle,
                    pxWindow->Info.Handle.WindowHandle
                );
#endif

                return PXResultInvalid;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "X-System",
                "Window-Mapping",
                "OK"
            );
#endif
#endif
        }

        // Set events
        {

        }

        // Set title. Windows does this with the creation function but X11 does not.
        {
            const PXBool setTextSuccess = PXNativDrawTextSet(pxNativDraw, pxWindow, pxWindowCreateInfo->Name, -1);
        }

#elif OSWindows

        HWND parrantHandle = pxWindowParent ? pxWindowParent->WindowHandle : PXNull;

        switch(pxWindowCreateInfo->WindowText.Format)
        {
            case TextFormatASCII:
            {
                pxWindow->WindowHandle = CreateWindowExA // Windows 2000, User32.dll, winuser.h
                (
                    pxWindowCreateInfo->StyleFlagsExtended,
                    pxWindowCreateInfo->WindowClassName.A,
                    pxWindowCreateInfo->WindowText.A, // Text content
                    pxWindowCreateInfo->StyleFlags,
                    pxWindowPositionInfo.Size.X,
                    pxWindowPositionInfo.Size.Y,
                    pxWindowPositionInfo.Size.Width,
                    pxWindowPositionInfo.Size.Height,
                    parrantHandle,
                    PXNull, // No menu.
                    pxWindowCreateInfo->InstanceHandle,
                    pxWindow // payload for userdata
                );
                break;
            }
            case TextFormatUNICODE:
            {
                pxWindow->WindowHandle = CreateWindowExW // Windows 2000, User32.dll, winuser.h
                (
                    pxWindowCreateInfo->StyleFlagsExtended,
                    pxWindowCreateInfo->WindowClassName.A,
                    pxWindowCreateInfo->WindowText.A, // Text content
                    pxWindowCreateInfo->StyleFlags,
                    pxWindowPositionInfo.Size.X,
                    pxWindowPositionInfo.Size.Y,
                    pxWindowPositionInfo.Size.Width,
                    pxWindowPositionInfo.Size.Height,
                    parrantHandle,
                    PXNull, // No menu.
                    pxWindowCreateInfo->InstanceHandle,
                    pxWindow // payload for userdata
                );
                break;
            }
            default:
            {
                return PXResultRefusedParameterInvalid;
            }
        }

        const PXResult createResult = PXErrorCurrent(PXNull != pxWindow->WindowHandle);

        if(PXResultOK != createResult)
        {
            return createResult;
        }

        // Get additional device context for rendering purpose
        pxWindow->DeviceContextHandle = PXWindowDCGet(pxWindow);

#else
        return PXActionRefusedNotSupportedByOperatingSystem;
#endif

        pxWindow->Info.Behaviour |=
            PXECSInfoExist |
            PXECSInfoActive |
            PXECSInfoRender |
            PXECSInfoUseByOS;
        pxWindow->Position.Form = pxWindowCreateInfo->Size;

#if 0
        // Window create, register..
        PXDictionaryEntryAdd
        (
            &pxNativDraw->ResourceManager->GUIElementLookup,
            &pxWindow->WindowHandle,
            pxWindow
        );
#endif

        if(pxWindowParent)
        {
            //PXHierarchicalNodeParent(&pxWindow->Info.Hierarchy, &pxWindowParent->Info.Hierarchy);
        }


#if PXLogEnable && 0
        const char* uielementName = PXUIElementTypeToString(pxWindowCreateInfo->Type);

        PXLogPrint
        (
            PXLoggingError,
            PXNativDraw,
            "Element-Create",
            "Failed: X:%4i, Y:%4i, W:%4i, H:%4i, (%s) : [%s]",
            (int)pxWindowCreateInfo->X,
            (int)pxWindowCreateInfo->Y,
            (int)pxWindowCreateInfo->Width,
            (int)pxWindowCreateInfo->Height,
            uielementName,
#if OSWindows
            pxWindowCreateInfo->WindowsTextContent
#else
            pxWindowCreateInfo->Name
#endif
        );
#endif


        PXWindowTitleBarColor(pxWindow);


        UpdateWindow(pxWindow->WindowHandle);
        ShowWindow(pxWindow->WindowHandle, SW_SHOWNORMAL);


#if 0
        PXWindowOpenGLEnable(pxWindow);
#endif


      // PXWindowDragAndDropBegin(pxWindow);

#if 0
        if(pxWindow->ID && pxWindowCreateInfo->UIElementParent)
        {
            PXWindow* parent = pxWindowCreateInfo->UIElementParent;

            HDWP resA = BeginDeferWindowPos(1);

            if(PXUIElementTypeButtonText == pxWindow->Type)
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    parent->ID,
                    pxWindow->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
                EndDeferWindowPos(resB);
            }
            else
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    pxWindow->ID,
                    parent->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
                EndDeferWindowPos(resB);
            }


        }
#endif
    }

#if 0

    char buffer[255];
    PXClearList(char, buffer, 255);

    PXSize* size = buffer;
    *size = 255;

    COLORREF colorAA = RGB(255, 0, 0);

    HDC xx = GetDC(pxWindow->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORSTATIC, xx, pxWindow->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));


    Pager_SetBkColor(pxWindow->ID, &colorAA);
#endif

    // SetTextColor(xx, colorAA);
    // 
    //LRESULT ww = SendMessageA(pxWindow->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR

    //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);

    // SendMessageA(pxWindow->ID, RB_SETTEXTCOLOR, 0, &colorAA);

#endif


    //-------------------------------------------------------------------------
    // POST-Update
    //-------------------------------------------------------------------------
#if 0
    switch(pxWindow->Type)
    {
        case PXUIElementTypeMenuStrip:
        {
            PXWindowMenuItemList PXREF menuItemListInput = &pxWindowCreateInfo->MenuItem;

            PXWindowExtendedMenuItem* menuItemListOut = PXMemoryHeapCallocT(PXWindowExtendedMenuItem, 1);

            pxWindow->ExtendedData = menuItemListOut;

            // setup extended data
            {
                menuItemListOut->MenuItemAmount = menuItemListInput->MenuItemInfoListAmount;
                menuItemListOut->MenuItemList = PXMemoryHeapCallocT(PXWindowMenuItem, menuItemListInput->MenuItemInfoListAmount);

                for(PXSize i = 0; i < menuItemListInput->MenuItemInfoListAmount; ++i)
                {
                    PXWindowMenuItemInfo PXREF pxWindowMenuItemSource = &menuItemListInput->MenuItemInfoListData[i];
                    PXWindowMenuItem PXREF pxWindowMenuItemTarget = &menuItemListOut->MenuItemList[i];

                    pxWindowMenuItemTarget->TextData = pxWindowMenuItemSource->TextData;
                    pxWindowMenuItemTarget->TextSize = pxWindowMenuItemSource->TextSize;
                }
            }


            switch(PXECSInfoUseByMask & pxWindow->Info.Behaviour)
            {
                case PXECSInfoUseByOS:
                {

#if OSWindows
                    pxWindow->MenuHandle = CreateMenu(); // Windows 2000, User32.dll, winuser.h
                    const PXResult res = PXErrorCurrent(PXNull != pxWindow->MenuHandle);

                    HMENU hSubMenu = CreatePopupMenu();

                    for(PXSize i = 0; i < menuItemListInput->MenuItemInfoListAmount; ++i)
                    {
                        PXWindowMenuItemInfo PXREF pxWindowMenuItemInfo = &menuItemListInput->MenuItemInfoListData[i];

                        MENUITEMINFOA menuItemInfo;
                        PXClear(MENUITEMINFOA, &menuItemInfo);
                        menuItemInfo.cbSize = sizeof(MENUITEMINFOA);
                        menuItemInfo.fMask = MIIM_STRING | MIIM_STATE | MIIM_SUBMENU;
                        menuItemInfo.fType = MFT_STRING;         // used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
                        menuItemInfo.fState = MFS_DEFAULT;        // used if MIIM_STATE
                        menuItemInfo.wID = 0;           // used if MIIM_ID
                        menuItemInfo.hSubMenu = pxWindow->MenuHandle;      // used if MIIM_SUBMENU
                        menuItemInfo.hbmpChecked = 0;   // used if MIIM_CHECKMARKS
                        menuItemInfo.hbmpUnchecked = 0; // used if MIIM_CHECKMARKS
                        menuItemInfo.dwItemData = 0;   // used if MIIM_DATA
                        menuItemInfo.dwTypeData = pxWindowMenuItemInfo->TextData;    // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
                        menuItemInfo.cch = pxWindowMenuItemInfo->TextSize;           // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
                        menuItemInfo.hbmpItem = 0;      // used if MIIM_BITMAP

                        UINT newID = i;

                        const BOOL itemAddResult = AppendMenuA
                        (
                            pxWindow->MenuHandle,
                            menuItemInfo.fState,
                            &newID,
                            menuItemInfo.dwTypeData
                        );
                        const PXResult sdfsdfghg = PXErrorCurrent(itemAddResult);

                        PXWindowMenuItemList* sub = pxWindowMenuItemInfo->ChildList;

                        if(sub)
                        {
                            for(size_t i = 0; i < sub->MenuItemInfoListAmount; i++)
                            {
                                UINT newIEED = newID;

                                PXWindowMenuItemInfo PXREF pxWindowMenuItemInfo = &sub->MenuItemInfoListData[i];

                                const PXBool asddassdad = AppendMenuA(hSubMenu, menuItemInfo.fState | MF_POPUP, &newIEED, pxWindowMenuItemInfo->TextData);
                                const PXResult wewrerew = PXErrorCurrent(asddassdad);
                            }
                        }




                        //  const PXBool itemAddResult = InsertMenuItemA(pxWindow->Info.Handle.MenuHandle, 0, PXTrue, &menuItemInfo);
                        const PXResult res = PXErrorCurrent(itemAddResult);

                        // DrawMenuBar(pxWindow->Parent->Info.Handle.WindowHandle);

                        menuItemInfo.wID += 0;
                    }

                   // PXWindow* parentElement = (PXWindow*)pxWindow->Info.Hierarchy.Parrent;
                    //const PXBool setResult = SetMenu(parentElement->WindowHandle, pxWindow->Info.Handle.MenuHandle);
#endif

                    break;
                }
                case PXECSInfoUseByEngine:
                {

                    // ToDo:

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case PXUIElementTypeFileDirectyView:
        {
            PXDirectorySearchCache* pxDirectorySearchCache = PXMemoryHeapCallocT(PXDirectorySearchCache, 1);

            pxWindow->ExtendedData = pxDirectorySearchCache;

            break;
        }
        case PXUIElementTypeText:
        {
            PXGUIProperty pxUIElementUpdateInfo[2];
            PXClearList(PXGUIProperty, pxUIElementUpdateInfo, 2);

            //pxUIElementUpdateInfo[0].WindowCurrent = pxWindowCurrent;
            //pxUIElementUpdateInfo[0].WindowParent = pxWindowParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;

            //pxUIElementUpdateInfo[1].WindowCurrent = pxWindowCurrent;
            //pxUIElementUpdateInfo[1].WindowParent = pxWindowParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyAllign;

            // PXWindowUpdate(pxGUIManager, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeButton:
        {
            PXGUIProperty pxUIElementUpdateInfo[2];
            PXClearList(PXGUIProperty, pxUIElementUpdateInfo, 2);

            //pxUIElementUpdateInfo[0].WindowCurrent = pxWindowCurrent;
            //pxUIElementUpdateInfo[0].WindowParent = pxWindowParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            //pxUIElementUpdateInfo[1].WindowCurrent = pxWindowCurrent;
            //pxUIElementUpdateInfo[1].WindowParent = pxWindowParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;


            //  PXWindowUpdate(pxGUIManager, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            PXUIElementTreeViewItemInfo PXREF pxUIElementTreeViewItemInfo = &pxWindowCreateInfo->TreeViewItem;
            // Create ui item for a tree view

#if OSUnix
#elif OSWindows

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

            // item.item.iImage = 1;


            const char text[] = "[N/A]";

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                item.hParent = 0;// pxUIElementTreeViewItemInfo->ItemParent->Info.Handle.WindowHandle;
            }

            if(pxUIElementTreeViewItemInfo->TextDataOverride)
            {
                item.item.pszText = pxUIElementTreeViewItemInfo->TextDataOverride;
                item.item.cchTextMax = pxUIElementTreeViewItemInfo->TextSizeOverride;
            }
            else
            {
                item.item.pszText = pxWindowCreateInfo->WindowText.A;
                item.item.cchTextMax = pxWindowCreateInfo->WindowText.SizeUsed;
            }



            if(PXResourceTypeGUIElement == pxUIElementTreeViewItemInfo->OwningObjectType)
            {
                PXWindow PXREF uiElement = (PXWindow*)pxUIElementTreeViewItemInfo->OwningObject;

                switch(uiElement->Type)
                {
                    // case PXUIElementTypeRenderFrame:


                    case PXUIElementTypeWindow:
                        item.item.iImage = 0;
                        break;
                    case PXUIElementTypeButton:
                        item.item.iImage = 1;
                        break;
                    case PXUIElementTypeTextEdit:
                        item.item.iImage = 2;
                        break;
                    case PXUIElementTypeRenderFrame:
                        item.item.iImage = 3;
                        break;
                    case PXUIElementTypeText:
                        item.item.iImage = 5;
                        break;
                    case PXUIElementTypeTreeView:
                        item.item.iImage = 4;
                        break;
                    case PXUIElementTypePanel:
                        item.item.iImage = 6;
                        break;

                        // case PXUIElementTypeWindow:                    item.item.iImage = 1;                    break;
                        // case PXUIElementTypeWindow:                    item.item.iImage = 1;                    break;



                    default:
                        item.item.iImage = -1;
                        break;
                }
            }

            item.item.iSelectedImage = item.item.iImage;

            switch(pxUIElementTreeViewItemInfo->InsertMode)
            {
                case PXUIElementTreeViewItemInsertModeROOT:
                    item.hInsertAfter = TVI_ROOT;
                    break;

                case PXUIElementTreeViewItemInsertModeFIRST:
                    item.hInsertAfter = TVI_FIRST;
                    break;

                case PXUIElementTreeViewItemInsertModeLAST:
                    item.hInsertAfter = TVI_LAST;
                    break;

                case PXUIElementTreeViewItemInsertModeSORT:
                    item.hInsertAfter = TVI_SORT;
                    break;
            }

            item.item.mask = TVIF_TEXT | TVIF_IMAGE;

            HTREEITEM itemID = TreeView_InsertItem(pxUIElementTreeViewItemInfo->TreeView->Info.Handle.WindowHandle, &item);

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                TreeView_Expand
                (
                    pxUIElementTreeViewItemInfo->TreeView->Info.Handle.WindowHandle,
                    pxUIElementTreeViewItemInfo->ItemParent->Info.Handle.WindowHandle,
                    TVE_EXPAND
                );
            }

            pxWindow->Info.Handle.WindowHandle = 0;// itemID;
            pxUIElementTreeViewItemInfo->ItemHandle = itemID;
#endif

            break;
        }
        case PXUIElementTypeResourceManger:
        {
            pxWindow->ExtendedData = PXMemoryHeapCallocT(PXWindowExtendedBehaviourResourceView, 1);



            break;
        }
        case PXUIElementTypeRichEdit:
        {


            // EM_SETBKGNDCOLOR(pxWindow->ID, RGB(10, 10, 10));
            // EM_SETTEXTCOLOR(pxWindow->ID, RGB(200, 200, 200));

            break;
        }
        case PXUIElementTypeTrackBar:
        {
#if OSUnix
#elif OSWindows

            HDC dc = GetDC(pxWindow->Info.Handle.WindowHandle);


            SetTextColor(dc, RGB(255, 0, 0));
            SetBkColor(dc, RGB(0, 0, 255));


            SendMessageA(pxWindow->Info.Handle.WindowHandle, TBM_SETTIPSIDE, TBTS_RIGHT, PXNull);


            // SendMessageA(pxWindow->ID, SET_BACKGROUND_COLOR, RGB(30, 30, 30), RGB(30, 30, 30));

            // SET_BACKGROUND_COLOR;

            // TreeView_SetBkColor(pxWindow->ID, RGB(30, 30, 30));

            //   TreeView_SetTextColor(pxWindow->ID, RGB(200, 200, 200));
#endif

            break;
        }
        case PXUIElementTypeProgressBar:
        {
            PXGUIProperty pxUIElementUpdateInfo[2];
            PXClearList(PXGUIProperty, pxUIElementUpdateInfo, 2);

            //pxUIElementUpdateInfo[0].WindowCurrent = pxWindowCurrent;
            //pxUIElementUpdateInfo[0].WindowParent = pxWindowParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            //pxUIElementUpdateInfo[1].WindowCurrent = pxWindowCurrent;
            //pxUIElementUpdateInfo[1].WindowParent = pxWindowParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;

            PXNativDrawWindowProperty(&_pxGUIManager.NativDraw, pxWindow, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeView:
        {
#if OSUnix
#elif OSWindows

            if(0)
            {



            }
            else
            {


                int sizeX = 16;
                int sizeY = 16;
                int amount = 11;

                TreeView_SetBkColor(pxWindow->Info.Handle.WindowHandle, RGB(30, 30, 30));

                TreeView_SetTextColor(pxWindow->Info.Handle.WindowHandle, RGB(200, 200, 200));


                // Add icons
                HIMAGELIST imageListHandle = ImageList_Create
                (
                    sizeX,
                    sizeY,
                    ILC_COLOR,
                    amount,
                    amount
                );


                HMODULE currentModule = GetModuleHandle(NULL);


                const char* pathList[11] =
                {
                    "Texture/Window.bmp",
                    "Texture/Button.bmp",
                    "Texture/Edit.bmp",
                    "Texture/FrameBuffer.bmp",
                    "Texture/TreeView.bmp",
                    "Texture/Text.bmp",
                    "Texture/Panel.bmp",
                    "Texture/Folder.bmp",
                    "Texture/H.bmp",
                    "Texture/HPP.bmp",
                    "Texture/SquareBlue.bmp"
                };


                for(size_t i = 0; i < amount; i++)
                {
                    // HBITMAP bitmapHandle = LoadBitmapA(PXNull, OBM_CLOSE);

                    const char* filePath = pathList[i];

                    HBITMAP bitmapHandle = LoadImageA
                    (
                        PXNull,
                        filePath,
                        IMAGE_BITMAP,
                        sizeX,
                        sizeY,
                        LR_LOADFROMFILE
                    );

                    int addedID = ImageList_Add(imageListHandle, bitmapHandle, PXNull);

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXWindowText,
                        "ImageList-Add",
                        "New icon %i",
                        addedID
                    );
                }

                TreeView_SetImageList(pxWindow->Info.Handle.WindowHandle, imageListHandle, TVSIL_NORMAL);
            }
#endif

            break;
        }      
        case PXUIElementTypeTabControll:
        {
            PXUIElementTabPageInfo PXREF pxUIElementTabPageInfo = &pxWindowCreateInfo->TabPage;


            // Create space for extended data
            PXWindowExtendedBehaviourTab* pxWindowExtendedBehaviourTab = PXMemoryHeapCallocT(PXWindowExtendedBehaviourTab, 1);
            pxWindow->ExtendedData = pxWindowExtendedBehaviourTab;

            // Fill extended data
            pxWindowExtendedBehaviourTab->TABPageIndexCurrent = -1;
            pxWindowExtendedBehaviourTab->TABPageAmount = pxWindowCreateInfo->TabPage.TabPageSingleInfoAmount;

            // NO!! NO ALLOC HERE
            //pxWindowExtendedBehaviourTab->TABPageList = PXMemoryHeapCallocT(PXWindow, pxWindowCreateInfo->Data.TabPage.TabPageSingleInfoAmount);


            PXIcon* pxIcon = PXMemoryHeapCallocT(PXIcon, 1);

            PXNativDrawIconLoad(&_pxGUIManager.NativDraw, pxIcon, 0);


            PXECSCreateInfo pxResourceCreateInfo;
            PXClear(PXECSCreateInfo, &pxResourceCreateInfo);

            // Create
            {
                // Create a panel for each page, to contain all elements, so that we can hide and show all at once
                pxResourceCreateInfo.Type = PXResourceTypeGUIElement;
                pxResourceCreateInfo.ObjectAmount = pxWindowExtendedBehaviourTab->TABPageAmount;
                pxResourceCreateInfo.ObjectReference = &pxWindowExtendedBehaviourTab->TABPageList;
                //pxResourceCreateInfo.Parent = pxWindowCurrent;
                pxResourceCreateInfo.UIElement.Type = PXUIElementTypePanel;
                pxResourceCreateInfo.UIElement.WindowText.A = "TAB-Page";
                pxResourceCreateInfo.UIElement.Invisible = PXFalse;
                //pxResourceCreateInfo.UIElement.WindowCurrent = pxWindowCurrent;
                //pxResourceCreateInfo.UIElement.WindowParent = pxWindowParent;
                pxResourceCreateInfo.UIElement.Setting = PXWindowBehaviourBorder | PXWindowAllignCenter;// | PXWindowAllignIgnoreParentMargin;
                pxResourceCreateInfo.UIElement.Position.Margin.Left = -0.40;
                pxResourceCreateInfo.UIElement.Position.Margin.Top = 0.025;
                pxResourceCreateInfo.UIElement.Position.Margin.Right = -0.40;
                pxResourceCreateInfo.UIElement.Position.Margin.Bottom = -0.525;

                //PXResourceManagerAdd(&pxResourceCreateInfo);
                //PXWindowCreate(pxGUIManager, &pxResourceCreateInfo, 1);
            }

            for(PXSize i = 0; i < pxUIElementTabPageInfo->TabPageSingleInfoAmount; ++i)
            {
                PXWindow PXREF pxWindowSub = &pxWindowExtendedBehaviourTab->TABPageList[i];

                pxResourceCreateInfo.ObjectAmount = 1;
                pxResourceCreateInfo.ObjectReference = &pxWindowSub;
                pxResourceCreateInfo.Parent = pxWindow;
                //pxResourceCreateInfo.UIElement.WindowCurrent = pxWindowSub;
                //pxResourceCreateInfo.UIElement.WindowParent = pxWindowCurrent;


                PXGUICreate(pxWindowSub, pxWindow, &pxResourceCreateInfo);

                PXUIElementTabPageSingleInfo PXREF pxUIElementTabPageSingleInfo = &pxUIElementTabPageInfo->TabPageSingleInfoList[i];
                *pxUIElementTabPageSingleInfo->UIElement = pxWindowSub; // Store reference to have it for the caller

                pxWindowSub->NameContent = pxUIElementTabPageSingleInfo->PageName;
                pxWindowSub->NameContentSize = PXTextLengthA(pxWindowSub->NameContent, PXTextLengthUnkown);

                if(pxUIElementTabPageSingleInfo->TABIcon)
                {
                    pxWindowSub->Icon = pxUIElementTabPageSingleInfo->TABIcon;
                }
                else
                {
                    pxWindowSub->Icon = pxIcon;
                }

                pxWindowSub->Info.Hierarchy.Parrent = pxWindow;
                pxWindowSub->Type = pxUIElementTabPageSingleInfo->UIElementType;


                switch(pxUIElementTabPageSingleInfo->UIElementType)
                {
                    case PXUIElementTypeCode:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawCode;
                        break;
                    }
                    case PXUIElementTypeHexEditor:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawHexEditor;
                        break;
                    }
                    case PXUIElementTypeGraphBehaviour:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawGraphBehaviour;
                        break;
                    }
                    case PXUIElementTypeGraphTime:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawGraphTime;
                        break;
                    }
                    case PXUIElementTypeSoundPlayerMixer:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawSoundPlayerMixer;
                        break;
                    }
                    case PXUIElementTypeVideoCutter:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawVideoCutter;
                        break;
                    }
                    case PXUIElementTypeDataBaseManager:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawDataBase;
                        break;
                    }
                    case PXUIElementTypeNetworkTester:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawNetwork;
                        break;
                    }
                    case PXUIElementTypeInputView:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawInputView;
                        break;
                    }
                    case PXUIElementTypeHardwareInfo:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawHardwareInfo;
                        break;
                    }

                    default:
                        break;
                }

                pxWindowSub->DrawFunction = PXGUIDrawFailback;



                switch(PXECSInfoUseByMask & pxWindow->Info.Behaviour)
                {
                    case PXECSInfoUseByOS:
                    {
#if OSUnix
#elif OSWindows && 0
                        TCITEM tie;
                        tie.mask = TCIF_TEXT | TCIF_IMAGE;
                        tie.iImage = pxUIElementTabPageSingleInfo->ImageID;
                        tie.pszText = pxUIElementTabPageSingleInfo->PageName;

                        auto x = TabCtrl_InsertItem(pxWindow->Info.Handle.WindowHandle, i, &tie);
#endif
                        break;
                    }

                    case PXECSInfoUseByEngine:
                    {


                        break;
                    }

                    default:
                        break;
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    PXGUIName,
                    "TabControl-Add",
                    "Page added %s",
                    pxUIElementTabPageSingleInfo->PageName
                );
            }

#if OSUnix
#elif OSWindows && 0
            SendMessage(pxWindow->Info.Handle.WindowHandle, TCM_SETCURFOCUS, 0, 0);
#endif
            break;
        }       
        case PXUIElementTypeRenderFrame:
        {
#if 1
            PXUIElementSceneRenderInfo PXREF pxUIElementSceneRenderInfo = &pxWindowCreateInfo->SceneRender;
            PXEngine PXREF pxEngine = pxUIElementSceneRenderInfo->Engine;
            PXEngineStartInfo PXREF pxEngineStartInfo = pxUIElementSceneRenderInfo->StartInfo;

            // pxEngine->Window = pxWindowCurrent; // Maybe override?

            pxEngineStartInfo->Mode = PXGraphicInitializeModeOSGUIElement;
            pxEngineStartInfo->Width = pxWindow->Position.Form.Width;
            pxEngineStartInfo->Height = pxWindow->Position.Form.Height;
            pxEngineStartInfo->Name = "UIElement-RenderFrame";
            pxEngineStartInfo->UseMouseInput = 1;
            pxEngineStartInfo->WindowRenderTarget = pxWindow; // Maybe override?
            pxEngineStartInfo->WindowRenderParent = pxWindowParent;

            PXEngineStart(pxEngine, pxEngineStartInfo);

#endif

            break;
        }
        case PXUIElementTypeWindow:
        {
            // PXWindowCreateWindowInfo PXREF pxWindowCreateWindowInfo = &pxWindowCreateInfo->Data.Window;

            PXGUIPropertyTitleBarColor(pxWindow);

#if OSWindows
            BOOL aaxx = EnableWindow(pxWindow->Info.Handle.WindowHandle, PXTrue);
#endif

            //  HWND qssa = SetCapture(pxWindowCurrent->Info.Handle.WindowHandle);

#if 0
#if OSUnix

            int numberOfDevices = 0;

            const XDeviceInfo* deviceInfoList = XListInputDevices(display, &numberOfDevices);

            for(int i = 0; i < numberOfDevices; ++i)
            {
                const XDeviceInfo& xDeviceInfo = deviceInfoList[i];

                printf
                (
                    "[Device] ID:%i Name:%s Use:%i\n",
                    xDeviceInfo.id,
                    xDeviceInfo.name,
                    xDeviceInfo.use
                );

                // XOpenDevice(display, i); // Cannot open mouse or keyboard??  invalid call

                // XEventClass eventList[8]={0,0,0,0,0,0,0,0};
                //  int listSize = xDeviceInfo.num_classes;

                //  int resultE = XSelectExtensionEvent(display, PXWindowID, eventList, listSize);

                // printf("");
            }
#endif



            if(info)
            {
                const PXResult actionResult = PXThreadRun(&window->MessageThread, "PXWindowMessageLoop", PXWindowMessageLoop, window);
                const PXBool sucessful = PXResultOK == actionResult;

                if(!sucessful)
                {
                    return; // TODO: return something?
                }

                PXBool expected = PXTrue;
                PXAwaitInfo pxAwaitInfo;
                pxAwaitInfo.DataTarget = &window->IsRunning;
                pxAwaitInfo.DataExpect = &expected;
                pxAwaitInfo.DataSize = sizeof(PXBool);

                PXAwaitChange(&pxAwaitInfo);
            }
#endif

            break;
        }

        default:
            return PXResultRefusedParameterInvalid;
    }
#endif

    return PXResultOK;


    /*
    switch(pxWindowCreateInfo->FLags)
    {
        case PXWindowCreatePhysical:
        {
            result = PXGUICreatePhysical(pxWindow, pxWindowCreateInfo);
            break;
        }
        case PXWindowCreateVirtual:
        {
            result = PXGUICreateVirtual(pxWindow, pxWindowCreateInfo);
            break;
        }
        default:
        {
            return PXResultRefusedParameterInvalid;
        }
    }*/

    return result;
}

PXResult PXAPI PXWindowPositionCalculcate(PXWindow PXREF pxWindow, PXWindowPositionCalulcateInfo PXREF pxUIElementPositionCalulcateInfo)
{
    if(!(pxWindow && pxUIElementPositionCalulcateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(!((PXWindowAllignIgnoreParentMargin & pxWindow->Info.Setting) > 0))
    {
#if 0
        for
            (
            PXWindow* pxUIElementParent = (PXWindow*)pxWindow->Info.Hierarchy.Parrent;
            pxUIElementParent;
            pxUIElementParent = (PXWindow*)pxUIElementParent->Info.Hierarchy.Parrent
            )
        {
            pxUIElementPositionCalulcateInfo->Margin.Left += pxUIElementParent->Position.Margin.Left;
            pxUIElementPositionCalulcateInfo->Margin.Top += pxUIElementParent->Position.Margin.Top;
            pxUIElementPositionCalulcateInfo->Margin.Right += pxUIElementParent->Position.Margin.Right;
            pxUIElementPositionCalulcateInfo->Margin.Bottom += pxUIElementParent->Position.Margin.Bottom;
        }
#endif
    }
    else
    {
        // pxUIElementPositionCalulcateInfo->MarginLeft += 0.2;
        // pxUIElementPositionCalulcateInfo->MarginTop += 0.2;
        // pxUIElementPositionCalulcateInfo->MarginRight += 0.2;
        // pxUIElementPositionCalulcateInfo->MarginBottom += 0.2;
    }

    pxUIElementPositionCalulcateInfo->Margin.Left += pxWindow->Position.Margin.Left;
    pxUIElementPositionCalulcateInfo->Margin.Top += pxWindow->Position.Margin.Top;
    pxUIElementPositionCalulcateInfo->Margin.Right += pxWindow->Position.Margin.Right;
    pxUIElementPositionCalulcateInfo->Margin.Bottom += pxWindow->Position.Margin.Bottom;

    // Normalizied space for OpenGL
    pxUIElementPositionCalulcateInfo->AA = -1 + pxUIElementPositionCalulcateInfo->Margin.Left;
    pxUIElementPositionCalulcateInfo->BA = -1 + pxUIElementPositionCalulcateInfo->Margin.Top;
    pxUIElementPositionCalulcateInfo->BB = +1 - pxUIElementPositionCalulcateInfo->Margin.Right,
        pxUIElementPositionCalulcateInfo->AB = +1 - pxUIElementPositionCalulcateInfo->Margin.Bottom;

    PXF32 remLeft = (pxUIElementPositionCalulcateInfo->Margin.Left) * pxUIElementPositionCalulcateInfo->WindowWidth / 2.0f;
    PXF32 remTop = (pxUIElementPositionCalulcateInfo->Margin.Top) * pxUIElementPositionCalulcateInfo->WindowHeight / 2.0f;
    PXF32 remRight = (pxUIElementPositionCalulcateInfo->Margin.Right) * pxUIElementPositionCalulcateInfo->WindowWidth / 2.0f;
    PXF32 remBottom = (pxUIElementPositionCalulcateInfo->Margin.Bottom) * pxUIElementPositionCalulcateInfo->WindowHeight / 2.0f;


    // Add perspective scaling
    {
        PXF32 factorX = pxUIElementPositionCalulcateInfo->WindowWidth / (PXF32)pxUIElementPositionCalulcateInfo->WindowHeight;
        PXF32 factorY = pxUIElementPositionCalulcateInfo->WindowHeight / (PXF32)pxUIElementPositionCalulcateInfo->WindowWidth;

        remLeft += factorX;
        remTop += factorY;
        remRight -= factorX;
        remBottom -= factorY;
    }


    PXF32 mathWithScaling = pxUIElementPositionCalulcateInfo->WindowWidth - remRight - remLeft;
    PXF32 heightWithScaling = pxUIElementPositionCalulcateInfo->WindowHeight - remBottom - remTop;












    // XYWH for WindowsAPI stuff0


    if(PXWindowKeepWidth & pxWindow->Info.Setting)
    {
        pxUIElementPositionCalulcateInfo->Size.Width = pxWindow->Position.Form.Width;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Size.Width = mathWithScaling;
    }

    if(PXWindowKeepHeight & pxWindow->Info.Setting)
    {
        pxUIElementPositionCalulcateInfo->Size.Height = pxWindow->Position.Form.Height;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Size.Height = heightWithScaling;
    }


    switch(PXWindowAllignFlags & pxWindow->Info.Setting)
    {
        default:
        case PXWindowAllignLeft:
        {
            pxUIElementPositionCalulcateInfo->Size.X = remLeft;
            pxUIElementPositionCalulcateInfo->Size.Y = remTop;
            break;
        }
        case PXWindowAllignTop:
        {
            pxUIElementPositionCalulcateInfo->Size.X = remLeft;
            pxUIElementPositionCalulcateInfo->Size.Y = remTop;
            break;
        }
        case PXWindowAllignRight:
        {
            pxUIElementPositionCalulcateInfo->Size.X = pxUIElementPositionCalulcateInfo->WindowWidth - (remRight + pxUIElementPositionCalulcateInfo->Size.Width);
            pxUIElementPositionCalulcateInfo->Size.Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Size.Height);
            break;

        }
        case PXWindowAllignBottom:
        {
            pxUIElementPositionCalulcateInfo->Size.X = remLeft;
            pxUIElementPositionCalulcateInfo->Size.Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Size.Height);
            break;
        }
    }
}























































const char* PXWindowDockSideString[] =
{
    "None",
    "Center",
    "Left",
    "Top",
    "Right",
    "Bottom"
};

const char* PXWindowDockSideToString(const PXWindowDockSide pxWindowDockSide)
{
    return PXWindowDockSideString[pxWindowDockSide];
}

RECT PXWindowRectOf(HWND h)
{
    RECT r;
    GetClientRect(h, &r);
    return r;
}

PXWindow* PaneFromHwnd(HWND h)
{
    return (PXWindow*)GetWindowLongPtrW(h, GWLP_USERDATA);
}

void SubSegment(RECT* m, RECT* r, PXWindowDockSide side)
{
    const int bandW = 80;
    const int bandH = 80;

    // Set as base
    *r = *m;

    switch(side)
    {
        default:
            // is already set, do nothing
            break;
        case PXWindowDockSideLeft:
            r->right = m->left + bandW;
            break;
        case PXWindowDockSideRight:
            r->left = m->right - bandW;
            break;
        case PXWindowDockSideTop:
            r->bottom = m->top + bandH;
            break;
        case PXWindowDockSideBottom:
            r->top = m->bottom - bandH;
            break;

    }
}
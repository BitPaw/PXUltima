#include "PXWindow.h"
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include <PX/Type/PXFont.h>
#include <PX/Type/PXFrameBuffer.h>
#include <PX/OS/PXOS.h>
#include <PX/Type/PXBrush.h>
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


const char PXEventText[] = "Event";

const char PXTextFailsafe[] = "<missing text>";
const PXI8U PXTextFailsafeLength = sizeof(PXTextFailsafe);

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <gl/GL.h>
#include <PX/Container/List/PXList.h>


ATOM _pxEngineBaseClassID = 0;
HKL _keyBoardLayout = PXNull; // Selected keyboardlayout in taskbar
wchar_t _keyBoardLayoutName[KL_NAMELENGTH];

#define PXWindowLayoutCalc 1

typedef struct PXWindowHierarchyNode_
{
    PXWindow* Parent;
    PXListT(PXWindow*) Children;
}
PXWindowHierarchyNode;



// Atomic UI-Element
// Only Text can be text
// Only image can be image
typedef struct PXWindow_
{
    PXECSInfo Info;
    PXWindowHierarchyNode Hierarchy;
    PXWindowLayout Layout;
    PXWindowState State;

    void* userData;
    void (*onClick)(PXWindow* self);
    void (*onLayout)(PXWindow* self);   // optional custom layout
    //-----------------------------------------------------

    PXGraphicSystem GraphicSystem; // The window can be drawn by different systems.

    // Used as the render sink target
    PXFrameBuffer* FrameBuffer;

#if OSWindows

    union
    {
        HMENU MenuHandle;
        HWND WindowHandle; // Created by "CreateWindow()"
    };

    HDC DeviceContextHandle; // Used by windows for direct context for rendering
    HGLRC RenderContext; // Only used by OpenGL

    PXBuffer InputBuffer; // Windows want a buffer like this
#endif


    PXWindowEventList EventList;

    // PXUIElementPosition Position;

     //---<State-Info>------------------------
    // PXWindowBrush* BrushFront;
    // PXWindowBrush* BrushBackground;
    PXFont* FontForText;
    PXIcon* Icon; // Icon to be rendered

    //PXColorRGBAF* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
   // PXUIHoverState Hover;
    PXI32U FlagsList;
    //---------------------------------------


    //---[Extended data]---------------------
    PXUIElementType Type;
    void* ExtendedData; // Extra data that will be allocated seperatly for additional use
    //---------------------------------------

    PXColorRGBI8 BackGroundColor;

    int       headerH;    // legacy; used as fallback min height

    // ---- Layout extensions (optional; defaults apply if 0) ----
    int       prefW;      // preferred band width for LEFT/RIGHT
    int       prefH;      // preferred band height for TOP/BOTTOM

    int       fixedLong;  // fixed size along stack axis (px). For LEFT/RIGHT: height; TOP/BOTTOM: width; CENTER: height.
    double    weight;     // flexible share weight along stack axis (<=0 => 1.0)

    int       minW;       // minimum width of the final rect given to the child (px)
    int       minH;       // minimum height of the final rect given to the child (px)

    // Optional per-pane spacing override among siblings; 0 => use container default
    int       spacing;

    // REMOVE??
    struct PXEngine_* Engine;
    struct PXRenderer_* Renderer;
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
    PXWindowDraw
};
PXECSRegisterInfoDynamic PXWindowRegisterInfoDynamic;

PXFont* _lastFont = PXNull;





















#include <windows.h>
#include <stdio.h>
#include <PX/Engine/ECS/PXNamePool.h>

typedef struct PXWindowSearchResult_
{
    PXText* Name;
    PXWindow* Target;

    DWORD g_OurProcessId;
} 
PXWindowSearchResult;

BOOL CALLBACK EnumMyWindowsProc(const HWND hwnd, PXWindowSearchResult* pxWindowSearchResult)
{
    PXWindow* pxWindow = PXNull;
    DWORD windowPid = 0;

    const DWORD result = GetWindowThreadProcessId(hwnd, &windowPid);
    const PXBool sameProcess = windowPid == pxWindowSearchResult->g_OurProcessId;

    // Only process windows belonging to our own process
    if(!sameProcess)
    {
        return TRUE;
    }

    // Retrieve user-defined object stored in GWLP_USERDATA
    pxWindow = (PXWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if(pxWindow == NULL)
    {
        return TRUE;
    }

    PXText pxText;

    PXNamePoolGet(pxWindow->Info.ID, &pxText);

    PXBool isTarget = PXTextCompare(&pxText, pxWindowSearchResult->Name);

    if(isTarget)
    {
        pxWindowSearchResult->Target = pxWindow;

        return FALSE;
    }
    

    return TRUE; // continue enumeration
}


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

    PXWindow* root = PXWindowParentGet(pxWindow);

    if(!root)
    {
        return pxWindow;
    }

    for(;;)
    {
        PXWindow PXREF parent = PXWindowParentGet(root);

        if(!parent)
        {
            break;
        }

        root = parent;
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

    // Check if we are a virtual window.
    // If no window handle exists, or any 
    // framebuffer, we cant do anything here
    PXBool canApply =
        pxWindow->WindowHandle &&
        pxWindow->DeviceContextHandle;

    // "Enable" openglk
    pxWindow->GraphicSystem = PXGraphicSystemOpenGL;

    if(!canApply)
    {
        return PXResultOK;
    }

    PXResult pxResult = PXResultInvalid;
    BOOL isOK = PXFalse;
    PXWindow* windowParent = PXWindowParentGet(pxWindow);  

    PXWindowPixelSystemInfo pxWindowPixelSystemInfo;
    PXClear(PXWindowPixelSystemInfo, &pxWindowPixelSystemInfo);
    pxWindowPixelSystemInfo.OpenGL = PXTrue;
    pxWindowPixelSystemInfo.GDI = PXTrue;
    pxWindowPixelSystemInfo.DirectX = PXTrue;
    pxWindowPixelSystemInfo.BitPerPixel = 32;

    PXWindowPixelSystemSet(pxWindow, &pxWindowPixelSystemInfo);

    PXRectangleXYWHI32 pxRectangleXYWHI32;
    PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWHI32);

    HGLRC glContextNew = wglCreateContext(pxWindow->DeviceContextHandle);
    pxResult = PXErrorCurrent(glContextNew);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "OpenGL",
        "Context-Create:<%s>",
        glContextNew != PXNull ? "OK" : "FAILURE"
    );
#endif

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
    //glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // dark theme
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- Fixed-function state ---
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);     // optional; useful if you add 3D later
    glShadeModel(GL_SMOOTH);

    // --- Model transform: center & rotate ---

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


PXResult PXAPI PXWindowDrawGDI(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
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

    HPEN pen = CreatePen(PS_SOLID, 3, RGB(75, 75, 75));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    MoveToEx(hdc, r.left, hdr.bottom, NULL);
    LineTo(hdc, r.right, hdr.bottom);

    SelectObject(hdc, oldPen);
    DeleteObject(pen);

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawGL(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
    PXColorRGBI8* color = &pxWindow->BackGroundColor;

    const PXRectangleXYWHI32 PXREF pxRectangle = &pxWindow->Layout.SpaceContent;


#if 0
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow,
        pxWindow->Layout.abs.X,
        pxWindow->Layout.abs.Y,
        pxWindow->Layout.abs.Width,
        pxWindow->Layout.abs.Height
    );

 
#endif

#if 1

    glColor3b(pxDrawInfo->Color.Red, pxDrawInfo->Color.Green, pxDrawInfo->Color.Blue);
    PXWindowDrawRect(pxWindow, pxRectangle->X, pxRectangle->Y, pxRectangle->Width, pxRectangle->Height);

#else

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    // ----------------------------- 
    // // Background rectangle 
    // // -----------------------------
    //glColor3ub(GetRValue(color->Red), GetGValue(color->Green), GetBValue(color->Blue));

    glColor3ub(80, 20, 10);
    PXWindowDrawRect(pxWindow, pxRectangle->X, pxRectangle->Y, pxRectangle->Width, pxRectangle->Height);


    // ----------------------------- 
    // Header rectangle 
    // ----------------------------- 
    glColor3ub(50, 50, 50);
    PXWindowDrawRect(pxWindow, pxRectangle->X, pxRectangle->Y, pxRectangle->Width, pxWindow->headerH);

    // -----------------------------
    // Text (using bitmap font lists)
    //----------------------------- 
#if 0
    char buffer[128];
    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

    int amount = sprintf(buffer, "Cool window - 0x%8.8x", pxWindow->DeviceContextHandle);
    glColor3ub(232, 232, 232);
    glRasterPos2i(5, 5 + 12);
#endif
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
    glVertex2i(0, pxWindow->headerH);
    glVertex2i(pxRectangle->Width, pxWindow->headerH);
    glEnd();
#endif

    return PXResultOK;
}

// Draw a hitbox + diamond + cross at cursor position in OpenGL 1.1
// mouseX, mouseY are in window coordinates (0,0 = top-left)
// winW, winH are the window size

void DrawCursorMarker2D(int mouseX, int mouseY, int winW, int winH)
{
    // Convert window coords (top-left origin) to OpenGL coords (center origin)
    // Assuming an orthographic projection like glOrtho(-w/2, w/2, -h/2, h/2, -1, 1)
    float x = (float)mouseX - winW * 0.5f;
    float y = (float)(winH - mouseY) - winH * 0.5f; // invert Y

    float halfBox = 16.0f;   // half-size of hitbox
    float halfCross = 10.0f; // half-size of cross arms

    glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);

    // 1) Rectangle (hitbox) - e.g. red
    glColor3f(1.0f, 0.2f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x - halfBox, y - halfBox);
    glVertex2f(x + halfBox, y - halfBox);
    glVertex2f(x + halfBox, y + halfBox);
    glVertex2f(x - halfBox, y + halfBox);
    glEnd();

    // 2) Diamond inside the box - e.g. green
    // Diamond points at midpoints of the box edges
    glColor3f(0.2f, 1.0f, 0.2f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y + halfBox); // top
    glVertex2f(x + halfBox, y);           // right
    glVertex2f(x, y - halfBox); // bottom
    glVertex2f(x - halfBox, y);           // left
    glEnd();

    // 3) Cross / plus sign inside the diamond - e.g. blue
    glColor3f(0.2f, 0.4f, 1.0f);
    glBegin(GL_LINES);
    // Horizontal line
    glVertex2f(x - halfCross, y);
    glVertex2f(x + halfCross, y);
    // Vertical line
    glVertex2f(x, y - halfCross);
    glVertex2f(x, y + halfCross);
    glEnd();

    glPopAttrib();
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

void PXAPI PXWindowPaintMain(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
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

PXSize PXAPI PXWindowChildAmount(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return 0;
    }

    return PXListItemAmount(&pxWindow->Hierarchy.Children);
}

PXWindow* PXAPI PXWindowChildGet(PXWindow PXREF pxWindow, const PXSize index)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    PXWindow** pxWindowChild = PXListItemAtIndexGetT(PXWindow**, &pxWindow->Hierarchy.Children, index);

    if(!pxWindowChild)
    {
        return PXNull;
    }

    return *pxWindowChild;
}

PXResult PXAPI PXWindowChildAdd(PXWindow PXREF pxWindowParent, PXWindow PXREF pxWindowChild)
{
    if(!(pxWindowParent && pxWindowChild))
    {
        return PXResultRefusedParameterNull;
    }

    pxWindowChild->Hierarchy.Parent = pxWindowParent;

    PXListAdd(&pxWindowParent->Hierarchy.Children, pxWindowChild);

    PXWindowLayoutCompute(pxWindowParent);

    return PXResultOK;
}

PXResult PXAPI PXWindowChildRemove(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    PXListAdd(&pxWindow->Hierarchy.Children, pxWindowChild);

    return PXResultOK;
}

PXWindow* PXAPI PXWindowParentGet(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    return pxWindow->Hierarchy.Parent;
}

PXWindow* PXAPI PXWindowGetViaName(PXText PXREF pxText)
{
    PXWindowSearchResult pxWindowSearchResult;
    PXClear(PXWindowSearchResult, &pxWindowSearchResult);
    pxWindowSearchResult.Name = pxText;
    pxWindowSearchResult.g_OurProcessId = GetCurrentProcessId();

    EnumWindows(EnumMyWindowsProc, &pxWindowSearchResult);

    return pxWindowSearchResult.Target;
}

PXResult PXAPI PXWindowHitReset(PXWindow PXREF pxWindow)
{
    return PXResultOK;
}

PXWindow* PXAPI PXWindowHitTest(PXWindow PXREF pxWindow, float x, float y)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    if(!pxWindow->State.DoRenderingSelf)
        return NULL;

    if(!PXRectangleXYWHI32ContainsPoint(&pxWindow->Layout.SpaceContent, x, y))
        return NULL;

    PXSize amount = PXWindowChildAmount(pxWindow);

    if(0 == amount)
    {
        // We dont have children
        return pxWindow;
    }

    // children last, reverse order (topmost first)
    for(PXSize i = 0; i < amount; ++i)
    {
        PXWindow PXREF child = PXWindowChildGet(pxWindow, amount - i);
        PXWindow PXREF hit = PXWindowHitTest(child, x, y);

        if(hit)
        {
            return hit;
        }
    }

    return pxWindow;
}

void PXAPI PXWindowLayoutComputeBoxModel(PXWindow PXREF pxWindow)
{
    PXWindowLayout PXREF pxLayout = &pxWindow->Layout;

    PXWindowSpaceDrawGet(pxWindow, &pxLayout->SpaceDraw);

    // Collect space from parents


    // Add marging
    pxLayout->SpaceBox = pxLayout->SpaceDraw;
        
    PXRectangleXYWHI32AddLTRBI32(&pxLayout->SpaceBox, &pxLayout->Margin);

    // Add padding
    pxLayout->SpaceContent = pxLayout->SpaceBox;

    PXRectangleXYWHI32AddLTRBI32(&pxLayout->SpaceContent, &pxLayout->Padding);

 
#if PXLogEnable && 1
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Offset",
        "%-10s : %4i\n"
        "%-10s : X:%4i Y:%4i W:%4i H:%4i\n"
        "%-10s : L:%4i T:%4i R:%4i B:%4i\n"
        "%-10s : L:%4i T:%4i R:%4i B:%4i",
        "PXID", pxWindow->Info.ID,
        "Content",
        pxLayout->SpaceContent.X,
        pxLayout->SpaceContent.Y,
        pxLayout->SpaceContent.Width,
        pxLayout->SpaceContent.Height,
        "Margin",
        pxLayout->Margin.Left,
        pxLayout->Margin.Top,
        pxLayout->Margin.Right,
        pxLayout->Margin.Bottom,
        "Padding",
        pxLayout->Padding.Left,
        pxLayout->Padding.Top,
        pxLayout->Padding.Right,
        pxLayout->Padding.Bottom
    );
#endif


    // We now have local solutions.
    // They are only correct on their own. 
    // Not nested!
}

void PXAPI PXWindowLayoutComputeChildren(PXWindow PXREF pxWindow)
{
    const PXSize amountChild = PXWindowChildAmount(pxWindow);

    for(PXSize i = 0; i < amountChild; ++i)
    {
        PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

        PXWindowLayoutCompute(child);
    }
}

void PXAPI PXWindowLayoutComputeAutoSize(PXWindow PXREF pxWindow)
{
#if 0
    if(pxWindow->Layout.AutoWidth || pxWindow->Layout.AutoHeight)
    {
        int maxRight = 0;
        int maxBottom = 0;

        for(size_t i = 0; i < amountChild; ++i)
        {
            PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

            int right =
                child->Layout.Final.X + child->Layout.Final.Width + child->Layout.Margin.Right;

            int bottom =
                child->Layout.Final.Y + child->Layout.Final.Height + child->Layout.Margin.Bottom;

            if(right > maxRight)
                maxRight = right;

            if(bottom > maxBottom)
                maxBottom = bottom;
        }

        if(pxWindow->Layout.AutoWidth)
        {
            pxWindow->Layout.Box.Width =
                (maxRight - pxWindow->Layout.Final.X)
                + pxWindow->Layout.Padding.Left + pxWindow->Layout.Padding.Right
                + pxWindow->Layout.Border.Left + pxWindow->Layout.Border.Right;
        }

        if(pxWindow->Layout.AutoHeight)
        {
            pxWindow->Layout.Box.Height =
                (maxBottom - pxWindow->Layout.Final.Y)
                + pxWindow->Layout.Padding.Top + pxWindow->Layout.Padding.Bottom
                + pxWindow->Layout.Border.Top + pxWindow->Layout.Border.Bottom;
        }
    }
#endif

}

void PXAPI PXWindowLayoutComputeLayout(PXWindow PXREF pxWindow)
{
#if 0
    int cursorX = pxWindow->Layout.Content.X;
    int cursorY = pxWindow->Layout.Content.Y;

    switch(pxWindow->Layout.LayoutMode)
    {
        case PXLayoutVertical:
        {
            for(size_t i = 0; i < amountChild; ++i)
            {
                PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

                child->Layout.Box.X = cursorX;
                child->Layout.Box.Y = cursorY;

                cursorY += child->Layout.Box.Height
                    + child->Layout.Margin.Top + child->Layout.Margin.Bottom
                    + pxWindow->Layout.SpacingY;
            }
            break;
        }

        case PXLayoutHorizontal:
        {
            for(size_t i = 0; i < amountChild; ++i)
            {
                PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

                child->Layout.Box.X = cursorX;
                child->Layout.Box.Y = cursorY;

                cursorX += child->Layout.Box.Width
                    + child->Layout.Margin.Left + child->Layout.Margin.Right
                    + pxWindow->Layout.SpacingX;
            }
            break;
        }

        case PXLayoutGrid:
        {
            int col = 0;
            int row = 0;

            for(PXSize i = 0; i < amountChild; ++i)
            {
                PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

                child->Layout.Box.X = cursorX + col * (child->Layout.Box.Width + pxWindow->Layout.SpacingX);
                child->Layout.Box.Y = cursorY + row * (child->Layout.Box.Height + pxWindow->Layout.SpacingY);

                col++;
                if(col >= pxWindow->Layout.GridColumns)
                {
                    col = 0;
                    row++;
                }
            }
            break;
        }

        case PXLayoutNone:
        default:
            break;
    }
#endif
}



void PXAPI PXWindowLayoutCompute(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return;
    }

    const PXSize amountChild = PXWindowChildAmount(pxWindow);
    const PXWindow PXREF pxWindowParent = PXWindowParentGet(pxWindow);
    PXWindowLayout PXREF pxLayout = &pxWindow->Layout;

#if PXWindowLayoutCalc && 0
    return;
#endif // 1

    // ---------------------------------------------------------
    // PASS 1: Compute this window's initial box model
    // ---------------------------------------------------------
    PXWindowLayoutComputeBoxModel(pxWindow);

    // ---------------------------------------------------------
    // PASS 2: Compute children first (bottom-up)
    // ---------------------------------------------------------
    PXWindowLayoutComputeChildren(pxWindow);

    // ---------------------------------------------------------
    // PASS 3: Auto-size this window from children
    // ---------------------------------------------------------
    //PXWindowLayoutComputeAutoSize(pxWindow);

    // ---------------------------------------------------------
    // PASS 4: Apply layout manager (positions children)
    // ---------------------------------------------------------
    //PXWindowLayoutComputeLayout(pxWindow);

    // ---------------------------------------------------------
    // PASS 5: Recompute children box model (final pass)
    // ---------------------------------------------------------
    //PXWindowLayoutComputeChildren(pxWindow);
}

#if OSWindows

typedef LRESULT(PXAPI* PXWindowEventHandlerFunction)(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent);

PXPrivate LRESULT PXAPI PXWindowEvent_WM_EMPTY(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCCREATE(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    CREATESTRUCTA* cs = (CREATESTRUCTA*)pxWindowEvent->ParamL;
    PXWindow PXREF pxWindow = (PXWindow*)cs->lpCreateParams;

    // Store our window payload
    SetWindowLongPtr(pxWindowEvent->WindowHandle, GWLP_USERDATA, (LONG_PTR)pxWindow);

    // Store created handle from event data to our object
    pxWindow->WindowHandle = pxWindowEvent->WindowHandle;

    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = PXTrue;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DESTROY(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementDestroy;

    PXWindowEventConsumer(pxWindowEvent);

#if 0
    if(hwnd == gMain)
        PostQuitMessage(0);
    return 0;
#endif

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementMove;
    pxWindowEvent->CallBack = PXWindowEventMoved;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SIZE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    int width = LOWORD(pxWindowEvent->ParamL);
    int height = HIWORD(pxWindowEvent->ParamL);

    pxWindowEvent->Type = PXWindowEventTypeElementResize;
    pxWindowEvent->CallBack = PXWindowEventResize;
    pxWindowEvent->Resize.Width = width;
    pxWindowEvent->Resize.Height = height;

    switch(pxWindowEvent->ParamW)
    {
        case SIZE_MAXHIDE: // (4) Message is sent to all pop - up windows when some other window is maximized.
            pxWindowEvent->Resize.Cause = PXWindowResizeCauseMAXHIDE;
            break;

        case SIZE_MAXIMIZED: // (2), The window has been maximized.
            pxWindowEvent->Resize.Cause = PXWindowResizeCauseMAXIMIZED;
            break;
        case SIZE_MAXSHOW: // (3), Message is sent to all pop - up windows when some other window has been restored to its former size.
            pxWindowEvent->Resize.Cause = PXWindowResizeCauseMAXSHOW;
            break;

        case SIZE_MINIMIZED: // (1) The window has been minimized.
            pxWindowEvent->Resize.Cause = PXWindowResizeCauseMINIMIZED;
            break;

        case SIZE_RESTORED: // (0) The window
            pxWindowEvent->Resize.Cause = PXWindowResizeCauseRESTORED;
            break;

        default:
            pxWindowEvent->Resize.Cause = PXWindowResizeCauseUnkown;
            break;
    }



    PXWindowEventConsumer(pxWindowEvent);

    DockLayoutConfig cfg = DockDefaultConfig();
    LayoutDockedEx(pxWindow, &cfg);

    if(!pxWindow)
    {
        LayoutDockedEx(pxWindow, NULL);
        InvalidateRect(pxWindow->WindowHandle, NULL, TRUE);
        UpdateWindow(pxWindow->WindowHandle);
    }
    else
    {
        if(height == 0)
            height = 1;

        if(pxWindow->RenderContext)
        {
            //wglMakeCurrent(pxWindow->DeviceContextHandle, pxWindow->RenderContext);
            //glViewport(0, 0, width, height);
            //wglMakeCurrent(pxWindow->DeviceContextHandle, 0);
        }

        //LayoutDockedEx(windowID, NULL);
        //InvalidateRect(windowID, NULL, FALSE);
    }

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CLOSE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementClose;
    pxWindowEvent->Close.CommitToClose = PXTrue;

    PXWindowEventConsumer(pxWindowEvent);

    if(!pxWindowEvent->Close.CommitToClose)
    {
        return PXFalse;
    }

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SETCURSOR(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    const HWND windowsHandle = (HWND)pxWindowEvent->ParamW;
    const WORD hitTestResult = LOWORD(pxWindowEvent->ParamL);
    const WORD sourceMessage = HIWORD(pxWindowEvent->ParamL);
    const PXBool showCursor = PXTrue;//!(window->CursorModeCurrent == PXWindowCursorInvisible || window->CursorModeCurrent == PXWindowCursorLockAndHide);

    if(showCursor)
    {
        while(ShowCursor(1) < 0);
    }
    else
    {
        while(ShowCursor(0) >= 0);
    }

    pxWindowEvent->DoReturnValue = 1;
    pxWindowEvent->ReturnValue = 1;  // prevent further processing

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NOTIFY(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    const HWND windowsHandle = (HWND)pxWindowEvent->ParamW;
    LPNMHDR notificationCode = (LPNMHDR)pxWindowEvent->ParamL;

    if(!(windowsHandle && notificationCode))
    {
        return 0;
    }

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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ERASEBKGND(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = PXTrue; // Signal we handled erasing. 0 would mean we didn't

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_PAINT(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // We don't do anything here.
    // This is only to validate any region 
    // windows would think needs redrawing, this only stops 
    // messages to redraw this region.

    HWND windowHandle = pxWindowEvent->WindowHandle;

    PAINTSTRUCT ps;
    BeginPaint(windowHandle, &ps);
    EndPaint(windowHandle, &ps);

    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = 0; // Signal we processed it

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT_DEVICE_CHANGE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // WARNING! 
           // This event is only triggerd when we have registerd a raw device!

    const UINT eventID = (UINT)pxWindowEvent->ParamW;
    HANDLE deviceHandle = (HANDLE)pxWindowEvent->ParamL;

    PXDeviceInputUpdateInfo pxDeviceInputUpdateInfo;
    PXClear(PXDeviceInputUpdateInfo, &pxDeviceInputUpdateInfo);
    pxDeviceInputUpdateInfo.DeviceHandle = deviceHandle;

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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEMOVE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    int x = GET_X_LPARAM(pxWindowEvent->ParamL); // Windows 2000, windowsx.h 
    int y = GET_Y_LPARAM(pxWindowEvent->ParamL); // Windows 2000, windowsx.h 

    pxWindowEvent->Type = PXWindowEventTypeInputMouseMove;
    pxWindowEvent->CallBack = PXWindowEventMouseMove;
    pxWindowEvent->MouseMove.Position.X = x;
    pxWindowEvent->MouseMove.Position.Y = y;


    if(pxWindow)
    {
        if(!pxWindow->State.IsInFocus)
        {
            pxWindow->State.IsInFocus = PXTrue;

            //-------------------------------------
            // Trigger enter event
            pxWindowEvent->Type = PXWindowEventTypeElementFocusEnter;
            PXWindowEventConsumer(pxWindowEvent);
            //-------------------------------------

            // Request WM_MOUSELEAVE for later, WM_MOUSEMOVE cant detect if we TAB out
            TRACKMOUSEEVENT tme = { 0 };
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = pxWindowEvent->WindowHandle;
            TrackMouseEvent(&tme);
        }
    }

    // Overrride
    if(1)
    {
        POINT p;
        GetCursorPos(&p);

        pxWindowEvent->WindowHandle = WindowFromPoint(p);
        pxWindowEvent->WindowSender = (PXWindow*)GetWindowLongPtr(pxWindowEvent->WindowHandle, GWLP_USERDATA);
    }

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSELEAVE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    if(pxWindow->State.IsInFocus)
    {
        pxWindow->State.IsInFocus = PXFalse;

        pxWindowEvent->Type = PXWindowEventTypeElementFocusLeave;
        pxWindowEvent->CallBack = PXWindowEventMouseEnter;

        PXWindowEventConsumer(pxWindowEvent);
    }

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCHITTEST(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    const int xPos = GET_X_LPARAM(pxWindowEvent->ParamL);
    const int yPos = GET_Y_LPARAM(pxWindowEvent->ParamL);

    if(pxWindow && pxWindow->State.Floating)
    {
        POINT s = { xPos, yPos };
        POINT c = s;
        ScreenToClient(pxWindowEvent->WindowHandle, &c);
        RECT rc;
        GetClientRect(pxWindowEvent->WindowHandle, &rc);
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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DISPLAYCHANGE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // Some display got plugged in, out or some setting changed
          // We need to update our monitors/displays

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    const HRAWINPUT rawInput = (HRAWINPUT)pxWindowEvent->ParamL;

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

    PXBufferEnsureTotal(&pxWindow->InputBuffer, size);

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

            pxWindowEvent->Type = PXWindowEventTypeInputKeyboard;
            pxWindowEvent->Keyboard.VirtualKey = wVirtKey;

            PXBool released = (raw->data.keyboard.Flags & RI_KEY_BREAK) != 0;

            if(released)
            {
                pxWindowEvent->Keyboard.PressState = PXKeyPressStateDown;
            }
            else
            {
                pxWindowEvent->Keyboard.PressState = PXKeyPressStateUp;
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

            pxWindowEvent->Keyboard.CharacterW = buffer[0];

            break;
        }
        case RIM_TYPEMOUSE:
        {
            pxWindowEvent->Type = PXWindowEventTypeInputMouseMove;
            pxWindowEvent->MouseMove.Axis.X = 0;
            pxWindowEvent->MouseMove.Axis.Y = 0;
            pxWindowEvent->MouseMove.Delta.X = raw->data.mouse.lLastX;
            pxWindowEvent->MouseMove.Delta.Y = raw->data.mouse.lLastY;
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

    PXWindowEventConsumer(pxWindowEvent);

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

                    PXWindowEventConsumer(pxWindowEvent);


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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CTLCOLOR(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // WM_CTLCOLORMSGBOX:
    // WM_CTLCOLOREDIT:
     // WM_CTLCOLORLISTBOX:
     // WM_CTLCOLORBTN:
     // WM_CTLCOLORDLG:
     // WM_CTLCOLORSCROLLBAR:
     // WM_CTLCOLORSTATIC:

    // HWND windowFocusedHandle = (HWND)GetFocus();;
    HWND windowHandleNow = (HWND)pxWindowEvent->ParamL;
    HDC hdc = (HDC)pxWindowEvent->ParamW;
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();

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

    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = (LRESULT)brushHandle;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVING(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    RECT* pr = (RECT*)pxWindowEvent->ParamL;

    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);
#if 1
    if(pxWindow && pxWindow->State.Floating && pxWindowRoot) {
        // lParam is LPRECT of proposed window rect during the drag      
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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ENTERSIZEMOVE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = 0;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_EXITSIZEMOVE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    if(pxWindow && pxWindow->State.Floating)
    {
        PXWindowHideDockOverlay(pxWindow);
        RECT wr;
        GetWindowRect(pxWindowEvent->WindowHandle, &wr);

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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_POWERBROADCAST(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
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
    switch(pxWindowEvent->ParamW)
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
            POWERBROADCAST_SETTING PXREF powerBroadCastSetting = (POWERBROADCAST_SETTING*)pxWindowEvent->ParamL;

            break;
        }
    }

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DEVICECHANGE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // Triggers in any case
    UINT eventID = (UINT)pxWindowEvent->ParamW;

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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUTLANGCHANGE(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // unrelayable function!

    _keyBoardLayout = (HKL)pxWindowEvent->ParamL; // new layout

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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_COMMAND(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    const PXI32U buttonType = pxWindowEvent->ParamW; // IDOK;
    const HANDLE handle = (HANDLE)pxWindowEvent->ParamL;

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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_KEY(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    const LPARAM characterInfo = pxWindowEvent->ParamL;

    pxWindowEvent->Type = PXWindowEventTypeInputKeyboard;
    pxWindowEvent->Keyboard.CharacterID = pxWindowEvent->ParamW;
    pxWindowEvent->Keyboard.VirtualKey = PXVirtualKeyFromID(pxWindowEvent->ParamW);

    switch(pxWindowEvent->EventID)
    {
        case WM_KEYUP:
        {
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateUp;
            break;
        }
        case WM_KEYDOWN:
        {
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateDown;
            break;
        }
    }

    PXWindowEventConsumer(pxWindowEvent);


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

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDOWN(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    POINT c;
    c.x = GET_X_LPARAM(pxWindowEvent->ParamL);
    c.y = GET_Y_LPARAM(pxWindowEvent->ParamL);

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonLeft;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    if(pxWindow && !pxWindow->State.Floating) {

        if(c.y < pxWindow->headerH) 
        {
            POINT s = c;
            ClientToScreen(pxWindowEvent->WindowHandle, &s);
            PXWindowTearOffToFloating(pxWindow, s);
            return 0;
        }
    }

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONUP(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonLeft;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDBLCLK(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonLeft;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDoubleClick;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDOWN(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonRight;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONUP(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonRight;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDBLCLK(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonRight;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDoubleClick;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDOWN(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonMiddle;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONUP(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonMiddle;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDBLCLK(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonMiddle;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDoubleClick;
    pxWindowEvent->CallBack = PXWindowEventMouseClick;

    PXWindowEventConsumer(pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEWHEEL(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_XBUTTON(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    // WM_XBUTTONDOWN: // Fall though
    // WM_XBUTTONUP:

    PXMouseButton mouseButton = PXMouseButtonInvalid;
    PXKeyPressState buttonState = PXKeyPressStateInvalid;

    const WORD releaseID = HIWORD(pxWindowEvent->ParamW);
    // const WORD xxxxx = LOWORD(wParam);
    // const WORD fwKeys = GET_KEYSTATE_WPARAM(wParam);
    // const WORD fwButton = GET_XBUTTON_WPARAM(wParam);
    // const int xPos = GET_X_LPARAM(lParam);
    // const int yPos = GET_Y_LPARAM(lParam);

    switch(pxWindowEvent->EventID)
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


    return 0;
}

const PXI16U _windowEventList[] =
{
    WM_NULL,
    WM_CREATE,  // Do nothíng here, as it's too soon to regard the window as 'created'
    WM_NCCREATE,
    WM_DESTROY,
    WM_MOVE,
    WM_SIZE,
    WM_CLOSE,
    WM_SETCURSOR,
    WM_NOTIFY,
    WM_ERASEBKGND,
    WM_PAINT,
    WM_INPUT,
    WM_INPUT_DEVICE_CHANGE,
    WM_MOUSEMOVE,
    WM_MOUSELEAVE,
    WM_NCHITTEST,
    WM_DISPLAYCHANGE,
    WM_CTLCOLOR,
    WM_MOVING,
    WM_ENTERSIZEMOVE,
    WM_EXITSIZEMOVE,
    WM_POWERBROADCAST,
    WM_DEVICECHANGE,
    WM_COMMAND,
    WM_KEYUP,
    WM_KEYDOWN,
    WM_LBUTTONDOWN,
    WM_LBUTTONUP,
    WM_LBUTTONDBLCLK,
    WM_RBUTTONDOWN,
    WM_RBUTTONUP,
    WM_RBUTTONDBLCLK,
    WM_MBUTTONDOWN,
    WM_MBUTTONUP,
    WM_MBUTTONDBLCLK,
    WM_MOUSEWHEEL,
    WM_XBUTTONUP,
    WM_XBUTTONDOWN
};
const PXI16U _windowEventListAmount = sizeof(_windowEventList) / sizeof(PXI16U);


// Not handled events
// WM_WINDOWPOSCHANGING
// WM_WINDOWPOSCHANGING
// WM_WINDOWPOSCHANGED
// WM_ACTIVATE
// WM_SYSCOMMAND

const PXWindowEventHandlerFunction _pxWindowEventHandleList[] =
{
    PXWindowEvent_WM_EMPTY,
    PXWindowEvent_WM_EMPTY,
    PXWindowEvent_WM_NCCREATE,
    PXWindowEvent_WM_DESTROY,
    PXWindowEvent_WM_MOVE,
    PXWindowEvent_WM_SIZE,
    PXWindowEvent_WM_CLOSE,
    PXWindowEvent_WM_SETCURSOR,
    PXWindowEvent_WM_NOTIFY,
    PXWindowEvent_WM_ERASEBKGND,
    PXWindowEvent_WM_PAINT,
    PXWindowEvent_WM_INPUT,
    PXWindowEvent_WM_INPUT_DEVICE_CHANGE,
    PXWindowEvent_WM_MOUSEMOVE,
    PXWindowEvent_WM_MOUSELEAVE,
    PXWindowEvent_WM_NCHITTEST,
    PXWindowEvent_WM_DISPLAYCHANGE,
    PXWindowEvent_WM_CTLCOLOR,
    PXWindowEvent_WM_MOVING,
    PXWindowEvent_WM_ENTERSIZEMOVE,
    PXWindowEvent_WM_EXITSIZEMOVE,
    PXWindowEvent_WM_POWERBROADCAST,
    PXWindowEvent_WM_DEVICECHANGE,
    PXWindowEvent_WM_COMMAND,
    PXWindowEvent_WM_KEY,
    PXWindowEvent_WM_KEY,
    PXWindowEvent_WM_LBUTTONDOWN,
    PXWindowEvent_WM_LBUTTONUP,
    PXWindowEvent_WM_LBUTTONDBLCLK,
    PXWindowEvent_WM_RBUTTONDOWN,
    PXWindowEvent_WM_RBUTTONUP,
    PXWindowEvent_WM_RBUTTONDBLCLK,
    PXWindowEvent_WM_MBUTTONDOWN,
    PXWindowEvent_WM_MBUTTONUP,
    PXWindowEvent_WM_MBUTTONDBLCLK,
    PXWindowEvent_WM_MOUSEWHEEL,
    PXWindowEvent_WM_XBUTTON,
    PXWindowEvent_WM_XBUTTON
};
const PXI16U _pxWindowEventHandleListAmount = sizeof(_pxWindowEventHandleList) / sizeof(PXWindowEventHandlerFunction);

#if _windowEventListAmount != _pxWindowEventHandleListAmount
#error "Call list missmatch!"
#endif


LRESULT CALLBACK PXWindowEventCallBack(const HWND windowHandle, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    PXWindow* pxWindow = (PXWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowHandle = windowHandle;
    pxWindowEvent.WindowSender = pxWindow;
    pxWindowEvent.EventID = eventID;
    pxWindowEvent.ParamW = wParam;
    pxWindowEvent.ParamL = lParam;


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

    PXI8U index = PXMemoryCompareI16V_AVX512(_windowEventList, _windowEventListAmount, eventID);

    if(index != (PXI8U)-1)
    {
        _pxWindowEventHandleList[index](pxWindow, &pxWindowEvent);
    }

    if(!pxWindowEvent.DoReturnValue)
    {
        // If nothing had grabbed the event by now, let the default funcion handle it.
        pxWindowEvent.ReturnValue = DefWindowProc(windowHandle, eventID, wParam, lParam);
    }

    return pxWindowEvent.ReturnValue;
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

void PXAPI PXWindowRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXWindowRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXWindowRegisterInfoDynamic;
}

PXResult PXAPI PXWindowEventConsumer(PXWindowEvent PXREF pxWindowEvent)
{
    // Invoke
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXECSInfo* pxECSInfo = (PXECSInfo*)pxWindow;
    PXBool wantToIgnore = PXFalse;

    if(!pxWindow)
    {
        // Window is not registerd, this means we cant send a message back
        return PXResultOK;
    }

    if(pxWindowEvent->CallBack)
    {
        pxWindowEvent->CallBack(pxWindow, pxWindowEvent);
    }

#if 0
    if(!pxWindow->IsInFocus && wantToIgnore)
    {
        return PXActionCancelled;
    }
#endif

    PXECSInfo* callBackOwner = pxWindow->EventList.CallBackOwner;
    PXWindowEventFunction pxWindowEventFunction = pxWindow->EventList.CallBackEvent;

    if(pxWindowEventFunction)
    {
        pxWindowEventFunction(callBackOwner, pxWindowEvent);
    }

    return PXResultOK;
}

void PXAPI PXWindowEventMouseEnter(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        PXEventText,
        "Focus-Enter, (PXID:%4i)",
        pxWindow->Info.ID
    );
}

void PXAPI PXWindowEventMouseLeave(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        PXEventText,
        "Focus-Leave (PXID:%4i)",
        pxWindow->Info.ID
    );

}

void PXAPI PXWindowEventMouseClick(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXWindowEventMouseButtonData PXREF mouseMoveData = &pxWindowEvent->MouseButton;

    PXWindow PXREF windowHit = PXWindowHitTest(pxWindow, mouseMoveData->Position.X, mouseMoveData->Position.Y);

    //wantToIgnore = PXTrue;

    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        PXEventText,
        "Click, <PXID:%4i>",
        pxWindow->Info.ID
    );

    if(!windowHit)
    {
        return;
    }

    windowHit->State.Active = PXTrue;

    if(windowHit->onClick)
        windowHit->onClick(windowHit);
}

void PXAPI PXWindowEventMouseMove(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXWindowEventMouseMoveData PXREF mouseMoveData = &pxWindowEvent->MouseMove;

    //wantToIgnore = PXTrue;

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

    // Hit-Check
    PXWindow PXREF windowHit = PXWindowHitTest(pxWindow, mouseMoveData->Position.X, mouseMoveData->Position.Y);

    // Set ALL hovers invalid.
    PXWindowHitReset(pxWindow);

    // Set our current hit
    if(windowHit)
    {
        windowHit->State.Hover = PXTrue;
    }
}
void drawBox_MBP(
    PXRectangleXYWHI32 rect,
    PXRectangleLTRBI32 margin,
    PXRectangleLTRBI32 padding,
    PXI32S border // uniform border thickness
) {
    float x = rect.X;
    float y = rect.Y;
    float w = rect.Width;
    float h = rect.Height;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // --- Margin box (outermost) ---
    glColor3f(0.8f, 0.2f, 0.2f); // red
    glBegin(GL_QUADS);
    glVertex2f(x - margin.Left, y - margin.Top);
    glVertex2f(x + w + margin.Right, y - margin.Top);
    glVertex2f(x + w + margin.Right, y + h + margin.Bottom);
    glVertex2f(x - margin.Left, y + h + margin.Bottom);
    glEnd();

    // --- Border box ---
    if(border > 0)
    {
        glColor3f(0.2f, 0.2f, 0.8f); // blue
        glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
        glEnd();
    }

    // --- Padding box ---
    glColor3f(0.2f, 0.8f, 0.2f); // green
    glBegin(GL_QUADS);
    glVertex2f(x + border, y + border);
    glVertex2f(x + w - border, y + border);
    glVertex2f(x + w - border, y + h - border);
    glVertex2f(x + border, y + h - border);
    glEnd();

    // --- Content box (innermost) ---
    glColor3f(0.8f, 0.6f, 0.6f); // light gray
    glBegin(GL_QUADS);
    glVertex2f(x + border + padding.Left, y + border + padding.Top);
    glVertex2f(x + w - border - padding.Right, y + border + padding.Top);
    glVertex2f(x + w - border - padding.Right, y + h - border - padding.Bottom);
    glVertex2f(x + border + padding.Left, y + h - border - padding.Bottom);
    glEnd();
}









void PXAPI PXWindowDraw_GDI(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
    const HDC hdc = PXWindowDCGet(pxWindow);

    pxDrawInfo->DrawCall = PXWindowDrawGDI;

#if OSWindows
    RECT rect;
    GetClientRect(pxWindow->WindowHandle, &rect);
   // FillRect(hdc, &rect, pxGUITheme->BrushDarkBackground->BrushHandle);
#endif
}






typedef struct PXLineSegment_
{
    float x1, y1;
    float x2, y2;
}
PXLineSegment;

void ComputeConnectorLines(
    const PXRectangleXYWHI32* inner,
    const PXRectangleXYWHI32* outer,
    PXLineSegment lines[4])
{
    // Inner centers
    float innerCenterX = inner->X + inner->Width * 0.5f;
    float innerCenterY = inner->Y + inner->Height * 0.5f;

    // Outer centers
    float outerCenterX = outer->X + outer->Width * 0.5f;
    float outerCenterY = outer->Y + outer->Height * 0.5f;

    // TOP
    lines[0].x1 = innerCenterX;
    lines[0].y1 = inner->Y;
    lines[0].x2 = outerCenterX;
    lines[0].y2 = outer->Y;

    // BOTTOM
    lines[1].x1 = innerCenterX;
    lines[1].y1 = inner->Y + inner->Height;
    lines[1].x2 = outerCenterX;
    lines[1].y2 = outer->Y + outer->Height;

    // LEFT
    lines[2].x1 = inner->X;
    lines[2].y1 = innerCenterY;
    lines[2].x2 = outer->X;
    lines[2].y2 = outerCenterY;

    // RIGHT
    lines[3].x1 = inner->X + inner->Width;
    lines[3].y1 = innerCenterY;
    lines[3].x2 = outer->X + outer->Width;
    lines[3].y2 = outerCenterY;
}


void RenderConnectorLines(const PXLineSegment lines[4])
{
    glBegin(GL_LINES);

    for(int i = 0; i < 4; ++i)
    {
        glVertex2f(lines[i].x1, lines[i].y1);
        glVertex2f(lines[i].x2, lines[i].y2);
    }

    glEnd();
}

void PXAPI PXWindowDrawClear_GL(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
    const HDC hdc = PXWindowDCGet(pxWindow);

    pxDrawInfo->DrawCall = PXWindowDrawGL;

    //pxDrawInfo->D

    if(hdc)
    {
        const PXF32 r = 0x20 / (PXF32)0xFF;
        const PXF32 g = 0x20 / (PXF32)0xFF;
        const PXF32 b = 0x20 / (PXF32)0xFF;

        const GLbitfield clearFlags =
            //GL_POLYGON_BIT |
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT |
            GL_ACCUM_BUFFER_BIT;

        glClearColor(r, g, b, 1.0f);
        glClear(clearFlags);       
    }

    // Select the rectangle to render uinto
    PXWindowViewPortSet(pxWindow, &pxWindow->Layout.SpaceDraw, PXFalse);

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Layout",
        "\n"
        "%20s : %4i\n"
        "%20s : (LTRB) %4i %4i %4i %4i\n"
        "%20s : (LTRB) %4i %4i %4i %4i\n"
        "%20s : (XYWH) %4i %4i %4i %4i\n"
        "%20s : (XYWH) %4i %4i %4i %4i\n"
        "%20s : (XYWH) %4i %4i %4i %4i\n"
        "%20s : %6.2f\n",
        "PXID", pxWindow->Info.ID,
        "Padding", pxWindow->Layout.Padding.Left, pxWindow->Layout.Padding.Top, pxWindow->Layout.Padding.Right, pxWindow->Layout.Padding.Bottom,
        "Margin", pxWindow->Layout.Margin.Left, pxWindow->Layout.Margin.Top, pxWindow->Layout.Margin.Right, pxWindow->Layout.Margin.Bottom,
        "SpaceDraw", pxWindow->Layout.SpaceDraw.X, pxWindow->Layout.SpaceDraw.Y, pxWindow->Layout.SpaceDraw.Width, pxWindow->Layout.SpaceDraw.Height,
        "SpaceBox", pxWindow->Layout.SpaceBox.X, pxWindow->Layout.SpaceBox.Y, pxWindow->Layout.SpaceBox.Width, pxWindow->Layout.SpaceBox.Height,
        "SpaceContent", pxWindow->Layout.SpaceContent.X, pxWindow->Layout.SpaceContent.Y, pxWindow->Layout.SpaceContent.Width, pxWindow->Layout.SpaceContent.Height,
        "AspectRatio", PXWindowAspectRatio(pxWindow)
    );
#endif


    // Draw debug lines
    PXRectangleXYWHI32 PXREF spaceDraw = &pxWindow->Layout.SpaceDraw;
    PXRectangleXYWHI32 PXREF spaceBox = &pxWindow->Layout.SpaceBox;
    PXRectangleXYWHI32 PXREF spaceContent = &pxWindow->Layout.SpaceContent;


    glLineWidth(3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor3f(0.0f, 0.0f, 0.6f);
    PXWindowDrawRect2(pxWindow, spaceDraw);

    glColor3f(0.6f, 0.0f, 0.0f);
    PXWindowDrawRect2(pxWindow, spaceBox);

    glColor3f(0.6f, 0.8f, 0.0f);
    PXWindowDrawRect2(pxWindow, spaceContent);
    



    PXLineSegment pxLineSegments[4];

    glColor3f(0.6f, 1.0f, 0.6f);
    ComputeConnectorLines(spaceBox, spaceDraw, pxLineSegments);
    RenderConnectorLines(pxLineSegments);

    ComputeConnectorLines(spaceContent, spaceBox, pxLineSegments);
    RenderConnectorLines(pxLineSegments);

    glColor3f(1.0f, 1.0f, 1.0f);

#if 0
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow, 
        pxWindow->Layout.SpaceDraw.X,
        pxWindow->Layout.SpaceDraw.Y,
        pxWindow->Layout.SpaceDraw.Width,
        pxWindow->Layout.SpaceDraw.Height
    );
#endif


    //PXWindowViewPortSet(pxWindow, &pxWindow->Layout.SpaceContent, PXTrue);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*

#if 1
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow,
        -1,
        -1,
        2,
        2
    );
#else
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow, 
        pxWindow->Layout.SpaceContent.X,
        pxWindow->Layout.SpaceContent.Y,
        pxWindow->Layout.SpaceContent.Width,
        pxWindow->Layout.SpaceContent.Height
    );
#endif
*/

   // PXWindowDrawHelloWorldTriangle(pxWindow, -1, -1, 2, 2);

 


    PXCamera pxCamera;
    //pxCamera.Rectangle = pxDrawInfo->RectangleXYWH;

    //PXCameraGLFPP(&pxCamera, PXCameraPerspective2D);

    //glDrawBuffer(GL_BACK);
    //glEnableClientState(GL_COLOR_ARRAY);   
}

// Waits until all GPU instuctions are finished
void PXAPI PXWindowWaitOnRenderer(PXWindow PXREF pxWindow)
{
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
            break;
        }
    }
}

void PXAPI PXWindowEventRender(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXDrawInfo PXREF pxDrawInfo = &pxWindowEvent->Draw;


    PXWindowDrawClear_GL(pxWindow, pxDrawInfo);

    if(!pxWindow->State.DoRenderingSelf)
    {
        return;
    }

    PXECSInfo* target = pxWindow->EventList.CallBackOwner;
    PXECSDrawFunction pxECSDrawFunction = pxWindow->EventList.CallBackDraw;

    if(pxECSDrawFunction && target)
    {
        pxDrawInfo->DrawCall = pxECSDrawFunction;
    }
    else
    {
        target = pxWindow;
    }



   

    PXResult pxResult = pxDrawInfo->DrawCall(target, pxDrawInfo);

#if 0
    // Draw 
    drawBox_MBP
    (
        pxWindow->Layout.abs, 
        pxWindow->Layout.Margin,
        pxWindow->Layout.Padding,
        pxWindow->Layout.BorderWidth
    );
#endif

#if 0
    if((PXGraphicSystemOpenGL == pxWindow->GraphicSystem) && 1)
    {
        if(pxWindow->State.IsInFocus)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        else
        {
            glColor3f(1.0f, 0.0f, 0.0f);
        }

        // Optional: set line width
        glLineWidth(2.0f);

        // glScalef(0.9, 0.9, 1);

         // Draw rectangle as a line loop
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
        glVertex2f(pxDrawInfo->RectangleXYWH.X, pxDrawInfo->RectangleXYWH.Y); // bottom-left
        glVertex2f(pxDrawInfo->RectangleXYWH.X + pxDrawInfo->RectangleXYWH.Width, pxDrawInfo->RectangleXYWH.Y); // bottom-right
        glVertex2f(pxDrawInfo->RectangleXYWH.X + pxDrawInfo->RectangleXYWH.Width, pxDrawInfo->RectangleXYWH.Y + pxDrawInfo->RectangleXYWH.Height); // top-right
        glVertex2f(pxDrawInfo->RectangleXYWH.X, pxDrawInfo->RectangleXYWH.Y + pxDrawInfo->RectangleXYWH.Height); // top-left
        glEnd();

        DrawCursorMarker2D(pxDrawInfo->CursorPosition.X, -pxDrawInfo->CursorPosition.Y, 5, 5);
    }
#endif
}

void PXAPI PXWindowEventKeyboardInput(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXWindowEventKeyboardData* inputKeyboard = &pxWindowEvent->Keyboard;
    const char* buttonState = PXKeyPressStateToString(inputKeyboard->PressState);

    // wantToIgnore = PXTrue;

    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        PXEventText,
        "KeyBoard, PXID:<%i>, KeyBoard, VKey:%-3i (%lc), %s",
        pxWindow->Info.ID,
        inputKeyboard->VirtualKey,
        inputKeyboard->CharacterW,
        buttonState
    );
}

void PXAPI PXWindowEventResize(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
    PXWindowEventResizeData PXREF resizeData = &pxWindowEvent->Resize;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        PXEventText,
        "Resize, <%ix%i> (PXID:%4i)",
        resizeData->Width,
        resizeData->Height,
        pxWindow->Info.ID
    );
#endif

    PXWindowLayoutCompute(pxWindow);
}

void PXAPI PXWindowEventMoved(PXWindow PXREF pxWindow, PXWindowEvent PXREF pxWindowEvent)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        PXEventText,
        "Move"
    );
#endif 
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
    PXResult pxResult = PXResultInvalid;

#if OSUnix
    glXSwapBuffers(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxWindow->WindowHandle);
    pxResult = PXResultOK; // TODO: Do we have error checking?

#elif OSWindows
    const BOOL result = SwapBuffers(pxWindow->DeviceContextHandle);
    pxResult = PXErrorCurrent(result);
#else
    pxResult = PXActionRefusedNotSupportedByLibrary;
#endif

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDraw,
        "BufferSwap",
        "%i",
        pxResult
    );
#endif

    return pxResult;
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

PXResult PXAPI PXWindowDraw(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }




    /*
    if(!pxWindow->isVirtual)
    {
        // bind OS framebuffer
        // clear, set viewport, etc.
    }*/

    // Render root element


    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowHandle = pxWindow->WindowHandle;
    pxWindowEvent.EventID = WM_PAINT;

    PXDrawInfo* pxDrawInfo = &pxWindowEvent.Draw;
    pxDrawInfo->Engine = pxWindow->Engine;
    pxDrawInfo->Rendrer = pxWindow->Renderer;

#if 0
    pxDrawInfo->Color.Red = 0xD0;
    pxDrawInfo->Color.Green = 0xD0;
    pxDrawInfo->Color.Blue = 0xD0;
#else
    pxDrawInfo->Color.Red = pxWindow->BackGroundColor.Red;
    pxDrawInfo->Color.Green = pxWindow->BackGroundColor.Green;
    pxDrawInfo->Color.Blue = pxWindow->BackGroundColor.Blue;
#endif

    // Fetch keyboard
    LPPOINT p = (LPPOINT)&pxDrawInfo->CursorPosition;
    GetCursorPos(p);

    // Fetch mouse
    ScreenToClient(pxWindowEvent.WindowHandle, p);   // hwnd = the window currently painting


#if 0
    //PXConsoleGoToXY(0, 10);
    PXLogPrint
    (
        PXLoggingInfo,
        "Window",
        "Darw",
        "ID:%4i, %4i,%4i,%4i,%4i",
        pxWindow->Info.ID,
        pxDrawInfo->RectangleXYWH.X,
        pxDrawInfo->RectangleXYWH.Y,
        pxDrawInfo->RectangleXYWH.Width,
        pxDrawInfo->RectangleXYWH.Height
    );
#endif


    PXBool isRoot = pxWindowEvent.WindowHandle > 0;

    PAINTSTRUCT painStruct;
    HDC hdcInvalid = PXNull;
    BOOL isOK = FALSE;

    if(isRoot)
    {
        //hdcInvalid = BeginPaint(pxWindowEvent.WindowHandle, &painStruct);
    }

    PXWindowEventRender(pxWindow, &pxWindowEvent);

    if(isRoot)
    {
        //isOK = EndPaint(pxWindowEvent.WindowHandle, &painStruct);
    }

    //-----------------------------------------------------

    //-----------------------------------------------------
    // Render child virtual windows

    if(pxWindow->State.DoRenderingChildren)
    {
        PXSize childAmount = PXWindowChildAmount(pxWindow);

        for(PXSize i = 0; i < childAmount; ++i)
        {
            PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

            PXWindowDraw(child);
        }
    }
    //-----------------------------------------------------

    if(isRoot)
    {
        isOK = SwapBuffers(pxWindow->DeviceContextHandle);
    }

    // NO!
    // BOOL openglInstance = wglMakeCurrent(hdc, PXNull);

    return PXResultOK;
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
    const PXResult pxResult = PXErrorCurrent(successul);

    if(PXResultOK != pxResult)
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

        return pxResult;
    }

    return pxResult;

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

PXBool PXAPI PXWindowIsPhysical(const PXWindow PXREF pxWindow)
{
    return pxWindow->WindowHandle > 0;
}

PXResult PXAPI PXWindowVisibilitySet(PXWindow PXREF pxWindow, const PXBool doRenderingSelf, const PXBool doRenderingChildren)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    pxWindow->State.DoRenderingSelf = doRenderingSelf;
    pxWindow->State.DoRenderingChildren = doRenderingChildren;

    return PXResultOK;
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

PXResult PXAPI PXWindowRedraw(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    pxWindow->State.Dirty = PXTrue;

    // optionally bubble up to parent if you want parent to know

    PXWindow PXREF parent = PXWindowParentGet(pxWindow);

    PXWindowRedraw(parent);

#if OSUnix
    // Todo..
#elif OSWindows

    if(pxWindow->WindowHandle)
    {
        InvalidateRect(pxWindow->WindowHandle, 0, PXFalse);
        //UpdateWindow(pxWindow->WindowHandle); // This is too early!
    }

#endif

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

    if(PXScollbarBehaviourVertical & pxScollbar->Info.FlagListSettings)
    {
        flags |= SB_VERT;
    }

    if(PXScollbarBehaviourHorrizontal & pxScollbar->Info.FlagListSettings)
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
    PXResult pxResult = PXResultInvalid;

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

    if(PXWindowBrushBehaviourColorEmbeded & pxBrush->Info.FlagListSettings)
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
    PXResult pxResult = PXResultInvalid;

#if OSUnix
    pxResult = PXActionRefusedNotImplemented;
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
    pxResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

PXResult PXAPI PXWindowTextSet(PXWindow PXREF pxWindow, PXText PXREF pxText)
{
    PXResult pxResult = PXResultInvalid;

#if OSUnix
    pxResult = PXActionRefusedNotImplemented;
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
    pxResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxResult;
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

PXResult PXAPI PXWindowSpaceDrawGet(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxWindow && pxRectangle))
    {
        return PXResultRefusedParameterNull;
    }

    PXBool isPhysical = PXWindowIsPhysical(pxWindow);

    if(isPhysical)
    {
#if 0
        * pxRectangleXYWHI32 = pxWindow->Position.Form;
#else
        PXRectangleLTRBI32 pxRectangleLTRB;
        RECT* rectangle = (RECT*)&pxRectangleLTRB;
        const BOOL result = GetClientRect(pxWindow->WindowHandle, rectangle);
        pxResult = PXErrorCurrent(result);

        if(PXResultOK != pxResult)
        {
            pxRectangle->X = 0;
            pxRectangle->Y = 0;
            pxRectangle->Width = 100;
            pxRectangle->Height = 100;

            return pxResult;
        }     

        PXRectangleXYWHIFromLTRBI32(pxRectangle, &pxRectangleLTRB);
#endif
    }
    else
    {
        PXWindow* pxWindowParent = PXWindowParentGet(pxWindow);

        // Add padding
        *pxRectangle = pxWindowParent->Layout.SpaceContent;

        for(;;)
        {
            if(!pxWindowParent)
            {
                break;
            }

            PXRectangleXYWHI32AddLTRBI32(pxRectangle, &pxWindowParent->Layout.Margin);
            PXRectangleXYWHI32AddLTRBI32(pxRectangle, &pxWindowParent->Layout.Padding);          

            pxWindowParent = PXWindowParentGet(pxWindowParent);
        }       
    }

    return pxResult;
}

PXF32 PXAPI PXWindowAspectRatio(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return 1.0f;
    }

    if(0 == pxWindow->Layout.SpaceContent.Height)
    {
        return 1.0f;
    }

    return (PXF32)pxWindow->Layout.SpaceContent.Width / (PXF32)pxWindow->Layout.SpaceContent.Height;
}

PXBool PXAPI PXWindowIsValid(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXFalse;
    }

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

    return isWindowValid;
}

PXBool PXAPI PXWindowIsEnabled(const PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXFalse;
#elif OSWindows
    const BOOL success = IsWindowEnabled(pxWindow->WindowHandle); // Windows 2000, User32.dll, winuser.h
    const PXResult pxResult = PXErrorCurrent(success);

    return pxResult;
#else
    return PXFalse;
#endif
}

PXResult PXAPI PXWindowViewPortSet(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, const PXBool normalized)
{
    GLint x = pxRectangle->X;
    GLint y = pxRectangle->Y;
    GLsizei width = pxRectangle->Width;
    GLsizei height = pxRectangle->Height;

    // Width and height cant't be negative!
    if(pxRectangle->Height < 0)
    {
        y += height;
        height = -height;
    }

    if(pxRectangle->Width < 0)
    {
        x += width;
        width = -width;
    }

    GLdouble left;
    GLdouble right;
    GLdouble bottom;
    GLdouble top;
    GLdouble zNear = 0.0f; // must be positive
    GLdouble zFar = +1.0f;

    if(normalized)
    {
        left    = -1.0;
        right   = +1.0;
        bottom  = -1.0;
        top     = +1.0;
    }
    else
    {
        left = 0;
        right = width;
        bottom = height;
        top = 0;
    }

   // GLint windowHeight = pxRectangle->Height; // You must store this somewhere
   // GLint glY = windowHeight - y - height;

    glViewport
    (
        x,
        y,
        width,
        height
    );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // This sets the dimensions we use inside our rendering
    glOrtho(left, right, bottom, top, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawRectangle2D(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxWindowDrawRectangleInfo)
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
    const PXBool isHovered = PXECSInfoFlagStateCheck(&pxWindow->Info, PXECSFlagBehaviourSelected);

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

    PXDrawInfo pxWindowDrawRectangleInfoSub;
    PXClear(PXDrawInfo, &pxWindowDrawRectangleInfoSub);
    pxWindowDrawRectangleInfoSub.Brush = pxWindowBrush;
    pxWindowDrawRectangleInfoSub.Rounded = PXFalse;

    PXResult pxResult = PXWindowDrawRectangle(pxWindow, &pxWindowDrawRectangleInfoSub);

    return pxResult;
}

PXResult PXAPI PXWindowDrawRectangle3D(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxWindowDrawRectangleInfo)
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

    const PXBool isHovered = PXECSInfoFlagStateCheck(&pxWindow->Info, PXECSFlagBehaviourSelected);
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

            PXI32S x = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.X;
            PXI32S y = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Y;
            PXI32S width = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Width;
            PXI32S height = 0;// pxWindowDrawRectangleInfo->RectangleXYWH.Height;

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

            PXI32S x = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.X;
            PXI32S y = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Y;
            PXI32S width = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Width;
            PXI32S height = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Height;


            if(0)
            {
                pxOpenGL->Binding.Begin(GL_LINES);

                //pxOpenGL->Binding.Color3bv(&highColor);

                // pxOpenGL->Binding.Vertex2f(-1.0f, -1.0f);
                // pxOpenGL->Binding.Vertex2f(+1.0f, -1.0f); // Top
                // pxOpenGL->Binding.Vertex2f(-1.0f, +1.0f);
                // pxOpenGL->Binding.Vertex2f(+1.0f, +1.0f); // Left


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

PXF32 hyCol = 0;
PXF32 offsetY = 20;

PXResult PXAPI PXTextDrawInfoRowNext(PXDrawInfo PXREF pxTextDrawInfo)
{
   // pxTextDrawInfo->RectangleXYWH.Y += offsetY;

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawText
(
    PXWindow PXREF pxWindow,
    PXDrawInfo PXREF pxTextDrawInfo
)
{
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();


    if(!(pxWindow && pxTextDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }


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
            PXWindowDrawTextNative(pxWindow, pxTextDrawInfo);
            break;
        }
        case PXGraphicSystemOpenGL:
        {
            //pxTextDrawInfo->RectangleXYWH.Y += pxTextDrawInfo->RectangleXYWH.Height - 5;
            PXWindowDrawTextGLFF(pxWindow, pxTextDrawInfo);
            //pxTextDrawInfo->RectangleXYWH.Y -= pxTextDrawInfo->RectangleXYWH.Height - 5;
            break;
        }
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

PXResult PXAPI PXWindowDrawTextNative(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxTextDrawInfo)
{
    PXText PXREF pxText = pxTextDrawInfo->Text;

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

    RECT rectangle;// *(RECT*)&pxTextDrawInfo->RectangleXYWH;

   // rectangle.left += pxTextDrawInfo->RectangleXYWH.X;
   // rectangle.top += pxTextDrawInfo->RectangleXYWH.Y;

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            const int nextHeightBBB = DrawTextExA
            (
                pxWindow->DeviceContextHandle,
                pxText->A,
                pxText->SizeUsed,
                &rectangle,
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
                pxText->W,
                pxText->SizeUsed,
                &rectangle,
                format,
                PXNull
            ); // Windows 2000, User32.dll, winuser.h
            break;
        }
        default:
        {
            break;
        }
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawTextGLFF(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxTextDrawInfo)
{
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXFont PXREF fontTitle = pxGUITheme->FontTitle;
    PXGraphic PXREF pxGraphic = PXGraphicInstantiateGET();
    PXText PXREF pxText = pxTextDrawInfo->Text;

    PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;
    PXBrush PXREF brushText = pxGUITheme->BrushTextWhite;

    PXWindowFontSet(pxWindow, fontTitle);

    if(fontTitle)
    {
        if(!fontTitle->IsOK)
        {
            int glyphsIndexFirst = 0;
            int glyphsAmount = 200;// Full Unicode range
            float maximumChordalDeviation = 0;
            float extrusion = 10; // Add to Z axis, 0=No extrusion

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
                return PXResultInvalid;
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


    hyCol += 0.0008;

    GLYPHMETRICSFLOAT* glythList = fontTitle->GlyphMetricsFloat;
    PXResult pxResult = PXResultOK;

    for(PXI8U i = 0; i < 2; i++)
    {
        //glLoadIdentity();

        glPushMatrix(); // We need to store the current matrix to use a new one    

        switch(i)
        {
            case 1:
            {
                PXBool hasColor =
                    (pxTextDrawInfo->Color.Red > 0) ||
                    (pxTextDrawInfo->Color.Green > 0) ||
                    (pxTextDrawInfo->Color.Blue > 0);

                if(hasColor)
                {
                    pxOpenGL->Binding.Color3f
                    (
                        pxTextDrawInfo->Color.Red,
                        pxTextDrawInfo->Color.Green,
                        pxTextDrawInfo->Color.Blue
                    );
                }
                else
                {
                    const PXF32 x = ((PXMathSinusRADF32(hyCol) + 1) / 2) * 0.20;

                    pxOpenGL->Binding.Color3f(0.6 + x, 0.8, 0.8);
                }

                //pxOpenGL->Binding.Translatef(pxTextDrawInfo->RectangleXYWH.X, pxTextDrawInfo->RectangleXYWH.Y, 0.0f);

                break;
            }
            case 0:
            {
                pxOpenGL->Binding.Color3f(0.2f, 0.2f, 0.2f);
                pxOpenGL->Binding.Translatef
                (
                    0,//pxTextDrawInfo->RectangleXYWH.X + 1,
                    0,//pxTextDrawInfo->RectangleXYWH.Y - 1,
                    0.0f
                );
                break;
            }
            default:
            {
                break;
            }
        }

        glScalef(15.0f, -15.0f, 1.0);

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
                    pxOpenGL->Binding.CallList(fontTitle->FontBase + character);
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
                    pxOpenGL->Binding.CallList(fontTitle->FontBase + character);
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
    }


    //pxOpenGL->Binding.Translatef(0, 0, 0);
    //pxOpenGL->Binding.PopMatrix();
    pxOpenGL->Binding.PopAttrib();
    //pxOpenGL->Binding.Flush();

    return PXResultOK;
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

    return PXResultOK;

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
    const BOOL success = PolylineTo
    (
        pxWindow->DeviceContextHandle, 
        0, 
        0
    );
#else
    return PXNotSupport;
#endif
}

#include <gl/GL.h>

PXResult PXAPI PXWindowDrawHelloWorldTriangle(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
    glDisable(GL_CULL_FACE);

    glBegin(GL_TRIANGLES);

#if 0
    glColor3f(1.0f, 0.0f, 0.0f); // Rot
    glVertex2f(0.0f, 1.0f);      // Oben

    glColor3f(0.0f, 1.0f, 0.0f); // Grün
    glVertex2f(-1.0f, -1.0f);    // Unten links

    glColor3f(0.0f, 0.0f, 1.0f); // Blau
    glVertex2f(1.0f, -1.0f);     // Unten rechts
#else
    PXF32 amount = x + (width / 2);

    glColor3f(1.0f, 0.0f, 0.0f); // Rot
    glVertex2f(x, y); // Rot
  
    glColor3f(0.0f, 1.0f, 0.0f); // Grün
    glVertex2f(x + width, y); // Grün
  
    glColor3f(0.0f, 0.0f, 1.0f); // Blau
    glVertex2f(amount, y+height); // Blau    
#endif

    glEnd();

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawRect(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawRect2(PXWindow PXREF pxWindow, const PXRectangleXYWHI32 PXREF pxRectangle)
{
    glBegin(GL_QUADS);
    glVertex2i(pxRectangle->X, pxRectangle->Y);
    glVertex2i(pxRectangle->X + pxRectangle->Width, pxRectangle->Y);
    glVertex2i(pxRectangle->X + pxRectangle->Width, pxRectangle->Y + pxRectangle->Height);
    glVertex2i(pxRectangle->X, pxRectangle->Y + pxRectangle->Height);
    glEnd();
}

PXResult PXAPI PXWindowDrawRectangle(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
    PXGraphic* pxGraphic = PXGraphicInstantiateGET();
    PXOpenGL PXREF pxOpenGL = &pxGraphic->OpenGLInstance;

    //  PXGUI

    PXGUITheme* pxGUITheme = PXGUIThemeGet();
    PXBrush* brushMainPrimary = pxGUITheme->BrushMainPrimary;
    PXBrush* brushMainSecoundary = pxGUITheme->BrushMainSecoundary;
    const PXRectangleXYWHI32 PXREF pxRect = 0;// &pxDrawInfo->RectangleXYWH;

    PXResult pxResult = PXResultInvalid;
    PXGraphicSystem pxGraphicSystem = PXGraphicSystemOpenGL;

    PXColorRGBF colorPrimary;// = pxDrawInfo->Color;
    PXColorRGBF colorSecoundary = colorPrimary;

    if(PXColorRGBFIsBlack(&colorPrimary))
    {
        colorPrimary.Red = brushMainPrimary->ColorDate.Red;
        colorPrimary.Green = brushMainPrimary->ColorDate.Green;
        colorPrimary.Blue = brushMainPrimary->ColorDate.Blue;

        colorPrimary.Red = 0x20;
        colorPrimary.Green = 0;
        colorPrimary.Blue = 0;
    }

    if(PXColorRGBFIsBlack(&colorSecoundary))
    {
        colorSecoundary.Red = brushMainSecoundary->ColorDate.Red;
        colorSecoundary.Green = brushMainSecoundary->ColorDate.Green;
        colorSecoundary.Blue = brushMainSecoundary->ColorDate.Blue;

        colorSecoundary.Red = 0x20;
        colorSecoundary.Green = 0x20;
        colorSecoundary.Blue = 0;
    }

    //&brushMainSecoundary->ColorDate


    if(pxWindow)
    {
        pxGraphicSystem = pxWindow->GraphicSystem;
    }

    switch(pxGraphicSystem)
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

            RECT* rect = 0;// (RECT*)&pxDrawInfo->RectangleXYWH;

            if(pxDrawInfo->Rounded)
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
                HBRUSH brush = NULL;

                if(pxDrawInfo->Brush)
                {
                    brush = pxDrawInfo->Brush->BrushHandle;
                }
                else
                {
                    brush = GetSysColorBrush(COLOR_WINDOW);
                }

                const int resultID = FillRect
                (
                    pxWindow->DeviceContextHandle,
                    rect,
                    brush
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
                0,//pxDrawInfo->RectangleXYWH.X,
                0,//pxDrawInfo->RectangleXYWH.Y,
                0,//pxDrawInfo->RectangleXYWH.Width,
                0//pxDrawInfo->RectangleXYWH.Height
            );
#endif

            break;
        }
        case PXGraphicSystemOpenGL:
        {
            //PXGUIProperty pxGUIProperty;
            //pxGUIProperty.Property = PXUIElementPropertySize;
            //PXGUIPropertyFetch(pxWindow, &pxGUIProperty, 1, PXFalse);

            //  PXRectangleXYWHI32ToVertex(&pxRectangleXYWHI32, &pxGUIProperty.Size, &pxRectangleVertexF32);

              //glEnable(GL_BLEND);
              //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            pxOpenGL->Binding.Enable(GL_LINE_SMOOTH);
            pxOpenGL->Binding.Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);


            // Draws a rectangle at (x, y) with width w and height h

            /*
                glBegin(GL_QUADS);
                glVertex2f(pxRect->X,       pxRect->Y);      // bottom-left
                glVertex2f(pxRect->X + pxRect->Width,   pxRect->Y);      // bottom-right
                glVertex2f(pxRect->X + pxRect->Width,   pxRect->Y + pxRect->Height);  // top-right
                glVertex2f(pxRect->X,       pxRect->Y + pxRect->Height);  // top-left
                glEnd();
                */




                // Draw gradient background
            GLenum renderModeList[2] = { GL_FILL , GL_LINE };

            if(1)
            {
                pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                pxOpenGL->Binding.Begin(GL_QUADS);
                pxOpenGL->Binding.Color3fv(&colorPrimary);
                pxOpenGL->Binding.Vertex2f(pxRect->X, pxRect->Y); // +1.0f - offset, 1.0f
                pxOpenGL->Binding.Vertex2f(pxRect->X + pxRect->Width, pxRect->Y); // -1.0f + offset, 1.0f
                pxOpenGL->Binding.Color3fv(&colorSecoundary);
                pxOpenGL->Binding.Vertex2f(pxRect->X + pxRect->Width, pxRect->Y + pxRect->Height); // -1.0f + offset, -1.0f
                pxOpenGL->Binding.Vertex2f(pxRect->X, pxRect->Y + pxRect->Height); // +1.0f - offset, -1.0f
                pxOpenGL->Binding.End();
            }

            if(1)
            {
                pxOpenGL->Binding.LineWidth(1);
                pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                pxOpenGL->Binding.Begin(GL_QUADS);
                pxOpenGL->Binding.Color3b(0x00, 0xFF, 0x00);
                pxOpenGL->Binding.Vertex2f(pxRect->X, pxRect->Y); // +1.0f - offset, 1.0f
                pxOpenGL->Binding.Vertex2f(pxRect->X + pxRect->Width, pxRect->Y); // -1.0f + offset, 1.0f     
                pxOpenGL->Binding.Vertex2f(pxRect->X + pxRect->Width, pxRect->Y + pxRect->Height); // -1.0f + offset, -1.0f
                pxOpenGL->Binding.Vertex2f(pxRect->X, pxRect->Y + pxRect->Height); // +1.0f - offset, -1.0f
                pxOpenGL->Binding.End();
            }


            pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);




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

PXResult PXAPI PXWindowDrawIcon(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo, PXIcon PXREF pxIcon)
{
    if(!(pxWindow && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(1)
    {
        PXWindowDrawRectangle(pxWindow, pxDrawInfo);
    }

    if(!pxIcon)
    {
        return PXResultOK;
    }

    PXRectangleXYWHI32 PXREF pxRect = 0;// &pxDrawInfo->RectangleXYWH;

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicSystemNative:
        {
#if OSUnix
            return PXActionRefusedNotImplemented;
#elif OSWindows
            const BOOL result = DrawIconEx
            (
                pxWindow->DeviceContextHandle,
                pxRect->X,
                pxRect->Y,
                pxIcon->IconHandle,
                pxRect->Width,
                pxRect->Height,
                0,
                0,
                DI_NORMAL
            );
#else
            return PXActionRefusedNotSupportedByOperatingSystem;
#endif
            break;
        }
        case PXGraphicSystemOpenGL:
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, pxIcon->Texture->OpenGLID);

            glColor3f(1.0f, 1.0f, 1.0f); // full brightness

            glBegin(GL_QUADS);

            // Bottom-left
            glTexCoord2f(0.0f, 1.0f);
            glVertex2i(pxRect->X, pxRect->Y);

            // Bottom-right
            glTexCoord2f(1.0f, 1.0f);
            glVertex2i(pxRect->X + pxRect->Width, pxRect->Y);

            // Top-right
            glTexCoord2f(1.0f, 0.0f);
            glVertex2i(pxRect->X + pxRect->Width, pxRect->Y + pxRect->Height);

            // Top-left
            glTexCoord2f(0.0f, 0.0f);
            glVertex2i(pxRect->X, pxRect->Y + pxRect->Height);

            glEnd();

            glDisable(GL_TEXTURE_2D);

            break;
        }
        default:
        {
            break;
        }
    }

    return PXResultOK;
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
    const PXResult pxResult = PXErrorCurrent(regsiterRawImputSuccess);

    if(PXResultOK != pxResult)
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

        return pxResult;
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
    PXResult pxResult = PXResultInvalid;

#if OSUnix
    pxResult = PXActionRefusedNotImplemented;
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
    pxResult = PXActionRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

PXResult PXAPI PXWindowCreate(PXWindow** pxWindowREF, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    pxWindowCreateInfo->DoOpenGL = PXTrue;

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

    // if we only have the parrent name, try to get actual parent via name
    if(pxWindowCreateInfo->WindowParentName.A)
    {
        pxWindowCreateInfo->WindowParent = PXWindowGetViaName(&pxWindowCreateInfo->WindowParentName);
    }

    PXWindow* pxWindow = *pxWindowREF;
    PXWindow* pxWindowParent = pxWindowCreateInfo->WindowParent;

    // Always add them
    pxWindowCreateInfo->Info.Static = &PXWindowRegisterInfoStatic;
    pxWindowCreateInfo->Info.Dynamic = &PXWindowRegisterInfoDynamic;




    char name[32];

    if(!pxWindow)
    {
        PXResult pxResult = PXECSCreate(pxWindowREF, pxWindowCreateInfo);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }

        pxWindow = *pxWindowREF;
        pxWindow->Hierarchy.Parent = pxWindowParent;

        pxWindow->State = pxWindowCreateInfo->State;
        pxWindow->Layout = pxWindowCreateInfo->Layout;

        pxWindow->State.DoRenderingSelf = PXTrue;
        pxWindow->State.DoRenderingChildren = PXTrue;

        PXBool hasMargin =
            pxWindowCreateInfo->Layout.Margin.Left > 0 ||
            pxWindowCreateInfo->Layout.Margin.Right > 0 ||
            pxWindowCreateInfo->Layout.Margin.Top > 0 ||
            pxWindowCreateInfo->Layout.Margin.Bottom > 0;

        PXBool hasPadding =
            pxWindowCreateInfo->Layout.Padding.Left > 0 ||
            pxWindowCreateInfo->Layout.Padding.Right > 0 ||
            pxWindowCreateInfo->Layout.Padding.Top > 0 ||
            pxWindowCreateInfo->Layout.Padding.Bottom > 0;

        if(!hasMargin && pxWindowParent)
        {
            pxWindow->Layout.Margin.Left = 20;
            pxWindow->Layout.Margin.Right = 20;
            pxWindow->Layout.Margin.Top = 20;
            pxWindow->Layout.Margin.Bottom = 20;
        }

        if(!hasPadding && pxWindowParent)
        {
            pxWindow->Layout.Padding.Left = 20;
            pxWindow->Layout.Padding.Right = 20;
            pxWindow->Layout.Padding.Top = 20;
            pxWindow->Layout.Padding.Bottom = 20;
        }

        PXListCreate(&pxWindow->Hierarchy.Children, sizeof(PXWindow*), 32);

        if(pxWindowParent)
        {
            pxWindow->Engine = pxWindowParent->Engine;
            pxWindow->Renderer = pxWindowParent->Renderer;

            PXWindowChildAdd(pxWindowParent, &pxWindow);
        }

        if(pxWindowCreateInfo->Engine)
        {
            pxWindow->Engine = pxWindowCreateInfo->Engine;
        }

        if(pxWindowCreateInfo->Renderer)
        {
            pxWindow->Renderer = pxWindowCreateInfo->Renderer;
        }


        if(PXGraphicSystemInvalid == pxWindow->GraphicSystem)
        {
            pxWindow->GraphicSystem = PXGraphicSystemOpenGL;
        }

        // These settings are only valid on startup, so we set them here ONCE
        pxWindow->Type = pxWindowCreateInfo->Type;
        pxWindow->EventList = pxWindowCreateInfo->EventList;
        pxWindow->Info.FlagListState |= pxWindowCreateInfo->Behaviour;
        pxWindow->Info.FlagListSettings |= pxWindowCreateInfo->Setting;
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



        const PXBool isDefaultSize = 1;// windowInfo->Width == 0 && windowInfo->Height == 0;

        if(isDefaultSize)
        {
            PXNativDrawScreenSizeGet(&pxWindow->Layout.SpaceContent);

            // Scale down
            //pxWindow->abs.X *= 0.125f;
            //pxWindow->abs.Y *=  0.125f;
            pxWindow->Layout.SpaceContent.Width *= 0.8f;
            pxWindow->Layout.SpaceContent.Height *= 0.8f;

            // Center
            pxWindow->Layout.SpaceContent.X = pxWindow->Layout.SpaceContent.Width / 8;
            pxWindow->Layout.SpaceContent.Y = pxWindow->Layout.SpaceContent.Height / 8;
        }
    }


    // Normally, we want to create a physical window from the OS.
    // But.. if we want to spawn our own buttons and elements, we can also create a "fake" one.
    // This decreses the amount of "managing" code and lets the UI scale way better.
    // For this fake window, we just create a framebuffer
    if(pxWindowCreateInfo->UseFrameBuffer)
    {
        PXWindowCreateFrameBuffer(pxWindow, pxWindowCreateInfo);
    }

    if(pxWindowCreateInfo->UsePhysical)
    {
        PXWindowCreatePhysical(pxWindow, pxWindowCreateInfo);
    }

    if(pxWindowCreateInfo->DoOpenGL)
    {
        PXWindowOpenGLEnable(pxWindow);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingWarning,
        PXWindowTextText,
        "Create",
        "PXID:%i, %.*s, creation done",
        pxWindow->Info.ID,
        pxWindowCreateInfo->Info.Static->NameOfType.SizeUsed,
        pxWindowCreateInfo->Info.Static->NameOfType.A
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowCreatePhysical(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    PXWindow PXREF pxWindowParent = PXWindowParentGet(pxWindow);

#if PXLogEnable
    if(pxWindowParent)
    {
        PXRectangleXYWHI32 pxRectangleParent;

        PXWindowSpaceDrawGet(pxWindowParent, &pxRectangleParent);

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
            (int)pxWindow->Layout.SpaceContent.X,
            (int)pxWindow->Layout.SpaceContent.Y,
            (int)pxWindow->Layout.SpaceContent.Width,
            (int)pxWindow->Layout.SpaceContent.Height,
            pxWindowCreateInfo->WindowText.A,
            "Parent",
            pxWindowParent->Info.ID,
            pxRectangleParent.X,
            pxRectangleParent.Y,
            pxRectangleParent.Width,
            pxRectangleParent.Height,
            "---"
        );

    }
    else
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXWindowTextText,
            "Create",
            "Attempting to create global window..\n"
            "%10s PX-ID:%i, X:%4i Y:%4i W:%4i H:%4i, Name:%s (%i)",
            "",
            pxWindow->Info.ID,
            (int)pxWindow->Layout.SpaceContent.X,
            (int)pxWindow->Layout.SpaceContent.Y,
            (int)pxWindow->Layout.SpaceContent.Width,
            (int)pxWindow->Layout.SpaceContent.Height,
            pxWindowCreateInfo->WindowText.A,
            pxWindowCreateInfo->WindowText.SizeUsed
        );
    }
#endif

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
        PXECSInfoFlagStateRemove(&pxWindow->Info, PXECSInfoUseByMask);
        PXECSInfoFlagStateAdd(&pxWindow->Info, PXECSInfoUseByEngine);

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

#endif

    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
    // pxWindow->Type == PXUIElementTypeText&&;

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
                pxWindow->Layout.SpaceContent.X,
                pxWindow->Layout.SpaceContent.Y,
                pxWindow->Layout.SpaceContent.Width,
                pxWindow->Layout.SpaceContent.Height,
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
                pxWindowCreateInfo->WindowClassName.W,
                pxWindowCreateInfo->WindowText.W, // Text content
                pxWindowCreateInfo->StyleFlags,
                pxWindow->Layout.SpaceContent.X,
                pxWindow->Layout.SpaceContent.Y,
                pxWindow->Layout.SpaceContent.Width,
                pxWindow->Layout.SpaceContent.Height,
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
#endif

    // We need to readjust the window size, as the titlebar does reduce our actual rendering size
    PXWindowSpaceDrawGet(pxWindow, &pxWindow->Layout.SpaceContent);


    PXECSInfoFlagStateAdd
    (
        &pxWindow->Info,
        PXECSFlagBehaviourActive |
        PXECSFlagBehaviourCanRender |
        PXECSInfoUseByOS
    );

    //pxWindow->Position.Form = pxWindowCreateInfo->Size;

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


    //UpdateWindow(pxWindow->WindowHandle);
    //ShowWindow(pxWindow->WindowHandle, SW_SHOWNORMAL);


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

    return PXResultOK;
}

PXResult PXAPI PXWindowCreateFrameBuffer(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    PXRectangleXYWHI32 pxRectangle;

    PXWindowSpaceDrawGet(pxWindow, &pxRectangle);

    PXFrameBufferCreateInfo pxFrameBufferCreateInfo;
    PXClear(PXFrameBufferCreateInfo, &pxFrameBufferCreateInfo);
    pxFrameBufferCreateInfo.System = PXGraphicSystemNative;
    pxFrameBufferCreateInfo.Width = pxRectangle.Width;
    pxFrameBufferCreateInfo.Height = pxRectangle.Height;

    const PXResult pxResult = PXFrameBufferCreate(&pxWindow->FrameBuffer, &pxFrameBufferCreateInfo);

    return pxResult;
}

#if 0
PXResult PXAPI PXWindowPositionCalculcate(PXWindow PXREF pxWindow, PXWindowPositionCalulcateInfo PXREF pxUIElementPositionCalulcateInfo)
{
    if(!(pxWindow && pxUIElementPositionCalulcateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    if(!((PXWindowAllignIgnoreParentMargin & pxWindow->Info.FlagListSettings) > 0))
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

    // pxUIElementPositionCalulcateInfo->Margin.Left += pxWindow->Margin.X;
   //  pxUIElementPositionCalulcateInfo->Margin.Top += pxWindow->Margin.Y;
    // pxUIElementPositionCalulcateInfo->Margin.Right += pxWindow->Margin.Height;
    // pxUIElementPositionCalulcateInfo->Margin.Bottom += pxWindow->Margin.Bottom;

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
    if(PXWindowKeepWidth & pxWindow->Info.FlagListSettings)
    {
        //   pxUIElementPositionCalulcateInfo->Size.Width = pxWindow->Position.Form.Width;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Size.Width = mathWithScaling;
    }

    if(PXWindowKeepHeight & pxWindow->Info.FlagListSettings)
    {
        //  pxUIElementPositionCalulcateInfo->Size.Height = pxWindow->Position.Form.Height;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Size.Height = heightWithScaling;
    }


    switch(PXWindowAllignFlags & pxWindow->Info.FlagListSettings)
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
#endif








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

#if OSUnix
#elif OSWindows
HDC PXAPI PXWindowDCGet(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    if(!pxWindow->WindowHandle)
    {
        return PXNull;
    }

    if(pxWindow->DeviceContextHandle)
    {
        return pxWindow->DeviceContextHandle;
    }

    pxWindow->DeviceContextHandle = GetDC(pxWindow->WindowHandle);

    // Sometimes we do want NULL? How to handle this??
    //PXAssert(pxWindow->DeviceContextHandle, "The device context can only be NULL if the window handle is invalid");

    return pxWindow->DeviceContextHandle;
}

HWND PXAPI PXWindowHandleGet(PXWindow PXREF pxWindow)
{
    return pxWindow->WindowHandle;
}

PXWindow* PXAPI PXWindowFromHandle(const HWND windowHandle)
{
    return (PXWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
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




static void LayoutDockedAA(PXWindow PXREF pxWindow)
{
    PXRectangleXYWHI32 pxRectangleXYWH;
    PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWH);

    RECT rc;// = PXWindowRectOf(pxWindow->WindowHandle);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;
    const int bandW = 260;  // left/right band width
    const int bandH = 180;  // top band height

    for(HWND child = GetWindow(pxWindow->WindowHandle, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT))
    {
        PXWindow* p = PXWindowFromHandle(child);
        if(!p)
            continue;

        RECT r = { 0 };
        switch(p->State.DockSide) {
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
    PXRectangleXYWHI32 pxRectangleXYWH;
    PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWH);

    RECT rc;// = PXWindowRectOf(pxWindow->WindowHandle);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;

    const int bandW = 260;  // left/right band width (could be per-pane)
    const int bandH = 180;  // top/bottom band height (could be per-pane)

    // PASS 1: LEFT/RIGHT
    for(HWND child = GetWindow(pxWindow->WindowHandle, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT))
    {
        PXWindow* p = PXWindowFromHandle(child);

        if(!p)
            continue;

        RECT r;
        switch(p->State.DockSide)
        {
            case PXWindowDockSideLeft:
                r.left = leftX;
                r.right = leftX + bandW;
                r.top = rc.top;
                r.bottom = rc.bottom; // full height
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
        PXWindow* p = PXWindowFromHandle(child);
        if(!p) continue;

        RECT r;
        switch(p->State.DockSide) {
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
        PXWindow* p = PXWindowFromHandle(child);
        if(!p) continue;

        if(p->State.DockSide == PXWindowDockSideNone /*DOCK_FILL  or your default/none enum */) {
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
        PXWindow* p = PXWindowFromHandle(child);
        if(!p || p->State.Floating) continue;
        ++total;
    }
    return total;
}



static int CollectSide(HWND hParent, PXWindowDockSide side, Paneref* buf, int cap)
{
    int n = 0;
    for(HWND child = GetWindow(hParent, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
        if(!IsWindowVisible(child)) continue;
        PXWindow* p = PXWindowFromHandle(child);
        if(!p || p->State.Floating) continue;
        if(p->State.DockSide != side) continue;
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
        PXI32S mBefore = vertical ? p->Layout.Margin.Top : p->Layout.Margin.Left;
        PXI32S mAfter = vertical ? p->Layout.Margin.Bottom : p->Layout.Margin.Right;
        PXI32S padBefore = vertical ? p->Layout.Padding.Top : p->Layout.Padding.Left;
        PXI32S padAfter = vertical ? p->Layout.Padding.Bottom : p->Layout.Padding.Right;
        PXI32S axisExtra = mBefore + mAfter + padBefore + padAfter;

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
        PXI32S mBefore = vertical ? p->Layout.Margin.Top : p->Layout.Margin.Left;
        PXI32S mAfter = vertical ? p->Layout.Margin.Bottom : p->Layout.Margin.Right;
        PXI32S padBefore = vertical ? p->Layout.Padding.Top : p->Layout.Padding.Left;
        PXI32S padAfter = vertical ? p->Layout.Padding.Bottom : p->Layout.Padding.Right;
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
static void ApplyStackWithBox
(
    HWND hParent,
    HDWP* phdwp,
    RECT area,
    BOOL vertical,
    Paneref* arr,
    int n,
    int spacing
)
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
        int mL = p->Layout.Margin.Left;
        int mT = p->Layout.Margin.Top;
        int mR = p->Layout.Margin.Right;
        int mB = p->Layout.Margin.Bottom;

        int padL = p->Layout.Padding.Left;
        int padT = p->Layout.Padding.Top;
        int padR = p->Layout.Padding.Right;
        int padB = p->Layout.Padding.Bottom;

        // SLOT rect (includes margins and padding)
        int x = area.left;
        int   y = area.top;
        int    w = area.right - area.left;
        int   h = area.bottom - area.top;


        if(vertical)
        {
            y = cursor;
            h = slotSizes[i];
        }
        else
        {
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

        if(*phdwp)
        {
            *phdwp = DeferWindowPos(*phdwp, arr[i].h, NULL, x, y, w, h, SWP_FLAGS);
        }
        else
        {
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
#if PXWindowLayoutCalc
    return;
#endif

    if(!(pxWindow && pCfg))
    {
        return;
    }

    DockLayoutConfig cfg = pCfg ? *pCfg : DockDefaultConfig();

    PXRectangleXYWHI32 pxRectangleXYWH;
    PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWH);

    RECT rc;// = PXWindowRectOf(pxWindow->WindowHandle);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;

    int total = CountVisiblePanes(pxWindow->WindowHandle);

    if(total <= 0)
        return;

    HDWP hdwp = BeginDeferWindowPos(total);
    const int spacingDefault = cfg.defaultSpacing;

    Paneref* buf = (Paneref*)HeapAlloc(GetProcessHeap(), 0, sizeof(Paneref) * (total > 0 ? total : 1));
    if(!buf)
    {
        if(hdwp)
            EndDeferWindowPos(hdwp); return;
    }

    // -----------------------------
    // PASS 1: LEFT band(s)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->WindowHandle, PXWindowDockSideLeft, buf, total);

        if(n > 0)
        {
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

        if(n > 0)
        {
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
    int limitDockSideRight = botomRight.x - marginX;

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

    PXRectangleXYWHI32 pxRectangleXYWH;
    PXWindowSpaceDrawGet(pxWindow, &pxRectangleXYWH);

    RECT m;// = PXWindowRectOf(pxWindowRoot->WindowHandle);
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
    if(pxWindow->State.Floating)
        return;

    PXBool reactGL = pxWindow->RenderContext > 0;

    PXWindowDestroy(pxWindow);

    PXWindowCreateInfo pxWindowCreateInfo;
    PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
    pxWindowCreateInfo.State.Floating = TRUE;
    pxWindowCreateInfo.State.DockSide = PXWindowDockSideNone;
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
    if(pxWindow->State.Floating)
        return;

    PXBool reactGL = pxWindow->RenderContext > 0;

    PXWindowDestroy(pxWindow);

    PXWindowCreateInfo pxWindowCreateInfo;
    PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
    pxWindowCreateInfo.WindowParent = PXWindowParentGet(pxWindow);
    pxWindowCreateInfo.State.Floating = FALSE;
    pxWindowCreateInfo.State.DockSide = side;
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

#endif
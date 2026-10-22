#include "PXWindow.h"

//---------------------------------------------------------
// Ours
//---------------------------------------------------------
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include <PX/Type/PXFont.h>
#include <PX/Type/PXFrameBuffer.h>
#include <PX/OS/PXOS.h>
#include <PX/Type/PXBrush.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Input/PXInput.h>
#include <PX/Container/List/PXList.h>
#include <PX/Engine/ECS/PXNamePool.h>
#include <PX/Container/ListHierarchical/PXListHierarchical.h>
#include <PX/Type/PXText.h>
#include <PX/Math/PXCollision.h>
#include <PX/Type/PXDisplay.h>
#include <PX/Engine/PXEngineUI.h>
#include <PX/Engine/PXEngine.h>
//---------------------------------------------------------



//---------------------------------------------------------
// Standrad libs
//---------------------------------------------------------
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <GL/gl.h>
//---------------------------------------------------------


//---------------------------------------------------------
// OS-Libs
//---------------------------------------------------------
#if OSUnix
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XInput2.h> // XI_RawMotion
#elif OSWindows
#include <Windows.h>
#include <CommCtrl.h>
#include <wingdi.h>
#include <windowsx.h>
#include <Dbt.h>
#include <uxtheme.h>   // optional, not required
#include <dwmapi.h>    // optional, not required, Set tilebar color
#include <dinput.h>
#include <WinUser.h>
#include <wtypes.h>
#include <ole2.h> // Object Linking and Embedding
#include <shellapi.h>
#include <PX/Engine/PXEngineMath.h>

//#include <gdiplusgraphics.h>
//#include <ddrawgdi.h>
//#include <hidusage.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Msimg32.lib")


#define Windows10DarkModeID 20 // DWMWA_USE_IMMERSIVE_DARK_MODE
const char WindowsLibraryDWMAPI[] = "DWMAPI.DLL";
const char WindowsLibraryDWMAPISET[] = "DwmSetWindowAttribute";
typedef HRESULT(WINAPI* PXDwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, _In_reads_bytes_(cbAttribute) LPCVOID pvAttribute, DWORD cbAttribute);

ATOM _pxEngineBaseClassID = 0;
HKL _keyBoardLayout = PXNull; // Selected keyboardlayout in taskbar
wchar_t _keyBoardLayoutName[KL_NAMELENGTH];

#endif
//---------------------------------------------------------




const char PXEventText[] = "Event";

const char PXTextFailsafe[] = "<missing text>";
const PXI8U PXTextFailsafeLength = sizeof(PXTextFailsafe);

#define PXWindowLayoutCalc 1


typedef PXResult (PXAPI* PXWindowOnDrawFunction)(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo);
typedef PXResult (PXAPI* PXWindowOnClickFunction)(PXWindow PXREF pxWindow);
typedef PXResult (PXAPI* PXWindowOnLayoutFunction)(PXWindow PXREF pxWindow); // optional custom layout

typedef struct PXWindow_
{
    PXECSInfo Info;
    PXHierarchicalNode Hierarchy;
    PXWindowLayout Layout;
    PXWindowState State;

    PXF32 DPIScaleFactor; // DPI / 96

    //----
    UIWindow UIData;
    UIState UIDataState;
    //----

    //-----------------------------------------------------
    void* CallbackOwner;
    PXWindowOnDrawFunction OnDraw;
    PXWindowOnClickFunction OnClick;
    PXWindowOnLayoutFunction OnLayout;
    //-----------------------------------------------------




    PXGraphicAPI GraphicSystem; // The window can be drawn by different systems.

    // Used as the render sink target
    PXFrameBuffer* FrameBuffer;

    PXWindowHandle Handle;
    PXWindowPixelBuffer PixelBuffer;
    PXOpenGLContext GLContext;

#if OSUnix

#elif OSWindows
    PXBuffer InputBuffer; // Windows want a buffer like this
#endif

    PXWindowEventList EventList;

    // PXEngineUIElementPosition Position;

     //---<State-Info>------------------------
    // PXWindowBrush* BrushFront;
    // PXWindowBrush* BrushBackground;
    PXFont* FontForText;
    PXIcon* Icon; // Icon to be rendered

    //PXColorRGBA_F32* ColorTintReference; // Point to a color to be able to share a theme. Can be null, equal to plain white.
   // PXEngineUIHoverState Hover;
    PXI32U FlagsList;
    //---------------------------------------


    //---[Extended data]---------------------
    PXEngineUIElementType Type;
    void* ExtendedData; // Extra data that will be allocated seperatly for additional use
    //---------------------------------------

    PXColorRGB_U8 BackGroundColor;

 

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
    PXEngineUI* EngineUI;
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


typedef struct PXWindowSearchResult_
{
    PXText* Name;
    PXWindow* Target;

    PXI32U g_OurProcessId;
}
PXWindowSearchResult;

PXWindow* PXAPI PXWindowRootGet(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXNull;
    }

#if 0 // Search

    HWND rootOwner = GetAncestor(pxWindow->Handle.Data, GA_PARENT);

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

    #if OSUnix
    #elif OSWindows
       LONG style = GetWindowLong(pxWindow->Handle.Data, GWL_STYLE);

    // A top-level window has no WS_CHILD and no parent
    if(!(style & WS_CHILD) && GetParent(pxWindow->Handle.Data) == NULL)
        return TRUE;
    return FALSE;
    #endif
}

PXResult PXAPI PXWindowOpenGLEnable(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    const PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    if(!pxWindowHandle.Data)
    {
        return PXResultRefusedWindowDoesNotExist;
    }

#if OSUnix
#elif OSWindows

    // Check if we are a virtual window.
    // If no window handle exists, or any
    // framebuffer, we cant do anything here
    const HDC windowDeviceContextHandle = PXWindowDCGet(pxWindow);

    // "Enable" openglk
    pxWindow->GraphicSystem = PXGraphicAPIOpenGL;

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

    HGLRC glContextNew = wglCreateContext(windowDeviceContextHandle);
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
        HGLRC glContextParent = windowParent->GLContext.Data;

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

    pxWindow->GLContext.Data = glContextNew;

    isOK = wglMakeCurrent(windowDeviceContextHandle, glContextNew);

    // NO!
    //isOK = wglMakeCurrent(pxWindow->DeviceContextHandle, 0);

#endif // OSUnix

    return PXResultOK;
}







// Draw a hitbox + diamond + cross at cursor position in OpenGL 1.1
// mouseX, mouseY are in window coordinates (0,0 = top-left)
// winW, winH are the window size

PXResult PXAPI DrawCursorMarker2D(int mouseX, int mouseY, int winW, int winH)
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

#if OSWindows
void PXAPI PXWindowPaintPattern(HWND hwnd, HDC hdc, RECT* rect, PXColorRGB_U8 PXREF color)
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
#endif // OSWindows

void PXAPI PXWindowPaintMain(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicAPINative:
        {
            #if OSUnix
            #elif OSWindows

            RECT rectangle;
            GetClientRect(pxWindow->Handle.Data, &rectangle);

            PXColorRGB_U8 color = { 28 , 28 , 28 };

            PXWindowPaintPattern
            (
                pxWindow->Handle.Data,
                pxWindow->PixelBuffer.Data,
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
  #endif // OSUnix

            break;
        }
        case PXGraphicAPIOpenGL:
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

PXSize PXAPI PXWindowSiblingAmount(const PXWindow PXREF pxWindow)
{
    PXSize amount = 0;

    if(!pxWindow)
    {
        return amount;
    }

    amount = PXHierarchicalNodeSiblingAmount(&pxWindow->Hierarchy);

    return amount;
}

PXWindow* PXAPI PXWindowSiblingGet(PXWindow PXREF pxWindow, const PXSize index)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    PXWindow PXREF pxWindowChild = (PXWindow*)PXHierarchicalNodeGetSiblingViaIndex(&pxWindow->Hierarchy, index);

    return pxWindowChild;
}

PXSize PXAPI PXWindowChildAmount(const PXWindow PXREF pxWindow)
{
    PXSize amount = 0;

    if(!pxWindow)
    {
        return amount;
    }

    amount = PXHierarchicalNodeChildAmount(&pxWindow->Hierarchy);

    return amount;
}

PXWindow* PXAPI PXWindowChildGet(const PXWindow PXREF pxWindow, const PXSize index)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    PXWindow PXREF pxWindowChild = (PXWindow*)PXHierarchicalNodeGetChildViaIndex(&pxWindow->Hierarchy, index);

    return pxWindowChild;
}

PXResult PXAPI PXWindowChildAdd(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild)
{
    if(!(pxWindow && pxWindowChild))
    {
        return PXResultRefusedParameterNull;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        "Hierarchy",
        "%p PXID:<%4i> -> %p PXID:<%4i>",
        pxWindow,
        pxWindow->Info.ID,
        pxWindowChild,
        pxWindowChild->Info.ID
    );
#endif

    PXHierarchicalNodeAddChild(&pxWindow->Hierarchy, &pxWindowChild->Hierarchy);

    PXWindowLayoutCompute(pxWindow);

    return PXResultOK;
}

PXResult PXAPI PXWindowChildRemove(PXWindow PXREF pxWindow, PXWindow PXREF pxWindowChild)
{
    if(!(pxWindow && pxWindowChild))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXHierarchicalNodeRemoveChild(&pxWindow->Hierarchy, &pxWindowChild->Hierarchy);

    return pxResult;
}

PXResult PXAPI PXWindowParentSet(PXWindow PXREF pxWindow, const PXWindow PXREF pxWindowParent)
{
    if(!(pxWindow && pxWindowParent))
    {
        return PXResultRefusedParameterNull;
    }

    const PXBool isOverWritingParent = pxWindow->Hierarchy.Parent > 0;

    PXHierarchicalNodeSetParent(&pxWindow->Hierarchy, &pxWindowParent->Hierarchy);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingEvent,
        PXWindowTextText,
        "Hierarchy",
        "%p PXID:<%4i> <- %p PXID:<%4i>",
        pxWindow,
        pxWindow->Info.ID,
        pxWindowParent,
        pxWindowParent->Info.ID
    );
#endif

    return PXResultOK;
}

PXWindow* PXAPI PXWindowParentGet(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    PXWindow PXREF parent = (PXWindow*)PXHierarchicalNodeGetParent(&pxWindow->Hierarchy);

    return parent;
}

#if OSUnix
Window PXAPI PXWindowParentHandleGet(const PXWindow PXREF pxWindow, const Display PXREF xDisplay)
{
    PXWindow PXREF pxWindowParrent = PXWindowParentGet(pxWindow);
    Window xWindowHandle = 0;

    if(pxWindowParrent)
    {
        PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

        xWindowHandle = pxWindowHandle.Data;
    }
    else
    {
        xWindowHandle = DefaultRootWindow(xDisplay);
    }

    return xWindowHandle;
}
#endif

PXWindow* PXAPI PXWindowGetViaName(PXText PXREF pxText)
{
    PXWindow PXREF pxWindow = (PXWindow*)PXECSEntryGetViaName(pxText);

#if OSWindows && 0
    //-----------------------------------------------------
    // Use the OS pathway to find the window.
    // This is redundant
    WNDENUMPROC function = (WNDENUMPROC)EnumMyWindowsProc;
    LPARAM parameter = (LPARAM)&pxWindowSearchResult;

    pxWindowSearchResult.g_OurProcessId = GetCurrentProcessId();
    EnumWindows(function, parameter);
    //-----------------------------------------------------
#endif

    return pxWindow;
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

    if(!PXWindowDoRenderingSelf(pxWindow))
        return NULL;

    PXBool isHit = PXRectangleXYWHI32ContainsPoint(&pxWindow->Layout.SpaceContent, x, y);

    if(!isHit)
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

PXBool PXAPI PXWindowDoRenderingSelf(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXFalse;
    }

    //return pxWindow->State.DoRenderingSelf;

    return 1;
}

PXBool PXAPI PXWindowDoRenderingChildren(const PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXFalse;
    }

    return pxWindow->State.DoRenderingChildren;
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


#if PXLogEnable && 0
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
    const PXSize amountSibling = PXWindowSiblingAmount(pxWindow);

    for(PXSize i = 0; i < amountSibling; ++i)
    {
        PXWindow PXREF sibling = PXWindowSiblingGet(pxWindow, i);

        PXWindowLayoutCompute(sibling);
    }

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


#if OSUnix

void PXAPI PXWindowEvent_X_KeyAction(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    XEvent PXREF xEventData = pxWindowEvent->EventData;
    const XKeyEvent* keyEvent = &xEventData->xkey;
    const unsigned int keyCode = keyEvent->keycode;
    const PXBool release = KeyRelease == xEventData->type;
    const KeySym keySym = XKeycodeToKeysym(pxWindowEvent->XDisplayHandle, keyCode, 0);
    const char* keyName = XKeysymToString(keySym);

    pxWindowEvent->Type = PXWindowEventTypeInputKeyboard;
    pxWindowEvent->Keyboard.VirtualKey = PXVirtualKeyFromID(keySym);
    pxWindowEvent->Keyboard.CharacterID = keySym;

    switch(xEventData->type)
    {
        case KeyPress:
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateDown;
            break;

        case KeyRelease:
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateUp;
            break;

        default:
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateInvalid;
            break;
    }

    //PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_ButtonAction(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    XEvent PXREF xEventData = pxWindowEvent->EventData;
    const XButtonEvent* buttonEvent = &xEventData->xbutton;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;

    switch(xEventData->type)
    {
        case ButtonPress:
            pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;
            break;

        case ButtonRelease:
            pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;
            break;

        default:
            pxWindowEvent->MouseButton.PressState = PXKeyPressStateInvalid;
            break;
    }

    switch(buttonEvent->button)
    {
        /*
        case MouseButtonLeft:
            pxWindowEvent->InputMouseButton.Button = PXMouseButtonLeft;
            break;

        case MouseButtonMiddle:
            pxWindowEvent->InputMouseButton.Button = PXMouseButtonMiddle;
            break;

        case MouseButtonRight:
            pxWindowEvent->InputMouseButton.Button = PXMouseButtonRight;
            break;

        case MouseScrollUp:
            pxWindowEvent->InputMouseButton.Button = PXMouseButtonScrollUp;
            break;

        case MouseScrollDown:
            pxWindowEvent->InputMouseButton.Button = PXMouseButtonRightDown;
            break;
            */

        default:
            pxWindowEvent->MouseButton.Button = PXMouseButtonInvalid;
            break;
    }

    //PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_MotionNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    // printf("[Event] MotionNotify \n");

      //  pxWindowEvent.Type = PXWindowEventTypeInputMouseMove;
      //  PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);

}
void PXAPI PXWindowEvent_X_EnterNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementFocusEnter;
   // PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_LeaveNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementFocusLeave;
    //PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_FocusIn(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementFocusEnter;
    // PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_FocusOut(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    pxWindowEvent->Type = PXWindowEventTypeElementFocusLeave;
    // PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_KeymapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    printf("[Event] KeymapNotify \n");
}
void PXAPI PXWindowEvent_X_Expose(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    XEvent PXREF xEventData = pxWindowEvent->EventData;
    XExposeEvent PXREF xExposeEventData = &xEventData->xexpose;
    PXWindow* windowCurrent = PXNull;

        //PXDictionaryFindEntry(&pxNativDraw->ResourceManager->GUIElementLookup, &xExposeEventData->window, &pxWindowEvent.UIElementReference);

    if(windowCurrent)
    {
        PXDrawInfo pxDrawInfo;
        PXClear(PXDrawInfo, &pxDrawInfo);
        //pxDrawInfo.RectangleXYWH.X = xExposeEventData->x;
        //pxDrawInfo.RectangleXYWH.Y = xExposeEventData->y;
        //pxDrawInfo.RectangleXYWH.Width = xExposeEventData->width;
        //pxDrawInfo.RectangleXYWH.Height = xExposeEventData->height;
        //pxDrawInfo.ScreenIDHandle = DefaultScreen(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle);
        //pxDrawInfo.DisplayHandle = pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle;
        //pxDrawInfo.WindowIDHandle = xExposeEventData->window;
        //pxDrawInfo.GraphicContntainerHandle = DefaultGC(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, pxDrawInfo.ScreenIDHandle);

        // Sync current pos with element
        //windowCurrent->Position.Form.X = xExposeEventData->x;
        //windowCurrent->Position.Form.Y = xExposeEventData->y;
        //windowCurrent->Position.Form.Width = xExposeEventData->width;
        //windowCurrent->Position.Form.Height = xExposeEventData->height;


        /*
        const PXBool shallDraw = windowCurrent->DrawCall > 0;

        if(windowCurrent->DrawFunction)
        {
            if(PXECSInfoRender & windowCurrent->Info.Flags)
            {
                printf("[Event] Expose <%8.8x>. Custom draw for PXID:%i\n", windowCurrent->Info.ID);

                PXNativDrawClear(pxNativDraw, windowCurrent);

                windowCurrent->DrawFunction(pxNativDraw->GUISystem, windowCurrent, &pxDrawInfo);
            }
            else
            {
                printf("[Event] Expose <%8.8x>. Skipped draw for PXID:%i, not visible!\n", windowCurrent->Info.ID);
            }
        }
        else
        {
            printf("[Event] Expose <%8.8x>. No custom draw for PXID:%i\n", windowCurrent->Info.ID);
        }
        */

    }
    else
    {
        printf("[Event] Expose <%8.8x>. No window registerd!\n", xExposeEventData->window);
    }

    // PXNativDrawWindowBufferSwap(pxNativDraw, pxWindowEvent.UIElementReference); ??
}
void PXAPI PXWindowEvent_X_GraphicsExpose(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_NoExpose(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_VisibilityNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_CreateNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_DestroyNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_UnmapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_MapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_MapRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_ReparentNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_ConfigureNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_ConfigureRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_GravityNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_ResizeRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    XEvent PXREF xEventData = pxWindowEvent->EventData;
    const XResizeRequestEvent* resizeRequestEvent = &xEventData->xresizerequest;
    const int width = resizeRequestEvent->width;
    const int height = resizeRequestEvent->height;

    pxWindowEvent->Type = PXWindowEventTypeElementResize;
    pxWindowEvent->Resize.Width = width;
    pxWindowEvent->Resize.Height = height;

    //PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
}
void PXAPI PXWindowEvent_X_CirculateNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_CirculateRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_PropertyNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_SelectionClear(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_SelectionRequest(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_SelectionNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_ColormapNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_ClientMessage(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{

}
void PXAPI PXWindowEvent_X_MappingNotify(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
}
void PXAPI PXWindowEvent_X_GenericEvent(void PXREF invalidObject, PXWindowEvent PXREF pxWindowEvent)
{
    XEvent PXREF xEventData = pxWindowEvent->EventData;
    const XGenericEventCookie PXREF cookie = &xEventData->xcookie; // Make Copy
    const int result = XGetEventData(pxWindowEvent->XDisplayHandle, &cookie);
    const unsigned char sucessful = result != 0 && cookie->data;

    if(sucessful)
    {
        switch(cookie->evtype)
        {
            case XI_RawMotion:
            {
                XIRawEvent* re = (XIRawEvent*)cookie->data;

                if(re->valuators.mask_len)
                {
                    const double* values = re->raw_values;
                    const unsigned char isX = XIMaskIsSet(re->valuators.mask, 0);
                    const unsigned char isY = XIMaskIsSet(re->valuators.mask, 1);
                    double xpos = 0;
                    double ypos = 0;

                    if(isX)
                    {
                        xpos += *values;
                        ++values;
                    }

                    if(isY)
                    {
                        ypos += *values;
                    }

                    pxWindowEvent->Type = PXWindowEventTypeInputMouseMove;
                    pxWindowEvent->MouseMove.Axis.X = xpos;
                    pxWindowEvent->MouseMove.Axis.Y = ypos;
                    pxWindowEvent->MouseMove.Delta.X = 0;// mouse->Position[0] - xpos;
                    pxWindowEvent->MouseMove.Delta.Y = 0;// mouse->Position[1] - ypos;

                    //PXNativDrawEventConsumer(pxGUIManager, &pxWindowEvent);
                }
            }
        }
    }
    else
    {
        printf("[Event] GenericEvent %i\n", cookie->evtype);
    }

    XFreeEventData(pxWindowEvent->XDisplayHandle, &cookie);
}

#elif OSWindows

PXPrivate LRESULT PXAPI PXWindowEvent_WM_EMPTY(PXEngineUIEvent PXREF pxWindowEvent)
{
    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCCREATE(PXEngineUIEvent PXREF pxWindowEvent)
{
    CREATESTRUCTA PXREF createStruct = (CREATESTRUCTA*)pxWindowEvent->ParamL;
    // Note: We dont use our reference form "pxWindowEvent", as that does not exist yet!
    PXWindow PXREF pxWindow = (PXWindow*)createStruct->lpCreateParams;

    // Store our window payload
    PXWindowStoreSelf(pxWindow, pxWindowEvent->WindowHandle);

    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = PXTrue;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DESTROY(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    // Window WILL be destroyed, too late to cancel.
    pxWindowEvent->Type = PXWindowEventTypeElementDestroy;

    PXEngineUIOnWindowClosed(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVE(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeElementMove;

    PXEngineUIOnMouseMoved(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SIZE(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    const int width = LOWORD(pxWindowEvent->ParamL);
    const int height = HIWORD(pxWindowEvent->ParamL);

    pxWindowEvent->Type = PXWindowEventTypeElementResize;
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

    PXEngineUIOnWindowSizeChanged(pxWindow->EngineUI, pxWindowEvent);

#if 0
    DockLayoutConfig cfg = DockDefaultConfig();
    LayoutDockedEx(pxWindow, &cfg);

    if(!pxWindow)
    {
        LayoutDockedEx(pxWindow, NULL);
        //InvalidateRect(pxWindow->Handle.Data, NULL, TRUE);
        //UpdateWindow(pxWindow->Handle.Data);
    }
    else
    {
        if(height == 0)
            height = 1;

        if(pxWindow->GLContext.Data)
        {
            //wglMakeCurrent(pxWindow->DeviceContextHandle, pxWindow->RenderContext);
            //glViewport(0, 0, width, height);
            //wglMakeCurrent(pxWindow->DeviceContextHandle, 0);
        }

        //LayoutDockedEx(windowID, NULL);
        //InvalidateRect(windowID, NULL, FALSE);
    }
#endif

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CLOSE(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeElementClose;
    pxWindowEvent->Close.CommitToClose = PXTrue; 

    PXEngineUIOnWindowClosing(pxWindow->EngineUI, pxWindowEvent);

    if(pxWindowEvent->Close.CommitToClose)
    {
        return 0;
    }

    pxWindowEvent->DoReturnValue = 0;
    pxWindowEvent->ReturnValue = 0;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_SETCURSOR(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NOTIFY(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

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

                        PXWindowProperty pxWindowUpdateInfo;
                        PXClear(PXWindowProperty, &pxWindowUpdateInfo);
                        pxWindowUpdateInfo.Show = PXFalse;
                        //pxWindowUpdateInfo.WindowCurrent = pxWindowTABPage;
                        pxWindowUpdateInfo.Property = PXWindowPropertyTypeVisibility;

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

                        PXWindowProperty pxWindowUpdateInfo;
                        PXClear(PXWindowProperty, &pxWindowUpdateInfo);
                        pxWindowUpdateInfo.Show = PXTrue;
                        //pxWindowUpdateInfo.WindowCurrent = pxWindowTABPageSelected;
                        pxWindowUpdateInfo.Property = PXWindowPropertyTypeVisibility;

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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ERASEBKGND(PXEngineUIEvent PXREF pxWindowEvent)
{
    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = 1; // Signal we handled erasing. 0 would mean we didn't

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_PAINT(PXEngineUIEvent PXREF pxWindowEvent)
{
    // We don't do anything here.
    // This is only to validate any region
    // windows would think needs redrawing, this only stops
    // messages to redraw this region.

    HWND windowHandle = pxWindowEvent->WindowHandle;

    BOOL result = ValidateRect(windowHandle, NULL);

#if 0
    PAINTSTRUCT ps;
    BeginPaint(windowHandle, &ps);
    EndPaint(windowHandle, &ps);
#endif

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_GETMINMAXINFO(PXEngineUIEvent PXREF pxWindowEvent)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)pxWindowEvent->ParamL;
    //lpMMI->ptMinTrackSize.x = xxxxx; // Minimum width
    //lpMMI->ptMinTrackSize.y = xxxxx; // Minimum height

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DEVMODECHANGE(PXEngineUIEvent PXREF pxWindowEvent)
{
    LPCTSTR deviceName = (LPCTSTR)pxWindowEvent->ParamL;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT_DEVICE_CHANGE(PXEngineUIEvent PXREF pxWindowEvent)
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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEMOVE(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindowEventMouseMoveData PXREF pxMouseMove = &pxWindowEvent->MouseMove;

    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseMove;
    pxWindowEvent->MouseMove.Position.X = GET_X_LPARAM(pxWindowEvent->ParamL); // Windows 2000, windowsx.h
    pxWindowEvent->MouseMove.Position.Y = GET_Y_LPARAM(pxWindowEvent->ParamL); // Windows 2000, windowsx.h
    pxWindowEvent->MouseMove.IsPressed_LBUTTON = (MK_LBUTTON & pxWindowEvent->ParamW) > 0;
    pxWindowEvent->MouseMove.IsPressed_RBUTTON = (MK_RBUTTON & pxWindowEvent->ParamW) > 0;
    pxWindowEvent->MouseMove.IsPressed_SHIFT = (MK_SHIFT & pxWindowEvent->ParamW) > 0;
    pxWindowEvent->MouseMove.IsPressed_CONTROL = (MK_CONTROL & pxWindowEvent->ParamW) > 0;
    pxWindowEvent->MouseMove.IsPressed_MBUTTON = (MK_MBUTTON & pxWindowEvent->ParamW) > 0;
    pxWindowEvent->MouseMove.IsPressed_XBUTTON1 = (MK_XBUTTON1 & pxWindowEvent->ParamW) > 0;
    pxWindowEvent->MouseMove.IsPressed_XBUTTON2 = (MK_XBUTTON2 & pxWindowEvent->ParamW) > 0;

    PXEngineUIOnMouseMoved(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSELEAVE(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeElementFocusLeave;

    PXEngineUIOnMouseLeave(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_NCHITTEST(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    // wParam unused
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
#if 1
        if(c.y < pxWindow->UIData.TitleBarHeight)
            return HTCAPTION;
#endif

        return HTCLIENT;
    }

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DISPLAYCHANGE(PXEngineUIEvent PXREF pxWindowEvent)
{
    // Some display got plugged in, out or some setting changed
    // We need to update our monitors/displays

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUT(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    const HRAWINPUT rawInput = (HRAWINPUT)pxWindowEvent->ParamL;

    int wparm = GET_RAWINPUT_CODE_WPARAM(pxWindowEvent->ParamW);
    PXBool isFromBackground = RIM_INPUTSINK == wparm;

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

    if(isFromBackground)
    {
        return 0;
    }

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
        pxWindow->InputBuffer.Address,
        &size,
        sizeof(RAWINPUTHEADER)
    );

    RAWINPUT* raw = (RAWINPUT*)pxWindow->InputBuffer.Address;

    switch(raw->header.dwType)
    {
        case RIM_TYPEKEYBOARD:
        {
            // raw->data.keyboard.Flags == RI_KEY_MAKE)

            UINT wVirtKey = raw->data.keyboard.VKey;
            UINT wScanCode = raw->data.keyboard.MakeCode;

            pxWindowEvent->Type = PXWindowEventTypeInputKeyboard;
            pxWindowEvent->Keyboard.CharacterID = wVirtKey;

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

            PXEngineUIOnKeyboardInput(pxWindow->EngineUI, pxWindowEvent);

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

            PXEngineUIOnMouseMoved(pxWindow->EngineUI, pxWindowEvent);

            break;
        }
        default:
        {
            return 0;
        }
    }

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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_CTLCOLOR(PXEngineUIEvent PXREF pxWindowEvent)
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
    PXEngineUITheme PXREF pxGUITheme = 0;// PXGUIThemeGet();

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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOVING(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXWindow PXREF pxWindowRoot = PXWindowRootGet(pxWindow);

    RECT* pr = (RECT*)pxWindowEvent->ParamL;

#if 0
    if(pxWindow && pxWindow->State.Floating && pxWindowRoot) {
        // lParam is LPRECT of proposed window rect during the drag
        PXVector2I32S center;
        center.X = (pr->left + pr->right) / 2;
        center.Y = (pr->top + pr->bottom) / 2;

        if(PXWindowScreenPtInMainClient(pxWindow, &center))
        {
            PXWindowDockSide side = 0;// ChooseDockSide(pxWindow, &center);


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
        else 
        {
            PXWindowHideDockOverlay(pxWindow);
        }
    }
#endif
    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_ENTERSIZEMOVE(PXEngineUIEvent PXREF pxWindowEvent)
{
    pxWindowEvent->DoReturnValue = PXTrue;
    pxWindowEvent->ReturnValue = 0;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_EXITSIZEMOVE(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

#if 0
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
            PXWindowDockSide side = 0;//ChooseDockSide(pxWindow, &center);
            PXWindowDockBackToChild(pxWindow, side);
        }
    }
#endif

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_POWERBROADCAST(PXEngineUIEvent PXREF pxWindowEvent)
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
                PXWindowTextText,
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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DEVICECHANGE(PXEngineUIEvent PXREF pxWindowEvent)
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
        case DBT_DEVICEARRIVAL: //     A device or piece of media has been inserted and is now available.
        {
            break;
        }
        case DBT_DEVICEQUERYREMOVE: //     Permission is requested to remove a device or piece of media.Any application can deny this request and cancel the removal.
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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_INPUTLANGCHANGE(PXEngineUIEvent PXREF pxWindowEvent)
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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_COMMAND(PXEngineUIEvent PXREF pxWindowEvent)
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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_GETICON(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    HICON* iconHandle = PXNull;

    switch(pxWindowEvent->ParamW)
    {
        case ICON_SMALL:
        {
            break;
        }
        case ICON_BIG:
        {
            break;
        }
#ifdef ICON_SMALL2
        case ICON_SMALL2:
        {
            break;
        }
#endif
        default:
            break;
    }

    if(!iconHandle)
    {
        return 0;
    }

    pxWindowEvent->DoReturnValue = 1;
    pxWindowEvent->ReturnValue = (LRESULT)iconHandle;

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_DPICHANGED(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    const RECT PXREF suggestedRect = (RECT*)pxWindowEvent->ParamL;
    const WORD newDpiX = LOWORD(pxWindowEvent->ParamW);

    pxWindow->DPIScaleFactor = (float)newDpiX / 96.0f;

    // Resize the native OS window host to prevent pixel distortion
    SetWindowPos
    (
        pxWindow->Handle.Data,
        NULL,
        suggestedRect->left,
        suggestedRect->top,
        suggestedRect->right - suggestedRect->left,
        suggestedRect->bottom - suggestedRect->top,
        SWP_NOZORDER | SWP_NOACTIVATE
    );

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_KEY(PXEngineUIEvent PXREF pxWindowEvent)
{
    const LPARAM characterInfo = pxWindowEvent->ParamL;

    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputKeyboard;
    pxWindowEvent->Keyboard.CharacterID = pxWindowEvent->ParamW;
    pxWindowEvent->Keyboard.VirtualKey = PXVirtualKeyFromID(pxWindowEvent->ParamW);

    switch(pxWindowEvent->EventID)
    {
        case WM_KEYUP:
        {
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateUp;

            PXEngineUIOnKeyboardKeyUp(pxWindow->EngineUI, pxWindowEvent);

            break;
        }
        case WM_KEYDOWN:
        {
            pxWindowEvent->Keyboard.PressState = PXKeyPressStateDown;

            PXEngineUIOnKeyboardKeyDown(pxWindow->EngineUI, pxWindowEvent);
            break;
        }
    }


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
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDOWN(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonLeft;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;
    pxWindowEvent->MouseButton.Position.X = GET_X_LPARAM(pxWindowEvent->ParamL);
    pxWindowEvent->MouseButton.Position.Y = GET_Y_LPARAM(pxWindowEvent->ParamL);

    PXEngineUIOnMouseButtonLeftDown(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONUP(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonLeft;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;

    PXEngineUIOnMouseButtonLeftUp(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_LBUTTONDBLCLK(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonLeft;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDoubleClick;

    PXEngineUIOnMouseButtonLeftDoubleClick(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDOWN(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonRight;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;

    PXEngineUIOnMouseButtonRightDown(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONUP(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonRight;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;

    PXEngineUIOnMouseButtonRightUp(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_RBUTTONDBLCLK(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonRight;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDoubleClick;

    PXEngineUIOnMouseButtonRightDoubleClick(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDOWN(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonMiddle;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDown;

    PXEngineUIOnMouseButtonMiddleDown(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONUP(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonMiddle;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateUp;

    PXEngineUIOnMouseButtonMiddleUp(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MBUTTONDBLCLK(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    pxWindowEvent->Type = PXWindowEventTypeInputMouseButton;
    pxWindowEvent->MouseButton.Button = PXMouseButtonMiddle;
    pxWindowEvent->MouseButton.PressState = PXKeyPressStateDoubleClick;

    PXEngineUIOnMouseButtonMiddleDoubleClick(pxWindow->EngineUI, pxWindowEvent);

    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_MOUSEWHEEL(PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    // Doc says: LOWORD and HIWORD yield invalid data when multible monitors are attached.
    // GET_X_LPARAM and GET_Y_LPARAM do use those. Is it correct?
    const int xPos = GET_X_LPARAM(pxWindowEvent->ParamL);
    const int yPos = GET_Y_LPARAM(pxWindowEvent->ParamL);
    const int virtualKeys = GET_KEYSTATE_WPARAM(pxWindowEvent->ParamW);
    // "expressed in multiples or divisions of WHEEL_DELTA, which is 120"
    pxWindowEvent->MouseWheel.UpDownDelta = GET_WHEEL_DELTA_WPARAM(pxWindowEvent->ParamW);
    pxWindowEvent->MouseWheel.IsPressed_LBUTTON = (virtualKeys & MK_LBUTTON) > 0;
    pxWindowEvent->MouseWheel.IsPressed_RBUTTON = (virtualKeys & MK_RBUTTON) > 0;
    pxWindowEvent->MouseWheel.IsPressed_SHIFT = (virtualKeys & MK_SHIFT) > 0;
    pxWindowEvent->MouseWheel.IsPressed_CONTROL = (virtualKeys & MK_CONTROL) > 0;
    pxWindowEvent->MouseWheel.IsPressed_MBUTTON = (virtualKeys & MK_MBUTTON) > 0;
    pxWindowEvent->MouseWheel.IsPressed_XBUTTON1 = (virtualKeys & MK_XBUTTON1) > 0;
    pxWindowEvent->MouseWheel.IsPressed_XBUTTON2 = (virtualKeys & MK_XBUTTON2) > 0;

#if 0
    // If we use windows GDI elements, we would need to send these messages and catch them aswell.

    // Convert screen coordinates to client (local) coordinates
    POINT clientPoint = { xPos, yPos };
    ScreenToClient(hwnd, &clientPoint);

    // Accumulate the delta to support modern high-precision scrolling
    g_wheelDeltaAccumulator += rawDelta;

    // 4. Query the system settings for scroll speed
    UINT linesToScrollPerDelta = 3; // Default fallback
    SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &linesToScrollPerDelta, 0);

    // WHEEL_DELTA is a standard constant defined as 120
    while(abs(g_wheelDeltaAccumulator) >= WHEEL_DELTA)
    {
        if(g_wheelDeltaAccumulator >= WHEEL_DELTA)
        {
            // Scrolled UP / FORWARD
            g_wheelDeltaAccumulator -= WHEEL_DELTA;

            if(linesToScrollPerDelta == WHEEL_PAGESCROLL) {
                // User prefers scrolling by a full page
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);
            }
            else {
                // Scroll the number of lines specified by the system
                for(UINT i = 0; i < linesToScrollPerDelta; ++i) {
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
                }
            }
        }
        else if(g_wheelDeltaAccumulator <= -WHEEL_DELTA)
        {
            // Scrolled DOWN / BACKWARD
            g_wheelDeltaAccumulator += WHEEL_DELTA;

            if(linesToScrollPerDelta == WHEEL_PAGESCROLL) {
                SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);
            }
            else {
                for(UINT i = 0; i < linesToScrollPerDelta; ++i) {
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
                }
            }
        }
    }
#endif


    return 0;
}
PXPrivate LRESULT PXAPI PXWindowEvent_WM_XBUTTON(PXEngineUIEvent PXREF pxWindowEvent)
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
#endif




#if OSUnix

const PXI16U _windowEventList[] =
{
    KeyPress,
    KeyRelease,
    ButtonRelease,
    ButtonPress,
    MotionNotify,
    EnterNotify,
    LeaveNotify,
    FocusIn,
    FocusOut,
    KeymapNotify,
    Expose,
    GraphicsExpose,
    NoExpose,
    VisibilityNotify,
    CreateNotify,
    DestroyNotify,
    UnmapNotify,
    MapNotify,
    MapRequest,
    ReparentNotify,
    ConfigureNotify,
    ConfigureRequest,
    GravityNotify,
    ResizeRequest,
    CirculateNotify,
    CirculateRequest,
    PropertyNotify,
    SelectionClear,
    SelectionRequest,
    SelectionNotify,
    ColormapNotify,
    ClientMessage,
    MappingNotify,
    GenericEvent
};

const PXWindowEventHandlerFunction _pxWindowEventHandleList[] =
{
    PXWindowEvent_X_KeyAction,
    PXWindowEvent_X_KeyAction,
    PXWindowEvent_X_ButtonAction,
    PXWindowEvent_X_ButtonAction,
    PXWindowEvent_X_MotionNotify,
    PXWindowEvent_X_EnterNotify,
    PXWindowEvent_X_LeaveNotify,
    PXWindowEvent_X_FocusIn,
    PXWindowEvent_X_FocusOut,
    PXWindowEvent_X_KeymapNotify,
    PXWindowEvent_X_Expose,
    PXWindowEvent_X_GraphicsExpose,
    PXWindowEvent_X_NoExpose,
    PXWindowEvent_X_VisibilityNotify,
    PXWindowEvent_X_CreateNotify,
    PXWindowEvent_X_DestroyNotify,
    PXWindowEvent_X_UnmapNotify,
    PXWindowEvent_X_MapNotify,
    PXWindowEvent_X_MapRequest,
    PXWindowEvent_X_ReparentNotify,
    PXWindowEvent_X_ConfigureNotify,
    PXWindowEvent_X_ConfigureRequest,
    PXWindowEvent_X_GravityNotify,
    PXWindowEvent_X_ResizeRequest,
    PXWindowEvent_X_CirculateNotify,
    PXWindowEvent_X_CirculateRequest,
    PXWindowEvent_X_PropertyNotify,
    PXWindowEvent_X_SelectionClear,
    PXWindowEvent_X_SelectionRequest,
    PXWindowEvent_X_SelectionNotify,
    PXWindowEvent_X_ColormapNotify,
    PXWindowEvent_X_ClientMessage,
    PXWindowEvent_X_MappingNotify,
    PXWindowEvent_X_GenericEvent
};

#elif OSWindows

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
    WM_GETMINMAXINFO, // OnSizeChange
    WM_DEVMODECHANGE,
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
    WM_GETICON,
    WM_DPICHANGED,
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
    PXWindowEvent_WM_GETMINMAXINFO,
    PXWindowEvent_WM_DEVMODECHANGE,
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
    PXWindowEvent_WM_GETICON,
    PXWindowEvent_WM_DPICHANGED,
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

#endif


const PXI16U _windowEventListAmount = sizeof(_windowEventList) / sizeof(PXI16U);
const PXI16U _pxWindowEventHandleListAmount = sizeof(_pxWindowEventHandleList) / sizeof(PXWindowEventHandlerFunction);

#if _windowEventListAmount != _pxWindowEventHandleListAmount
#error "Call list missmatch!"
#endif


#if OSUnix
void PXAPI PXWindowEventTranslator(const XEvent PXREF xEventData)
{
    Window windowHandle = xEventData->xany.window;
    PXWindow* pxWindow = PXNull;

    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.EventData = xEventData;
    pxWindowEvent.XDisplayHandle = 0;

    const PXI8U index = PXMemoryCompareI16V_AVX512(_windowEventList, _windowEventListAmount, xEventData->type);

    if(index != (PXI8U)-1)
    {
        _pxWindowEventHandleList[index](pxWindow, &pxWindowEvent);
    }
}
#elif OSWindows
LRESULT CALLBACK PXWindowEventCallBack(const HWND windowHandle, const UINT eventID, const WPARAM wParam, const LPARAM lParam)
{
    PXWindow PXREF pxWindow = PXWindowFromHandle(windowHandle);

    PXEngineUIEvent pxWindowEvent;
    PXClear(PXEngineUIEvent, &pxWindowEvent);
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

    PXEngineMath PXREF pxEngineMath = PXEngineMathInstanceGet();    

    const PXI16U index = pxEngineMath->CompareI16V(_windowEventList, _windowEventListAmount, eventID);
    const PXBool hasMatch = PXIndexI16UNotFound != index;

    if(hasMatch)
    {
        _pxWindowEventHandleList[index](&pxWindowEvent);
    }
    else
    {
        // HANDLE??
        //PXDebugBreak();
    }

    if(!pxWindowEvent.DoReturnValue)
    {
        // If nothing had grabbed the event by now, let the default funcion handle it.
        pxWindowEvent.ReturnValue = DefWindowProc(windowHandle, eventID, wParam, lParam);
    }

    return pxWindowEvent.ReturnValue;
}

#endif

const char* PXAPI PXWindowTypeToString(const PXEngineUIElementType pxUIElementType)
{
    switch(pxUIElementType)
    {
        case PXEngineUIElementTypeCustom:            return "Custom";
        case PXEngineUIElementTypeWindow:            return "Window";
        case PXEngineUIElementTypePanel:            return "Panel";
        case PXEngineUIElementTypeText:            return "Text";
        case PXEngineUIElementTypeButton:               return "Button";
        case PXEngineUIElementTypeImage:                return "Image";
        case PXEngineUIElementTypeDropDown:             return "DropDown";
        case PXEngineUIElementTypeListBox:              return "ListBox";
        case PXEngineUIElementTypeTextEdit:             return "TextEdit";
        case PXEngineUIElementTypeRichEdit:             return "RichText";
        case PXEngineUIElementTypeScrollBar:            return "ScrollBar";
        case PXEngineUIElementTypeTrackBar:             return "TrackBar";
        case PXEngineUIElementTypeStatusBar:            return "StatusBar";
        case PXEngineUIElementTypeUpDown:               return "UpDown";
        case PXEngineUIElementTypeProgressBar:          return "ProgressBar";
        case PXEngineUIElementTypeHotKey:               return "HotKey";
        case PXEngineUIElementTypeCalender:             return "Calender";
        case PXEngineUIElementTypeToolTip:              return "ToolTip";
        case PXEngineUIElementTypeAnimate:              return "Animate";
        case PXEngineUIElementTypeDatePicker:           return "DatePicker";
        case PXEngineUIElementTypeGroupBox:             return "GroupBox";
        case PXEngineUIElementTypeRadioButton:          return "RadioButton";
        case PXEngineUIElementTypeGroupRadioButton:     return "GroupRadioButton";
        case PXEngineUIElementTypeTreeView:             return "TreeView";
        case PXEngineUIElementTypeTreeViewItem:         return "TreeViewItem";
        case PXEngineUIElementTypeFileDirectyView:      return "FileDirectyView";
        case PXEngineUIElementTypeFileDirectyViewEntry: return "FileDirectyViewEntry";
        case PXEngineUIElementTypeIPInput:              return "IPInput";
        case PXEngineUIElementTypeLink:                 return "Link";
        case PXEngineUIElementTypeHeader:               return "Header";
        case PXEngineUIElementTypeFontSelector:         return "FontSelector";
        case PXEngineUIElementTypePageScroll:           return "PageScroll";
        case PXEngineUIElementTypeTabControll:          return "TabControll";
        case PXEngineUIElementTypeTabPage:              return "TAB-Page";
        case PXEngineUIElementTypeToggle:               return "Toggle";
        case PXEngineUIElementTypeCheckBox:             return "CheckBox";
        case PXEngineUIElementTypeComboBox:             return "ComboBox";
        case PXEngineUIElementTypeColorPicker:          return "ColorPicker";
        case PXEngineUIElementTypeSlider:               return "Slider";
        case PXEngineUIElementTypeRenderFrame:          return "FrameRender";
        case PXEngineUIElementTypeMenuStrip:            return "MenuStrip";
        case PXEngineUIElementTypeResourceManger:       return "ResourceManger";
        case PXEngineUIElementTypeResourceEntry:        return "ResourceEntry";

        default:
            return PXNull;
    }
}

void PXAPI PXWindowRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXWindowRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXWindowRegisterInfoDynamic;
}

void drawBox_MBP
(
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


void PXAPI PXWindowDraw_GDI(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
#if OSWindows
    const HDC hdc = PXWindowDCGet(pxWindow);

    pxDrawInfo->DrawCall = PXNull;

    RECT rect;
    GetClientRect(pxWindow->Handle.Data, &rect);
   // FillRect(hdc, &rect, pxGUITheme->BrushDarkBackground->BrushHandle);
#endif
}

// Waits until all GPU instuctions are finished
PXBool PXAPI PXWindowWaitOnRenderer(PXWindow PXREF pxWindow)
{
    PXBool isOK = PXFalse;

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicAPINative:
        {
#if OSUnix
#elif OSWindows
            isOK = GdiFlush();
#endif
            break;
        }
        case PXGraphicAPIOpenGL:
        {
            glFinish();
            isOK = PXTrue; // Assuming
            break;
        }
    }

    return isOK;
}

PXResult PXAPI PXWindowBufferSwap(PXWindow PXREF pxWindow)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);

    glXSwapBuffers(xDisplay, pxWindow->Handle.Data);
    pxResult = PXResultOK; // TODO: Do we have error checking?

#elif OSWindows
    const HDC handleDeviceContext = PXWindowDCGet(pxWindow);
    const BOOL result = SwapBuffers(handleDeviceContext);
    pxResult = PXErrorCurrent(result);
#else
    pxResult = PXResultRefusedNotSupportedByLibrary;
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
    /*
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
    }*/

#if OSUnix
    PXEngineUI PXREF pxUI = PXEngineUIDisplayInstance();
    Display PXREF xDisplay = PXEngineUIDisplayGet(pxUI);

    const int resultID = XDestroyWindow(xDisplay, pxWindow->Handle.Data);
    result = PXErrorCurrent(resultID);
#elif OSWindows
    const BOOL success = DestroyWindow(pxWindow->Handle.Data);
    result = PXErrorCurrent(success);
#else
    result PXResultRefusedNotSupportedByLibrary;
#endif

    if(PXResultOK != result)
    {
        return result;
    }

    // Step 4, reset handles
    pxWindow->Handle.Data = PXNull;

#if OSWindows
    pxWindow->PixelBuffer.Data = PXNull;
#endif

    return result;
}

PXResult PXAPI PXWindowFindViaTitle(PXWindow PXREF pxWindow, const PXText PXREF pxText)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

    #if OSUnix
    #elif OSWindows

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
      #endif // OSUnix

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

    const DWORD aa = DragObject(pxWindow->Handle.Data, hwndFrom, fmt, data, hcur);
#else
    return PXResultRefusedNotSupportedByOperatingSystem;
#endif

    //const BOOL res = DragDetect();

    return PXResultRefusedNotImplemented;
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
    return PXResultRefusedNotSupported;
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
    const PXBool isInFocus = pxWindow->Handle.Data == windowIDInFocus;

    return isInFocus;
#endif
}

PXBool PXAPI PXWindowIsPhysical(const PXWindow PXREF pxWindow)
{
    return pxWindow->Handle.Data > 0;
}

PXBool PXAPI PXWindowIsMinimized(const PXWindow PXREF pxWindow)
{
    PXBool isMinimized = PXFalse;

    if(!pxWindow)
    {
        return isMinimized;
    }

#if OSUnix
#elif OSWindows
    isMinimized = IsIconic(pxWindow->Handle.Data);
#endif

    return isMinimized;
}

PXBool PXAPI PXWindowIsSurfaceValid(const PXWindow PXREF pxWindow)
{
    PXBool isValid = PXFalse;

    if(!pxWindow)
    {
        return isValid;
    }

#if OSUnix
#elif OSWindows
    RECT rectangle;
    BOOL result = GetClientRect(pxWindow->Handle.Data, &rectangle);

    int width = rectangle.right;
    int height = rectangle.bottom;
    
    isValid = !(width <= 0 || height <= 0);  

#endif

    return isValid;
}

PXBool PXAPI PXWindowClientRectangleGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle)
{
    if(!(pxWindow && pxRectangle))
    {
        return PXResultRefusedParameterNull;
    }



    return PXResultOK;
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

#if OSUnix
#elif OSWindows
    // EnableWindow(pxWindow->ID, show);
    HWND windowHandle = pxWindow->Handle.Data;
    BOOL res = FALSE;

    res = ShowWindow(windowHandle, SW_SHOW);
    res = ShowWindow(windowHandle, SW_SHOW); // Twice to prevent some errors
#endif

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

#if 0
PXResult PXAPI PXWindowRedraw(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    // Todo..
#elif OSWindows

    if(pxWindow->Handle.Data)
    {
        InvalidateRect(pxWindow->Handle.Data, 0, PXFalse);
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

    PXWindowHandle windowHandle = PXWindowHandleGet(pxWindow);

    const LRESULT result = SendMessage(windowHandle.Data, WM_SETREDRAW, enable, 0);

    if(enable)
    {
        const BOOL result = InvalidateRect(windowHandle.Data, NULL, TRUE); // forces the window to redraw.
    }

#endif

    return PXResultOK;
}
#endif

PXResult PXAPI PXWindowDragAndDropBegin(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows
    PXWindowHandle windowHandle = PXWindowHandleGet(pxWindow);

    // Gegister drag&Drop
    IDropTarget dropTarget;

    const HRESULT dragResult = RegisterDragDrop(windowHandle.Data, &dropTarget); // Windows 2000, Ole32.dll, ole2.h
#endif

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

    const HDC hdc = BeginPaint(pxWindow->Handle.Data, &paintSturct);
#endif

    return PXResultRefusedNotImplemented;
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

    const PXBool end = EndPaint(pxWindow->Handle.Data, &paintSturct);
#endif

    return PXResultRefusedNotImplemented;
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
        pxWindow->Handle.Data,
        flags,
        ESB_ENABLE_BOTH
    );

    const BOOL show = ShowScrollBar
    (
        pxWindow->Handle.Data,
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
        pxWindow->Handle.Data,
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

PXResult PXAPI PXWindowTitleBarColor(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    return PXResultRefusedNotImplemented;
#elif OSWindows

    PXLibrary pxLibrary;
    PXDwmSetWindowAttribute pxDwmSetWindowAttribute = PXNull;

    // If not loaded already
    if(!pxDwmSetWindowAttribute)
    {
        // Open lib
        {
            PXText pxText;
            PXTextFromAddressA(&pxText, WindowsLibraryDWMAPI, sizeof(WindowsLibraryDWMAPI), sizeof(WindowsLibraryDWMAPI));
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

            return PXResultRefusedNotSupportedByOperatingSystem;
        }
    }

    if(!pxDwmSetWindowAttribute)
    {
        return PXResultRefusedNotSupportedByOperatingSystem;
    }

    const HWND windowHandle = pxWindow->Handle.Data;
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

    UpdateWindow(windowHandle);
    ShowWindow(windowHandle, SW_HIDE);
    ShowWindow(windowHandle, SW_SHOWNORMAL);

    return setResult;
#else
    return PXResultRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXWindowForegroundSet(PXWindow PXREF pxWindow, PXColorRGB_U8 PXREF PXColorRGB_U8)
{
    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;

    if(!(xDisplay && xGraphicContext))
    {
        return PXResultRefusedHandleNull;
    }

    const PXI32U color = PXColorI32FromBGR(PXColorRGB_U8->Red, PXColorRGB_U8->Green, PXColorRGB_U8->Blue);
    const int resultID = XSetForeground
    (
        xDisplay,
        xGraphicContext,
        color
    );
#elif OSWindows
    HDC deviceContextHandle = pxWindow->PixelBuffer.Data;

    if(!deviceContextHandle)
    {
        return PXResultRefusedHandleNull;
    }

    const COLORREF colorNew = RGB(PXColorRGB_U8->Red, PXColorRGB_U8->Green, PXColorRGB_U8->Blue);
    const COLORREF colorPrevious = SetTextColor(deviceContextHandle, colorNew);
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowBackgroundSet(PXWindow PXREF pxWindow, PXColorRGB_U8 PXREF PXColorRGB_U8)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix

    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;

    if(!(xDisplay && xGraphicContext))
    {
        return PXResultRefusedHandleNull;
    }

    const PXI32U color = PXColorI32FromBGR(PXColorRGB_U8->Red, PXColorRGB_U8->Green, PXColorRGB_U8->Blue);
    const int resultID = XSetBackground
    (
        xDisplay,
        xGraphicContext,
        color
    );
#elif OSWindows

    HDC deviceContextHandle = pxWindow->PixelBuffer.Data;

    if(!deviceContextHandle)
    {
        return PXResultRefusedHandleNull;
    }

    const int mode = SetBkMode(deviceContextHandle, OPAQUE);

    const COLORREF colorNew = RGB(PXColorRGB_U8->Red, PXColorRGB_U8->Green, PXColorRGB_U8->Blue);
    const COLORREF colorPrevious = SetBkColor(deviceContextHandle, colorNew);
    const PXBool successful = CLR_INVALID != colorPrevious;

    if(!successful)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXWindowTextText,
            "Color-Set",
            "Failed set background color"
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

    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;

    if(!(xDisplay && xGraphicContext && pxFont->FontHandle))
    {
        return PXResultRefusedHandleNull;
    }

    const int resultID = XSetFont
    (
        xDisplay,
        pxFont->FontHandle,
        xGraphicContext
    );
#elif OSWindows

    HDC deviceContextHandle = pxWindow->PixelBuffer.Data;

    if(!(deviceContextHandle && pxFont->FontHandle))
    {
        return PXResultRefusedHandleNull;
    }

    const HFONT fontHandleOld = (HFONT)SelectObject(deviceContextHandle, pxFont->FontHandle);
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

    PXColorRGB_U8* colorRef = PXNull;

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
    pxResult = PXResultRefusedNotImplemented;
#elif OSWindows

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            pxText->SizeUsed = GetWindowTextA
            (
                pxWindow->Handle.Data,
                pxText->A,
                pxText->SizeAllocated
            );
            break;
        }
        case TextFormatUNICODE:
        {
            pxText->SizeUsed = GetWindowTextW
            (
                pxWindow->Handle.Data,
                pxText->W,
                pxText->SizeAllocated
            );

            break;
        }

        default:
            break;
    }

#else
    pxResult = PXResultRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

PXResult PXAPI PXWindowTextSet(PXWindow PXREF pxWindow, PXText PXREF pxText)
{
    PXResult pxResult = PXResultInvalid;

#if OSUnix
    pxResult = PXResultRefusedNotImplemented;
#elif OSWindows

    BOOL result = FALSE;

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            result = SetWindowTextA
            (
                pxWindow->Handle.Data,
                pxText->A
            );

            break;
        }
        case TextFormatUNICODE:
        {
            result = SetWindowTextW
            (
                pxWindow->Handle.Data,
                pxText->W
            );
            break;
        }

        default:
            break;
    }

#else
    pxResult = PXResultRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

PXResult PXAPI PXWindowStyleUpdate(PXWindow PXREF pxWindow)
{
    PXResult result = PXResultInvalid;

    if(!pxWindow)
    {
        return PXResultOK;
    }

#if OSUnix
    result = PXResultRefusedNotImplemented;
#elif OSWindows
    SetWindowLongPtr(pxWindow->Handle.Data, GWL_STYLE, WS_SYSMENU); //3d argument=style
    const PXBool setSuccess = SetWindowPos
    (
        pxWindow->Handle.Data,
        HWND_TOPMOST,
        100,
        100,
        100,
        100,
        SWP_SHOWWINDOW
    );
#else
    result = PXResultRefusedNotSupportedByLibrary;
#endif

    return result;
}

UIWindow* PXAPI PXWindowUIDataGet(const PXWindow PXREF pxWindow)
{
    return &pxWindow->UIData;
}

UIState* PXAPI PXWindowUIStateGet(const PXWindow PXREF pxWindow)
{
    return &pxWindow->UIDataState;
}

PXResult PXAPI PXWindowClientArea(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle)
{
    if(!(pxWindow && pxRectangle))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows
    RECT rectangle;
    const BOOL result = GetClientRect(pxWindow->Handle.Data, &rectangle);

    pxRectangle->X = rectangle.left;
    pxRectangle->Y = rectangle.top;
    pxRectangle->Width = rectangle.right - rectangle.left;
    pxRectangle->Height = rectangle.bottom - rectangle.top;
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowCursorPosition
(
    const PXWindow PXREF pxWindow, 
    PXVector2I32S PXREF positionGlobal,
    PXVector2I32S PXREF positionLocal
)
{
#if OSUnix

    Display* display = XOpenDisplay(NULL);
    if(!display) 
        return -1;

    int screen = DefaultScreen(display);
    Window root = RootWindow(display, screen);

    Window returned_root;
    Window returned_child;
    int root_x;
    int root_y;
    int win_x;
    int win_y;
    unsigned int mask_return;

    XQueryPointer
    (
        display, 
        root, 
        &returned_root, 
        &returned_child,
        &root_x, 
        &root_y, 
        &win_x, 
        &win_y, 
        &mask_return
    );

    if(positionGlobal)
    {
        positionGlobal->X = root_x;
        positionGlobal->Y = root_y;
    }

    XTranslateCoordinates
    (
        display,
        root,
        target_window,
        root_x,
        root_y,
        &client_x,
        &client_y,
        &child_return
    );


#elif OSWindows
    POINT p;
    BOOL resultID = GetCursorPos(&p);

    if(positionGlobal)
    {
        positionGlobal->X = p.x;
        positionGlobal->Y = p.y;
    }

    if(pxWindow)
    {
        resultID = ScreenToClient(pxWindow->Handle.Data, &p);

        if(positionLocal)
        {
            positionLocal->X = p.x;
            positionLocal->Y = p.y;
        }
    }

#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowSpaceDrawGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxWindow && pxRectangle))
    {
        return PXResultRefusedParameterNull;
    }

    PXBool isPhysical = PXWindowIsPhysical(pxWindow);

    if(isPhysical)
    {
        const PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

#if OSUnix
#elif OSWindows
#if 0
        *pxRectangleXYWHI32 = pxWindow->Position.Form;
#else
        PXRectangleLTRBI32 pxRectangleLTRB;
        RECT* rectangle = (RECT*)&pxRectangleLTRB;
        const BOOL result = GetClientRect(pxWindowHandle.Data, rectangle);
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
        #endif

    }
    else
    {
        PXWindow* pxWindowParent = PXWindowParentGet(pxWindow);

        if(!pxWindowParent)
        {
            // We dont have a physical handle, yet no parent.
            return PXResultOK;
        }

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

PXResult PXAPI PXWindowSpaceBoxGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle)
{
    *pxRectangle = pxWindow->Layout.SpaceBox;

    return PXResultOK;
}

PXResult PXAPI PXWindowSpaceContentGet(const PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle)
{
    *pxRectangle = pxWindow->Layout.SpaceContent;

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawFunctionSet(PXWindow PXREF pxWindow, const PXECSDrawFunction pxECSDrawFunction, void* owner)
{
    if(!(pxWindow && pxECSDrawFunction))
    {
        return PXResultRefusedParameterNull;
    }

    pxWindow->EventList.CallBackOwner = owner;
    pxWindow->EventList.CallBackDraw = pxECSDrawFunction;

    return PXResultOK;
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
    PXBool isWindowValid = PXFalse;

    if(!pxWindow)
    {
        return PXFalse;
    }

#if OSUnix

    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);

    if(!xDisplay)
    {
        return PXFalse;
    }

    XWindowAttributes attr;
    int resultID = XGetWindowAttributes(xDisplay, pxWindow->Handle.Data, &attr); // X11

    isWindowValid = 0 != resultID;
#elif OSWindows
    isWindowValid = IsWindow(pxWindow->Handle.Data);
#endif

    if(!isWindowValid)
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXWindowTextText,
            "Valid",
            "%20s (0x%p), Invalid",
            "",//pxWindow->NameData,
            pxWindow->Handle.Data
        );
    }

    return isWindowValid;
}

PXBool PXAPI PXWindowIsEnabled(const PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXFalse;
#elif OSWindows
    const BOOL success = IsWindowEnabled(pxWindow->Handle.Data); // Windows 2000, User32.dll, winuser.h
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

PXF32 hyCol = 0;
PXF32 offsetY = 20;

PXResult PXAPI PXTextDrawInfoRowNext(PXEngineUIDrawInfo PXREF pxTextDrawInfo)
{
   // pxTextDrawInfo->RectangleXYWH.Y += offsetY;

    return PXResultOK;
}

PXResult PXAPI PXEngineUIDrawText
(
    PXWindow PXREF pxWindow,
    PXEngineUIDrawInfo PXREF pxTextDrawInfo
)
{
    PXEngineUITheme PXREF pxGUITheme = pxTextDrawInfo->UITheme;


    if(!(pxWindow && pxTextDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }


    PXText pxTextFailsafe;
    PXTextFromAddressA(&pxTextFailsafe, PXTextFailsafe, PXTextFailsafeLength, PXTextFailsafeLength);

    PXText* targetText = pxTextDrawInfo->Text;

    if(!targetText)
    {
        targetText = &pxTextFailsafe;
    }

    switch(pxWindow->GraphicSystem)
    {
        case PXGraphicAPINative:
        {
            PXWindowDrawTextNative(pxWindow, pxTextDrawInfo);
            break;
        }
        case PXGraphicAPIOpenGL:
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

PXResult PXAPI PXWindowDrawTextNative(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxTextDrawInfo)
{
    PXText PXREF pxText = pxTextDrawInfo->Text;

#if OSUnix && 0

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

    HDC deviceContextHandle = pxWindow->PixelBuffer.Data;

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
                deviceContextHandle,
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
                deviceContextHandle,
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

PXResult PXAPI PXWindowDrawTextGLFF(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxTextDrawInfo)
{
#if 0
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXFont PXREF fontTitle = pxGUITheme->FontTitle;
    PXGraphic PXREF pxGraphic = PXGraphicInstantiateGET();
    PXText PXREF pxText = pxTextDrawInfo->Text;

    PXOpenGL PXREF pxOpenGL = &pxGraphic->APIOpenGL;
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

            #if OSWindows
            fontTitle->GlyphMetricsFloat = PXMemoryHeapCallocT(GLYPHMETRICSFLOAT, glyphsAmount);
            fontTitle->FontBase = pxOpenGL->Binding.GenLists(glyphsAmount);

            fontTitle->IsOK = wglUseFontOutlinesW
            (
                pxWindow->PixelBuffer.Data,
                glyphsIndexFirst,
                glyphsAmount,
                fontTitle->FontBase,
                maximumChordalDeviation,
                extrusion,
                WGL_FONT_POLYGONS,
                fontTitle->GlyphMetricsFloat
            ); // Ungodly slow!!
            PXResult pxResult = PXErrorCurrent(fontTitle->IsOK);
            #endif // OSWindows

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

    #if OSWindows
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
    #endif // OSWindows


    //pxOpenGL->Binding.Translatef(0, 0, 0);
    //pxOpenGL->Binding.PopMatrix();
    pxOpenGL->Binding.PopAttrib();
    //pxOpenGL->Binding.Flush();

#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawRectangle(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    if(!(pxWindow && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXOpenGL PXREF pxOpenGL = 0;// &pxGraphic->APIOpenGL;

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;

    if(!xDisplay)
    {
        return PXResultRefusedHandleNull;
    }

    if(!xGraphicContext)
    {
        return PXResultRefusedHandleNull;
    }
#elif OSWindows
    HDC deviceContextHandle = pxWindow->PixelBuffer.Data;
#endif

    PXEngineUITheme* pxGUITheme = pxDrawInfo->UITheme;
    PXBrush* brushMainPrimary = pxGUITheme->BrushMainPrimary;
    PXBrush* brushMainSecoundary = pxGUITheme->BrushMainSecoundary;
    const PXRectangleXYWHI32 PXREF pxRect = 0;// &pxDrawInfo->RectangleXYWH;

    PXResult pxResult = PXResultInvalid;
    PXGraphicAPI PXGraphicAPI = PXGraphicAPIOpenGL;

    PXColorRGB_F32 colorPrimary;// = pxDrawInfo->Color;
    PXColorRGB_F32 colorSecoundary = colorPrimary;

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
        PXGraphicAPI = pxWindow->GraphicSystem;
    }

    switch(PXGraphicAPI)
    {
        case PXGraphicAPINative:
        {
#if OSUnix
            const int resultID = XFillRectangle
            (
                xDisplay,
                pxWindow->Handle.Data,
                xGraphicContext,
                0,//x,
                0,//y,
                0,//width,
                0//height
            );
            pxResult = PXResultOK;// PXErrorCurrent(resultID);
#elif OSWindows

            PXResult pxResult;

            RECT* rect = 0;// (RECT*)&pxDrawInfo->RectangleXYWH;

            if(pxDrawInfo->Rounded)
            {
                const BOOL success = RoundRect
                (
                    deviceContextHandle,
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
                    deviceContextHandle,
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
        case PXGraphicAPIOpenGL:
        {
            //PXWindowProperty pxGUIProperty;
            //pxGUIProperty.Property = PXWindowPropertyTypeSize;
            //PXWindowPropertyFetch(pxWindow, &pxGUIProperty, 1, PXFalse);

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
                pxOpenGL->Binding.Color3fv(colorPrimary.Data);
                pxOpenGL->Binding.Vertex2f(pxRect->X, pxRect->Y); // +1.0f - offset, 1.0f
                pxOpenGL->Binding.Vertex2f(pxRect->X + pxRect->Width, pxRect->Y); // -1.0f + offset, 1.0f
                pxOpenGL->Binding.Color3fv(colorSecoundary.Data);
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

PXResult PXAPI PXWindowMouseTrack(PXWindow PXREF window)
{
#if OSUnix
#elif OSWindows
    TRACKMOUSEEVENT trackMouseEvent;
    trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
    trackMouseEvent.dwFlags = TME_HOVER | TME_LEAVE;
    trackMouseEvent.hwndTrack = window->Handle.Data;
    trackMouseEvent.dwHoverTime = HOVER_DEFAULT;

    const PXBool rs = TrackMouseEvent(&trackMouseEvent);
#endif

    return PXResultOK;
}



#if OSUnix
#elif OSWindows
LPDIRECTINPUT8 g_di = NULL;
LPDIRECTINPUTDEVICE8 g_wheel = NULL;

BOOL CALLBACK EnumDevicesCallback(const DIDEVICEINSTANCE* inst, VOID* ctx) 
{
    if(inst->dwDevType & DI8DEVTYPE_DRIVING) 
    {
        IDirectInput8_CreateDevice(g_di, &inst->guidInstance, &g_wheel, NULL);
        return DIENUM_STOP;
    }
    return DIENUM_CONTINUE;
}

void InitDirectInput(HWND hwnd) 
{
    DirectInput8Create
    (
        GetModuleHandle(NULL), 
        DIRECTINPUT_VERSION,
        &IID_IDirectInput8,
        (void**)&g_di,
        NULL
    );

    IDirectInput8_EnumDevices
    (
        g_di, 
        DI8DEVCLASS_GAMECTRL,
        EnumDevicesCallback,
        NULL, 
        DIEDFL_ATTACHEDONLY
    );

    if(g_wheel) 
    {
        IDirectInputDevice8_SetDataFormat(g_wheel, &c_dfDIJoystick2);
        IDirectInputDevice8_SetCooperativeLevel
        (
            g_wheel,
            hwnd,
            DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
        );
        IDirectInputDevice8_Acquire(g_wheel);
    }
}
#endif


PXResult PXAPI PXWindowMouseMovementEnable(PXWindow PXREF pxWindow)
{
#if OSUnix
    return PXResultRefusedNotImplemented;

#elif OSWindows
#if PXOSWindowsDestop && WindowsAtleastXP
    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    RAWINPUTDEVICE rawInputDeviceList[2];
    const PXI8U rawInputDeviceListAmount = sizeof(rawInputDeviceList) / sizeof(RAWINPUTDEVICE);

    // Keyboard
    // rawInputDeviceList[0].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    // rawInputDeviceList[0].usUsage = 0x03;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[0].usUsagePage = 0x01;
    rawInputDeviceList[0].usUsage = 0x06;
    rawInputDeviceList[0].dwFlags = RIDEV_DEVNOTIFY; // RIDEV_INPUTSINK
    rawInputDeviceList[0].hwndTarget = pxWindowHandle.Data;

    // Mouse
    // rawInputDeviceList[1].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    // rawInputDeviceList[1].usUsage = 0x02;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[1].usUsagePage = 0x01;
    rawInputDeviceList[1].usUsage = 0x02;
    rawInputDeviceList[1].dwFlags = RIDEV_DEVNOTIFY;
    rawInputDeviceList[1].hwndTarget = pxWindowHandle.Data;

    // WARNING
    // RegisterRawInputDevices should not be used from a library!
    // As it may interfere with any raw input processing logic already present in applications that load it.
    const BOOL regsiterRawImputSuccess = RegisterRawInputDevices
    (
        rawInputDeviceList,
        rawInputDeviceListAmount, 
        sizeof(RAWINPUTDEVICE)
    ); // Windows XP, User32.dll, winuser.h
    const PXResult pxResult = PXErrorCurrent(regsiterRawImputSuccess);

    if(PXResultOK != pxResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXWindowTextText,
            "Input",
            "Failed to registered device for PXID:<%4i>",
            pxWindow->Info.ID
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
        "Registered device for PXID:<%4i>",
        pxWindow->Info.ID
    );
#endif

    return PXResultOK;

#else
    return PXResultRefusedNotSupported;
#endif

    return PXResultRefusedNotSupportedByOperatingSystem;

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
                GetClientRect(pxWindow->Handle.Data, &clipRect);
                ClientToScreen(pxWindow->Handle.Data, (POINT*)&clipRect.left);
                ClientToScreen(pxWindow->Handle.Data, (POINT*)&clipRect.right);
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
                GetClientRect(pxWindow->Handle.Data, &clipRect);
                ClientToScreen(pxWindow->Handle.Data, (POINT*)&clipRect.left);
                ClientToScreen(pxWindow->Handle.Data, (POINT*)&clipRect.right);

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

return PXResultOK;
}

PXResult PXAPI PXWindowCursorPositionGet(PXWindow PXREF pxWindow, PXVector2I32S PXREF position)
{
    PXResult pxResult = PXResultInvalid;

    if(pxWindow->Handle.Data)
    {
        // We have a target window,

#if OSUnix


#elif PXOSWindowsDestop
        POINT* point = (POINT*)position; // Illegal cast

        const BOOL sucessful = ScreenToClient(pxWindow->Handle.Data, point);  // are now relative to hwnd's client area
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
    pxResult = PXResultRefusedNotImplemented;
#elif OSWindows

    switch(pxText->Format)
    {
        case TextFormatASCII:
        {
            if(doWrite)
            {
                BOOL result = SetWindowTextA
                (
                    pxWindow->Handle.Data,
                    pxText->A
                );
            }
            else
            {
                pxText->SizeUsed = GetWindowTextA
                (
                    pxWindow->Handle.Data,
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
                    pxWindow->Handle.Data,
                    pxText->W
                );
            }
            else
            {
                pxText->SizeUsed = GetWindowTextW
                (
                    pxWindow->Handle.Data,
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
    pxResult = PXResultRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

PXResult PXAPI PXWindowCreate(PXWindow** pxWindowREF, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    if(!(pxWindowREF && pxWindowCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult result = PXResultInvalid;
    PXWindow* pxWindow = *pxWindowREF;
    PXWindow* pxWindowParent = pxWindowCreateInfo->WindowParent;

    // Always add them
    pxWindowCreateInfo->Info.Static = &PXWindowRegisterInfoStatic;
    pxWindowCreateInfo->Info.Dynamic = &PXWindowRegisterInfoDynamic;

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

    // if we only have the parent name, try to get actual parent via name
    if(pxWindowCreateInfo->WindowParentName.A)
    {
        pxWindowParent = PXWindowGetViaName(&pxWindowCreateInfo->WindowParentName);

        PXBool failedNameToHandle = !pxWindowParent;

        // if we failed, we could not resolve our parent!
        // This means we can either not spawn it or make it global.
        // We decide here to spawn it globally but also print out an error.
        if(failedNameToHandle)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                PXWindowTextText,
                "Lookup",
                "WindowHame:<%.*s> couln't not be located! Window is global now!",
                pxWindowCreateInfo->WindowParentName.SizeUsed,
                pxWindowCreateInfo->WindowParentName.A
            );
#endif

            // We force global window.
            pxWindowCreateInfo->SkipHandleCreation = PXTrue;

            PXNamePoolPrintAll();
        }
    }

    char name[32];

    if(!pxWindow)
    {
        PXResult pxResult = PXECSCreate(pxWindowREF, pxWindowCreateInfo);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }      

        pxWindow = *pxWindowREF; 

        pxWindow->State = pxWindowCreateInfo->State;
        pxWindow->Layout = pxWindowCreateInfo->Layout;
        pxWindow->Type = pxWindowCreateInfo->Type;

        // These settings are only valid on startup, so we set them here ONCE

        pxWindow->EventList = pxWindowCreateInfo->EventList;
        pxWindow->Info.FlagListState |= pxWindowCreateInfo->Behaviour;
        pxWindow->Info.FlagListSettings |= pxWindowCreateInfo->Setting;
        pxWindow->BackGroundColor = pxWindowCreateInfo->BackGroundColor;
        pxWindow->UIData.TitleBarHeight = 28;


        PXBool isBlack =
            (0 == pxWindow->BackGroundColor.Red) &&
            (0 == pxWindow->BackGroundColor.Green) &&
            (0 == pxWindow->BackGroundColor.Blue);

        if(isBlack)
        {
            pxWindow->BackGroundColor.Red = 0xFF;
            pxWindow->BackGroundColor.Green = 0x00;
            pxWindow->BackGroundColor.Blue = 0x00;
        }

        PXWindowVisibilitySet(pxWindow, PXTrue, PXTrue);
        PXHierarchicalNodeCreate(&pxWindow->Hierarchy, pxWindow);

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

        if(pxWindowParent)
        {
            pxWindow->EngineUI = pxWindowParent->EngineUI;

            PXWindowParentSet(pxWindow, pxWindowParent);
        }

        if(pxWindowCreateInfo->EngineUI)
        {
            pxWindow->EngineUI = pxWindowCreateInfo->EngineUI;
        }

        if(PXGraphicAPIInvalid == pxWindow->GraphicSystem)
        {
            pxWindow->GraphicSystem = PXGraphicAPIOpenGL;
        }

        PXTextFromAddressA(&pxWindowCreateInfo->WindowClassName, PXWindowClassName, sizeof(PXWindowClassName), sizeof(PXWindowClassName));
    }
    else
    {
        //PXNamePoolStore(pxID, &pxECSCreateInfo->Name, &pxECSCreateInfo->Name);
    }


    if(pxWindowParent && pxWindowCreateInfo->DoSpawnInParent)
    {
#if OSUnix

#elif OSWindows
        // If we want to put this as a subwindow, we need the handle of the parranet
        pxWindowCreateInfo->InstanceHandle = (HINSTANCE)GetWindowLongPtr
        (
            pxWindowParent->Handle.Data,
            GWLP_HINSTANCE
        );

        pxWindowCreateInfo->StyleFlags |=
            WS_CHILD |
            CS_OWNDC |
            WS_CLIPSIBLINGS |
            WS_CLIPCHILDREN;
#endif
    }
    else
    {
#if OSUnix
#elif OSWindows

        // Get the current module context
        pxWindowCreateInfo->InstanceHandle = GetModuleHandle(NULL);

        pxWindowCreateInfo->StyleFlags |= WS_OVERLAPPEDWINDOW;
#endif
    }

    //-----------------------------------------------------
    // Calc screensize
    const PXBool isDefaultSize = 1;// windowInfo->Width == 0 && windowInfo->Height == 0;

    if(isDefaultSize)
    {
        PXEngineUIScreenSizeGet(&pxWindow->Layout.SpaceContent);

        // Scale down
        //pxWindow->abs.X *= 0.125f;
        //pxWindow->abs.Y *=  0.125f;
        pxWindow->Layout.SpaceContent.Width *= 0.8f;
        pxWindow->Layout.SpaceContent.Height *= 0.8f;

        // Center
        pxWindow->Layout.SpaceContent.X = pxWindow->Layout.SpaceContent.Width / 8;
        pxWindow->Layout.SpaceContent.Y = pxWindow->Layout.SpaceContent.Height / 8;
    }
    //-----------------------------------------------------

#if OSUnix
    const PXEngineUI PXREF pxUI = PXEngineUIDisplayInstance();
    const Display PXREF xDisplayHandle = PXEngineUIDisplayGet(pxUI);
    const int screenIDHandle = PXEngineUIScreenIDDefaultGet(xDisplayHandle);

    pxWindowCreateInfo->Simple = PXTrue;

    pxWindowCreateInfo->DisplayHandle = xDisplayHandle;
    pxWindowCreateInfo->ScreenHandleNumber = screenIDHandle;
#elif OSWindows
    pxWindowCreateInfo->StyleFlagsExtended |= WS_EX_WINDOWEDGE | WS_EX_COMPOSITED;
#endif

    // Add title if not set
    if(!pxWindowCreateInfo->WindowText.SizeUsed)
    {
        PXTextFromAddressA(&pxWindowCreateInfo->WindowText, name, 0, sizeof(name));
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


    // Normally, we want to create a physical window from the OS.
    // But.. if we want to spawn our own buttons and elements, we can also create a "fake" one.
    // This decreses the amount of "managing" code and lets the UI scale way better.
    // For this fake window, we just create a framebuffer
    if(pxWindowCreateInfo->UseFrameBuffer)
    {
        PXWindowCreateFrameBuffer(pxWindow, pxWindowCreateInfo);
    }

    if(pxWindowCreateInfo->SkipHandleCreation)
    {
        PXWindowCreatePhysical(pxWindow, pxWindowCreateInfo);
    }

    if(pxWindowCreateInfo->DoOpenGL)
    {
       // PXWindowOpenGLEnable(pxWindow);
    }

#if PXLogEnable
    PXID windowParentID = (pxWindowParent ? pxWindowParent->Info.ID : -1);
    PXWindowHandle pxWindowHandleParent = PXWindowHandleGet(pxWindowParent);    

    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "Create",
        "Window creation done.\n"
        "%20s : %i\n"
        "%20s : %.*s (%i)\n"
        "%20s : %p\n"
        "%20s : %p (PXID:%4i)\n"
        "%20s : %p\n"
        "%20s : %p\n"
        "%20s : R:%3.3i G:%3.3i B:%3.3i",
        "PXID", pxWindow->Info.ID,
        "Name", pxWindowCreateInfo->Info.Name.SizeUsed,
        pxWindowCreateInfo->Info.Name.A,
        pxWindowCreateInfo->Info.Name.SizeUsed,
        "Window-Handle",        pxWindow->Handle.Data,
        "Parent-Handle", pxWindowHandleParent.Data, windowParentID,
        "PixelBuffer-Handle", pxWindow->PixelBuffer.Data,
        "OpenGL-Handle", pxWindow->GLContext.Data,
        "BackGround", pxWindow->BackGroundColor.Red, pxWindow->BackGroundColor.Green, pxWindow->BackGroundColor.Blue
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
#endif


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
                case PXEngineUIElementTypeButton:
                    magicID = BS_OWNERDRAW;
                    break;

                case PXEngineUIElementTypeRenderFrame:
                case PXEngineUIElementTypeColorPicker:
                case PXEngineUIElementTypePanel:
                default: // TODO: problem with default value, we cant detect if we have the wrong enum type
                    magicID = SS_OWNERDRAW | SS_BLACKRECT;
                    break;
            }

            pxWindowCreateInfo->StyleFlags |= magicID;
        }
#endif // OSWindows
    }

    PXWindowCreatePhysicalEventDragAndDropAdd(pxWindow, pxWindowCreateInfo);
    PXWindowCreatePhysicalEventListenerAdd(pxWindow, pxWindowCreateInfo);
    PXWindowCreatePhysicalSpawn(pxWindow, pxWindowCreateInfo);

    PXWindowTitleBarColor(pxWindow);

    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
    // pxWindow->Type == PXEngineUIElementTypeText&&;

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



    // We need to readjust the window size, as the titlebar does reduce our actual rendering size
    PXWindowSpaceDrawGet(pxWindow, &pxWindow->Layout.SpaceContent);


    PXECSInfoFlagStateAdd
    (
        &pxWindow->Info,
        PXECSFlagBehaviourActive |
        PXECSFlagBehaviourCanRender |
        PXECSInfoUseByOS
    );

#if PXLogEnable && 0
    const char* uielementName = PXEngineUIElementTypeToString(pxWindowCreateInfo->Type);

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





#if 0
    PXWindowOpenGLEnable(pxWindow);
#endif


    // PXWindowDragAndDropBegin(pxWindow);

#if 0
    if(pxWindow->ID && pxWindowCreateInfo->UIElementParent)
    {
        PXWindow* parent = pxWindowCreateInfo->UIElementParent;

        HDWP resA = BeginDeferWindowPos(1);

        if(PXEngineUIElementTypeButtonText == pxWindow->Type)
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

PXResult PXAPI PXWindowCreatePhysicalEventListenerAdd(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    if(!(pxWindow && pxWindowCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplayHandle = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;

    if(!xDisplayHandle)
    {
        return PXResultRefusedHandleNull;
    }

    if(!xGraphicContext)
    {
        return PXResultRefusedHandleNull;
    }

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

    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    const int selectResultID = XISelectEvents
    (
        xDisplayHandle,
        pxWindowHandle.Data,
        &eventmask,
        1u
    );
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
            xDisplayHandle,
            pxWindowHandle.Data,
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
            pxWindow->Handle.Data
        );
#endif
    }
#elif OSWindows

#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowCreatePhysicalEventDragAndDropAdd(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    if(!(pxWindow && pxWindowCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplayHandle = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;
    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    if(!xDisplayHandle)
    {
        return PXResultRefusedHandleNull;
    }

    if(!xGraphicContext)
    {
        return PXResultRefusedHandleNull;
    }

#if 0 // Grab means literally Drag%Drop grab. This is not mouse motion
    //bool   ret    = false;
    XID cursor = XCreateFontCursor(xDisplayHandle, XC_crosshair);
    int root = DefaultRootWindow(xDisplayHandle);

    const int grabResult = XGrabPointer
    (
        xDisplayHandle,
        pxWindowHandle.Data,
        0,
        ButtonMotionMask,
        GrabModeAsync,
        GrabModeAsync,
        root,
        None,
        CurrentTime
    );
#endif
#elif OSWindows
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowCreatePhysicalSpawn(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    if(!(pxWindow && pxWindowCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXWindow PXREF pxWindowParent = PXWindowParentGet(pxWindow);


    // Add the engine context here, to be able to fetch it inside the window events
    pxWindow->EngineUI = pxWindowCreateInfo->EngineUI;

#if OSUnix
    const Display PXREF xDisplayHandle = pxWindowCreateInfo->DisplayHandle;
    const Window xWindowHandleParrent = PXWindowParentHandleGet(pxWindow, xDisplayHandle);
    const int screenIndex = pxWindowCreateInfo->ScreenHandleNumber; // ???
    PXBool successful = PXFalse;

    if(!xDisplayHandle)
    {
        return PXResultRefusedHandleInvalid;
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

    const XVisualInfo PXREF visualInfo = glXChooseVisual
    (
        xDisplayHandle,
        screenIndex,
        attributeList
    );
    successful = visualInfo != 0;

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

    // default color map?

    // Create colormapping
    Colormap colormap = XCreateColormap
    (
        xDisplayHandle,
        xWindowHandleParrent,
        visualInfo->visual,
        AllocNone
    );
#endif

    // Create window

    if(pxWindowCreateInfo->Simple)
    {
        pxWindow->Handle.Data = XCreateSimpleWindow
        (
            xDisplayHandle,
            xWindowHandleParrent,
            pxWindow->Layout.SpaceContent.X,
            pxWindow->Layout.SpaceContent.Y,
            pxWindow->Layout.SpaceContent.Width,
            pxWindow->Layout.SpaceContent.Height,
            pxWindow->Layout.BorderWidth,
            WhitePixel(xDisplayHandle, screenIndex),
            BlackPixel(xDisplayHandle, screenIndex)
        );

        if(pxWindow->Handle.Data)
        {
            // Create events
          //  XSelectInput(pxWindow->DisplayHandle, pxWindow->Handle.Data, inputEventsToListen);
        }
    }
    else
    {
        pxWindow->Handle.Data = XCreateWindow
        (
            xDisplayHandle,
            xWindowHandleParrent,
            pxWindow->Layout.SpaceContent.X,
            pxWindow->Layout.SpaceContent.Y,
            pxWindow->Layout.SpaceContent.Width,
            pxWindow->Layout.SpaceContent.Height,
            pxWindow->Layout.BorderWidth,
            24,//visualInfo->depth,
            InputOutput,
            0, //visualInfo->visual,
            CWColormap | CWEventMask,
            0//&setWindowAttributes
        );
    }

    successful = PXNull != pxWindow->Handle.Data;

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
        pxWindow->Handle.Data,
        "Position", pxWindow->Layout.SpaceContent.X, pxWindow->Layout.SpaceContent.Y,
        "Size", pxWindow->Layout.SpaceContent.Width, pxWindow->Layout.SpaceContent.Height,
        "BorderWidth", pxWindow->Layout.BorderWidth,
        "Depth", 0//visualInfo->depth
    );
#endif


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

    XSelectInput(xDisplayHandle, pxWindow->Handle.Data, inputEventsToListen);


    // Attach to render engine
    {
        const int mapResultID = XMapWindow(xDisplayHandle, pxWindow->Handle.Data);

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
        // const PXBool setTextSuccess = PXNativDrawTextSet(pxNativDraw, pxWindow, pxWindowCreateInfo->Name, -1);
    }

#elif OSWindows

    HWND parrantHandle = pxWindowParent ? pxWindowParent->Handle.Data : PXNull;

    switch(pxWindowCreateInfo->WindowText.Format)
    {
        case TextFormatASCII:
        {
            pxWindow->Handle.Data = CreateWindowExA // Windows 2000, User32.dll, winuser.h
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
            pxWindow->Handle.Data = CreateWindowExW // Windows 2000, User32.dll, winuser.h
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

    const PXResult createResult = PXErrorCurrent(PXNull != pxWindow->Handle.Data);

    if(PXResultOK != createResult)
    {
        return createResult;
    }

    PXWindowDCGet(pxWindow);
#endif

    return PXResultOK;
}

PXResult PXAPI PXWindowCreateFrameBuffer(PXWindow PXREF pxWindow, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
    PXRectangleXYWHI32 pxRectangle;

    PXWindowSpaceDrawGet(pxWindow, &pxRectangle);

    PXFrameBufferCreateInfo pxFrameBufferCreateInfo;
    PXClear(PXFrameBufferCreateInfo, &pxFrameBufferCreateInfo);
    pxFrameBufferCreateInfo.System = PXGraphicAPINative;
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

const char* PXWindowDockSideToString(const PXEngineUIDockSide pxWindowDockSide)
{
    return PXWindowDockSideString[pxWindowDockSide];
}

PXWindowHandle PXAPI PXWindowHandleGet(const PXWindow PXREF pxWindow)
{
    PXWindowHandle pxWindowHandle;

    if(!pxWindow)
    {
        pxWindowHandle.Data = 0;
        return pxWindowHandle;
    }

    return pxWindow->Handle;
}

PXWindowHandle PXAPI PXWindowHandleGetAtCursorPosition()
{
    POINT point;
    const BOOL result = GetCursorPos(&point);
    PXWindowHandle pxWindowHandle = PXWindowHandleGetAtPosition(point.x, point.y);

    return pxWindowHandle;
}

PXWindowHandle PXAPI PXWindowHandleGetAtPosition(const int x, const int y)
{
    PXWindowHandle pxWindowHandle;

    POINT p = {x, y};
    pxWindowHandle.Data = WindowFromPoint(p);

    return pxWindowHandle;
}

PXBool PXAPI PXWindowOwnedByThisProcess(PXWindow PXREF pxWindow)
{
    if(!pxWindow)
    {
        return PXNull;
    }

    return PXWindowHandleOwnedByThisProcess(pxWindow->Handle);
}

PXBool PXAPI PXWindowHandleOwnedByThisProcess(const PXWindowHandle pxWindowHandle)
{
    HWND windowHandle = pxWindowHandle.Data;

    if(!windowHandle)
    {
        return PXFalse;
    }

    const DWORD myProcessID = GetCurrentProcessId();
    DWORD windowProcessID = 0;  

    // Get the process ID of the window under the mouse
    GetWindowThreadProcessId(windowHandle, &windowProcessID);

    return windowProcessID == myProcessID;
}

// 1. Define the function pointer signature for the modern Windows 10 API
typedef UINT(WINAPI* PFN_GetDpiForWindow)(HWND);

PXF32 PXAPI PXWindowDPIScaleFactorGet(const PXWindowHandle pxWindowHandle)
{
    HWND windowHandle = pxWindowHandle.Data;
    UINT dpi = 96; // 96 DPI is the industry standard 1.0x baseline scale

    // 2. Try to dynamically load the modern user32 library
    HMODULE hUser32 = LoadLibraryA("user32.dll");
    if(hUser32) 
    {
        // Look for the modern Windows 10 function inside the library
        PFN_GetDpiForWindow pGetDpiForWindow = (PFN_GetDpiForWindow)GetProcAddress(hUser32, "GetDpiForWindow");

        if(pGetDpiForWindow) {
            // Success! We are running on Windows 10/11. Query the window's exact monitor DPI.
            dpi = pGetDpiForWindow(windowHandle);
        }
        FreeLibrary(hUser32);
    }

    // 3. Fallback: If pGetDpiForWindow is null, we are running on Windows 95/98/XP/7
    if(dpi == 96) 
    {
        HDC hScreenDC = GetDC(NULL);
        if(hScreenDC) 
        {
            // GetDeviceCaps works perfectly even on Windows 95
            dpi = GetDeviceCaps(hScreenDC, LOGPIXELSX);
            ReleaseDC(NULL, hScreenDC);
        }
    }

    // 4. Return the floating scale factor multiplier (e.g., 96->1.0f, 144->1.5f, 192->2.0f)
    return (float)dpi / 96.0f;
}

#if OSUnix

#elif OSWindows
HDC PXAPI PXWindowDCGet(PXWindow PXREF pxWindow)
{
    HWND windowHandle = PXNull;

    if(!pxWindow)
    {
        return PXNull;
    }

    windowHandle = pxWindow->Handle.Data;

    if(!windowHandle)
    {
        return PXNull;
    }

    if(pxWindow->PixelBuffer.Data)
    {
        return pxWindow->PixelBuffer.Data;
    }

    pxWindow->PixelBuffer.Data = GetDC(windowHandle);

    // Sometimes we do want NULL? How to handle this??
    //PXAssert(pxWindow->DeviceContextHandle, "The device context can only be NULL if the window handle is invalid");

    return pxWindow->PixelBuffer.Data;
}

PXWindow* PXAPI PXWindowFromHandle(const HWND windowHandle)
{
    return (PXWindow*)GetWindowLongPtr(windowHandle, GWLP_USERDATA);
}

void PXAPI PXWindowStoreSelf(PXWindow PXREF pxWindow, const HWND windowHandle)
{
    SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)pxWindow);

    // Store created handle from event data to our object
    pxWindow->Handle.Data = windowHandle;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        "REF",
        "PXID:<%i> can now be fetched from a HWND:<%p>",
        pxWindow->Info.ID,
        windowHandle
    );
#endif
}

PXOpenGLContext PXAPI PXWindowGLContextGet(const PXWindow PXREF pxWindow)
{
    return pxWindow->GLContext;
}

PXResult PXAPI PXWindowGLContextSet(PXWindow PXREF pxWindow, const PXOpenGLContext pxOpenGLContext)
{
    pxWindow->GLContext = pxOpenGLContext;

    return PXResultOK;
}

PXResult PXAPI PXWindowDisplayListFetch()
{
#if OSUnix && 0

    Display* xDisplay = &pxNativDraw->GUISystem->DisplayCurrent;

    pxNativDraw->GUISystem->DisplayCurrent.ScreenDefaultID = XDefaultScreen(xDisplay);
    pxNativDraw->GUISystem->DisplayCurrent.ScreenListAmount = XScreenCount(xDisplay);

    for(PXSize screenID = 0; screenID < pxNativDraw->GUISystem->DisplayCurrent.ScreenListAmount; ++screenID)
    {
        PXDisplayScreen* pxDisplayScreen = &pxNativDraw->GUISystem->DisplayCurrent.DisplayScreenList[screenID];

        pxDisplayScreen->Width = XDisplayWidth(xDisplay, screenID);
        pxDisplayScreen->Height = XDisplayHeight(xDisplay, screenID);
        pxDisplayScreen->Cells = XDisplayCells(xDisplay, screenID);
        pxDisplayScreen->Planes = XDisplayPlanes(xDisplay, screenID);
        pxDisplayScreen->WidthMM = XDisplayWidthMM(xDisplay, screenID);
        pxDisplayScreen->HeightMM = XDisplayHeightMM(xDisplay, screenID);

        pxDisplayScreen->IsConnected = PXTrue;
        pxDisplayScreen->IsPrimary = screenID == pxNativDraw->GUISystem->DisplayCurrent.ScreenDefaultID;
    }

#elif OSWindows && 0




    //
    {
        PXEindowsDisplayEnumInfo pxEindowsDisplayEnumInfo;
        pxEindowsDisplayEnumInfo.MonitorList = PXNull;
        pxEindowsDisplayEnumInfo.OffsetCurrent = 0;
        pxEindowsDisplayEnumInfo.OffsetMaxmimal = 0;
        pxEindowsDisplayEnumInfo.CheckMode = 1;

        pxNativDraw->MonitorListAmount = 0;

        const BOOL success = EnumDisplayMonitors
        (
            NULL,
            NULL,
            (MONITORENUMPROC)PXWindowsMonitorFetch,
            (LPARAM)&pxEindowsDisplayEnumInfo
        );

        pxNativDraw->MonitorListAmount = pxEindowsDisplayEnumInfo.OffsetMaxmimal;
        pxNativDraw->MonitorList = PXMemoryHeapReallocT(PXMonitor, PXNull, pxNativDraw->MonitorListAmount);

        pxEindowsDisplayEnumInfo.MonitorList = pxNativDraw->MonitorList;
        pxEindowsDisplayEnumInfo.CheckMode = 0;

        EnumDisplayMonitors
        (
            NULL,
            NULL,
            (MONITORENUMPROC)PXWindowsMonitorFetch,
            (LPARAM)&pxEindowsDisplayEnumInfo
        );
    }





    // List all devices
    {
        DWORD amount = 0;
        DWORD dwFlags = 0;

        DISPLAY_DEVICEA displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICEA);

        for(;;)
        {
            const PXBool didDetectDevice = EnumDisplayDevicesA(NULL, amount, &displayDevice, dwFlags);

            if(!didDetectDevice)
            {
                break;
            }

            ++(amount);
        }



        if(0 == amount) // No devices found
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                PXNativDraw,
                "Display-Fetch",
                "Devices detected : none"
            );
#endif

            return PXResultOK;
        }


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXNativDraw,
            "Display-Fetch",
            "Devices detected : %i",
            amount
        );
#endif

        PXDisplay* pxDisplayList = PXMemoryHeapReallocT(PXDisplay, PXNull, amount);

        pxNativDraw->DisplayList = pxDisplayList;
        pxNativDraw->DisplayListAmount = amount;

        for(PXSize deviceID = 0; EnumDisplayDevicesA(NULL, deviceID, &displayDevice, dwFlags); deviceID++)
        {
            PXDisplay PXREF pxDisplay = &pxDisplayList[deviceID];
            pxDisplay->IsConnected = PXFlagIsSet(displayDevice.StateFlags, DISPLAY_DEVICE_ATTACHED_TO_DESKTOP);
            pxDisplay->IsPrimary = PXFlagIsSet(displayDevice.StateFlags, DISPLAY_DEVICE_PRIMARY_DEVICE);

            PXTextCopyA(displayDevice.DeviceName, 32, pxDisplay->NameID, PXDisplayScreenNameLength);
            PXTextCopyA(displayDevice.DeviceString, 128, pxDisplay->GraphicDeviceName, PXDisplayScreenDeviceLength);
            // PXTextCopyA(displayDevice.DeviceID, 128, pxDisplayScreen->DeviceID, PXDeviceIDSize);
            //PXTextCopyA(displayDevice.DeviceKey, 128, pxDisplayScreen->DeviceKey, PXDeviceKeySize);

            /*
             PXSize position = PXTextFindFirstCharacterA(monitorDeviceA.DeviceID, 128, '\\');

                    PXSize targetZize = 128 - position + 1;
                    char* target = monitorDeviceA.DeviceID + position + 1;

                    PXSize positionB = PXTextFindFirstCharacterA(target, targetZize, '\\');

                    PXTextCopyA(target, positionB, pxDisplayScreen->NameMonitor, PXDisplayScreenMonitorLength);
            */


            DWORD    iModeNum = ENUM_CURRENT_SETTINGS;
            DEVMODEA devMode = { 0 };
            DWORD    dwFlags = EDS_RAWMODE;

            devMode.dmSize = sizeof(DEVMODEA);

            const BOOL settingsResult = EnumDisplaySettingsExA
            (
                displayDevice.DeviceName,
                iModeNum,
                &devMode,
                dwFlags
            );

            if(!pxDisplay->IsConnected)
            {
                continue;
            }

            pxDisplay->Width = devMode.dmPelsWidth;
            pxDisplay->Height = devMode.dmPelsHeight;
        }
    }

#endif


#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXNativDraw,
        "Display-Device",
        "Fetching <%i> monitor devices",
        pxNativDraw->DisplayListAmount
    );

    for(PXSize i = 0; i < pxNativDraw->DisplayListAmount; ++i)
    {
        PXDisplay PXREF pxDisplay = &pxNativDraw->DisplayList[i];

        char* Data;
        char* Name;

        int ProtocolVersion;
        int ProtocolRevision;

        char* ServerVendor;
        int VendorRelease;

        int ScreenDefaultID;
        int ScreenListAmount;

        PXSize DisplayScreenListAmount;
        PXDisplayScreen* DisplayScreenList;


        PXLogPrint
        (
            PXLoggingInfo,
            PXNativDraw,
            "Display-Device",
            "Detected:\n"
            "+--------------------------------------------------------+\n"
            "| DeviceName     : %-27s %-4ix%4i |\n"
            "| DeviceString   : %-37.37s |\n"
            "+--------------------------------------------------------+\n",
            pxDisplay->Name,
            pxDisplay->Width,
            pxDisplay->Height,
            pxDisplay->NameID
        );
    }
#endif

    return PXResultOK;
}
/*
PXResult PXAPI PXWindowProperty(PXWindow PXREF pxWindow, PXWindowProperty PXREF pxWindowPropertyInfoList, const PXSize amount)
{
    PXWindow PXREF pxWindowParrent = 0;// (PXWindow*)pxWindow->Info.Hierarchy.Parrent;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXWindowProperty PXREF pxGUIProperty = &pxWindowPropertyInfoList[i];

        if(!pxWindow)
        {
            continue;
        }

        switch(pxGUIProperty->Property)
        {
            case PXWindowPropertyTypeTextContent:
            {
#if 0
                PXEngineUIElementTextInfo PXREF pxUIElementTextInfo = &pxWindowUpdateInfo->Data.Text;

                if(!pxWindow)
                {
                    return PXResultInvalid;
                }

#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNativDraw,
                    "Update-Text",
                    "(0x%p) %s",
                    (int)pxWindow->ID,
                    pxUIElementTextInfo->Content
                );
#endif

                PXWindowTextSet(pxGUIManager, pxWindow, pxUIElementTextInfo->Content);
#endif


#if OSWindows



                //   SendMessageA(pxWindow->ID, PBM_SETPOS, stepsConverted, 0);


#if 0
                PXEngineUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXEngineUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                PXWindowSizeInfo pxWindowSizeInfo;

                PXWindowID pxWindowID = pxWindowUpdateInfo->WindowReference ? pxWindowUpdateInfo->WindowReference->ID : PXNull;

                PXWindowSizeGet(pxWindowID, &pxWindowSizeInfo);

                pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Height;

                PXEngineUIElementPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);

                const PXBool succes54s = MoveWindow
                (
                    pxWindow->ID,
                    pxUIElementPositionCalulcateInfo.X,
                    pxUIElementPositionCalulcateInfo.Y,
                    pxUIElementPositionCalulcateInfo.Width,
                    pxUIElementPositionCalulcateInfo.Height,
                    PXTrue
                );
#endif
#endif




                break;
            }
            case PXWindowPropertyTypeAllign:
            {
                // PXEngineUIElementTextInfo PXREF pxUIElementTextInfo = &pxWindowUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                HDC hdc = PXWindowDCGet(pxWindow);
                LONG ww = SetWindowLongA(hdc, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

                // const UINT allign = SetAlign(xx, TA_CENTER);
                // const PXBool success = GDI_ERROR == allign;

                if(!ww)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        "UI",
                        "Text-Update",
                        "Allign Failed"
                    );
#endif
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "UI",
                    "Text-Update",
                    "Allign successful"
                );
#endif

#endif

                break;
            }
            case PXWindowPropertyTypeSize:
            {
                PXWindowProperty sizeFetchInfo;
                PXClear(PXWindowProperty, &sizeFetchInfo);
                //sizeFetchInfo.WindowCurrent = pxGUIProperty->WindowParent;
                sizeFetchInfo.Property = PXWindowPropertyTypeSize;
                sizeFetchInfo.UpdateType = PXWindowPropertyUpdateTypeRead;

#if 0
                PXNativDrawWindowProperty(pxNativDraw, pxWindowParrent, &sizeFetchInfo, 1);

                PXWindowPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
                PXClear(PXWindowPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

                //PXWindowSizeInfo pxWindowSizeInfo;
                //PXWindowID pxWindowID = pxWindow->Parent ? pxWindow->Parent->ID : PXNull;

                //PXWindowSizeGet(PXNull, &pxWindowSizeInfo); // TODO: Problematic

                pxUIElementPositionCalulcateInfo.WindowWidth = sizeFetchInfo.Size.Width;
                pxUIElementPositionCalulcateInfo.WindowHeight = sizeFetchInfo.Size.Height;

                //PXWindowPositionCalculcate(pxWindow, &pxUIElementPositionCalulcateInfo);
#endif


                // If the window is an actual window, we will have bad allignment if we use MoveWindow
                //if(pxWindow->Type != PXEngineUIElementTypeWindow)

#if 0
                if(1)
                {
                    const PXBool moveSuccess = MoveWindow
                    (
                        pxWindow->Info.Handle.WindowHandle,
                        pxUIElementPositionCalulcateInfo.Size.X,
                        pxUIElementPositionCalulcateInfo.Size.Y,
                        pxUIElementPositionCalulcateInfo.Size.Width,
                        pxUIElementPositionCalulcateInfo.Size.Height,
                        PXTrue
                    );
                    const PXResult moveResult = PXErrorCurrent(moveSuccess);
                }
#endif
#if 0
                else
                {
                    const BOOL result = SetWindowPos
                    (
                        pxWindow->ID,
                        PXNull,
                        pxUIElementPositionCalulcateInfo.X,
                        pxUIElementPositionCalulcateInfo.Y,
                        pxUIElementPositionCalulcateInfo.Width,
                        pxUIElementPositionCalulcateInfo.Height,
                        SWP_NOZORDER
                    );
                }
#endif

                // If we have a render window, we also need to fix the viewport
               // if(PXEngineUIElementTypeRenderFrame == pxWindow->Type)
                {
#if 0
                    PXEngineUIElementSceneRenderInfo PXREF pxUIElementSceneRenderInfo = &pxGUIProperty->SceneRender;
                    PXEngine* pxEngine = pxUIElementSceneRenderInfo->Engine;

                    if(pxEngine)
                    {
                        PXViewPort pxViewPort;

                        PXFunctionInvoke(pxEngine->Graphic.ViewPortGet, pxEngine->Graphic.EventOwner, &pxViewPort);

                        pxViewPort.X = 0;//  pxUIElementPositionCalulcateInfo.X;
                        pxViewPort.Y = 0;// pxUIElementPositionCalulcateInfo.Y;
                        pxViewPort.Width = pxUIElementPositionCalulcateInfo.Width;
                        pxViewPort.Height = pxUIElementPositionCalulcateInfo.Height;

                        PXFunctionInvoke(pxEngine->Graphic.ViewPortSet, pxEngine->Graphic.EventOwner, &pxViewPort);
                    }
#endif

                }


#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNativDrawText,
                    "Update-Size",
                    "X:%4i, Y:%4i, W:%4i, H:%4i\n\n\n\n",
                    (int)pxUIElementPositionCalulcateInfo.Size.X,
                    (int)pxUIElementPositionCalulcateInfo.Size.Y,
                    (int)pxUIElementPositionCalulcateInfo.Size.Width,
                    (int)pxUIElementPositionCalulcateInfo.Size.Height
                );
#endif

                break;
            }
            case PXWindowPropertyTypeProgressbarPercentage:
            {
#if 0
                PXEngineUIElementProgressBarInfo PXREF progressBar = &pxWindowUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                PXI32U stepsConverted = progressBar->Percentage * 100;
                SendMessageA(pxWindow->Info.Handle.WindowHandle, PBM_SETPOS, stepsConverted, 0);
#endif
#endif

                break;
            }
            case PXWindowPropertyTypeProgressbarBarColor:
            {
#if 0
                PXEngineUIElementProgressBarInfo PXREF progressBar = &pxWindowUpdateInfo->Data.Text;

#if OSUnix
#elif OSWindows

                COLORREF color = RGB(progressBar->BarColor.Red, progressBar->BarColor.Green, progressBar->BarColor.Blue);
                SendMessageA(pxWindow->Info.Handle.WindowHandle, PBM_SETBARCOLOR, 0, color);
#endif
#endif

                break;
            }
            case PXWindowPropertyTypeVisibility:
            {
#if OSUnix
#elif OSWindows

                PXBool show = pxGUIProperty->Show;
                int showID = show ? SW_SHOW : SW_HIDE;


                const PXBool isWindowValid = PXWindowIsValid(pxWindow);


                if(show)
                {
                    PXWindowShow(pxWindow);

                    // pxWindow->Info.Behaviour |= PXECSInfoRender;
                }
                else
                {
                    PXWindowHide(pxWindow);

                    //pxWindow->Info.Behaviour &= ~PXECSInfoRender;
                }







                //  HWND parrent = GetParent(pxWindow->ID);

                //  HWND grandParrent = GetParent(parrent);

                //  UpdateWindow(pxWindow->ID);
                //  UpdateWindow(parrent);
                //  UpdateWindow(grandParrent);

                //  UpdateWindow(sourceObject);
                // FlashWindow(sourceObject, 1);

#if PXLogEnable && 0
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXNativDraw,
                    "Visibility",
                    "Page %20s, (0x%p), Mode:%s Status:%s",
                    "",//pxWindow->NameData,
                    pxWindow->Info.Handle.WindowHandle,
                    show ? "Show" : "Hide",
                    res ? "OK" : "FAIL"
                );
#endif


                // Own solution
                {
                    //  PXWindowChildListEnumerate(pxGUIManager, pxWindow, show);



                }

                /*
                const BOOL success = EnumChildWindows
                (
                    pxWindow->Info.Handle.WindowHandle,
                    PXNativeWindowListUpdateEnum,
                    &showID
                );
#endif
* /

                break;
            }
            case PXWindowPropertyTypeComboBoxAdd:
            {
#if 0
                PXEngineUIElementComboBoxInfo PXREF pxUIElementComboBoxInfo = &pxGUIProperty->Data.ComboBox;

#if OSUnix
#elif OSWindows

                for(size_t i = 0; i < pxUIElementComboBoxInfo->DataListAmount; i++)
                {
                    char* name = pxUIElementComboBoxInfo->DataList[i];

                    ComboBox_AddString(pxWindow->Info.Handle.WindowHandle, name);
                }

                ComboBox_SetCurSel(pxWindow->Info.Handle.WindowHandle, 0);
#endif
#endif

                break;
            }
            case PXWindowPropertyTypeItemAdd:
            {
#if 0

                PXEngineUIElementTreeViewItemInfo PXREF pxUIElementTreeViewItemInfo = &pxGUIProperty->Data.TreeViewItem;

#if OSUnix
#elif OSWindows

                TVINSERTSTRUCT item;
                PXClear(TVINSERTSTRUCT, &item);

                const char text[] = "[N/A]";

                item.item.pszText = text;
                item.item.cchTextMax = sizeof(text);
                item.hInsertAfter = TVI_ROOT;
                item.item.mask = TVIF_TEXT;

                TreeView_InsertItem(pxWindow->Info.Handle.WindowHandle, &item);

#endif
#endif

                break;
            }
        }
    }

    return PXResultOK;
}
*/


PXResult PXAPI PXWindowResize(PXWindow PXREF pxWindow, const int width, const int height)
{
    PXResult pxResult = PXResultInvalid;

#if 0

#if OSUnix
    const int resultID = 0;//XResizeWindow(Display *display, Window w, width, height);
    pxResult = PXErrorCurrent(resultID);
#elif PXOSWindowsDestop
    const UINT flags = SWP_NOMOVE | SWP_NOZORDER | SWP_NOOWNERZORDER;
    const PXBool success = SetWindowPos
    (
        pxWindow->Info.Handle.WindowHandle,
        PXNull,
        0,
        0,
        width,
        height,
        flags
    ); // Windows 2000, User32.dll
    pxResult = PXErrorCurrent(success);
#else
    pxResult = PXResultRefusedNotSupportedByLibrary;
#endif

#endif

    return pxResult;
}

PXResult PXAPI PXWindowMoveAndResize(PXWindow PXREF pxWindow, const int x, const int y, const int width, const int height)
{
    PXResult pxResult = PXResultInvalid;

#if OSUnix && 0
    const int resultID = 0;//XMoveResizeWindow(Display *display, Window w, int x, int y, unsigned int width, unsigned int height);
    pxResult = PXErrorCurrent(resultID);
#elif PXOSWindowsDestop && 0
    const PXBool success = MoveWindow(pxWindow->Info.Handle.WindowHandle, x, y, width, height, PXTrue); // Windows 2000, User32.dll, winuser.h
    pxResult = PXErrorCurrent(success);
#else
    pxResult = PXResultRefusedNotSupportedByLibrary;
#endif

    return pxResult;
}

PXResult PXAPI PXWindowCursorCollisionCheck()
{
#if 0
    const int cursorSize = 16;
    PXVector2I32S cursorPosition;

    PXNativDrawCursorPosition(pxNativDraw, &cursorPosition, 0);

    PXDictionary PXREF pxDictionaryUI = &pxNativDraw->ResourceManager->GUIElementLookup;

    for(size_t i = 0; i < pxDictionaryUI->List.EntryAmountUsed; i++)
    {
        PXVector2I32S windowPosition;

        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(pxDictionaryUI, i, &pxDictionaryEntry);

        PXWindow PXREF pxWindow = *(PXWindow**)pxDictionaryEntry.Value;

#if OSWindows
        RECT winRec;

        GetWindowRect(pxWindow->Info.Handle.WindowHandle, &winRec);
        // PXNativDrawWindowPosition(pxNativDraw, pxWindow, &windowPosition, 0);

        const PXBool isColliding = PXCollisionAABB
        (
            winRec.left,
            winRec.top,
            winRec.right,// - winRec.left,
            winRec.bottom,// - winRec.top,
            cursorPosition.X - cursorSize,
            cursorPosition.Y - cursorSize,
            cursorPosition.X + cursorSize,
            cursorPosition.Y + cursorSize
        );

#endif // OSWindows
        const PXBool shallDetect = (pxWindow->Info.Behaviour & PXWindowBehaviourHoverable);
        const PXI32U valueBefore = pxWindow->Info.Behaviour;

        if(!shallDetect)
        {
            //  continue;
        }


#if OSWindows
        if(isColliding)
        {
            pxWindow->Info.Behaviour |= PXWindowBehaviourIsBeingHovered;
        }
        else
        {
            pxWindow->Info.Behaviour &= ~PXWindowBehaviourIsBeingHovered;
        }


        const PXBool valueChanged = valueBefore != pxWindow->Info.Behaviour;

        if(valueChanged)
        {
            const BOOL xx = RedrawWindow(pxWindow->Info.Handle.WindowHandle, PXNull, PXNull, RDW_INVALIDATE);

            PXBool hovernow = (pxWindow->Info.Behaviour & PXWindowBehaviourIsBeingHovered) > 0;

            PXLogPrint
            (
                PXLoggingInfo,
                "UI-Event",
                "Hover",
                "ID:%i - %s",
                pxWindow->Info.ID,
                hovernow ? "Yes" : "No"
            );
        }
#endif // OSWindows
    }
#endif
    return PXResultOK;
}














































#if 0










#if 0
// Call this once per frame to draw a spinning triangle.
// Requirements: a current OpenGL context (wglMakeCurrent),
// a valid viewport/projection, and double buffering enabled.
void PXAPI PXWindowDrawScene(void)
{
#if OSUnix
#elif OSWindows

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

#endif
}


PXResult PXAPI PXWindowDrawGDI(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
#if OSUnix
#elif OSWindows

    PXColorRGB_U8* color = &pxWindow->BackGroundColor;

    HWND windowHandle = pxWindow->Handle.Data;
    HDC hdc = pxWindow->PixelBuffer.Data;

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
    hdr.bottom = hdr.top + pxWindow->TitleBarHeight;

    PXColorRGB_U8 PXColorRGB_U8 = { 60, 60, 60 };
    PXWindowPaintPattern(windowHandle, hdc, &hdr, &PXColorRGB_U8);

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

#endif // OSUnix

    return PXResultOK;
}

#endif







#if OSWindows
BOOL CALLBACK EnumMyWindowsProc(const HWND hwnd, PXWindowSearchResult PXREF pxWindowSearchResult)
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

    pxWindow = PXWindowFromHandle(hwnd);

    if(pxWindow == NULL)
    {
        return TRUE;
    }

    PXText pxText;

    PXNamePoolGet(pxWindow->Info.ID, &pxText);

    PXBool isTarget = PXTextCompare(&pxText, pxWindowSearchResult->Name, 0);

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
#endif


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

    RECT rc;// = PXWindowRectOf(pxWindow->Handle.Data);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;
    const int bandW = 260;  // left/right band width
    const int bandH = 180;  // top band height

    for(HWND child = GetWindow(pxWindow->Handle.Data, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT))
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

    RECT rc;// = PXWindowRectOf(pxWindow->Handle.Data);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;

    const int bandW = 260;  // left/right band width (could be per-pane)
    const int bandH = 180;  // top/bottom band height (could be per-pane)

    // PASS 1: LEFT/RIGHT
    for(HWND child = GetWindow(pxWindow->Handle.Data, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT))
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
    for(HWND child = GetWindow(pxWindow->Handle.Data, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
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
    for(HWND child = GetWindow(pxWindow->Handle.Data, GW_CHILD); child; child = GetWindow(child, GW_HWNDNEXT)) {
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
    HWND hwnd = pxWindow->Handle.Data;

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
    HWND hwnd = pxWindow->Handle.Data;
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
            int minH = (p->minH > 0 ? p->minH : (p->TitleBarHeight > 0 ? p->TitleBarHeight : 0));
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
    if(axisLen < 0)
        axisLen = 0;

    // Compute SLOT sizes (including margins/padding)
    int* slotSizes = PXMemoryHeapCallocT(int, (n > 0 ? n : 1));

    if(!slotSizes)
        return;

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
#if OSUnix
#elif OSWindows
            * phdwp = DeferWindowPos(*phdwp, arr[i].h, NULL, x, y, w, h, SWP_FLAGS);
#endif
        }
        else
        {
#if OSUnix
#elif OSWindows
            MoveWindow(arr[i].h, x, y, w, h, TRUE);
#endif
        }

        // Advance cursor by SLOT size + spacing (spacing is outside panes)
        cursor += slotSizes[i] + (i + 1 < n ? sp : 0);
    }

    PXMemoryHeapFree(PXNull, slotSizes);
}

// Inset a rect by BoxInsets (band-level padding)
static RECT InsetRectBy(RECT r, BoxInsets in)
{
    r.left += in.l;
    r.top += in.t;
    r.right -= in.r;
    r.bottom -= in.b;

    if(r.right < r.left)
        r.right = r.left;
    if(r.bottom < r.top)
        r.bottom = r.top;
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

    RECT rc;// = PXWindowRectOf(pxWindow->Handle.Data);
    int leftX = rc.left;
    int rightX = rc.right;
    int topY = rc.top;
    int bottomY = rc.bottom;

    int total = CountVisiblePanes(pxWindow->Handle.Data);

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
        int n = CollectSide(pxWindow->Handle.Data, PXWindowDockSideLeft, buf, total);

        if(n > 0)
        {
            int bandW = cfg.defaultBandW;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefW > bandW) bandW = buf[i].p->prefW;

            RECT band = (RECT){ leftX, topY, leftX + bandW, bottomY };
            // Band-level padding:
            band = InsetRectBy(band, cfg.padLeft);

            leftX += bandW; // consume thickness regardless of inner padding

            ApplyStackWithBox(pxWindow->Handle.Data, &hdwp, band, TRUE /* vertical stack */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 1: RIGHT band(s)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->Handle.Data, PXWindowDockSideRight, buf, total);
        if(n > 0) {
            int bandW = cfg.defaultBandW;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefW > bandW) bandW = buf[i].p->prefW;

            RECT band = (RECT){ rightX - bandW, topY, rightX, bottomY };
            band = InsetRectBy(band, cfg.padRight);

            rightX -= bandW;

            ApplyStackWithBox(pxWindow->Handle.Data, &hdwp, band, TRUE /* vertical stack */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 2: TOP band(s) — use remaining width
    // -----------------------------
    {
        int n = CollectSide(pxWindow->Handle.Data, PXWindowDockSideTop, buf, total);
        if(n > 0) {
            int bandH = cfg.defaultBandH;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefH > bandH) bandH = buf[i].p->prefH;

            RECT band = (RECT){ leftX, topY, rightX, topY + bandH };
            band = InsetRectBy(band, cfg.padTop);

            topY += bandH;

            ApplyStackWithBox(pxWindow->Handle.Data, &hdwp, band, FALSE /* horizontal row */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 2: BOTTOM band(s)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->Handle.Data, PXWindowDockSideBottom, buf, total);

        if(n > 0)
        {
            int bandH = cfg.defaultBandH;
            for(int i = 0; i < n; ++i) if(buf[i].p->prefH > bandH) bandH = buf[i].p->prefH;

            RECT band = (RECT){ leftX, bottomY - bandH, rightX, bottomY };
            band = InsetRectBy(band, cfg.padBottom);

            bottomY -= bandH;

            ApplyStackWithBox(pxWindow->Handle.Data, &hdwp, band, FALSE /* horizontal row */, buf, n, spacingDefault);
        }
    }

    // -----------------------------
    // PASS 3: CENTER / FILL (DOCK_NONE)
    // -----------------------------
    {
        int n = CollectSide(pxWindow->Handle.Data, PXWindowDockSideNone, buf, total);
        RECT remain = (RECT){ leftX, topY, rightX, bottomY };
        remain = InsetRectBy(remain, cfg.padCenter);

        if(n > 0) {
            ApplyStackWithBox(pxWindow->Handle.Data, &hdwp, remain, cfg.centerVertical /* orientation */, buf, n, spacingDefault);
        }
    }

    HeapFree(GetProcessHeap(), 0, buf);

    if(hdwp)
        EndDeferWindowPos(hdwp);

    PXWindowEventPoll();

    // InvalidateRect(pxWindow->Handle.Data, 0, 0);
    // UpdateWindow(pxWindow->Handle.Data);
    // ShowWindow(pxWindow->Handle.Data, SW_MINIMIZE);
    // ShowWindow(pxWindow->Handle.Data, SW_SHOW);
}

BOOL PXAPI PXWindowScreenPtInMainClient(PXWindow PXREF pxWindow, PXVector2I32S PXREF pxVector2I32S)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    if(!pxWindowRoot)
        return FALSE;

    RECT rc; GetClientRect(pxWindowRoot->Handle.Data, &rc);

    POINT tl = { rc.left, rc.top },
        br = { rc.right, rc.bottom };

    ClientToScreen(pxWindowRoot->Handle.Data, &tl);
    ClientToScreen(pxWindowRoot->Handle.Data, &br);

    RECT scr = { tl.x, tl.y, br.x, br.y };

    POINT point = *(POINT*)pxVector2I32S;

    return PtInRect(&scr, point);
}

PXWindowDockSide PXAPI ChooseDockSide(PXWindow PXREF pxWindow, PXVector2I32S PXREF pxVector2I32S)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    RECT rectangle;
    GetClientRect(pxWindowRoot->Handle.Data, &rectangle);
    POINT topLeft = { rectangle.left, rectangle.top };
    POINT botomRight = { rectangle.right, rectangle.bottom };

    ClientToScreen(pxWindowRoot->Handle.Data, &topLeft);
    ClientToScreen(pxWindowRoot->Handle.Data, &botomRight);

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
    InvalidateRect(pxWindowRoot->Handle.Data, NULL, FALSE);
}

void PXAPI PXWindowHideDockOverlay(PXWindow PXREF pxWindow)
{
    PXWindow* pxWindowRoot = PXWindowRootGet(pxWindow);

    if(!pxWindowRoot)
        return;

    if(gOverlay.active) {
        gOverlay.active = FALSE;
        InvalidateRect(pxWindowRoot->Handle.Data, NULL, FALSE);
    }
}

void PXAPI PXWindowTearOffToFloating(PXWindow* pxWindow, POINT startScreen)
{
    if(pxWindow->State.Floating)
        return;

    PXBool reactGL = pxWindow->GLContext.Data > 0;

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

    RECT wr; GetWindowRect(pxWindow->Handle.Data, &wr);
    int w = wr.right - wr.left, h = wr.bottom - wr.top;
    MoveWindow(pxWindow->Handle.Data, startScreen.x - w / 2, startScreen.y - pxWindow->TitleBarHeight / 2, w, h, TRUE);
    ShowWindow(pxWindow->Handle.Data, SW_SHOWNORMAL);

    ReleaseCapture();
    SendMessageW(pxWindow->Handle.Data, WM_SYSCOMMAND, SC_MOVE | 0x0002, 0); // start system drag
}

void PXAPI PXWindowDockBackToChild(PXWindow* pxWindow, PXWindowDockSide side)
{
    if(pxWindow->State.Floating)
        return;

    PXBool reactGL = pxWindow->GLContext.Data > 0;

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
    if(!ov->active) 
        return;

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

#endif




/*

    XWindowAttributes attributes;

    const Status status = XGetWindowAttributes(pxWindow->DisplayHandle, pxNativDrawWindowHandle, &attributes);
    const PXBool validID = 0 != status;

    if(!validID)
    {
        return PXResultRefusedObjectIDInvalid;
    }

    return PXResultOK;
    
    */

#include "PXTransformView.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <PX/Engine/PXGUI.h>

const char PXTransformViewName[] = "TransformView";
const PXI8U PXTransformViewNameLength = sizeof(PXTransformViewName);
const PXECSRegisterInfoStatic PXTransformViewInfoStatic =
{
    {sizeof(PXTransformViewName), sizeof(PXTransformViewName), PXTransformViewName, TextFormatASCII},
    sizeof(PXTransformView),
    __alignof(PXTransformView),
    PXECSTypeEntity,
    PXTransformViewCreate,
    PXTransformViewRelease,
    PXTransformViewDraw
};
PXECSRegisterInfoDynamic PXTransformViewInfoDynamic;

void PXAPI PXTransformViewRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXTransformViewInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXTransformViewInfoDynamic;
}

PXResult PXAPI PXTransformViewCreate(PXTransformView** pxTransformViewREF, PXTransformViewCrerateInfo PXREF pxTransformViewCrerateInfo)
{
    PXTransformView* pxTransformView = PXNull;

    pxTransformViewCrerateInfo->Info.Static = &PXTransformViewInfoStatic;
    pxTransformViewCrerateInfo->Info.Dynamic = &PXTransformViewInfoDynamic;
    PXResult pxResult = PXECSCreate(pxTransformViewREF, pxTransformViewCrerateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTransformView = *pxTransformViewREF;
    pxTransformView->Position = pxTransformViewCrerateInfo->Position;

    pxTransformViewCrerateInfo->Window.EventList.CallBackOwner = pxTransformView;
    pxTransformViewCrerateInfo->Window.EventList.CallBackDraw = PXTransformViewDraw;

    PXWindowCreate(&pxTransformView->WindowBase, &pxTransformViewCrerateInfo->Window);

    return PXResultOK;
}

PXResult PXAPI PXTransformViewRelease(PXTransformView PXREF pxTransformView)
{
    return PXActionRefusedNotImplemented;
}

void DrawRect
(
    PXWindow PXREF pxWindow, 
    PXDrawInfo PXREF pxDrawInfo, 
    float x, 
    float y, 
    float w, 
    float h, 
    float r, 
    float g, 
    float b
)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}


void DrawRectWithText
(
    PXWindow PXREF pxWindow,
    PXDrawInfo PXREF pxDrawInfo,
    float x,
    float y,
    float w,
    float h,
    float r,
    float g,
    float b
)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}



void DrawRectVA(float x, float y, float w, float h, float r, float g, float b)
{
    GLfloat verts[8] = {
        x,     y,
        x + w,   y,
        x + w,   y + h,
        x,     y + h
    };

    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, verts);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
}

int GetTextWidth(PXWindow PXREF pxWindow, const char* text)
{
    HDC hdc = PXWindowDCGet(pxWindow);

    SIZE size;
    GetTextExtentPoint32A(hdc, text, (int)strlen(text), &size);
    return size.cx;
}

const PXColorRGBF axisColors[4] = 
{
    {1.0f, 0.0f, 0.0f}, // X
    {0.0f, 1.0f, 0.0f}, // Y
    {0.0f, 0.0f, 1.0f}, // Z
    {0.5f, 0.0f, 1.0f}  // W
};
const char axisLetters[4] =
{
    'X',
    'Y',
    'Z',
    'W'
};
const PXI8U componentCount = sizeof(axisLetters) / sizeof(char);
const char unitText[] = "cm";
const char title[] = "Position";

void PXAPI PXTransformViewEntryDraw(PXTransformView PXREF pxTransformView, PXDrawInfo PXREF pxDrawInfo, const PXSize index)
{
    PXWindow* pxWindow = pxTransformView->WindowBase;
    PXText pxText;

    char buf[32];
    float axisBoxWidth = 25.0f;
    float rowHeight = 20.0f;
    float fieldWidth = 175.0f;

    PXDrawInfo old = *pxDrawInfo;  

    // Axis colored box
    //pxDrawInfo->RectangleXYWH.Width = axisBoxWidth;
    //pxDrawInfo->RectangleXYWH.Height = rowHeight;
    //pxDrawInfo->Color = axisColors[index];
    PXWindowDrawRectangle(pxWindow, pxDrawInfo);


    // Axis Letter
    pxDrawInfo->Text = &pxText;
    pxDrawInfo->Color.Red = 0;
    pxDrawInfo->Color.Green = 0;
    pxDrawInfo->Color.Blue = 0;  
    //pxDrawInfo->RectangleXYWH.X += 5;
    //pxDrawInfo->RectangleXYWH.Y += 5;
    PXTextFromAdressA(&pxText, buf, 0, sizeof(buf));
    PXTextPrint(&pxText, "%c", axisLetters[index]);
    PXWindowDrawText(pxWindow, pxDrawInfo);
    // pxDrawInfo->RectangleXYWH.X -= 5;
    //pxDrawInfo->RectangleXYWH.Y -= 5;


   // pxDrawInfo->RectangleXYWH.X += axisBoxWidth;
    //pxDrawInfo->RectangleXYWH.Width = fieldWidth;
    pxDrawInfo->Color.Red = 0.15f;
    pxDrawInfo->Color.Green = 0.15f;
    pxDrawInfo->Color.Blue = 0.15f;
    PXWindowDrawRectangle(pxWindow, pxDrawInfo);

#if 1

    // Value
    //pxDrawInfo->RectangleXYWH.X += 5;
    //pxDrawInfo->RectangleXYWH.Y += 5;
    pxDrawInfo->Color.Red = 0;
    pxDrawInfo->Color.Green = 0;
    pxDrawInfo->Color.Blue = 0;
    PXTextFromAdressA(&pxText, buf, 0, sizeof(buf));

    if(pxTransformView->Position)
    {
        PXTextPrint(&pxText, "%6.2f", pxTransformView->Position->Data4[index]);
    }
    else
    {
        PXTextPrint(&pxText, "---.--");
    }


    PXWindowDrawText(pxWindow, pxDrawInfo);
#endif

    *pxDrawInfo = old;    
}

PXResult PXAPI PXTransformViewDraw(PXTransformView PXREF pxTransformView, PXDrawInfo PXREF pxDrawInfo)
{
    PXWindow* pxWindow = pxTransformView->WindowBase;
    PXVector3F32* values = pxTransformView->Position;
    PXGUITheme* theme = PXGUIThemeGet();
    HDC hdc = PXWindowDCGet(pxWindow);
    PXText pxText;  

    SelectObject(hdc, theme->FontTitle->FontHandle);


#if 0 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(90, pxDrawInfo->AspectRatio, 0, 10000);
    gluOrtho2D
    (
        pxDrawInfo->RectangleXYWH.X,
        pxDrawInfo->RectangleXYWH.Y + pxDrawInfo->RectangleXYWH.Height,
        pxDrawInfo->RectangleXYWH.X + pxDrawInfo->RectangleXYWH.Width,
        pxDrawInfo->RectangleXYWH.Y
    );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif


   // glViewport(0,0, pxDrawInfo->RectangleXYWH.Width, pxDrawInfo->RectangleXYWH.Height);
  //  glClearColor(0.2, 0.2, 0.4, 1.0);
   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    pxDrawInfo->Text = &pxText;
    // pxDrawInfo->RectangleXYWH.Width = 150;
    // pxDrawInfo->RectangleXYWH.Height = 20;

    PXTextFromAdressA(&pxText, title, sizeof(title), sizeof(title));
    PXWindowDrawText(pxWindow, pxDrawInfo);

    PXTextDrawInfoRowNext(pxDrawInfo);

#if 0
    pxDrawInfo->RectangleXYWH.X += sizeof(title);
    PXTextFromAdressA(&pxText, unitText, sizeof(unitText), sizeof(unitText));
    PXWindowDrawText(pxWindow, pxDrawInfo);
    pxDrawInfo->RectangleXYWH.X = 0.0;
#endif

    // pxDrawInfo->RectangleXYWH.Y += 5;

    for(PXI8U i = 0; i < componentCount; i++) // componentCount
    {
        PXTransformViewEntryDraw(pxTransformView, pxDrawInfo, i);
        PXTextDrawInfoRowNext(pxDrawInfo);
    }

    return PXActionRefusedNotImplemented;
}
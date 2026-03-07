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

PXResult PXAPI PXTransformViewRegisterToECS()
{
    PXECSRegister(&PXTransformViewInfoStatic, &PXTransformViewInfoDynamic);

    return PXResultOK;
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

PXResult PXAPI PXTransformViewDraw(PXTransformView PXREF pxTransformView, PXDrawInfo PXREF pxDrawInfo)
{
    PXWindow* pxWindow = pxTransformView->WindowBase;
    PXVector3F32* values = pxTransformView->Position;
    PXGUITheme* theme = PXGUIThemeGet();
    HDC hdc = PXWindowDCGet(pxWindow);
    PXText pxText;  

    float axisBoxWidth = 25.0f;
    float rowHeight = 20.0f;
    float fieldWidth = 150.0f;

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




    PXTextDrawInfo pxTextDrawInfo;
    PXClear(PXTextDrawInfo, &pxTextDrawInfo);
    pxTextDrawInfo.WindowDrawInfo = pxDrawInfo;
    pxTextDrawInfo.Text = &pxText;
    //pxTextDrawInfo.X = pxDrawInfo->RectangleXYWH.X;
    //pxTextDrawInfo.Y = pxDrawInfo->RectangleXYWH.Height;
    //pxTextDrawInfo.Size = 200;
    //pxTextDrawInfo.OffsetX = 0.2;

    //pxTextDrawInfo.Y = 0;

    pxTextDrawInfo.Position.Y -= rowHeight;

    PXTextFromAdressA(&pxText, title, sizeof(title), sizeof(title));
    PXWindowDrawText(pxWindow, &pxTextDrawInfo);

    //pxTextDrawInfo.X += 12;
    PXTextFromAdressA(&pxText, unitText, sizeof(unitText), sizeof(unitText));
    PXWindowDrawText(pxWindow, &pxTextDrawInfo);

    // Row 2: Axis fields
    float axisColors[4][3] = {
        {1,0,0},   // X
        {0,1,0},   // Y
        {0,0,1},   // Z
        {0.5f,0,1} // W
    };


   // pxTextDrawInfo.Y -= 2.0;
    pxTextDrawInfo.Position.X = pxDrawInfo->RectangleXYWH.X;

    for(int i = 0; i < componentCount; i++) // componentCount
    {
        char buf[32];

        // Axis colored box
        DrawRect
        (
            pxWindow,
            pxDrawInfo,
            pxTextDrawInfo.Position.X,
            pxTextDrawInfo.Position.Y,
            axisBoxWidth,
            rowHeight,
            axisColors[i][0],
            axisColors[i][1],
            axisColors[i][2]
        );
        DrawRect
        (
            pxWindow,
            pxDrawInfo,
            pxTextDrawInfo.Position.X + axisBoxWidth,
            pxTextDrawInfo.Position.Y,
            fieldWidth,
            rowHeight,
            0.15f,
            0.15f,
            0.15f
        );

        //pxTextDrawInfo.Size = 2;

#if 1

       // pxTextDrawInfo.Y = pxDrawInfo->RectangleXYWH.Height;

        float offsetVal = 25;

        pxTextDrawInfo.Position.X += 2;
        pxTextDrawInfo.Position.Y += 2;

        // Axis Letter
        PXTextFromAdressA(&pxText, buf, 0, sizeof(buf));
        PXTextPrint(&pxText, "%c", axisLetters[i]);
        PXWindowDrawText(pxWindow, &pxTextDrawInfo);

        pxTextDrawInfo.Position.X -= 2;
   



        pxTextDrawInfo.Position.X += offsetVal;

        // Value
        PXTextFromAdressA(&pxText, buf, 0, sizeof(buf));
        PXTextPrint(&pxText, "%6.2f", values->Data[i]);
        PXWindowDrawText(pxWindow, &pxTextDrawInfo);

        pxTextDrawInfo.Position.X -= offsetVal;
        pxTextDrawInfo.Position.Y -= 2;
#endif

        //pxTextDrawInfo.Y = pxDrawInfo->RectangleXYWH.Y + 20;

        pxTextDrawInfo.Position.X = pxDrawInfo->RectangleXYWH.X;
        pxTextDrawInfo.Position.Y -= rowHeight;

        //pxTextDrawInfo.X += fieldWidth;
    }


    return PXActionRefusedNotImplemented;
}
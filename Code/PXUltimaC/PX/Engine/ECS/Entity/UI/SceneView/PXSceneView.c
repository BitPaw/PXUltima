#include "PXSceneView.h"

#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment(lib, "glu32.lib")


const char PXSceneViewText[] = "SceneView";
const PXI8U PXSceneViewTextLength = sizeof(PXSceneViewText);
const PXECSRegisterInfoStatic PXSceneRegisterInfoStatic =
{
    {sizeof(PXSceneViewText), sizeof(PXSceneViewText), PXSceneViewText, TextFormatASCII},
    sizeof(PXSceneView),
    __alignof(PXSceneView),
    PXECSTypeEntity,
    PXFontCreate
};
PXECSRegisterInfoDynamic PXSceneRegisterInfoDynamic;


/* Window size (update these in your reshape callback) */
static int gWindowWidth = 800;
static int gWindowHeight = 600;

/* Simple demo rotation */
static float gAngle = 0.0f;

/* Helper: draw a filled rectangle in screen coordinates */
static void DrawFilledRect(float x0, float y0, float x1, float y1,
                           float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x0, y0);
    glVertex2f(x1, y0);
    glVertex2f(x1, y1);
    glVertex2f(x0, y1);
    glEnd();
}

/* Helper: draw button rectangle + very simple “label” (just colored bar) */
static void DrawButton(float x, float y, float w, float h,
                       float r, float g, float b)
{
    /* Button background */
    DrawFilledRect(x, y, x + w, y + h, r, g, b);

    /* Simple highlight border */
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

/* Helper: draw a simple 3D cube in the scene view */
static void Draw3DScene(int x, int y, int w, int h)
{
    /* Set viewport to the scene region */
    glViewport(x, y, w, h);

    /* Clear only the depth for this region (color already cleared for whole screen) */
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, w, h);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    /* Set up perspective projection for the scene */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,   /* eye */
              0.0, 0.0, 0.0,   /* center */
              0.0, 1.0, 0.0);  /* up */

    glEnable(GL_DEPTH_TEST);

    /* Simple rotating cube */
    glRotatef(gAngle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
    /* Front (red) */
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    /* Back (green) */
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    /* Left (blue) */
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    /* Right (yellow) */
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    /* Top (cyan) */
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    /* Bottom (magenta) */
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glEnd();

    glDisable(GL_DEPTH_TEST);
}

/* Typical GLUT reshape callback to keep sizes updated */
void Reshape(int w, int h)
{
    gWindowWidth = w;
    gWindowHeight = (h == 0) ? 1 : h;
}

/* Basic idle callback for animation */
void Idle(void)
{
    gAngle += 0.2f;
    if(gAngle >= 360.0f)
        gAngle -= 360.0f;
}

PXResult PXAPI PXSceneViewCreate(PXSceneView** pxSceneViewREF, PXSceneViewCreateInfo PXREF pxSceneViewCreateInfo)
{
    PXSceneView* pxSceneView = PXNull; 

    pxSceneViewCreateInfo->Info.Info.Static = &PXSceneRegisterInfoStatic;
    pxSceneViewCreateInfo->Info.Info.Dynamic = &PXSceneRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxSceneViewREF, pxSceneViewCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxSceneView = *pxSceneViewREF; 
    pxSceneView->CameraTarget = pxSceneViewCreateInfo->CameraTarget;

    return PXResultOK;
}

PXResult PXAPI PXSceneViewDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
    const float topBarHeight = 40.0f;
    const float bottomBarHeight = 30.0f;

    int width = pxDrawInfo->RectangleXYWH.Width;
    int height = pxDrawInfo->RectangleXYWH.Height;

    Reshape(width, height);

    Idle();

    /* ----------------- UI LAYERS (orthographic) ----------------- */

    /* Top bar background */
    DrawFilledRect(0.0f, height - topBarHeight,
                   (float)width, (float)height,
                   0.15f, 0.15f, 0.18f);

    /* Bottom bar background (for text) */
    DrawFilledRect(0.0f, 0.0f,
                   (float)width, bottomBarHeight,
                   0.12f, 0.12f, 0.14f);

    /* Top bar buttons */
    float btnW = 60.0f;
    float btnH = topBarHeight - 05.0f;
    float x = 10.0f;
    float y = height - topBarHeight + 5.0f;

    /* Play */
    DrawButton(x, y, btnW, btnH, 0.0f, 0.6f, 0.0f);
    x += btnW + 10.0f;

    /* Pause */
    DrawButton(x, y, btnW, btnH, 0.6f, 0.6f, 0.0f);
    x += btnW + 10.0f;

    /* Stop */
    DrawButton(x, y, btnW, btnH, 0.6f, 0.0f, 0.0f);
    x += btnW + 10.0f;

    /* Additional buttons (e.g., camera, gizmo, etc.) */
    DrawButton(x, y, btnW, btnH, 0.3f, 0.3f, 0.8f);
    x += btnW + 10.0f;
    DrawButton(x, y, btnW, btnH, 0.3f, 0.8f, 0.8f);

    /* Optional: very crude “text” indication in bottom bar
       (proper text needs bitmap fonts or your own font system) */
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_LINES);
    glVertex2f(10.0f, bottomBarHeight / 2.0f);
    glVertex2f(200.0f, bottomBarHeight / 2.0f);
    glEnd();

    /* ------------- 3D SCENE VIEW AREA ---------------- */

    /* Scene view rectangle (in window space) */
    int sceneX = 0;
    int sceneY = (int)bottomBarHeight;
    int sceneW = width;
    int sceneH = height - (int)topBarHeight - (int)bottomBarHeight;

    Draw3DScene(sceneX, sceneY, sceneW, sceneH);


    return PXResultOK;
}
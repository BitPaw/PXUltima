#include "PXSpaceGrid.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include <math.h>

const char PXSpaceGridName[] = "SpaceGrid";
const PXI8U PXSpaceGridNameLength = sizeof(PXSpaceGridName);
const PXECSRegisterInfoStatic PXSpaceGridRegisterInfoStatic =
{
    {sizeof(PXSpaceGridName), sizeof(PXSpaceGridName), PXSpaceGridName, TextFormatASCII},
    sizeof(PXSpaceGrid),
    __alignof(PXSpaceGrid),
    PXECSTypeEntity,
    PXSpaceGridCreate,
    PXNull,
    PXSpaceGridDraw
};
PXECSRegisterInfoDynamic PXSpaceGridRegisterInfoDynamic;

PXResult PXAPI PXSpaceGridRegisterToECS()
{
    PXECSRegister(&PXSpaceGridRegisterInfoStatic, &PXSpaceGridRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXSpaceGridCreate(PXSpaceGrid** pxSpaceGridREF, PXSpaceGridCreateInfo PXREF pxSpaceGridCreateInfo)
{   
    PXSpaceGrid* pxSpaceGrid = PXNull;

    pxSpaceGridCreateInfo->Info.Static = &PXSpaceGridRegisterInfoStatic;
    pxSpaceGridCreateInfo->Info.Dynamic = &PXSpaceGridRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxSpaceGridREF, pxSpaceGridCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxSpaceGrid = *pxSpaceGridREF;

    if(pxSpaceGridCreateInfo->WindowTarget)
    {
        pxSpaceGrid->WindowBase = pxSpaceGridCreateInfo->WindowTarget;
    }
    else
    {
        pxSpaceGridCreateInfo->Window.EventList.CallBackOwner = pxSpaceGrid;
        pxSpaceGridCreateInfo->Window.EventList.CallBackDraw = PXSpaceGridDraw;

        PXWindowCreate(&pxSpaceGrid->WindowBase, &pxSpaceGridCreateInfo->Window);
    }

    PXCameraCreate(&pxSpaceGrid->CameraView, &pxSpaceGridCreateInfo->Camera);

    return PXResultOK;
}

PXResult PXAPI PXSpaceGridDraw(PXSpaceGrid PXREF pxSpaceGrid, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXWindow PXREF pxWindowBase = pxSpaceGrid->WindowBase;
    PXCamera PXREF pxCamera = pxSpaceGrid->CameraView;

    float camX = 0;
    float camZ = 0;

    const float GRID_SIZE = 200.0f; // half-extent (so 400x400 total)
    const float GRID_STEP = 1.0f;   // minor grid spacing
    const int   MAJOR_EVERY = 10;   // major line every 10 units

    glClearColor(0.6, 0.6, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspectRatio = pxWindowDrawInfo->RectangleXYWH.Height / pxWindowDrawInfo->RectangleXYWH.Width;    

    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluPerspective(60.0, aspectRatio, 0.1, 5000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // Camera transform
    glTranslatef(0.0f, -5.0f, -10.0f);
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f); // tilt downward 
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f); // rotate around Y



    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.0f);

    glBegin(GL_LINES);

    for(float x = -GRID_SIZE; x <= GRID_SIZE; x += GRID_STEP)
    {
        int isMajor = ((int)x % MAJOR_EVERY == 0);

        float dist = fabsf(x - camX);
        float fade = 1.0f - (dist / GRID_SIZE);
        if(fade < 0.0f) fade = 0.0f;

        if(isMajor)
            glColor4f(0.25f, 0.25f, 0.25f, fade);
        else
            glColor4f(0.15f, 0.15f, 0.15f, fade);

        glVertex3f(x, 0.0f, -GRID_SIZE);
        glVertex3f(x, 0.0f, GRID_SIZE);
    }

    for(float z = -GRID_SIZE; z <= GRID_SIZE; z += GRID_STEP)
    {
        int isMajor = ((int)z % MAJOR_EVERY == 0);

        float dist = fabsf(z - camZ);
        float fade = 1.0f - (dist / GRID_SIZE);
        if(fade < 0.0f) fade = 0.0f;

        if(isMajor)
            glColor4f(0.25f, 0.25f, 0.25f, fade);
        else
            glColor4f(0.15f, 0.15f, 0.15f, fade);

        glVertex3f(-GRID_SIZE, 0.0f, z);
        glVertex3f(GRID_SIZE, 0.0f, z);
    }

    glEnd();

    glEnable(GL_LIGHTING);

    return PXResultOK;
}

#include "PXSpaceGrid.h"

#include <gl/GL.h>
#include <gl/GLU.h>

#include <math.h>

#include <PX/Engine/ECS/Entity/Gizmo/PXGizmo.h>
#include <PX/Engine/ECS/Entity/Tesseract/PXTesseract.h>

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

    pxSpaceGrid->CameraView->Position.Y += 10;

    return PXResultOK;
}

#include <PX/Engine/ECS/Entity/Camera/PXCamera.h>
#include <PX/Math/PXMath.h>

PXBool doFog = PXFalse; 

PXResult PXAPI PXSpaceGridDraw(PXSpaceGrid PXREF pxSpaceGrid, PXDrawInfo PXREF pxDrawInfo)
{
    PXWindow PXREF pxWindowBase = pxSpaceGrid->WindowBase;
    PXCamera PXREF pxCamera = pxSpaceGrid->CameraView;

    pxDrawInfo->Camera = pxCamera;

    const float GRID_SIZE = 200.0f; // half-extent (so 400x400 total)
    const float GRID_STEP = 0.5f;   // minor grid spacing
    const int   MAJOR_EVERY = 10;   // major line every 10 units

    PXColorRGBF ClearColor = { 0.2, 0.2, 0.4 };

    glClearColor(ClearColor.Red, ClearColor.Green, ClearColor.Blue, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    PXCameraGLFPP(pxCamera, pxDrawInfo->AspectRatio);


    
    

    PXVector3F32 forward;
    PXVector3F32 right;
    PXVector3F32 up;

    PXCameraForward(pxCamera, &forward);
    PXCameraRight(pxCamera, &right);
    PXCameraUp(pxCamera, &up);


    glBegin(GL_LINES);
    glColor4f(0.8f, 0.00f, 0.0f, 1.0f);
    glVertex3f(right.X, right.Y, right.Z);
    glVertex3f(right.X+3, right.Y, right.Z);

    glColor4f(0.00f, 0.8f, 0.0f, 1.0f);
    glVertex3f(up.X, up.Y, up.Z);
    glVertex3f(up.X, up.Y+3, up.Z);

    glColor4f(0.0f, 0.0f, 0.8f, 1.0f);
    glVertex3f(forward.X, forward.Y, right.Z);
    glVertex3f(forward.X, forward.Y, right.Z+3);
    glEnd();


    if(doFog)
    {
        glEnable(GL_FOG);
        glFogf(GL_FOG_MODE, GL_LINEAR);
        glFogf(GL_FOG_START, 5.0f);
        glFogf(GL_FOG_END, 100000.0f);
        GLfloat fogColor[] = { ClearColor.Red, ClearColor.Green, ClearColor.Blue, 1.0f };
        glFogfv(GL_FOG_COLOR, fogColor);
    }




    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.5f);

    glBegin(GL_LINES);

    PXVector3F32 cameraPos = { 0, 0, 0};

    if(pxCamera)
    {
        cameraPos.X = pxCamera->Position.X;
        cameraPos.Z = pxCamera->Position.Z;
    }

    for(float x = -GRID_SIZE; x <= GRID_SIZE; x += GRID_STEP)
    {
        int isMajor = ((int)x % MAJOR_EVERY == 0);

        float dist = fabsf(x - cameraPos.X);
        float fade = 1.0f - (dist / GRID_SIZE);
        if(fade < 0.0f)
            fade = 0.0f;

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

        float dist = fabsf(z - cameraPos.Z);
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

    //glEnable(GL_LIGHTING);

    if(doFog)
    {
        glDisable(GL_FOG);
    }


    PXSceneDraw(pxSpaceGrid->Scene, pxDrawInfo);





    PXCameraGLFPP(pxCamera, pxDrawInfo->AspectRatio);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    PXGizmoDraw(PXNull, pxDrawInfo);



    PXTesseractDraw(PXNull, pxDrawInfo);


    PXCameraGLFPP(pxCamera, pxDrawInfo->AspectRatio);

    PXCameraDraw(pxSpaceGrid->CameraPlayer, pxDrawInfo);



 


  



    char buffer[64];
    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));

    if(pxCamera)
    {
        PXTextPrint
        (
            &pxText,
            "CAM:%4.2f, %4.2f, %4.2f\nTest",
            pxCamera->Position.X,
            pxCamera->Position.Y,
            pxCamera->Position.Z
        );
    }
    else
    {
        PXTextPrint
        (
            &pxText,
            "CAM:%4.2s, %4.2s, %4.2s\nTest",
            "---",
            "---",
            "---"
        );
    }





    PXTextDrawInfo pxTextDrawInfo;
    PXClear(PXTextDrawInfo, &pxTextDrawInfo);
    pxTextDrawInfo.WindowDrawInfo = pxDrawInfo;
    pxTextDrawInfo.Text = &pxText;
    pxTextDrawInfo.Behaviour = PXWindowAllignLeft | PXWindowAllignTop;
    pxTextDrawInfo.Position.X = 0;
    pxTextDrawInfo.Position.Y = 0;


    pxTextDrawInfo.Color.Red = 0.3;
    pxTextDrawInfo.Color.Green = 0.3;
    pxTextDrawInfo.Color.Blue = 0.3;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    PXWindowDrawText(pxWindowBase, &pxTextDrawInfo);


    
    pxTextDrawInfo.Color.Red = 0.0;
    pxTextDrawInfo.Color.Green = 0.0;
    pxTextDrawInfo.Color.Blue = 0.0;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    PXWindowDrawText(pxWindowBase, &pxTextDrawInfo);





    return PXResultOK;
}

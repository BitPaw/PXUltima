#pragma once

#ifndef PXSpaceGridIncluded
#define PXSpaceGridIncluded

#include <PX/Type/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Type/PXWindow.h>
#include <PX/Type/PXCamera.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXScene.h>

typedef struct PXSpaceGrid_
{
    PXECSInfo Info;

    PXWindow* WindowBase;

    //PXCamera* CameraView;
    //PXCamera* CameraPlayer;

    PXScene* Scene;
}
PXSpaceGrid;

typedef struct PXSpaceGridCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;

    PXWindow* WindowTarget;
}
PXSpaceGridCreateInfo;

PXPublic void PXAPI PXSpaceGridRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXSpaceGridCreate(PXSpaceGrid** pxSpaceGrid, PXSpaceGridCreateInfo PXREF pxSpaceGridCreateInfo);
PXPublic PXResult PXAPI PXSpaceGridDraw(PXSpaceGrid PXREF pxSpaceGrid, PXDrawInfo PXREF pxDrawInfo);

#endif
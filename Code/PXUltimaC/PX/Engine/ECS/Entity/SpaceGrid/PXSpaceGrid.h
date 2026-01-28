#pragma once

#ifndef PXSpaceGridIncluded
#define PXSpaceGridIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Engine/ECS/Entity/Camera/PXCamera.h>
#include <PX/Engine/ECS/Entity/Model/PXModel.h>

typedef struct PXSpaceGrid_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
    PXCamera* CameraView;

    PXCamera* CameraPlayer;

    PXModel* ModelRender;
}
PXSpaceGrid;

typedef struct PXSpaceGridCreateInfo_
{
    PXECSCreateInfo Info;

    PXCameraCreateInfo Camera;
    PXWindowCreateInfo Window;

    PXWindow* WindowTarget;
}
PXSpaceGridCreateInfo;

PXPublic PXResult PXAPI PXSpaceGridRegisterToECS();

PXPublic PXResult PXAPI PXSpaceGridCreate(PXSpaceGrid** pxSpaceGrid, PXSpaceGridCreateInfo PXREF pxSpaceGridCreateInfo);
PXPublic PXResult PXAPI PXSpaceGridDraw(PXSpaceGrid PXREF pxSpaceGrid, PXDrawInfo PXREF pxDrawInfo);

#endif
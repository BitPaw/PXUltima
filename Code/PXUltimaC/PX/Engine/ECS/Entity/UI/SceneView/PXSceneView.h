#pragma once

#ifndef PXSceneViewIncluded
#define PXSceneViewIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Engine/ECS/Entity/Camera/PXCamera.h>

typedef struct PXSceneView_
{
    PXECSInfo Info;

    PXCamera* CameraTarget;
}
PXSceneView;

typedef struct PXSceneViewCreateInfo_
{
    PXWindowCreateInfo Info;

    PXCamera* CameraTarget;
}
PXSceneViewCreateInfo;

PXPublic PXResult PXAPI PXSceneViewCreate(PXSceneView** pxSceneViewREF, PXSceneViewCreateInfo PXREF pxSceneViewCreateInfo);
PXPublic PXResult PXAPI PXSceneViewDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
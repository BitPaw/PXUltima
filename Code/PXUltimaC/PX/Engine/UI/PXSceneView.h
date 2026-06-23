#pragma once

#ifndef PXSceneViewIncluded
#define PXSceneViewIncluded

#include <PX/Type/PXWindow.h>
#include <PX/Type/PXCamera.h>

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
PXPublic PXResult PXAPI PXSceneViewDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
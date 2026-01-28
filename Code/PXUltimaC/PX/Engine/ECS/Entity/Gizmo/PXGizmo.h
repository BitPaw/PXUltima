#pragma once

#ifndef PXGizmoIncluded
#define PXGizmoIncluded

#include <PX/Engine/ECS/PXECS.h>


typedef struct PXGizmo_
{
    PXECSInfo Info; 

    int x;
}
PXGizmo;

typedef struct PXGizmoCreateInfo_
{
    PXECSCreateInfo Info;
}
PXGizmoCreateInfo;

PXPublic PXResult PXAPI PXGizmoRegisterToECS(void);

PXPublic PXResult PXAPI PXGizmoCreate(PXGizmo** pxGizmoREF, PXGizmoCreateInfo PXREF pxGizmoCreateInfo);
PXPublic PXResult PXAPI PXGizmoRelease(PXGizmo PXREF pxGizmo);
PXPublic PXResult PXAPI PXGizmoDraw(PXGizmo PXREF pxGizmo, struct PXDrawInfo_ PXREF pxDrawInfo);

#endif
#pragma once

#ifndef PXECSEntityInfoIncluded
#define PXECSEntityInfoIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXECSEntityInfo_
{
    int x;
}
PXECSEntityInfo;

PXPublic PXResult PXAPI PXECSEntityInfoDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
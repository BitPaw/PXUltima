#pragma once

#ifndef PXECSEntityListIncluded
#define PXECSEntityListIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXECSEntityList_
{
    int x;
}
PXECSEntityList;

PXPublic PXResult PXAPI PXECSEntityListDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
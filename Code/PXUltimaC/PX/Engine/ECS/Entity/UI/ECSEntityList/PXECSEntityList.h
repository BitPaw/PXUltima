#pragma once

#ifndef PXECSEntityListIncluded
#define PXECSEntityListIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXECSEntityList_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
}
PXECSEntityList;

typedef struct PXECSEntityListCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;
}
PXECSEntityListCreateInfo;

PXPublic PXResult PXAPI PXECSEntityListRegisterToECS();

PXPublic PXResult PXAPI PXECSEntityListCreate(PXECSEntityList** pxECSEntityListREF, PXECSEntityListCreateInfo PXREF pxECSEntityListCreateInfo);
PXPublic PXResult PXAPI PXECSEntityListDraw(PXECSEntityList PXREF pxECSEntityList, PXDrawInfo PXREF pxDrawInfo);

#endif
#pragma once

#ifndef PXECSEntityInfoIncluded
#define PXECSEntityInfoIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXECSEntityInfo_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
}
PXECSEntityInfo;

typedef struct PXECSEntityInfoCreateInfo_
{
    PXECSCreateInfo Info;
}
PXECSEntityInfoCreateInfo;

PXPublic PXResult PXAPI PXECSEntityInfoRegisterToECS();

PXPublic PXResult PXAPI PXECSEntityInfoCreate(PXECSEntityInfo** pxECSEntityInfo, PXECSEntityInfoCreateInfo PXREF pxECSEntityInfoCreateInfo);
PXPublic PXResult PXAPI PXECSEntityInfoDraw(PXECSEntityInfo PXREF pxECSEntityInfo, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
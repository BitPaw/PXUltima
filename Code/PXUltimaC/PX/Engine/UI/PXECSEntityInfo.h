#pragma once

#ifndef PXECSEntityInfoIncluded
#define PXECSEntityInfoIncluded

#include <PX/Type/PXWindow.h>

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

PXPublic void PXAPI PXECSEntityInfoRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXECSEntityInfoCreate(PXECSEntityInfo** pxECSEntityInfo, PXECSEntityInfoCreateInfo PXREF pxECSEntityInfoCreateInfo);
PXPublic PXResult PXAPI PXECSEntityInfoDraw(PXECSEntityInfo PXREF pxECSEntityInfo, PXDrawInfo PXREF pxDrawInfo);

#endif
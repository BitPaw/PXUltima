#pragma once

#ifndef PXFooterIncluded
#define PXFooterIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXFooter_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
}
PXFooter;

typedef struct PXFooterCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;
}
PXFooterCreateInfo;

PXPublic PXResult PXAPI PXFooterRegisterToECS();

PXPublic PXResult PXAPI PXFooterCreate(PXFooter** pxFooterREF, PXFooterCreateInfo PXREF pxFooterCreateInfo);
PXPublic PXResult PXAPI PXFooterDraw(PXFooter PXREF pxFooter, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
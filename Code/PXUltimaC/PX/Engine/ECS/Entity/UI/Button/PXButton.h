#pragma once

#ifndef PXButtonIncluded
#define PXButtonIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Engine/ECS/Entity/UI/Label/PXLabel.h>

typedef struct PXButton_
{
    PXWindow* WindowBase;
    PXLabel* ContentText;
    PXFont* TextFont;
}
PXButton;

typedef struct PXButtonCreateInfo_
{
    PXWindowCreateInfo WindowInfo;
    PXLabelCreateInfo TextInfo;
}
PXButtonCreateInfo;

PXPublic PXResult PXAPI PXButtonDraw(PXButton PXREF pxButton, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPublic PXResult PXAPI PXButtonCreate(PXButton** pxButtonREF, PXButtonCreateInfo PXREF pxButtonCreateInfo);

#endif
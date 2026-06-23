#pragma once

#ifndef PXButtonIncluded
#define PXButtonIncluded

#include <PX/Type/PXWindow.h>
#include <PX/Engine/UI/PXLabel.h>

typedef struct PXButton_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
    PXLabel* ContentText;
    PXFont* TextFont;
}
PXButton;

typedef struct PXButtonCreateInfo_
{
    PXWindowCreateInfo Window;
    PXLabelCreateInfo Text;
}
PXButtonCreateInfo;

PXPublic PXResult PXAPI PXButtonDraw(PXButton PXREF pxButton, PXDrawInfo PXREF pxDrawInfo);
PXPublic PXResult PXAPI PXButtonCreate(PXButton** pxButtonREF, PXButtonCreateInfo PXREF pxButtonCreateInfo);

#endif
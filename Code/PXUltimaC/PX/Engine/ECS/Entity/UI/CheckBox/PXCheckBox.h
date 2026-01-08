#pragma once

#ifndef PXCheckBoxIncluded
#define PXCheckBoxIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXCheckBox_
{
    int x;
}
PXCheckBox;

typedef struct PXCheckBoxCreateInfo_
{
    PXWindowCreateInfo Info;

    char** DataList;
    PXSize DataListAmount;
}
PXCheckBoxCreateInfo;

PXPublic PXResult PXAPI PXCheckBoxDraw(PXCheckBox PXREF pxCheckBox, PXCheckBoxCreateInfo PXREF pxCheckBoxCreateInfo);

#endif
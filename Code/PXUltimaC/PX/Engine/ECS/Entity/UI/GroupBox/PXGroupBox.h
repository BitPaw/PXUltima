#pragma once

#ifndef PXGroupBoxIncluded
#define PXGroupBoxIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXGroupBox_
{
    PXWindow* WindowBase;
}
PXGroupBox;

typedef struct PXGroupBoxCreateInfo_
{
    PXWindowCreateInfo WindowInfo;
}
PXGroupBoxCreateInfo;

PXPublic PXResult PXAPI PXGroupBoxDraw(PXGroupBox PXREF pxGroupBox, PXGroupBoxCreateInfo PXREF pxGroupBoxCreateInfo);
PXPublic PXResult PXAPI PXGroupBoxCreate(PXGroupBox** pxGroupBox, PXGroupBoxCreateInfo PXREF pxGroupBoxCreateInfo);

#endif
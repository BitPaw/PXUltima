#pragma once

#ifndef PXFooterIncluded
#define PXFooterIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXFooter_
{
    int x;
}
PXFooter;

PXPublic PXResult PXAPI PXFooterDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
#pragma once

#ifndef PXHeaderIncluded
#define PXHeaderIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXHeader_
{
    int x;
}
PXHeader;

PXPublic PXResult PXAPI PXHeaderDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
#pragma once

#ifndef PXGraphTimeIncluded
#define PXGraphTimeIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXGraphTime_
{
    int x;
}
PXGraphTime;

PXPublic PXResult PXAPI PXGraphTimeDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
#pragma once

#ifndef PXGraphTimeIncluded
#define PXGraphTimeIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXGraphTime_
{
    int x;
}
PXGraphTime;

PXPublic PXResult PXAPI PXGraphTimeDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
#pragma once

#ifndef PXHeaderIncluded
#define PXHeaderIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXHeader_
{
    int x;
}
PXHeader;

PXPublic PXResult PXAPI PXHeaderDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
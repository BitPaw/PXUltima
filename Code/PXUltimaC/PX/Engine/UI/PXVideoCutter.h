#pragma once

#ifndef PXVideoCutterIncluded
#define PXVideoCutterIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXVideoCutter_
{
    int x;
}
PXVideoCutter;

PXPublic PXResult PXAPI PXVideoCutterDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
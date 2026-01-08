#pragma once

#ifndef PXVideoCutterIncluded
#define PXVideoCutterIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXVideoCutter_
{
    int x;
}
PXVideoCutter;

PXPublic PXResult PXAPI PXVideoCutterDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
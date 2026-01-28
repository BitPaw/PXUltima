#pragma once

#ifndef PXHexEditIncluded
#define PXHexEditIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXHexEdit_
{
    int x;
}
PXHexEdit;

PXPublic PXResult PXAPI Draw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
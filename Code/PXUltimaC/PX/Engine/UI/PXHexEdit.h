#pragma once

#ifndef PXHexEditIncluded
#define PXHexEditIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXHexEdit_
{
    int x;
}
PXHexEdit;

PXPublic PXResult PXAPI Draw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
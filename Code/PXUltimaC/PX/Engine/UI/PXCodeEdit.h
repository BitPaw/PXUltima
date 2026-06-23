#pragma once

#ifndef PXCodeEditIncluded
#define PXCodeEditIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXCodeEdit_
{
    int x;
}
PXCodeEdit;

PXPrivate PXResult PXAPI PXCodeEditDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif
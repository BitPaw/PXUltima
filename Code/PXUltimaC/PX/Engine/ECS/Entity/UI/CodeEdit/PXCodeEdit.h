#pragma once

#ifndef PXCodeEditIncluded
#define PXCodeEditIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXCodeEdit_
{
    int x;
}
PXCodeEdit;

PXPrivate PXResult PXAPI PXCodeEditDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
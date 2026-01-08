#pragma once

#ifndef PXTransformViewIncluded
#define PXTransformViewIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXTransformView
{
    int x;
}
PXTransformView;

PXPublic PXResult PXAPI PXTransformViewDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif
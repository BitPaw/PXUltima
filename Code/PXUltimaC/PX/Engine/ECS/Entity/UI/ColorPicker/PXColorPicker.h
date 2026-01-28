#pragma once

#ifndef PXColorPickerIncluded
#define PXColorPickerIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>
#include <PX/Media/PXColor.h>

typedef struct PXColorCircleVertex_
{
    PXF32 X;
    PXF32 Y;
    PXI8U Red;
    PXI8U Green;
    PXI8U Blue;
}
PXColorCircleVertex;

typedef struct PXColorCircle_
{
    int StartX;
    int StartY;
    int Size;
    int Precision;
    PXColorCircleVertex* VertexList;

    // Triangle
    PXColorCircleVertex VertexListTriangle[3];

    PXColorHSV ColorSelected;
}
PXColorCircle;

typedef struct PXColorPicker_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
}
PXColorPicker;

typedef struct PXColorPickerCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;
}
PXColorPickerCreateInfo;

PXPublic PXResult PXAPI PXColorPickerRegisterToECS();

PXPublic PXResult PXAPI PXColorPickerDraw(PXColorPicker PXREF pxColorPicker, PXDrawInfo PXREF pxDrawInfo);
PXPublic PXResult PXAPI PXColorPickerCreate(PXColorPicker** pxColorPickerREF, PXColorPickerCreateInfo PXREF pxColorPickerCreateInfo);

#endif
#pragma once

#ifndef PXLabelIncluded
#define PXLabelIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXLabel_
{
    PXWindow* WindowBase;
}
PXLabel;

typedef struct PXLabelCreateInfo_
{
    PXWindowCreateInfo Info;

    char* Content;
    struct PXFont_* FontID;
    PXF32 Scale;
    PXUIElementAllign Allign;
}
PXLabelCreateInfo;

PXPrivate PXResult PXAPI PXLabelDraw(PXLabel PXREF pxLabel, PXLabelCreateInfo PXREF pxLabelCreateInfo);

#endif
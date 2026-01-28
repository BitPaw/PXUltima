#pragma once

#ifndef PXBrushIncluded
#define PXBrushIncluded

#include <PX/Media/PXType.h>
#include <PX/Media/PXColor.h>
#include <PX/Engine/ECS/PXECS.h>


#define PXGUIDrawModeFront (1<<0)
#define PXGUIDrawModeBack  (1<<1)

// if true, color is a struct, otherwise a pointer to that struct
#define PXWindowBrushBehaviourColorEmbeded  (1<<0)

// Color of GUI Element and tools to apply
typedef struct PXBrush_ PXBrush;

typedef struct PXBrush_
{
    PXECSInfo Info;

    union
    {
        HBRUSH BrushHandle;
    };

    union
    {
        PXColorRGBI8 ColorDate;
        PXColorRGBI8* ColorReference;
    };
}
PXBrush;

typedef struct PXBrushCreateInfo_
{
    PXECSCreateInfo Info;

    PXColorRGBI8 Color;
}
PXBrushCreateInfo;

PXPublic PXResult PXAPI PXBrushRegisterToECS();
PXPublic PXResult PXAPI PXBrushCreate(PXBrush** pxBrushRef, PXBrushCreateInfo PXREF pxBrushCreateInfo);
PXPublic PXResult PXAPI PXBrushColorSet(PXBrush PXREF pxBrush, const PXByte red, const PXByte green, const PXByte blue);

#endif
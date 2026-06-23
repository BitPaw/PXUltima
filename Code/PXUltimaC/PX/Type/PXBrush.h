#pragma once

#ifndef PXBrushIncluded
#define PXBrushIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXColor.h>
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
        #if OSWindows
        HBRUSH BrushHandle;
        #endif
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

PXPublic void PXAPI PXBrushRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXBrushCreate(PXBrush** pxBrushRef, PXBrushCreateInfo PXREF pxBrushCreateInfo);
PXPublic PXResult PXAPI PXBrushColorSet(PXBrush PXREF pxBrush, const PXByte red, const PXByte green, const PXByte blue);

#endif

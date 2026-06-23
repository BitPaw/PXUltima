#pragma once

#ifndef PXLightPointIncluded
#define PXLightPointIncluded
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXWindow.h>
#include <PX/Math/PXVector.h>


typedef struct PXLightPoint_
{
    PXECSInfo Info;

    PXVector3F32 Position;
    PXVector3F32 Color;
    PXF32 Intensity;
    PXF32 Radius;
}
PXLightPoint;

typedef struct PXLightPointCreateInfo_
{
    PXECSCreateInfo Info;
}
PXLightPointCreateInfo;


PXPublic void PXAPI PXLightPointRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);

PXPublic PXResult PXAPI PXLightPointCreate(PXLightPoint** pxLightPointREF, PXLightPointCreateInfo PXREF pxLightPointCreateInfo);
PXPublic PXResult PXAPI PXLightPointRelease(const PXLightPoint PXREF pxLightPoint);
PXPublic PXResult PXAPI PXLightPointDraw(const PXLightPoint PXREF pxLightPoint, struct PXWindowDrawInfo_ PXREF pxDrawInfo);

#endif
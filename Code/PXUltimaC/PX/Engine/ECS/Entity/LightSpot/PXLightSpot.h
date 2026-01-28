#pragma once

#ifndef PXLightSpotIncluded
#define PXLightSpotIncluded
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Math/PXVector.h>

typedef struct PXLightSpot_
{
    PXECSInfo Info;
    PXVector3F32 Position;
    PXVector3F32 Direction;
    PXVector3F32 Color;
    PXF32 Intensity;
    PXF32 InnerAngle; // radians
    PXF32 OuterAngle; // radians 
    PXF32 Range;
}
PXLightSpot;

typedef struct PXLightSpotCreateInfo_
{
    PXECSCreateInfo Info;
}
PXLightSpotCreateInfo;


PXPublic PXResult PXAPI PXLightSpotRegisterToECS(void);

PXPublic PXResult PXAPI PXLightSpotCreate(PXLightSpot** pxLightSpotREF, PXLightSpotCreateInfo PXREF pxLightSpotCreateInfo);
PXPublic PXResult PXAPI PXLightSpotRelease(const PXLightSpot PXREF pxLightSpot);
PXPublic PXResult PXAPI PXLightSpotDraw(const PXLightSpot PXREF pxLightSpot, struct PXWindowDrawInfo_ PXREF pxDrawInfo);

#endif
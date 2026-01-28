#pragma once

#ifndef PXLightDirectionalIncluded
#define PXLightDirectionalIncluded
#include <PX/Engine/ECS/PXECS.h>

typedef struct PXLightDirectional_
{
    PXECSInfo Info;
}
PXLightDirectional;

typedef struct PXLightDirectionalCreateInfo_
{
    PXECSCreateInfo Info;
}
PXLightDirectionalCreateInfo;


PXPublic PXResult PXAPI PXLightDirectionalRegisterToECS(void);

PXPublic PXResult PXAPI PXLightDirectionalCreate(PXLightDirectional** pxLightDirectionalREF, PXLightDirectionalCreateInfo PXREF pxLightDirectionalCreateInfo);
PXPublic PXResult PXAPI PXLightDirectionalRelease(const PXLightDirectional PXREF pxLightDirectional);
PXPublic PXResult PXAPI PXLightDirectionalDraw(const PXLightDirectional PXREF pxLightDirectional, struct PXDrawInfo_ PXREF pxDrawInfo);

#endif
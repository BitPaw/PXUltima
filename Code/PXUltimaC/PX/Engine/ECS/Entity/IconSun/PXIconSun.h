#pragma once

#ifndef PXIconSunIncluded
#define PXIconSunIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXIconSun_
{
    PXECSInfo Info;
}
PXIconSun;

typedef struct PXIconSunCreateInfo_
{
    PXECSCreateInfo Info;
}
PXIconSunCreateInfo;

PXPublic PXResult PXAPI PXIconSunRegisterToECS(void);
PXPublic PXResult PXAPI PXIconSunCreate(PXIconSun** pxIconSunREF, PXIconSunCreateInfo PXREF pxIconSunCreateInfo);
PXPublic PXResult PXAPI PXIconSunRelease(PXIconSun PXREF pxIconSun);
PXPublic PXResult PXAPI PXIconSunDraw(PXIconSun PXREF pxIconSun, PXDrawInfo PXREF pxDrawInfo);

#endif
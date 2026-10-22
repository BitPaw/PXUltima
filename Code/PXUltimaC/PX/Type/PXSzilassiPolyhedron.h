#pragma once

#ifndef PXSzilassiPolyhedronIncluded
#define PXSzilassiPolyhedronIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXSzilassiPolyhedron_
{
    PXECSInfo Info;
}
PXSzilassiPolyhedron;

typedef struct PXSzilassiPolyhedronCreateInfo_
{
    PXECSCreateInfo Info;
}
PXSzilassiPolyhedronCreateInfo;

PXPublic void PXAPI PXSzilassiPolyhedronRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSzilassiPolyhedronCreate(PXSzilassiPolyhedron** pxSzilassiPolyhedronREF, PXSzilassiPolyhedronCreateInfo PXREF pxSzilassiPolyhedronCreateInfo);
PXPublic PXResult PXAPI PXSzilassiPolyhedronRelease(PXSzilassiPolyhedron PXREF pxSzilassiPolyhedron);

#endif
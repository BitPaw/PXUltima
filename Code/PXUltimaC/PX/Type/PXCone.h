#pragma once

#ifndef PXConeIncluded
#define PXConeIncluded

#include <PX/Type/PXModel.h>

typedef struct PXConeCreateInfo_
{
    PXSize Segments;
    PXSize Radius;
    PXSize Height;
}
PXConeCreateInfo;

PXPublic PXResult PXAPI PXConeCreate(PXModel PXREF pxModel, PXConeCreateInfo PXREF pxConeCreateInfo);

#endif
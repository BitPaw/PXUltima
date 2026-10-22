#pragma once

#ifndef PXCylinderIncluded
#define PXCylinderIncluded

#include <PX/Type/PXModel.h>

typedef struct PXCylinderCreateInfo_
{
    int x;
}
PXCylinderCreateInfo;

PXPublic PXResult PXAPI PXCylinderCreate(PXModel PXREF pxModel, PXCylinderCreateInfo PXREF pxCylinderCreateInfo);

#endif
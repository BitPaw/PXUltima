#pragma once

#ifndef PXModelPoolIncluded
#define PXModelPoolIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include "PXModel.h"

typedef struct PXModelPool_ PXModelPool;

PXPublic PXResult PXAPI PXModelPoolRegisterToECS();
PXPublic PXResult PXAPI PXModelPoolCreate(PXModelPool** pxModelPoolREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXModelPoolRelease(PXModelPool PXREF pxModelPool);

PXPublic PXResult PXAPI PXModelPoolReserveAmount(PXModelPool PXREF pxModelPool, const PXSize amount);

PXPublic PXSize PXAPI PXModelPoolModelAmount(const PXModelPool PXREF pxModelPool);
PXPublic PXModel* PXAPI PXModelPoolModelGet(const PXModelPool PXREF pxModelPool, const PXSize index);
PXPublic PXModel* PXAPI PXModelPoolModelSet(PXModelPool PXREF pxModelPool, PXModel PXREF pxModel);

#endif
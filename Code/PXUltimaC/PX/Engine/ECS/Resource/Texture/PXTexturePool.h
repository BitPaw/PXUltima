#pragma once


#ifndef PXTexturePoolIncluded
#define PXTexturePoolIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/Texture/PXTextureSet.h>

// A collection of texture sets
typedef struct PXTexturePool_ PXTexturePool;

PXPublic PXResult PXAPI PXTexturePoolRegisterToECS();
PXPublic PXResult PXAPI PXTexturePoolCreate(PXTexturePool** pxTexturePoolREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXTexturePoolRelease(PXTexturePool PXREF pxTexturePool);

PXPublic PXSize PXAPI PXTexturePoolAmountGet(PXTexturePool PXREF pxTexturePool);
PXPublic PXResult PXAPI PXTexturePoolReserveAmount(PXTexturePool PXREF pxTexturePool, const PXSize amount);
PXPublic PXTextureSet* PXAPI PXTexturePoolSetGet(const PXTexturePool PXREF pxTexturePool, const PXSize index);
PXPublic PXTextureSet* PXAPI PXTexturePoolTextureGet(const PXTexturePool PXREF pxTexturePool, const PXSize indexTotal);
PXPublic PXResult PXAPI PXTexturePoolSetSet(PXTexturePool PXREF pxTexturePool, PXTextureSet PXREF pxTextureSet);

#endif
#pragma once


#ifndef PXTexturePoolIncluded
#define PXTexturePoolIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXTextureSet.h>

// A collection of texture sets
typedef struct PXTexturePool_ PXTexturePool;

PXPublic void PXAPI PXTexturePoolRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXTexturePoolCreate(PXTexturePool** pxTexturePoolREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXTexturePoolRelease(PXTexturePool PXREF pxTexturePool);

PXPublic PXSize PXAPI PXTexturePoolAmountGet(PXTexturePool PXREF pxTexturePool);
PXPublic PXResult PXAPI PXTexturePoolReserveAmount(PXTexturePool PXREF pxTexturePool, const PXSize amount);
PXPublic PXTextureSet* PXAPI PXTexturePoolSetGet(const PXTexturePool PXREF pxTexturePool, const PXSize index);
PXPublic PXTextureSet* PXAPI PXTexturePoolTextureGet(const PXTexturePool PXREF pxTexturePool, const PXSize indexTotal);
PXPublic PXResult PXAPI PXTexturePoolSetSet(PXTexturePool PXREF pxTexturePool, PXTextureSet PXREF pxTextureSet);

#endif
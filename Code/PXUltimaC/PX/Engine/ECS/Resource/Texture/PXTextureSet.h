#pragma once

#ifndef PXTextureSetIncluded
#define PXTextureSetIncluded

#include "PXTexture.h"

typedef struct PXTextureSet_ PXTextureSet;


PXPublic PXResult PXAPI PXTextureSetRegisterToECS();
PXPublic PXResult PXAPI PXTextureSetCreate(PXTextureSet** pxTextureSetREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXTextureSetRelease(PXTextureSet PXREF pxTextureSet);

PXPublic PXResult PXAPI PXTextureSetReserveAmount(PXTextureSet PXREF pxTextureSet, const PXSize amount);
PXPublic PXResult PXAPI PXTextureSetEntryAdd(PXTextureSet PXREF pxTextureSet, PXTexture PXREF pxTexture);

PXPublic PXSize PXAPI PXTextureSetEntryAmountGet(PXTextureSet PXREF pxTextureSet);

PXPublic PXTexture* PXAPI PXTextureSetEntryGet(PXTextureSet PXREF pxTextureSet, const PXSize index);

#endif
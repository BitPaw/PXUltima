#pragma once

#ifndef PXDDSIncluded
#define PXDDSIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXDDSLoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXDDSSaveToFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXRSkinIncluded
#define PXRSkinIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXRSkinLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXRSkinSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

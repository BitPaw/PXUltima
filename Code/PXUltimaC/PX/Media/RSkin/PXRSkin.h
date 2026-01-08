#pragma once

#ifndef PXRSkinIncluded
#define PXRSkinIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXRSkinLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXRSkinSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

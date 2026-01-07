#pragma once

#ifndef PXQOIIncluded
#define PXQOIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXQOI_
{
    unsigned int __dummy__;
}
PXQOI;

PXPublic PXResult PXAPI PXQOILoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXQOISaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXPLYIncluded
#define PXPLYIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPLY_
{
    unsigned int __dummy__;
}
PXPLY;

PXPublic PXResult PXAPI PXPLYLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPLYSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

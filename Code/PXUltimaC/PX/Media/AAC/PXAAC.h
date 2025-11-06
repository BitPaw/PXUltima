#pragma once

#ifndef PXAACIncluded
#define PXAACIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXAAC_
{
    unsigned int __dummy__;
}
PXAAC;

PXPublic PXResult PXAPI PXAACLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXAACSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXAACIncluded
#define PXAACIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXAAC_
{
    unsigned int __dummy__;
}
PXAAC;

PXPublic PXResult PXAPI PXAACLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXAACSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXWEBMIncluded
#define PXWEBMIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXWEBM_
{
    unsigned int __dummy__;
}
PXWEBM;

PXPublic PXResult PXAPI PXWEBMLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWEBMSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXVOBLIncluded
#define PXVOBLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXVOB_
{
    unsigned int __dummy__;
}
PXVOB;

PXPublic PXResult PXAPI PXVOBLLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXVOBLSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
#pragma once

#ifndef PXPHPIncluded
#define PXPHPIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPHP_
{
    int x;
}
PXPHP;

PXPublic PXResult PXAPI PXPHPLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPHPSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

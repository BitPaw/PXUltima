#pragma once

#ifndef PXPHPIncluded
#define PXPHPIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPHP_
{
    int x;
}
PXPHP;

PXPublic PXResult PXAPI PXPHPLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPHPSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

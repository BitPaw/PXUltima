#pragma once

#ifndef PXPHPIncluded
#define PXPHPIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPHP_
{
    int x;
}
PXPHP;

PXPublic PXResult PXAPI PXPHPLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPHPSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

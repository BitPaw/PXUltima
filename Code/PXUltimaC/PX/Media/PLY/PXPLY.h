#pragma once

#ifndef PXPLYIncluded
#define PXPLYIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXPLY_
{
    unsigned int __dummy__;
}
PXPLY;

PXPublic PXResult PXAPI PXPLYLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPLYSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

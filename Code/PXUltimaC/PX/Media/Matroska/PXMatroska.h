#pragma once

#ifndef PXMatroskaIncluded
#define PXMatroskaIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMatroska_
{
    unsigned int __dummy__;
}
PXMatroska;

PXPublic PXResult PXAPI PXMatroskaLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMatroskaSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

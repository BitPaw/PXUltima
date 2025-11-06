#pragma once

#ifndef PXMatroskaIncluded
#define PXMatroskaIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMatroska_
{
    unsigned int __dummy__;
}
PXMatroska;

PXPublic PXResult PXAPI PXMatroskaLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMatroskaSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

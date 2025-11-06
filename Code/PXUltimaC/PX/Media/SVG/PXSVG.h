#pragma once

#ifndef PXSVGIncluded
#define PXSVGIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXSVG_
{
    unsigned int __dummy__;
}
PXSVG;

PXPublic PXResult PXAPI PXSVGLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSVGSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

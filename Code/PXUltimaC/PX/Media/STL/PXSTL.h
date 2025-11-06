#pragma once

#ifndef PXSTLIncluded
#define PXSTLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXSTL_
{
    unsigned int __dummy__;
}
PXSTL;

PXPublic PXResult PXAPI PXSTLLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSTLSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

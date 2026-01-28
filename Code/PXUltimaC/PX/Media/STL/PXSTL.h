#pragma once

#ifndef PXSTLIncluded
#define PXSTLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXSTL_
{
    unsigned int __dummy__;
}
PXSTL;

PXPublic PXResult PXAPI PXSTLLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSTLSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

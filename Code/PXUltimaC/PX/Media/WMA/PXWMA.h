#pragma once

#ifndef PXWMAIncluded
#define PXWMAIncluded

#include <PX/Engine/PXResource.h>

// Windows Media Audio
typedef struct PXWMA_
{
    unsigned int __dummy__;
}
PXWMA;

PXPublic PXResult PXAPI PXWMALoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWMASaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

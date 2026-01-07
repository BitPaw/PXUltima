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

PXPublic PXResult PXAPI PXWMALoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWMASaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

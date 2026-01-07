#pragma once

#ifndef PXWEBMIncluded
#define PXWEBMIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXWEBM_
{
    unsigned int __dummy__;
}
PXWEBM;

PXPublic PXResult PXAPI PXWEBMLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWEBMSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

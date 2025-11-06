#pragma once

#ifndef PXMCAIncluded
#define PXMCAIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMCA_
{
    unsigned int __dummy__;
}
PXMCA;

PXPublic PXResult PXAPI PXMCALoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMCASaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

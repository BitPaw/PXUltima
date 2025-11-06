#pragma once

#ifndef PX3DSIncluded
#define PX3DSIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXAutodesk3DS
{
    unsigned int __dummy__;
}
PXAutodesk3DS;

PXPublic PXResult PXAPI PXAutodesk3DSLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXAutodesk3DSSafeFromFile(PXResourceTransphereInfo PXREF pxResourceSafeInfo);

#endif

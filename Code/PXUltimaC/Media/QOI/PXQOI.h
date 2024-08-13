#ifndef PXQOIINCLUDE
#define PXQOIINCLUDE

#include "../PXResource.h"

typedef struct PXQOI_
{
    unsigned int __dummy__;
}
PXQOI;

PXPublic PXActionResult PXAPI PXQOILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXQOISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
#ifndef PXPLYINCLUDE
#define PXPLYINCLUDE

#include "../PXResource.h"

typedef struct PXPLY_
{
    unsigned int __dummy__;
}
PXPLY;

PXPublic PXActionResult PXAPI PXPLYLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXPLYSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

#ifndef PXVOBLINCLUDE
#define PXVOBLINCLUDE

#include "../PXResource.h"

typedef struct PXVOB_
{
    unsigned int __dummy__;
}
PXVOB;

PXPublic PXActionResult PXAPI PXVOBLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXVOBLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
#ifndef PXWEBMINCLUDE
#define PXWEBMINCLUDE

#include "../PXResource.h"

typedef struct PXWEBM_
{
    unsigned int __dummy__;
}
PXWEBM;

PXPublic PXActionResult PXAPI PXWEBMLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXWEBMSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

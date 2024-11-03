#ifndef PXAACINCLUDE
#define PXAACINCLUDE

#include "../PXResource.h"

typedef struct PXAAC_
{
    unsigned int __dummy__;
}
PXAAC;

PXPublic PXActionResult PXAPI PXAACLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXAACSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

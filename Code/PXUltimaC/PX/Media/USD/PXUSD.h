#ifndef PXUSDINCLUDE
#define PXUSDINCLUDE

#include "../PXResource.h"

typedef struct PXUSD_
{
    PXF32 Version;
}
PXUSD;

PXPublic PXActionResult PXAPI PXUSDLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXUSDSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
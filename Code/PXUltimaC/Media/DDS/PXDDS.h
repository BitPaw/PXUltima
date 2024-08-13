#ifndef PXDDSINCLUDE
#define PXDDSINCLUDE

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXDDSLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXDDSSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
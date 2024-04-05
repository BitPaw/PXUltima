#ifndef PXDDSINCLUDE
#define PXDDSINCLUDE

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXDDSLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXDDSSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
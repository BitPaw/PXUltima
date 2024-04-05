#ifndef PXJSONINCLUDE
#define PXJSONINCLUDE

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXJSONLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXJSONSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
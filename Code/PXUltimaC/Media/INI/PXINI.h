#ifndef PXINIInclude
#define PXINIInclude

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXINILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXINISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
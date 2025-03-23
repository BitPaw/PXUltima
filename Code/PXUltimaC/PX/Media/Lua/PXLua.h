#ifndef PXLuaInclude
#define PXLuaInclude

#include "../PXResource.h"

PXPublic PXActionResult PXAPI PXLuaLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXLuaSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
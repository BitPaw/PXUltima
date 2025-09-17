#ifndef PXLuaIncluded
#define PXLuaIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLuaLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLuaSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
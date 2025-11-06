#pragma once

#ifndef PXLuaIncluded
#define PXLuaIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLuaLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLuaSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
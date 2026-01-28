#pragma once

#ifndef PXLuaIncluded
#define PXLuaIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLuaLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLuaSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
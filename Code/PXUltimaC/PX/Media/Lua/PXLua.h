#pragma once

#ifndef PXLuaIncluded
#define PXLuaIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLuaLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLuaSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
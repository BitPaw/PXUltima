#pragma once

#ifndef PXJSONIncluded
#define PXJSONIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXJSONLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJSONSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

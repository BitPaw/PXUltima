#pragma once

#ifndef PXJSONIncluded
#define PXJSONIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXJSONLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJSONSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

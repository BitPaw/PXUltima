#pragma once

#ifndef PXJSONIncluded
#define PXJSONIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXJSONLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXJSONSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

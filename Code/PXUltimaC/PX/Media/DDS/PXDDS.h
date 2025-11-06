#pragma once

#ifndef PXDDSIncluded
#define PXDDSIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXDDSLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXDDSSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

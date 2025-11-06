#pragma once

#ifndef PXTARIncluded
#define PXTARIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXTARLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXTARSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
#pragma once

#ifndef PXTARIncluded
#define PXTARIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXTARLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXTARSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
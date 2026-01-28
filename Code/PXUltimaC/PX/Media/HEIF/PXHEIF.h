#pragma once

#ifndef PXHEIFIncluded
#define PXHEIFIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXHEIFLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXHEIFSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

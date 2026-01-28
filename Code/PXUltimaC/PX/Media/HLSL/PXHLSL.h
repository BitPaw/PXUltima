#pragma once

#ifndef PXHLSLIncluded
#define PXHLSLIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXHLSLLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXHLSLSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
#pragma once

#ifndef PXCPPIncluded
#define PXCPPIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXCPPLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCPPSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

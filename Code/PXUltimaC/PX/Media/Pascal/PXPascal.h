#pragma once

#ifndef PXPascalIncluded
#define PXPascalIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXPascalLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPascalSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
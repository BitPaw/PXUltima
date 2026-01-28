#pragma once

#ifndef PXINIIncluded
#define PXINIIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXINILoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXINISaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

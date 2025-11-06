#pragma once

#ifndef PXINIIncluded
#define PXINIIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXINILoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXINISaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXDDSIncluded
#define PXDDSIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXDDSLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXDDSSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

#pragma once

#ifndef PXPascalIncluded
#define PXPascalIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXPascalLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXPascalSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
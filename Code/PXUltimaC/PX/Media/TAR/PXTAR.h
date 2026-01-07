#pragma once

#ifndef PXTARIncluded
#define PXTARIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXTARLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXTARSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
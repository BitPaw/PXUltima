#pragma once

#ifndef PXHEIFIncluded
#define PXHEIFIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXHEIFLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXHEIFSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif

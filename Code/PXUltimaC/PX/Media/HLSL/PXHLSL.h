#pragma once

#ifndef PXHLSLIncluded
#define PXHLSLIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXHLSLLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXHLSLSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
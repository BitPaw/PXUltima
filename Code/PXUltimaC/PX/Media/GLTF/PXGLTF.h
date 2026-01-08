#pragma once

#ifndef PXGLTFIncluded
#define PXGLTFIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXGLTFLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGLTFSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
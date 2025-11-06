#pragma once

#ifndef PXGLTFIncluded
#define PXGLTFIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXGLTFLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGLTFSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif
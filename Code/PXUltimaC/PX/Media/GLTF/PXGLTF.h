#pragma once

#ifndef PXGLTFIncluded
#define PXGLTFIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXGLTFLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGLTFSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
#pragma once

#ifndef PXGLSLIncluded
#define PXGLSLIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXGLSLLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGLSLSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
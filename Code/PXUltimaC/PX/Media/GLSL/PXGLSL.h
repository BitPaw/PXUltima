#pragma once

#ifndef PXGLSLIncluded
#define PXGLSLIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXGLSLLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGLSLSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
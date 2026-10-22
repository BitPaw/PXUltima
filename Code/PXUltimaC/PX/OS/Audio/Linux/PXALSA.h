#pragma once

#ifndef PXALSAIncluded
#define PXALSAIncluded
#include <PX/Engine/PXResource.h>

#if OSUnix

PXPublic PXResult PXAPI PXALSAInitialize(PXEngineAudio PXREF pxAudio);
PXPublic PXResult PXAPI PXALSARelease(PXEngineAudio PXREF pxAudio);

#endif

#endif


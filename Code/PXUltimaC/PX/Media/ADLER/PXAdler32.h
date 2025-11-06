#pragma once

#ifndef PXAdler32Included
#define PXAdler32Included

#include <PX/Engine/PXResource.h>

PXPublic PXSize PXAPI PXAdler32Check(const void PXREF data, const PXSize length);
PXPublic PXSize PXAPI PXAdler32Create(const PXSize adler, const void PXREF data, PXSize length);

#endif

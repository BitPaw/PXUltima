#ifndef PXAdler32INCLUDE
#define PXAdler32INCLUDE

#include "../PXResource.h"

PXPublic PXSize PXAPI PXAdler32Check(const void* const data, const PXSize length);
PXPublic PXSize PXAPI PXAdler32Create(const PXSize adler, const void* const data, PXSize length);

#endif
#ifndef PXAdler32INCLUDE
#define PXAdler32INCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXSize PXAdler32Check(const void* const data, const PXSize length);
	PXPublic PXSize PXAdler32Create(const PXSize adler, const void* const data, PXSize length);

#ifdef __cplusplus
}
#endif

#endif
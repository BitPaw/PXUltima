#ifndef PXCRC32INCLUDE
#define PXCRC32INCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXInt32U PXCRC32Generate(const PXByte* const data, const PXSize length);

#ifdef __cplusplus
}
#endif

#endif
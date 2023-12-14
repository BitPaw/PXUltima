#ifndef PXMP4MINCLUDE
#define PXMP4MINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXMP4_
	{
		unsigned int __dummy__;
	}
	PXMP4;

	PXPublic PXActionResult PXAPI PXMP4LoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXMP4SaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
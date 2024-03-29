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

	PXPublic PXActionResult PXAPI PXMP4LoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXMP4SaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
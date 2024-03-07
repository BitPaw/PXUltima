#ifndef PXPLYINCLUDE
#define PXPLYINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPLY_
	{
		unsigned int __dummy__;
	}
	PXPLY;

	PXPublic PXActionResult PXAPI PXPLYLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXPLYSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif

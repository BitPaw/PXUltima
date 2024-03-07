#ifndef PXSTLINCLUDE
#define PXSTLINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSTL_
	{
		unsigned int __dummy__;
	}
	PXSTL;

	PXPublic PXActionResult PXAPI PXSTLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXSTLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif

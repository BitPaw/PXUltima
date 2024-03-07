#ifndef PXPHPINCLUDE
#define PXPHPINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPHP_
	{
		int x;
	}
	PXPHP;

	PXPublic PXActionResult PXAPI PXPHPLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXPHPSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
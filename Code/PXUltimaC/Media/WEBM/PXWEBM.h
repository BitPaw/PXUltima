#ifndef PXWEBMINCLUDE
#define PXWEBMINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXWEBM_
	{
		unsigned int __dummy__;
	}
	PXWEBM;

	PXPublic PXActionResult PXAPI PXWEBMLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXWEBMSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
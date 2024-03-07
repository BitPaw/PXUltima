#ifndef PXAACINCLUDE
#define PXAACINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif


	typedef struct PXAAC_
	{
		unsigned int __dummy__;
	}
	PXAAC;

	PXPublic PXActionResult PXAPI PXAACLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXAACSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
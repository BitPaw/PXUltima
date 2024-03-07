#ifndef PXJSONINCLUDE
#define PXJSONINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXJSONLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXJSONSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
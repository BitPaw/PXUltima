#ifndef PXINIInclude
#define PXINIInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXINILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXINISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
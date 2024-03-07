#ifndef PXHTMLInclude
#define PXHTMLInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXHTMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXHTMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXSVGINCLUDE
#define PXSVGINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSVG_
	{
		unsigned int __dummy__;
	}
	PXSVG;

	PXPublic PXActionResult PXAPI PXSVGLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXSVGSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
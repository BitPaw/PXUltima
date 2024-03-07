#ifndef PXVRMLINCLUDE
#define PXVRMLINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXVRML_
	{
		unsigned int __dummy__;
	}
	PXVRML;

	PXPublic PXActionResult PXAPI PXVRMLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXVRMLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif

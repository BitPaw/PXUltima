#ifndef PXJSONINCLUDE
#define PXJSONINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXJSONLoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXJSONSaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
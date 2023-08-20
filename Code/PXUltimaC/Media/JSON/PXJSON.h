#ifndef PXJSONINCLUDE
#define PXJSONINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXJSONLoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXJSONSaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
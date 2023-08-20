#ifndef PXINIInclude
#define PXINIInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXINILoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXINISaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXINIInclude
#define PXINIInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXINILoadFromFile(void* const data, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXINISaveToFile(void* const data, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
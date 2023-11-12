#ifndef PXMatroskaInclude
#define PXMatroskaInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMatroska_
	{
		unsigned int __dummy__;
	}
	PXMatroska;

	PXPublic PXActionResult PXAPI PXMatroskaLoadFromFile(PXVideo* const pxVideo, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXMatroskaSaveToFile(PXVideo* const pxVideo, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
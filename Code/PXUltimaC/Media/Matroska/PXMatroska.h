#ifndef PXMatroskaInclude
#define PXMatroskaInclude

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXMatroska_
	{
		unsigned int __dummy__;
	}
	PXMatroska;

	PXPublic PXActionResult PXMatroskaLoadFromFile(PXVideo* const pxVideo, PXFile* const pxFile);
	PXPublic PXActionResult PXMatroskaSaveToFile(PXVideo* const pxVideo, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
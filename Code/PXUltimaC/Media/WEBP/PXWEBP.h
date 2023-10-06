#ifndef PXWEBPINCLUDE
#define PXWEBPINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXWEBP_
	{
		unsigned int __dummy__;
	}
	PXWEBP;

	PXPublic PXActionResult PXAPI PXWEBPLoadFromFile(PXImage* const pxImage, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXWEBPLoadSaveToFile(PXImage* const pxImage, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
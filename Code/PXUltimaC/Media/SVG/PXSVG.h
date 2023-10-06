#ifndef PXSVGINCLUDE
#define PXSVGINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>
#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSVG_
	{
		unsigned int __dummy__;
	}
	PXSVG;

	PXPublic PXActionResult PXAPI PXSVGLoadFromFile(PXImage* const pxImage, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXSVGSaveToFile(PXImage* const pxImage, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
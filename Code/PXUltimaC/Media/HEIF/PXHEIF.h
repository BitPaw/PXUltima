#ifndef PXHEIFInclude
#define PXHEIFInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXHEIFLoadFromFile(PXImage* const pxImage, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXHEIFSaveToFile(PXImage* const pxImage, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
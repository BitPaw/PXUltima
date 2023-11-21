#ifndef PXCSSINCLUDE
#define PXCSSINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXCSSLoadFromFile(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXCSSSaveToFile(PXDocument* const pxDocument, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
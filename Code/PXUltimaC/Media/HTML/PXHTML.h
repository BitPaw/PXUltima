#ifndef PXHTMLInclude
#define PXHTMLInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXHTMLLoadFromFile(void* const st, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXHTMLSaveToFile(void* const st, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
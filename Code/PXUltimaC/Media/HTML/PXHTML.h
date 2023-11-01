#ifndef PXHTMLInclude
#define PXHTMLInclude

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXHTMLLoadFromFile(void* const st, PXFile* const pxFile);
	PXPublic PXActionResult PXHTMLSaveToFile(void* const st, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
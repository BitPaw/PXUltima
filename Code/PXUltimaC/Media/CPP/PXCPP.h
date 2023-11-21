#ifndef PXCPPINCLUDE
#define PXCPPINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXActionResult PXAPI PXCPPLoadFromFile(PXDocument* const pxDocument, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXCPPSaveToFile(PXDocument* const pxDocument, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
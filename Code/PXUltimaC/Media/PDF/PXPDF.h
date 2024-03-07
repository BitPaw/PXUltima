#ifndef PXPDFINCLUDE
#define PXPDFINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPDF_
	{
		PXInt8U VersionMajor;
		PXInt8U VersionMinor;
	}
	PXPDF;

	PXPublic PXActionResult PXAPI PXPDFLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXPDFSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
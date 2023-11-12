#ifndef PXPDFINCLUDE
#define PXPDFINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXPDF_
	{
		unsigned char VersionMajor;
		unsigned char VersionMinor;
	}
	PXPDF;

	PXPublic PXActionResult PXAPI PXPDFLoadFromFile(PXPDF* const pxPDF, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXPDFSaveToFile(PXPDF* const pxPDF, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
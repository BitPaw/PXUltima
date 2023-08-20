#ifndef PXPDFINCLUDE
#define PXPDFINCLUDE

#include <OS/Error/PXActionResult.h>
#include <Media/PXType.h>
#include <OS/File/PXFile.h>

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

	PXPublic PXActionResult PXPDFLoadFromFile(PXPDF* const pxPDF, PXFile* const pxFile);
	PXPublic PXActionResult PXPDFSaveToFile(PXPDF* const pxPDF, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
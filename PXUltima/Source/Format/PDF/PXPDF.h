#ifndef PXPDFINCLUDE
#define PXPDFINCLUDE

#include <Error/ActionResult.h>
#include <Format/Type.h>
#include <File/PXDataStream.h>

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

	PXPublic ActionResult PXPDFCompile(PXPDF* const pxPDF, PXDataStream* const fileStream);

#ifdef __cplusplus
}
#endif

#endif
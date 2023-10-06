#ifndef PXFLACINCLUDE
#define PXFLACINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>
#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXFLAC_
	{
		unsigned int __dummy__;
	}
	PXFLAC;

	PXPublic PXActionResult PXAPI PXFLACLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXFLACSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
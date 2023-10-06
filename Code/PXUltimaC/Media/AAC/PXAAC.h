#ifndef PXAACINCLUDE
#define PXAACINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif


	typedef struct PXAAC_
	{
		unsigned int __dummy__;
	}
	PXAAC;

	PXPublic PXActionResult PXAPI PXAACLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXAACSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
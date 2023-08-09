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

	PXPublic PXActionResult PXAACLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAACSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXWEBMINCLUDE
#define PXWEBMINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXWEBM_
	{
		unsigned int __dummy__;
	}
	PXWEBM;

	PXPublic PXActionResult PXAPI PXWEBMLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXWEBMSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
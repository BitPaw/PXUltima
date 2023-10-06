#ifndef PXSTEPINCLUDE
#define PXSTEPINCLUDE

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// (St)andard (E)xchange (P)roduct Data (ISO 10303-2xx)
	typedef struct PXSTEP_
	{
		unsigned int __dummy__;
	}
	PXSTEP;

	PXPublic PXActionResult PXAPI PXSTEPLoadFromFile(PXSTEP* const step, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXSTEPSaveToFile(PXSTEP* const step, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
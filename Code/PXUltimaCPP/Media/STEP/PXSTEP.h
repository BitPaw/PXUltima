#ifndef PXSTEPINCLUDE
#define PXSTEPINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

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

	PXPublic PXActionResult PXSTEPParse(PXSTEP* step, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
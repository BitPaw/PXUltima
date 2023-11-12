#ifndef PXWMAINCLUDE
#define PXWMAINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// Windows Media Audio
	typedef struct PXWMA_
	{
		unsigned int __dummy__;
	}
	PXWMA;

	PXPublic PXActionResult PXAPI PXWMALoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXWMASaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXWMAINCLUDE
#define PXWMAINCLUDE

#include <Media/PXResource.h>

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

	PXPublic PXActionResult PXWMALoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXWMASaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
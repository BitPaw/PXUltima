#ifndef PXWMAINCLUDE
#define PXWMAINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

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

	PXPublic PXActionResult PXWMAParse(PXWMA* wma, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXWEBPINCLUDE
#define PXWEBPINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXWEBP_
	{
		unsigned int __dummy__;
	}
	PXWEBP;

	PXPublic PXActionResult PXWEBPParse(PXWEBP* webp, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
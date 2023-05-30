#ifndef PXSVGINCLUDE
#define PXSVGINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXSVG_
	{
		unsigned int __dummy__;
	}
	PXSVG;

	PXPublic PXActionResult PXSVGParse(PXSVG* svg, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
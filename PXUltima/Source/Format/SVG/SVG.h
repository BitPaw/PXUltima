#ifndef SVGInclude
#define SVGInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct SVG_
	{
		unsigned int __dummy__;
	}
	SVG;

	PXPublic ActionResult SVGParse(SVG* svg, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
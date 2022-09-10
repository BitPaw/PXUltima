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

	CPublic ActionResult SVGParse(SVG* svg, const void* data, const size_t dataSize, size_t* dataRead);

#ifdef __cplusplus
}
#endif

#endif
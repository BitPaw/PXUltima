#ifndef STLInclude
#define STLInclude

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct STL_
	{
		unsigned int __dummy__;
	}
	STL;

	PXPublic PXActionResult STLParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
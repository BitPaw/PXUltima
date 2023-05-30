#ifndef PXSTLINCLUDE
#define PXSTLINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXSTL_
	{
		unsigned int __dummy__;
	}
	PXSTL;

	PXPublic PXActionResult PXSTLParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXAACINCLUDE
#define PXAACINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXAAC_
	{
		unsigned int __dummy__;
	}
	PXAAC;

	PXPublic PXActionResult PXAACParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
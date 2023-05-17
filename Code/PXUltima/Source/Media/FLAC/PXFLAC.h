#ifndef PXFLACINCLUDE
#define PXFLACINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXFLAC_
	{
		unsigned int __dummy__;
	}
	PXFLAC;

	PXPublic PXActionResult PXFLACParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
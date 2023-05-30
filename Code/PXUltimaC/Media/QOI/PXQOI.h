#ifndef PXQOIINCLUDE
#define PXQOIINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXQOI_
	{
		unsigned int __dummy__;
	}
	PXQOI;

	PXPublic PXActionResult PXQOIParse(PXQOI* qoi, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
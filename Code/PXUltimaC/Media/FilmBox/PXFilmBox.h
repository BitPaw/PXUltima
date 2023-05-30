#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXFBX_
	{
		unsigned int __dummy__;
	}
	PXFBX;

	PXPublic PXActionResult PXFBXParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
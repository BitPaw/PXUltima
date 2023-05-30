#ifndef PXVRMLINCLUDE
#define PXVRMLINCLUDE

#include <Media/PXType.h>

#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct PXVRML_
	{
		unsigned int __dummy__;
	}
	PXVRML;

	PXPublic PXActionResult PXVRMLParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
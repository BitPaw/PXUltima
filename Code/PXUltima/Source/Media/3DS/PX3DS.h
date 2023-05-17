#ifndef PX3DSINCLUDE
#define PX3DSINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PX3DS
	{
		unsigned int __dummy__;
	}
	PX3DS;

	PXPublic PXActionResult PX3DSParse(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
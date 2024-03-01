#ifndef PXAwaitInclude
#define PXAwaitInclude

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXAwaitInfo_
	{
		volatile void* DataTarget;
		const void* DataExpect;
		PXSize DataSize;
	}
	PXAwaitInfo;

	PXPublic PXBool PXAPI PXAwaitChange(PXAwaitInfo* const pxAwaitInfo);

#ifdef __cplusplus
}
#endif

#endif

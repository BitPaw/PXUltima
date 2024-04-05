#ifndef PXAwaitInclude
#define PXAwaitInclude

#include <Media/PXType.h>

typedef struct PXAwaitInfo_
{
	volatile void* DataTarget;
	const void* DataExpect;
	PXSize DataSize;
}
PXAwaitInfo;

PXPublic PXBool PXAPI PXAwaitChange(PXAwaitInfo* const pxAwaitInfo);

#endif
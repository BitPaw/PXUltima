#ifndef PXAwaitInclude
#define PXAwaitInclude

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool PXAPI PXAwaitChange(volatile void* const dataAdress, const PXSize dataSize);
	PXPublic PXBool PXAPI PXAwaitChangeCU(volatile unsigned char* const dataAdress);

#ifdef __cplusplus
}
#endif

#endif

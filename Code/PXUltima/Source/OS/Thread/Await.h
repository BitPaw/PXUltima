#ifndef PXAwaitInclude
#define PXAwaitInclude

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	PXPublic PXBool PXAwaitChange(const volatile void* const dataAdress, const PXSize dataSize);
	PXPublic PXBool PXAwaitChangeCU(const volatile unsigned char* const dataAdress);

#ifdef __cplusplus
}
#endif

#endif

#ifndef PXISBNINCLUDE
#define PXISBNINCLUDE

#include <Media/PXType.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXISBN_
	{
		int x;
	}
	PXISBN;

	PXPublic PXBool PXISBNIsValid(const void* data, const PXSize dataSize);

#ifdef __cplusplus
}
#endif

#endif
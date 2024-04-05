#ifndef PXISBNINCLUDE
#define PXISBNINCLUDE

#include "../PXResource.h"

typedef struct PXISBN_
{
	int x;
}
PXISBN;

PXPublic PXBool PXAPI PXISBNIsValid(const void* data, const PXSize dataSize);

#endif
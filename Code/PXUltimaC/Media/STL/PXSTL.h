#ifndef PXSTLINCLUDE
#define PXSTLINCLUDE

#include "../PXResource.h"

typedef struct PXSTL_
{
	unsigned int __dummy__;
}
PXSTL;

PXPublic PXActionResult PXAPI PXSTLLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSTLSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
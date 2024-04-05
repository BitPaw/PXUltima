#ifndef PXQOIINCLUDE
#define PXQOIINCLUDE

#include "../PXResource.h"

typedef struct PXQOI_
{
	unsigned int __dummy__;
}
PXQOI;

PXPublic PXActionResult PXAPI PXQOILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXQOISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
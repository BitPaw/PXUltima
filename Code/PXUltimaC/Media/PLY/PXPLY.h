#ifndef PXPLYINCLUDE
#define PXPLYINCLUDE

#include "../PXResource.h"

typedef struct PXPLY_
{
	unsigned int __dummy__;
}
PXPLY;

PXPublic PXActionResult PXAPI PXPLYLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXPLYSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
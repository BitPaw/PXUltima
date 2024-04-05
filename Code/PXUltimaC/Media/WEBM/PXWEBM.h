#ifndef PXWEBMINCLUDE
#define PXWEBMINCLUDE

#include "../PXResource.h"

typedef struct PXWEBM_
{
	unsigned int __dummy__;
}
PXWEBM;

PXPublic PXActionResult PXAPI PXWEBMLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXWEBMSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
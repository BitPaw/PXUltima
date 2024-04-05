#ifndef PXPHPINCLUDE
#define PXPHPINCLUDE

#include "../PXResource.h"

typedef struct PXPHP_
{
	int x;
}
PXPHP;

PXPublic PXActionResult PXAPI PXPHPLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXPHPSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
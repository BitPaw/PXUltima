#ifndef PXPHPINCLUDE
#define PXPHPINCLUDE

#include "../PXResource.h"

typedef struct PXPHP_
{
    int x;
}
PXPHP;

PXPublic PXActionResult PXAPI PXPHPLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXPHPSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
#ifndef PXSTLINCLUDE
#define PXSTLINCLUDE

#include "../PXResource.h"

typedef struct PXSTL_
{
    unsigned int __dummy__;
}
PXSTL;

PXPublic PXActionResult PXAPI PXSTLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSTLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
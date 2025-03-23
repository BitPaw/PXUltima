#ifndef PXSVGINCLUDE
#define PXSVGINCLUDE

#include "../PXResource.h"

typedef struct PXSVG_
{
    unsigned int __dummy__;
}
PXSVG;

PXPublic PXActionResult PXAPI PXSVGLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXSVGSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

#ifndef PXMatroskaInclude
#define PXMatroskaInclude

#include "../PXResource.h"

typedef struct PXMatroska_
{
    unsigned int __dummy__;
}
PXMatroska;

PXPublic PXActionResult PXAPI PXMatroskaLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMatroskaSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

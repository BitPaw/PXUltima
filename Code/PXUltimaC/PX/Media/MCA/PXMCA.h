#ifndef PXMCAInclude
#define PXMCAInclude

#include "../PXResource.h"

typedef struct PXMCA_
{
    unsigned int __dummy__;
}
PXMCA;

PXPublic PXActionResult PXAPI PXMCALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXMCASaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

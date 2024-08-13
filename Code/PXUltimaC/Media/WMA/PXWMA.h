#ifndef PXWMAINCLUDE
#define PXWMAINCLUDE

#include "../PXResource.h"

// Windows Media Audio
typedef struct PXWMA_
{
    unsigned int __dummy__;
}
PXWMA;

PXPublic PXActionResult PXAPI PXWMALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXWMASaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
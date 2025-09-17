#ifndef PXWMAIncluded
#define PXWMAIncluded

#include <PX/Engine/PXResource.h>

// Windows Media Audio
typedef struct PXWMA_
{
    unsigned int __dummy__;
}
PXWMA;

PXPublic PXResult PXAPI PXWMALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWMASaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

#ifndef PXMCAIncluded
#define PXMCAIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMCA_
{
    unsigned int __dummy__;
}
PXMCA;

PXPublic PXResult PXAPI PXMCALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMCASaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

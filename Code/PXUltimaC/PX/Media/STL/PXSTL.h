#ifndef PXSTLIncluded
#define PXSTLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXSTL_
{
    unsigned int __dummy__;
}
PXSTL;

PXPublic PXResult PXAPI PXSTLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSTLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

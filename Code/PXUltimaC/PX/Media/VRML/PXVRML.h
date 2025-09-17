#ifndef PXVRMLIncluded
#define PXVRMLIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXVRML_
{
    unsigned int __dummy__;
}
PXVRML;

PXPublic PXResult PXAPI PXVRMLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXVRMLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

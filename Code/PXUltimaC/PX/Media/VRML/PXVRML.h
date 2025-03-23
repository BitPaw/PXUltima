#ifndef PXVRMLINCLUDE
#define PXVRMLINCLUDE

#include "../PXResource.h"

typedef struct PXVRML_
{
    unsigned int __dummy__;
}
PXVRML;

PXPublic PXActionResult PXAPI PXVRMLLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXVRMLSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include "../PXResource.h"

typedef struct PXFilmBox_
{
    unsigned int __dummy__;
}
PXFilmBox;

PXPublic PXActionResult PXAPI PXFilmBoxLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXFilmBoxSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

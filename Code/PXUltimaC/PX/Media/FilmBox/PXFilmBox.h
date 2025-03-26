#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include "../PXResource.h"

typedef struct PXFilmBox_
{
    PXInt32U Version;
}
PXFilmBox;

PXPublic PXActionResult PXAPI PXFilmBoxLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXFilmBoxSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

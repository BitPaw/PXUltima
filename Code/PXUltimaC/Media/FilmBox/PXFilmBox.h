#ifndef PXFBXINCLUDE
#define PXFBXINCLUDE

#include "../PXResource.h"

typedef struct PXFilmBox_
{
	unsigned int __dummy__;
}
PXFilmBox;

PXPublic PXActionResult PXAPI PXFilmBoxLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXFilmBoxSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
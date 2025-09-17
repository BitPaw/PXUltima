#ifndef PXFBXIncluded
#define PXFBXIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXFilmBox_
{
    PXI32U Version;
}
PXFilmBox;

PXPublic PXResult PXAPI PXFilmBoxLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFilmBoxSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

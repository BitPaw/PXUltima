#ifndef PXFastFileINCLUDE
#define PXFastFileINCLUDE

#include "../PXResource.h"

typedef struct PXFastFile_
{
    PXInt32U Version;
}
PXFastFile;

PXPublic PXActionResult PXAPI PXFastFileLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXFastFileSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
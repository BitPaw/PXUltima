#ifndef PXFastFileIncluded
#define PXFastFileIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXFastFile_
{
    PXI32U Version;
}
PXFastFile;

PXPublic PXResult PXAPI PXFastFileLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFastFileSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

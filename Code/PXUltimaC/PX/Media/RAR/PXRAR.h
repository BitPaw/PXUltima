#ifndef PXRARIncluded
#define PXRARIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXRARVersion_
{
    PXRARVersion1x3,
    PXRARVersion1x5,
    PXRARVersion2x0,
    PXRARVersion2x9,
    PXRARVersion5x0
}
PXRARVersion;

// Roshal Archive
typedef struct PXRAR_
{
    PXRARVersion Version;
}
PXRAR;

PXPublic PXResult PXAPI PXRARLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXRARSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
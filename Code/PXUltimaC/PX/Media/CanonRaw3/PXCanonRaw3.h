#pragma once

#ifndef PXCanonRaw3Included
#define PXCanonRaw3Included

#include <PX/Engine/PXResource.h>

#define PXCanonRaw3UUIDSize 16u

typedef struct PXCanonRaw3_
{


    // ISO chunk
    PXI32U ISOChunkSize;

    char UUID[PXCanonRaw3UUIDSize];
}
PXCanonRaw3;

PXPublic PXResult PXAPI PXCanonRaw3LoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXCanonRaw3SaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

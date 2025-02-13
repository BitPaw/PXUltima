#ifndef PXWEBPINCLUDE
#define PXWEBPINCLUDE

#include "../PXResource.h"

typedef enum PXWEBPChunk_
{
    PXWEBPChunkInvalid,
    PXWEBPChunkUnkown,
    PXWEBPChunkVP8L,
    PXWEBPChunkVP8X,
    PXWEBPChunkICCP,
    PXWEBPChunkANIM,
    PXWEBPChunkEXIF,
    PXWEBPChunkXMP
}
PXWEBPChunk;

typedef struct PXWEBP_
{
    unsigned int __dummy__;
}
PXWEBP;

PXPublic PXActionResult PXAPI PXWEBPLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXWEBPSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

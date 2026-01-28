#pragma once

#ifndef PXWEBPIncluded
#define PXWEBPIncluded

#include <PX/Engine/PXResource.h>

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

PXPublic PXResult PXAPI PXWEBPLoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWEBPSaveToFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif

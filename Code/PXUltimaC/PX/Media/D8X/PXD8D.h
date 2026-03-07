#pragma once

#ifndef PXD8DIncluded
#define PXD8DIncluded

#include <PX/Engine/PXResource.h>
#include <PX/Engine/ECS/Resource/Scene/PXScene.h>


typedef struct PXD8XArchiveEntry_
{
    PXI32U Hash;
    PXI32U Offset;
    PXI32S ChunksCount;
    PXI32S CompressedSize;
    PXI32S DecompressedSize; // Max chunk size 0x8000 (32768)

    PXFile* FileData;
}
PXD8XArchiveEntry;

typedef struct PXD8XDat_
{
    PXI32U TableEntryAmount;
    PXI32U TableOffset;

    PXD8XArchiveEntry* PXD8XDatEntry;
}
PXD8XDat;


PXPublic PXResult PXAPI PXD8DatLoadFromFile(PXScene PXREF pxScene, PXECSCreateInfo PXREF pxResourceLoadInfo);

#endif

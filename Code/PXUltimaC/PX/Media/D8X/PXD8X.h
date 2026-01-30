#pragma once

#ifndef PXD8XIncluded
#define PXD8XIncluded

#include <PX/Engine/PXResource.h>




typedef struct PXD8XDatEntry_
{
    PXI32U Hash;
    PXI32U Offset;
    PXI32S ChunksCount;
    PXI32S CompressedSize;
    PXI32S DecompressedSize; // Max chunk size 0x8000 (32768)

    PXFile* FileData;
}
PXD8XDatEntry;

typedef struct PXD8XDat_
{
    PXI32U TableEntryAmount;
    PXI32U TableOffset;

    PXD8XDatEntry* PXD8XDatEntry;
}
PXD8XDat;





typedef struct PXD8X_
{
    int x;
}
PXD8X;






PXPublic PXResult PXAPI PXD8XLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);

PXPublic PXResult PXAPI PXD8DatLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXD8GLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXD8WLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXD8TLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXD8MLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);

#endif

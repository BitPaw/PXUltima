#pragma once

#ifndef PXJFSIncluded
#define PXJFSIncluded

#include <PX/Engine/PXResource.h>

#define PXJFSEntryCompressionNone 0
#define PXJFSEntryCompressionZLIB 1

typedef struct PXJFSEntry_
{
    PXI32U Hash;
    PXI32U Offset;
    PXI32S CompressedSize;
    PXI32S DecompressedSize;
    PXI32S CompressionLevel;
    PXI32S Compressed;

    PXFile* Data;
}
PXJFSEntry;

typedef struct PXJFS_
{
    PXI32U FileEntryAmount;
    PXJFSEntry* FileEntry;
}
PXJFS;

PXPublic PXResult PXAPI PXJFSRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXJFSLoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxECSCreateInfo);

#endif
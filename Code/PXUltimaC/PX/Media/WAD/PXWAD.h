#pragma once

#ifndef PXWADIncluded
#define PXWADIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXWADHeader_
{
    char MagicCode[2];
    PXI8U VersionMajor;
    PXI8U VersionMinor;
}
PXWADHeader;

typedef struct PXWADVersion1_
{
    PXI16U EntryHeaderOffset;
    PXI16U EntryHeaderSize;
    PXI32U EntryCount;
}
PXWADVersion1;

typedef struct PXWADVersion2_
{
    PXI8U ECDSASignatureLength;
    char ECDSASignature[83];
    PXI64U  XXH64Checksum;
    PXI16U EntryHeaderOffset;
    PXI16U EntryHeaderSize;
    PXI32U  EntryCount;
}
PXWADVersion2;

typedef struct PXWADVersion3_
{
    char ECDSASignature[256];
    char XXH64Checksum[8];
    PXI32U EntryCount;
}
PXWADVersion3;

typedef struct PXWADEntry_
{
    PXI64U PathHash;
    PXI32U DataOffset;
    PXI32U CompressedSize;
    PXI32U UncompressedSize;
    PXI8U DataType;
    PXI8U CountOfSubbchunks;
    PXI8U Setforduplicateentries;
    PXI16U IndexOfFirstSubbchunk;
    PXI64U EntryChecksum;
}
PXWADEntry;

typedef struct PXWAD_
{
    PXWADHeader Header;

    union
    {
        char Data[1];
        PXWADVersion1 Version1;
        PXWADVersion2 Version2;
        PXWADVersion3 Version3;
    };

    PXWADEntry* EntryList;
}
PXWAD;

PXPublic PXResult PXAPI PXWADLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXWADSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
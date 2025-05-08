#ifndef PXWADINCLUDE
#define PXWADINCLUDE

#include "../PXResource.h"

typedef struct PXWADHeader_
{
    char MagicCode[2];
    PXInt8U VersionMajor;
    PXInt8U VersionMinor;
}
PXWADHeader;

typedef struct PXWADVersion1_
{
    PXInt16U EntryHeaderOffset;
    PXInt16U EntryHeaderSize;
    PXInt32U EntryCount;
}
PXWADVersion1;

typedef struct PXWADVersion2_
{
    PXInt8U ECDSASignatureLength;
    char ECDSASignature[83];
    PXInt64U  XXH64Checksum;
    PXInt16U EntryHeaderOffset;
    PXInt16U EntryHeaderSize;
    PXInt32U  EntryCount;
}
PXWADVersion2;

typedef struct PXWADVersion3_
{
    char ECDSASignature[256];
    char XXH64Checksum[8];
    PXInt32U EntryCount;
}
PXWADVersion3;

typedef struct PXWADEntry_
{
    PXInt64U PathHash;
    PXInt32U DataOffset;
    PXInt32U CompressedSize;
    PXInt32U UncompressedSize;
    PXInt8U DataType;
    PXInt8U CountOfSubbchunks;
    PXInt8U Setforduplicateentries;
    PXInt16U IndexOfFirstSubbchunk;
    PXInt64U EntryChecksum;
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

PXPublic PXActionResult PXAPI PXWADLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXWADSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
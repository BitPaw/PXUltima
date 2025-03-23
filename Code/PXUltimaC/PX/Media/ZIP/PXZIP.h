#ifndef PXZIPINCLUDE
#define PXZIPINCLUDE

#include "../PXResource.h"

typedef struct PXZIP_
{
    PXInt16U VersionMinimum; // needed to extract(minimum);
    PXInt16U GeneralPurposeBitFlag;
    PXInt16U CompressionMethod; // e.g.none = 0, DEFLATE = 8 (or "\0x08\0x00");
    PXInt16U FileLastmodificationtime;
    PXInt16U FileLastmodificationdate;
    PXInt32U CRC32ofuncompressedData;
    PXInt32U CompressedSize; // (or 0xffffffff for ZIP64);
    PXInt32U UncompressedSize; // (or 0xffffffff for ZIP64);
    PXInt16U Filenamelength;
    PXInt16U Extrafieldlength;
    char* FileName;
    char* ExtraField;
}
PXZIP;

PXPublic PXActionResult PXAPI PXZIPLoadFromFile(PXResourceTransphereInfo* const pxResourceTransphereInfo);
PXPublic PXActionResult PXAPI PXZIPSaveToFile(PXResourceTransphereInfo* const pxResourceTransphereInfo);

#endif
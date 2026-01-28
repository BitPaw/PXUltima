#pragma once

#ifndef PXZIPIncluded
#define PXZIPIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXZIP_
{
    PXI16U VersionMinimum; // needed to extract(minimum);
    PXI16U GeneralPurposeBitFlag;
    PXI16U CompressionMethod; // e.g.none = 0, DEFLATE = 8 (or "\0x08\0x00");
    PXI16U FileLastmodificationtime;
    PXI16U FileLastmodificationdate;
    PXI32U CRC32ofuncompressedData;
    PXI32U CompressedSize; // (or 0xffffffff for ZIP64);
    PXI32U UncompressedSize; // (or 0xffffffff for ZIP64);
    PXI16U Filenamelength;
    PXI16U Extrafieldlength;
    char* FileName;
    char* ExtraField;
}
PXZIP;

PXPublic PXResult PXAPI PXZIPLoadFromFile(PXECSCreateInfo PXREF PXECSCreateInfo);
PXPublic PXResult PXAPI PXZIPSaveToFile(PXECSCreateInfo PXREF PXECSCreateInfo);

#endif
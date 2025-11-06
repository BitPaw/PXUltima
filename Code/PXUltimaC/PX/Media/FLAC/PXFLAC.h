#pragma once

#ifndef PXFLACIncluded
#define PXFLACIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXFLACBlockType_
{
    PXFLACBlockTypeInvalid,
    PXFLACBlockTypeReserved,
    PXFLACBlockTypeSTREAMINFO,
    PXFLACBlockTypePADDING,
    PXFLACBlockTypeAPPLICATION,
    PXFLACBlockTypeSEEKTABLE,
    PXFLACBlockTypeVORBIS_COMMENT,
    PXFLACBlockTypeCUESHEET,
    PXFLACBlockTypePICTURE
}
PXFLACBlockType;

typedef struct PXFLACBlock_
{
    PXI32U BlockLength;
    PXFLACBlockType BlockType;
    PXBool IsLastMetaDataBlock;
}
PXFLACBlock;

typedef struct PXFLACSteamInfo_
{
    char MD5Signature[16];
    PXI64U TotalSamples;
    PXI32U FrameSizeMinimum;
    PXI32U FrameSizeMaximum;
    PXI32U SampleRate;
    PXI16U SampleSizeMinimum;
    PXI16U SampleSizeMaximum;
    PXI8U NumberOfChannels;
    PXI8U BitsPerSample;
}
PXFLACSteamInfo;

typedef struct PXFLAC_
{
    unsigned int __dummy__;
}
PXFLAC;

PXPrivate inline PXFLACBlockType PXAPI PXFLACBlockTypeFromID(const PXI8U typeID);

PXPublic PXResult PXAPI PXFLACLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXFLACSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif

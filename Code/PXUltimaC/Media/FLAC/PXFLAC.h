#ifndef PXFLACINCLUDE
#define PXFLACINCLUDE

#include "../PXResource.h"

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
	PXInt32U BlockLength;
	PXFLACBlockType BlockType;
	PXBool IsLastMetaDataBlock;
}
PXFLACBlock;

typedef struct PXFLACSteamInfo_
{
	char MD5Signature[16];
	PXInt64U TotalSamples;
	PXInt32U FrameSizeMinimum;
	PXInt32U FrameSizeMaximum;
	PXInt32U SampleRate;
	PXInt16U SampleSizeMinimum;
	PXInt16U SampleSizeMaximum;
	PXInt8U NumberOfChannels;
	PXInt8U BitsPerSample;
}
PXFLACSteamInfo;

typedef struct PXFLAC_
{
	unsigned int __dummy__;
}
PXFLAC;

PXPrivate inline PXFLACBlockType PXAPI PXFLACBlockTypeFromID(const PXInt8U typeID);

PXPublic PXActionResult PXAPI PXFLACLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXFLACSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
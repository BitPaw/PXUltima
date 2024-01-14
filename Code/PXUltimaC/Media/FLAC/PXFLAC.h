#ifndef PXFLACINCLUDE
#define PXFLACINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

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

	PXPublic PXActionResult PXAPI PXFLACLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXFLACSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
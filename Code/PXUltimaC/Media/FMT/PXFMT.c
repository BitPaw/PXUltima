#include "PXFMT.h"

#include <OS/File/PXFile.h>

static const char FMTSignature[4] = { 'f', 'm', 't', ' ' };

PXActionResult PXAPI PXFMTLoadFromFile(PXFMT* const fmt, PXFile* const pxFile, const PXEndian endian)
{
	PXClear(PXFMT, fmt);

	// Check header signature
	{
		const PXBool valid = PXFileReadAndCompare(pxFile, FMTSignature, sizeof(FMTSignature));

		if(!valid)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}
	}

	const PXInt32U x16 = endian == PXEndianLittle ? PXDataTypeInt16ULE : PXDataTypeInt16UBE;
	const PXInt32U x32 = endian == PXEndianLittle ? PXDataTypeInt32ULE : PXDataTypeInt32UBE;

	const PXFileDataElementType pxDataStreamElementList[] =
	{
		{&fmt->ChunkSize,x32},
		{&fmt->AudioFormat,x16},
		{&fmt->NumerOfChannels,x16},
		{&fmt->SampleRate,x32},
		{&fmt->ByteRate,x32},
		{&fmt->BlockAllign,x16},
		{&fmt->BitsPerSample,x16}
	};

	PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

	return PXActionSuccessful;
}

PXActionResult PXAPI PXFMTSaveToFile(const PXFMT* const fmt, PXFile* const pxFile, const PXEndian endian)
{
	const PXInt32U x16 = endian == PXEndianLittle ? PXDataTypeInt16ULE : PXDataTypeInt16UBE;
	const PXInt32U x32 = endian == PXEndianLittle ? PXDataTypeInt32ULE : PXDataTypeInt32UBE;

	const PXFileDataElementType pxDataStreamElementList[] =
	{
		{FMTSignature, PXDataTypeDatax4},
		{&fmt->ChunkSize,x32},
		{&fmt->AudioFormat,x16},
		{&fmt->NumerOfChannels,x16},
		{&fmt->SampleRate,x32},
		{&fmt->ByteRate,x32},
		{&fmt->BlockAllign,x16},
		{&fmt->BitsPerSample,x16}
	};

	PXFileWriteMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

	return PXActionSuccessful;
}
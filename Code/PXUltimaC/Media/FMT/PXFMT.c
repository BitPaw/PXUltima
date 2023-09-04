#include "PXFMT.h"

static const char FMTSignature[4] = { 'f', 'm', 't', ' ' };

PXActionResult PXFMTLoadFromFile(PXFMT* const fmt, PXFile* const pxFile, const PXEndian endian)
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

	const PXDataType x16 = endian == PXEndianLittle ? PXDataTypeLEInt16U : PXDataTypeBEInt16U;
	const PXDataType x32 = endian == PXEndianLittle ? PXDataTypeLEInt32U : PXDataTypeBEInt32U;

	const PXFileDataElementType pxDataStreamElementList[] =
	{
		{x32, &fmt->ChunkSize},
		{x16, &fmt->AudioFormat},
		{x16, &fmt->NumerOfChannels},
		{x32, &fmt->SampleRate},
		{x32, &fmt->ByteRate},
		{x16, &fmt->BlockAllign},
		{x16, &fmt->BitsPerSample}
	};
	const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

	PXFileReadMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

	return PXActionSuccessful;
}

PXActionResult PXFMTSaveToFile(const PXFMT* const fmt, PXFile* const pxFile, const PXEndian endian)
{
	const PXDataType x16 = endian == PXEndianLittle ? PXDataTypeLEInt16U : PXDataTypeBEInt16U;
	const PXDataType x32 = endian == PXEndianLittle ? PXDataTypeLEInt32U : PXDataTypeBEInt32U;

	const PXFileDataElementType pxDataStreamElementList[] =
	{
		{PXDataTypeTextx4, FMTSignature},
		{x32, &fmt->ChunkSize},
		{x16, &fmt->AudioFormat},
		{x16, &fmt->NumerOfChannels},
		{x32, &fmt->SampleRate},
		{x32, &fmt->ByteRate},
		{x16, &fmt->BlockAllign},
		{x16, &fmt->BitsPerSample}
	};
	const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXFileDataElementType);

	PXFileWriteMultible(pxFile, pxDataStreamElementList, pxDataStreamElementListSize);

	return PXActionSuccessful;
}
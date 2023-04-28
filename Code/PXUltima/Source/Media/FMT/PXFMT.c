#include "PXFMT.h"

#include <OS/Memory/PXMemory.h>

#define FMTSignature {'f', 'm', 't', ' '}

void PXFMTConstruct(PXFMT* const fmt)
{
	MemoryClear(fmt, sizeof(PXFMT));
}

PXActionResult PXFMTParse(PXFMT* const fmt, PXFile* const PXFile, const PXEndian endian)
{
	PXFMTConstruct(fmt);

	// Check header signature
	{
		const char expectedValue[] = FMTSignature;
		const PXSize size = sizeof(expectedValue);
		const PXBool valid = PXFileReadAndCompare(PXFile, expectedValue, size);

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

	PXFileReadMultible(PXFile, pxDataStreamElementList, pxDataStreamElementListSize);

	return PXActionSuccessful;
}

PXActionResult FMTSerialize(const PXFMT* const fmt, PXFile* const PXFile, const PXEndian endian)
{
	PXFileWriteB(PXFile, "fmt ", 4);
	PXFileWriteI32U(PXFile, fmt->ChunkSize);
	PXFileWriteI16U(PXFile, fmt->AudioFormat);
	PXFileWriteI16U(PXFile, fmt->NumerOfChannels);
	PXFileWriteI32U(PXFile, fmt->SampleRate);
	PXFileWriteI32U(PXFile, fmt->BitsPerSample); 
	PXFileWriteI16U(PXFile, fmt->BlockAllign);
	PXFileWriteI16U(PXFile, fmt->BitsPerSample); 

	return PXActionSuccessful;
}
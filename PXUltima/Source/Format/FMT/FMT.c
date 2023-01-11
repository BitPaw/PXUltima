#include "FMT.h"

#include <Memory/PXMemory.h>

#define FMTSignature {'f', 'm', 't', ' '}

void FMTConstruct(FMT* const fmt)
{
	MemoryClear(fmt, sizeof(FMT));
}

PXActionResult FMTParse(FMT* const fmt, PXDataStream* const pxDataStream, const Endian endian)
{
	FMTConstruct(fmt);

	// Check header signature
	{
		const char expectedValue[] = FMTSignature;
		const PXSize size = sizeof(expectedValue);
		const PXBool valid = PXDataStreamReadAndCompare(pxDataStream, expectedValue, size);

		if(!valid)
		{
			return PXActionRefusedInvalidHeaderSignature;
		}
	}

	const PXDataType x16 = endian == EndianLittle ? PXDataTypeLEInt16U : PXDataTypeBEInt16U;
	const PXDataType x32 = endian == EndianLittle ? PXDataTypeLEInt32U : PXDataTypeBEInt32U;

	const PXDataStreamElementType pxDataStreamElementList[] =
	{
		{x32, &fmt->ChunkSize},
		{x16, &fmt->AudioFormat},
		{x16, &fmt->NumerOfChannels},
		{x32, &fmt->SampleRate},
		{x32, &fmt->ByteRate},
		{x16, &fmt->BlockAllign},
		{x16, &fmt->BitsPerSample}
	};
	const PXSize pxDataStreamElementListSize = sizeof(pxDataStreamElementList) / sizeof(PXDataStreamElementType);

	PXDataStreamReadMultible(pxDataStream, pxDataStreamElementList, pxDataStreamElementListSize);

	return PXActionSuccessful;
}

PXActionResult FMTSerialize(const FMT* const fmt, PXDataStream* const pxDataStream, const Endian endian)
{
	PXDataStreamWriteB(pxDataStream, "fmt ", 4);
	PXDataStreamWriteI32U(pxDataStream, fmt->ChunkSize);
	PXDataStreamWriteI16U(pxDataStream, fmt->AudioFormat);
	PXDataStreamWriteI16U(pxDataStream, fmt->NumerOfChannels);
	PXDataStreamWriteI32U(pxDataStream, fmt->SampleRate);
	PXDataStreamWriteI32U(pxDataStream, fmt->BitsPerSample); 
	PXDataStreamWriteI16U(pxDataStream, fmt->BlockAllign);
	PXDataStreamWriteI16U(pxDataStream, fmt->BitsPerSample); 

	return PXActionSuccessful;
}
#include "FMT.h"

#include <File/PXDataStream.h>
#include <Memory/PXMemory.h>
#include <Container/ClusterValue.h>

#define FMTSignature {'f', 'm', 't', ' '}

void FMTConstruct(FMT* const fmt)
{
	MemorySet(fmt, sizeof(FMT), 0);
}

ActionResult FMTParse(FMT* const fmt, const void* data, const PXSize dataSize, PXSize* dataRead, const Endian endian)
{
	PXDataStream dataStream;

	FMTConstruct(fmt);
	*dataRead = 0;

	PXDataStreamConstruct(&dataStream);
	PXDataStreamFromExternal(&dataStream, data, dataSize);

	// Check header signature
	{
		const unsigned char expectedValue[] = FMTSignature;

		ClusterInt fmtHeader;

		PXDataStreamReadB(&dataStream, fmtHeader.Data, 4u);

		const unsigned char valid =
			expectedValue[0] == fmtHeader.A &&
			expectedValue[1] == fmtHeader.B &&
			expectedValue[2] == fmtHeader.C &&
			expectedValue[3] == fmtHeader.D;

		if(!valid)
		{
			return ActionInvalidHeaderSignature;
		}
	}

	PXDataStreamReadB(&dataStream, &fmt->ChunkSize, endian);
	PXDataStreamReadB(&dataStream, &fmt->AudioFormat, endian);
	PXDataStreamReadB(&dataStream, &fmt->NumerOfChannels, endian);
	PXDataStreamReadB(&dataStream, &fmt->SampleRate, endian);
	PXDataStreamReadB(&dataStream, &fmt->ByteRate, endian);
	PXDataStreamReadB(&dataStream, &fmt->BlockAllign, endian);
	PXDataStreamReadB(&dataStream, &fmt->BitsPerSample, endian);

	*dataRead = dataStream.DataCursor;

	return ActionSuccessful;
}

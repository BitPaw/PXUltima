#include "FMT.h"

#include <File/DataStream.h>
#include <Memory/PXMemory.h>
#include <Container/ClusterValue.h>

#define FMTSignature {'f', 'm', 't', ' '}

void FMTConstruct(FMT* const fmt)
{
	MemorySet(fmt, sizeof(FMT), 0);
}

ActionResult FMTParse(FMT* const fmt, const void* data, const PXSize dataSize, PXSize* dataRead, const Endian endian)
{
	DataStream dataStream;

	FMTConstruct(fmt);
	*dataRead = 0;

	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);

	// Check header signature
	{
		const unsigned char expectedValue[] = FMTSignature;

		ClusterInt fmtHeader;

		DataStreamReadP(&dataStream, fmtHeader.Data, 4u);

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

	DataStreamReadP(&dataStream, &fmt->ChunkSize, endian);
	DataStreamReadP(&dataStream, &fmt->AudioFormat, endian);
	DataStreamReadP(&dataStream, &fmt->NumerOfChannels, endian);
	DataStreamReadP(&dataStream, &fmt->SampleRate, endian);
	DataStreamReadP(&dataStream, &fmt->ByteRate, endian);
	DataStreamReadP(&dataStream, &fmt->BlockAllign, endian);
	DataStreamReadP(&dataStream, &fmt->BitsPerSample, endian);

	*dataRead = dataStream.DataCursor;

	return ActionSuccessful;
}

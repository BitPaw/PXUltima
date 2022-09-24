#include "FMT.h"

#include <File/DataStream.h>
#include <Memory/Memory.h>
#include <Container/ClusterValue.h>

#define FMTSignature {'f', 'm', 't', ' '}

void FMTConstruct(FMT* const fmt)
{
	MemorySet(fmt, sizeof(FMT), 0);
}

ActionResult FMTParse(FMT* const fmt, const void* data, const size_t dataSize, size_t* dataRead, const Endian endian)
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

		DataStreamReadD(&dataStream, fmtHeader.Data, 4u);

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

	DataStreamReadD(&dataStream, &fmt->ChunkSize, endian);
	DataStreamReadD(&dataStream, &fmt->AudioFormat, endian);
	DataStreamReadD(&dataStream, &fmt->NumerOfChannels, endian);
	DataStreamReadD(&dataStream, &fmt->SampleRate, endian);
	DataStreamReadD(&dataStream, &fmt->ByteRate, endian);
	DataStreamReadD(&dataStream, &fmt->BlockAllign, endian);
	DataStreamReadD(&dataStream, &fmt->BitsPerSample, endian);

	*dataRead = dataStream.DataCursor;

	return ActionSuccessful;
}

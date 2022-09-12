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
	DataStream DataStream;

	FMTConstruct(fmt);
	*dataRead = 0;

	DataStreamConstruct(&DataStream, data, dataSize);

	// Check header signature
	{
		const unsigned char expectedValue[] = FMTSignature;

		ClusterInt fmtHeader;

		DataStreamReadD(&DataStream, fmtHeader.Data, 4u);

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

	DataStreamReadD(&DataStream, &fmt->ChunkSize, endian);
	DataStreamReadD(&DataStream, &fmt->AudioFormat, endian);
	DataStreamReadD(&DataStream, &fmt->NumerOfChannels, endian);
	DataStreamReadD(&DataStream, &fmt->SampleRate, endian);
	DataStreamReadD(&DataStream, &fmt->ByteRate, endian);
	DataStreamReadD(&DataStream, &fmt->BlockAllign, endian);
	DataStreamReadD(&DataStream, &fmt->BitsPerSample, endian);

	*dataRead = DataStream.DataCursor;

	return ActionSuccessful;
}
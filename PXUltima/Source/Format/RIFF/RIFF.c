#include "RIFF.h"

#include <File/DataStream.h>
#include <Container/ClusterValue.h>
#include <Memory/Memory.h>

#define RIFFSignature MakeInt('R', 'I', 'F', 'F')
#define RIFXSignature MakeInt('R', 'I', 'F', 'X')

//#define RIFFSubTypeANI MakeInt('x', 'x', 'x', 'x')
#define RIFFSubTypeAVI MakeInt('A', 'V', 'I', ' ')
#define RIFFSubTypeRDIB MakeInt('R', 'D', 'I', 'B')
//#define RIFFSubTypeBND MakeInt('x', 'x', 'x', 'x')
//#define RIFFSubTypeDXR MakeInt('x', 'x', 'x', 'x')
#define RIFFSubTypePAL MakeInt('P', 'A', 'L', ' ')
//#define RIFFSubTypeRDI MakeInt('x', 'x', 'x', 'x')
#define RIFFSubTypeRMMP MakeInt('R', 'M', 'M', 'P')
#define RIFFSubTypeRMID MakeInt('R', 'M', 'I', 'D')
#define RIFFSubTypeWAVE MakeInt('W', 'A', 'V', 'E')

ActionResult RIFFParse(RIFF* riff, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;

	MemorySet(riff, sizeof(RIFF), 0);
	*dataRead = 0;

	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);

	ClusterInt chunkID;
	ClusterInt formatID;

	DataStreamReadIU(&dataStream, chunkID.Data, 4u);
	DataStreamReadIU(&dataStream, &riff->ChunkSize, EndianLittle);
	DataStreamReadIU(&dataStream, formatID.Data, 4u);

	switch(chunkID.Value) // Detect Endiantype
	{
		case RIFXSignature:
			riff->EndianFormat = EndianBig;
			break;

		case RIFFSignature:
			riff->EndianFormat = EndianLittle;
			break;

		default:
			riff->EndianFormat = EndianInvalid;
			break;
	}

	switch(formatID.Value)
	{
		case RIFFSubTypeAVI:
			riff->Format = RIFFAudioVideoInterleave;
			break;

		case RIFFSubTypeRDIB:
			riff->Format = RIFFDeviceIndependentBitmap;
			break;

		case RIFFSubTypePAL:
			riff->Format = RIFFPalette;
			break;

		case RIFFSubTypeRMMP:
			riff->Format = RIFFMultimediaMovieFile;
			break;

		case RIFFSubTypeRMID:
			riff->Format = RIFFMIDI;
			break;

		case RIFFSubTypeWAVE:
			riff->Format = RIFFWaveformAudio;
			break;

		default:
			riff->Format = RIFFInvalid;
			break;
	}

	riff->Valid = (riff->EndianFormat != EndianInvalid) && (riff->Format != RIFFInvalid);

	*dataRead = dataStream.DataCursor;

	return ActionSuccessful;
}
#include "RIFF.h"

#include <File/PXDataStream.h>
#include <Container/ClusterValue.h>
#include <Memory/PXMemory.h>

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

PXActionResult RIFFParse(RIFF* riff, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXDataStream dataStream;

	MemoryClear(riff, sizeof(RIFF));
	*dataRead = 0;

	PXDataStreamConstruct(&dataStream);
	PXDataStreamFromExternal(&dataStream, data, dataSize);

	ClusterInt chunkID;
	ClusterInt formatID;

	PXDataStreamReadB(&dataStream, chunkID.Data, 4u);
	PXDataStreamReadI32UE(&dataStream, &riff->ChunkSize, EndianLittle);
	PXDataStreamReadB(&dataStream, formatID.Data, 4u);

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

	return PXActionSuccessful;
}

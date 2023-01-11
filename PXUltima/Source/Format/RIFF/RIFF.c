#include "RIFF.h"

#include <File/PXDataStream.h>
#include <Container/ClusterValue.h>
#include <Memory/PXMemory.h>

#define RIFFSignature MakeInt('R', 'I', 'F', 'F')
#define RIFXSignature MakeInt('R', 'I', 'F', 'X')

//#define RIFFSubTypeANI MakeInt('x', 'x', 'x', 'x')
#define RIFFSubTypeAVI MakeInt('A', 'V', 'I', ' ')
#define RIFFSubTypeAVIX MakeInt('A', 'V', 'I', 'X')
#define RIFFSubTypeRDIB MakeInt('R', 'D', 'I', 'B')
//#define RIFFSubTypeBND MakeInt('x', 'x', 'x', 'x')
//#define RIFFSubTypeDXR MakeInt('x', 'x', 'x', 'x')
#define RIFFSubTypePAL MakeInt('P', 'A', 'L', ' ')
//#define RIFFSubTypeRDI MakeInt('x', 'x', 'x', 'x')
#define RIFFSubTypeRMMP MakeInt('R', 'M', 'M', 'P')
#define RIFFSubTypeRMID MakeInt('R', 'M', 'I', 'D')
#define RIFFSubTypeWAVE MakeInt('W', 'A', 'V', 'E')

PXActionResult RIFFParse(RIFF* const riff, PXDataStream* const pxDataStream)
{
	MemoryClear(riff, sizeof(RIFF));

	ClusterInt chunkID;
	ClusterInt formatID;

	PXDataStreamReadB(pxDataStream, chunkID.Data, 4u);
	PXDataStreamReadI32UE(pxDataStream, &riff->ChunkSize, EndianLittle);
	PXDataStreamReadB(pxDataStream, formatID.Data, 4u);

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

		case RIFFSubTypeAVIX:
			riff->Format = RIFFAudioVideoInterleave; // Is this a different version?
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

	return PXActionSuccessful;
}

PXActionResult RIFFSerialize(const RIFF* const riff, PXDataStream* const pxDataStream)
{
	unsigned int riffSignature = 0;
	unsigned int riffType = 0;

	switch (riff->EndianFormat) // Detect Endiantype
	{
		case EndianBig:
			riffSignature = RIFXSignature;
			break;

		case EndianLittle:
			riffSignature = RIFFSignature;
			break;

		default:
			return PXActionInvalid;
	}

	switch (riff->Format)
	{
		case RIFFWaveformAudio:
			riffType = RIFFSubTypeWAVE;
			break;
		case RIFFMultimediaMovieFile:
			riffType = RIFFSubTypeRMMP;
			break;
		case RIFFMIDI:
			riffType = RIFFSubTypeRMID;
			break;
		case RIFFDeviceIndependentBitmap:
			riffType = 0;
			break;
		case RIFFPalette:
			riffType = 0;
			break;
		case RIFFAudioVideoInterleave:
			riffType = RIFFSubTypeAVI;
			break;

		default:
			return PXActionInvalid;
	}


	PXDataStreamWriteI32UE(pxDataStream, riffSignature, EndianLittle);
	PXDataStreamWriteI32UE(pxDataStream, &riff->ChunkSize, EndianLittle);
	PXDataStreamWriteI32UE(pxDataStream, riffType, EndianLittle);

	return PXActionSuccessful;
}
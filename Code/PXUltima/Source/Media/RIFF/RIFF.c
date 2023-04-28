#include "RIFF.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>



#define RIFFSignature PXInt32Make('R', 'I', 'F', 'F')
#define RIFXSignature PXInt32Make('R', 'I', 'F', 'X')
//#define RIFFSubTypeANI PXInt32MakeEndianBig('x', 'x', 'x', 'x')
#define RIFFSubTypeAVI PXInt32Make('A', 'V', 'I', ' ')
#define RIFFSubTypeAVIX PXInt32Make('A', 'V', 'I', 'X')
#define RIFFSubTypeRDIB PXInt32Make('R', 'D', 'I', 'B')
//#define RIFFSubTypeBND PXInt32MakeEndianBig('x', 'x', 'x', 'x')
//#define RIFFSubTypeDXR PXInt32MakeEndianBig('x', 'x', 'x', 'x')
#define RIFFSubTypePAL PXInt32Make('P', 'A', 'L', ' ')
//#define RIFFSubTypeRDI PXInt32MakeEndianBig('x', 'x', 'x', 'x')
#define RIFFSubTypeRMMP PXInt32Make('R', 'M', 'M', 'P')
#define RIFFSubTypeRMID PXInt32Make('R', 'M', 'I', 'D')
#define RIFFSubTypeWAVE PXInt32Make('W', 'A', 'V', 'E')

PXActionResult RIFFParse(RIFF* const riff, PXFile* const PXFile)
{
	MemoryClear(riff, sizeof(RIFF));

	PXInt32UCluster chunkID;
	PXInt32UCluster formatID;

	PXFileReadB(PXFile, chunkID.Data, 4u);
	PXFileReadI32UE(PXFile, &riff->ChunkSize, PXEndianLittle);
	PXFileReadB(PXFile, formatID.Data, 4u);

	switch(chunkID.Value) // Detect Endiantype
	{
		case RIFXSignature:
			riff->EndianFormat = PXEndianBig;
			break;

		case RIFFSignature:
			riff->EndianFormat = PXEndianLittle;
			break;

		default:
			riff->EndianFormat = PXEndianInvalid;
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

	riff->Valid = (riff->EndianFormat != PXEndianInvalid) && (riff->Format != RIFFInvalid);

	return PXActionSuccessful;
}

PXActionResult RIFFSerialize(const RIFF* const riff, PXFile* const PXFile)
{
	unsigned int riffSignature = 0;
	unsigned int riffType = 0;

	switch (riff->EndianFormat) // Detect Endiantype
	{
		case PXEndianBig:
			riffSignature = RIFXSignature;
			break;

		case PXEndianLittle:
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


	PXFileWriteI32UE(PXFile, riffSignature, PXEndianLittle);
	PXFileWriteI32UE(PXFile, &riff->ChunkSize, PXEndianLittle);
	PXFileWriteI32UE(PXFile, riffType, PXEndianLittle);

	return PXActionSuccessful;
}
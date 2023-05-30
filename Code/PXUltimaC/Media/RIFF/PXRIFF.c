#include "PXRIFF.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

#define PXRIFFSignature PXInt32Make('R', 'I', 'F', 'F')
#define RIFXSignature PXInt32Make('R', 'I', 'F', 'X')
//#define PXRIFFSubTypeANI PXInt32MakeEndianBig('x', 'x', 'x', 'x')
#define PXRIFFSubTypeAVI PXInt32Make('A', 'V', 'I', ' ')
#define PXRIFFSubTypeAVIX PXInt32Make('A', 'V', 'I', 'X')
#define PXRIFFSubTypeRDIB PXInt32Make('R', 'D', 'I', 'B')
//#define PXRIFFSubTypeBND PXInt32MakeEndianBig('x', 'x', 'x', 'x')
//#define PXRIFFSubTypeDXR PXInt32MakeEndianBig('x', 'x', 'x', 'x')
#define PXRIFFSubTypePAL PXInt32Make('P', 'A', 'L', ' ')
//#define PXRIFFSubTypeRDI PXInt32MakeEndianBig('x', 'x', 'x', 'x')
#define PXRIFFSubTypeRMMP PXInt32Make('R', 'M', 'M', 'P')
#define PXRIFFSubTypeRMID PXInt32Make('R', 'M', 'I', 'D')
#define PXRIFFSubTypeWAVE PXInt32Make('W', 'A', 'V', 'E')

PXActionResult PXRIFFParse(PXRIFF* const riff, PXFile* const PXFile)
{
	PXMemoryClear(riff, sizeof(PXRIFF));

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

		case PXRIFFSignature:
			riff->EndianFormat = PXEndianLittle;
			break;

		default:
			riff->EndianFormat = PXEndianInvalid;
			break;
	}

	switch(formatID.Value)
	{
		case PXRIFFSubTypeAVI:
			riff->Format = PXRIFFAudioVideoInterleave;
			break;

		case PXRIFFSubTypeAVIX:
			riff->Format = PXRIFFAudioVideoInterleave; // Is this a different version?
			break;

		case PXRIFFSubTypeRDIB:
			riff->Format = PXRIFFDeviceIndependentBitmap;
			break;

		case PXRIFFSubTypePAL:
			riff->Format = PXRIFFPalette;
			break;

		case PXRIFFSubTypeRMMP:
			riff->Format = PXRIFFMultimediaMovieFile;
			break;

		case PXRIFFSubTypeRMID:
			riff->Format = PXRIFFPXMIDI;
			break;

		case PXRIFFSubTypeWAVE:
			riff->Format = PXRIFFWaveformAudio;
			break;

		default:
			riff->Format = PXRIFFInvalid;
			break;
	}

	riff->Valid = (riff->EndianFormat != PXEndianInvalid) && (riff->Format != PXRIFFInvalid);

	return PXActionSuccessful;
}

PXActionResult PXRIFFSerialize(const PXRIFF* const riff, PXFile* const PXFile)
{
	unsigned int riffSignature = 0;
	unsigned int riffType = 0;

	switch (riff->EndianFormat) // Detect Endiantype
	{
		case PXEndianBig:
			riffSignature = RIFXSignature;
			break;

		case PXEndianLittle:
			riffSignature = PXRIFFSignature;
			break;

		default:
			return PXActionInvalid;
	}

	switch (riff->Format)
	{
		case PXRIFFWaveformAudio:
			riffType = PXRIFFSubTypeWAVE;
			break;
		case PXRIFFMultimediaMovieFile:
			riffType = PXRIFFSubTypeRMMP;
			break;
		case PXRIFFPXMIDI:
			riffType = PXRIFFSubTypeRMID;
			break;
		case PXRIFFDeviceIndependentBitmap:
			riffType = 0;
			break;
		case PXRIFFPalette:
			riffType = 0;
			break;
		case PXRIFFAudioVideoInterleave:
			riffType = PXRIFFSubTypeAVI;
			break;

		default:
			return PXActionInvalid;
	}


	PXFileWriteI32UE(PXFile, riffSignature, PXEndianLittle);
	PXFileWriteI32UE(PXFile, &riff->ChunkSize, PXEndianLittle);
	PXFileWriteI32UE(PXFile, riffType, PXEndianLittle);

	return PXActionSuccessful;
}
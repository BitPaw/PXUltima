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
#define PXRIFFSubTypeWEBP PXInt32Make('W', 'E', 'B', 'P')

PXEndian PXAPI PXRIFFEndianFromID(const PXInt32U value)
{
	switch (value)
	{
		case RIFXSignature:		return PXEndianBig;
		case PXRIFFSignature:	return PXEndianLittle;
		default:				return PXEndianInvalid;
	}
}

PXInt32U PXAPI PXRIFFEndianToID(const PXEndian value)
{
	switch (value)
	{
		case PXEndianBig:		return RIFXSignature;
		case PXEndianLittle:	return PXRIFFSignature;
		default:				return 0;
	}
}

PXRIFFFormat PXAPI PXRIFFFormatFromID(const PXInt32U value)
{
	switch (value)
	{
		case PXRIFFSubTypeAVI:	return PXRIFFAudioVideoInterleave;
		case PXRIFFSubTypeAVIX: return PXRIFFAudioVideoInterleave; // Is this a different version?
		case PXRIFFSubTypeRDIB:	return PXRIFFDeviceIndependentBitmap;
		case PXRIFFSubTypePAL:	return PXRIFFPalette;
		case PXRIFFSubTypeRMMP:	return PXRIFFMultimediaMovieFile;
		case PXRIFFSubTypeRMID:	return PXRIFFPXMIDI;
		case PXRIFFSubTypeWAVE:	return PXRIFFWaveformAudio;
		case PXRIFFSubTypeWEBP: return PXRIFFWebPicture;
		default: return PXRIFFInvalid;
	}
}

PXInt32U PXAPI PXRIFFFormatToID(const PXRIFFFormat value)
{
	switch (value)
	{
		case PXRIFFAudioVideoInterleave: return PXRIFFSubTypeAVI;
		//case PXRIFFAudioVideoInterleave: return PXRIFFSubTypeAVIX; // Is this a different version?
		case PXRIFFDeviceIndependentBitmap:	return PXRIFFSubTypeRDIB;
		case PXRIFFPalette:	return PXRIFFSubTypePAL;
		case PXRIFFMultimediaMovieFile:	return PXRIFFSubTypeRMMP;
		case PXRIFFPXMIDI:	return PXRIFFSubTypeRMID;
		case PXRIFFWaveformAudio:	return PXRIFFSubTypeWAVE;
		case PXRIFFWebPicture: return PXRIFFSubTypeWEBP;
		default: return 0;
	}
}

PXActionResult PXAPI PXRIFFLoadFromFile(PXRIFF* const riff, PXFile* const pxFile)
{
	PXInt32UCluster chunkID;
	PXInt32UCluster formatID;

	const PXFileDataElementType pxDataStreamElementList[] =
	{
		{chunkID.Data, PXDataTypeDatax4},
		{&riff->ChunkSize, PXDataTypeInt32ULE},
		{formatID.Data, PXDataTypeDatax4}
	};

	PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

	riff->EndianFormat = PXRIFFEndianFromID(chunkID.Value);
	riff->Format = PXRIFFFormatFromID(formatID.Value);
	riff->Valid = (riff->EndianFormat != PXEndianInvalid) && (riff->Format != PXRIFFInvalid);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXRIFFSaveToFile(const PXRIFF* const riff, PXFile* const pxFile)
{
	const PXInt32U riffSignature = PXRIFFEndianToID(riff->EndianFormat);
	const PXInt32U riffType = PXRIFFFormatToID(riff->Format);

	const PXFileDataElementType pxDataStreamElementList[] =
	{
		{&riffSignature, PXDataTypeInt32ULE},
		{&riff->ChunkSize, PXDataTypeInt32ULE},
		{&riffType, PXDataTypeInt32ULE}
	};

	PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

	return PXActionSuccessful;
}
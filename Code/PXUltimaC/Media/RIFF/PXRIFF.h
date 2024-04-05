#ifndef PXRIFFINCLUDE
#define PXRIFFINCLUDE

#include "../PXResource.h"

typedef enum PXRIFFFormat_
{
	PXRIFFInvalid,
	PXRIFFWebPicture, // WEBP
	PXRIFFWaveformAudio, // WAVE
	PXRIFFMultimediaMovieFile, // RMMP
	PXRIFFPXMIDI, // RMID
	PXRIFFDeviceIndependentBitmap, // RDIB
	PXRIFFPalette,  // PAL
	PXRIFFAudioVideoInterleave, // AVI 
	//DLS,
}
PXRIFFFormat;

// Resource Interchange File Format. 
typedef struct PXRIFF_
{
	PXInt32U ChunkSize;
	PXRIFFFormat Format;
	PXEndian EndianFormat;
	PXBool Valid;
}
PXRIFF;

PXPrivate inline PXEndian PXAPI PXRIFFEndianFromID(const PXInt32U value);
PXPrivate inline PXInt32U PXAPI PXRIFFEndianToID(const PXEndian value);

PXPrivate inline PXRIFFFormat PXAPI PXRIFFFormatFromID(const PXInt32U value);
PXPrivate inline PXInt32U PXAPI PXRIFFFormatToID(const PXRIFFFormat value);


PXPublic PXActionResult PXAPI PXRIFFLoadFromFile(PXRIFF* const riff, PXFile* const PXFile);
PXPublic PXActionResult PXAPI PXRIFFSaveToFile(const PXRIFF* const riff, PXFile* const PXFile);

#endif
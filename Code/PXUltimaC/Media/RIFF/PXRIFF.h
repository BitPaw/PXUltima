#ifndef PXRIFFINCLUDE
#define PXRIFFINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXRIFFFormat_
	{
		PXRIFFInvalid,
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

	PXPrivate inline PXEndian PXRIFFEndianFromID(const PXInt32U value);
	PXPrivate inline PXInt32U PXRIFFEndianToID(const PXEndian value);

	PXPrivate inline PXRIFFFormat PXRIFFFormatFromID(const PXInt32U value);
	PXPrivate inline PXInt32U PXRIFFFormatToID(const PXRIFFFormat value);


	PXPublic PXActionResult PXRIFFParse(PXRIFF* const riff, PXFile* const PXFile);

	PXPublic PXActionResult PXRIFFSerialize(const PXRIFF* const riff, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif
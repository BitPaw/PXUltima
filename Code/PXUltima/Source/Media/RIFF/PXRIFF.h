#ifndef PXRIFFINCLUDE
#define PXRIFFINCLUDE

#include <OS/File/PXFile.h>

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
		PXBool Valid;
		PXEndian EndianFormat;
		unsigned int ChunkSize;
		PXRIFFFormat Format;
	}
	PXRIFF;

	PXPublic PXActionResult PXRIFFParse(PXRIFF* const riff, PXFile* const PXFile);

	PXPublic PXActionResult PXRIFFSerialize(const PXRIFF* const riff, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif
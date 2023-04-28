#ifndef RIFFInclude
#define RIFFInclude

#include <Media/PXType.h>
#include <OS/File/PXFile.h>
#include <OS/Error/PXActionResult.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum RIFFFormat_
	{
		RIFFInvalid,
		RIFFWaveformAudio, // WAVE
		RIFFMultimediaMovieFile, // RMMP
		RIFFMIDI, // RMID
		RIFFDeviceIndependentBitmap, // RDIB
		RIFFPalette,  // PAL
		RIFFAudioVideoInterleave, // AVI 
		//DLS,
	}
	RIFFFormat;

	// Resource Interchange File Format. 
	typedef struct RIFF_
	{
		PXBool Valid;
		PXEndian EndianFormat;
		unsigned int ChunkSize;
		RIFFFormat Format;
	}
	RIFF;

	PXPrivate void RIFFText();

	PXPublic PXActionResult RIFFParse(RIFF* const riff, PXFile* const PXFile);

	PXPublic PXActionResult RIFFSerialize(const RIFF* const riff, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif
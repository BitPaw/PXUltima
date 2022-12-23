#ifndef RIFFInclude
#define RIFFInclude

#include <Format/Type.h>
#include <Error/PXActionResult.h>
#include <File/Endian.h>
#include <File/PXDataStream.h>

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
		Endian EndianFormat;
		unsigned int ChunkSize;
		RIFFFormat Format;
	}
	RIFF;

	PXPublic PXActionResult RIFFParse(RIFF* const riff, PXDataStream* const pxDataStream);

#ifdef __cplusplus
}
#endif

#endif
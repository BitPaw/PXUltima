#ifndef WAVInclude
#define WAVInclude

#include <Format/Type.h>
#include <Error/PXActionResult.h>
#include <File/PXDataStream.h>
#include <Format/FMT/FMT.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// Waveform Audio File
	typedef struct WAV_
	{
		FMT Format;

		unsigned int SoundDataSize; // 32-Bit
		unsigned char* SoundData;
	}
	WAV;

	PXPublic PXActionResult WAVParse(WAV* const wav, PXDataStream* const pxDataStream);

#ifdef __cplusplus
}
#endif

#endif
#ifndef WAVInclude
#define WAVInclude

#include <Format/Type.h>

#include <Error/ActionResult.h>
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

	PXPublic ActionResult WAVParse(WAV* wav, const void* data, const PXSize dataSize, PXSize* dataRead);

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXWAVINCLUDE
#define PXWAVINCLUDE

#include "../PXResource.h"

#include <Media/FMT/PXFMT.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// Waveform Audio File
	typedef struct PXWave_
	{
		PXFMT Format;

		PXInt32U SoundDataSize; // 32-Bit
		void* SoundData;
	}
	PXWave;

	PXPublic PXActionResult PXAPI PXWaveLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXAPI PXWaveSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
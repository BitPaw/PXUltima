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

	PXPublic PXActionResult PXAPI PXWaveLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
	PXPublic PXActionResult PXAPI PXWaveSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#ifdef __cplusplus
}
#endif

#endif
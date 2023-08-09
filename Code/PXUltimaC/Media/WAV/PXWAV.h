#ifndef PXWAVINCLUDE
#define PXWAVINCLUDE

#include <OS/File/PXFile.h>
#include <Media/FMT/PXFMT.h>

#include <Media/PXResource.h>

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

	PXPublic PXActionResult PXWaveLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXWaveSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif
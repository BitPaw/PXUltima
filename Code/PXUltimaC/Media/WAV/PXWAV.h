#ifndef PXWAVINCLUDE
#define PXWAVINCLUDE

#include <OS/File/PXFile.h>
#include <Media/FMT/PXFMT.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// Waveform Audio File
	typedef struct PXWAV_
	{
		PXFMT Format;

		PXInt32U SoundDataSize; // 32-Bit
		void* SoundData;
	}
	PXWAV;

	PXPublic PXActionResult PXWAVParse(PXWAV* const wav, PXFile* const PXFile);
	PXPublic PXActionResult PXWAVSerialize(PXWAV* const wav, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif
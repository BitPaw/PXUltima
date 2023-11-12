#ifndef PXFMTINCLUDE
#define PXFMTINCLUDE

#include "../PXResource.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// Format header
	typedef struct PXFMT_
	{
		PXInt32U ChunkSize;
		PXInt32U SampleRate;
		PXInt32U ByteRate;

		PXInt16U AudioFormat;
		PXInt16U NumerOfChannels;
		PXInt16U BlockAllign;
		PXInt16U BitsPerSample;
	}
	PXFMT;

	PXPublic PXActionResult PXFMTLoadFromFile(PXFMT* const pxFMT, PXFile* const pxFile, const PXEndian endian);

	PXPublic PXActionResult PXFMTSaveToFile(const PXFMT* const pxFMT, PXFile* const pxFile, const PXEndian endian);

#ifdef __cplusplus
}
#endif

#endif
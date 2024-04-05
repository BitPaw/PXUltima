#ifndef PXFMTINCLUDE
#define PXFMTINCLUDE

#include "../PXResource.h"

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

PXPublic PXActionResult PXAPI PXFMTLoadFromFile(PXFMT* const pxFMT, PXFile* const pxFile, const PXEndian endian);
PXPublic PXActionResult PXAPI PXFMTSaveToFile(const PXFMT* const pxFMT, PXFile* const pxFile, const PXEndian endian);

#endif
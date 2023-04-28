#ifndef PXFMTINCLUDE
#define PXFMTINCLUDE

#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	// Format header
	typedef struct PXFMT_
	{
		PXInt32U ChunkSize;
		PXInt16U AudioFormat;
		PXInt16U NumerOfChannels;
		PXInt32U SampleRate;
		PXInt32U ByteRate;
		PXInt16U BlockAllign;
		PXInt16U BitsPerSample;
	}
	PXFMT;

	PXPublic void PXFMTConstruct(PXFMT* const pxFMT);

	PXPublic PXActionResult PXFMTParse(PXFMT* const pxFMT, PXFile* const PXFile, const PXEndian endian);

	PXPublic PXActionResult PXFMTSerialize(const PXFMT* const pxFMT, PXFile* const PXFile, const PXEndian endian);

#ifdef __cplusplus
}
#endif

#endif
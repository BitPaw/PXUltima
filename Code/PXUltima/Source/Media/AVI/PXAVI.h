#ifndef PXAVIINCLUDE
#define PXAVIINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXAVIHeader_
	{
		PXInt32U MicroSecPerFrame; // frame display rate (or 0)
		PXInt32U MaxBytesPerSec; // max. transfer rate
		PXInt32U PaddingGranularity; // pad to multiples of this
		// size;
		PXInt32U Flags; // the ever-present flags
		PXInt32U TotalFrames; // # frames in file
		PXInt32U InitialFrames;
		PXInt32U Streams;
		PXInt32U SuggestedBufferSize;
		PXInt32U Width;
		PXInt32U Height;
		PXInt32U Reserved[4];
	}
	PXAVIHeader;


	typedef struct PXAVI_
	{
		PXAVIHeader Header;
	}
	PXAVI;

	PXPublic PXActionResult PXAVIParse(PXAVI* const avi, PXFile* const PXFile);

#ifdef __cplusplus
}
#endif

#endif
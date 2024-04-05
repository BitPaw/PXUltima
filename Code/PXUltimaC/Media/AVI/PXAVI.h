#ifndef PXAVIINCLUDE
#define PXAVIINCLUDE

#include "../PXResource.h"

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

PXPublic PXActionResult PXAPI PXAVILoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXAVISaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif
#ifndef PXAVIIncluded
#define PXAVIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXAVIHeader_
{
    PXI32U MicroSecPerFrame; // frame display rate (or 0)
    PXI32U MaxBytesPerSec; // max. transfer rate
    PXI32U PaddingGranularity; // pad to multiples of this
    // size;
    PXI32U Flags; // the ever-present flags
    PXI32U TotalFrames; // # frames in file
    PXI32U InitialFrames;
    PXI32U Streams;
    PXI32U SuggestedBufferSize;
    PXI32U Width;
    PXI32U Height;
    PXI32U Reserved[4];
}
PXAVIHeader;

typedef struct PXAVI_
{
    PXAVIHeader Header;
}
PXAVI;

PXPublic PXResult PXAPI PXAVILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXAVISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif

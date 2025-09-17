#ifndef PXFMTIncluded
#define PXFMTIncluded

#include <PX/Engine/PXResource.h>

// Format header
typedef struct PXFMT_
{
    PXI32U ChunkSize;
    PXI32U SampleRate;
    PXI32U ByteRate;

    PXI16U AudioFormat;
    PXI16U NumerOfChannels;
    PXI16U BlockAllign;
    PXI16U BitsPerSample;
}
PXFMT;

PXPublic PXResult PXAPI PXFMTLoadFromFile(PXFMT* const pxFMT, PXFile* const pxFile, const PXEndian endian);
PXPublic PXResult PXAPI PXFMTSaveToFile(const PXFMT* const pxFMT, PXFile* const pxFile, const PXEndian endian);

#endif

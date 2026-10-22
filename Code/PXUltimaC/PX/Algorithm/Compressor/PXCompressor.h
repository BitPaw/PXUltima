#pragma once

#ifndef PXFilterCompressorIncluded
#define PXFilterCompressorIncluded

#include <PX/OS/Error/PXResult.h>

typedef struct PXFilterCompressor_
{
    PXF32 Gain;
    PXF32 Attack;
    PXF32 Release;
    PXF32 Threshold;
    PXF32 Ratio;
    PXF32 Predelay;
}
PXFilterCompressor;

PXPublic void PXAPI PXFilterCompressorProcess(PXFilterCompressor PXREF pxFilterCompressor, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif
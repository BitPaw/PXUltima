#pragma once

#ifndef PXFilterDistortionIncluded
#define PXFilterDistortionIncluded

#include <PX/OS/Error/PXResult.h>

typedef struct PXFilterDistortion_
{
    PXF32 Gain;
    PXF32 Edge;
    PXF32 PostEQCenterFrequency;
    PXF32 PostEQBandwidth;
    PXF32 PreLowpassCutoff;
}
PXFilterDistortion;

PXPublic void PXAPI PXFilterDistortionProcess(PXFilterDistortion PXREF pxFilterDistortion, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif
#pragma once

#ifndef PXFilterChorusIncluded
#define PXFilterChorusIncluded

#include <PX/OS/Error/PXResult.h>

typedef struct PXFilterChorus_
{
    PXF32 Wet;
    PXF32 Dry;
    PXF32 Depth;
    PXF32 Feedback;
    PXF32 Frequency;
    PXF32 Waveform; // LFO shape; DSFXCHORUS_WAVE_xxx
    PXF32 Delay;
    PXF32 Phase;

    PXF32 Rate;

    PXSize IDX;
}
PXFilterChorus;

PXPublic void PXAPI PXFilterChorusProcess(PXFilterChorus PXREF pxFilterChorus, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif
#pragma once

#ifndef PXEffectFlangerIncluded
#define PXEffectFlangerIncluded

#include <PX/Type/PXType.h>

typedef struct PXFilterFlanger_
{
    PXF32 WetDryMix;
    PXF32 Depth;
    PXF32 Feedback;
    PXF32 Frequency;
    PXI32U Waveform;
    PXF32 Delay;
    PXI32U Phase;
}
PXFilterFlanger;

#endif
#pragma once

#ifndef PXEffectFlangerIncluded
#define PXEffectFlangerIncluded

#include <PX/Media/PXType.h>

typedef struct PXFilterFlanger_
{
    PXFAudio WetDryMix;
    PXFAudio Depth;
    PXFAudio Feedback;
    PXFAudio Frequency;
    PXI32U Waveform;
    PXFAudio Delay;
    PXI32U Phase;
}
PXFilterFlanger;

#endif
#pragma once

#ifndef PXFilterChorusIncluded
#define PXFilterChorusIncluded

#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXFilterChorus_
{
    PXFAudio Wet;
    PXFAudio Dry;
    PXFAudio Depth;
    PXFAudio Feedback;
    PXFAudio Frequency;
    PXFAudio Waveform; // LFO shape; DSFXCHORUS_WAVE_xxx
    PXFAudio Delay;
    PXFAudio Phase;

    PXFAudio Rate;

    PXSize IDX;
}
PXFilterChorus;

PXPublic void PXAPI PXFilterChorusProcess(PXFilterChorus PXREF pxFilterChorus, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif
#pragma once

#ifndef PXFilterCompressorIncluded
#define PXFilterCompressorIncluded

#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXFilterCompressor_
{
    PXFAudio Gain;
    PXFAudio Attack;
    PXFAudio Release;
    PXFAudio Threshold;
    PXFAudio Ratio;
    PXFAudio Predelay;
}
PXFilterCompressor;

PXPublic void PXAPI PXFilterCompressorProcess(PXFilterCompressor PXREF pxFilterCompressor, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif
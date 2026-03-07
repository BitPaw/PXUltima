#pragma once

#ifndef PXFilterEqualizerIncluded
#define PXFilterEqualizerIncluded

#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXFilterEqualizer_
{
    PXFAudio Center;
    PXFAudio Bandwidth;
    PXFAudio Gain;
}
PXFilterEqualizer;

PXPublic void PXAPI PXFilterEqualizerProcess(PXFilterEqualizer PXREF pxFilterEqualizer, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);


#endif
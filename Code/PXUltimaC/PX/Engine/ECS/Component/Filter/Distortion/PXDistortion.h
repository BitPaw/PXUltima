#pragma once

#ifndef PXFilterDistortionIncluded
#define PXFilterDistortionIncluded

#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXFilterDistortion_
{
    PXFAudio Gain;
    PXFAudio Edge;
    PXFAudio PostEQCenterFrequency;
    PXFAudio PostEQBandwidth;
    PXFAudio PreLowpassCutoff;
}
PXFilterDistortion;

PXPublic void PXAPI PXFilterDistortionProcess(PXFilterDistortion PXREF pxFilterDistortion, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif
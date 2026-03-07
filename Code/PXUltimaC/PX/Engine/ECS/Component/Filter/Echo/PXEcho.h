#pragma once

#ifndef PXEffectEchoIncluded
#define PXEffectEchoIncluded
#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXFilterEcho_
{
    PXFAudio WetDryMix;
    PXFAudio Feedback;
    PXFAudio LeftDelay;
    PXFAudio RightDelay;
    PXBool PanDelay;
}
PXFilterEcho;

PXPublic void PXAPI PXFilterEchoProcess(PXFilterEcho PXREF pxFilterEcho, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif

#pragma once

#ifndef PXEffectEchoIncluded
#define PXEffectEchoIncluded

#include <PX/OS/Error/PXResult.h>

typedef struct PXFilterEcho_
{
    PXF32 WetDryMix;
    PXF32 Feedback;
    PXF32 LeftDelay;
    PXF32 RightDelay;
    PXBool PanDelay;
}
PXFilterEcho;

PXPublic void PXAPI PXFilterEchoProcess(PXFilterEcho PXREF pxFilterEcho, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

#endif

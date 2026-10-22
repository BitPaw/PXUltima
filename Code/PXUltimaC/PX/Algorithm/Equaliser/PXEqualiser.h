#pragma once

#ifndef PXFilterEqualizerIncluded
#define PXFilterEqualizerIncluded

#include <PX/OS/Error/PXResult.h>

typedef struct PXFilterEqualizer_
{
    PXF32 Center;
    PXF32 Bandwidth;
    PXF32 Gain;
}
PXFilterEqualizer;

PXPublic void PXAPI PXFilterEqualizerProcess(PXFilterEqualizer PXREF pxFilterEqualizer, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);


#endif
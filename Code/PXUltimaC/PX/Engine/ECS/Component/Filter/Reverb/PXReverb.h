#pragma once

#ifndef PXFilterReverbIncluded
#define PXFilterReverbIncluded

#include <PX/Media/PXType.h>

typedef struct PXFilterReverbI3DL2_
{
    PXI32U Room;           // [-10000, 0]      default: -1000 mB
    PXI32U RoomHF;         // [-10000, 0]      default: 0 mB
    PXF32 RoomRolloffFactor; // [0.0, 10.0]      default: 0.0
    PXF32 DecayTime;         // [0.1, 20.0]      default: 1.49s
    PXF32 DecayHFRatio;      // [0.1, 2.0]       default: 0.83
    PXI32U Reflections;    // [-10000, 1000]   default: -2602 mB
    PXF32 ReflectionsDelay;  // [0.0, 0.3]       default: 0.007 s
    PXI32U Reverb;         // [-10000, 2000]   default: 200 mB
    PXF32 ReverbDelay;       // [0.0, 0.1]       default: 0.011 s
    PXF32 Diffusion;         // [0.0, 100.0]     default: 100.0 %
    PXF32 Density;           // [0.0, 100.0]     default: 100.0 %
    PXF32 HFReference;       // [20.0, 20000.0]  default: 5000.0 Hz
}
PXFilterReverbI3DL2;

typedef struct PXFilterReverbWaves_
{
    PXF32 InGain;                // [-96.0,0.0]            default: 0.0 dB
    PXF32 ReverbMix;             // [-96.0,0.0]            default: 0.0 db
    PXF32 ReverbTime;            // [0.001,3000.0]         default: 1000.0 ms
    PXF32 HighFreqRTRatio;       // [0.001,0.999]          default: 0.001
}
PXFilterReverbWaves;

#endif
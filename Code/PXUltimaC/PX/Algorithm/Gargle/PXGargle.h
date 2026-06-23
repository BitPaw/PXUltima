
#pragma once

#ifndef PXEffectGargleIncluded
#define PXEffectGargleIncluded

#include <PX/Type/PXType.h>

typedef struct PXFilterGargle_
{
    PXI32U RateHz;    // Rate of modulation in hz
    PXI32U WaveShape; // DSFXGARGLE_WAVE_xxx
}
PXFilterGargle;

#endif
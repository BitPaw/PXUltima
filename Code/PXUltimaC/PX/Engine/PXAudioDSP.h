#pragma once

#ifndef PXAudioDSPIncluded
#define PXAudioDSPIncluded

#include <PX/Type/PXType.h>

typedef struct PXAudioDSP_ PXAudioDSP;

typedef PXResult(PXAPI* PXAudioDSPEffectFuntion)(PXAudioDSP PXREF pxAudioDSP);

typedef struct PXAudioDSP_
{
    int x;
}
PXAudioDSP;


// 1. MIXING & VECTOR UTILITIES
// ?????????????????????????????????????????????????????????????????????????????

// Sums source samples into destination with a static volume multiplier.
// Ideal for SIMD vectorization (SSE2/AVX) across parallel worker threads.
PXPublic void PXAPI PXAudioDSPMixBuffers(PXAudioDSP PXREF pxAudioDSP, float* dest, const float* src, float gain, int frameCount);

// Smoothly transitions a buffer's gain from start to end over a block.
// Mandatoy for eliminating audio pops/clicks when volume sliders move.
PXPublic void PXAPI PXAudioDSPApplyGainRamp(PXAudioDSP PXREF pxAudioDSP, float* buffer, float startGain, float endGain, int frameCount);

// Clears an audio buffer block to absolute silence (0.0f).
PXPublic void PXAPI PXAudioDSPClearBuffer(PXAudioDSP PXREF pxAudioDSP, float* buffer, int frameCount);


//---------------------------------------------------------
// 3D SPATIAL AUDIO & GEOMETRY
//---------------------------------------------------------
// Calculates volume reduction (0.0f to 1.0f) based on distance and distance roll-off models.
PXPublic float PXAPI PXAudioDSPCalculateDistanceAttenuation(PXAudioDSP PXREF pxAudioDSP, const float listenerPos[3], const float sourcePos[3], float rollOffFactor, float maxDistance);

// Computes the specific Left and Right channel gain multipliers using vector dot products.
PXPublic void PXAPI PXAudioDSPCalculatePannedGains(PXAudioDSP PXREF pxAudioDSP, const float listenerPos[3], const float listenerForward[3], const float sourcePos[3], float* outLeftGain, float* outRightGain);

// Calculates the pitch scaling multiplier required to simulate moving sound sources.
PXPublic float PXAPI PXAudioDSPCalculateDopplerShift(PXAudioDSP PXREF pxAudioDSP, const float sourceVel[3], const float listenerVel[3], float speedOfSound, float basePitch);
//---------------------------------------------------------


//---------------------------------------------------------
// DIGITAL SIGNAL PROCESSING EFFECTS
//---------------------------------------------------------

// Universal filter engine. Processes a block through a standard 2nd-order difference equation.
// By feeding different coefficients, this acts as Low-Pass, High-Pass, or Band-Pass.
PXPublic void PXAPI PXAudioDSPProcessBiquad(PXAudioDSP PXREF pxAudioDSP, float* buffer, const float coefficients[5], float history[4], int frameCount);

// Writes audio into a looping delay history ring-buffer and reads back old frames to create echoes.
PXPublic void PXAPI PXAudioDSPProcessDelayLine(PXAudioDSP PXREF pxAudioDSP, float* buffer, float* delayRingBuffer, int* writeCursor, int delayLengthSamples, float feedback, int frameCount);

// Specialized delay line variation that passes frequencies uniformly but changes phase.
// Chaining multiple All-Pass nodes in parallel is the foundation of digital reverb.
PXPublic void PXAPI PXAudioDSPProcessAllPass(PXAudioDSP PXREF pxAudioDSP, float* buffer, float* delayRingBuffer, int* writeCursor, int delayLengthSamples, float feedback, int frameCount);
//---------------------------------------------------------

//---------------------------------------------------------
// DIAGNOSTICS & UI METERS
// ?????????????????????????????????????????????????????????????????????????????

// Scans a mixed buffer to find the absolute peak value and the Root-Mean-Square (average power).
// Safely exports these values via atomics/ring-buffers to drive real-time DAW volume meters.
PXPublic void PXAPI PXAudioDSPCalculateBlockMetrics(PXAudioDSP PXREF pxAudioDSP, const float* buffer, int frameCount, float* outPeak, float* outRMS);


#endif
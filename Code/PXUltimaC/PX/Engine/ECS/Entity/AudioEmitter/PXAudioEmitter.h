#pragma once

#ifndef PXAudioEmitterIncluded
#define PXAudioEmitterIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXAudioEmitter_
{
    PXECSInfo Info;

    PXF32 DistanceMin;
    PXF32 DistanceMax;
    PXF32 Rolloff;          // attenuation curve strength
    PXF32 SpatialBlend;     // 0=2D, 1=3D
    PXI8U UseDoppler;
    PXI8U Enabled;
}
PXAudioEmitter;

typedef struct PXAudioEmitterCreateInfo_
{
    PXECSCreateInfo Info;
}
PXAudioEmitterCreateInfo;

PXPublic PXResult PXAPI PXAudioEmitterRegisterToECS(void);
PXPublic PXResult PXAPI PXAudioEmitterCreate(PXAudioEmitter** pxAudioEmitterREF, PXAudioEmitterCreateInfo PXREF pxAudioEmitterCreateInfo);
PXPublic PXResult PXAPI PXAudioEmitterRelease(const PXAudioEmitter PXREF pxAudioEmitter);
PXPublic PXResult PXAPI PXAudioEmitterDraw(const PXAudioEmitter PXREF pxAudioEmitter, PXDrawInfo PXREF pxDrawInfo);

#endif
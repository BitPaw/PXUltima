#pragma once

#ifndef PXAudioEmitterIncluded
#define PXAudioEmitterIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef enum PXAudioEmitterMode_
{
    PXAudioEmitterModeInvalid,
    PXAudioEmitterModeLocal, // For SFX and 3D-Music. Can only be MONO!
    PXAudioEmitterModeGlobal // Global music, ambient or sountracks. Should be sterio!
}
PXAudioEmitterMode;

// "Ingame" object that can holds 
// soundclips or make its own noise.
typedef struct PXAudioEmitter_
{
    PXECSInfo Info;

    // DirectSound handle
    // General sound buffer.
    struct IDirectSoundBuffer* BufferHandle;
    // Used for ingame sources of SFX or music. 
    // 3D collapses audio into mono!
    // NOT for normal listen-to Music! 
    struct IDirectSound3DBuffer* Buffer3DHandle; // IDirectSound3DBuffer8

    PXAudioEmitterMode Mode;

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

    PXAudioEmitterMode Mode;
}
PXAudioEmitterCreateInfo;

PXInternal void PXAPI PXAudioEmitterRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXAudioEmitterCreate(PXAudioEmitter** pxAudioEmitterREF, PXAudioEmitterCreateInfo PXREF pxAudioEmitterCreateInfo);
PXPublic PXResult PXAPI PXAudioEmitterRelease(const PXAudioEmitter PXREF pxAudioEmitter);
PXPublic PXResult PXAPI PXAudioEmitterDraw(const PXAudioEmitter PXREF pxAudioEmitter, PXEngineUIDrawInfo PXREF pxDrawInfo);

#endif
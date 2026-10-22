#pragma once

#ifndef PXAudioListenerIncluded
#define PXAudioListenerIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Math/PXVector.h>

typedef struct PXAudioListener_
{
    PXECSInfo Info;

    struct IDirectSoundBuffer* BufferHandle; // Base handle
    struct IDirectSound3DListener* Listener3D;

    PXF32 Gain;          // master listener gain (rarely needed; can be global)
    PXF32 SpeedOfSound;  // optional
    PXI8U Enabled;

    PXVector4F32 Position;
    PXVector4F32 Velocity;
    PXVector4F32 OrientFront;
    PXVector4F32 OrientTop;
    PXF32 DistanceFactor;
    PXF32 RolloffFactor;
    PXF32 DopplerFactor;
}
PXAudioListener;

typedef struct PXAudioListenerCreateInfo_
{
    PXECSCreateInfo Info;
}
PXAudioListenerCreateInfo;

PXPublic void PXAPI PXAudioListenerRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXAudioListenerCreate(PXAudioListener** pxAudioListenerREF, PXAudioListenerCreateInfo PXREF pxAudioListenerCreateInfo);
PXPublic PXResult PXAPI PXAudioListenerRelease(const PXAudioListener PXREF pxAudioListener);
PXPublic PXResult PXAPI PXAudioListenerDraw(const PXAudioListener PXREF pxAudioListener, PXEngineUIDrawInfo PXREF pxDrawInfo);

#endif
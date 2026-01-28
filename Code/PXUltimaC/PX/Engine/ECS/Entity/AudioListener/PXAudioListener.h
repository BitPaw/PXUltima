#pragma once

#ifndef PXAudioListenerIncluded
#define PXAudioListenerIncluded

#include <PX/Engine/ECS/PXECS.h>

typedef struct PXAudioListener_
{
    PXECSInfo Info;

    PXF32 Gain;          // master listener gain (rarely needed; can be global)
    PXF32 DopplerFactor; // optional
    PXF32 SpeedOfSound;  // optional
    PXI8U Enabled;
}
PXAudioListener;

typedef struct PXAudioListenerCreateInfo_
{
    PXECSCreateInfo Info;
}
PXAudioListenerCreateInfo;

PXPublic PXResult PXAPI PXAudioListenerRegisterToECS(void);
PXPublic PXResult PXAPI PXAudioListenerCreate(PXAudioListener** pxAudioListenerREF, PXAudioListenerCreateInfo PXREF pxAudioListenerCreateInfo);
PXPublic PXResult PXAPI PXAudioListenerRelease(const PXAudioListener PXREF pxAudioListener);
PXPublic PXResult PXAPI PXAudioListenerDraw(const PXAudioListener PXREF pxAudioListener, PXDrawInfo PXREF pxDrawInfo);

#endif
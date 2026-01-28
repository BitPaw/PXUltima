#pragma once

#ifndef PXIAudioIncluded
#define PXIAudioIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/System/PXAudioSystem.h>

typedef struct PXCoreAudio_
{
    // Core Audio
    struct IMMDeviceEnumerator* AudioDeviceEnumerator; // IMMDeviceEnumerator*
    struct IMMDevice* AudioDevice; // IMMDevice;

    // WSA-API
    struct IAudioClient* AudioClient;

    // XAudio
    struct IXAudio2* XAudio2API;
}
PXCoreAudio;

PXPublic PXResult PXAPI PXCoreAudioInitialize(PXAudioSystem PXREF pxAudio);

#endif
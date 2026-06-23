#pragma once

#ifndef PXIAudioIncluded
#define PXIAudioIncluded

#include <PX/Engine/PXAudioSystem.h>

typedef struct PXCoreAudio_ PXCoreAudio;

PXPublic PXResult PXAPI PXCoreAudioInitialize(PXAudioSystem PXREF pxAudio);

#endif
#pragma once

#ifndef PXALSAIncluded
#define PXALSAIncluded

#include <PX/Engine/PXResource.h>
//#include <PX/Engine/ECS/System/PXAudioSystem.h>
#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

PXPublic PXResult PXAPI PXALSAInitialize(PXAudioSystem PXREF pxAudio);
PXPublic PXResult PXAPI PXALSARelease(PXAudioSystem PXREF pxAudio);

#endif


#pragma once

#ifndef PXPulseIncluded
#define PXPulseIncluded

#include <PX/Type/PXType.h>

#if OSUnix
PXPublic PXResult PXAPI PXPulseInitialize(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo);
#endif

#endif
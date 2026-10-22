#include "PXPulse.h"

#if OSUnix

#include <pulse/simple.h>
#include <pulse/error.h>

#pragma comment(lib, "pulse.lib")

PXResult PXAPI PXPulseInitialize(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo)
{
    return PXPublic PXResult PXAPI();
}

#endif
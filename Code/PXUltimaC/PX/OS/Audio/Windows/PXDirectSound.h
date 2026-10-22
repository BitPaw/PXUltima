#pragma once

#ifndef PXDirectSoundIncluded
#define PXDirectSoundIncluded

#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXVector.h>

PXPublic PXResult PXAPI PXDirectSoundInitialize(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo);

// Device-Input
PXPublic PXSize PXAPI PXDirectSoundInputAmount(PXEngineAudio PXREF pxEngineAudio);
PXPublic PXAudioDevice* PXAPI PXDirectSoundInputIndex(PXEngineAudio PXREF pxEngineAudio, const PXSize index);
PXPublic PXResult PXAPI PXDirectSoundInputOpen(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundInputClose(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);

PXPublic PXSize PXAPI PXDirectSoundOutputAmount(PXEngineAudio PXREF pxEngineAudio);
PXPublic PXAudioDevice* PXAPI PXDirectSoundOutputIndex(PXEngineAudio PXREF pxEngineAudio, const PXSize index);
PXPublic PXResult PXAPI PXDirectSoundOutputOpen(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundOutputClose(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);

PXPublic PXResult PXAPI PXDirectSoundListenerCreate
(
    PXEngineAudio PXREF pxEngineAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioListener PXREF pxAudioListener
);
PXPublic PXResult PXAPI PXDirectSoundListenerDestroy(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener);
PXPublic PXResult PXAPI PXDirectSoundListenerPositionSet(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener, PXVector4F32 PXREF position);
PXPublic PXResult PXAPI PXDirectSoundListenerOrientationSet(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener, PXVector4F32 PXREF forward, PXVector4F32 PXREF up);

PXPublic PXResult PXAPI PXDirectSoundListenerStagedUpdatesCommit(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener);

PXPublic PXResult PXAPI PXDirectSoundEmitterCreate
(
    PXEngineAudio PXREF pxEngineAudio, 
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioEmitter PXREF pxAudioEmitter, 
    PXSound PXREF pxSound
);

PXPublic PXResult PXAPI PXDirectSoundEmitterDestroy(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter);

PXPublic PXResult PXAPI PXDirectSoundEmitterDataSet
(
    PXEngineAudio PXREF pxEngineAudio, 
    PXAudioEmitter PXREF pxAudioEmitter, 
    PXSound PXREF pxSound
);

PXPublic PXResult PXAPI PXDirectSoundEmitterPlay(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXBool loop);
PXPublic PXResult PXAPI PXDirectSoundEmitterStop(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter);
PXPublic PXResult PXAPI PXDirectSoundEmitterRestart(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter);
PXPublic PXResult PXAPI PXDirectSoundEmitterPositionGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXVector4F32 PXREF position);
PXPublic PXResult PXAPI PXDirectSoundEmitterPositionSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXVector4F32 PXREF position);
PXPublic PXResult PXAPI PXDirectSoundEmitterVelocityGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXVector4F32 PXREF velocity);
PXPublic PXResult PXAPI PXDirectSoundEmitterVelocitySet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXVector4F32 PXREF velocity); // For Doppler effect
PXPublic PXResult PXAPI PXDirectSoundEmitterVolumeGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXF32 PXREF volume);
PXPublic PXResult PXAPI PXDirectSoundEmitterVolumeSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXF32 volume);
PXPublic PXResult PXAPI PXDirectSoundEmitterPitchGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXSize PXREF pitch);
PXPublic PXResult PXAPI PXDirectSoundEmitterPitchSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXSize pitch);
PXPublic PXResult PXAPI PXDirectSoundEmitterCursorGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXAudioCursor PXREF pxAudioCursor);
PXPublic PXResult PXAPI PXDirectSoundEmitterCursorSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXAudioCursor PXREF pxAudioCursor);


PXPublic PXResult PXAPI PXDirectSoundEffectEnable(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundEffectUpdate(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice, void PXREF pxAudioEffect);

#endif
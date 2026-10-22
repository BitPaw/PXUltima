#pragma once

#ifndef PXEngineAudioIncluded
#define PXEngineAudioIncluded

#include <PX/Type/PXType.h>
#include <PX/Type/PXAudioDevice.h>
#include <PX/OS/Library/PXLibrary.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Container/Buffer/PXBuffer.h>
#include <PX/Type/PXAudioFilterBuffer.h>
#include <PX/Math/PXVector.h>
#include "PXDictionaryECS.h"
#include <PX/Type/PXAudioEmitter.h>
#include <PX/Type/PXAudioListener.h>
#include <PX/Engine/PXEngineModule.h>


#define PitchMaximum 2.9f
#define PitchMinimum 0.2f


#define PXAudioWaveTypeSINE         1
#define PXAudioWaveTypeSQUARE       2
#define PXAudioWaveTypeTRIANGLE     3
#define PXAudioWaveTypeSAWTOOTH     4
#define PXAudioWaveTypeREVERSE_SAW  5
#define PXAudioWaveTypePULSE        7
#define PXAudioWaveTypeNoiseWhite   8
#define PXAudioWaveTypeNoiseGray    9
#define PXAudioWaveTypeNoisePink    10
#define PXAudioWaveTypeNoiseBrown   11

typedef enum PXSoundPlayStyle_
{
    SoundPlayStyleOnce,
    SoundPlayStyleLoop
}
PXSoundPlayStyle;

typedef enum PXAudioChannelLayout_
{
    PXAudioChannelLayoutMONO,
    PXAudioChannelLayoutSTEREO,
    PXAudioChannelLayout5x1,
    PXAudioChannelLayout7x1
}
PXAudioChannelLayout;

typedef struct PXAudioClip_
{
    unsigned int ID;
    PXSoundPlayStyle PlayStyle;
}
PXAudioClip;

typedef struct PXAudioConfig_
{
    PXF32 VolumeMusic;
    PXF32 VolumeSFX;
}
PXAudioConfig;


// Current cursor index of read and write operations.
typedef struct PXAudioCursor_
{
    PXSize Read;
    PXSize Write;
}
PXAudioCursor;

// If we have a ring buffer, with 
// overlap data, we will have two inertion points.
typedef struct PXAudioCursorInsert_
{
    void* A;
    PXSize ALength;

    void* B;
    PXSize BLength;
}
PXAudioCursorInsert;


// Effects
typedef enum PXAudioEffectType_
{
    PXAudioEffectTypeInvalid,
    PXAudioEffectTypeChorus,
    PXAudioEffectTypeCompressor,
    PXAudioEffectTypeDistortion,
    PXAudioEffectTypeEcho,
    PXAudioEffectTypeFlanger,
    PXAudioEffectTypeGargle,
    PXAudioEffectTypeInteractive3DLevel2Reverb,
    PXAudioEffectTypeParamEq,
    PXAudioEffectTypeWavesReverb
}
PXAudioEffectType;

/*
typedef struct PXAudioEffect_
{
    PXAudioEffectType Type;
    
    void* Filter;

    PXBool Enable;
    PXBool Fetch;
}
PXAudioEffect;


typedef PXResult(PXAPI* PXAudioDeviceEffectUpdate)(void PXREF audioAPI, PXAudioDevice PXREF pxAudioDevice, PXAudioEffect PXREF pxAudioEffect);
*/


typedef struct PXEngineAudio_ PXEngineAudio;
typedef struct PXEngineAudioCreateInfo_ PXEngineAudioCreateInfo;


typedef PXResult(PXAPI* PXAudioInitializeFunction)(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo);
typedef PXResult(PXAPI* PXAudioShutdownFunction)(PXEngineAudio PXREF pxEngineAudio);

typedef void (PXAPI* PXAudioPlaybackStartFunction)(PXEngineAudio PXREF pxEngineAudio);
typedef void (PXAPI* PXAudioPlaybackStopFunction)(PXEngineAudio PXREF pxEngineAudio);


// Device-Input
typedef PXSize(PXAPI* PXAudioDeviceInputAmountFunction)(PXEngineAudio PXREF pxEngineAudio);
typedef PXAudioDevice*(PXAPI* PXAudioDeviceInputIndexFunction)(PXEngineAudio PXREF pxEngineAudio, const PXSize index);
typedef PXResult(PXAPI* PXAudioDeviceInputOpenFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
typedef PXResult(PXAPI* PXAudioDeviceInputCloseFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
// Device-Output
typedef PXSize (PXAPI* PXAudioDeviceOutputAmountFunction)(PXEngineAudio PXREF pxEngineAudio);
typedef PXAudioDevice* (PXAPI* PXAudioDeviceOutputIndexFunction)(PXEngineAudio PXREF pxEngineAudio, const PXSize index);
typedef PXResult(PXAPI* PXAudioDeviceOutputOpenFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);
typedef PXResult(PXAPI* PXAudioDeviceOutputCloseFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice);


// Listener
typedef PXResult(PXAPI* PXAudioListenerCreateFunction)
(
    PXEngineAudio PXREF pxEngineAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioListener PXREF pxAudioListener
);
typedef PXResult(PXAPI* PXAudioListenerDestroyFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener);
typedef PXResult(PXAPI* PXAudioListenerPositionSetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener, PXVector4F32 PXREF position);
typedef PXResult(PXAPI* PXAudioListenerOrientationSetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener, PXVector4F32 PXREF forward, PXVector4F32 PXREF up);
typedef PXResult(PXAPI* PXAudioListenerStagedUpdatesCommit)(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener);

typedef void (PXAPI* PXAudioMasterVolumeSetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener, const PXF32 volume);

// --- Sound Emitter Management ---
typedef PXResult(PXAPI* PXAudioEmitterCreateFunction)
(
    PXEngineAudio PXREF pxEngineAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioEmitter PXREF pxAudioEmitter,
    PXSound PXREF pxSound
);
typedef PXResult(PXAPI* PXAudioEmitterDestroyFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter);

// --- Emitter Control ---
typedef PXResult (PXAPI* PXAudioEmitterPlayFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXBool loop);
typedef PXResult (PXAPI* PXAudioEmitterStopFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter);
typedef PXResult (PXAPI* PXAudioEmitterPositionSetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXVector4F32 PXREF position);
typedef PXResult (PXAPI* PXAudioEmitterVelocitySetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXVector4F32 PXREF velocity); // For Doppler effect
typedef PXResult (PXAPI* PXAudioEmitterVolumeSetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXF32 volume);
typedef PXResult (PXAPI* PXAudioEmitterPitchSetFunction)(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXSize pitch);

// General audio wrapper around system solutions
typedef struct PXEngineAudio_
{
    PXEngineModule ModuleInfo;

    PXAudioInitializeFunction Initialize;
    PXAudioShutdownFunction Shutdown;
    PXAudioDeviceInputAmountFunction DeviceInputAmount;
    PXAudioDeviceInputIndexFunction DeviceInputIndex;
    PXAudioDeviceInputOpenFunction DeviceInputOpen;
    PXAudioDeviceInputCloseFunction DeviceInputClose;

    PXAudioDeviceOutputAmountFunction DeviceOutputAmount;
    PXAudioDeviceOutputIndexFunction DeviceOutputIndex;
    PXAudioDeviceOutputOpenFunction DeviceOutputOpen;
    PXAudioDeviceOutputCloseFunction DeviceOutputClose;

    PXAudioListenerCreateFunction ListenerCreate;
    PXAudioListenerDestroyFunction ListenerDestroy;
    PXAudioPlaybackStartFunction PlaybackStart;
    PXAudioPlaybackStopFunction PlaybackStop;
    PXAudioListenerStagedUpdatesCommit ListenerStagedUpdatesCommit;
    PXAudioListenerPositionSetFunction ListenerPositionSet;
    PXAudioListenerOrientationSetFunction ListenerOrientationSet;
    PXAudioMasterVolumeSetFunction MasterVolumeSet;

    PXAudioEmitterCreateFunction EmitterCreate;
    PXAudioEmitterDestroyFunction EmitterDestroy;
    PXAudioEmitterPlayFunction EmitterPlay;
    PXAudioEmitterStopFunction EmitterStop;
    PXAudioEmitterPositionSetFunction EmitterPositionSet;
    PXAudioEmitterVelocitySetFunction EmitterVelocitySet;
    PXAudioEmitterVolumeSetFunction EmitterVolumeSet;
    PXAudioEmitterPitchSetFunction EmitterPitchSet;

    // General list of all audio devices.
    PXDictionaryECS DeviceOutputLookup;
    PXDictionaryECS DeviceInputLookup;
}
PXEngineAudio;

typedef struct PXEngineAudioCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize AudioInitializeIndexTarget;

    PXAudioInitializeFunction* AudioInitializeList;
    PXSize AudioInitializeListAmount;
}
PXEngineAudioCreateInfo;


PXPublic void PXAPI PXEngineAudioRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXEngineAudioCreate(PXEngineAudio** pxEngineAudioREF, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo);
PXPublic PXResult PXAPI PXEngineAudioRelease(PXEngineAudio PXREF pxEngineAudio);
PXPublic PXResult PXAPI PXEngineAudioUpdate(PXEngineAudio PXREF pxEngineAudio, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);

// Global function to register solutions
PXPublic PXSize PXAPI PXEngineAudioAmount(void);
PXPublic PXAudioInitializeFunction PXAPI PXEngineAudioIndex(const PXSize index);
PXPublic PXSize PXAPI PXEngineAudioRegister(PXAudioInitializeFunction pxAudioInitializeFunction);
PXPublic PXResult PXAPI PXEngineAudioUnregister(const PXSize index);

PXPublic PXResult PXAPI PXAudioSpeakerBeep(const PXI32U hz, const PXI32U time);

PXPublic PXResult PXAPI PXAudioDeviceInputAmountEnsure(PXEngineAudio PXREF pxEngineAudio, const PXSize amount);
PXPublic PXResult PXAPI PXAudioDeviceOutputAmountEnsure(PXEngineAudio PXREF pxEngineAudio, const PXSize amount);
PXPublic PXResult PXAPI PXAudioDeviceDetect(PXEngineAudio PXREF pxEngineAudio, PXAudioDeviceCreateInfo PXREF pxAudioDeviceCreateInfo);

/*
//___________________________
// Basic Filters

One ? Pole Low ? Pass — smooths, darkens, removes highs
One ? Pole High ? Pass — removes lows, makes sounds thinner
One ? Pole All ? Pass — alters phase only
Biquad LPF / HPF / BPF / Notch — clean standard filters, flexible
Shelving Filters(Low / High Shelf) — boosts / dips highs or lows
Peak / Parametric EQ — boosts or cuts a frequency band
Notch Filter — removes narrow frequency band
Band ? Reject — broad version of notch

//___________________________
// Advanced Filters / Structures

State Variable Filter(SVF) — multi ? mode, stable, expressive
Chamberlin SVF — simple, musical SVF variant
Butterworth Filter — flat response, smooth
Chebyshev Filter — sharp cutoff, ripple
Linkwitz ? Riley Filter — crossover, smooth summing
Elliptic Filter — extremely sharp, heavy ripple
Moog Ladder Filter — analog ? style warm filtering
Korg MS20 Filter — aggressive analog behavior
Sallen ? Key Filter — analog circuit model, resonant

//___________________________
// Nonlinear / Creative Filters

Waveshaper — shapes waveform, harmonic creation
Saturator — soft clipping / harmonic fattening
Hard Clipping — aggressive distortion
Foldback Distortion — metallic, chaotic distortion
Bitcrusher — reduces resolution, retro effect
Sample ? Rate Reducer — aliasing ? style digital crunch

//___________________________
// Delay?Based Filters

Comb Filter(Feedforward) — metallic resonance
Comb Filter(Feedback) — pitched resonance, Karplus–Strong
All ? Pass Delay — phase shaping, basis for reverb
Fractional Delay Filter — delay with interpolation

//___________________________
// Modulated Filters / FX

Flanger — sweeping metallic comb effect
Chorus — thickening, widening
Vibrato — pitch wobble via modulated delay
Phaser — sweeping notches via all ? pass network
Tremolo — volume LFO

//___________________________
// Spatial Filters

Panning Filter(Equal ? Power) — stereo placement
Distance Low ? Pass — rolloff with distance
Head ? Shadow Filter — simple directional filter
HRTF Filters — 3D ear ? shaping filters
Doppler Shift — moving source pitch shift

//___________________________
// Reverb Components

Schroeder All ? Pass — early diffusion
Schroeder Comb — resonance for reverb body
FDN(Feedback Delay Network) — modern reverb core
Diffuse Delay — smears echo into reverb tail

//___________________________
// Dynamic Filters

Envelope Follower — outputs loudness curve
Compressor / Limiter — reduces peaks
Transient Shaper — boosts attack or sustain
Noise Gate — cuts signal below threshold
Expander — increases dynamic range

//___________________________
// Pitch / Frequency Filters

Pitch Shifter(Grain ? Based) — simple detune
Frequency Shifter(Hilbert) — shifts spectrum linearly
Ring Modulator — multiplies signals, metallic tones
Octaver — signal cloned at double / half frequency

//___________________________
// Spectral Filters(FFT?based)

EQ(FFT bin shaping) — frequency ? domain filter
Spectral Gate — mutes certain bins
Spectral Blur / Smear — reverb - like smearing
Spectral Freeze — holds a spectral snapshot

//___________________________
// Synthesis?Oriented Filters
Formant Filter(Vocal Filter) — vowel ? like resonances
Resonator Bank — multiple tuned bandpass filters
Modal Filter — simulates resonant objects
Karplus–Strong Loop Filter — damping for plucked strings
Waveterrain Filter — odd nonlinear waveshaping

//___________________________
// Noise Filters
Pink Noise Filter(1 / f) — smooth noisy textures
Brown Noise Filter — deep rumbling noise
Blue / Violet Noise Filters — bright digital noise
*/


typedef struct PXSFXParams_
{
    int WaveType;
    float base_freq;
    float freq_slide;
    float vibrato_speed;
    float vibrato_depth;
    float attack;
    float sustain;
    float decay;
    float volume;

    float PI;
    float SampleRate;
} 
PXSFXParams;

typedef struct PXAudioWaveGenerateInfo_
{
    int WaveType;

    float X; // Time varable
   // float Amplitude;

    float Hz;
    float PWM;
    float Amplitude; // Value, [0.0, 1.0)
}
PXAudioWaveGenerateInfo;

PXPublic PXF32 PXAPI PXAudioWaveGenerate(PXAudioWaveGenerateInfo* pxAudioWaveGenerateInfo);

PXPublic PXF32 PXAPI envelope(PXF32 t, PXF32 attack, PXF32 sustain, PXF32 decay);
PXPublic void PXAPI generate_sfx(PXI16U* buffer, int samples, PXSFXParams* params);
PXPublic PXF32 PXAPI lowpass_filter(PXF32 input, PXF32* prev, PXF32 alpha);

#endif
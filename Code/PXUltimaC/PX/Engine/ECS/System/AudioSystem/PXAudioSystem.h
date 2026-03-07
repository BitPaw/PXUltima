#pragma once

#ifndef PXAudioSystemIncluded
#define PXAudioSystemIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>
#include <PX/Engine/ECS/Resource/Sound/PXSound.h>
#include <PX/Engine/ECS/Resource/AudioDevice/PXAudioDevice.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Container/Buffer/PXBuffer.h>


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

typedef enum PXAudioSystemAPI_
{
    PXAudioSystemInvalid,

    // Legacy - Windows 2000
    PXAudioSystemWindowsMultiMedia, // Windows 2000
    PXAudioSystemWindowsMIDI,

    // Legcy - Windows XP
    PXAudioSystemWindowsDirectSound, // Windows XP (successor to WINMM)

    // Modern
    PXAudioSystemWindowsAudioSessionAPI, // Windows Vista (successor to DirectSound)
    PXAudioSystemWindowsXAudio // Windows 8 (successor to IAudio)
}
PXAudioSystemAPI;

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


//--------------------------------------------------------
// Note: these do not require a any device!
#define PXSoundDeviceAmount    0

#define PXSoundDevicePropertyPositionCursor     1
#define PXSoundDevicePropertySpeed              2
#define PXSoundDevicePropertyPositionSpace      3
#define PXSoundDevicePropertyAngleCone          4
#define PXSoundDevicePropertyConeOrientation    5
#define PXSoundDevicePropertyDistanceMaximum    6
#define PXSoundDevicePropertyDistanceMinimum    7
#define PXSoundDevicePropertyMode               8
#define PXSoundDevicePropertyVelocity           9
#define PXSoundDevicePropertyDistanceFactor     10
#define PXSoundDevicePropertyDopplerFactor      11
#define PXSoundDevicePropertyOrientation        12
#define PXSoundDevicePropertyRolloffFactor      13
#define PXSoundDevicePropertyPitch              14
#define PXSoundDevicePropertyVolume             15

// State Tranitions



//  DS3D_MINDOPPLERFACTOR to DS3D_MAXDOPPLERFACTOR
#define PXSoundDevicePropertyStatePlay      16
#define PXSoundDevicePropertyStatePause     17
#define PXSoundDevicePropertyStateRestart   18
#define PXSoundDevicePropertyStateReset     19


// Shall we read or write this property?
#define PXSoundDevicePropertyRWMask     0b11
#define PXSoundDevicePropertyRead       0b01
#define PXSoundDevicePropertyWrite      0b10

//#define PXSoundDevicePropertyRead
//#define PXSoundDevicePropertyWrite

// Transphere object
typedef struct PXSoundDeviceProperty_
{
    PXI16U Type;
    PXI16U Flags;

    union
    {
        PXI32U Value;
        PXVector2F32 Position2D;
        PXVector3F32 Position3D;
    };
}
PXSoundDeviceProperty;


typedef PXResult (PXAPI* PXSoundDevicePropertyFunction)(PXAudioDevice PXREF pxAudioDevice, PXSoundDeviceProperty PXREF pxSoundDeviceProperty);











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









typedef struct PXAudioEffect_
{
    PXAudioEffectType Type;
    
    void* Filter;

    PXBool Enable;
    PXBool Fetch;
}
PXAudioEffect;


typedef PXResult(PXAPI* PXAudioDeviceEffectUpdate)(void PXREF audioAPI, PXAudioDevice PXREF pxAudioDevice, PXAudioEffect PXREF pxAudioEffect);





typedef struct PXAudioSystem_ PXAudioSystem;
typedef struct PXAudioSystemCreateInfo_ PXAudioSystemCreateInfo;



typedef PXResult(PXAPI* PXAudioInitializeFunction)(PXAudioSystem PXREF pxAudioSystem, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo);


typedef struct PXAudioSystemBinding_
{
    PXAudioInitializeFunction Initialize;
    PXAudioDeviceFetchFunction DeviceFetch;
    PXAudioDeviceFetchAllFunction DeviceFetchAll;
    PXAudioDeviceOpenFunction DeviceOpen;
    PXAudioDeviceCloseFunction DeviceClose;
    PXAudioDeviceLoadFunction DeviceLoad;


    PXSoundDevicePropertyFunction DeviceProperty;

    // Effects
    PXAudioDeviceEffectUpdate DeviceEffectUpdate;
}
PXAudioSystemBinding;

// General audio wrapper around system solutions
typedef struct PXAudioSystem_
{
    PXECSInfo Info;

#if 0
    union
    {
        //PXAudioDirectSound DirectSound;
        //PXAudioMultiMedia MultiMedia;
        //PXAudioXSystem XSystem;
    };
#endif


    PXI16U DeviceAmountOutput;
    PXI16U DeviceAmountInput;

    PXAudioDevice* DeviceOutputList;


    // Binding for API
    void* SystemReference; // Private field to hold reference to object
    PXAudioSystemAPI AudioSystemAPI; // Type of system

    PXAudioSystemBinding Binding;
}
PXAudioSystem;

typedef struct PXAudioSystemCreateInfo_
{
    PXECSCreateInfo Info;

    PXAudioSystemAPI System;
}
PXAudioSystemCreateInfo;



PXPublic PXResult PXAPI PXAudioSystemRegisterToECS();
PXPublic PXResult PXAPI PXAudioSystemCreate(PXAudioSystem** pxAudioSystemREF, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo);
PXPublic PXResult PXAPI PXAudioSystemRelease(PXAudioSystem PXREF pxAudioSystem);

PXPrivate PXResult PXAPI PXAudioSystemDeviceCreate(PXAudioSystem PXREF pxAudioSystem);
PXPrivate PXResult PXAPI PXAudioSystemDeviceDestroy(PXAudioSystem PXREF pxAudioSystem);
PXPrivate PXResult PXAPI PXAudioSystemSourceCreate(PXAudioSystem PXREF pxAudioSystem);
PXPrivate PXResult PXAPI PXAudioSystemSourceDestroy(PXAudioSystem PXREF pxAudioSystem);
PXPrivate PXResult PXAPI PXAudioSystemStart(PXAudioSystem PXREF pxAudioSystem);
PXPrivate PXResult PXAPI PXAudioSystemRender(PXAudioSystem PXREF pxAudioSystems);

PXPublic PXResult PXAPI PXAudioSpeakerBeep(const PXI32U hz, const PXI32U time);




typedef struct PXAudioDeviceDetectInfo_
{
    PXText Name;

    PXAudioDeviceType Type;
    PXI32U InternalID;

#if OSWindows
    GUID DeviceGUID;
#endif

}
PXAudioDeviceDetectInfo;


PXPublic PXResult PXAPI PXAudioDeviceDetect(PXAudioSystem PXREF pxAudioSystem, PXAudioDeviceDetectInfo PXREF pxAudioDeviceDetectInfo);


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




PXPublic PXFAudio PXAPI PXAudioWaveGenerate(PXAudioWaveGenerateInfo* pxAudioWaveGenerateInfo);

PXPublic PXFAudio PXAPI envelope(PXFAudio t, PXFAudio attack, PXFAudio sustain, PXFAudio decay);
PXPublic void PXAPI generate_sfx(PXI16U* buffer, int samples, PXSFXParams* params);
PXPublic PXFAudio PXAPI lowpass_filter(PXFAudio input, PXFAudio* prev, PXFAudio alpha);

#endif
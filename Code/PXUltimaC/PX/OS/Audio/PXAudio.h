#ifndef PXAudioIncluded
#define PXAudioIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

#define PitchMaximum 2.9f
#define PitchMinimum 0.2f


typedef enum PXSoundPlayStyle_
{
    SoundPlayStyleOnce,
    SoundPlayStyleLoop
}
PXSoundPlayStyle;

typedef enum PXAudioSystem_
{
    PXAudioSystemInvalid,
    PXAudioSystemWindowsMultiMedia, // Windows 2000
    PXAudioSystemWindowsMIDI,
    PXAudioSystemWindowsDirectSound, // Windows XP (successor to WINMM)
    PXAudioSystemWindowsIAudio, // Windows Vista (successor to DirectSound)
    PXAudioSystemWindowsXAudio // Windows 8 (successor to IAudio)
}
PXAudioSystem;

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

typedef struct PXAudioInitializeInfo_
{
    struct PXAudio_* AudioReference;
    PXAudioSystem AudioSystem;
}
PXAudioInitializeInfo;



typedef PXActionResult(PXAPI* PXAudioInitializeFunction)(void* const audioAPI, PXAudioInitializeInfo* const pxAudioInitializeInfo);

//--------------------------------------------------------
// Device
//--------------------------------------------------------
typedef struct PXAudioDeviceAmountInfo_
{
    PXI32U DeviceInput;
    PXI32U DeviceOutput;
}
PXAudioDeviceAmountInfo;

typedef PXActionResult(PXAPI* PXAudioDeviceFetchFunction)(void* const audioAPI, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice* const pxAudioDevice);
typedef PXActionResult(PXAPI* PXAudioDeviceFetchAllFunction)(void* const audioAPI, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

typedef PXActionResult(PXAPI* PXAudioDeviceOpenFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
typedef PXActionResult(PXAPI* PXAudioDeviceCloseFunction)(void* const audioAPI, PXAudioDevice* const pxAudioDevice);
typedef PXActionResult(PXAPI* PXAudioDeviceLoadFunction)
(
    void* const audioAPI,
    PXAudioDevice* const pxAudioDevice,
    PXSound* const pxSound
);

typedef PXActionResult(PXAPI* PXAudioDeviceUnloadFunction)
(
    void* const audioAPI,
    PXAudioDevice* const pxAudioDevice,
    PXSound* const pxSound,
    const PXAudioDeviceType pxAudioDeviceType,
    const PXI32U deviceID
);








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


typedef PXActionResult (PXAPI* PXSoundDevicePropertyFunction)(PXAudioDevice* const pxAudioDevice, PXSoundDeviceProperty* const pxSoundDeviceProperty);











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


typedef struct PXAudioEffectChorus_
{
    PXF32       WetDryMix;
    PXF32       Depth;
    PXF32       Feedback;
    PXF32       Frequency;
    PXF32        Waveform;          // LFO shape; DSFXCHORUS_WAVE_xxx
    PXF32       Delay;
    PXF32        Phase;
}
PXAudioEffectChorus;

typedef struct PXAudioEffectCompressor_
{
    PXF32 Gain;
    PXF32 Attack;
    PXF32 Release;
    PXF32 Threshold;
    PXF32 Ratio;
    PXF32 Predelay;
}
PXAudioEffectCompressor;

typedef struct PXAudioEffectDistortion_
{
    PXF32 Gain;
    PXF32 Edge;
    PXF32 PostEQCenterFrequency;
    PXF32 PostEQBandwidth;
    PXF32 PreLowpassCutoff;
}
PXAudioEffectDistortion;

typedef struct PXAudioEffectEcho_
{
    PXF32 WetDryMix;
    PXF32 Feedback;
    PXF32 LeftDelay;
    PXF32 RightDelay;
    PXBool PanDelay;
}
PXAudioEffectEcho;

typedef struct PXAudioEffectFlanger_
{
    PXF32    WetDryMix;
    PXF32    Depth;
    PXF32    Feedback;
    PXF32    Frequency;
    PXI32U Waveform;
    PXF32    Delay;
    PXI32U Phase;
}
PXAudioEffectFlanger;

typedef struct PXAudioEffectGargle_
{
    PXI32U dwRateHz;    // Rate of modulation in hz
    PXI32U dwWaveShape; // DSFXGARGLE_WAVE_xxx
}
PXAudioEffectGargle;

typedef struct PXAudioEffectParamEq_
{
    PXF32 Center;
    PXF32 Bandwidth;
    PXF32 Gain;
}
PXAudioEffectParamEq;

typedef struct PXAudioEffectI3DL2Reverb_
{
    PXI32U Room;           // [-10000, 0]      default: -1000 mB
    PXI32U RoomHF;         // [-10000, 0]      default: 0 mB
    PXF32 RoomRolloffFactor; // [0.0, 10.0]      default: 0.0
    PXF32 DecayTime;         // [0.1, 20.0]      default: 1.49s
    PXF32 DecayHFRatio;      // [0.1, 2.0]       default: 0.83
    PXI32U Reflections;    // [-10000, 1000]   default: -2602 mB
    PXF32 ReflectionsDelay;  // [0.0, 0.3]       default: 0.007 s
    PXI32U Reverb;         // [-10000, 2000]   default: 200 mB
    PXF32 ReverbDelay;       // [0.0, 0.1]       default: 0.011 s
    PXF32 Diffusion;         // [0.0, 100.0]     default: 100.0 %
    PXF32 Density;           // [0.0, 100.0]     default: 100.0 %
    PXF32 HFReference;       // [20.0, 20000.0]  default: 5000.0 Hz
}
PXAudioEffectI3DL2Reverb;

typedef struct PXAudioEffectWavesReverb_
{
    PXF32 InGain;                // [-96.0,0.0]            default: 0.0 dB
    PXF32 ReverbMix;             // [-96.0,0.0]            default: 0.0 db
    PXF32 ReverbTime;            // [0.001,3000.0]         default: 1000.0 ms
    PXF32 HighFreqRTRatio;       // [0.001,0.999]          default: 0.001
}
PXAudioEffectWavesReverb;


typedef struct PXAudioEffect_
{
    union
    {
        PXAudioEffectChorus Chorus;
        PXAudioEffectCompressor Compressor;
        PXAudioEffectDistortion Distortion;
        PXAudioEffectEcho Echo;
        PXAudioEffectFlanger Flanger;
        PXAudioEffectGargle Gargle;
        PXAudioEffectParamEq ParamEq;
        PXAudioEffectI3DL2Reverb I3DL2Reverb;
        PXAudioEffectWavesReverb WavesReverb;
    };

    PXAudioEffectType Type;
    PXBool Enable;
    PXBool Fetch;
}
PXAudioEffect;


typedef PXActionResult(PXAPI* PXAudioDeviceEffectUpdate)(void* const audioAPI, PXAudioDevice* const pxAudioDevice, PXAudioEffect* const pxAudioEffect);

typedef struct PXAudioMultiMedia_
{
    PXLibrary MultiMediaLibrary;

    void* WaveOutGetDevCapsA;
    void* WaveOutGetDevCapsW;
    void* WaveOutGetNumDevs;
    void* WaveOutGetVolume;
    void* WaveOutSetVolume;
    void* WaveOutGetErrorA;
    void* WaveOutGetErrorW;
    void* WaveOutOpen;
    void* WaveOutClose;
    void* WaveOutPrepareHeader;
    void* WaveOutUnprepareHeader;
    void* WaveOutWrite;
    void* WaveOutPause;
    void* WaveOutRestart;
    void* WaveOutReset;
    void* WaveOutBreakLoop;
    void* WaveOutGetPosition;
    void* WaveOutGetPitch;
    void* WaveOutSetPitch;
    void* WaveOutGetPlaybackRate;
    void* WaveOutSetPlaybackRate;
    void* WaveOutGetID;
    void* WaveOutMessage;
    void* WaveInGetNumDevs;
    void* WaveInGetDevCapsA;
    void* WaveInGetDevCapsW;
    void* WaveInGetErrorA;
    void* WaveInGetErrorW;
    void* WaveInOpen;
    void* WaveInClose;
    void* WaveInPrepareHeader;
    void* WaveInUnprepareHeader;
    void* WaveInAddBuffer;
    void* WaveInStart;
    void* WaveInStop;
    void* WaveInReset;
    void* WaveInGetPosition;
    void* WaveInGetID;
    void* WaveInMessage;
    void* MidiOutGetNumDevs;
    void* MidiStreamOpen;
    void* MidiStreamClose;
    void* MidiStreamProperty;
    void* MidiStreamPosition;
    void* MidiStreamOut;
    void* MidiStreamPause;
    void* MidiStreamRestart;
    void* MidiStreamStop;
    void* MidiConnect;
    void* MidiDisconnect;
    void* MidiOutGetDevCapsA;
    void* MidiOutGetDevCapsW;
    void* MidiOutGetVolume;
    void* MidiOutSetVolume;
    void* MidiOutGetErrorA;
    void* MidiOutGetErrorW;
    void* MidiOutOpen;
    void* MidiOutClose;
    void* MidiOutPrepareHeader;
    void* MidiOutUnprepareHeader;
    void* MidiOutShortMsg;
    void* MidiOutLongMsg;
    void* MidiOutReset;
    void* MidiOutCachePatches;
    void* MidiOutCacheDrumPatches;
    void* MidiOutGetID;
    void* MidiOutMessage;
    void* MidiInGetNumDevs;
    void* MidiInGetDevCapsA;
    void* MidiInGetDevCapsW;
    void* MidiInGetErrorA;
    void* MidiInGetErrorW;
    void* MidiInOpen;
    void* MidiInClose;
    void* MidiInPrepareHeader;
    void* MidiInUnprepareHeader;
    void* MidiInAddBuffer;
    void* MidiInStart;
    void* MidiInStop;
    void* MidiInReset;
    void* MidiInGetID;
    void* MidiInMessage;
    void* AuxGetNumDevs;
    void* AuxGetDevCapsA;
    void* AuxGetDevCapsW;
    void* AuxSetVolume;
    void* AuxGetVolume;
    void* AuxOutMessage;
    void* MixerGetNumDevs;
    void* MixerGetDevCapsA;
    void* MixerGetDevCapsW;
    void* MixerOpen;
    void* MixerClose;
    void* MixerMessage;
    void* MixerGetLineInfoA;
    void* MixerGetLineInfoW;
    void* MixerGetID;
    void* MixerGetLineControlsA;
    void* MixerGetLineControlsW;
    void* MixerGetControlDetailsA;
    void* MixerGetControlDetailsW;
}
PXAudioMultiMedia;

// Direct Sound
typedef struct PXAudioDirectSound_
{
    PXLibrary DirectSoundLibrary;

    void* DirectSoundInterface; // LPDIRECTSOUND8

    void* SoundCreate;
    void* SoundEnumerateA;
    void* SoundEnumerateW;
    void* SoundCaptureCreate;
    void* SoundCaptureEnumerateA;
    void* SoundCaptureEnumerateW;
    void* ClassObjectGet;
    void* CanUnloadNow;

    void* SoundCreate8;
    void* SoundCaptureCreate8;
}
PXAudioDirectSound;

typedef struct PXAudioXSystem_
{
    PXLibrary XSystemLibrary;

    struct IXAudio2* XAudioInterface;
    void* XAudioMasterVoice;
}
PXAudioXSystem;

// General audio wrapper around system solutions
typedef struct PXAudio_
{
    union
    {
        PXAudioDirectSound DirectSound;
        PXAudioMultiMedia MultiMedia;
        PXAudioXSystem XSystem;
    };

    void* SystemReference;
    PXAudioSystem System;


    PXI32U DeviceOutputAmount;
    PXI32U DeviceInputAmount;

    PXAudioDevice* DeviceOutputList;


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
PXAudio;

PXPublic PXResult PXAPI PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem);
PXPublic PXResult PXAPI PXAudioRelease(PXAudio* const pxAudio);

PXPublic void PXAPI PXAudioSpeakerBeep(const PXI32U hz, const PXI32U time);









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

typedef struct PXSFXWeather_
{
    // Wind
    float wind_intensity;     // 0.0 to 1.0
    float wind_variation;     // 0.0 to 1.0 (how gusty it is)

    // Rain
    float rain_density;       // drops per second
    float rain_loudness;      // 0.0 to 1.0

    // Thunder
    float thunder_freq;       // average rumbles per minute
    float thunder_loudness;   // 0.0 to 1.0
    float thunder_rumble;     // 0.0 to 1.0 (how long it rumbles)

    int SampleRate;
    float PI;
} 
PXSFXWeather;


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




PXPublic float PXAPI PXAudioWaveGenerate(PXAudioWaveGenerateInfo* pxAudioWaveGenerateInfo);

PXPublic float PXAPI envelope(float t, float attack, float sustain, float decay);
PXPublic void PXAPI generate_sfx(PXI16U* buffer, int samples, PXSFXParams* params);
PXPublic float PXAPI lowpass_filter(float input, float* prev, float alpha);
PXPublic void PXAPI add_raindrop(PXI16U* buffer, int sampleRate, int pos, int length, float freq, float amp);
PXPublic void PXAPI add_thunder(PXI16U* buffer, int sampleRate, int pos, int length, float amp, float rumble);
PXPublic void PXAPI generate_weather_sfx(PXI16U* buffer, int samples, PXSFXWeather* params);



/*

typedef enum PXAudioSystemSoundBeepType_
{
    PXAudioSystemSoundBeepTypeSimple,// A simple beep.If the sound card is not available, the sound is generated using the speaker.
    PXAudioSystemSoundBeepTypexxxxxxxxxxxx,
    PXAudioSystemSoundBeepType, // A simple beep.If the sound card is not available, the sound is generated using the speaker.
    PXAudioSystemSoundBeepType, // See MB_ICONINFORMATION.
    PXAudioSystemSoundBeepType, // See MB_ICONWARNING.
    PXAudioSystemSoundBeepType, // The sound specified as the Windows Critical Stop sound.
    PXAudioSystemSoundBeepType, // See MB_ICONERROR.
    PXAudioSystemSoundBeepType, // The sound specified as the Windows Asterisk sound.
    PXAudioSystemSoundBeepType, // The sound specified as the Windows Question sound.
    PXAudioSystemSoundBeepType, // See MB_ICONERROR.
    PXAudioSystemSoundBeepType, // The sound specified as the Windows Exclamation sound.
    PXAudioSystemSoundBeepType, // The sound specified as the Windows Default Beep sound.

};

PXPublic void PXAPI PXAudioSystemSoundBeep(const PXI32U hz,);
*/

#endif

#pragma once

#ifndef PXAudioDeviceIncluded
#define PXAudioDeviceIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/Resource/Sound/PXSound.h>


#define PXAudioDeviceNameSize 64
#define PXAudioFX3DPositionEnable    (1 <<  0)
#define PXAudioFXPlaySpeedEnable    (1 <<  1)
#define PXAudioFXVolumeMONOEnable    (1 <<  2)
#define PXAudioFXVolumeSTEREOEnable    (1 <<  3)
#define PXAudioFXEnable                (1 <<  4)
#define PXAudioFXGargle                (1 <<  5) // modulates the amplitude of the signal
#define PXAudioFXChorus                (1 <<  6) // Mixing of similar pitches
#define PXAudioFXFlanger            (1 <<  7) // Delay with smal distortion over time, simpar effect as "Chorus"
#define PXAudioFXEcho                (1 <<  8) // Delay and Repeat
#define PXAudioFXDistortion            (1 <<  9)
#define PXAudioFXCompressor            (1 << 10)
#define PXAudioFXParamEq            (1 << 11)
#define PXAudioFXWavesReverb        (1 << 12)
#define PXAudioFXI3DL2Reverb        (1 << 13)

typedef enum PXAudioDeviceRole_
{
    PXAudioDeviceRoleInvalid,
    PXAudioDeviceRoleNotDefault,
    PXAudioDeviceRoleDefaultConsole,
    PXAudioDeviceRoleDefaultMultimedia,
    PXAudioDeviceRoleDefaultCommunications,
    PXAudioDeviceRoleDefaultGame,
    PXAudioDeviceRoleGlobalDefault
}
PXAudioDeviceRole;

typedef enum PXAudioDeviceType_
{
    PXAudioDeviceTypeInvalid,
    PXAudioDeviceTypeInput,
    PXAudioDeviceTypeOutput
}
PXAudioDeviceType;

typedef struct PXAudioDevice_ PXAudioDevice;

typedef PXResult(PXAPI* PXAudioDeviceFetchFunction)(void PXREF audioAPI, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);
typedef PXResult(PXAPI* PXAudioDeviceFetchAllFunction)(void PXREF audioAPI, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount);

typedef PXResult(PXAPI* PXAudioDeviceOpenFunction)(void PXREF audioAPI, PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
typedef PXResult(PXAPI* PXAudioDeviceCloseFunction)(void PXREF audioAPI, PXAudioDevice PXREF pxAudioDevice);
typedef PXResult(PXAPI* PXAudioDeviceLoadFunction)
(
    void PXREF audioAPI,
    PXAudioDevice PXREF pxAudioDevice,
    PXSound PXREF pxSound
    );

typedef PXResult(PXAPI* PXAudioDeviceUnloadFunction)
(
    void PXREF audioAPI,
    PXAudioDevice PXREF pxAudioDevice,
    PXSound PXREF pxSound,
    const PXAudioDeviceType pxAudioDeviceType,
    const PXI32U deviceID
    );

typedef struct PXAudioDeviceCreateInfo_
{
    PXECSCreateInfo Info;
}
PXAudioDeviceCreateInfo;


typedef struct PXAudioDevice_
{
    PXECSInfo Info;

    union
    {
        void* GenericHandle;
        struct HWAVEOUT__* WaveOutHandle; // HWAVEOUT
        struct HWAVEIN__* WaveInHandle; // HWAVEIN
    };


#if OSWindows
    union
    {
        UINT DeviceID; // Windows MultiMedia
        GUID DeviceGUID; // DirectSound Device Identifier GUID
    };
#endif


    PXAudioDeviceRole Role;
    PXAudioDeviceType Type;
    PXBool IsRunning;

    PXI32U FXSettingFlagList;


    PXF32 Pitch; // [0.5 - 2.0]
    PXF32 Volume;
    PXF32 Position[3];
    PXF32 Velocity[3];
    PXBool Looping;




    //-------------------------------------------------
    // General Shared Info
    //-------------------------------------------------
    char DeviceName[PXAudioDeviceNameSize];
    PXI16U ManufacturerID;
    PXI16U ProductID;
    PXI8U DriverVersionMajor;
    PXI8U DriverVersionMinor;
    PXI32U SupportFlags;
    //-------------------------------------------------


    PXI32U FormatSupportFlags;

    PXI16U FormatTag;         // format type
    PXI16U Channels;          // number of channels (i.e. mono, stereo...)
    PXI32U SamplesPerSecound;     // sample rate
    PXI32U AverageBytesPerSecound;    // for buffer estimation
    PXI16U BlockAlignSize;        // block size of data
    PXI16U BitsPerSample;     // number of bits per sample of mono data

    //-------------------------------------------------
    // Used By MIDI
    //-------------------------------------------------
    PXI16U Technology;           // type of device
    PXI16U Voices;               // # of voices (internal synth only)
    PXI16U Notes;                // max # of notes (internal synth only)
    PXI16U ChannelMask;          // channels used (internal synth only)
    //-------------------------------------------------

    union
    {
        PXI16U wValidBitsPerSample; // Valid bits in each sample container
        PXI16U wSamplesPerBlock;    // Samples per block of audio data; valid
        // if wBitsPerSample=0 (but rarely used).
        PXI16U wReserved;           // Zero if neither case above applies.
    } Samples;
    PXI32U dwChannelMask;          // Positions of the audio channels


    struct IDirectSoundBuffer* SoundBuffer;
    struct IDirectSound3DBuffer8* Buffer3DInterface;
    struct IDirectSound3DListener8* Listen3DInterface;
    void* FXGargle;
    void* FXChorus;
    void* FXFlanger;
    void* FXEcho;
    void* FXDistortion;
    void* FXCompressor;
    void* FXParamEq;
    void* FXWavesReverb;
    void* FXI3DL2Reverb;
}
PXAudioDevice;

PXPublic PXResult PXAPI PXAudioDeviceRegisterToECS();
PXPublic PXResult PXAPI PXAudioDeviceCreate(PXAudioDevice** pxAudioDeviceREF, PXAudioDeviceCreateInfo PXREF pxAudioDeviceCreateInfo);
PXPublic PXResult PXAPI PXAudioDeviceRelase(PXAudioDevice PXREF pxAudioDevice);

#endif
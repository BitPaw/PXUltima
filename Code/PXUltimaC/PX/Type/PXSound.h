#pragma once


#ifndef PXSoundIncluded
#define PXSoundIncluded


#define PXSoundBehaviourLoop (1<<0)

#include <PX/Engine/PXResource.h>

typedef struct PXSound_
{
    PXECSInfo Info;

    PXBuffer Data;

    //void* BaseObject; ???

    PXI16U NumerOfChannels;

    PXI32U SampleRate;
    PXI16U BitsPerSample; // 16-bit, 32-bit

    PXI32U ChunkSize;
    PXI16U AudioFormat;
}
PXSound;

typedef struct PXSoundCreateInfo_
{
    PXECSCreateInfo Info;
    PXBool SoundLoop;
}
PXSoundCreateInfo;

PXPublic void PXAPI PXSoundRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSoundCreate(PXSound** pxSound, PXSoundCreateInfo PXREF pxSoundCreateInfo);

PXPublic PXResult PXAPI PXSoundStereoToMono(PXSound PXREF pxSound);

PXPublic PXSize PXAPI PXSoundSamplesAmount(const PXSound PXREF pxSound);
PXPublic PXSize PXAPI PXSoundFramesAmount(const PXSound PXREF pxSound);
PXPublic PXSize PXAPI PXSoundBytePerSample(const PXSound PXREF pxSound);
PXPublic PXSize PXAPI PXSoundBlockAllign(const PXSound PXREF pxSound);
PXPublic PXSize PXAPI PXSoundBytesPerSecound(const PXSound PXREF pxSound);

PXPublic void PXAPI FillBufferWithElectricHum(const int sampleRate, short* samples, int sampleCount, float frequency);

PXPublic void PXAPI FillBufferWithRain(short* samples, int sampleCount);

PXPublic void PXAPI FillBufferWithThunder(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithGravelFootsteps(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithConcreteFootsteps(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithFireCrackle(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithWind(short* samples, int sampleCount);

PXPublic void PXAPI FillBufferWithPerlinNoise(short* samples, int sampleCount);

PXPublic void PXAPI FillBufferWithIceCracks(short* samples, int sampleCount);

PXPublic void PXAPI FillBufferWithScannerBeep(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithMagneticWobble(short* samples, int sampleCount);

PXPublic void PXAPI FillBufferWithBubbles(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithZombieMoan(short* samples, int sampleCount);


PXPublic void PXAPI FillBufferWithExplosion(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithPsychoDrone(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithSteamHiss(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithAlienSignal(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithElectricArc(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithMeditationBell(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithBrainwavePulse(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithGeigerClicks(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithFrozenWind(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithMysticChimes(short* samples, int sampleCount);
PXPublic void PXAPI FillBufferWithLaserZap(short* samples, int sampleCount);


PXPublic void PXAPI FillBufferWithWindAndRain(short* samples, int sampleCount, float windStrength, float rainStrength);
PXPublic void PXAPI FillBufferWithWeather(short* samples, int count, float rain, float wind, float thunder);

/*
Thunder    Combine bursts of low - frequency noise + decay envelope + randomization
Rain    Mix many fast random blips(white noise bursts, modulated by amplitude curves)
Footsteps: Gravel    Randomized grain / burst - like noise with short envelopes
Footsteps : Wood    Filtered impulse sound + slight reverb
Wind    Pink noise with fluctuating low - pass filter cutoff
Fire crackling    Random popping sine waves with frequency jitter
Glass breaking    String of rapidly rising sine bursts + shatter - like white noise burst
*/

#endif
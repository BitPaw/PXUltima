#pragma once


#ifndef PXSoundIncluded
#define PXSoundIncluded


#define PXSoundBehaviourLoop (1<<0)

#include <PX/Engine/PXResource.h>

typedef struct PXSound_
{
    PXECSInfo Info;

    void* BaseObject;
    void* Data;

    PXI32U SampleRate;
    PXI32U ByteRate;
    PXI16U NumerOfChannels;
    PXI16U BlockAllign;
    PXI16U BitsPerSample;

    PXSize DataSize;
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

PXPublic PXResult PXAPI PXSoundRegisterToECS();
PXPublic PXResult PXAPI PXSoundCreate(PXSound** pxSound, PXSoundCreateInfo PXREF pxSoundCreateInfo);





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
Thunder	Combine bursts of low - frequency noise + decay envelope + randomization
Rain	Mix many fast random blips(white noise bursts, modulated by amplitude curves)
Footsteps: Gravel	Randomized grain / burst - like noise with short envelopes
Footsteps : Wood	Filtered impulse sound + slight reverb
Wind	Pink noise with fluctuating low - pass filter cutoff
Fire crackling	Random popping sine waves with frequency jitter
Glass breaking	String of rapidly rising sine bursts + shatter - like white noise burst
*/

#endif
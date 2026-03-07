#pragma once

#ifndef PXFilterWeatherIncluded
#define PXFilterWeatherIncluded
#include <PX/Engine/ECS/System/AudioSystem/PXAudioSystem.h>

typedef struct PXFilterWeather_
{
    // Wind
    PXFAudio wind_intensity;     // 0.0 to 1.0
    PXFAudio wind_variation;     // 0.0 to 1.0 (how gusty it is)

    // Rain
    PXFAudio rain_density;       // drops per second
    PXFAudio rain_loudness;      // 0.0 to 1.0

    // Thunder
    PXFAudio thunder_freq;       // average rumbles per minute
    PXFAudio thunder_loudness;   // 0.0 to 1.0
    PXFAudio thunder_rumble;     // 0.0 to 1.0 (how long it rumbles)
}
PXFilterWeather;


PXPublic void PXAPI PXFilterWeatherProcess(PXFilterWeather PXREF pxFilterWeather, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

PXPublic void PXAPI PXFilterWeatherRaindrop(PXAudioFilterBuffer PXREF pxAudioFilterBuffer, int pos, int length, float freq, float amp);
PXPublic void PXAPI PXFilterWeatherThunder(PXAudioFilterBuffer PXREF pxAudioFilterBuffer, int pos, int length, float amp, float rumble);


#endif
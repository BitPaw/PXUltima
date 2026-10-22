#pragma once

#ifndef PXFilterWeatherIncluded
#define PXFilterWeatherIncluded
#include <PX/Engine//PXEngineAudio.h>

typedef struct PXFilterWeather_
{
    // Wind
    PXF32 wind_intensity;     // 0.0 to 1.0
    PXF32 wind_variation;     // 0.0 to 1.0 (how gusty it is)

    // Rain
    PXF32 rain_density;       // drops per second
    PXF32 rain_loudness;      // 0.0 to 1.0

    // Thunder
    PXF32 thunder_freq;       // average rumbles per minute
    PXF32 thunder_loudness;   // 0.0 to 1.0
    PXF32 thunder_rumble;     // 0.0 to 1.0 (how long it rumbles)
}
PXFilterWeather;


PXPublic void PXAPI PXFilterWeatherProcess(PXFilterWeather PXREF pxFilterWeather, PXAudioFilterBuffer PXREF pxAudioFilterBuffer);

PXPublic void PXAPI PXFilterWeatherRaindrop(PXAudioFilterBuffer PXREF pxAudioFilterBuffer, int pos, int length, float freq, float amp);
PXPublic void PXAPI PXFilterWeatherThunder(PXAudioFilterBuffer PXREF pxAudioFilterBuffer, int pos, int length, float amp, float rumble);


#endif
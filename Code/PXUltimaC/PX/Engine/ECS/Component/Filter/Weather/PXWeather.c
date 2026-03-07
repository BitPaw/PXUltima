#include "PXWeather.h"
#include <PX/Math/PXMath.h>

#include <math.h>
#include <stdlib.h>

void PXAPI PXFilterWeatherProcess(PXFilterWeather PXREF pxFilterWeather, PXAudioFilterBuffer PXREF pxAudioFilterBuffer)
{
    PXFAudio wind_prev = 0.0f;
    PXFAudio thunder_timer = 0.0f;
    PXFAudio thunder_interval = 60.0f / fmaxf(pxFilterWeather->thunder_freq, 0.01f);

    for(PXSize i = 0; i < pxAudioFilterBuffer->Samples; i++)
    {
        PXFAudio t = (PXFAudio)i / (PXFAudio)pxAudioFilterBuffer->SampleRate;

        // Wind: filtered white noise with slow modulation
        PXFAudio gust = sinf(2.0f * PXMathPI * 0.1f * t) * pxFilterWeather->wind_variation;

        PXAudioWaveGenerateInfo pxAudioWaveGenerateInfo;
        pxAudioWaveGenerateInfo.WaveType = PXAudioWaveTypeNoiseWhite;

        PXFAudio noise = PXAudioWaveGenerate(&pxAudioWaveGenerateInfo);
        PXFAudio wind = lowpass_filter(noise, &wind_prev, 0.01f);
        PXFAudio wind_sample = wind * (pxFilterWeather->wind_intensity + gust) * 0.3f;

        // Rain: random short sine bursts
        if((rand() / (PXFAudio)RAND_MAX) < (pxFilterWeather->rain_density / pxAudioFilterBuffer->SampleRate))
        {
            int drop_len = 100 + rand() % 100;
            PXFAudio freq = 600.0f + rand() % 800;
            PXFAudio amp = pxFilterWeather->rain_loudness * (0.2f + ((PXFAudio)rand() / RAND_MAX) * 0.3f);
            PXFilterWeatherRaindrop(pxAudioFilterBuffer, i, drop_len, freq, amp);
        }

        // Thunder: low-frequency filtered noise bursts
        thunder_timer += 1.0f / pxAudioFilterBuffer->SampleRate;

        if(thunder_timer >= thunder_interval)
        {
            thunder_timer = 0.0f;
            int thunder_len = pxAudioFilterBuffer->SampleRate * (1.0f + pxFilterWeather->thunder_rumble);
            PXFAudio amp = pxFilterWeather->thunder_loudness * (0.5f + ((PXFAudio)rand() / RAND_MAX) * 0.5f);
            PXFilterWeatherThunder(pxAudioFilterBuffer, i, thunder_len, amp, 1.5f + pxFilterWeather->thunder_rumble);
        }

        // Mix wind into buffer
        pxAudioFilterBuffer->Buffer.F64[i] += (PXI16U)(wind_sample * 32767);
    }
}

void PXAPI PXFilterWeatherRaindrop(PXAudioFilterBuffer PXREF pxAudioFilterBuffer, int pos, int length, float freq, float amp)
{
    for(int i = 0; i < length; i++)
    {
        if(pos + i >= pxAudioFilterBuffer->SampleRate * 10)
            break;

        PXFAudio env = 1.0f - (PXFAudio)i / (PXFAudio)length;
        PXFAudio sample = amp * env * sin(2.0 * PXMathPI * freq * i / pxAudioFilterBuffer->SampleRate);

        pxAudioFilterBuffer->Buffer.F64[pos + i] += (PXI16U)(sample * 32767);
    }
}

void PXAPI PXFilterWeatherThunder(PXAudioFilterBuffer PXREF pxAudioFilterBuffer, int pos, int length, float amp, float rumble)
{
    for(int i = 0; i < length; i++)
    {
        if(pos + i >= pxAudioFilterBuffer->SampleRate * 10)
            break;

        PXFAudio t = (PXFAudio)i / (PXFAudio)pxAudioFilterBuffer->SampleRate;
        PXFAudio decay = exp(-t * rumble);

        PXAudioWaveGenerateInfo pxAudioWaveGenerateInfo;
        pxAudioWaveGenerateInfo.WaveType = PXAudioWaveTypeNoiseWhite;

        float sample = amp * decay * PXAudioWaveGenerate(&pxAudioWaveGenerateInfo);

        pxAudioFilterBuffer->Buffer.F64[pos + i] += (PXI16U)(sample * 32767);
    }
}
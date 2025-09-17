#include "PXAudio.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Console/PXConsole.h>

//#include "PXDirectSound/PXDirectSound.h"
//#include "PXMultiMedia/PXMultiMedia.h"
//#include "PXIAudio/PXIAudio.h"
//#include "PXXAudio/PXXAudio.h"
//#include "PXAudioMIDI/PXAudioMIDI.h"

PXResult PXAPI  PXAudioInitialize(PXAudio* const pxAudio, const PXAudioSystem pxAudioSystem)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Audio",
        "Initialize",
        "---Start---"
    );
#endif

    PXClear(PXAudio, pxAudio);

    pxAudio->System = pxAudioSystem;

    switch(pxAudioSystem)
    {
#if OSWindows && 0
        case PXAudioSystemWindowsMIDI:
        {
            //pxAudio->Initialize = PXMIDIInitialize;
            pxAudio->SystemReference = &pxAudio->MultiMedia;
            break;
        }
        case PXAudioSystemWindowsMultiMedia:
        {
           // pxAudio->Initialize = (PXAudioInitializeFunction)PXMultiMediaInitialize;
            pxAudio->SystemReference = &pxAudio->MultiMedia;
            break;
        }
        case PXAudioSystemWindowsDirectSound:
        {
            //pxAudio->Initialize = (PXAudioInitializeFunction)PXDirectSoundInitialize;
            pxAudio->SystemReference = &pxAudio->DirectSound;
            break;
        }
        case PXAudioSystemWindowsIAudio:
        {
            //pxAudio->Initialize = (PXAudioInitializeFunction)PXIAudioInitialize;
            pxAudio->SystemReference = &pxAudio->XSystem;
            break;
        }
        case PXAudioSystemWindowsXAudio:
        {
            //pxAudio->Initialize = (PXAudioInitializeFunction)PXXAudioInitialize;
            //pxAudio->SystemReference = &pxAudio->XSystem;
            break;
        }
#endif
        default:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "Audio",
                "Initialize",
                "There is no audio system"
            );
#endif
            return PXActionRefusedArgumentInvalid;

        }
    }

    // Initialize
    {
        PXAudioInitializeInfo pxAudioInitializeInfo;
        pxAudioInitializeInfo.AudioSystem = pxAudioSystem;
        pxAudioInitializeInfo.AudioReference = pxAudio;

        const PXActionResult initializeResult = pxAudio->Initialize(pxAudio->SystemReference, &pxAudioInitializeInfo);

        if(PXActionSuccessful != initializeResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "Audio",
                "Initialize",
                "Failed!"
            );
#endif
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Audio",
            "Initialize",
            "Success"
        );
#endif

    }



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Audio",
        "Initialize",
        "---Done---"
    );
#endif


    return PXActionSuccessful;
}

PXResult PXAPI  PXAudioRelease(PXAudio* const pxAudio)
{
    return PXActionSuccessful;
}

void PXAPI PXAudioSpeakerBeep(const PXI32U hz, const PXI32U time)
{
#if OSUnix
#elif OSWindows
    const PXBool success = Beep(hz, time);
#endif
}

#include <PX/Math/PXMath.h>
#include <stdlib.h>
#include <math.h>

float PXAPI PXAudioWaveGenerate(PXAudioWaveGenerateInfo* pxAudioWaveGenerateInfo)
{
    float period = 1.0f / pxAudioWaveGenerateInfo->Hz;
    float p = fmodf(pxAudioWaveGenerateInfo->X, period); // Normalize phase to [0,1)

    switch(pxAudioWaveGenerateInfo->WaveType)
    {
        case PXAudioWaveTypeSINE:
        {
            // 0.3f * sinf(2.0f * PXMathConstantPI * pxAudioWaveGenerateInfo.Phase * pxAudioWaveGenerateInfo.T);

            return pxAudioWaveGenerateInfo->Amplitude * sinf
            (
                2.0f *
                PXMathConstantPI *
                pxAudioWaveGenerateInfo->Hz *
                pxAudioWaveGenerateInfo->X
            );

            break;
        }
        case PXAudioWaveTypeSQUARE:
        {
            pxAudioWaveGenerateInfo->PWM = 0.5f;
        }
        case PXAudioWaveTypePULSE:
        {
            PXBool isHigh = p > (period * pxAudioWaveGenerateInfo->PWM);

            if(isHigh)
            {
                return pxAudioWaveGenerateInfo->Amplitude;
            }
            else
            {
                return -pxAudioWaveGenerateInfo->Amplitude;
            }
            break;
        }
        case PXAudioWaveTypeTRIANGLE:
        {
            // y = (2 * A / P) * (P - abs((x % (2 * P)) - P))

          //  int P = pxAudioWaveGenerateInfo->Phase / 2.0f;
            float t = pxAudioWaveGenerateInfo->X;
     
            float amplitude = pxAudioWaveGenerateInfo->Amplitude;

            return (2.0f * amplitude / PXMathConstantPI) * asinf(sinf(2.0f * PXMathConstantPI * t / period));

         

          //  return (4.0f * fabsf(pxAudioWaveGenerateInfo->Duty * p - 0.5f) - 1.0f);

            // return (4.0f * fabsf(pxAudioWaveGenerateInfo->Duty * p - 0.5f) - 1.0f);
        } 
        case PXAudioWaveTypeSAWTOOTH:
            return 2.0f * p - 1.0f;

        case PXAudioWaveTypeREVERSE_SAW:
            return 1.0f - 2.0f * p;

        case PXAudioWaveTypeNoiseWhite:
        {
            return 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
        }
        case PXAudioWaveTypeNoiseGray:
        {
            break;
        }
        case PXAudioWaveTypeNoisePink:
        {
            break;
        }
        default:
            return 0.0f;
    }
}


float PXAPI envelope(float t, float attack, float sustain, float decay) 
{
    if(t < attack) return t / attack;
    else if(t < attack + sustain) return 1.0f;
    else if(t < attack + sustain + decay) return 1.0f - (t - attack - sustain) / decay;
    else return 0.0f;
}

void PXAPI generate_sfx(PXI16U* buffer, int samples, PXSFXParams* params)
{
    float phase = 0.0f;
    float freq = params->base_freq;
    float time = 0.0f;
    float dt = 1.0f / params->SampleRate;

    for(int i = 0; i < samples; i++) 
    {
        float env = envelope(time, params->attack, params->sustain, params->decay);
        float vibrato = sinf(2.0f * params->PI * params->vibrato_speed * time) * params->vibrato_depth;
        float current_freq = freq + vibrato;
        phase += current_freq * dt;


        PXAudioWaveGenerateInfo pxAudioWaveGenerateInfo;
        pxAudioWaveGenerateInfo.WaveType = params->WaveType;
        pxAudioWaveGenerateInfo.Hz = phase;
        pxAudioWaveGenerateInfo.PWM = 1;
        pxAudioWaveGenerateInfo.Amplitude = 1;
        pxAudioWaveGenerateInfo.X = 1;

        float sample = PXAudioWaveGenerate(&pxAudioWaveGenerateInfo) * env * params->volume;
        
        
        buffer[i] = (PXI16U)(sample * 32767);
        freq += params->freq_slide * dt;
        time += dt;
    }
}

float PXAPI lowpass_filter(float input, float* prev, float alpha)
{
    *prev = alpha * input + (1.0f - alpha) * (*prev);
    return *prev;
}

void PXAPI add_raindrop(PXI16U* buffer, int sampleRate, int pos, int length, float freq, float amp)
{
    for(int i = 0; i < length; i++) 
    {
        if(pos + i >= sampleRate * 10)
            break;

        float env = 1.0f - (float)i / length;
        float sample = amp * env * sinf(2.0f * PXMathConstantPI * freq * i / sampleRate);
        buffer[pos + i] += (PXI16U)(sample * 32767);
    }
}

void PXAPI add_thunder(PXI16U* buffer, int sampleRate, int pos, int length, float amp, float rumble)
{
    for(int i = 0; i < length; i++) 
    {       
        if(pos + i >= sampleRate * 10)
            break;

        float t = (float)i / sampleRate;
        float decay = expf(-t * rumble);

        PXAudioWaveGenerateInfo pxAudioWaveGenerateInfo;
        pxAudioWaveGenerateInfo.WaveType = PXAudioWaveTypeNoiseWhite;

        float sample = amp * decay * PXAudioWaveGenerate(&pxAudioWaveGenerateInfo);
        buffer[pos + i] += (PXI16U)(sample * 32767);
    }
}

void PXAPI generate_weather_sfx(PXI16U* buffer, int samples, PXSFXWeather* params)
{
    float wind_prev = 0.0f;
    float thunder_timer = 0.0f;
    float thunder_interval = 60.0f / fmaxf(params->thunder_freq, 0.01f);

    for(int i = 0; i < samples; i++) 
    {
        float t = (float)i / params->SampleRate;

        // Wind: filtered white noise with slow modulation
        float gust = sinf(2.0f * params->PI * 0.1f * t) * params->wind_variation;

        PXAudioWaveGenerateInfo pxAudioWaveGenerateInfo;
        pxAudioWaveGenerateInfo.WaveType = PXAudioWaveTypeNoiseWhite;

        float noise = PXAudioWaveGenerate(&pxAudioWaveGenerateInfo);
        float wind = lowpass_filter(noise, &wind_prev, 0.01f);
        float wind_sample = wind * (params->wind_intensity + gust) * 0.3f;

        // Rain: random short sine bursts
        if((rand() / (float)RAND_MAX) < (params->rain_density / params->SampleRate))
        {
            int drop_len = 100 + rand() % 100;
            float freq = 600.0f + rand() % 800;
            float amp = params->rain_loudness * (0.2f + ((float)rand() / RAND_MAX) * 0.3f);
            add_raindrop(buffer, params->SampleRate, i, drop_len, freq, amp);
        }

        // Thunder: low-frequency filtered noise bursts
        thunder_timer += 1.0f / params->SampleRate;

        if(thunder_timer >= thunder_interval) 
        {
            thunder_timer = 0.0f;
            int thunder_len = params->SampleRate * (1.0f + params->thunder_rumble);
            float amp = params->thunder_loudness * (0.5f + ((float)rand() / RAND_MAX) * 0.5f);
            add_thunder(buffer, params->SampleRate, i, thunder_len, amp, 1.5f + params->thunder_rumble);
        }

        // Mix wind into buffer
        buffer[i] += (PXI16U)(wind_sample * 32767);
    }
}





/*
printf
    (
        "+----------------+--------+\n"
        "| Type           : %5i\n"
        "| DeviceID       : %5i\n"
        "| ManufacturerID : %5i\n"
        "| ProductID      : %5i\n"
        "| DriverVersion  : Major:%i, Minor:%i\n"
        "| ProductName    : %ls\n"
        "| Formats        : %i\n"
        "| Channels       : %i\n"
        "| Support        : %i\n"
        "+----------------+--------+\n",
        audioDeviceCapabilities->Type,
        audioDeviceCapabilities->DeviceID,
        audioDeviceCapabilities->ManufacturerID,
        audioDeviceCapabilities->ProductID,
        audioDeviceCapabilities->DriverVersionMajor,
        audioDeviceCapabilities->DriverVersionMinor,
        audioDeviceCapabilities->ProductName,
        audioDeviceCapabilities->dwFormats,
        audioDeviceCapabilities->Channels,
        audioDeviceCapabilities->dwSupport
    );
*/

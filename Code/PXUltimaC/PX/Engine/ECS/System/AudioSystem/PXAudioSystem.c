#include "PXAudioSystem.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Console/PXConsole.h>

//#include "PXDirectSound/PXDirectSound.h"
//#include "PXMultiMedia/PXMultiMedia.h"
//#include "PXIAudio/PXIAudio.h"
//#include "PXXAudio/PXXAudio.h"
//#include "PXAudioMIDI/PXAudioMIDI.h"


#include <PX/OS/Audio/Windows/PXMultiMedia.h>
#include <PX/OS/Audio/Windows/PXDirectSound.h>
#include <PX/OS/Audio/Windows/PXWASAPI.h>
#include <PX/Math/PXMath.h>
#include <stdlib.h>
#include <math.h>


const char PXAudioSystemText[] = "AudioSystem";
const PXI8U PXAudioSystemTextLength = sizeof(PXAudioSystemText);
const PXECSRegisterInfoStatic PXAudioSystemRegisterInfoStatic =
{
    {sizeof(PXAudioSystemText), sizeof(PXAudioSystemText), PXAudioSystemText, TextFormatASCII},
    sizeof(PXAudioSystem),
    __alignof(PXAudioSystem),
    PXECSTypeSystem,
    PXAudioSystemCreate,
    PXAudioSystemRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXAudioSystemRegisterInfoDynamic;

PXResult PXAPI PXAudioSystemRegisterToECS()
{
    PXECSRegister(&PXAudioSystemRegisterInfoStatic, &PXAudioSystemRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemCreate(PXAudioSystem** pxAudioSystemREF, PXAudioSystemCreateInfo PXREF pxAudioSystemCreateInfo)
{
    PXAudioSystem* pxAudioSystem = PXNull;

    if(!(pxAudioSystemREF && pxAudioSystemCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxAudioSystemCreateInfo->Info.Static = &PXAudioSystemRegisterInfoStatic;
    pxAudioSystemCreateInfo->Info.Dynamic = &PXAudioSystemRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxAudioSystemREF, pxAudioSystemCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxAudioSystem = *pxAudioSystemREF;
    pxAudioSystem->AudioSystemAPI = pxAudioSystemCreateInfo->System;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXAudioSystemText,
        "Initialize",
        "---Start---"
    );
#endif  

    switch(pxAudioSystem->AudioSystemAPI)
    {
#if OSWindows
        case PXAudioSystemWindowsMIDI:
        {
            //pxAudioSystem->Binding.Initialize = PXMultiMediaInitialize;
            break;
        }
        case PXAudioSystemWindowsMultiMedia:
        {
            pxAudioSystem->Binding.Initialize = PXMultiMediaInitialize;
            break;
        }
        case PXAudioSystemWindowsDirectSound:
        {
            pxAudioSystem->Binding.Initialize = PXDirectSoundInitialize;
            break;
        }
        case PXAudioSystemWindowsAudioSessionAPI:
        {
            pxAudioSystem->Binding.Initialize = PXWASAPIInitialize;
            break;
        }
        case PXAudioSystemWindowsXAudio:
        {
            //pxAudioSystem->Binding.Initialize = PXMultiMediaInitialize;
            break;
        }
#endif
        default:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXAudioSystemText,
                "Initialize",
                "There is no audio system"
            );
#endif
            return PXResultRefusedParameterInvalid;

        }
    }

    // Initialize
    {
        const PXResult initializeResult = pxAudioSystem->Binding.Initialize
        (
            &pxAudioSystem->SystemReference,
            pxAudioSystemCreateInfo
        );

        if(PXResultOK != initializeResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXAudioSystemText,
                "Initialize",
                "Failed!"
            );
#endif

            return initializeResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXAudioSystemText,
            "Initialize",
            "Success"
        );
#endif
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXAudioSystemText,
        "Initialize",
        "---Done---"
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemRelease(PXAudioSystem PXREF pxAudioSystem)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXAudioSystemDeviceCreate(PXAudioSystem PXREF pxAudioSystem)
{
    if(!pxAudioSystem)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemDeviceDestroy(PXAudioSystem PXREF pxAudioSystem)
{
    if(!pxAudioSystem)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemSourceCreate(PXAudioSystem PXREF pxAudioSystem)
{
    if(!pxAudioSystem)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemSourceDestroy(PXAudioSystem PXREF pxAudioSystem)
{
    if(!pxAudioSystem)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemStart(PXAudioSystem PXREF pxAudioSystem)
{
    if(!pxAudioSystem)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioSystemRender(PXAudioSystem PXREF pxAudioSystem)
{
    if(!pxAudioSystem)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXAudioSpeakerBeep(const PXI32U hz, const PXI32U time)
{
#if OSUnix
#elif OSWindows
    const PXBool success = Beep(hz, time);
#endif

    return PXResultOK;
}

PXResult PXAPI PXAudioDeviceDetect(PXAudioSystem PXREF pxAudioSystem, PXAudioDeviceDetectInfo PXREF pxAudioDeviceDetectInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXAudioSystemText,
        "Device",
        "Detected..\n"
        "%20s : %ls\n"
        "%20s : %s\n"
        "%20s : %i",
        "Name", pxAudioDeviceDetectInfo->Name.W,
        "Type", pxAudioDeviceDetectInfo->Type == PXAudioDeviceTypeInput ? "MICRO" : "SPEAKER",
        "ID", pxAudioDeviceDetectInfo->InternalID
    );
#endif

    return PXResultOK;
}

PXFAudio PXAPI PXAudioWaveGenerate(PXAudioWaveGenerateInfo* pxAudioWaveGenerateInfo)
{
    PXFAudio period = 1.0f / pxAudioWaveGenerateInfo->Hz;
    PXFAudio p = fmodf(pxAudioWaveGenerateInfo->X, period); // Normalize phase to [0,1)

    switch(pxAudioWaveGenerateInfo->WaveType)
    {
        case PXAudioWaveTypeSINE:
        {
            // 0.3f * sinf(2.0f * PXMathConstantPI * pxAudioWaveGenerateInfo.Phase * pxAudioWaveGenerateInfo.T);

            return pxAudioWaveGenerateInfo->Amplitude * sinf
            (
                2.0f *
                PXMathPI *
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
            PXFAudio t = pxAudioWaveGenerateInfo->X;
     
            PXFAudio amplitude = pxAudioWaveGenerateInfo->Amplitude;

            return (2.0f * amplitude / PXMathPI) * asinf(sinf(2.0f * PXMathPI * t / period));

         

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

PXFAudio PXAPI envelope(PXFAudio t, PXFAudio attack, PXFAudio sustain, PXFAudio decay)
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

PXFAudio PXAPI lowpass_filter(PXFAudio input, PXFAudio* prev, PXFAudio alpha)
{
    *prev = alpha * input + (1.0f - alpha) * (*prev);
    return *prev;
}
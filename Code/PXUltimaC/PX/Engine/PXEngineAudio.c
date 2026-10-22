#include "PXEngineAudio.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Type/PXText.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Math/PXMath.h>
#include <stdlib.h>
#include <math.h>


const char PXEngineAudioText[] = "AudioSystem";
const PXI8U PXEngineAudioTextLength = sizeof(PXEngineAudioText);
const PXECSRegisterInfoStatic PXEngineAudioRegisterInfoStatic =
{
    {sizeof(PXEngineAudioText), sizeof(PXEngineAudioText), PXEngineAudioText, TextFormatASCII},
    sizeof(PXEngineAudio),
    __alignof(PXEngineAudio),
    PXECSTypeSystem,
    PXEngineAudioCreate,
    PXEngineAudioRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineAudioRegisterInfoDynamic;


static PXAudioInitializeFunction _pxAudioInitializeList[8] = { 0 };
static PXSize _pxAudioInitializeListAmount = 0;


void PXAPI PXEngineAudioRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXEngineAudioRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXEngineAudioRegisterInfoDynamic;
}

PXResult PXAPI PXEngineAudioCreate(PXEngineAudio** pxEngineAudioREF, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo)
{
    PXEngineAudio* pxEngineAudio = PXNull;
    PXResult pxResult = PXResultInvalid;

    if(!(pxEngineAudioREF && pxEngineAudioCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineAudioCreateInfo->Info.Static = &PXEngineAudioRegisterInfoStatic;
    pxEngineAudioCreateInfo->Info.Dynamic = &PXEngineAudioRegisterInfoDynamic;
    pxResult = PXECSCreate(pxEngineAudioREF, pxEngineAudioCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineAudio = *pxEngineAudioREF;

    PXEngineModuleCallbackUpdateASYNCSet(&pxEngineAudio->ModuleInfo, PXEngineAudioUpdate);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineAudioText,
        PXCreateText,
        "\n"
        "%20s : %i",
        "APIs", pxEngineAudioCreateInfo->AudioInitializeListAmount
    );
#endif

    PXDictionaryECSCreate(&pxEngineAudio->DeviceOutputLookup);
    PXDictionaryECSCreate(&pxEngineAudio->DeviceInputLookup);

    for(PXSize i = 0; i < pxEngineAudioCreateInfo->AudioInitializeListAmount; i++)
    {
        PXAudioInitializeFunction pxAudioInitialize = pxEngineAudioCreateInfo->AudioInitializeList[i];

        PXEngineAudioRegister(pxAudioInitialize);
    }

    pxEngineAudio->Initialize = PXEngineAudioIndex(pxEngineAudioCreateInfo->AudioInitializeIndexTarget);

    if(!pxEngineAudio->Initialize)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXEngineAudioText,
            PXCreateText,
            "There is no audio system"
        );
#endif
        return PXResultRefusedParameterInvalid;
    }

    // Initialize
    pxResult = pxEngineAudio->Initialize
    (
        pxEngineAudio,
        pxEngineAudioCreateInfo
    );

    if(PXResultOK != pxResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXEngineAudioText,
            PXCreateText,
            "Failed!"
        );
#endif

        return pxResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineAudioText,
        PXCreateText,
        "---Done---"
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineAudioRelease(PXEngineAudio PXREF pxEngineAudio)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineAudioUpdate(PXEngineAudio PXREF pxEngineAudio, PXEngineUpdateInfo PXREF PXEngineUpdateInfo)
{
    return PXResultOK;
}

PXSize PXAPI PXEngineAudioAmount(void)
{
    return _pxAudioInitializeListAmount;
}

PXAudioInitializeFunction PXAPI PXEngineAudioIndex(const PXSize index)
{
    return _pxAudioInitializeList[index];
}

PXSize PXAPI PXEngineAudioRegister(PXAudioInitializeFunction pxAudioInitializeFunction)
{
    for(PXSize i = 0; i < 8; i++)
    {
        PXBool isEmpty = _pxAudioInitializeList[i] == 0;

        if(isEmpty)
        {
            _pxAudioInitializeList[i] = pxAudioInitializeFunction;

            ++_pxAudioInitializeListAmount;

            return i;
        }
    }

    return 0;
}

PXResult PXAPI PXEngineAudioUnregister(const PXSize index)
{
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

PXResult PXAPI PXAudioDeviceInputAmountEnsure(PXEngineAudio PXREF pxEngineAudio, const PXSize amount)
{
    return PXResultOK;
}

PXResult PXAPI PXAudioDeviceOutputAmountEnsure(PXEngineAudio PXREF pxEngineAudio, const PXSize amount)
{
    return PXResultOK;
}

PXResult PXAPI PXAudioDeviceDetect(PXEngineAudio PXREF pxEngineAudio, PXAudioDeviceCreateInfo PXREF pxAudioDeviceCreateInfo)
{
    PXResult pxResult = PXResultInvalid;  

    PXAudioDevice* pxAudioDeviceCurrent = 0;
    PXAudioDevice* pxAudioDeviceNew = 0;

    // Find object
    pxAudioDeviceCurrent = (PXAudioDevice*)PXECSEntryGetViaName(&pxAudioDeviceCreateInfo->Info.Name);


    // Create if not exist
    if(pxAudioDeviceNew)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineAudioText,
            "Device",
            "Detected already known device.\n"
            "%20s : %ls\n"
            "%20s : %s",
            "Name", pxAudioDeviceCreateInfo->Info.Name.W,
            "Type", pxAudioDeviceNew->Type == PXAudioDeviceTypeInput ? "MICRO" : "SPEAKER"
        );
#endif

        return PXResultOK;
    }


    pxResult = PXAudioDeviceCreate(&pxAudioDeviceNew, pxAudioDeviceCreateInfo);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineAudioText,
        "Device",
        "Detected new device.\n"
        "%20s : %ls\n"
        "%20s : %s",
        "Name", pxAudioDeviceCreateInfo->Info.Name.W,
        "Type", pxAudioDeviceNew->Type == PXAudioDeviceTypeInput ? "MICRO" : "SPEAKER"
    );
#endif

    switch(pxAudioDeviceNew->Type)
    {
        case PXAudioDeviceTypeInput:
        {
            PXDictionaryECSAdd(&pxEngineAudio->DeviceInputLookup, pxAudioDeviceNew);
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXDictionaryECSAdd(&pxEngineAudio->DeviceOutputLookup, pxAudioDeviceNew);
            break;
        }
    }

    return PXResultOK;
}

PXF32 PXAPI PXAudioWaveGenerate(PXAudioWaveGenerateInfo* pxAudioWaveGenerateInfo)
{
    PXF32 period = 1.0f / pxAudioWaveGenerateInfo->Hz;
    PXF32 p = fmodf(pxAudioWaveGenerateInfo->X, period); // Normalize phase to [0,1)

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
            PXF32 t = pxAudioWaveGenerateInfo->X;
     
            PXF32 amplitude = pxAudioWaveGenerateInfo->Amplitude;

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

PXF32 PXAPI envelope(PXF32 t, PXF32 attack, PXF32 sustain, PXF32 decay)
{
    if(t < attack)
        return t / attack;

    else if(t < attack + sustain)
        return 1.0f;
    else if(t < attack + sustain + decay) 
        return 1.0f - (t - attack - sustain) / decay;

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

PXF32 PXAPI lowpass_filter(PXF32 input, PXF32* prev, PXF32 alpha)
{
    *prev = alpha * input + (1.0f - alpha) * (*prev);
    return *prev;
}
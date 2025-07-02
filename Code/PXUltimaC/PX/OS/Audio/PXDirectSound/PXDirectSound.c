#include "PXDirectSound.h"

#if OSUnix
#pragma warning(comment, "DirectX does not exist under linux")
#elif OSWindows

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "uuid.lib")

#include <Windows.h>
#if WindowsAtleastVista
#include <mmeapi.h>
#else
#include <MMSystem.h>
#endif
#include <dsound.h> // Windows Direct Sound


#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

const char PXDirectSoundText[] = "DirectSound";
const char PXDirectSoundLibrary[] = "DSOUND.DLL";

#define PXDirectSound8Use DIRECTSOUND_VERSION >= 0x0800

#if PXDirectSound8Use
#define PXDirectSoundInputInterface IDirectSoundCapture8
#define PXDirectSoundOutputInterface IDirectSound8
#define PXDirectSoundInputCreate DirectSoundCaptureCreate8
#define PXDirectSoundOutputCreate DirectSoundCreate8
#define PXDirectSoundBuffer IDirectSoundBuffer8
#else
#define PXDirectSoundInputInterface IDirectSoundCapture
#define PXDirectSoundOutputInterface IDirectSound
#define PXDirectSoundInputCreate DirectSoundCaptureCreate8
#define PXDirectSoundOutputCreate DirectSoundCreate
#define PXDirectSoundBuffer IDirectSoundBuffer
#endif


typedef HRESULT(WINAPI* PXDirectSoundCreate8)(__in_opt LPCGUID pcGuidDevice, __deref_out LPDIRECTSOUND8* ppDS8, __null LPUNKNOWN pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundCaptureCreate8)(__in_opt LPCGUID pcGuidDevice, __deref_out LPDIRECTSOUNDCAPTURE8* ppDSC8, __null LPUNKNOWN pUnkOuter);



BOOL CALLBACK PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXInt32U* const amount)
{
    if(!guid)
    {
        return PXTrue;
    }

    ++(*amount);

    return PXTrue;
}

BOOL CALLBACK PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDeviceDetectObjectEventData* const pxAudioDeviceDetectObjectEventData)
{
    if(!guid) // First device gets listed twice for no reason. GUID is NULL for first element.
    {
        return PXTrue;
    }

    if(pxAudioDeviceDetectObjectEventData->IndexCurrent > pxAudioDeviceDetectObjectEventData->IndexTarget)
    {
        return PXTrue;
    }

    if(pxAudioDeviceDetectObjectEventData->IndexCurrent != pxAudioDeviceDetectObjectEventData->IndexTarget)
    {
        ++pxAudioDeviceDetectObjectEventData->IndexCurrent;
        return PXTrue;
    }
    PXAudioDevice* const pxAudioDevice = pxAudioDeviceDetectObjectEventData->AudioDevice;

    PXClear(PXAudioDevice, pxAudioDevice);

    pxAudioDevice->DeviceGUID = *guid;

    PXTextCopyA(cstrDescription, PXTextUnkownLength, pxAudioDevice->DeviceName, PXAudioDeviceNameSize);

    return PXFalse;
}
#endif



PXAudioDirectSound _pxAudioDirectSound;


PXActionResult PXAPI PXDirectSoundInitialize(PXAudioDirectSound** const pxAudioDirectSound, PXAudioInitializeInfo* const pxAudioInitializeInfo)
{
    PXAudio* pxAudio = PXNull;

    PXClear(PXAudioDirectSound, &_pxAudioDirectSound);

    if(pxAudioDirectSound)
    {
        *pxAudioDirectSound = &_pxAudioDirectSound;
    }

    if(pxAudioInitializeInfo)
    {
        pxAudio = pxAudioInitializeInfo->AudioReference;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectSoundText,
        "Initialize",
        "---Start---"
    );
#endif

    // Library Open
    {
        const PXActionResult libraryOpenResult = PXLibraryOpenA(&_pxAudioDirectSound.DirectSoundLibrary, PXDirectSoundLibrary);

        if(PXActionSuccessful != libraryOpenResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectSoundText,
        "Initialize",
        "Fetching functions"
    );
#endif

    {
        PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
        {
            // OpenGL Standard functions
            { &_pxAudioDirectSound.SoundCreate, "DirectSoundCreate"},
            { &_pxAudioDirectSound.SoundEnumerateA, "DirectSoundEnumerateA"},
            { &_pxAudioDirectSound.SoundEnumerateW, "DirectSoundEnumerateW"},
            { &_pxAudioDirectSound.CanUnloadNow, "DllCanUnloadNow"},
            { &_pxAudioDirectSound.ClassObjectGet, "DllGetClassObject"},
            { &_pxAudioDirectSound.SoundCaptureCreate, "DirectSoundCaptureCreate"},
            { &_pxAudioDirectSound.SoundCaptureEnumerateA, "DirectSoundCaptureEnumerateA"},
            { &_pxAudioDirectSound.SoundCaptureEnumerateW, "DirectSoundCaptureEnumerateW"},
            { &_pxAudioDirectSound.SoundCreate8, "DirectSoundCreate8"},
            { &_pxAudioDirectSound.SoundCaptureCreate8, "DirectSoundCaptureCreate8"}
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&_pxAudioDirectSound.DirectSoundLibrary, pxLibraryFuntionEntry, amount);
    }

    // Link functions
    if(pxAudio)
    {
        pxAudio->DeviceAmount = (PXAudioDeviceAmountFunction)PXDirectSoundDeviceAmount;
        pxAudio->DeviceFetch = PXDirectSoundDeviceFetch;
        pxAudio->DeviceFetchAll = PXDirectSoundDeviceFetchAll;
        pxAudio->DeviceOpen = PXDirectSoundDeviceOpen;
        pxAudio->DeviceClose = PXDirectSoundDeviceClose;
        pxAudio->DeviceLoad = PXDirectSoundDeviceLoad;  
        pxAudio->DeviceEffectUpdate = PXDirectSoundEffectUpdate;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectSoundText,
        "Initialize",
        "---Done---"
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceAmount(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo)
{
#if OSUnix
#elif OSWindows
    HRESULT enumResultID;

    pxAudioDeviceAmountInfo->DeviceInput = 0;
    pxAudioDeviceAmountInfo->DeviceOutput = 0;

    //-----------------------------------------------------
    // Input
    //-----------------------------------------------------
    {
        PXDirectSoundCaptureEnumerateA pxDirectSoundCaptureEnumerateA = (PXDirectSoundCaptureEnumerateA)_pxAudioDirectSound.SoundCaptureEnumerateA;
        PXLPDSENUMCALLBACKA callBack = (PXLPDSENUMCALLBACKA)PXAudioDeviceDetectAmountCallBack;

        enumResultID = pxDirectSoundCaptureEnumerateA(callBack, &pxAudioDeviceAmountInfo->DeviceInput);
    }
    //-----------------------------------------------------


    //-----------------------------------------------------
    // Output
    //-----------------------------------------------------
    {
        PXDirectSoundEnumerateA pxDirectSoundEnumerateA = (PXDirectSoundEnumerateA)_pxAudioDirectSound.SoundCaptureEnumerateA;
        PXLPDSENUMCALLBACKA callBack = (PXLPDSENUMCALLBACKA)PXAudioDeviceDetectAmountCallBack;

        enumResultID = pxDirectSoundEnumerateA(callBack, &pxAudioDeviceAmountInfo->DeviceOutput);
    }
    //-----------------------------------------------------

    const PXActionResult enumResult = PXWindowsHandleErrorFromID(enumResultID);

    PXActionReturnOnError(enumResult);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceFetch(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
#elif OSWindows
    PXAudioDeviceDetectObjectEventData pxAudioDeviceDetectObjectEventData;
    pxAudioDeviceDetectObjectEventData.IndexCurrent = 0;
    pxAudioDeviceDetectObjectEventData.IndexTarget = deviceID;
    pxAudioDeviceDetectObjectEventData.AudioDevice = pxAudioDevice;

    HRESULT enumResultID;

    switch(pxAudioDeviceType)
    {
        case PXAudioDeviceTypeInput:
        {
            PXDirectSoundEnumerateA pxDirectSoundEnumerateA = _pxAudioDirectSound.SoundCaptureEnumerateA;

            enumResultID = pxDirectSoundEnumerateA
            (
                PXAudioDeviceDetectObjectCallBack,
                &pxAudioDeviceDetectObjectEventData
            );
            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXDirectSoundCaptureEnumerateA pxDirectSoundCaptureEnumerateA = _pxAudioDirectSound.SoundCaptureEnumerateA;

            enumResultID = pxDirectSoundCaptureEnumerateA
            (
                PXAudioDeviceDetectObjectCallBack,
                &pxAudioDeviceDetectObjectEventData
            );
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }

    const PXActionResult enumResult = PXWindowsHandleErrorFromID(enumResultID);

    PXActionReturnOnError(enumResult);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceFetchAll(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXDirectSoundDeviceFetch(pxAudioDirectSound, pxAudioDeviceType, i, &pxAudioDevice[i]);
    }

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOpen(PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
#if OSUnix
#elif OSWindows
    pxAudioDevice->Type = pxAudioDeviceType;

    switch(pxAudioDeviceType)
    {
        case PXAudioDeviceTypeInput:
        {
            IDirectSoundCapture8* directSoundCapture;
            HRESULT crateResultID = 0;

            if(_pxAudioDirectSound.SoundCaptureCreate8) // Can create 8?
            {
                PXDirectSoundCaptureCreate8 pxDirectSoundCaptureCreate8 = (PXDirectSoundCaptureCreate8)_pxAudioDirectSound.SoundCaptureCreate8;

                crateResultID = pxDirectSoundCaptureCreate8(PXNull, (IDirectSoundCapture8**)&_pxAudioDirectSound.DirectSoundInterface, PXNull);
            }
            else
            {
                PXDirectSoundCaptureCreate pxDirectSoundCaptureCreate = (PXDirectSoundCaptureCreate)_pxAudioDirectSound.SoundCaptureCreate;

                crateResultID = pxDirectSoundCaptureCreate(PXNull, (IDirectSoundCapture**)&_pxAudioDirectSound.DirectSoundInterface, PXNull);
            }

            const PXActionResult crateResult = PXWindowsHandleErrorFromID(crateResultID);

            PXActionReturnOnError(crateResult);

            directSoundCapture = (IDirectSoundCapture8*)_pxAudioDirectSound.DirectSoundInterface;

            DSCCAPS capabiltys;
            PXClear(DSCCAPS, &capabiltys);
            capabiltys.dwSize = sizeof(DSCCAPS);

            directSoundCapture->lpVtbl->GetCaps(directSoundCapture, &capabiltys);

            pxAudioDevice->SupportFlags = capabiltys.dwFlags;
            pxAudioDevice->FormatSupportFlags = capabiltys.dwFormats;
            pxAudioDevice->Channels = capabiltys.dwChannels;

            break;
        }
        case PXAudioDeviceTypeOutput:
        {
            PXDirectSoundOutputInterface* directSound = PXNull;

            {
                HRESULT crateResultID = 0;

                if(_pxAudioDirectSound.SoundCreate8) // Can create 8?
                {
                    PXDirectSoundCaptureCreate8 pxDirectSoundCaptureCreate8 = (PXDirectSoundCaptureCreate8)_pxAudioDirectSound.SoundCreate8;

                    crateResultID = pxDirectSoundCaptureCreate8(PXNull, (PXDirectSoundOutputInterface**)&_pxAudioDirectSound.DirectSoundInterface, PXNull);
                }
                else
                {
                    PXDirectSoundCaptureCreate pxDirectSoundCaptureCreate = (PXDirectSoundCaptureCreate)_pxAudioDirectSound.SoundCreate;

                    crateResultID = pxDirectSoundCaptureCreate(PXNull, (PXDirectSoundOutputInterface**)&_pxAudioDirectSound.DirectSoundInterface, PXNull);
                }

                const PXActionResult crateResult = PXWindowsHandleErrorFromID(crateResultID);

                PXActionReturnOnError(crateResult);

                directSound = (PXDirectSoundOutputInterface*)_pxAudioDirectSound.DirectSoundInterface;
            }

            /// Capability
            {
                DSCAPS capabiltys;
                PXClear(DSCAPS, &capabiltys);
                capabiltys.dwSize = sizeof(DSCAPS);

                const HRESULT capResultID = directSound->lpVtbl->GetCaps(directSound, &capabiltys);
                const PXActionResult initResult = PXWindowsHandleErrorFromID(capResultID);

                PXActionReturnOnError(initResult);

                pxAudioDevice->SupportFlags = capabiltys.dwFlags;

            }

          
/* Alternativ jank
            LPDIRECTSOUND8 directSound = PXNull;
            LPDIRECTSOUNDBUFFER buffer;

            const HRESULT createResult = DirectSoundCreate8
            (
                NULL,
                &(LPDIRECTSOUND8)_pxAudioDirectSound.DirectSoundInterface,
                NULL
            );

            directSound = (LPDIRECTSOUND8)_pxAudioDirectSound.DirectSoundInterface;
            */

            {
                // const HWND windowHandle = GetConsoleWindow();
                HWND windowHandle = GetForegroundWindow();

                if(windowHandle == NULL)
                {
                    windowHandle = GetDesktopWindow();
                }

                directSound->lpVtbl->SetCooperativeLevel(directSound, windowHandle, DSSCL_PRIORITY);
            }

            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceClose(PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceLoad(PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
{
    if(!(pxAudioDevice && pxSound))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    PXDirectSoundOutputInterface* const directSound = (PXDirectSoundOutputInterface*)_pxAudioDirectSound.DirectSoundInterface;
    PXDirectSoundBuffer* soundBuffer = PXNull;
    const PXBool canUse3DStuff = pxSound->NumerOfChannels == 1u; // DirectSoundBuffer3D can only use MONO sounds

    // Create buffer to load data into
    {
        WAVEFORMATEX waveFormat;
        waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.nChannels = pxSound->NumerOfChannels;
        waveFormat.nSamplesPerSec = pxSound->SampleRate;
        waveFormat.nAvgBytesPerSec = pxSound->ByteRate;
        waveFormat.nBlockAlign = pxSound->BlockAllign;
        waveFormat.wBitsPerSample = pxSound->BitsPerSample;
        waveFormat.cbSize = sizeof(WAVEFORMATEX);

        //CoInitialize(PXNull);


        DSBUFFERDESC pcDSBufferDesc;
        pcDSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
        pcDSBufferDesc.dwFlags =
            //DSBCAPS_PRIMARYBUFFER |
            //DSBCAPS_STATIC |
            DSBCAPS_CTRLFX | // Does not work with "DSBCAPS_STATIC"
            //DSBCAPS_CTRLPAN |
            DSBCAPS_CTRLVOLUME |
            DSBCAPS_CTRLFREQUENCY |
            DSBCAPS_CTRLPOSITIONNOTIFY |
            DSBCAPS_GLOBALFOCUS; // Prevent the music from stopping if you TAB out;


        if(canUse3DStuff)
        {
            pcDSBufferDesc.dwFlags |=
                DSBCAPS_CTRL3D | // Only works with MONO sound
                DSBCAPS_GETCURRENTPOSITION2;
            //DSBCAPS_PRIMARYBUFFER; // Add position capability
        }

        pcDSBufferDesc.dwBufferBytes = pxSound->DataSize; // DSBSIZE_MIN, DSBSIZE_MAX
        pcDSBufferDesc.dwReserved = 0;
        pcDSBufferDesc.lpwfxFormat = &waveFormat;
#if DIRECTSOUND_VERSION >= 0x0700
        pcDSBufferDesc.guid3DAlgorithm = DS3DALG_DEFAULT;
        //PXClear(GUID, &pcDSBufferDesc.guid3DAlgorithm);
#endif



        directSound->lpVtbl->Initialize(directSound, &pxAudioDevice->DeviceGUID);


        const HRESULT createResultID = directSound->lpVtbl->CreateSoundBuffer
        (
            directSound,
            &pcDSBufferDesc,
            &(IDirectSoundBuffer*)pxAudioDevice->SoundBuffer,
            PXNull
        );
        const PXActionResult createResult = PXWindowsHandleErrorFromID(createResultID);

        PXActionReturnOnError(createResult);

        soundBuffer = pxAudioDevice->SoundBuffer;
    }

    PXSize dataOffset = 0;
    void* dataBlockAdressA = 0;
    void* dataBlockAdressB = 0;
    DWORD dataBlockSizeA = 0;
    DWORD dataBlockSizeB = 0;
    DWORD flags = 0;
    // Lock directSound buffer so we are allowed to write to it.
    {
        const HRESULT lockResultID = soundBuffer->lpVtbl->Lock(soundBuffer, dataOffset, pxSound->DataSize, &dataBlockAdressA, &dataBlockSizeA, &dataBlockAdressB, dataBlockSizeB, flags);
        const PXActionResult lockResult = PXWindowsHandleErrorFromID(lockResultID);

        PXActionReturnOnError(lockResult);
    }

    // Write sound data to actual soundBuffer.
    {
        PXCopyList(PXByte, dataBlockSizeA, pxSound->Data, dataBlockAdressA);

        if(dataBlockAdressB)
        {
            PXCopyList(PXByte, dataBlockSizeB, ((PXByte*)pxSound->Data) + dataBlockSizeA, dataBlockAdressB);
        }
    }

    // Unlock buffer to release it back to DirectSound
    {
        const HRESULT lockResultID = soundBuffer->lpVtbl->Unlock(soundBuffer, dataBlockAdressA, dataBlockSizeA, dataBlockAdressB, dataBlockSizeB);
        const PXActionResult lockResult = PXWindowsHandleErrorFromID(lockResultID);

        PXActionReturnOnError(lockResult);
    }



    //{
    IDirectSound3DListener8* listener = PXNull;

    const HRESULT listenResultID = soundBuffer->lpVtbl->QueryInterface(soundBuffer, &IID_IDirectSound3DListener, &pxAudioDevice->Listen3DInterface);
    const PXActionResult lockResult = PXWindowsHandleErrorFromID(listenResultID);

    //listener->lpVtbl->SetPosition

    //listener->lpVtbl->

    //PXActionReturnOnError(lockResult);
//}

    if(canUse3DStuff)
    {
        const HRESULT bufferResultID = soundBuffer->lpVtbl->QueryInterface(soundBuffer, &IID_IDirectSound3DBuffer, &pxAudioDevice->Buffer3DInterface);
        const PXActionResult bufferResult = PXWindowsHandleErrorFromID(bufferResultID);
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceProperty(PXAudioDevice* const pxAudioDevice, PXSoundDeviceProperty* const pxSoundDeviceProperty)
{
    if(!(pxAudioDevice && pxSoundDeviceProperty))
    {
        return PXActionRefusedArgumentNull;
    }

    const PXBool isWriteOP = 0 < (PXSoundDevicePropertyWrite & pxSoundDeviceProperty->Flags);

    


#if OSUnix
#elif OSWindows
    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;
    D3DVECTOR* vector3f = (D3DVECTOR*)&pxSoundDeviceProperty->Position3D; // [WARNING] Trusting cast!
    HRESULT resultID = 0;

    if(!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

    // only quit here if we dont have this feature AND we need it
    if(!directSound3DBuffer)
    {
        return PXActionRefuedObjectInterfaceNotAvailable;
    }
#endif

    switch(pxSoundDeviceProperty->Type)
    {
        case PXSoundDevicePropertyStatePlay:
        {
            resultID = soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);
            pxAudioDevice->IsRunning = PXTrue;
            break;
        }
        case PXSoundDevicePropertyStatePause:
        {
            resultID = soundBuffer->lpVtbl->Stop(soundBuffer);
            pxAudioDevice->IsRunning = PXFalse;
            break;
        }
        case PXSoundDevicePropertyStateRestart:
        {
            PXSoundDeviceProperty pxSoundDeviceProperty;
            pxSoundDeviceProperty.Flags = 0;
            pxSoundDeviceProperty.Value = 0;

            pxSoundDeviceProperty.Type = PXSoundDevicePropertyStatePause;
            PXDirectSoundDeviceProperty(pxAudioDevice, &pxSoundDeviceProperty); // Stop

            pxSoundDeviceProperty.Type = PXSoundDevicePropertyPositionCursor;
            PXDirectSoundDeviceProperty(pxAudioDevice, &pxSoundDeviceProperty); // Reset to 0

            pxSoundDeviceProperty.Type = PXSoundDevicePropertyStatePlay;
            PXDirectSoundDeviceProperty(pxAudioDevice, &pxSoundDeviceProperty); // Play

            break;
        }
        case PXSoundDevicePropertyPositionCursor:
        {
            if(isWriteOP)
            {
                resultID = soundBuffer->lpVtbl->SetCurrentPosition(soundBuffer, pxSoundDeviceProperty->Value);
            }
            else
            {
                DWORD positionRead = 0;
                DWORD positionWrite = 0;

                resultID = soundBuffer->lpVtbl->GetCurrentPosition(soundBuffer, &positionRead, &positionWrite);

                pxSoundDeviceProperty->Value = positionRead;
            }        

            break;
        }
        case PXSoundDevicePropertySpeed:
        {
            if(isWriteOP)
            {
                resultID = soundBuffer->lpVtbl->SetFrequency(soundBuffer, pxSoundDeviceProperty->Value);
            }
            else
            {
                DWORD frequencyDW = 0;

                resultID = soundBuffer->lpVtbl->GetFrequency(soundBuffer, &frequencyDW);

                pxSoundDeviceProperty->Value = frequencyDW;
            }

            break;
        }
        case PXSoundDevicePropertyPositionSpace:
        {
            if(isWriteOP)
            {
                resultID = directSound3DBuffer->lpVtbl->SetPosition
                (
                    directSound3DBuffer, 
                    pxSoundDeviceProperty->Position3D.X,
                    pxSoundDeviceProperty->Position3D.Y,
                    pxSoundDeviceProperty->Position3D.Z,
                    DS3D_IMMEDIATE
                );
            }
            else
            {
                resultID = directSound3DBuffer->lpVtbl->GetPosition(directSound3DBuffer, vector3f);
            }

            break;
        }
        case PXSoundDevicePropertyAngleCone:
        {


            break;
        }
        case PXSoundDevicePropertyVelocity:
        {
            if(isWriteOP)
            {
                const HRESULT getResultID = directSound3DBuffer->lpVtbl->SetVelocity
                (
                    directSound3DBuffer,
                    pxSoundDeviceProperty->Position3D.X,
                    pxSoundDeviceProperty->Position3D.Y,
                    pxSoundDeviceProperty->Position3D.Z,
                    DS3D_DEFERRED
                );
            }
            else
            {
                resultID = directSound3DBuffer->lpVtbl->GetVelocity(directSound3DBuffer, vector3f);  
            }
        }
        case PXSoundDevicePropertyVolume:
        {
            if(isWriteOP)
            {
                const HRESULT getResultID = soundBuffer->lpVtbl->SetVolume(soundBuffer, pxSoundDeviceProperty->Value); // DSBPLAY_LOOPING
            }
            else
            {
                LONG volume = 0;

                const HRESULT getResultID = soundBuffer->lpVtbl->GetVolume(soundBuffer, &volume);

                pxSoundDeviceProperty->Value = volume;
            }

            break;
        }
        default:
        {
            return PXActionRefusedArgumentInvalid;
        }
    }

#if OSUnix
#elif OSWindows
    const PXActionResult setResult = PXWindowsHandleErrorFromID(resultID);

    if(PXActionSuccessful != setResult) 
        return setResult;
#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundEffectEnable(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
#if OSUnix
#elif OSWindows

    const PXBool wasRunning = pxAudioDevice->IsRunning; // Store current state to keep davice playing after setting effect
    PXDirectSoundBuffer* soundBuffer = (PXDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    typedef struct PXListEntryEE_
    {
        GUID* ElementFilterID;
        GUID* ElementID;
        void** AdressReference;
        PXBool Enabled;
    }
    PXAudioEffectEntry;

    const PXAudioEffectEntry pxAudioEffectEntryList[] =
    {
        { &GUID_DSFX_STANDARD_GARGLE,        &IID_IDirectSoundFXGargle,        &pxAudioDevice->FXGargle,        (pxAudioDevice->FXSettingFlagList & PXAudioFXGargle) > 0},
        { &GUID_DSFX_STANDARD_CHORUS,        &IID_IDirectSoundFXChorus,        &pxAudioDevice->FXChorus,        (pxAudioDevice->FXSettingFlagList & PXAudioFXChorus) > 0},
        { &GUID_DSFX_STANDARD_FLANGER,        &IID_IDirectSoundFXFlanger,        &pxAudioDevice->FXFlanger,        (pxAudioDevice->FXSettingFlagList & PXAudioFXFlanger) > 0},
        { &GUID_DSFX_STANDARD_ECHO,            &IID_IDirectSoundFXEcho,        &pxAudioDevice->FXEcho,            (pxAudioDevice->FXSettingFlagList & PXAudioFXEcho) > 0},
        { &GUID_DSFX_STANDARD_DISTORTION,    &IID_IDirectSoundFXDistortion,    &pxAudioDevice->FXDistortion,    (pxAudioDevice->FXSettingFlagList & PXAudioFXDistortion) > 0},
        { &GUID_DSFX_STANDARD_COMPRESSOR,    &IID_IDirectSoundFXCompressor,    &pxAudioDevice->FXCompressor,    (pxAudioDevice->FXSettingFlagList & PXAudioFXCompressor) > 0},
        { &GUID_DSFX_STANDARD_PARAMEQ,        &IID_IDirectSoundFXParamEq,        &pxAudioDevice->FXParamEq,        (pxAudioDevice->FXSettingFlagList & PXAudioFXParamEq) > 0},
        { &GUID_DSFX_WAVES_REVERB,            &IID_IDirectSoundFXWavesReverb,    &pxAudioDevice->FXWavesReverb,    (pxAudioDevice->FXSettingFlagList & PXAudioFXWavesReverb) > 0},
        { &GUID_DSFX_STANDARD_I3DL2REVERB,    &IID_IDirectSoundFXI3DL2Reverb,    &pxAudioDevice->FXWavesReverb,    (pxAudioDevice->FXSettingFlagList & PXAudioFXI3DL2Reverb) > 0},
    };
    const PXSize amount = sizeof(pxAudioEffectEntryList) / sizeof(PXAudioEffectEntry);

    PXSize amountCounter = 0;

    DSEFFECTDESC dsEffectList[9];
    DWORD dwResults[9];
    PXClearList(DSEFFECTDESC, dsEffectList, 9);

    for(PXSize i = 0; i < amount; ++i)
    {
        const PXAudioEffectEntry* const pxListEntry = &pxAudioEffectEntryList[i];

        if(!pxListEntry->Enabled)
        {
            continue;
        }

        DSEFFECTDESC* const effectDESC = &dsEffectList[amountCounter++];

        effectDESC->dwSize = sizeof(DSEFFECTDESC);
        effectDESC->dwFlags = 0;
        effectDESC->guidDSFXClass = *pxListEntry->ElementFilterID;
    }

    if(wasRunning)
    {
        PXDirectSoundDeviceStop(pxAudioDirectSound, pxAudioDevice); // Stop the device, we are not allowed to update effects while playing
    }

    soundBuffer->lpVtbl->SetFX(soundBuffer, 0, 0, dwResults);

    const HRESULT effectSetResultID = soundBuffer->lpVtbl->SetFX(soundBuffer, amountCounter, &dsEffectList, dwResults);
    const PXActionResult effectSetResult = PXWindowsHandleErrorFromID(effectSetResultID);

    for(PXSize i = 0; i < amount; ++i)
    {
        const PXAudioEffectEntry* const pxListEntry = &pxAudioEffectEntryList[i];

        if(!pxListEntry->Enabled)
        {
            *pxListEntry->AdressReference = PXNull;
            continue;
        }

        soundBuffer->lpVtbl->GetObjectInPath(soundBuffer, pxListEntry->ElementFilterID, 0, pxListEntry->ElementID, pxListEntry->AdressReference);
    }

    if(wasRunning)
    {
        PXDirectSoundDeviceStart(pxAudioDirectSound, pxAudioDevice);
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundEffectUpdate(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXAudioEffect* const pxAudioEffect)
{
#if OSUnix
#elif OSWindows

    // check input

    // Check if enabled
    {
        PXInt32U flag = 0;

        switch(pxAudioEffect->Type)
        {
            case PXAudioEffectTypeChorus:
            {
                flag = PXAudioFXChorus;
                break;
            }
            case PXAudioEffectTypeCompressor:
            {
                flag = PXAudioFXCompressor;
                break;
            }
            case PXAudioEffectTypeDistortion:
            {
                flag = PXAudioFXDistortion;
                break;
            }
            case PXAudioEffectTypeEcho:
            {
                flag = PXAudioFXEcho;
                break;
            }
            case PXAudioEffectTypeFlanger:
            {
                flag = PXAudioFXFlanger;
                break;
            }
            case PXAudioEffectTypeGargle:
            {
                flag = PXAudioFXGargle;
                break;
            }
            case PXAudioEffectTypeInteractive3DLevel2Reverb:
            {
                flag = PXAudioFXI3DL2Reverb;
                break;
            }
            case PXAudioEffectTypeParamEq:
            {
                flag = PXAudioFXParamEq;
                break;
            }
            case PXAudioEffectTypeWavesReverb:
            {
                flag = PXAudioFXWavesReverb;
                break;
            }

            default:
                return PXActionRefusedArgumentInvalid;
        }


        // Update API
        {
            const PXBool before = pxAudioDevice->FXSettingFlagList;

            if(pxAudioEffect->Enable)
            {
                pxAudioDevice->FXSettingFlagList |= flag;
            }
            else
            {
                pxAudioDevice->FXSettingFlagList &= ~flag;
            }

            const PXBool hasChanged = before != pxAudioDevice->FXSettingFlagList;

            if(hasChanged)
            {
                PXDirectSoundEffectEnable(pxAudioDirectSound, pxAudioDevice);

            }
        }

        // quit if we dont process anything
        if(!pxAudioEffect->Enable)
        {
            return PXActionSuccessful;
        }
    }


    // Handle spesific action
    {
        HRESULT result = 0;

        switch(pxAudioEffect->Type)
        {
            case PXAudioEffectTypeChorus:
            {
                IDirectSoundFXChorus8* const directSoundFXChorus = (IDirectSoundFXChorus8*)pxAudioDevice->FXChorus;
                PXAudioEffectChorus* const pxAudioEffectChorus = &pxAudioEffect->Chorus;
                DSFXChorus dxfxChorus;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXChorus->lpVtbl->GetAllParameters(directSoundFXChorus, &dxfxChorus);

                    pxAudioEffectChorus->WetDryMix = dxfxChorus.fWetDryMix;
                    pxAudioEffectChorus->Depth = dxfxChorus.fDepth;
                    pxAudioEffectChorus->Feedback = dxfxChorus.fFeedback;
                    pxAudioEffectChorus->Frequency = dxfxChorus.fFrequency;
                    pxAudioEffectChorus->Waveform = dxfxChorus.lWaveform;
                    pxAudioEffectChorus->Delay = dxfxChorus.fDelay;
                    pxAudioEffectChorus->Phase = dxfxChorus.lPhase;
                }
                else
                {
                    dxfxChorus.fWetDryMix = PXMathLiniarF(DSFXCHORUS_WETDRYMIX_MIN, DSFXCHORUS_WETDRYMIX_MAX, 0, 1, pxAudioEffectChorus->WetDryMix);
                    dxfxChorus.fDepth = PXMathLiniarF(DSFXCHORUS_DEPTH_MIN, DSFXCHORUS_DEPTH_MAX, 0, 1, pxAudioEffectChorus->Depth);
                    dxfxChorus.fFeedback = PXMathLiniarF(DSFXCHORUS_FEEDBACK_MIN, DSFXCHORUS_FEEDBACK_MAX, 0, 1, pxAudioEffectChorus->Feedback);
                    dxfxChorus.fFrequency = PXMathLiniarF(DSFXCHORUS_FREQUENCY_MIN, DSFXCHORUS_FREQUENCY_MAX, 0, 1, pxAudioEffectChorus->Frequency);
                    dxfxChorus.lWaveform = PXMathLiniarF(0, 1, 0, 1, pxAudioEffectChorus->Waveform);
                    dxfxChorus.fDelay = PXMathLiniarF(DSFXCHORUS_DELAY_MIN, DSFXCHORUS_DELAY_MAX, 0, 1, pxAudioEffectChorus->Delay);
                    dxfxChorus.lPhase = PXMathLiniarF(DSFXCHORUS_PHASE_MIN, DSFXCHORUS_PHASE_MAX, 0, 1, pxAudioEffectChorus->Phase);

                    result = directSoundFXChorus->lpVtbl->SetAllParameters(directSoundFXChorus, &dxfxChorus);
                }


                PXLogPrint
                (
                    PXLoggingInfo,
                    "DirectSound",
                    "FX-Update",
                    "Chorus : %s\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6i\n"
                    "%10s : %6.2f\n"
                    "%10s : %6i\n",
                    result == DS_OK ? "OK" : "FAIL",
                    "WetDryMix", dxfxChorus.fWetDryMix,
                    "Depth", dxfxChorus.fDepth,
                    "Feedback", dxfxChorus.fFeedback,
                    "Frequency", dxfxChorus.fFrequency,
                    "Waveform", dxfxChorus.lWaveform,
                    "Delay", dxfxChorus.fDelay,
                    "Phase", dxfxChorus.lPhase
                );


                break;
            }
            case PXAudioEffectTypeCompressor:
            {
                IDirectSoundFXCompressor8* const directSoundFXCompressor = (IDirectSoundFXCompressor8*)pxAudioDevice->FXCompressor;
                PXAudioEffectCompressor* const pxAudioEffectCompressor = &pxAudioEffect->Compressor;
                DSFXCompressor dsfxCompressor;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXCompressor->lpVtbl->GetAllParameters(directSoundFXCompressor, &dsfxCompressor);


                    pxAudioEffectCompressor->Gain = dsfxCompressor.fGain;
                    pxAudioEffectCompressor->Attack = dsfxCompressor.fAttack;
                    pxAudioEffectCompressor->Release = dsfxCompressor.fRelease;
                    pxAudioEffectCompressor->Threshold = dsfxCompressor.fThreshold;
                    pxAudioEffectCompressor->Ratio = dsfxCompressor.fRatio;
                    pxAudioEffectCompressor->Predelay = dsfxCompressor.fPredelay;
                }
                else
                {
                    dsfxCompressor.fGain = PXMathLiniarF(DSFXCOMPRESSOR_GAIN_MIN, DSFXCOMPRESSOR_GAIN_MAX, 0, 1, pxAudioEffectCompressor->Gain);
                    dsfxCompressor.fAttack = PXMathLiniarF(DSFXCOMPRESSOR_ATTACK_MIN, DSFXCOMPRESSOR_ATTACK_MAX, 0, 1, pxAudioEffectCompressor->Attack);
                    dsfxCompressor.fRelease = PXMathLiniarF(DSFXCOMPRESSOR_RELEASE_MIN, DSFXCOMPRESSOR_RELEASE_MAX, 0, 1, pxAudioEffectCompressor->Release);
                    dsfxCompressor.fThreshold = PXMathLiniarF(DSFXCOMPRESSOR_THRESHOLD_MIN, DSFXCOMPRESSOR_THRESHOLD_MAX, 0, 1, pxAudioEffectCompressor->Threshold);
                    dsfxCompressor.fRatio = PXMathLiniarF(DSFXCOMPRESSOR_RATIO_MIN, DSFXCOMPRESSOR_RATIO_MAX, 0, 1, pxAudioEffectCompressor->Ratio);
                    dsfxCompressor.fPredelay = PXMathLiniarF(DSFXCOMPRESSOR_PREDELAY_MIN, DSFXCOMPRESSOR_PREDELAY_MAX, 0, 1, pxAudioEffectCompressor->Predelay);

                    result = directSoundFXCompressor->lpVtbl->SetAllParameters(directSoundFXCompressor, &dsfxCompressor);
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    "DirectSound",
                    "FX-Update",
                    "Compressor : %s\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n",
                    result == DS_OK ? "OK" : "FAIL",
                    "Gain", dsfxCompressor.fGain,
                    "Attack", dsfxCompressor.fAttack,
                    "Release", dsfxCompressor.fRelease,
                    "Threshold", dsfxCompressor.fThreshold,
                    "Ratio", dsfxCompressor.fRatio,
                    "Predelay", dsfxCompressor.fPredelay
                );

                break;
            }
            case PXAudioEffectTypeDistortion:
            {
                IDirectSoundFXDistortion8* const directSoundFXDistortion = (IDirectSoundFXDistortion8*)pxAudioDevice->FXDistortion;
                PXAudioEffectDistortion* const pxAudioEffectDistortion = &pxAudioEffect->Distortion;
                DSFXDistortion dsfxDistortion;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXDistortion->lpVtbl->GetAllParameters(directSoundFXDistortion, &dsfxDistortion);

                    pxAudioEffectDistortion->Gain = dsfxDistortion.fGain;
                    pxAudioEffectDistortion->Edge = dsfxDistortion.fEdge;
                    pxAudioEffectDistortion->PostEQCenterFrequency = dsfxDistortion.fPostEQCenterFrequency;
                    pxAudioEffectDistortion->PostEQBandwidth = dsfxDistortion.fPostEQBandwidth;
                    pxAudioEffectDistortion->PreLowpassCutoff = dsfxDistortion.fPreLowpassCutoff;
                }
                else
                {
                    dsfxDistortion.fGain = PXMathLiniarF(DSFXDISTORTION_GAIN_MIN, DSFXDISTORTION_GAIN_MAX, 0, 1, pxAudioEffectDistortion->Gain);
                    dsfxDistortion.fEdge = PXMathLiniarF(DSFXDISTORTION_EDGE_MIN, DSFXDISTORTION_EDGE_MAX, 0, 1, pxAudioEffectDistortion->Edge);
                    dsfxDistortion.fPostEQCenterFrequency = PXMathLiniarF(DSFXDISTORTION_POSTEQCENTERFREQUENCY_MIN, DSFXDISTORTION_POSTEQCENTERFREQUENCY_MAX, 0, 1, pxAudioEffectDistortion->PostEQCenterFrequency);
                    dsfxDistortion.fPostEQBandwidth = PXMathLiniarF(DSFXDISTORTION_POSTEQBANDWIDTH_MIN, DSFXDISTORTION_POSTEQBANDWIDTH_MAX, 0, 1, pxAudioEffectDistortion->PostEQBandwidth);
                    dsfxDistortion.fPreLowpassCutoff = PXMathLiniarF(DSFXDISTORTION_PRELOWPASSCUTOFF_MIN, DSFXDISTORTION_PRELOWPASSCUTOFF_MAX, 0, 1, pxAudioEffectDistortion->PreLowpassCutoff);

                    result = directSoundFXDistortion->lpVtbl->SetAllParameters(directSoundFXDistortion, &dsfxDistortion);
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    "DirectSound",
                    "FX-Update",
                    "Distortion : %s\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n",
                    result == DS_OK ? "OK" : "FAIL",
                    "Gain", dsfxDistortion.fGain,
                    "Edge", dsfxDistortion.fEdge,
                    "PostEQCenterFrequency", dsfxDistortion.fPostEQCenterFrequency,
                    "PostEQBandwidth", dsfxDistortion.fPostEQBandwidth,
                    "PreLowpassCutoff", dsfxDistortion.fPreLowpassCutoff
                );

                break;
            }
            case PXAudioEffectTypeEcho:
            {
                IDirectSoundFXEcho8* const directSoundFXEcho = (IDirectSoundFXEcho8*)pxAudioDevice->FXEcho;
                PXAudioEffectEcho* const pxAudioEffectEcho = &pxAudioEffect->Echo;
                DSFXEcho dsfxEcho;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXEcho->lpVtbl->GetAllParameters(directSoundFXEcho, &dsfxEcho);

                    pxAudioEffectEcho->WetDryMix = dsfxEcho.fWetDryMix;
                    pxAudioEffectEcho->Feedback = dsfxEcho.fFeedback;
                    pxAudioEffectEcho->LeftDelay = dsfxEcho.fLeftDelay;
                    pxAudioEffectEcho->RightDelay = dsfxEcho.fRightDelay;
                    pxAudioEffectEcho->PanDelay = dsfxEcho.lPanDelay;
                }
                else
                {
                    dsfxEcho.fWetDryMix = PXMathLiniarF(DSFXECHO_WETDRYMIX_MIN, DSFXECHO_WETDRYMIX_MAX, 0, 1, pxAudioEffectEcho->WetDryMix);
                    dsfxEcho.fFeedback = PXMathLiniarF(DSFXECHO_FEEDBACK_MIN, DSFXECHO_FEEDBACK_MAX, 0, 1, pxAudioEffectEcho->Feedback);
                    dsfxEcho.fLeftDelay = PXMathLiniarF(DSFXECHO_LEFTDELAY_MIN, DSFXECHO_LEFTDELAY_MAX, 0, 1, pxAudioEffectEcho->LeftDelay);
                    dsfxEcho.fRightDelay = PXMathLiniarF(DSFXECHO_RIGHTDELAY_MIN, DSFXECHO_RIGHTDELAY_MAX, 0, 1, pxAudioEffectEcho->RightDelay);
                    dsfxEcho.lPanDelay = PXMathLiniarF(DSFXECHO_PANDELAY_MIN, DSFXECHO_PANDELAY_MAX, 0, 1, pxAudioEffectEcho->PanDelay);

                    result = directSoundFXEcho->lpVtbl->SetAllParameters(directSoundFXEcho, &dsfxEcho);
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    "DirectSound",
                    "FX-Update",
                    "Echo : %s\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n"
                    "%10s : %6.2f\n",
                    result == DS_OK ? "OK" : "FAIL",
                    "WetDryMix", dsfxEcho.fWetDryMix,
                    "Feedback", dsfxEcho.fFeedback,
                    "LeftDelay", dsfxEcho.fLeftDelay,
                    "RightDelay", dsfxEcho.fRightDelay,
                    "PanDelay", dsfxEcho.lPanDelay
                );

                break;
            }
            case PXAudioEffectTypeFlanger:
            {
                IDirectSoundFXFlanger8* const directSoundFXFlanger = (IDirectSoundFXFlanger8*)pxAudioDevice->FXFlanger;
                PXAudioEffectFlanger* const pxAudioEffectFlanger = &pxAudioEffect->Flanger;
                DSFXFlanger dsfxFlanger;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXFlanger->lpVtbl->GetAllParameters(directSoundFXFlanger, &dsfxFlanger);

                    pxAudioEffectFlanger->WetDryMix = dsfxFlanger.fWetDryMix;
                    pxAudioEffectFlanger->Depth = dsfxFlanger.fDepth;
                    pxAudioEffectFlanger->Feedback = dsfxFlanger.fFeedback;
                    pxAudioEffectFlanger->Frequency = dsfxFlanger.fFrequency;
                    pxAudioEffectFlanger->Waveform = dsfxFlanger.lWaveform;
                    pxAudioEffectFlanger->Delay = dsfxFlanger.fDelay;
                    pxAudioEffectFlanger->Phase = dsfxFlanger.lPhase;
                }
                else
                {
                    dsfxFlanger.fWetDryMix = pxAudioEffectFlanger->WetDryMix;
                    dsfxFlanger.fDepth = pxAudioEffectFlanger->Depth;
                    dsfxFlanger.fFeedback = pxAudioEffectFlanger->Feedback;
                    dsfxFlanger.fFrequency = pxAudioEffectFlanger->Frequency;
                    dsfxFlanger.lWaveform = pxAudioEffectFlanger->Waveform;
                    dsfxFlanger.fDelay = pxAudioEffectFlanger->Delay;
                    dsfxFlanger.lPhase = pxAudioEffectFlanger->Phase;

                    result = directSoundFXFlanger->lpVtbl->SetAllParameters(directSoundFXFlanger, &dsfxFlanger);
                }

                break;
            }
            case PXAudioEffectTypeGargle:
            {
                IDirectSoundFXGargle8* const directSoundFXGargle = (IDirectSoundFXGargle8*)pxAudioDevice->FXGargle;
                PXAudioEffectGargle* const pxAudioEffectGargle = &pxAudioEffect->Gargle;
                DSFXGargle dsfxGargle;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXGargle->lpVtbl->GetAllParameters(directSoundFXGargle, &dsfxGargle);

                    pxAudioEffectGargle->dwRateHz = dsfxGargle.dwRateHz;
                    pxAudioEffectGargle->dwWaveShape = dsfxGargle.dwWaveShape;
                }
                else
                {
                    dsfxGargle.dwRateHz = pxAudioEffectGargle->dwRateHz;
                    dsfxGargle.dwWaveShape = pxAudioEffectGargle->dwWaveShape;

                    result = directSoundFXGargle->lpVtbl->SetAllParameters(directSoundFXGargle, &dsfxGargle);
                }

                break;
            }
            case PXAudioEffectTypeInteractive3DLevel2Reverb:
            {
                IDirectSoundFXI3DL2Reverb8* const directSoundFXI3DL2Reverb = (IDirectSoundFXI3DL2Reverb8*)pxAudioDevice->FXI3DL2Reverb;
                PXAudioEffectI3DL2Reverb* const pxAudioEffectI3DL2Reverb = &pxAudioEffect->I3DL2Reverb;
                DSFXI3DL2Reverb dsfxI3DL2Reverb;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXI3DL2Reverb->lpVtbl->GetAllParameters(directSoundFXI3DL2Reverb, &dsfxI3DL2Reverb);

                    pxAudioEffectI3DL2Reverb->Room = dsfxI3DL2Reverb.lRoom;
                    pxAudioEffectI3DL2Reverb->RoomHF = dsfxI3DL2Reverb.lRoomHF;
                    pxAudioEffectI3DL2Reverb->RoomRolloffFactor = dsfxI3DL2Reverb.flRoomRolloffFactor;
                    pxAudioEffectI3DL2Reverb->DecayTime = dsfxI3DL2Reverb.flDecayTime;
                    pxAudioEffectI3DL2Reverb->DecayHFRatio = dsfxI3DL2Reverb.flDecayHFRatio;
                    pxAudioEffectI3DL2Reverb->Reflections = dsfxI3DL2Reverb.lReflections;
                    pxAudioEffectI3DL2Reverb->ReflectionsDelay = dsfxI3DL2Reverb.flReflectionsDelay;
                    pxAudioEffectI3DL2Reverb->Reverb = dsfxI3DL2Reverb.lReverb;
                    pxAudioEffectI3DL2Reverb->ReverbDelay = dsfxI3DL2Reverb.flReverbDelay;
                    pxAudioEffectI3DL2Reverb->Diffusion = dsfxI3DL2Reverb.flDiffusion;
                    pxAudioEffectI3DL2Reverb->Density = dsfxI3DL2Reverb.flDensity;
                    pxAudioEffectI3DL2Reverb->HFReference = dsfxI3DL2Reverb.flHFReference;
                }
                else
                {
                    dsfxI3DL2Reverb.lRoom = pxAudioEffectI3DL2Reverb->Room;
                    dsfxI3DL2Reverb.lRoomHF = pxAudioEffectI3DL2Reverb->RoomHF;
                    dsfxI3DL2Reverb.flRoomRolloffFactor = pxAudioEffectI3DL2Reverb->RoomRolloffFactor;
                    dsfxI3DL2Reverb.flDecayTime = pxAudioEffectI3DL2Reverb->DecayTime;
                    dsfxI3DL2Reverb.flDecayHFRatio = pxAudioEffectI3DL2Reverb->DecayHFRatio;
                    dsfxI3DL2Reverb.lReflections = pxAudioEffectI3DL2Reverb->Reflections;
                    dsfxI3DL2Reverb.flReflectionsDelay = pxAudioEffectI3DL2Reverb->ReflectionsDelay;
                    dsfxI3DL2Reverb.lReverb = pxAudioEffectI3DL2Reverb->Reverb;
                    dsfxI3DL2Reverb.flReverbDelay = pxAudioEffectI3DL2Reverb->ReverbDelay;
                    dsfxI3DL2Reverb.flDiffusion = pxAudioEffectI3DL2Reverb->Diffusion;
                    dsfxI3DL2Reverb.flDensity = pxAudioEffectI3DL2Reverb->Density;
                    dsfxI3DL2Reverb.flHFReference = pxAudioEffectI3DL2Reverb->HFReference;

                    result = directSoundFXI3DL2Reverb->lpVtbl->SetAllParameters(directSoundFXI3DL2Reverb, &dsfxI3DL2Reverb);
                }

                break;
            }
            case PXAudioEffectTypeParamEq:
            {
                IDirectSoundFXParamEq8* const directSoundFXParamEq = (IDirectSoundFXParamEq8*)pxAudioDevice->FXParamEq;
                PXAudioEffectParamEq* const pxAudioEffectParamEq = &pxAudioEffect->ParamEq;
                DSFXParamEq dsfxParamEq;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXParamEq->lpVtbl->GetAllParameters(directSoundFXParamEq, &dsfxParamEq);

                    pxAudioEffectParamEq->Center = dsfxParamEq.fCenter;
                    pxAudioEffectParamEq->Bandwidth = dsfxParamEq.fBandwidth;
                    pxAudioEffectParamEq->Gain = dsfxParamEq.fGain;

                }
                else
                {
                    dsfxParamEq.fCenter = pxAudioEffectParamEq->Center;
                    dsfxParamEq.fBandwidth = pxAudioEffectParamEq->Bandwidth;
                    dsfxParamEq.fGain = pxAudioEffectParamEq->Gain;

                    result = directSoundFXParamEq->lpVtbl->SetAllParameters(directSoundFXParamEq, &dsfxParamEq);
                }

                break;
            }
            case PXAudioEffectTypeWavesReverb:
            {
                IDirectSoundFXWavesReverb8* const directSoundFXWave = (IDirectSoundFXWavesReverb8*)pxAudioDevice->FXWavesReverb;
                PXAudioEffectWavesReverb* const pxAudioEffectWavesReverb = &pxAudioEffect->WavesReverb;
                DSFXWavesReverb wavesReverb;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXWave->lpVtbl->GetAllParameters(directSoundFXWave, &wavesReverb);

                    pxAudioEffect->WavesReverb.InGain = wavesReverb.fInGain;
                    pxAudioEffect->WavesReverb.ReverbMix = wavesReverb.fReverbMix;
                    pxAudioEffect->WavesReverb.ReverbTime = wavesReverb.fReverbTime;
                    pxAudioEffect->WavesReverb.HighFreqRTRatio = wavesReverb.fHighFreqRTRatio;
                }
                else
                {
                    wavesReverb.fInGain = PXMathLiniarF(DSFX_WAVESREVERB_INGAIN_MIN, DSFX_WAVESREVERB_INGAIN_MAX, 0, 1, pxAudioEffectWavesReverb->InGain);
                    wavesReverb.fReverbMix = PXMathLiniarF(DSFX_WAVESREVERB_REVERBMIX_MIN, DSFX_WAVESREVERB_REVERBMIX_MAX, 0, 1, pxAudioEffectWavesReverb->ReverbMix);
                    wavesReverb.fReverbTime = PXMathLiniarF(DSFX_WAVESREVERB_REVERBTIME_MIN, DSFX_WAVESREVERB_REVERBTIME_MAX, 0, 1, pxAudioEffectWavesReverb->ReverbTime);
                    wavesReverb.fHighFreqRTRatio = PXMathLiniarF(DSFX_WAVESREVERB_HIGHFREQRTRATIO_MIN, DSFX_WAVESREVERB_HIGHFREQRTRATIO_MAX, 0, 1, pxAudioEffectWavesReverb->HighFreqRTRatio);

                    result = directSoundFXWave->lpVtbl->SetAllParameters(directSoundFXWave, &wavesReverb);
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    "DirectSound",
                    "FX-Update",
                    "WavesReverb set InGain:%6.2f ReverbMix:%6.2f ReverbTime:%6.2f HighFreqRTRatio:%6.2f Res:%s",
                    wavesReverb.fInGain,
                    wavesReverb.fReverbMix,
                    wavesReverb.fReverbTime,
                    wavesReverb.fHighFreqRTRatio,
                    result == DS_OK ? "OK" : "FAIL"
                );

                break;
            }

            default:
                return PXActionRefusedArgumentInvalid;
        }
    }
#endif

    return PXActionSuccessful;
}
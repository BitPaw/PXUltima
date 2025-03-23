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
    if (!guid)
    {
        return PXTrue;
    }

    ++(*amount);

    return PXTrue;
}

BOOL CALLBACK PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDeviceDetectObjectEventData* const pxAudioDeviceDetectObjectEventData)
{
    if (!guid) // First device gets listed twice for no reason. GUID is NULL for first element.
    {
        return PXTrue;
    }

    if (pxAudioDeviceDetectObjectEventData->IndexCurrent > pxAudioDeviceDetectObjectEventData->IndexTarget)
    {
        return PXTrue;
    }

    if (pxAudioDeviceDetectObjectEventData->IndexCurrent != pxAudioDeviceDetectObjectEventData->IndexTarget)
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

PXActionResult PXAPI PXDirectSoundInitialize(PXAudioDirectSound* const pxAudioDirectSound, PXAudioInitializeInfo* const pxAudioInitializeInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectSound",
        "Initialize",
        "---Start---"
    );
#endif

    // Library Open
    {
        const PXActionResult libraryOpenResult = PXLibraryOpenA(&pxAudioDirectSound->DirectSoundLibrary, "DSOUND.DLL");

        if(PXActionSuccessful != libraryOpenResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectSound",
        "Initialize",
        "Fetching functions"
    );
#endif

    {
        PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
        {
            // OpenGL Standard functions
            { &pxAudioDirectSound->SoundCreate, "DirectSoundCreate"},
            { &pxAudioDirectSound->SoundEnumerateA, "DirectSoundEnumerateA"},
            { &pxAudioDirectSound->SoundEnumerateW, "DirectSoundEnumerateW"},
            { &pxAudioDirectSound->CanUnloadNow, "DllCanUnloadNow"},
            { &pxAudioDirectSound->ClassObjectGet, "DllGetClassObject"},
            { &pxAudioDirectSound->SoundCaptureCreate, "DirectSoundCaptureCreate"},
            { &pxAudioDirectSound->SoundCaptureEnumerateA, "DirectSoundCaptureEnumerateA"},
            { &pxAudioDirectSound->SoundCaptureEnumerateW, "DirectSoundCaptureEnumerateW"},

            { &pxAudioDirectSound->SoundCreate8, "DirectSoundCreate8"},
            { &pxAudioDirectSound->SoundCaptureCreate8, "DirectSoundCaptureCreate8"},

        };

        const PXSize amount = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&pxAudioDirectSound->DirectSoundLibrary, pxLibraryFuntionEntry, amount);
    }

    // Link functions
    {
        PXAudio* const pxAudio = pxAudioInitializeInfo->AudioReference;

        pxAudio->DeviceAmount = (PXAudioDeviceAmountFunction)PXDirectSoundDeviceAmount;
        pxAudio->DeviceFetch = PXDirectSoundDeviceFetch;
        pxAudio->DeviceFetchAll = PXDirectSoundDeviceFetchAll;
        pxAudio->DeviceOpen = PXDirectSoundDeviceOpen;
        pxAudio->DeviceClose = PXDirectSoundDeviceClose;
        pxAudio->DeviceLoad = PXDirectSoundDeviceLoad;
        pxAudio->DevicePitchIncrease = PXDirectSoundDevicePitchIncrease;
        pxAudio->DevicePitchSet = PXDirectSoundDevicePitchSet;
        pxAudio->DevicePitchReduce = PXDirectSoundDevicePitchReduce;
        pxAudio->DeviceVolumeGet = PXDirectSoundDeviceVolumeGet;
        pxAudio->DeviceVolumeSetEqual = PXDirectSoundDeviceVolumeSetEqual;
        pxAudio->DeviceVolumeSetIndividual = PXDirectSoundDeviceVolumeSetIndividual;
        pxAudio->DeviceStart = PXDirectSoundDeviceStart;
        pxAudio->DeviceRestart = PXDirectSoundDeviceRestart;
        pxAudio->DeviceStop = PXDirectSoundDeviceStop;
        pxAudio->DevicePause = PXDirectSoundDevicePause;
        pxAudio->PlayCursorSet = PXDirectSoundDevicePlayCursorSet;
        pxAudio->PlayCursorGet = PXDirectSoundDevicePlayCursorGet;
        pxAudio->PlaySpeedSet = PXDirectSoundDevicePlaySpeedSet;
        pxAudio->PlaySpeedGet = PXDirectSoundDevicePlaySpeedGet;
        pxAudio->PositionSet = PXDirectSoundDevicePositionSet;
        pxAudio->PositionGet = PXDirectSoundDevicePositionGet;
        pxAudio->ConeAnglesSet = PXDirectSoundDeviceConeAnglesSet;
        pxAudio->ConeAnglesGet = PXDirectSoundDeviceConeAnglesGet;
        pxAudio->ConeOrientationGet = PXDirectSoundDeviceConeOrientationGet;
        pxAudio->ConeOrientationSet = PXDirectSoundDeviceConeOrientationSet;
        pxAudio->ConeOutsideVolumeGet = PXDirectSoundDeviceConeOutsideVolumeGet;
        pxAudio->ConeOutsideVolumeSet = PXDirectSoundDeviceConeOutsideVolumeSet;
        pxAudio->DistanceMaxGet = PXDirectSoundDeviceDistanceMaxGet;
        pxAudio->DistanceMaxSet = PXDirectSoundDeviceDistanceMaxSet;
        pxAudio->DistanceMinGet = PXDirectSoundDeviceDistanceMinGet;
        pxAudio->DistanceMinSet = PXDirectSoundDeviceDistanceMinSet;
        pxAudio->ModeGet = PXDirectSoundDeviceModeGet;
        pxAudio->ModeSet = PXDirectSoundDeviceModeSet;
        pxAudio->VelocityGet = PXDirectSoundDeviceVelocityGet;
        pxAudio->VelocitySet = PXDirectSoundDeviceVelocitySet;
        pxAudio->DistanceFactorGet = PXDirectSoundDeviceDistanceFactorGet;
        pxAudio->DistanceFactorSet = PXDirectSoundDeviceDistanceFactorSet;
        pxAudio->DopplerFactorGet = PXDirectSoundDeviceDopplerFactorGet;
        pxAudio->DopplerFactorSet = (PXAudioDeviceDopplerFactorSetFunction)PXDirectSoundDeviceDopplerFactorSet;
        pxAudio->OrientationGet = (PXAudioDeviceOrientationGetFunction)PXDirectSoundDeviceOrientationGet;
        pxAudio->OrientationSet = (PXAudioDeviceOrientationSetFunction)PXDirectSoundDeviceOrientationSet;
        pxAudio->RolloffFactorGet = (PXAudioDeviceRolloffFactorGetFunction)PXDirectSoundDeviceRolloffFactorGet;
        pxAudio->RolloffFactorSet = (PXAudioDeviceRolloffFactorSetFunction)PXDirectSoundDeviceRolloffFactorSet;
        pxAudio->DeferredSettingsCommit = (PXAudioDeviceDeferredSettingsCommitFunction)PXDirectSoundDeviceDeferredSettingsCommit;


        pxAudio->DeviceEffectUpdate = PXDirectSoundEffectUpdate;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "DirectSound",
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
        PXDirectSoundCaptureEnumerateA pxDirectSoundCaptureEnumerateA = (PXDirectSoundCaptureEnumerateA)pxAudioDirectSound->SoundCaptureEnumerateA;
        PXLPDSENUMCALLBACKA callBack = (PXLPDSENUMCALLBACKA)PXAudioDeviceDetectAmountCallBack;

        enumResultID = pxDirectSoundCaptureEnumerateA(callBack, &pxAudioDeviceAmountInfo->DeviceInput);
    }
    //-----------------------------------------------------


    //-----------------------------------------------------
    // Output
    //-----------------------------------------------------
    {
        PXDirectSoundEnumerateA pxDirectSoundEnumerateA = (PXDirectSoundEnumerateA)pxAudioDirectSound->SoundCaptureEnumerateA;
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

    switch (pxAudioDeviceType)
    {
    case PXAudioDeviceTypeInput:
    {
        PXDirectSoundEnumerateA pxDirectSoundEnumerateA = pxAudioDirectSound->SoundCaptureEnumerateA;

        enumResultID = pxDirectSoundEnumerateA
                       (
                           PXAudioDeviceDetectObjectCallBack,
                           &pxAudioDeviceDetectObjectEventData
                       );
        break;
    }
    case PXAudioDeviceTypeOutput:
    {
        PXDirectSoundCaptureEnumerateA pxDirectSoundCaptureEnumerateA = pxAudioDirectSound->SoundCaptureEnumerateA;

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
    for (PXSize i = 0; i < amount; ++i)
    {
        PXDirectSoundDeviceFetch(pxAudioDirectSound, pxAudioDeviceType, i, &pxAudioDevice[i]);
    }

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOpen(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID)
{
#if OSUnix
#elif OSWindows
    pxAudioDevice->Type = pxAudioDeviceType;

    switch (pxAudioDeviceType)
    {
    case PXAudioDeviceTypeInput:
    {
        IDirectSoundCapture8* directSoundCapture;
        HRESULT crateResultID = 0;

        if(pxAudioDirectSound->SoundCaptureCreate8) // Can create 8?
        {
            PXDirectSoundCaptureCreate8 pxDirectSoundCaptureCreate8 = (PXDirectSoundCaptureCreate8)pxAudioDirectSound->SoundCaptureCreate8;

            crateResultID = pxDirectSoundCaptureCreate8(PXNull, (IDirectSoundCapture8**)&pxAudioDirectSound->DirectSoundInterface, PXNull);
        }
        else
        {
            PXDirectSoundCaptureCreate pxDirectSoundCaptureCreate = (PXDirectSoundCaptureCreate)pxAudioDirectSound->SoundCaptureCreate;

            crateResultID = pxDirectSoundCaptureCreate(PXNull, (IDirectSoundCapture**)&pxAudioDirectSound->DirectSoundInterface, PXNull);
        }

        const PXActionResult crateResult = PXWindowsHandleErrorFromID(crateResultID);

        PXActionReturnOnError(crateResult);

        directSoundCapture = (IDirectSoundCapture8*)pxAudioDirectSound->DirectSoundInterface;

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

            if(pxAudioDirectSound->SoundCreate8) // Can create 8?
            {
                PXDirectSoundCaptureCreate8 pxDirectSoundCaptureCreate8 = (PXDirectSoundCaptureCreate8)pxAudioDirectSound->SoundCreate8;

                crateResultID = pxDirectSoundCaptureCreate8(PXNull, (PXDirectSoundOutputInterface**)&pxAudioDirectSound->DirectSoundInterface, PXNull);
            }
            else
            {
                PXDirectSoundCaptureCreate pxDirectSoundCaptureCreate = (PXDirectSoundCaptureCreate)pxAudioDirectSound->SoundCreate;

                crateResultID = pxDirectSoundCaptureCreate(PXNull, (PXDirectSoundOutputInterface**)&pxAudioDirectSound->DirectSoundInterface, PXNull);
            }

            const PXActionResult crateResult = PXWindowsHandleErrorFromID(crateResultID);

            PXActionReturnOnError(crateResult);

            directSound = (PXDirectSoundOutputInterface*)pxAudioDirectSound->DirectSoundInterface;
        }

        /// Capability
        {
            DSCAPS capabiltys;
            PXClear(DSCAPS,&capabiltys);
            capabiltys.dwSize = sizeof(DSCAPS);

            const HRESULT capResultID = directSound->lpVtbl->GetCaps(directSound, &capabiltys);
            const PXActionResult initResult = PXWindowsHandleErrorFromID(capResultID);

            PXActionReturnOnError(initResult);

            pxAudioDevice->SupportFlags = capabiltys.dwFlags;

        }

        {
            HWND windowHandle = GetForegroundWindow();

            if (windowHandle == NULL)
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

PXActionResult PXAPI PXDirectSoundDeviceClose(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceLoad(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXSound* const pxSound)
{
    if(!(pxAudioDirectSound && pxAudioDevice && pxSound))
    {
        return PXActionRefusedArgumentNull;
    }
#if OSUnix
#elif OSWindows

    PXDirectSoundOutputInterface* const directSound = (PXDirectSoundOutputInterface*)pxAudioDirectSound->DirectSoundInterface;
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


        if (canUse3DStuff)
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

        if (dataBlockAdressB)
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

    if (canUse3DStuff)
    {
        const HRESULT bufferResultID = soundBuffer->lpVtbl->QueryInterface(soundBuffer, &IID_IDirectSound3DBuffer, &pxAudioDevice->Buffer3DInterface);
        const PXActionResult bufferResult = PXWindowsHandleErrorFromID(bufferResultID);
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlayCursorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXSize offset)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows
    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    const HRESULT setResultID = soundBuffer->lpVtbl->SetCurrentPosition(soundBuffer, offset);
    const PXActionResult setResult = PXWindowsHandleErrorFromID(setResultID);

    PXActionReturnOnError(setResult);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlayCursorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXSize* const offset)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        DWORD positionRead = 0;
        DWORD positionWrite = 0;

        const HRESULT getResultID = soundBuffer->lpVtbl->GetCurrentPosition(soundBuffer, &positionRead, &positionWrite);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);

        *offset = positionRead;
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlaySpeedSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }
#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        const HRESULT getResultID = soundBuffer->lpVtbl->SetFrequency(soundBuffer, frequency);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePlaySpeedGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        DWORD frequencyDW = 0;

        const HRESULT getResultID = soundBuffer->lpVtbl->GetFrequency(soundBuffer, &frequencyDW);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);

        *frequency = frequencyDW;
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePositionSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

    if (!directSound3DBuffer)
    {
        return PXActionRefuedObjectInterfaceNotAvailable;
    }

    const HRESULT getResultID = directSound3DBuffer->lpVtbl->SetPosition(directSound3DBuffer,x, y, z, DS3D_IMMEDIATE);
    const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

    PXActionReturnOnError(getResult);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePositionGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

    if (!directSound3DBuffer)
    {
        return PXActionRefuedObjectInterfaceNotAvailable;
    }

    D3DVECTOR vector;

    const HRESULT getResultID = directSound3DBuffer->lpVtbl->GetPosition(directSound3DBuffer, &vector);
    const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

    PXActionReturnOnError(getResult);

    *x = vector.x;
    *y = vector.y;
    *z = vector.z;

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceConeAnglesSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeAnglesGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOrientationGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOrientationSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDevicePitchIncrease(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32 amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDevicePitchSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const unsigned int pitch)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDevicePitchReduce(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32 amount)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceVolumeGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, unsigned short* const volume)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        LONG volumeL = 0;

        const HRESULT getResultID = soundBuffer->lpVtbl->GetVolume(soundBuffer, &volumeL);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);

        *volume = volumeL;
    }

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceVolumeSetEqual(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const unsigned int volume)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        const HRESULT getResultID = soundBuffer->lpVtbl->SetVolume(soundBuffer, volume); // DSBPLAY_LOOPING
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);
    }

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceVolumeSetIndividual(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceStart(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        const HRESULT getResultID = soundBuffer->lpVtbl->Play(soundBuffer, 0, 0, DSBPLAY_LOOPING);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);
    }

    pxAudioDevice->IsRunning = PXTrue;

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceRestart(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
    const PXActionResult cursorResetResult = PXDirectSoundDevicePlayCursorSet(pxAudioDirectSound, pxAudioDevice, 0);

    const PXActionResult startResult = PXDirectSoundDeviceStart(pxAudioDirectSound, pxAudioDevice);

    return startResult;
}

PXActionResult PXAPI PXDirectSoundDeviceStop(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows

    IDirectSoundBuffer* const soundBuffer = (IDirectSoundBuffer*)pxAudioDevice->SoundBuffer;

    if (!soundBuffer)
    {
        return PXActionRefusedArgumentInvalid;
    }

    {
        DWORD frequencyDW = 0;

        const HRESULT getResultID = soundBuffer->lpVtbl->Stop(soundBuffer);
        const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

        PXActionReturnOnError(getResult);
    }

    pxAudioDevice->IsRunning = PXFalse;

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDevicePause(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
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

                pxAudioEffectGargle->dwRateHz =    dsfxGargle.dwRateHz;
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

PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32* const pflMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXF32 flMaxDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMinGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32* const pflMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceMinSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXF32 flMinDistance)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceModeGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceModeSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceVelocityGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXF32* const x, PXF32* const y, PXF32* const z)
{
    if (!(pxAudioDirectSound && pxAudioDevice))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
#elif OSWindows
    IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

    if (!directSound3DBuffer)
    {
        return PXActionRefuedObjectInterfaceNotAvailable;
    }

    D3DVECTOR vector;

    const HRESULT getResultID = directSound3DBuffer->lpVtbl->GetVelocity(directSound3DBuffer, &vector);
    const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

    PXActionReturnOnError(getResult);

    *x = vector.x;
    *y = vector.y;
    *z = vector.z;
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOrientationGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceOrientationSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceDeferredSettingsCommit(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXDirectSoundDeviceVelocitySet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXF32 x, const PXF32 y, const PXF32 z)
{
#if OSUnix
#elif OSWindows
    if (!pxAudioDirectSound || !pxAudioDevice)
    {
        return PXActionRefusedArgumentNull;
    }

    IDirectSound3DBuffer8* const directSound3DBuffer = (IDirectSound3DBuffer8*)pxAudioDevice->Buffer3DInterface;

    if (!directSound3DBuffer)
    {
        return PXActionRefuedObjectInterfaceNotAvailable;
    }

    const HRESULT getResultID = directSound3DBuffer->lpVtbl->SetVelocity(directSound3DBuffer, x, y, z, DS3D_DEFERRED);
    const PXActionResult getResult = PXWindowsHandleErrorFromID(getResultID);

    PXActionReturnOnError(getResult);
#endif

    return PXActionSuccessful;
}

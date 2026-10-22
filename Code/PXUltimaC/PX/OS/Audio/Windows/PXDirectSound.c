#include "PXDirectSound.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>
#include <PX/Algorithm/Chorus/PXChorus.h>
#include <PX/Algorithm/Compressor/PXCompressor.h>
#include <PX/Algorithm/Distortion/PXDistortion.h>
#include <PX/Algorithm/Flanger/PXFlanger.h>
#include <PX/Algorithm/Gargle/PXGargle.h>
#include <PX/Algorithm/Reverb/PXReverb.h>
#include <PX/Algorithm/Equaliser/PXEqualiser.h>
#include <PX/Algorithm/Echo/PXEcho.h>
#include <PX/Engine/PXEngineAudio.h>


#if OSUnix
#pragma warning(comment, "DirectX does not exist under linux")
#elif OSWindows

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "uuid.lib")

#include <windows.h>

#if WindowsAtleastVista
#include <mmeapi.h> // Windows Vista
#else
#include <MMSystem.h> // Windows XP
#endif
#include <dsound.h> // Windows Direct Sound
//#include <strmif.h>
//#include <ksproxy.h>
//#include <dshow.h>

typedef BOOL(CALLBACK* PXLPDSENUMCALLBACKA)(struct _GUID*, char*, char*, void*);
typedef BOOL(CALLBACK* PXLPDSENUMCALLBACKW)(struct _GUID*, wchar_t*, wchar_t*, void*);
typedef HRESULT(WINAPI* PXDirectSoundCreate)(struct _GUID* pcGuidDevice, void* ppDS, void* pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundCreate8)(__in_opt LPCGUID pcGuidDevice, __deref_out LPDIRECTSOUND8* ppDS8, __null LPUNKNOWN pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundEnumerateA)(PXLPDSENUMCALLBACKA pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDirectSoundEnumerateW)(PXLPDSENUMCALLBACKW pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDirectSoundCaptureCreate)(struct _GUID* pcGuidDevice, void* ppDSC, void* pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundCaptureCreate8)(__in_opt LPCGUID pcGuidDevice, __deref_out LPDIRECTSOUNDCAPTURE8* ppDSC8, __null LPUNKNOWN pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundCaptureEnumerateA)(PXLPDSENUMCALLBACKA pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDirectSoundCaptureEnumerateW)(PXLPDSENUMCALLBACKW pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDllGetClassObject)(REFCLSID rclsid, REFIID riid, void* ppv);
typedef HRESULT(WINAPI* PXDllCanUnloadNow)(void);

#endif


typedef struct PXDirectSoundBinding_
{
    PXDirectSoundCaptureCreate CaptureCreate;
    PXDirectSoundCaptureCreate8 CaptureCreate8;
    PXDirectSoundCaptureEnumerateA CaptureEnumerateA;
    PXDirectSoundCaptureEnumerateW CaptureEnumerateW;
    PXDirectSoundCreate Create;
    PXDirectSoundCreate8 Create8;
    PXDirectSoundEnumerateA EnumerateA;
    PXDirectSoundEnumerateW EnumerateW;
    void* DirectSoundFullDuplexCreate;
    void* DllCanUnloadNow;
    PXDllGetClassObject DllGetClassObject;
    void* GetDeviceID;
}
PXDirectSoundBinding;


// Direct Sound
typedef struct PXDirectSound_
{
    PXLibrary DirectSoundLibrary;
    PXDirectSoundBinding Binding; 
}
PXDirectSound;

PXDirectSound _pxDirectSound;

const char PXDirectSoundText[] = "DirectSound";
const char PXDirectSoundLibrary[] = "DSOUND.DLL";
const PXI8U PXDirectSoundLibraryLength = sizeof(PXDirectSoundLibrary);

const char PXDirectSoundBindingList[] =
"DirectSoundCaptureCreate\0"
"DirectSoundCaptureCreate8\0"
"DirectSoundCaptureEnumerateA\0"
"DirectSoundCaptureEnumerateW\0"
"DirectSoundCreate\0"
"DirectSoundCreate8\0"
"DirectSoundEnumerateA\0"
"DirectSoundEnumerateW\0"
"DirectSoundFullDuplexCreate\0"
"DllCanUnloadNow\0"
"DllGetClassObject\0"
"GetDeviceID\0";

const PXI8U PXDirectSoundBindingAmount = sizeof(PXDirectSoundBinding) / sizeof(void*);


PXResult PXAPI PXDirectSoundInitialize(PXEngineAudio PXREF pxEngineAudio, PXEngineAudioCreateInfo PXREF pxEngineAudioCreateInfo)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    if(!(pxEngineAudio && pxEngineAudioCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXClear(PXDirectSound, &_pxDirectSound);

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
        PXText pxText;
        PXTextFromAddressA(&pxText, PXDirectSoundLibrary, PXDirectSoundLibraryLength, PXDirectSoundLibraryLength);

        pxResult = PXLibraryOpen(&_pxDirectSound.DirectSoundLibrary, &pxText);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }
    
    pxResult = PXLibraryGetSymbolBinding
    (
        &_pxDirectSound.DirectSoundLibrary,
        &_pxDirectSound.Binding,
        PXDirectSoundBindingList,
        PXDirectSoundBindingAmount,
        PXTrue
    );

    if(PXResultOK != pxResult)
    {
        return pxResult; // Function fetch failed
    }
    
    // Link functions
    pxEngineAudio->DeviceInputAmount = PXDirectSoundInputAmount;
    pxEngineAudio->DeviceInputIndex = PXDirectSoundInputIndex;
    pxEngineAudio->DeviceInputOpen = PXDirectSoundInputOpen;
    pxEngineAudio->DeviceInputClose = PXDirectSoundInputClose;
    pxEngineAudio->DeviceOutputAmount = PXDirectSoundOutputAmount;
    pxEngineAudio->DeviceOutputIndex = PXDirectSoundOutputIndex;
    pxEngineAudio->DeviceOutputOpen = PXDirectSoundOutputOpen;
    pxEngineAudio->DeviceOutputClose = PXDirectSoundOutputClose;
    pxEngineAudio->PlaybackStart = PXNull;
    pxEngineAudio->PlaybackStop = PXNull;
    pxEngineAudio->MasterVolumeSet = PXNull;
    pxEngineAudio->ListenerCreate = PXDirectSoundListenerCreate;
    pxEngineAudio->ListenerDestroy = PXDirectSoundListenerDestroy;
    pxEngineAudio->ListenerStagedUpdatesCommit = PXDirectSoundListenerStagedUpdatesCommit;
    pxEngineAudio->ListenerPositionSet = PXDirectSoundListenerPositionSet;
    pxEngineAudio->ListenerOrientationSet = PXDirectSoundListenerOrientationSet;
    pxEngineAudio->EmitterCreate = PXDirectSoundEmitterCreate;
    pxEngineAudio->EmitterDestroy = PXDirectSoundEmitterDestroy;
    pxEngineAudio->EmitterPlay = PXDirectSoundEmitterPlay;
    pxEngineAudio->EmitterStop = PXDirectSoundEmitterStop;
    pxEngineAudio->EmitterPositionSet = PXDirectSoundEmitterPositionSet;
    pxEngineAudio->EmitterVelocitySet = PXDirectSoundEmitterVelocitySet;
    pxEngineAudio->EmitterVolumeSet = PXDirectSoundEmitterVolumeSet;
    pxEngineAudio->EmitterPitchSet = PXDirectSoundEmitterPitchSet;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXDirectSoundText,
        "Initialize",
        "---Done---"
    );
#endif

    return PXResultOK;
}

BOOL CALLBACK PXDirectSoundDeviceCallBackCount
(
    LPGUID guid,
    LPCWSTR cstrDescription,
    LPCWSTR cstrModule,
    PXSize PXREF counter
)
{
    if(!guid)
    {
        *counter = 0; // Is only null on first object
        return PXTrue;
    }

    ++(*counter);

    return PXTrue;
}

BOOL CALLBACK PXAudioDeviceInputDetectCallBack
(
    LPGUID guid,
    LPCWSTR cstrDescription,
    LPCWSTR cstrModule,
    PXEngineAudio PXREF pxEngineAudio
)
{
    if(!guid)
    {
        return PXTrue; // Is only null on first object
    }

    PXAudioDeviceCreateInfo pxAudioDeviceCreateInfo;
    PXClear(PXAudioDeviceCreateInfo, &pxAudioDeviceCreateInfo);
    PXTextFromAddressW(&pxAudioDeviceCreateInfo.Info.Name, cstrDescription, PXTextLengthUnkown, PXTextLengthUnkown);
    pxAudioDeviceCreateInfo.WindowsGUID = *guid;
    pxAudioDeviceCreateInfo.Type = PXAudioDeviceTypeInput;

    PXAudioDeviceDetect(pxEngineAudio, &pxAudioDeviceCreateInfo);

    return PXTrue;
}

BOOL CALLBACK PXAudioDeviceOutputDetectCallBack
(
    LPGUID guid,
    LPCWSTR cstrDescription,
    LPCWSTR cstrModule,
    PXEngineAudio PXREF pxEngineAudio
)
{
    if(!guid)
    {
        return PXTrue; // Is only null on first object
    }

    PXAudioDeviceCreateInfo pxAudioDeviceCreateInfo;
    PXClear(PXAudioDeviceCreateInfo, &pxAudioDeviceCreateInfo);
    PXTextFromAddressW(&pxAudioDeviceCreateInfo.Info.Name, cstrDescription, PXTextLengthUnkown, PXTextLengthUnkown);
    pxAudioDeviceCreateInfo.WindowsGUID = *guid;
    pxAudioDeviceCreateInfo.Type = PXAudioDeviceTypeOutput;

    PXAudioDeviceDetect(pxEngineAudio, &pxAudioDeviceCreateInfo);

    return PXTrue;
}

PXSize PXAPI PXDirectSoundInputAmount(PXEngineAudio PXREF pxEngineAudio)
{
    PXSize counter = 0;

#if OSUnix
#elif OSWindows
    HRESULT result = 0;
    PXResult pxResult = PXResultInvalid;
    PXLPDSENUMCALLBACKW callBack = (PXLPDSENUMCALLBACKW)PXDirectSoundDeviceCallBackCount;
    
    // Get elements first
    result = _pxDirectSound.Binding.CaptureEnumerateW(callBack, &counter);

    // Ensure space
    // ???

    // Pull again, to fetch elements
    callBack = (PXLPDSENUMCALLBACKW)PXAudioDeviceInputDetectCallBack;
    result = _pxDirectSound.Binding.CaptureEnumerateW(callBack, pxEngineAudio);

    pxResult = PXErrorFromHRESULT(result);
#endif

    return counter;
}

PXAudioDevice* PXAPI PXDirectSoundInputIndex(PXEngineAudio PXREF pxEngineAudio, const PXSize index)
{
    PXAudioDevice PXREF pxAudioDevice = (PXAudioDevice*)PXDictionaryECSGetViaIndex(&pxEngineAudio->DeviceInputLookup, index);

    return pxAudioDevice;
}

PXResult PXAPI PXDirectSoundInputOpen(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;
    PXBool useDirectSound8 = _pxDirectSound.Binding.CaptureCreate8 > 0;

    if(PXAudioDeviceTypeInput != pxAudioDevice->Type)
    {
        return PXAudioDeviceTypeInvalid;
    }

    if(useDirectSound8) // Can create 8?
    {
        resultID = _pxDirectSound.Binding.CaptureCreate8
        (
            PXNull,
            &pxAudioDevice->Input.DS8,
            PXNull
        );
    }
    else
    {
        resultID = _pxDirectSound.Binding.CaptureCreate
        (
            PXNull,
            &pxAudioDevice->Input.DS,
            PXNull
        );
    }

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
        return pxResult;


    IDirectSoundCapture8* directSound8 = pxAudioDevice->Input.DS8;
    IDirectSoundCapture* directSound = pxAudioDevice->Input.DS;


    DSCCAPS capabiltys;
    PXClear(DSCCAPS, &capabiltys);
    capabiltys.dwSize = sizeof(DSCCAPS);

    if(useDirectSound8)
    {
#if PXLanguageCPP
        resultID = directSound8->GetCaps(&capabiltys);
#else
        resultID = directSound8->lpVtbl->GetCaps(directSound8, &capabiltys);
#endif
    }
    else
    {
#if PXLanguageCPP
        resultID = directSound->GetCaps(&capabiltys);
#else
        resultID = directSound->lpVtbl->GetCaps(directSound, &capabiltys);
#endif
    }

    pxResult = PXErrorFromHRESULT(resultID);

    pxAudioDevice->SupportFlags = capabiltys.dwFlags;
    pxAudioDevice->FormatSupportFlags = capabiltys.dwFormats;
    pxAudioDevice->Channels = capabiltys.dwChannels;

    return pxResult;
}

PXResult PXAPI PXDirectSoundInputClose(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice)
{
    return PXResultOK;
}

PXSize PXAPI PXDirectSoundOutputAmount(PXEngineAudio PXREF pxEngineAudio)
{
    if(!pxEngineAudio)
    {
        return 0;
    }

    PXSize counter = 0;

#if OSUnix
#elif OSWindows
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    PXLPDSENUMCALLBACKW callBack = (PXLPDSENUMCALLBACKW)PXDirectSoundDeviceCallBackCount;

    resultID = _pxDirectSound.Binding.EnumerateW(callBack, &counter);
    pxResult = PXErrorFromHRESULT(resultID);

    callBack = (PXLPDSENUMCALLBACKW)PXAudioDeviceOutputDetectCallBack;

    resultID = _pxDirectSound.Binding.EnumerateW(callBack, pxEngineAudio);
    pxResult = PXErrorFromHRESULT(resultID);
#endif

    return counter;
}

PXAudioDevice* PXAPI PXDirectSoundOutputIndex(PXEngineAudio PXREF pxEngineAudio, const PXSize index)
{
    if(!pxEngineAudio)
    {
        return PXNull;
    }

    PXAudioDevice PXREF pxAudioDevice = (PXAudioDevice*)PXDictionaryECSGetViaIndex(&pxEngineAudio->DeviceOutputLookup, index);

    return pxAudioDevice;
}

PXResult PXAPI PXDirectSoundOutputOpen(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice)
{
    if(!(pxEngineAudio && pxAudioDevice))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;
    PXBool useDirectSound8 = _pxDirectSound.Binding.Create8 > 0;

    if(!(pxEngineAudio && pxAudioDevice))
    {
        return PXResultRefusedParameterNull;
    }

    if(PXAudioDeviceTypeOutput != pxAudioDevice->Type)
    {
        return PXAudioDeviceTypeInvalid;
    }

    if(useDirectSound8) // Can create 8?
    {
        resultID = _pxDirectSound.Binding.Create8
        (
            PXNull,
            &pxAudioDevice->Output.DS8,
            PXNull
        );
    }
    else
    {
        resultID = _pxDirectSound.Binding.Create
        (
            PXNull,
            &pxAudioDevice->Output.DS,
            PXNull
        );
    }

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
        return pxResult;


    IDirectSound8* directSound8 = pxAudioDevice->Output.DS8;
    IDirectSound* directSound = pxAudioDevice->Output.DS;

    //-----------------------------------------------------
    // Capability
    DSCAPS capabiltys;
    PXClear(DSCAPS, &capabiltys);
    capabiltys.dwSize = sizeof(DSCAPS);

    if(useDirectSound8)
    {
#if PXLanguageCPP
        resultID = directSound8->GetCaps(&capabiltys);
#else
        resultID = directSound8->lpVtbl->GetCaps(directSound, &capabiltys);
#endif
    }
    else
    {
#if PXLanguageCPP
        resultID = directSound->GetCaps(&capabiltys);
#else
        resultID = directSound->lpVtbl->GetCaps(directSound, &capabiltys);
#endif
    }

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
        return pxResult;

    pxAudioDevice->SupportFlags = capabiltys.dwFlags;
    //-----------------------------------------------------

 

    HWND windowHandle = GetForegroundWindow();

    if(windowHandle == NULL)
    {
        windowHandle = GetDesktopWindow();
    }

    DWORD cooperativeLevel = DSSCL_PRIORITY;

    if(useDirectSound8)
    {
#if PXLanguageCPP
        resultID = directSound8->SetCooperativeLevel(windowHandle, cooperativeLevel);
#else
        resultID = directSound8->lpVtbl->SetCooperativeLevel(directSound, windowHandle, cooperativeLevel);
#endif
    }
    else
    {
#if PXLanguageCPP
        resultID = directSound->SetCooperativeLevel(windowHandle, cooperativeLevel);
#else
        resultID = directSound->lpVtbl->SetCooperativeLevel(directSound, windowHandle, cooperativeLevel);
#endif
    }

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;    
}

PXResult PXAPI PXDirectSoundOutputClose(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice)
{
    if(!(pxEngineAudio && pxAudioDevice))
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultOK;
}

PXResult PXAPI PXDirectSoundListenerCreate
(
    PXEngineAudio PXREF pxEngineAudio, 
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioListener PXREF pxAudioListener
)
{
    if(!(pxEngineAudio && pxAudioListener && pxAudioListener))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound8 PXREF directSound8 = pxAudioDevice->Output.DS8;
    IDirectSound PXREF directSound = pxAudioDevice->Output.DS;

    // Describe the primary hardware buffer for listening.
    // We dont set anything here, as settings will be defined by the device itself.
    DSBUFFERDESC buffer_desc;
    PXClear(DSBUFFERDESC, &buffer_desc);
    buffer_desc.dwSize = sizeof(DSBUFFERDESC);
    // DSBCAPS_PRIMARYBUFFER=Listening, DSBCAPS_CTRL3D= Allow processing of 3D Emitters.
    buffer_desc.dwFlags = 
        DSBCAPS_PRIMARYBUFFER | // Required, otherwise its an emitter
        DSBCAPS_CTRL3D; // Required for IID_IDirectSound3DListener

    // DSBCAPS_CTRLPAN and DSBCAPS_CTRLVOLUME are incompatible with DSBCAPS_CTRL3D.

    // Create Master Primary Buffer
#if PXLanguageCPP
    resultID = directSound8->CreateSoundBuffer(&buffer_desc, &pxAudioListener->BufferHandle, NULL);
#else
    resultID = directSound8->lpVtbl->CreateSoundBuffer(directSound8, &buffer_desc, &pxAudioListener->BufferHandle, NULL);
#endif

    pxResult = PXErrorFromHRESULT(resultID);


    IDirectSoundBuffer* directSoundBuffer = pxAudioListener->BufferHandle;

    // Extract the Listener interface out of the primary buffer
#if PXLanguageCPP
    resultID = directSoundBuffer->QueryInterface
    (
        IID_IDirectSound3DListener, 
        (void**)&pxAudioListener->Listener3D
    );
#else
    resultID = directSoundBuffer->lpVtbl->QueryInterface
    (
        directSoundBuffer,
        &IID_IDirectSound3DListener,
        (void**)&pxAudioListener->Listener3D
    );
#endif
    pxResult = PXErrorFromHRESULT(resultID);



    IDirectSound3DListener PXREF directSound3DListener = pxAudioListener->Listener3D;

    // Optional optimization: Set the global 3D distance model to match real physics
    // 1.0f = meters
#if PXLanguageCPP
    directSound3DListener->SetDistanceFactor(1.0f, DS3D_IMMEDIATE);
#else
    directSound3DListener->lpVtbl->SetDistanceFactor(directSound3DListener, 1.0f, DS3D_IMMEDIATE);
#endif

    DS3DLISTENER ds3DListener;
    ds3DListener.dwSize = sizeof(DS3DLISTENER);


#if PXLanguageCPP
    resultID = directSound3DListener->GetAllParameters(&ds3DListener);
#else
    resultID = directSound3DListener->lpVtbl->GetAllParameters(directSound3DListener, &ds3DListener);
#endif

    pxAudioListener->Position.X = ds3DListener.vPosition.x;
    pxAudioListener->Position.Y = ds3DListener.vPosition.y;
    pxAudioListener->Position.Z = ds3DListener.vPosition.z;
    pxAudioListener->Position.W = 0.0f;

    pxAudioListener->Velocity.X = ds3DListener.vVelocity.x;
    pxAudioListener->Velocity.Y = ds3DListener.vVelocity.y;
    pxAudioListener->Velocity.Z = ds3DListener.vVelocity.z;
    pxAudioListener->Velocity.W = 0.0f;

    pxAudioListener->OrientFront.X = ds3DListener.vOrientFront.x;
    pxAudioListener->OrientFront.Y = ds3DListener.vOrientFront.y;
    pxAudioListener->OrientFront.Z = ds3DListener.vOrientFront.z;
    pxAudioListener->OrientFront.W = 0.0f;

    pxAudioListener->OrientTop.X = ds3DListener.vOrientTop.x;
    pxAudioListener->OrientTop.Y = ds3DListener.vOrientTop.y;
    pxAudioListener->OrientTop.Z = ds3DListener.vOrientTop.z;
    pxAudioListener->OrientTop.W = 0.0f;

    pxAudioListener->DistanceFactor = ds3DListener.flDistanceFactor;
    pxAudioListener->RolloffFactor = ds3DListener.flRolloffFactor;
    pxAudioListener->DopplerFactor = ds3DListener.flDopplerFactor;

    return pxResult;
}

PXResult PXAPI PXDirectSoundListenerDestroy(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener)
{
    return PXResultOK;
}

PXResult PXAPI PXDirectSoundListenerPositionSet(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener, PXVector4F32 PXREF position)
{
    if(!(pxEngineAudio && pxAudioListener && position))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound3DListener* directSound3DListener = pxAudioListener->Listener3D;

    // Set the listener's location in the 3D scene
#if PXLanguageCPP
    resultID = directSound3DListener->SetPosition
    (
        position->X,
        position->Y,
        position->Z,
        DS3D_DEFERRED
    );
#else
    resultID = directSound3DListener->lpVtbl->SetPosition
    (
        directSound3DListener,
        position->X,
        position->Y, 
        position->Z,
        DS3D_DEFERRED
    );
#endif
   
    pxResult = PXErrorFromHRESULT(resultID);

    // CRITICAL: Call CommitDeferredSettings once per game frame!
    // DirectSound calculates all 3D adjustments at once rather than doing it piecemeal.
    //g_Listener->lpVtbl->CommitDeferredSettings(g_Listener);

    return pxResult;
}

PXResult PXAPI PXDirectSoundListenerOrientationSet
(
    PXEngineAudio PXREF pxEngineAudio, 
    PXAudioListener PXREF pxAudioListener, 
    PXVector4F32 PXREF forward,
    PXVector4F32 PXREF up
)
{
    if(!(pxEngineAudio && pxAudioListener && forward && up))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound3DListener* directSound3DListener = pxAudioListener->Listener3D;

    // IDirectSound3DListener_SetOrientation
#if PXLanguageCPP
    resultID = directSound3DListener->SetOrientation
    (
        forward->X,
        forward->Y,
        forward->Z,
        up->X,
        up->Y,
        up->Z,
        DS3D_DEFERRED
    );
#else
    resultID = directSound3DListener->lpVtbl->SetOrientation
    (
        directSound3DListener,
        forward->X,
        forward->Y,
        forward->Z,
        up->X,
        up->Y,
        up->Z,
        DS3D_DEFERRED
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundListenerStagedUpdatesCommit(PXEngineAudio PXREF pxEngineAudio, PXAudioListener PXREF pxAudioListener)
{
    if(!(pxEngineAudio && pxAudioListener))
    {
        return PXResultRefusedParameterNull;
    }

    IDirectSound3DListener* directSound3DListener = pxAudioListener->Listener3D;
    HRESULT resultID = S_OK;
  
#if PXLanguageCPP
    resultID = directSound3DListener->CommitDeferredSettings();
#else
    resultID = directSound3DListener->lpVtbl->CommitDeferredSettings(directSound3DListener);
#endif

    PXResult pxResult = PXErrorFromHRESULT(resultID);
    
    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterCreate
(
    PXEngineAudio PXREF pxEngineAudio,
    PXAudioDevice PXREF pxAudioDevice,
    PXAudioEmitter PXREF pxAudioEmitter, 
    PXSound PXREF pxSound
)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxEngineAudio && pxAudioEmitter && pxSound))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows
    HRESULT resultID = 0;

    IDirectSound8* directSound8 = pxAudioDevice->Output.DS8;
    IDirectSound* directSound = pxAudioDevice->Output.DS;

    // DirectSoundBuffer3D can only use MONO sounds!
    PXBool expectMono = PXAudioEmitterModeLocal == pxAudioEmitter->Mode;
    PXBool needDownmix = expectMono && pxSound->NumerOfChannels > 1;

    if(needDownmix)
    {
        // Convert mo
        PXSoundStereoToMono(pxSound);
    }

    // Create buffer to load data into
    {
        WAVEFORMATEX waveFormat;
        PXClear(WAVEFORMATEX, &waveFormat);
        waveFormat.cbSize = 0; // Defined extra data, not size of self! sizeof(WAVEFORMATEX) is false!
        waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        waveFormat.nChannels = pxSound->NumerOfChannels;
        waveFormat.nSamplesPerSec = pxSound->SampleRate;
        waveFormat.nAvgBytesPerSec = PXSoundBytesPerSecound(pxSound);
        waveFormat.nBlockAlign = PXSoundBlockAllign(pxSound);
        waveFormat.wBitsPerSample = pxSound->BitsPerSample;
  

        DSBUFFERDESC pcDSBufferDesc;
        PXClear(DSBUFFERDESC, &pcDSBufferDesc);
        pcDSBufferDesc.dwSize = sizeof(DSBUFFERDESC);
        pcDSBufferDesc.dwBufferBytes = pxSound->Data.SizeAllowedToUse; // DSBSIZE_MIN, DSBSIZE_MAX
        pcDSBufferDesc.dwReserved = 0;
        pcDSBufferDesc.lpwfxFormat = &waveFormat;
        pcDSBufferDesc.dwFlags =
            // DSBCAPS_PRIMARYBUFFER is never used here!
            //DSBCAPS_STATIC | Tells directsound our data is fixed, "ReadOnly"
            DSBCAPS_LOCSOFTWARE | // Storage location?
            //DSBCAPS_CTRLFX | // Does not work with "DSBCAPS_STATIC"
            //DSBCAPS_CTRLFREQUENCY |
            //DSBCAPS_CTRLPOSITIONNOTIFY |
            DSBCAPS_GLOBALFOCUS; // Prevent the music from stopping if you TAB out;

        if(expectMono)
        {
            pcDSBufferDesc.dwFlags |=
                //DSBCAPS_GETCURRENTPOSITION2 |
                DSBCAPS_CTRLVOLUME | 
                DSBCAPS_MUTE3DATMAXDISTANCE |
                DSBCAPS_CTRL3D; // Only works with MONO sound
        }
        else
        {
            // if 3D is not used, we need to set this, or positioning can be wierd.
            pcDSBufferDesc.dwFlags |= DSBCAPS_CTRLPAN;
        }

        if(directSound8)
        {
#if PXLanguageCPP
            resultID = directSound8->CreateSoundBuffer
            (
                &pcDSBufferDesc,
                &pxAudioEmitter->BufferHandle,
                PXNull
            );
#else
            resultID = directSound8->lpVtbl->CreateSoundBuffer
            (
                directSound,
                &pcDSBufferDesc,
                &pxAudioEmitter->BufferHandle,
                PXNull
            );
#endif
        }
        else
        {
#if PXLanguageCPP
            resultID = directSound->CreateSoundBuffer
            (
                &pcDSBufferDesc,
                &pxAudioEmitter->BufferHandle,
                PXNull
            );
#else
            resultID = directSound->lpVtbl->CreateSoundBuffer
            (
                directSound,
                &pcDSBufferDesc,
                &pxAudioEmitter->BufferHandle,
                PXNull
            );
#endif
        }

        pxResult = PXErrorFromHRESULT(resultID);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }


    // If we have a 3D sound object, we need to create this interface
    if(expectMono)
    {
        IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

#if PXLanguageCPP
        resultID = directSoundBuffer->QueryInterface
        (
            IID_IDirectSound3DBuffer,
            (void**)&pxAudioEmitter->Buffer3DHandle
        );
#else
        resultID = directSoundBuffer->lpVtbl->QueryInterface
        (
            directSoundBuffer,
            &IID_IDirectSound3DBuffer,
            (void**)&pxAudioEmitter->Buffer3DHandle
        );
#endif


        pxResult = PXErrorFromHRESULT(resultID);
    }
#endif

    pxResult = PXDirectSoundEmitterDataSet(pxEngineAudio, pxAudioEmitter, pxSound);





    IDirectSound3DBuffer PXREF directSound3DBuffer = pxAudioEmitter->Buffer3DHandle;

#if PXLanguageCPP
    resultID = directSoundBuffer->QueryInterface
    (
        IID_IDirectSound3DBuffer,
        (void**)&pxAudioEmitter->Buffer3DHandle
    );
#else
    directSound3DBuffer->lpVtbl->SetPosition(directSound3DBuffer, 0, 0, 0, DS3D_IMMEDIATE);
    directSound3DBuffer->lpVtbl->SetMinDistance(directSound3DBuffer, 2.0f, DS3D_IMMEDIATE);
    directSound3DBuffer->lpVtbl->SetMaxDistance(directSound3DBuffer, 10.0f, DS3D_IMMEDIATE);
#endif



    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterDestroy(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter)
{
    return PXResultOK;
}

PXResult PXAPI PXDirectSoundEmitterDataSet
(
    PXEngineAudio PXREF pxEngineAudio, 
    PXAudioEmitter PXREF pxAudioEmitter, 
    PXSound PXREF pxSound
)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    PXSize dataOffset = 0;

    PXAudioCursorInsert pxAudioCursorInsert;

    DWORD dataBlockSizeA = 0;
    DWORD dataBlockSizeB = 0;
    DWORD flags = 0;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

    // Lock directSound buffer so we are allowed to write to it.
#if PXLanguageCPP
    resultID = directSoundBuffer->Lock
    (
        dataOffset,
        pxSound->Data.SizeAllowedToUse,
        &pxAudioCursorInsert.A,
        &dataBlockSizeA,
        &pxAudioCursorInsert.B,
        &dataBlockSizeB,
        flags
    );
#else
    resultID = directSoundBuffer->lpVtbl->Lock
    (
        directSoundBuffer,
        dataOffset,
        pxSound->Data.SizeAllowedToUse,
        &pxAudioCursorInsert.A,
        &dataBlockSizeA,
        &pxAudioCursorInsert.B,
        &dataBlockSizeB,
        flags
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    if(PXResultOK != pxResult)
        return pxResult;
    
    pxAudioCursorInsert.ALength = dataBlockSizeA;
    pxAudioCursorInsert.BLength = dataBlockSizeB;

    // Write sound data to actual soundBuffer.
    {
        PXByte* a = pxSound->Data.Data;

        PXCopyList(PXByte, pxAudioCursorInsert.ALength, pxSound->Data.Data, pxAudioCursorInsert.A);

        if(pxAudioCursorInsert.B)
        {
            PXByte* b = a + dataBlockSizeA;

            PXCopyList(PXByte, pxAudioCursorInsert.BLength, b, pxAudioCursorInsert.B);
        }
    }

    // Unlock buffer to release it back to DirectSound
#if PXLanguageCPP
    resultID = directSoundBuffer->Unlock
    (
        pxAudioCursorInsert.A,
        pxAudioCursorInsert.ALength,
        pxAudioCursorInsert.B,
        pxAudioCursorInsert.BLength
    );
#else
    resultID = directSoundBuffer->lpVtbl->Unlock
    (
        directSoundBuffer,
        pxAudioCursorInsert.A,
        pxAudioCursorInsert.ALength,
        pxAudioCursorInsert.B,
        pxAudioCursorInsert.BLength
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterPlay(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXBool loop)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

    if(!directSoundBuffer)
    {
        return PXResultRefusedHandleInvalid;
    }

#if PXLanguageCPP
    resultID = directSoundBuffer->Play(0, 0, DSBPLAY_LOOPING);
#else
    resultID = directSoundBuffer->lpVtbl->Play(directSoundBuffer, 0, 0, DSBPLAY_LOOPING);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterStop(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

    if(!directSoundBuffer)
    {
        return PXResultRefusedHandleInvalid;
    }

#if PXLanguageCPP
    resultID = directSoundBuffer->Stop();
#else
    resultID = directSoundBuffer->lpVtbl->Stop(directSoundBuffer);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return PXResultOK;
}

PXResult PXAPI PXDirectSoundEmitterRestart(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter)
{
    PXResult pxResult = PXResultInvalid;
    PXAudioCursor pxAudioCursor;
    pxAudioCursor.Read = 0;
    pxAudioCursor.Write = 0;

    pxResult = PXDirectSoundEmitterStop(pxEngineAudio, pxAudioEmitter); // Stop

    pxResult = PXDirectSoundEmitterCursorSet(pxEngineAudio, pxAudioEmitter, &pxAudioCursor); // Reset to 0

    pxResult = PXDirectSoundEmitterPlay(pxEngineAudio, pxAudioEmitter, PXTrue); // Play

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterPositionGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXVector4F32 PXREF position)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound3DBuffer* directSound3DBuffer = pxAudioEmitter->Buffer3DHandle;

    if(!directSound3DBuffer)
    {
        return PXResultRefusedHandleInvalid;
    }

    D3DVECTOR positionCurrent;

#if PXLanguageCPP
    resultID = directSound3DBuffer->GetPosition(&positionCurrent);
#else
    resultID = directSound3DBuffer->lpVtbl->GetPosition(directSound3DBuffer, &positionCurrent);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    position->X = positionCurrent.x;
    position->Y = positionCurrent.y;
    position->Z = positionCurrent.z;
    position->W = 0.0f;

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterPositionSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXVector4F32 PXREF position)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound3DBuffer* directSound3DBuffer = pxAudioEmitter->Buffer3DHandle;

#if PXLanguageCPP
    resultID = directSound3DBuffer->SetPosition
    (
        position->X,
        position->Y,
        position->Z,
        DS3D_IMMEDIATE
    );
#else
    resultID = directSound3DBuffer->lpVtbl->SetPosition
    (
        directSound3DBuffer,
        position->X,
        position->Y,
        position->Z,
        DS3D_IMMEDIATE
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterVelocityGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXVector4F32 PXREF velocity)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound3DBuffer* directSound3DBuffer = pxAudioEmitter->Buffer3DHandle;

    D3DVECTOR velocityCurrent;

#if PXLanguageCPP
    resultID = directSound3DBuffer->GetVelocity(&velocityCurrent);
#else
    resultID = directSound3DBuffer->lpVtbl->GetVelocity(directSound3DBuffer, &velocityCurrent);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    velocity->X = velocityCurrent.x;
    velocity->Y = velocityCurrent.y;
    velocity->Z = velocityCurrent.z;
    velocity->W = 0.0f;

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterVelocitySet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXVector4F32 PXREF velocity)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSound3DBuffer* directSound3DBuffer = pxAudioEmitter->Buffer3DHandle;

#if PXLanguageCPP
    resultID = directSound3DBuffer->SetVelocity
    (
        velocity->X,
        velocity->Y,
        velocity->Z,
        DS3D_DEFERRED
    );
#else
    resultID = directSound3DBuffer->lpVtbl->SetVelocity
    (
        directSound3DBuffer,
        velocity->X,
        velocity->Y,
        velocity->Z,
        DS3D_DEFERRED
    );
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return PXResultOK;
}

PXResult PXAPI PXDirectSoundEmitterVolumeGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXF32 PXREF volume)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;
    LONG volumeCurrent = 0;

#if PXLanguageCPP
    resultID = directSoundBuffer->GetVolume(&volumeCurrent);
#else
    resultID = directSoundBuffer->lpVtbl->GetVolume(directSoundBuffer, &volumeCurrent);
#endif

    *volume = volumeCurrent;

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterVolumeSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXF32 volume)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

#if PXLanguageCPP
    resultID = directSoundBuffer->SetVolume(volume); // DSBPLAY_LOOPING
#else
    resultID = directSoundBuffer->lpVtbl->SetVolume(directSoundBuffer, volume); // DSBPLAY_LOOPING
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterPitchGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXSize PXREF pitch)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

    DWORD frequencyDW = 0;

#if PXLanguageCPP
    resultID = directSoundBuffer->GetFrequency(&frequencyDW);
#else
    resultID = directSoundBuffer->lpVtbl->GetFrequency(directSoundBuffer, &frequencyDW);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    *pitch = frequencyDW;

    return PXResultOK;
}

PXResult PXAPI PXDirectSoundEmitterPitchSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXSize pitch)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

#if PXLanguageCPP
    resultID = directSoundBuffer->SetFrequency(pitch);
#else
    resultID = directSoundBuffer->lpVtbl->SetFrequency(directSoundBuffer, pitch);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return PXResultOK;
}

PXResult PXAPI PXDirectSoundEmitterCursorGet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, PXAudioCursor PXREF pxAudioCursor)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

    DWORD positionRead = 0;
    DWORD positionWrite = 0;

#if PXLanguageCPP
    resultID = directSoundBuffer->GetCurrentPosition(&positionRead, &positionWrite);
#else
    resultID = directSoundBuffer->lpVtbl->GetCurrentPosition(directSoundBuffer, &positionRead, &positionWrite);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    pxAudioCursor->Read = positionRead;
    pxAudioCursor->Write = positionWrite;

    return pxResult;
}

PXResult PXAPI PXDirectSoundEmitterCursorSet(PXEngineAudio PXREF pxEngineAudio, PXAudioEmitter PXREF pxAudioEmitter, const PXAudioCursor PXREF pxAudioCursor)
{
    PXResult pxResult = PXResultInvalid;
    HRESULT resultID = S_OK;

    IDirectSoundBuffer* directSoundBuffer = pxAudioEmitter->BufferHandle;

#if PXLanguageCPP
    resultID = directSoundBuffer->SetCurrentPosition(pxAudioCursor->Read);
#else
    resultID = directSoundBuffer->lpVtbl->SetCurrentPosition(directSoundBuffer, pxAudioCursor->Read);
#endif

    pxResult = PXErrorFromHRESULT(resultID);

    return pxResult;
}

PXResult PXAPI PXDirectSoundEffectEnable(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice)
{
    if(!(pxEngineAudio && pxAudioDevice))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
#elif OSWindows && 0

    const PXBool wasRunning = pxAudioDevice->IsRunning; // Store current state to keep davice playing after setting effect
    PXDirectSoundBuffer* soundBuffer = pxAudioDevice->;

    typedef struct PXListEntryEE_
    {
        GUID* ElementFilterID;
        GUID* ElementID;
        void** AddressReference;
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
        const PXAudioEffectEntry PXREF pxListEntry = &pxAudioEffectEntryList[i];

        if(!pxListEntry->Enabled)
        {
            continue;
        }

        DSEFFECTDESC PXREF effectDESC = &dsEffectList[amountCounter++];

        effectDESC->dwSize = sizeof(DSEFFECTDESC);
        effectDESC->dwFlags = 0;
        effectDESC->guidDSFXClass = *pxListEntry->ElementFilterID;
    }

    if(wasRunning)
    {
      //  PXDirectSoundDeviceStop(pxAudioDirectSound, pxAudioDevice); // Stop the device, we are not allowed to update effects while playing
    }

    soundBuffer->lpVtbl->SetFX(soundBuffer, 0, 0, dwResults);

    const HRESULT effectSetResultID = soundBuffer->lpVtbl->SetFX(soundBuffer, amountCounter, &dsEffectList, dwResults);
    const PXResult effectSetResult = PXErrorFromHRESULT(effectSetResultID);

    for(PXSize i = 0; i < amount; ++i)
    {
        const PXAudioEffectEntry PXREF pxListEntry = &pxAudioEffectEntryList[i];

        if(!pxListEntry->Enabled)
        {
            *pxListEntry->AddressReference = PXNull;
            continue;
        }

        soundBuffer->lpVtbl->GetObjectInPath(soundBuffer, pxListEntry->ElementFilterID, 0, pxListEntry->ElementID, pxListEntry->AddressReference);
    }

    if(wasRunning)
    {
       // PXDirectSoundDeviceStart(pxAudioDirectSound, pxAudioDevice);
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXDirectSoundEffectUpdate(PXEngineAudio PXREF pxEngineAudio, PXAudioDevice PXREF pxAudioDevice, void PXREF pxAudioEffect)
{
#if OSUnix
#elif OSWindows && 0

    // check input

    // Check if enabled
    {
        PXI32U flag = 0;

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
                return PXResultRefusedParameterInvalid;
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
                PXDirectSoundEffectEnable(pxEngineAudio, pxAudioDevice);

            }
        }

        // quit if we dont process anything
        if(!pxAudioEffect->Enable)
        {
            return PXResultOK;
        }
    }


    // Handle spesific action
    {
        HRESULT result = 0;

        switch(pxAudioEffect->Type)
        {
            case PXAudioEffectTypeChorus:
            {
                IDirectSoundFXChorus8 PXREF directSoundFXChorus = (IDirectSoundFXChorus8*)pxAudioDevice->FXChorus;
                PXFilterChorus PXREF pxAudioEffectChorus = &pxAudioEffect->Filter;
                DSFXChorus dxfxChorus;



                if(pxAudioEffect->Fetch)
                {
#if PXLanguageCPP
                    result = directSoundFXChorus->GetAllParameters(&dxfxChorus);
#else
                    result = directSoundFXChorus->lpVtbl->GetAllParameters(directSoundFXChorus, &dxfxChorus);
#endif

                    pxAudioEffectChorus->Wet = dxfxChorus.fWetDryMix;
                    pxAudioEffectChorus->Dry = dxfxChorus.fWetDryMix;
                    pxAudioEffectChorus->Depth = dxfxChorus.fDepth;
                    pxAudioEffectChorus->Feedback = dxfxChorus.fFeedback;
                    pxAudioEffectChorus->Frequency = dxfxChorus.fFrequency;
                    pxAudioEffectChorus->Waveform = dxfxChorus.lWaveform;
                    pxAudioEffectChorus->Delay = dxfxChorus.fDelay;
                    pxAudioEffectChorus->Phase = dxfxChorus.lPhase;
                }
                else
                {
                    dxfxChorus.fWetDryMix = PXMathLiniarF(DSFXCHORUS_WETDRYMIX_MIN, DSFXCHORUS_WETDRYMIX_MAX, 0, 1, pxAudioEffectChorus->Wet);
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
                IDirectSoundFXCompressor8 PXREF directSoundFXCompressor = (IDirectSoundFXCompressor8*)pxAudioDevice->FXCompressor;
                PXFilterCompressor PXREF pxAudioEffectCompressor = &pxAudioEffect->Filter;
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
                IDirectSoundFXDistortion8 PXREF directSoundFXDistortion = (IDirectSoundFXDistortion8*)pxAudioDevice->FXDistortion;
                PXFilterDistortion PXREF pxAudioEffectDistortion = &pxAudioEffect->Filter;
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
                IDirectSoundFXEcho8 PXREF directSoundFXEcho = (IDirectSoundFXEcho8*)pxAudioDevice->FXEcho;
                PXFilterEcho PXREF pxAudioEffectEcho = &pxAudioEffect->Filter;
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
                IDirectSoundFXFlanger8 PXREF directSoundFXFlanger = (IDirectSoundFXFlanger8*)pxAudioDevice->FXFlanger;
                PXFilterFlanger PXREF pxAudioEffectFlanger = &pxAudioEffect->Filter;
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
                IDirectSoundFXGargle8 PXREF directSoundFXGargle = (IDirectSoundFXGargle8*)pxAudioDevice->FXGargle;
                PXFilterGargle PXREF pxAudioEffectGargle = &pxAudioEffect->Filter;
                DSFXGargle dsfxGargle;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXGargle->lpVtbl->GetAllParameters(directSoundFXGargle, &dsfxGargle);

                    pxAudioEffectGargle->RateHz = dsfxGargle.dwRateHz;
                    pxAudioEffectGargle->WaveShape = dsfxGargle.dwWaveShape;
                }
                else
                {
                    dsfxGargle.dwRateHz = pxAudioEffectGargle->RateHz;
                    dsfxGargle.dwWaveShape = pxAudioEffectGargle->WaveShape;

                    result = directSoundFXGargle->lpVtbl->SetAllParameters(directSoundFXGargle, &dsfxGargle);
                }

                break;
            }
            case PXAudioEffectTypeInteractive3DLevel2Reverb:
            {
                IDirectSoundFXI3DL2Reverb8 PXREF directSoundFXI3DL2Reverb = (IDirectSoundFXI3DL2Reverb8*)pxAudioDevice->FXI3DL2Reverb;
                PXFilterReverbI3DL2 PXREF pxAudioEffectI3DL2Reverb = &pxAudioEffect->Filter;
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
                IDirectSoundFXParamEq8 PXREF directSoundFXParamEq = (IDirectSoundFXParamEq8*)pxAudioDevice->FXParamEq;
                PXFilterEqualizer PXREF pxAudioEffectParamEq = &pxAudioEffect->Filter;
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
                IDirectSoundFXWavesReverb8 PXREF directSoundFXWave = (IDirectSoundFXWavesReverb8*)pxAudioDevice->FXWavesReverb;
                PXFilterReverbWaves PXREF pxAudioEffectWavesReverb = &pxAudioEffect->Filter;
                DSFXWavesReverb wavesReverb;

                if(pxAudioEffect->Fetch)
                {
                    result = directSoundFXWave->lpVtbl->GetAllParameters(directSoundFXWave, &wavesReverb);

                    pxAudioEffectWavesReverb->InGain = wavesReverb.fInGain;
                    pxAudioEffectWavesReverb->ReverbMix = wavesReverb.fReverbMix;
                    pxAudioEffectWavesReverb->ReverbTime = wavesReverb.fReverbTime;
                    pxAudioEffectWavesReverb->HighFreqRTRatio = wavesReverb.fHighFreqRTRatio;
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
                return PXResultRefusedParameterInvalid;
        }
    }
#endif

    return PXResultOK;
}

#pragma once

#ifndef PXDirectSoundIncluded
#define PXDirectSoundIncluded

#include <PX/Engine/ECS/Resource/AudioDevice/PXAudioDevice.h>
#include <PX/Engine/ECS/System/PXAudioSystem.h>

typedef struct PXAudioDeviceDetectObjectEventData_
{
    PXI32U IndexCurrent;
    PXI32U IndexTarget;
    PXAudioDevice* AudioDevice;
}
PXAudioDeviceDetectObjectEventData;

#if OSUnix
#elif OSWindows


PXPrivate BOOL CALLBACK PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXI32U PXREF amount);
PXPrivate BOOL CALLBACK PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDeviceDetectObjectEventData PXREF pxAudioDeviceDetectObjectEventData);


typedef BOOL(CALLBACK* PXLPDSENUMCALLBACKA)(struct _GUID*, char*, char*, void*);
typedef BOOL(CALLBACK* PXLPDSENUMCALLBACKW)(struct _GUID*, wchar_t*, wchar_t*, void*);
typedef HRESULT(WINAPI* PXDirectSoundCreate)(struct _GUID* pcGuidDevice, void* ppDS, void* pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundEnumerateA)(PXLPDSENUMCALLBACKA pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDirectSoundEnumerateW)(PXLPDSENUMCALLBACKW pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDirectSoundCaptureCreate)(struct _GUID* pcGuidDevice, void* ppDSC, void* pUnkOuter);
typedef HRESULT(WINAPI* PXDirectSoundCaptureEnumerateA)(PXLPDSENUMCALLBACKA pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDirectSoundCaptureEnumerateW)(PXLPDSENUMCALLBACKW pDSEnumCallback, void* pContext);
typedef HRESULT(WINAPI* PXDllGetClassObject)(REFCLSID rclsid, REFIID riid, void* ppv);
typedef HRESULT(WINAPI* PXDllCanUnloadNow)(void);

#endif

//--------------------------------------------------------
PXPublic PXResult PXAPI PXDirectSoundInitialize(PXAudioDirectSound* PXREF pxAudioDirectSound, PXAudioDeviceCreateInfo PXREF pxAudioInitializeInfo);

PXPublic PXResult PXAPI PXDirectSoundDeviceAmount(PXAudioDirectSound PXREF pxAudioDirectSound, PXAudioDeviceAmountInfo PXREF pxAudioDeviceAmountInfo);
PXPublic PXResult PXAPI PXDirectSoundDeviceFetch(PXAudioDirectSound PXREF pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundDeviceFetchAll(PXAudioDirectSound PXREF pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice PXREF pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXDirectSoundDeviceOpen(PXAudioDevice PXREF pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXDirectSoundDeviceClose(PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundDeviceBufferCreate
(
    PXAudioDevice PXREF pxAudioDevice,
    PXSound PXREF pxSound
);






// Retrieves or writes a property value on a sound device
PXPublic PXResult PXAPI PXDirectSoundDeviceProperty(PXAudioDevice PXREF pxAudioDevice, PXSoundDeviceProperty PXREF pxSoundDeviceProperty);


PXPublic PXResult PXAPI PXDirectSoundEffectEnable(PXAudioDirectSound PXREF pxAudioDirectSound, PXAudioDevice PXREF pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundEffectUpdate(PXAudioDirectSound PXREF pxAudioDirectSound, PXAudioDevice PXREF pxAudioDevice, PXAudioEffect PXREF pxAudioEffect);


#endif

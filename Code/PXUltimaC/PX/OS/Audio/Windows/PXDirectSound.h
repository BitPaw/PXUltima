#ifndef PXDirectSoundIncluded
#define PXDirectSoundIncluded

#include <PX/OS/Audio/PXAudio.h>

typedef struct PXAudioDeviceDetectObjectEventData_
{
    PXI32U IndexCurrent;
    PXI32U IndexTarget;
    PXAudioDevice* AudioDevice;
}
PXAudioDeviceDetectObjectEventData;

#if OSUnix
#elif OSWindows


PXPrivate BOOL CALLBACK PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXI32U* const amount);
PXPrivate BOOL CALLBACK PXAudioDeviceDetectObjectCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXAudioDeviceDetectObjectEventData* const pxAudioDeviceDetectObjectEventData);


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
PXPublic PXResult PXAPI PXDirectSoundInitialize(PXAudioDirectSound** const pxAudioDirectSound, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXResult PXAPI PXDirectSoundDeviceAmount(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo);
PXPublic PXResult PXAPI PXDirectSoundDeviceFetch(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundDeviceFetchAll(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXResult PXAPI PXDirectSoundDeviceOpen(PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXI32U deviceID);
PXPublic PXResult PXAPI PXDirectSoundDeviceClose(PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundDeviceBufferCreate
(
    PXAudioDevice* const pxAudioDevice,
    PXSound* const pxSound
);






// Retrieves or writes a property value on a sound device
PXPublic PXResult PXAPI PXDirectSoundDeviceProperty(PXAudioDevice* const pxAudioDevice, PXSoundDeviceProperty* const pxSoundDeviceProperty);


PXPublic PXResult PXAPI PXDirectSoundEffectEnable(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);
PXPublic PXResult PXAPI PXDirectSoundEffectUpdate(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXAudioEffect* const pxAudioEffect);


#endif

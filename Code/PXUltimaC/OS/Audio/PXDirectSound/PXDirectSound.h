#ifndef PXDirectSoundInclude
#define PXDirectSoundInclude

#include <OS/Audio/PXAudio.h>

typedef struct PXAudioDeviceDetectObjectEventData_
{
	PXInt32U IndexCurrent;
	PXInt32U IndexTarget;
	PXAudioDevice* AudioDevice;
}
PXAudioDeviceDetectObjectEventData;

#if OSUnix
#elif OSWindows


PXPrivate BOOL CALLBACK PXAudioDeviceDetectAmountCallBack(LPGUID guid, LPCSTR cstrDescription, LPCSTR cstrModule, PXInt32U* const amount);
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
PXPublic PXActionResult PXAPI PXDirectSoundInitialize(PXAudioDirectSound* const pxAudioDirectSound, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceAmount(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceFetch(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceFetchAll(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceOpen(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceClose(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceLoad
(
	PXAudioDirectSound* const pxAudioDirectSound,
	PXAudioDevice* const pxAudioDevice,
	PXSound* const pxSound
);

//--------------------------------------------------------
// Device - Input
//--------------------------------------------------------


//--------------------------------------------------------
// Device - Output
//--------------------------------------------------------


PXPublic PXActionResult PXAPI PXDirectSoundDevicePlayCursorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXSize offset);
PXPublic PXActionResult PXAPI PXDirectSoundDevicePlayCursorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXSize* const offset);


PXPublic PXActionResult PXAPI PXDirectSoundDevicePlaySpeedSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U frequency);
PXPublic PXActionResult PXAPI PXDirectSoundDevicePlaySpeedGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const frequency);


PXPublic PXActionResult PXAPI PXDirectSoundDevicePositionSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);
PXPublic PXActionResult PXAPI PXDirectSoundDevicePositionGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeAnglesSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U dwInsideConeAngle, const PXInt32U dwOutsideConeAngle);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeAnglesGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwInsideConeAngle, PXInt32U* const pdwOutsideConeAngle);


//PXPublic PXActionResult PXAPI PXDirectSoundDeviceParametersAllGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, __out LPDS3DBUFFER pDs3dBuffer);
//PXPublic PXActionResult PXAPI PXDirectSoundDeviceParametersAllSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, __in LPCDS3DBUFFER pcDs3dBuffer, DWORD dwApply);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOrientationGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float* const x, float* const  y, float* const  z);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOrientationSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const plConeOutsideVolume);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceConeOutsideVolumeSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U lConeOutsideVolume);


PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float* const pflMaxDistance);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMaxSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const float flMaxDistance);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMinGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float* const pflMinDistance);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceMinSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const float flMinDistance);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceModeGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pdwMode);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceModeSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U dwMode);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceVelocityGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float* const x, float* const y, float* const z);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceVelocitySet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const float x, const float y, const float z);


PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDistanceFactor);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceDistanceFactorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U flDistanceFactor);

//  DS3D_MINDOPPLERFACTOR to DS3D_MAXDOPPLERFACTOR
PXPublic PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflDopplerFactor);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceDopplerFactorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U flDopplerFactor);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceOrientationGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pvOrientFront, PXInt32U* const pvOrientTop);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceOrientationSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U xFront, const PXInt32U yFront, const PXInt32U zFront, const PXInt32U xTop, const PXInt32U yTop, const PXInt32U zTop);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXInt32U* const pflRolloffFactor);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceRolloffFactorSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const PXInt32U flRolloffFactor);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceDeferredSettingsCommit(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);


//--------------------------------------------------------
// Device - Pitch
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectSoundDevicePitchIncrease(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float amount);
PXPublic PXActionResult PXAPI PXDirectSoundDevicePitchSet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const unsigned int pitch);
PXPublic PXActionResult PXAPI PXDirectSoundDevicePitchReduce(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, float amount);

//--------------------------------------------------------
// Device - Volume
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectSoundDeviceVolumeGet(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, unsigned short* const volume);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceVolumeSetEqual(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const unsigned int volume);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceVolumeSetIndividual(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, const unsigned short volumeLeft, const unsigned short volumeRight);

//--------------------------------------------------------
// Device - Playback
//--------------------------------------------------------
PXPublic PXActionResult PXAPI PXDirectSoundDeviceStart(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceRestart(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceStop(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDevicePause(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);


PXPublic PXActionResult PXAPI PXDirectSoundEffectEnable(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);


PXPublic PXActionResult PXAPI PXDirectSoundEffectUpdate(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXAudioEffect* const pxAudioEffect);


#endif
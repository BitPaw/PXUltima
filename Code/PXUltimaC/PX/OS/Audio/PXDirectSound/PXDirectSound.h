#ifndef PXDirectSoundInclude
#define PXDirectSoundInclude

#include <PX/OS/Audio/PXAudio.h>

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
PXPublic PXActionResult PXAPI PXDirectSoundInitialize(PXAudioDirectSound** const pxAudioDirectSound, PXAudioInitializeInfo* const pxAudioInitializeInfo);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceAmount(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDeviceAmountInfo* const pxAudioDeviceAmountInfo);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceFetch(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceFetchAll(PXAudioDirectSound* const pxAudioDirectSound, const PXAudioDeviceType pxAudioDeviceType, PXAudioDevice* const pxAudioDevice, const PXSize amount);

PXPublic PXActionResult PXAPI PXDirectSoundDeviceOpen(PXAudioDevice* const pxAudioDevice, const PXAudioDeviceType pxAudioDeviceType, const PXInt32U deviceID);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceClose(PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundDeviceLoad
(
    PXAudioDevice* const pxAudioDevice,
    PXSound* const pxSound
);

//--------------------------------------------------------
// Device - Input
//--------------------------------------------------------


//--------------------------------------------------------
// Device - Output
//--------------------------------------------------------

#define PXSoundDevicePropertyPositionCursor     1
#define PXSoundDevicePropertySpeed              2
#define PXSoundDevicePropertyPositionSpace      3
#define PXSoundDevicePropertyAngleCone          4
#define PXSoundDevicePropertyConeOrientation    5
#define PXSoundDevicePropertyDistanceMaximum    6
#define PXSoundDevicePropertyDistanceMinimum    7
#define PXSoundDevicePropertyMode               8
#define PXSoundDevicePropertyVelocity           9
#define PXSoundDevicePropertyDistanceFactor     10
#define PXSoundDevicePropertyDopplerFactor      11
#define PXSoundDevicePropertyOrientation        12
#define PXSoundDevicePropertyRolloffFactor      13
#define PXSoundDevicePropertyPitch              14
#define PXSoundDevicePropertyVolume             15

// State Tranitions



//  DS3D_MINDOPPLERFACTOR to DS3D_MAXDOPPLERFACTOR
#define PXSoundDevicePropertyStatePlay      16
#define PXSoundDevicePropertyStatePause     17
#define PXSoundDevicePropertyStateRestart   18



// Shall we read or write this property?
#define PXSoundDevicePropertyRWMask     0b11
#define PXSoundDevicePropertyRead       0b01
#define PXSoundDevicePropertyWrite      0b10

//#define PXSoundDevicePropertyRead
//#define PXSoundDevicePropertyWrite

// Transphere object
typedef struct PXSoundDeviceProperty_
{
    PXInt16U Type;
    PXInt16U Flags;

    union
    {
        PXInt32U Value;
        PXVector2F32 Position2D;
        PXVector3F32 Position3D;
    };
}
PXSoundDeviceProperty;


// Retrieves or writes a property value on a sound device
PXPublic PXActionResult PXAPI PXDirectSoundDeviceProperty(PXAudioDevice* const pxAudioDevice, PXSoundDeviceProperty* const pxSoundDeviceProperty);


PXPublic PXActionResult PXAPI PXDirectSoundEffectEnable(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice);
PXPublic PXActionResult PXAPI PXDirectSoundEffectUpdate(PXAudioDirectSound* const pxAudioDirectSound, PXAudioDevice* const pxAudioDevice, PXAudioEffect* const pxAudioEffect);


#endif

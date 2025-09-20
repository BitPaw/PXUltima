#ifndef PXInputIncluded
#define PXInputIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXDeviceInputMouse_
{
    DWORD dwId;
    DWORD dwNumberOfButtons;
    DWORD dwSampleRate;
    BOOL  fHasHorizontalWheel;
}
PXDeviceInputMouse;

typedef struct PXDeviceInputKeyBoard_
{
    DWORD dwType;
    DWORD dwSubType;
    DWORD dwKeyboardMode;
    DWORD dwNumberOfFunctionKeys;
    DWORD dwNumberOfIndicators;
    DWORD dwNumberOfKeysTotal;
}
PXDeviceInputKeyBoard;

typedef struct PXDeviceInputGeneric_
{
    DWORD dwVendorId;
    DWORD dwProductId;
    DWORD dwVersionNumber;
    USHORT usUsagePage;
    USHORT usUsage;
}
PXDeviceInputGeneric;


typedef enum PXDeviceInputType_
{
    PXDeviceInputTypeMouse = 1,
    PXDeviceInputTypeKeyBoard = 2,
    PXDeviceInputTypeGeneric = 3
}
PXDeviceInputType;

//#define PXDeviceInputTypeMouse      1
//#define PXDeviceInputTypeKeyBoard   2
//#define PXDeviceInputTypeGeneric    3

typedef struct PXDeviceInput_
{
    PXDeviceInputType Type;

    char Name[128];

    union
    {
        PXDeviceInputKeyBoard KeyBoard;
        PXDeviceInputMouse Mouse;
        PXDeviceInputGeneric Generic;
    };
}
PXDeviceInput;




typedef enum  PXDeviceInputUpdateType
{
    PXDeviceInputUpdateTypeInvalid,
    PXDeviceInputUpdateTypeArrival,
    PXDeviceInputUpdateTypeRemoval,
}
PXDeviceInputUpdateType;

typedef struct PXDeviceInputUpdateInfo_
{
    PXDeviceInputUpdateType Type;

#if OSWindows
    HANDLE DeviceHandle;
#endif
}
PXDeviceInputUpdateInfo;


PXPrivate void PXAPI PXDeviceFill(PXDeviceInput* const pxDeviceInput, const HANDLE deviceHandle);


PXPublic PXResult PXAPI PXDeviceInputUpdate(PXDeviceInputUpdateInfo* const pxDeviceInputUpdateInfo);

PXPublic PXResult PXAPI PXDeviceInputFetch(PXDeviceInput** const pxDeviceInput, PXSize* const amount);

#endif
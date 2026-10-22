#pragma once

#ifndef PXInputIncluded
#define PXInputIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXDeviceInputMouse_
{
    PXI32U ID;
    PXI32U NumberOfButtons;
    PXI32U SampleRate;
    PXBool HasHorizontalWheel;
}
PXDeviceInputMouse;

typedef struct PXDeviceInputKeyBoard_
{
    PXI32U dwType;
    PXI32U dwSubType;
    PXI32U dwKeyboardMode;
    PXI32U dwNumberOfFunctionKeys;
    PXI32U dwNumberOfIndicators;
    PXI32U dwNumberOfKeysTotal;
}
PXDeviceInputKeyBoard;

typedef struct PXDeviceInputGeneric_
{
    PXI32U dwVendorId;
    PXI32U dwProductId;
    PXI32U dwVersionNumber;
    PXI16U usUsagePage;
    PXI16U usUsage;
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


#if OSWindows
PXPrivate void PXAPI PXDeviceFill(PXDeviceInput PXREF pxDeviceInput, const HANDLE deviceHandle);
#endif

PXPublic PXResult PXAPI PXDeviceInputUpdate(PXDeviceInputUpdateInfo PXREF pxDeviceInputUpdateInfo);

PXPublic PXResult PXAPI PXDeviceInputFetch(PXDeviceInput* PXREF pxDeviceInput, PXSize PXREF amount);

#endif
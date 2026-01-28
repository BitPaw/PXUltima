#ifndef PXInputDeviceIncluded
#define PXInputDeviceIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXVector.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Resource/Controller/PXController.h>

typedef enum PXInputDeviceType_
{
    PXInputDeviceTypeInvalid,
    PXInputDeviceTypeMouse,
    PXInputDeviceTypeKeyBoard,
    PXInputDeviceTypeJoystick,
    PXInputDeviceTypeGamepad,
    PXInputDeviceTypeMultiAxisController,
    PXInputDeviceTypeSteeringWheel,
    PXInputDeviceTypePedals,
    PXInputDeviceTypeFlightStick,
    PXInputDeviceTypeRudderPedals,

    PXInputDeviceTypeDRIVING,
    PXInputDeviceTypeFLIGHT,
    PXInputDeviceType1STPERSON,
    PXInputDeviceTypeDEVICECTRL,
    PXInputDeviceTypeSCREENPOINTER,
    PXInputDeviceTypeREMOTE,
    PXInputDeviceTypeSUPPLEMENTAL,

    PXInputDeviceTypeUnkown
}
PXInputDeviceType;

PXPublic const char* PXInputDeviceTypeToString(const PXInputDeviceType pxInputDeviceType);


typedef struct PXInputMouse_
{
    PXI32U ID;
    PXI32U NumberOfButtons;
    PXI32U SampleRate;
    PXBool HasHorizontalWheel;
}
PXInputMouse;

typedef struct PXInputKeyBoard_
{
    PXI32U Type;
    PXI32U SubType;
    PXI32U KeyboardMode;
    PXI32U NumberOfFunctionKeys;
    PXI32U NumberOfIndicators;
    PXI32U NumberOfKeysTotal;
}
PXInputKeyBoard;

typedef struct PXInputHID_
{
    PXI32U VendorID;
    PXI32U ProductID;
    PXI32U VersionNumber;
    PXI16U UsagePage;
    PXI16U Usage;
}
PXInputHID;

typedef struct PXInputDevice_
{
    PXECSInfo Info;

#if OSWindows
    HANDLE DeviceHandle;

    struct IDirectInputDevice8W* DeviceInterface;

    GUID DeviceGUID;
#endif

    PXBool IsAquired;

    PXInputDeviceType Type;

    PXI32U DirectInput;
    PXI32U WindowsMM;
    PXI32U XInput;
    PXI32U RAWInput;

    union 
    {
        PXInputKeyBoard KeyBoard;
        PXInputMouse Mouse;
        PXInputHID HID;
    };

    wchar_t Name[MAX_PATH];

    union
    {
        PXController ControllerData;
    };
}
PXInputDevice;




#endif
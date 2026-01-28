#include "PXInputSystem.h"
#include "PXInputDevice.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>


#if OSUnix

#elif OSWindows

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800 // Documenation demands this
#endif


#include <windows.h>
#include <setupapi.h>
#pragma comment(lib, "setupapi.lib")
#include <hidsdi.h>
#pragma comment(lib, "hid.lib")
#include <dinput.h> // Doc states Dinput8.h

#pragma comment(lib, "Dinput8.lib") // This is D8 not D7!

#include <Xinput.h> // XBOX controllers

#endif

BOOL CALLBACK PXDirectInputDeviceListCallback(const DIDEVICEINSTANCEW* inst, PXInputSystem PXREF pxInputSystem)
{
    inst->guidProduct;
    inst->dwDevType;
    inst->tszInstanceName[MAX_PATH];
    inst->tszProductName[MAX_PATH];
    inst->guidFFDriver;
    inst->wUsagePage;
    inst->wUsage;

    WORD vid = LOWORD(inst->guidProduct.Data1);
    WORD pid = HIWORD(inst->guidProduct.Data1);
    DWORD devClass = GET_DIDEVICE_TYPE(inst->dwDevType);
    DWORD devSubtype = GET_DIDEVICE_SUBTYPE(inst->dwDevType);

    for(PXSize i = 0; i < pxInputSystem->DeviceAmount; i++)
    {
        PXInputDevice* pxInputDevice = &pxInputSystem->DeviceList[i];

        PXBool isTarget = 
            pxInputDevice->HID.VendorID == vid &&
            pxInputDevice->HID.ProductID == pid &&
            pxInputDevice->HID.UsagePage == inst->wUsagePage &&
            pxInputDevice->HID.Usage == inst->wUsage;

     
        pxInputDevice->DeviceGUID = inst->guidInstance;

        if(!isTarget)
        {
            //DebugBreak();
            continue;
        }


        /*
        switch(devTypeID)
        {
        case DI8DEVTYPE_1STPERSON:
        {
            
        }

        default:
            break;
    }
        */

#if 1
        // Decode class
        switch(devClass)
        {
            case DI8DEVCLASS_POINTER:   
                pxInputDevice->Type = PXInputDeviceTypeMouse;
                break;
            case DI8DEVCLASS_KEYBOARD: 
                pxInputDevice->Type = PXInputDeviceTypeKeyBoard;
                break;
            case DI8DEVCLASS_GAMECTRL:   
                pxInputDevice->Type = PXInputDeviceTypeJoystick;
                break;
        }
#endif
        // Decode subtype
        switch(devSubtype)
        {
            case DI8DEVTYPE_DEVICE:
                pxInputDevice->Type = PXInputDeviceTypeUnkown; 
                break;
            case DI8DEVTYPE_MOUSE: 
                pxInputDevice->Type = PXInputDeviceTypeMouse;
                break;
            case DI8DEVTYPE_KEYBOARD:
                pxInputDevice->Type = PXInputDeviceTypeKeyBoard;
                break;
            case DI8DEVTYPE_JOYSTICK: 
                pxInputDevice->Type = PXInputDeviceTypeJoystick;
                break;
            case DI8DEVTYPE_GAMEPAD: 
                pxInputDevice->Type = PXInputDeviceTypeGamepad;
                break;
            case DI8DEVTYPE_DRIVING: 
                pxInputDevice->Type = PXInputDeviceTypeDRIVING;
                break;
            case DI8DEVTYPE_FLIGHT:
                pxInputDevice->Type = PXInputDeviceTypeFLIGHT;
                break;
            case DI8DEVTYPE_1STPERSON: 
                pxInputDevice->Type = PXInputDeviceType1STPERSON;
                break;
            case DI8DEVTYPE_DEVICECTRL: 
                pxInputDevice->Type = PXInputDeviceTypeDEVICECTRL;
                break;
            case DI8DEVTYPE_SCREENPOINTER:
                pxInputDevice->Type = PXInputDeviceTypeDEVICECTRL;
                break;
            case DI8DEVTYPE_REMOTE: 
                pxInputDevice->Type = PXInputDeviceTypeREMOTE;
                break;
            case DI8DEVTYPE_SUPPLEMENTAL:
                pxInputDevice->Type = PXInputDeviceTypeSUPPLEMENTAL;
                break;
        }

       // PXTextPrintA(pxInputDevice->Name, MAX_PATH, "%ls", inst->tszProductName);
        //PXTextCopyW(inst->tszProductName, MAX_PATH, pxInputDevice->Name, MAX_PATH);

        pxInputDevice->DirectInput = PXTrue;
    }



    PXConsoleWrite(0,0);


    /*
    InputDevice dev;
    dev.api = DeviceAPI::DirectInput;
    dev.name = inst->tszProductName;
    dev.diGuid = inst->guidInstance;

    // Extract VID/PID from GUID
    dev.vid = LOWORD(inst->guidProduct.Data1);
    dev.pid = HIWORD(inst->guidProduct.Data1);*/





    return DIENUM_CONTINUE;
}


PXInputPlayer* PXAPI PXInputSystemPlayerGet(const PXSize playerNR)
{
    return PXNull;
}


PXResult PXAPI PXInputSystemDevicesRefreshRawInput(PXInputSystem PXREF pxInputSystem)
{
    UINT numDevices = 0;

    union
    {
        RAWINPUTDEVICELIST Stack[32];
        RAWINPUTDEVICELIST* Heap;
    }
    rawInputDeviceList;

    const PXSize size = sizeof(rawInputDeviceList);

    RAWINPUTDEVICELIST* list = rawInputDeviceList.Stack;

    GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST));

    if(numDevices > 32)
    {
        rawInputDeviceList.Heap = PXNull;
        list = rawInputDeviceList.Heap;
    }

    pxInputSystem->DeviceAmount = numDevices;
    pxInputSystem->DeviceList = PXMemoryHeapCallocT(PXInputDevice, numDevices);

    GetRawInputDeviceList(list, &numDevices, sizeof(RAWINPUTDEVICELIST));

    for(PXSize i = 0; i < numDevices; ++i)
    {
        RAWINPUTDEVICELIST* device = &list[i];
        PXInputDevice* pxInputDevice = &pxInputSystem->DeviceList[i];

        pxInputDevice->DeviceHandle = device->hDevice;
        pxInputDevice->RAWInput = PXTrue;

        UINT nameSize = 0;
        GetRawInputDeviceInfoW(device->hDevice, RIDI_DEVICENAME, NULL, &nameSize);

        GetRawInputDeviceInfoW(device->hDevice, RIDI_DEVICENAME, pxInputDevice->Name, &nameSize);



        RID_DEVICE_INFO info;
        info.cbSize = sizeof(info);
        UINT size = sizeof(info);

        GetRawInputDeviceInfoW(device->hDevice, RIDI_DEVICEINFO, &info, &size);

        switch(device->dwType)
        {
            case RIM_TYPEHID:
            {
                pxInputDevice->Type = PXInputDeviceTypeUnkown;
                break;
            }
            case RIM_TYPEKEYBOARD:
            {
                pxInputDevice->Type = PXInputDeviceTypeKeyBoard;
                break;
            }
            case RIM_TYPEMOUSE:
            {
                pxInputDevice->Type = PXInputDeviceTypeMouse;
                break;
            }
            default:
            {
                pxInputDevice->Type = PXInputDeviceTypeInvalid;
                break;
            }
        }

        switch(device->dwType)
        {
            case RIM_TYPEHID:
            {
                RID_DEVICE_INFO_HID* hid = &info.hid;
                PXInputHID* pxInputHID = &pxInputDevice->HID;

                pxInputHID->VendorID = hid->dwVendorId;
                pxInputHID->ProductID = hid->dwProductId;
                pxInputHID->VersionNumber = hid->dwVersionNumber;
                pxInputHID->UsagePage = hid->usUsagePage;
                pxInputHID->Usage = hid->usUsage;

                switch(hid->usUsagePage)
                {
                    case 0x01:
                    {
                        switch(hid->usUsage)
                        {
                            case 0x02:
                                pxInputDevice->Type = PXInputDeviceTypeSteeringWheel;
                                break;

                            case 0x04:
                                pxInputDevice->Type = PXInputDeviceTypeJoystick;
                                //PXInputDeviceTypeFlightStick
                                break;

                            case 0x05:
                                pxInputDevice->Type = PXInputDeviceTypeGamepad;
                                break;

                            case 0x08:
                                pxInputDevice->Type = PXInputDeviceTypeMultiAxisController;
                                break;
                        }

                        break;
                    }
                    case 0x02:
                    {
                        switch(hid->usUsage)
                        {
                            case 0x02:
                                pxInputDevice->Type = PXInputDeviceTypePedals;
                                // PXInputDeviceTypeRudderPedals
                                break;
                        }

                        break;
                    }
                }

                break;
            }
            case RIM_TYPEKEYBOARD:
            {
                RID_DEVICE_INFO_KEYBOARD* keyboard = &info.keyboard;
                PXInputKeyBoard* pxInputKeyBoard = &pxInputDevice->KeyBoard;

                pxInputKeyBoard->Type = keyboard->dwType;
                pxInputKeyBoard->SubType = keyboard->dwSubType;
                pxInputKeyBoard->KeyboardMode = keyboard->dwKeyboardMode;
                pxInputKeyBoard->NumberOfFunctionKeys = keyboard->dwNumberOfFunctionKeys;
                pxInputKeyBoard->NumberOfIndicators = keyboard->dwNumberOfIndicators;
                pxInputKeyBoard->NumberOfKeysTotal = keyboard->dwNumberOfKeysTotal;

                break;
            }
            case RIM_TYPEMOUSE:
            {
                RID_DEVICE_INFO_MOUSE* mouse = &info.mouse;
                PXInputMouse* pxInputMouse = &pxInputDevice->Mouse;

                pxInputMouse->ID = mouse->dwId;
                pxInputMouse->NumberOfButtons = mouse->dwNumberOfButtons;
                pxInputMouse->SampleRate = mouse->dwSampleRate;
                pxInputMouse->HasHorizontalWheel = mouse->fHasHorizontalWheel;

                break;
            }
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXInputSystemDevicesRefreshXInput(PXInputSystem PXREF pxInputSystem)
{
    for(DWORD playerID = 0; playerID < 4; ++playerID) 
    {       
        XINPUT_STATE xInputState;
        DWORD result = XInputGetState(playerID, &xInputState);
        PXBool isOK = ERROR_SUCCESS == result;

        if(!result)
        {
            continue;
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXInputSystemDevicesRefreshDirectInput(PXInputSystem PXREF pxInputSystem)
{
    HRESULT result;
    PXResult pxResult;

    result = DirectInput8Create
    (
        GetModuleHandle(NULL),
        DIRECTINPUT_VERSION,
        &IID_IDirectInput8W,
        (void**)&pxInputSystem->DirectInput8Handle,
        NULL
    );
    pxResult = PXErrorCurrent(result);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    IDirectInput8W* directInput = pxInputSystem->DirectInput8Handle;


    result = 
#if PXLanguageCPP
        directInput->EnumDevices
#else
        directInput->lpVtbl->EnumDevices
#endif
    (
#if PXLanguageC
        directInput,
#endif
        DI8DEVCLASS_ALL,
        PXDirectInputDeviceListCallback,
        pxInputSystem,
        DIEDFL_ATTACHEDONLY
    );
    pxResult = PXErrorCurrent(result);
    
    return pxResult;
}


PXInputSystem _pxInputSystem;

PXResult PXAPI PXInputSystemDevicesRefresh(void)
{
    PXInputSystemDevicesRefreshRawInput(&_pxInputSystem);
    PXInputSystemDevicesRefreshXInput(&_pxInputSystem);
    PXInputSystemDevicesRefreshDirectInput(&_pxInputSystem);   



    for(PXSize i = 0; i < _pxInputSystem.DeviceAmount; i++)
    {
        PXInputDevice* pxInputDevice = &_pxInputSystem.DeviceList[i];

#if PXLogEnable
        const char* typeName = PXInputDeviceTypeToString(pxInputDevice->Type);

        PXLogPrint
        (
            PXLoggingInfo,
            "Device",
            "Fetch",
            "Type:%-10s Rw:%c Yj:%c Dx:%c Xi:%c - Name:%ls",
            typeName,
            pxInputDevice->RAWInput ? 'Y' : '-',
            pxInputDevice->WindowsMM ? 'Y' : '-',         
            pxInputDevice->DirectInput ? 'Y' : '-',
            pxInputDevice->XInput ? 'Y' : '-',
            pxInputDevice->Name
        );
#endif
    }




    return PXResultOK;
}

PXBool SendVibration_0079_0006(UCHAR strong, UCHAR weak)
{
    GUID hidGuid;
    HidD_GetHidGuid(&hidGuid);

    HDEVINFO deviceInfo = SetupDiGetClassDevsW
    (
        &hidGuid,
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_DEVICEINTERFACE
    );

    if(deviceInfo == INVALID_HANDLE_VALUE)
        return FALSE;

    SP_DEVICE_INTERFACE_DATA interfaceData;
    interfaceData.cbSize = sizeof(interfaceData);

    BYTE detailBuffer[1024];
    PSP_DEVICE_INTERFACE_DETAIL_DATA_W detailData =
        (PSP_DEVICE_INTERFACE_DETAIL_DATA_W)detailBuffer;
    detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_W);

    for(DWORD i = 0; SetupDiEnumDeviceInterfaces(deviceInfo, NULL, &hidGuid, i, &interfaceData); i++)
    {
        DWORD required = 0;
        SetupDiGetDeviceInterfaceDetailW(deviceInfo, &interfaceData, NULL, 0, &required, NULL);

        if(required > sizeof(detailBuffer))
            continue;

        if(!SetupDiGetDeviceInterfaceDetailW(deviceInfo, &interfaceData, detailData, sizeof(detailBuffer), NULL, NULL))
            continue;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Device",
            "Search",
            "Name:%ls",
            detailData->DevicePath
        );
#endif

        // vid_0079&pid_0006

        // Check VID/PID
        if(wcsstr(detailData->DevicePath, L"vid_0079") &&
           wcsstr(detailData->DevicePath, L"pid_0006"))
        {

            return SendVibration_HIDPATH(detailData->DevicePath, strong, weak);

            //   SetupDiDestroyDeviceInfoList(deviceInfo);
        }
    }

    SetupDiDestroyDeviceInfoList(deviceInfo);
    return NULL;
}

HANDLE h = 0;

PXBool SendVibration_HIDPATH(WCHAR* path, UCHAR strong, UCHAR weak)
{
    if(!h)
    {
        h = CreateFileW
        (
            path,
            GENERIC_WRITE | GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        PXResult pxResult = PXErrorCurrent(INVALID_HANDLE_VALUE != h);

        if(h == INVALID_HANDLE_VALUE)
            return PXFalse;
    }



    UCHAR report[8];
    PXClearList(UCHAR, report, 8);
    report[0] = 0x01;      // Report ID
    report[1] = strong;   // Strong motor
    report[2] = weak;     // Weak motor

    DWORD written = 0;
    BOOL ok = 0;// WriteFile(h, report, sizeof(report), &written, NULL);
    BOOLEAN x = 0;

    if(!ok || 1)
    {
        x = HidD_SetOutputReport(h, report, sizeof(report));
    }

  //  CloseHandle(h);
  
    return ok;
}



PXResult PXAPI PXInputDevicePoll(PXInputDevice PXREF pxInputDevice)
{
    HRESULT result;
    PXResult pxResult;

    DIJOYSTATE2 directInputJoyState;
    ZeroMemory(&directInputJoyState, sizeof(directInputJoyState));

    IDirectInputDevice8W* device = pxInputDevice->DeviceInterface;


    // 1. Try to acquire if not already acquired
    if(!pxInputDevice->IsAquired)
    {
        pxInputDevice->IsAquired = PXTrue;
        result = device->lpVtbl->Acquire(device);
    }

    // 2. Poll the device (required for joysticks/gamepads)
    result = device->lpVtbl->Poll(device);
    pxResult = PXErrorFromHRESULT(result);

    if(PXResultOK != pxResult)
    {
        // 3. If Poll failed, try to re-acquire
        result = device->lpVtbl->Acquire(device);
        pxResult = PXErrorFromHRESULT(result);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }

        // Try polling again after re-acquire
        result = device->lpVtbl->Poll(result);
        pxResult = PXErrorFromHRESULT(result);

        if(PXResultOK != pxResult)
        {
            return pxResult;
        }
    }

    result = device->lpVtbl->GetDeviceState
    (
        device,
        sizeof(DIJOYSTATE2),
        &directInputJoyState
    );
    pxResult = PXErrorFromHRESULT(result);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    // 4. Get the device state
    result = device->lpVtbl->GetDeviceState
    (
        device,
        sizeof(DIJOYSTATE2),
        &directInputJoyState
    );
    pxResult = PXErrorFromHRESULT(result);



    // Convert
    PXController* pxController = &pxInputDevice->ControllerData;

    pxController->Axis[0] = directInputJoyState.lX;
    pxController->Axis[1] = directInputJoyState.lY;
    pxController->Axis[2] = directInputJoyState.lZ;
    pxController->Axis[3] = directInputJoyState.lRx;
    pxController->Axis[4] = directInputJoyState.lRy;
    pxController->Axis[5] = directInputJoyState.lRz;    
    pxController->Axis[6] = directInputJoyState.rglSlider[0];
    pxController->Axis[7] = directInputJoyState.rglSlider[1];

    pxController->AxisVelocity[0] = directInputJoyState.lVX;
    pxController->AxisVelocity[1] = directInputJoyState.lVY;
    pxController->AxisVelocity[2] = directInputJoyState.lVZ;
    pxController->AxisVelocity[3] = directInputJoyState.lVRx;
    pxController->AxisVelocity[4] = directInputJoyState.lVRy;
    pxController->AxisVelocity[5] = directInputJoyState.lVRz;
    pxController->AxisVelocity[6] = directInputJoyState.rglVSlider[0];
    pxController->AxisVelocity[7] = directInputJoyState.rglVSlider[1];

   // directInputJoyState.rgbButtons;

    return pxResult;
}

PXInputDevice* PXAPI PXInputControllerGet(const PXSize index)
{
    PXSize matchCounter = 0;

    for(PXSize i = 0; i < _pxInputSystem.DeviceAmount; i++)
    {
        PXInputDevice* pxInputDevice = &_pxInputSystem.DeviceList[i];

        if(pxInputDevice->Type == PXInputDeviceTypeJoystick)
        {
            if(matchCounter == index)
            {
                return pxInputDevice;
            }

            ++matchCounter;
        }
    }

    return PXNull;
}


BOOL CALLBACK PXInputEnumEffectsCallback(LPCDIEFFECTINFOW pei, LPVOID pvRef)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Device",
        "Fetch",
        "GUID:%4.4x Name:%ls",
        pei->guid,
        pei->tszName
    );
#endif

    return DIENUM_CONTINUE;
}

PXResult PXAPI PXInputDeviceOpen(PXInputDevice PXREF pxInputDevice, PXWindow PXREF pxWindow)
{
    if(!(pxInputDevice && pxWindow))
    {
        return PXResultRefusedParameterNull;
    }

    IDirectInput8W* directInput = &_pxInputSystem;
    HRESULT result;
    PXResult pxResult;

    result = directInput->lpVtbl->CreateDevice
    (
        directInput,
        &pxInputDevice->DeviceGUID,
        &pxInputDevice->DeviceInterface, 
        PXNull
    );
    pxResult = PXErrorFromHRESULT(result);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    LPDIRECTINPUTDEVICE8W device = pxInputDevice->DeviceInterface;


    /*
    result = device->lpVtbl->SetProperty
    (
        device,
        DIPROP_AUTOCENTER
    );*/

    result = device->lpVtbl->SetCooperativeLevel
    (
        device,
        PXWindowHandleGet(pxWindow),
        DISCL_FOREGROUND | DISCL_EXCLUSIVE // DISCL_NONEXCLUSIVE
    );
    pxResult = PXErrorFromHRESULT(result);



    DIDEVCAPS deviceCapabilities;
    PXClear(DIDEVCAPS, &deviceCapabilities);
    deviceCapabilities.dwSize = sizeof(deviceCapabilities);

    result = device->lpVtbl->GetCapabilities(device, &deviceCapabilities);
    pxResult = PXErrorFromHRESULT(result);

    PXBool vibrat = (deviceCapabilities.dwFlags & DIDC_FORCEFEEDBACK) > 0;


    result = device->lpVtbl->SetDataFormat
    (
        device,
        &c_dfDIJoystick2
    );
    pxResult = PXErrorFromHRESULT(result);



    result = device->lpVtbl->EnumEffects
    (
        device,
        PXInputEnumEffectsCallback,
        PXNull, 
        DIEFT_ALL // enumerate all effect types
    );
    pxResult = PXErrorFromHRESULT(result);




    DICONSTANTFORCE cf;
    cf.lMagnitude = 5000; // 0–10000

    DIEFFECT eff;
    ZeroMemory(&eff, sizeof(eff));
    eff.dwSize = sizeof(DIEFFECT);
    eff.dwFlags = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
    eff.dwDuration = INFINITE; // run until stopped
    eff.dwSamplePeriod = 0;
    eff.dwGain = DI_FFNOMINALMAX;
    eff.dwTriggerButton = DIEB_NOTRIGGER;
    eff.cAxes = 1;

    LONG axes[1] = { DIJOFS_X };
    LONG dir[1] = { 0 };

    eff.rgdwAxes = (DWORD*)axes;
    eff.rglDirection = (LONG*)dir;
    eff.cbTypeSpecificParams = sizeof(DICONSTANTFORCE);
    eff.lpvTypeSpecificParams = &cf;

    LPDIRECTINPUTEFFECT effect = PXNull;

    result = device->lpVtbl->CreateEffect
    (
        device, 
        &GUID_ConstantForce,
        &eff, 
        &effect,
        PXNull
    );
    pxResult = PXErrorFromHRESULT(result);
    
    if(PXResultOK != pxResult)
    {
        return pxResult;
    }


    effect->lpVtbl->Start(effect, 1, 0); // 1 = play once, 0 = no flags



    cf.lMagnitude = 8000; // new strength
    effect->lpVtbl->SetParameters
    (
        effect, 
        &eff, 
        DIEP_TYPESPECIFICPARAMS | DIEP_START
    );


    effect->lpVtbl->Stop(effect);


    /*
    XINPUT_VIBRATION vib = {};
vib.wLeftMotorSpeed  = 30000;
vib.wRightMotorSpeed = 60000;
XInputSetState(0, &vib);

    */

    return PXResultOK;
}




#if OSUnix

#elif OSWindows

#include <windows.h>

#if WindowsAtleastVista
#include <joystickapi.h> // Missing?? -> documentation says you should use "Dinput.h" but thats not correct.
#else
#include <MMSystem.h>
#endif

#pragma comment( lib, "winmm.lib" )

typedef UINT(WINAPI* PXjoyGetNumDevs)(void);
typedef MMRESULT(WINAPI* PXjoyGetDevCapsA)(_In_ UINT_PTR uJoyID, _Out_writes_bytes_(cbjc) LPJOYCAPSA pjc, _In_ UINT cbjc);
typedef MMRESULT(WINAPI* PXjoyGetPosEx)(_In_ UINT uJoyID, _Out_ LPJOYINFOEX pji);

#endif

const char PXWindowsMultiMedia[] = "WINMM.DLL";
const PXI8U PXWindowsMultiMediaLength = sizeof(PXWindowsMultiMedia);

PXResult PXAPI PXControllerSystemInitilize(PXMMJoystick PXREF pxControllerSystem)
{
    // Library
    {
        PXText pxText;
        PXTextFromAdressA(&pxText, PXWindowsMultiMedia, sizeof(PXWindowsMultiMediaLength), sizeof(PXWindowsMultiMediaLength));

        const PXResult loadLibResult = PXLibraryOpen(&pxControllerSystem->InputLibrary, &pxText);

        if(PXResultOK != loadLibResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    // Fetch
    {
        PXLibraryFuntionEntry pxLibraryFuntionEntryList[] =
        {
            { &pxControllerSystem->NumDevsGet, "joyGetNumDevs"},
            { &pxControllerSystem->DevCapsGetA,"joyGetDevCapsA"},
            { &pxControllerSystem->GetPosEx,"joyGetPosEx" }
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntryList) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&pxControllerSystem->InputLibrary, pxLibraryFuntionEntryList, amount);
    }

    return PXResultOK;
}

PXResult PXAPI PXControllerSystemShutdown(PXMMJoystick PXREF pxControllerSystem)
{
   // PXMemoryHeapFree(PXNull, pxControllerSystem->DeviceList);

   // PXLibraryClose(&pxControllerSystem->InputLibrary);

   // PXClear(PXControllerSystem, pxControllerSystem);

    return PXResultOK;
}

PXResult PXAPI PXControllerSystemDevicesListRefresh(PXInputSystem PXREF pxControllerSystem)
{
#if OSUnix
#elif OSWindows
    // Get amount
    {
        PXjoyGetNumDevs pxjoyGetNumDevs = pxControllerSystem->JoystickMM.NumDevsGet;

        ///pxControllerSystem->DeviceListAmount = 1;

        //pxControllerSystem->DeviceListAmount = pxjoyGetNumDevs();
    }

   // pxControllerSystem->DeviceListData = PXMemoryHeapCallocT(PXController, pxControllerSystem->DeviceListAmount);

    // Get devices
    {
        const PXjoyGetDevCapsA pxjoyGetDevCapsA = pxControllerSystem->JoystickMM.DevCapsGetA;

        for(PXSize i = 0; i < pxControllerSystem->DeviceAmount; ++i)
        {
            PXController PXREF pxController = &pxControllerSystem->DeviceList[i];

            pxController->ID = i;
            JOYCAPSA pjc;
            const PXSize size = sizeof(JOYCAPSA);
            const MMRESULT devResult = pxjoyGetDevCapsA(i, &pjc, size);
            const PXResult pxResult = PXErrorCurrent(JOYERR_NOERROR == devResult);

            if(PXResultOK != pxResult)
            {
                return pxResult;
            }

          //  PXTextCopyA(pjc.szPname, MAXPNAMELEN, pxController->Name, PXControllerNameSize);

            pxController->ButtonAmountInUse = pjc.wNumButtons;
            pxController->AxesAmountInUse = pjc.wNumAxes;

            pxController->AxisMax[PXControllerAxisX] = pjc.wXmax;
            pxController->AxisMax[PXControllerAxisY] = pjc.wYmax;
            pxController->AxisMax[PXControllerAxisZ] = pjc.wZmax;
            pxController->AxisMax[PXControllerAxisR] = pjc.wRmax;
            pxController->AxisMax[PXControllerAxisU] = pjc.wUmax;
            pxController->AxisMax[PXControllerAxisV] = pjc.wVmax;

            pxController->AxisMin[PXControllerAxisX] = pjc.wXmin;
            pxController->AxisMin[PXControllerAxisY] = pjc.wYmin;
            pxController->AxisMin[PXControllerAxisZ] = pjc.wZmin;
            pxController->AxisMin[PXControllerAxisR] = pjc.wRmin;
            pxController->AxisMin[PXControllerAxisU] = pjc.wUmin;
            pxController->AxisMin[PXControllerAxisV] = pjc.wVmin;
        }
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXControllerSystemDevicesDataUpdate(PXInputSystem PXREF pxControllerSystem)
{
    for(PXSize i = 0; i < pxControllerSystem->DeviceAmount; ++i)
    {
        PXInputDevice PXREF pxInputDevice = &pxControllerSystem->DeviceList[i];
        PXController PXREF pxController = &pxInputDevice->ControllerData;

#if OSUnix
        return PXActionRefusedNotImplemented;

#elif WindowsAtleastVista



#if (Version_Windows_NT)
        JOYINFOEX joystickInfo; // must set the 'dwSize' and 'dwFlags' or joyGetPosEx will fail.

        joystickInfo.dwSize = sizeof(JOYINFOEX);
        joystickInfo.dwFlags = JOY_RETURNALL;

        // For devices that have four to six axes of movement a point-of-view control,
        // or more than four buttons, use the joyGetPosEx function. 'joyGetPos()'

        const PXjoyGetPosEx pxjoyGetPosEx = pxControllerSystem->JoystickMM.GetPosEx;
        const MMRESULT positionInfoResult = pxjoyGetPosEx(pxController->ID, &joystickInfo);
        const PXBool successful = positionInfoResult == 0;

        if(successful)
        {
            pxController->Axis[PXControllerAxisX] = joystickInfo.dwXpos;
            pxController->Axis[PXControllerAxisY] = joystickInfo.dwYpos;
            pxController->Axis[PXControllerAxisZ] = joystickInfo.dwZpos;
            pxController->Axis[PXControllerAxisR] = joystickInfo.dwRpos;
            pxController->Axis[PXControllerAxisU] = joystickInfo.dwUpos;
            pxController->Axis[PXControllerAxisV] = joystickInfo.dwVpos;
            pxController->ButtonPressedBitList = joystickInfo.dwButtons;
            pxController->ButtonAmountPressed = joystickInfo.dwButtonNumber;
            pxController->ControlPad = joystickInfo.dwPOV;

            pxController->AxisNormalised[PXControllerAxisX] = (pxController->Axis[PXControllerAxisX] * 2.0f) / (PXF32)0xFFFF - 1;
            pxController->AxisNormalised[PXControllerAxisY] = (pxController->Axis[PXControllerAxisY] * 2.0f) / (PXF32)0xFFFF - 1;
            pxController->AxisNormalised[PXControllerAxisZ] = (pxController->Axis[PXControllerAxisZ] * 2.0f) / (PXF32)0xFFFF - 1;
            pxController->AxisNormalised[PXControllerAxisR] = (pxController->Axis[PXControllerAxisR] * 2.0f) / (PXF32)0xFFFF - 1;
            pxController->AxisNormalised[PXControllerAxisU] = (pxController->Axis[PXControllerAxisU] * 2.0f) / (PXF32)0xFFFF - 1;
            pxController->AxisNormalised[PXControllerAxisV] = (pxController->Axis[PXControllerAxisV] * 2.0f) / (PXF32)0xFFFF - 1;
        }
#else
        JOYINFO joystickInfo{ 0 };

        const MMRESULT positionInfoResult = joyGetPos(controllerID, &joystickInfo);
        const bool successful = positionInfoResult == 0;

        if(successful)
        {
            controller.Axis[ControllerAxisX] = joystickInfo.wXpos;
            controller.Axis[ControllerAxisY] = joystickInfo.wYpos;
            controller.Axis[ControllerAxisZ] = joystickInfo.wZpos;
            controller.ButtonPressedBitList = joystickInfo.wButtons;
        }

        return successful;
#endif
#else
        return PXFalse;
#endif
    }

    return PXResultOK;
}

void PXAPI PXControllerSystemDebugPrint(PXController PXREF pxController)
{
    //PXConsoleClear();
    PXConsoleGoToXY(0, 0);

    char bufferAAA[64];

    PXTextFromIntToBinary16U(bufferAAA, 64, pxController->ButtonPressedBitList);

    PXLogPrint
    (
        PXLoggingInfo,
        "Controller",
        "Data",
        "%s, ID: %i\n"
        "Axis  : X:%5i, Y:%5i, Z:%5i, R:%5i, U:%5i, V:%5i\n"
        "Button: %s",
        "N/A",
        pxController->ID,
        pxController->Axis[PXControllerAxisX],
        pxController->Axis[PXControllerAxisY],
        pxController->Axis[PXControllerAxisZ],
        pxController->Axis[PXControllerAxisR],
        pxController->Axis[PXControllerAxisU],
        pxController->Axis[PXControllerAxisV],
        bufferAAA
    );
}

#if PXWindowUSE
PXBool PXAPI PXControllerAttachToWindow(const PXControllerID controllerID, const PXWindowID PXWindowID)
{
#if OSUnix
    return 0u;

#elif WindowsAtleastVista
    UINT uPeriod = 1;
    BOOL fChanged = 1u;

    const MMRESULT captureResult = joySetCapture(PXWindowID, controllerID, uPeriod, fChanged);
    const PXBool successful = captureResult == JOYERR_NOERROR;

    return successful;
#else
    return PXFalse;
#endif
}

PXBool PXAPI PXControllerDetachToWindow(const PXControllerID controllerID)
{
#if OSUnix
    return 0u;

#elif WindowsAtleastVista
    const MMRESULT releaseResult = joyReleaseCapture(controllerID);
    const unsigned char successful = releaseResult == JOYERR_NOERROR;

    return successful;
#else
    return PXFalse;
#endif
}
#endif

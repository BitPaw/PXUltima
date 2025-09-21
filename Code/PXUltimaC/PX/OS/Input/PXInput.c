#include "PXInput.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>


#include <windows.h>
#include <setupapi.h>

#include <stdio.h>

#pragma comment(lib, "setupapi.lib")

PXResult PXAPI PXDeviceNameResolveA(const char* devicePath, const PXSize textSize, char* const textResol, PXSize* const textSizeResol)
{
    *textSizeResol = PXTextCopyA("*No device name*", 17, textResol, *textSizeResol);

    HDEVINFO hDevInfo = SetupDiGetClassDevsA(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if(hDevInfo == INVALID_HANDLE_VALUE) 
        return 0;

    SP_DEVINFO_DATA devInfoData;
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    DWORD i = 0;
    while(SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData)) {
        char instanceId[512];
        if(SetupDiGetDeviceInstanceIdA(hDevInfo, &devInfoData, instanceId, sizeof(instanceId), NULL)) {
            if(strstr(devicePath, instanceId)) {
                // Try friendly name
                if(SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, NULL,
                   (BYTE*)textResol, *textSizeResol, NULL)) {
                    
                    break;
                }

                // Try device description
                if(SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_DEVICEDESC, NULL,
                   (BYTE*)textResol, *textSizeResol, NULL)) {
                    
                    break;
                }

                // Try manufacturer
                if(SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_MFG, NULL,
                   (BYTE*)textResol, *textSizeResol, NULL)) {
          
                    break;
                }

                break;  // Match found but no name, fallback to "device"
            }
        }
        i++;
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
    ///return name;

    return PXActionInvalid;
}

void PXAPI PXDeviceFill(PXDeviceInput* const pxDeviceInput, const HANDLE deviceHandle)
{
    RID_DEVICE_INFO deviceInfo;
    deviceInfo.cbSize = sizeof(RID_DEVICE_INFO);
    UINT size = sizeof(RID_DEVICE_INFO);

    UINT AA = GetRawInputDeviceInfo(deviceHandle, RIDI_DEVICEINFO, &deviceInfo, &size) > 0;

    if(AA == 0)
    {
        return;
    }

    switch(deviceInfo.dwType)
    {
        case RIM_TYPEKEYBOARD:
        {
            RID_DEVICE_INFO_KEYBOARD* const deviceInfoKeyBoard = &deviceInfo.keyboard;
            pxDeviceInput->Type = PXDeviceInputTypeKeyBoard;

            pxDeviceInput->KeyBoard.dwType = deviceInfoKeyBoard->dwType;
            pxDeviceInput->KeyBoard.dwSubType = deviceInfoKeyBoard->dwSubType;
            pxDeviceInput->KeyBoard.dwKeyboardMode = deviceInfoKeyBoard->dwKeyboardMode;
            pxDeviceInput->KeyBoard.dwNumberOfFunctionKeys = deviceInfoKeyBoard->dwNumberOfFunctionKeys;
            pxDeviceInput->KeyBoard.dwNumberOfIndicators = deviceInfoKeyBoard->dwNumberOfIndicators;
            pxDeviceInput->KeyBoard.dwNumberOfKeysTotal = deviceInfoKeyBoard->dwNumberOfKeysTotal;

            break;
        }
        case RIM_TYPEMOUSE:
        {
            RID_DEVICE_INFO_MOUSE* const deviceInfoMouse = &deviceInfo.mouse;
            pxDeviceInput->Type = PXDeviceInputTypeMouse;

            pxDeviceInput->Mouse.dwId = deviceInfoMouse->dwId;
            pxDeviceInput->Mouse.dwNumberOfButtons = deviceInfoMouse->dwNumberOfButtons;
            pxDeviceInput->Mouse.dwSampleRate = deviceInfoMouse->dwSampleRate;
            pxDeviceInput->Mouse.fHasHorizontalWheel = deviceInfoMouse->fHasHorizontalWheel;

            break;
        }
        case RIM_TYPEHID:
        {
            RID_DEVICE_INFO_HID* const deviceInfoHID = &deviceInfo.hid;
            pxDeviceInput->Type = PXDeviceInputTypeGeneric;

            pxDeviceInput->Generic.dwVendorId = deviceInfoHID->dwVendorId;
            pxDeviceInput->Generic.dwProductId = deviceInfoHID->dwProductId;
            pxDeviceInput->Generic.dwVersionNumber = deviceInfoHID->dwVersionNumber;
            pxDeviceInput->Generic.usUsagePage = deviceInfoHID->usUsagePage;
            pxDeviceInput->Generic.usUsage = deviceInfoHID->usUsage;

            break;
        }
        default:
        {
            return;
        }
    }
}

PXResult PXAPI PXDeviceInputUpdate(PXDeviceInputUpdateInfo* const pxDeviceInputUpdateInfo)
{


    PXDeviceInput pxDeviceInput;

    PXDeviceFill(&pxDeviceInput, pxDeviceInputUpdateInfo->DeviceHandle);

#if PXLogEnable

    char* typeText = 0;

    switch(pxDeviceInput.Type)
    {
        case PXDeviceInputTypeMouse:
            typeText = "Mouse";
            break;

        case PXDeviceInputTypeKeyBoard:
            typeText = "KeyBoard";
            break;

        case PXDeviceInputTypeGeneric:
            typeText = "Generic";
            break;

        default:
            break;
    }

    PXLogPrint
    (
        PXLoggingInfo,
        "DeviceInput",
        "Update",
        "%s",
        typeText
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXDeviceInputFetch(PXDeviceInput** const pxDeviceInput, PXSize* const amount)
{
    if(!amount)
    {
        return PXActionRefusedArgumentNull;
    }

    UINT numDevices = 0;

    // Count the amount of devices
    {
        UINT numDevicesRes = GetRawInputDeviceList(NULL, &numDevices, sizeof(RAWINPUTDEVICELIST)); // Windows XP, User32.dll, winuser.h
        PXResult pxResult = PXErrorCurrent(-1 == numDevicesRes);

        if(PXActionSuccessful != pxResult)
        {
            *amount = 0;
            return pxResult;
        }

        *amount = numDevices;
    }

    // if we dont have an array, quit now
    if(!pxDeviceInput)
    {
        return PXActionSuccessful;
    }

    PXDeviceInput* pxDeviceInputList = PXMemoryHeapCallocT(PXDeviceInput, numDevices);
    RAWINPUTDEVICELIST* deviceList = PXMemoryHeapCallocT(RAWINPUTDEVICELIST, numDevices);
   
    if(!deviceList) 
        return PXActionFailedMemoryAllocation;

    *pxDeviceInput = pxDeviceInputList;

    // Get devices
    numDevices = GetRawInputDeviceList(deviceList, &numDevices, sizeof(RAWINPUTDEVICELIST));

    if(numDevices == 0)
    {
        PXMemoryHeapFree(PXNull, deviceList);
        return;
    }

    for(UINT i = 0; i < numDevices; ++i) 
    {
        PXDeviceInput* const pxDeviceInput = &pxDeviceInputList[i];
        RAWINPUTDEVICELIST* const rawINPUTDEVICELIST = &deviceList[i];

        PXDeviceFill(pxDeviceInput, rawINPUTDEVICELIST->hDevice);
      
        // Get name
        {
            UINT deviceNameSize = 0;

            // First call to get the size of the buffer needed
            if(GetRawInputDeviceInfo(rawINPUTDEVICELIST->hDevice, RIDI_DEVICENAME, NULL, &deviceNameSize) == -1) {
                //printf("Failed to get device name size.\n");
                return;
            }

            char xxxxxxxxx[260];

            // Second call to get the actual device name
            if(GetRawInputDeviceInfo(rawINPUTDEVICELIST->hDevice, RIDI_DEVICENAME, xxxxxxxxx, &deviceNameSize) < 0) {
                //printf("Failed to get device name.\n");
               // free(deviceName);
                return;
            }

            PXSize siii = 260;

            PXDeviceNameResolveA(xxxxxxxxx, deviceNameSize, pxDeviceInput->Name, &siii);
        }    
    }

    free(deviceList);

    return PXActionSuccessful;
}
#include "PXGraphicsCard.h"

#include <PX/OS/Console/PXConsole.h>


const char PXGraphicsCardName[] = "GraphicsCard";
const PXI8U PXGraphicsCardNameLength = sizeof(PXGraphicsCardName);
const PXECSRegisterInfoStatic PXGraphicsCardRegisterInfoStatic =
{
    {sizeof(PXGraphicsCardName), sizeof(PXGraphicsCardName), PXGraphicsCardName, TextFormatASCII},
    sizeof(PXGraphicsCard),
    __alignof(PXGraphicsCard),
    PXECSTypeResource,
    PXNull,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXGraphicsCardRegisterInfoDynamic;


PXResult PXAPI PXGraphicsCardRegisterToECS()
{
    PXECSRegister(&PXGraphicsCardRegisterInfoStatic, &PXGraphicsCardRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXGraphicsCardFetchList()
{
#if OSUnix
#elif OSWindows
    DISPLAY_DEVICEW displayDevice;
    displayDevice.cb = sizeof(DISPLAY_DEVICEW);
    DWORD displayDeviceIndex = 0;

    for(;;)
    {
        const BOOL isOK = EnumDisplayDevicesW(NULL, displayDeviceIndex, &displayDevice, 0);

        if(!isOK)
        {
            break;
        }

        PXBool isActive = (DISPLAY_DEVICE_ACTIVE & displayDevice.StateFlags) > 0;
        PXBool isDeviceAttached = (DISPLAY_DEVICE_ATTACHED_TO_DESKTOP & displayDevice.StateFlags) > 0;
        PXBool isDevice = isActive || isDeviceAttached;

        if(!isDevice)
        {
            continue;
        }

        ++displayDeviceIndex;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXGraphicsCardName,
            "GPU",
            "%20s : %s\n"
            "%20s : %s\n"
            "%20s : %s\n"
            "%20s : %s"
            "DeviceName", displayDevice.DeviceName,
            "DeviceString", displayDevice.DeviceString,
            "DeviceID", displayDevice.DeviceID,
            "DeviceKey", displayDevice.DeviceKey
        );
#endif
    }
#endif

    return PXResultOK;
}
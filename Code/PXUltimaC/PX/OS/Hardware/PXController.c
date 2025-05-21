#include "PXController.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/PXOS.h>

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

PXActionResult PXAPI PXControllerSystemInitilize(PXControllerSystem* const pxControllerSystem)
{
    // Library
    {
        const PXActionResult loadLibResult = PXLibraryOpenA(&pxControllerSystem->InputLibrary, "WINMM.DLL");

        if(PXActionSuccessful != loadLibResult)
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

    return PXActionSuccessful;
}

void PXAPI PXControllerSystemShutdown(PXControllerSystem* const pxControllerSystem)
{
    PXMemoryHeapFree(PXNull, pxControllerSystem->DeviceListData);

    PXLibraryClose(&pxControllerSystem->InputLibrary);

    PXClear(PXControllerSystem, pxControllerSystem);
}

PXActionResult PXAPI PXControllerSystemDevicesListRefresh(PXControllerSystem* const pxControllerSystem)
{
#if OSUnix
#elif OSWindows
    // Get amount
    {
        PXjoyGetNumDevs pxjoyGetNumDevs = pxControllerSystem->NumDevsGet;

        pxControllerSystem->DeviceListAmount = 1;

        //pxControllerSystem->DeviceListAmount = pxjoyGetNumDevs();
    }

    pxControllerSystem->DeviceListData = PXMemoryHeapCallocT(PXController, pxControllerSystem->DeviceListAmount);

    // Get devices
    {
        const PXjoyGetDevCapsA pxjoyGetDevCapsA = pxControllerSystem->DevCapsGetA;

        for(PXSize i = 0; i < pxControllerSystem->DeviceListAmount; ++i)
        {
            PXController* const pxController = &pxControllerSystem->DeviceListData[i];

            pxController->ID = i;
            JOYCAPSA pjc;
            const PXSize size = sizeof(JOYCAPSA);
            const MMRESULT devResult = pxjoyGetDevCapsA(i, &pjc, size);
            const PXActionResult pxActionResult = PXErrorCurrent(JOYERR_NOERROR == devResult);

            if(PXActionSuccessful != pxActionResult)
            {
                return pxActionResult;
            }

            PXTextCopyA(pjc.szPname, MAXPNAMELEN, pxController->Name, PXControllerNameSize);

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

    return PXActionSuccessful;
}

PXActionResult PXAPI PXControllerSystemDevicesDataUpdate(PXControllerSystem* const pxControllerSystem)
{
    for(PXSize i = 0; i < pxControllerSystem->DeviceListAmount; ++i)
    {
        PXController* const pxController = &pxControllerSystem->DeviceListData[i];


#if OSUnix
        return PXActionRefusedNotImplemented;

#elif WindowsAtleastVista



#if (Version_Windows_NT)
        JOYINFOEX joystickInfo; // must set the 'dwSize' and 'dwFlags' or joyGetPosEx will fail.

        joystickInfo.dwSize = sizeof(JOYINFOEX);
        joystickInfo.dwFlags = JOY_RETURNALL;

        // For devices that have four to six axes of movement a point-of-view control,
        // or more than four buttons, use the joyGetPosEx function. 'joyGetPos()'

        const PXjoyGetPosEx pxjoyGetPosEx = pxControllerSystem->GetPosEx;
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
        JOYINFO joystickInfo { 0 };

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

    return PXActionSuccessful;
}

void PXAPI PXControllerSystemDebugPrint(PXController* const pxController)
{
    //PXConsoleClear();
    PXConsoleGoToXY(0,0);

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
        pxController->Name,
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

#pragma once

#ifndef PXInputSystemIncluded
#define PXInputSystemIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Math/PXMatrix.h>
#include "PXInputPlayer.h"
#include "PXInputDevice.h"



typedef struct PXMMJoystick_
{
    PXLibrary InputLibrary;

    // Funcitons
    void* NumDevsGet;
    void* DevCapsGetA;
    void* GetPosEx;
}
PXMMJoystick;

typedef struct PXInputSystem_
{
    // LIB
    struct IDirectInput8W* DirectInput8Handle;

    PXSize InputPlayerAmount;
    PXInputPlayer* InputPlayerList;

    PXSize DeviceAmount;
    PXInputDevice* DeviceList;

    PXMMJoystick JoystickMM;
}
PXInputSystem;

PXPublic PXInputPlayer* PXAPI PXInputSystemPlayerGet(const PXSize playerNR);

PXPublic PXResult PXAPI PXInputSystemDevicesRefresh(void);

PXPublic PXResult PXAPI PXInputDeviceOpen(PXInputDevice PXREF pxInputDevice, PXWindow PXREF pxWindow);
PXPublic PXResult PXAPI PXInputDevicePoll(PXInputDevice PXREF pxInputDevice);


PXPublic PXInputDevice* PXAPI PXInputControllerGet(const PXSize index);


PXPublic PXResult PXAPI PXControllerSystemInitilize(PXMMJoystick PXREF pxControllerSystem);
PXPublic PXResult PXAPI PXControllerSystemShutdown(PXMMJoystick PXREF pxControllerSystem);

PXPublic PXResult PXAPI PXControllerSystemDevicesListRefresh(PXInputSystem PXREF pxControllerSystem);
PXPublic PXResult PXAPI PXControllerSystemDevicesDataUpdate(PXInputSystem PXREF pxControllerSystem);

PXPublic void PXAPI PXControllerSystemDebugPrint(PXController PXREF pxController);

#if PXWindowUSE
PXPublic PXBool PXAPI PXControllerAttachToWindow(const PXControllerID controllerID, const PXWindowID PXWindowID);
PXPublic PXBool PXAPI PXControllerDetachToWindow(const PXControllerID controllerID);
#endif


// TODO: TEST
PXPublic PXBool SendVibration_0079_0006(UCHAR strong, UCHAR weak);
PXPublic PXBool SendVibration_HIDPATH(WCHAR* path, UCHAR strong, UCHAR weak);


#endif
#include "PXController.h"

#include <OS/System/PXOSVersion.h>

#if OSUnix

#elif WindowsAtleastVista

#include <windows.h>
#include <joystickapi.h> // Missing?? -> documentation says you should use "Dinput.h" but thats not correct.

#pragma comment( lib, "winmm.lib" )
#endif

PXBool PXControllerScanDevices(NewControllerDetectedCallback callback)
{
#if OSUnix
#elif WindowsAtleastVista
const PXSize amountOfJoySticksSupported = joyGetNumDevs();

	for (PXSize i = 0; i < amountOfJoySticksSupported; i++)
	{
		const PXControllerID joyID = i;
		JOYCAPSW pjc;
		const PXSize size = sizeof(JOYCAPSW);
		const MMRESULT devResult = joyGetDevCapsW(joyID, &pjc, size);
		const PXBool succesful = devResult == JOYERR_NOERROR;

		if (succesful)
		{
			if (callback)
			{
				callback(joyID);
			}
		}

		/*
		//JOYSTICKID1
		switch (devResult)
		{
			case MMSYSERR_BADDEVICEID: // Windows 95 / 98 / Me : The specified joystick identifier is invalid.
			{
				printf("MMSYSERR_BADDEVICEID\n");
				break;
			}
			case JOYERR_NOERROR:
			{




				printf("OK\n");

				break;
			}
			case MMSYSERR_NODRIVER://The joystick driver is not present.
			{
				printf("No Driver\n");
				break;
			}
			case MMSYSERR_INVALPARAM://An invalid parameter was passed.
			{
				printf("Invalid Paramerer\n");
				break;
			}
			case JOYERR_PARMS: //Windows NT / 2000 / XP : The specified joystick identifier is invalid.
			{
				printf("JOYERR_PARMS\n");
				break;
			}
			case JOYERR_UNPLUGGED: // The specified joystick is not connected to the system.
			{
				printf("JOYERR_UNPLUGGED\n");
				break;
			}

			default:
				break;
		}*/
	}
	return amountOfJoySticksSupported > 0;

#else
	return PXFalse;
#endif
}

PXBool PXControllerDataGet(PXController* controller)
{
#if OSUnix
    return 0u;

#elif WindowsAtleastVista
#if (Version_Windows_NT)
	JOYINFOEX joystickInfo; // must set the 'dwSize' and 'dwFlags' or joyGetPosEx will fail.

	joystickInfo.dwSize = sizeof(JOYINFOEX);
	joystickInfo.dwFlags = JOY_RETURNALL;

	// For devices that have four to six axes of movement a point-of-view control,
	// or more than four buttons, use the joyGetPosEx function. 'joyGetPos()'
	const MMRESULT positionInfoResult = joyGetPosEx(controller->ID, &joystickInfo);
	const unsigned char successful = positionInfoResult == 0;

	if (successful)
	{
		controller->Axis[PXControllerAxisX] = joystickInfo.dwXpos;
		controller->Axis[PXControllerAxisY] = joystickInfo.dwYpos;
		controller->Axis[PXControllerAxisZ] = joystickInfo.dwZpos;
		controller->Axis[PXControllerAxisR] = joystickInfo.dwRpos;
		controller->Axis[PXControllerAxisU] = joystickInfo.dwUpos;
		controller->Axis[PXControllerAxisV] = joystickInfo.dwVpos;
		controller->ButtonPressedBitList = joystickInfo.dwButtons;
		controller->ButtonAmountPressed = joystickInfo.dwButtonNumber;
		controller->ControlPad = joystickInfo.dwPOV;


		controller->AxisNormalised[PXControllerAxisX] = (controller->Axis[PXControllerAxisX] * 2.0f) / (float)0xFFFF - 1;
		controller->AxisNormalised[PXControllerAxisY] = (controller->Axis[PXControllerAxisY] * 2.0f) / (float)0xFFFF - 1;
		controller->AxisNormalised[PXControllerAxisZ] = (controller->Axis[PXControllerAxisZ] * 2.0f) / (float)0xFFFF - 1;
		controller->AxisNormalised[PXControllerAxisR] = (controller->Axis[PXControllerAxisR] * 2.0f) / (float)0xFFFF - 1;
		controller->AxisNormalised[PXControllerAxisU] = (controller->Axis[PXControllerAxisU] * 2.0f) / (float)0xFFFF - 1;
		controller->AxisNormalised[PXControllerAxisV] = (controller->Axis[PXControllerAxisV] * 2.0f) / (float)0xFFFF - 1;
	}

	return successful;
#else
	JOYINFO joystickInfo{ 0 };

	const MMRESULT positionInfoResult = joyGetPos(controllerID, &joystickInfo);
	const bool successful = positionInfoResult == 0;

	if (successful)
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

#if PXWindowUSE
PXBool PXControllerAttachToWindow(const PXControllerID controllerID, const PXWindowID PXWindowID)
{
#if OSUnix
    return 0u;

#elif WindowsAtleastVista
	UINT uPeriod = 1;
	BOOL fChanged = 1u;

	const MMRESULT captureResult = joySetCapture(PXWindowID, controllerID, uPeriod, fChanged);
	const unsigned char successful = captureResult == JOYERR_NOERROR;

	return successful;
#else
	return PXFalse;
#endif
}

PXBool PXControllerDetachToWindow(const PXControllerID controllerID)
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
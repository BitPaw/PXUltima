#ifndef PXControllerINCLUDE
#define PXControllerINCLUDE

#define PXControllerAxisX 0
#define PXControllerAxisY 1
#define PXControllerAxisZ 2
#define PXControllerAxisR 3
#define PXControllerAxisU 4
#define PXControllerAxisV 5

#include <OS/System/OSVersion.h>
#include <OS/Window/PXWindow.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef unsigned int PXControllerID;

	typedef struct PXController_
	{
		PXControllerID ID;

		PXInt32U Axis[6];  // X,Y,Z,R,U,V
		PXInt32U ButtonPressedBitList;
		PXInt32U ButtonAmountPressed;        // current button number pressed
		PXInt32U ControlPad;
	}
	PXController;

	typedef void (*NewControllerDetectedCallback)(PXControllerID controllerID);

	PXPublic PXBool PXControllerScanDevices(NewControllerDetectedCallback callback);
	PXPublic PXBool PXControllerDataGet(PXController* controller);
	PXPublic PXBool PXControllerAttachToWindow(const PXControllerID controllerID, const PXWindowID PXWindowID);
	PXPublic PXBool PXControllerDetachToWindow(const PXControllerID controllerID);

#ifdef __cplusplus
}
#endif

#endif
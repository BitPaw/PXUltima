#ifndef PXControllerINCLUDE
#define PXControllerINCLUDE

#define PXControllerAxisX 0
#define PXControllerAxisY 1
#define PXControllerAxisZ 2
#define PXControllerAxisR 3
#define PXControllerAxisU 4
#define PXControllerAxisV 5

#include <Media/PXType.h>
#include <OS/Window/PXWindow.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef PXInt32U PXControllerID;

	typedef struct PXController_
	{
		PXControllerID ID;

		PXInt32S Axis[6];  // X,Y,Z,R,U,V
		float AxisNormalised[6];

		PXInt32U ButtonPressedBitList;
		PXInt32U ButtonAmountPressed;        // current button number pressed
		PXInt32U ControlPad;
	}
	PXController;

	typedef void (*NewControllerDetectedCallback)(PXControllerID controllerID);

	PXPublic PXBool PXControllerScanDevices(NewControllerDetectedCallback callback);
	PXPublic PXBool PXControllerDataGet(PXController* controller);

#if PXWindowUSE
	PXPublic PXBool PXControllerAttachToWindow(const PXControllerID controllerID, const PXWindowID PXWindowID);
	PXPublic PXBool PXControllerDetachToWindow(const PXControllerID controllerID);
#endif

#ifdef __cplusplus
}
#endif

#endif
#ifndef PXControllerINCLUDE
#define PXControllerINCLUDE

#include <Media/PXResource.h>
#include <OS/GUI/PXGUI.h>

#define PXControllerButton1 (1 << 0)
#define PXControllerButton2 (1 << 1)
#define PXControllerButton3 (1 << 2)
#define PXControllerButton4 (1 << 3)
#define PXControllerButtonR1 (1 << 4)
#define PXControllerButtonR2 (1 << 5)
#define PXControllerButtonL1 (1 << 6)
#define PXControllerButtonL2 (1 << 7)
#define PXControllerButtonR3 (1 << 8)
#define PXControllerButtonL3 (1 << 9)

#define PXControllerAxisX 0
#define PXControllerAxisY 1
#define PXControllerAxisZ 2
#define PXControllerAxisR 3
#define PXControllerAxisU 4
#define PXControllerAxisV 5

#define PXControllerNameSize 32

typedef struct PXController_
{
	char Name[PXControllerNameSize];
	PXInt32U ID;

	PXInt32S Axis[6];  // X,Y,Z,R,U,V
	PXInt32S AxisMin[6];
	PXInt32S AxisMax[6];

	float AxisNormalised[6];

	PXInt32U ButtonPressedBitList;
	PXInt32U ButtonAmountPressed;        // current button number pressed
	PXInt32U ControlPad;


	PXInt32U ButtonAmountInUse;
	PXInt32U AxesAmountInUse;
}
PXController;


typedef struct PXControllerSystem_
{
	PXLibrary InputLibrary;

	// Funcitons
	void* NumDevsGet;
	void* DevCapsGetA;
	void* GetPosEx;

	// Number of devices
	PXSize DeviceListAmount;
	PXController* DeviceListData;
}
PXControllerSystem;


PXPublic void PXAPI PXControllerSystemInitilize(PXControllerSystem* const pxControllerSystem);
PXPublic PXActionResult PXAPI PXControllerSystemDevicesListRefresh(PXControllerSystem* const pxControllerSystem);
PXPublic PXActionResult PXAPI PXControllerSystemDevicesDataUpdate(PXControllerSystem* const pxControllerSystem);

PXPublic void PXAPI PXControllerSystemDebugPrint(PXController* const pxController);

#if PXWindowUSE
PXPublic PXBool PXAPI PXControllerAttachToWindow(const PXControllerID controllerID, const PXWindowID PXWindowID);
PXPublic PXBool PXAPI PXControllerDetachToWindow(const PXControllerID controllerID);
#endif

#endif
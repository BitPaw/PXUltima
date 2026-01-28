#pragma once

#ifndef PXControllerIncluded
#define PXControllerIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Library/PXLibrary.h>

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
    PXECSInfo Info;

    PXI32U ID;

    PXI32S Axis[8]; // X, Y, Z, R, U, V
    PXI32S AxisVelocity[8];

    PXI32S AxisMin[6];
    PXI32S AxisMax[6];

    PXF32 AxisNormalised[6];

    PXI32U ButtonPressedBitList;
    PXI32U ButtonAmountPressed;        // current button number pressed
    PXI32U ControlPad;


    PXI32U ButtonAmountInUse;
    PXI32U AxesAmountInUse;
}
PXController;



#endif

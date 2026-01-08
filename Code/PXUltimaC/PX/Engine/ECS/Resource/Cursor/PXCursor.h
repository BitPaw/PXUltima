#pragma once

#ifndef PXCursorIncluded
#define PXCursorIncluded

#define UseRawMouseData 1
#define PXWindowTitleSizeMax 256
#define PXWindowSizeDefault -1

#include <PX/Media/PXType.h>

// The mode in which the mouse pointer will be.
typedef enum PXWindowCursorMode_
{
    PXWindowCursorIgnore,     // Show the cursor but dont register any Input.
    PXWindowCursorShow,     // Show the cursor and register Input as normal.
    PXWindowCursorInvisible,     // Hide Cursor. You can still use it as normal. Not locked.
    PXWindowCursorLock,
    PXWindowCursorLockAndHide
}
PXWindowCursorMode;

typedef enum PXCursorIcon_
{
    PXCursorIconInvalid,
    PXCursorIconNormal,
    PXCursorIconIBeam,
    PXCursorIconWait,
    PXCursorIconCross,
    PXCursorIconUp,
    PXCursorIconHand,
    PXCursorIconNotAllowed,
    PXCursorIconAppStarting,
    PXCursorIconResizeHorizontal,
    PXCursorIconResizeVertical,
    PXCursorIconResizeClockwise,
    PXCursorIconResizeClockwiseCounter,
    PXCursorIconResizeAll,
}
PXCursorIcon;

PXPublic PXI32U PXAPI PXCursorIconToID(const PXCursorIcon cursorIcon);

#endif
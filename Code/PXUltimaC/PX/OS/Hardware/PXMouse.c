#include "PXMouse.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Console/PXConsole.h>

void PXAPI PXMouseInputReset(PXMouse PXREF mouse)
{
    //PXMemoryClear(mouse, sizeof(PXMouse));
    mouse->Delta[0] = 0;
    mouse->Delta[1] = 0;
}

void PXAPI PXMouseInputPrint(const PXMouse PXREF mouse)
{
    const PXSize buttonTextSize = 2 + 4 * 8+1;
    char buttons[34+1];

    PXTextFromBinaryDataA(&mouse->Buttons, sizeof(PXI8U), buttons, buttonTextSize);

    PXLogPrint
    (
        PXLoggingInfo,
        "Mouse",
        "Input",
        "Data\n",
        "+-----------+---------------|\n"
        "| Position  | %4i / %-4i |\n"
        "| InputAxis | %-3.2f / %-3.2f |\n"
        "| Buttons   | %11s |\n"
        "+-----------+---------------|\n",
        mouse->Position[0],
        mouse->Position[1],
        mouse->PositionNormalisized[0],
        mouse->PositionNormalisized[1],
        buttons
    );
}

void PXAPI PXMouseInputFetch(PXMouse PXREF mouse)
{
#if OSUnix
#elif OSWindows
    MOUSEMOVEPOINT mouseMovePoint;


    MOUSEMOVEPOINT mouseMovePointList[64];
    PXClearList(MOUSEMOVEPOINT, mouseMovePointList, 64);

    const PXSize mouseMovePointSize = sizeof(MOUSEMOVEPOINT);

    const int resID = GetMouseMovePointsEx
    (
        mouseMovePointSize,
        &mouseMovePoint,
        mouseMovePointList,
        64,
        GMMP_USE_DISPLAY_POINTS
    );
    const PXResult res = PXErrorCurrent(-1 != resID);


#endif
}
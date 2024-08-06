#include "PXMouse.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <OS/Console/PXConsole.h>

void PXAPI PXMouseInputReset(PXMouse* const mouse)
{
    //PXMemoryClear(mouse, sizeof(PXMouse));
    mouse->Delta[0] = 0;
    mouse->Delta[1] = 0;
}

void PXAPI PXMouseInputPrint(const PXMouse* const mouse)
{
    const PXSize buttonTextSize = 2 + 4 * 8+1;
    char buttons[34+1];
    
    PXTextFromBinaryDataA(&mouse->Buttons, sizeof(PXInt8U), buttons, buttonTextSize);

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
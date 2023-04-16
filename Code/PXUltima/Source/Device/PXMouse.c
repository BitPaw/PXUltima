#include "PXMouse.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <stdio.h>

void MouseInputReset(PXMouse* const mouse)
{
	MemoryClear(mouse, sizeof(PXMouse));
}

void MouseInputPrint(const PXMouse* const mouse)
{
	const PXSize buttonTextSize = 2 + 4 * 8+1;
	char buttons[34+1];
	
	PXTextFromBinaryDataA(&mouse->Buttons, sizeof(unsigned int), buttons, buttonTextSize);

	printf
	(
		"+-----------+---------------|\n"
		"| Position  | %-5i / %-5i |\n"
		"| InputAxis | %-3.2f / %-3.2f |\n"
		"| Buttons   | %s |\n"
		"+-----------+---------------|\n",
		mouse->Position[0],
		mouse->Position[1],
		mouse->PositionNormalisized[0],
		mouse->PositionNormalisized[1],
		buttons
	);
}
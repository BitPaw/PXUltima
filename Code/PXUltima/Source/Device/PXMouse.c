#include "PXMouse.h"

#include <OS/Memory/PXMemory.h>
#include <Text/PXText.h>
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
		"| InputAxis | %-5i / %-5i |\n"
		"| Buttons   | %s |\n"
		"+-----------+---------------|\n",
		mouse->Position[0],
		mouse->Position[1],
		mouse->InputAxis[0],
		mouse->InputAxis[1],
		buttons
	);
}
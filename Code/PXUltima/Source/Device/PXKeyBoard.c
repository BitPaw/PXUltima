#include "PXKeyBoard.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#include <stdio.h>

void KeyBoardInputReset(PXKeyBoard* const keyBoard)
{
	MemoryClear(keyBoard, sizeof(PXKeyBoard));
}

unsigned char KeyBoardKeyPressedGet(PXKeyBoard* const keyBoard, const VirtualKey virtualKey)
{
	return 0;
}

unsigned char KeyBoardKeyPressedSet(PXKeyBoard* const keyBoard, const VirtualKey virtualKey, const unsigned char isPressed)
{
	return 0;
}

void KeyBoardInputPrint(const PXKeyBoard* const keyBoard)
{
	char letters[11];
	char numbers[11];
	char commands[11];
	char actions[11];

	PXTextFromBinaryDataA(&keyBoard->Letters, sizeof(unsigned int), letters, 11u);
	PXTextFromBinaryDataA(&keyBoard->Numbers, sizeof(unsigned int), numbers, 11u);
	PXTextFromBinaryDataA(&keyBoard->Commands, sizeof(unsigned int), commands, 11u);
	PXTextFromBinaryDataA(&keyBoard->Actions, sizeof(unsigned int), actions, 11u);

	printf
	(
		"| Letters  | %s |\n"
		"| Numbers  | %s |\n"
		"| Commands | %s |\n"
		"| PXActions  | %s |\n",
		letters,
		numbers,
		commands,
		actions
	);
}
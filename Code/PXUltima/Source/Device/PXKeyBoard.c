#include "PXKeyBoard.h"

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>

#include <stdio.h>

void PXKeyBoardInputReset(PXKeyBoard* const keyBoard)
{
	PXMemoryClear(keyBoard, sizeof(PXKeyBoard));
}

unsigned char PXKeyBoardKeyPressedGet(PXKeyBoard* const keyBoard, const VirtualKey virtualKey)
{
	return 0;
}

unsigned char PXKeyBoardKeyPressedSet(PXKeyBoard* const keyBoard, const VirtualKey virtualKey, const unsigned char isPressed)
{
	return 0;
}

void PXKeyBoardInputPrint(const PXKeyBoard* const keyBoard)
{
	char letters[64];
	char numbers[64];
	char commands[64];
	char actions[64];

	PXTextFromBinaryDataA(&keyBoard->Letters, sizeof(unsigned int), letters, 64);
	PXTextFromBinaryDataA(&keyBoard->Numbers, sizeof(unsigned int), numbers, 64);
	PXTextFromBinaryDataA(&keyBoard->Commands, sizeof(unsigned int), commands, 64);
	PXTextFromBinaryDataA(&keyBoard->Actions, sizeof(unsigned int), actions, 64);

	printf
	(
		"+----------+------------------------------------+\n"
		"| Letters  | %34s |\n"
		"| Numbers  | %34s |\n"
		"| Commands | %34s |\n"
		"| Actions  | %34s |\n"
		"+----------+------------------------------------+\n",
		letters,
		numbers,
		commands,
		actions
	);
}
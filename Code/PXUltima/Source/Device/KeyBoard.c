#include "KeyBoard.h"

#include <OS/Memory/PXMemory.h>
#include <Text/Text.h>

#include <stdio.h>

void KeyBoardInputReset(KeyBoard* keyBoard)
{
	MemoryClear(keyBoard, sizeof(KeyBoard));
}

unsigned char KeyBoardKeyPressedGet(KeyBoard* keyBoard, const VirtualKey virtualKey)
{
	return 0;
}

unsigned char KeyBoardKeyPressedSet(KeyBoard* keyBoard, const VirtualKey virtualKey, const unsigned char isPressed)
{
	return 0;
}

void KeyBoardInputPrint(KeyBoard* keyBoard)
{
	char letters[11];
	char numbers[11];
	char commands[11];
	char actions[11];

	TextFromBinaryDataA(&keyBoard->Letters, sizeof(unsigned int), letters, 11u);
	TextFromBinaryDataA(&keyBoard->Numbers, sizeof(unsigned int), numbers, 11u);
	TextFromBinaryDataA(&keyBoard->Commands, sizeof(unsigned int), commands, 11u);
	TextFromBinaryDataA(&keyBoard->PXActions, sizeof(unsigned int), actions, 11u);

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
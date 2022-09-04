
#include <stdio.h>

#include "ImageTest.h"
#include <Processor/Processor.h>
#include <Library/Library.h>

int main()
{
	printf("[i] Starting testing...\n");

	unsigned int x = 0;
	unsigned int y = 0;


	x = ProcessorFrequencyCurrent();
	y = ProcessorTimeReal();

	LibraryParseSymbols();


	printf("[i] Bencah %i, %i\n", x, y);

	TestSaveImageSmal();
	TestSaveImage();
	ImageWriteText();

	printf("[i] Finished testing...\n");

	return 0;
}
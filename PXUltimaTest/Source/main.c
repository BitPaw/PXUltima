
#include <stdio.h>

//#include "TestImage.h"
//#include "TestSBP.h"
#include "TestWindow.h"

//#include <Processor/Processor.h>
//#include <Library/Library.h>

int main()
{
	printf("[i] Starting testing...\n");

	unsigned int x = 0;
	unsigned int y = 0;

	//x = ProcessorFrequencyCurrent();
	//y = ProcessorTimeReal();

	//LibraryParseSymbols();

	printf("[i] Bencah %i, %i\n", x, y);

	TestOpenGLAll();
	//TestWindowAll();
	//TestImageAll();
	//TestSBPAll();

	printf("[i] Finished testing...\n");

	return 0;
}
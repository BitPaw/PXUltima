
#include <stdio.h>

#include "TestSystemInfo.h"
#include "TestWindow.h"
#include "TestOpenGL.h"
#include "TestPXCompiler.h"
#include "TestImage.h"

int main()
{
	printf("[i] Starting testing...\n");

	//TestSystemInfoAll();
	//TestPXCompilerAll();
	TestOpenGLAll();
	//TestWindowAll();
	//TestImageAll();
	//TestSBPAll();

	printf("[i] Finished testing...\n");

	return 0;
}


#include <stdio.h>

#include "TestSystemInfo.h"
#include "TestWindow.h"
#include "TestOpenGL.h"
#include "TestPXCompiler.h"
#include "TestImage.h"
#include "TestYAML.h"

#include <Graphic/DirectX/DirectX.h>

int main()
{
	printf("[i] Starting testing...\n");

	//TestYAMLAll();
	//TestSystemInfoAll();
	//TestPXCompilerAll();
	//TestOpenGLAll();
	TestWindowAll();
	//TestImageAll();
	//TestSBPAll();

	printf("[i] Finished testing...\n");

	return 0;
}

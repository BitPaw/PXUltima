#include "TestWindow.h"

#include <stdio.h>

#include <OS/CWindow.h>

void TestWindowAll()
{
	TestWindowOpen();
}

void TestWindowOpen()
{
	CWindow window;

	CWindowConstruct(&window);

	CWindowCreate(&window, 400, 600, "Test", 1);

	int x = 1;

	while (!window.IsRunning)
	{
		x++;
	}

	OpenGL* openGL = &window.OpenGLContext;

	printf
	(
		"+---<OpenGL>------------------------------------------+\n"
		"| Vendor   : %-40s |\n"
		"| Renderer : %-40s |\n"
		"| Version  : %-40s |\n"
		"+----------+------------------------------------------+\n",
		openGL->Vendor,
		openGL->Renderer,
		openGL->VersionText
	);

	CWindowDestruct(&window);
}
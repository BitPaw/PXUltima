#include "TestWindow.h"

#include <OS/CWindow.h>

void TestWindowAll()
{
	TestWindowOpen();
}

void TestWindowOpen()
{
	CWindow window;

	CWindowConstruct(&window);

	CWindowCreate(&window, 400, 600, "Test", 0);
}
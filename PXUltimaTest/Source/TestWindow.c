#include "TestWindow.h"

#include <stdio.h>

#include <OS/PXWindow.h>
#include <Processor/Processor.h>
#include <Memory/Memory.h>

void TestWindowAll()
{
	TestWindowOpen();
}

void TestWindowOpen()
{
	PXWindow window;

	PXWindowConstruct(&window);

	PXWindowCreate(&window, 400, 600, "[PX] OpenGL-Test", 1);

	int x = 1;

	while (!window.IsRunning)
	{
		x++;
	}

	Processor processor;

	ProcessorFetchInfo(&processor);

	printf
	(
		"+---<CPU>-------------------------------------------------+\n"
		"| Name  : %-47s |\n"
		"| Cores : %-47i |\n",
		processor.BrandName,
		processor.NumberOfProcessors
	);

	MemoryUsage memoryUsage;

	MemoryScan(&memoryUsage);

	printf
	(
		"+---<RAM>-------------------------------------------------+\n"
		"| Size : %-45zi MB |\n",
		memoryUsage.PhysicalAvailable / 1000000
	);


	GraphicContext* graphicContext = &window.GraphicInstance;

	printf
	(
		"+---<OpenGL>----------------------------------------------+\n"
		"| Vendor   : %-44s |\n"
		"| Renderer : %-44s |\n"
		"| Version  : %-44s |\n"
		"+----------+----------------------------------------------+\n",
		graphicContext->OpenGLInstance.Vendor,
		graphicContext->OpenGLInstance.Renderer,
		graphicContext->OpenGLInstance.VersionText
	);

	OpenGLContextSelect(&graphicContext->OpenGLInstance);

	while (1)
	{
		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBegin(GL_POLYGON);
		glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
		glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
		glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
		glEnd();

		glFlush();

		OpenGLRenderBufferSwap(&graphicContext->OpenGLInstance);
	}

	PXWindowDestruct(&window);
}

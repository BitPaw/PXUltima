#include "TestWindow.h"

#include <stdio.h>

#include <OS/Window/PXWindow.h>
#include <OS/Processor/PXProcessor.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Graphic/DirectX/DirectX.h>

void TestWindowAll()
{
	TestWindowOpen();
}

void TestWindowOpen()
{
	PXWindow window;

	PXWindowConstruct(&window);

	PXText title;
	PXTextMakeFixedA(&title, "[PX] PXOpenGL-Test");

	PXWindowCreate(&window, -1, -1, &title, 1);

	int x = 1;

	while (!window.IsRunning)
	{
		x++;
	}

	Processor processor;

	PXProcessorFetchInfo(&processor);

	printf
	(
		"+---<CPU>-------------------------------------------------+\n"
		"| Name  : %-47s |\n"
		"| Ident : %-47s |\n"
		"| Cores : %-47i |\n"
		"| Family: %-47i |\n"
		"| Model : %-47i |\n",
		processor.BrandName,
		processor.IdentityString,
		processor.NumberOfProcessors,
		processor.Family,
		processor.Model
	);

	MemoryUsage memoryUsage;

	PXMemoryScan(&memoryUsage);

	char ramBuffer[256];

	unsigned char biggerThanMB = memoryUsage.PhysicalTotal > 1000000;
	unsigned char biggerThanGB = memoryUsage.PhysicalTotal > 1000000000;

	char ideex =
		'A' * (!biggerThanMB && !biggerThanGB) +
		'B' * (biggerThanMB && !biggerThanGB) +
		'C' * (biggerThanMB && biggerThanGB);
		
	//ideex = 'F';

	switch (ideex)
	{
		case 'A':
			sprintf(ramBuffer, "%zi KB", memoryUsage.PhysicalTotal / 1000);
			break;

		case 'B':
			sprintf(ramBuffer, "%zi MB", memoryUsage.PhysicalTotal /1000000);
			break;
		
		case 'C':
			sprintf(ramBuffer, "%zi GB", memoryUsage.PhysicalTotal / 1000000000);
			break;

		default:
			sprintf(ramBuffer, "%zi Byte", memoryUsage.PhysicalTotal);
			break;
	}
	

	printf
	(
		"+---<RAM>-------------------------------------------------+\n"
		"| Size : %-48s |\n",
		ramBuffer
	);

	// DirectX
	{
		PXDirectXContext directXContext;		

		PXDirectXContextCreate(&directXContext);

		printf
		(
			"+---<DirectX>---------------------------------------------+\n"
			"| Driver      : %-41s |\n"
			"| Description : %-41s |\n"
			"| DeviceName  : %-41s |\n",
			directXContext.Driver,
			directXContext.Description,
			directXContext.DeviceName
		);

		PXDirectXContextDestruct(&directXContext);
	}



	PXGraphicContext* graphicContext = &window.GraphicInstance;

	printf
	(
		"+---<PXOpenGL>----------------------------------------------+\n"
		"| Vendor   : %-44s |\n"
		"| Renderer : %-44s |\n"
		"| Version  : %-44s |\n"
		"| GLSL Ver.: %-44s |\n"
		"+----------+----------------------------------------------+\n",
		graphicContext->PXOpenGLInstance.Vendor,
		graphicContext->PXOpenGLInstance.Renderer,
		graphicContext->PXOpenGLInstance.VersionText,
		graphicContext->PXOpenGLInstance.GLSLVersionText
	);


	PXOpenGLContextSelect(&graphicContext->PXOpenGLInstance);

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

		PXOpenGLRenderBufferSwap(&graphicContext->PXOpenGLInstance);
	}

	PXWindowDestruct(&window);
}

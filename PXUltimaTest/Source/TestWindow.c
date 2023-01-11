#include "TestWindow.h"

#include <stdio.h>

#include <OS/PXWindow.h>
#include <Processor/Processor.h>
#include <Memory/PXMemory.h>
#include <Graphic/DirectX/DirectX.h>

void TestWindowAll()
{
	TestWindowOpen();
}

void TestWindowOpen()
{
	PXWindow window;

	PXWindowConstruct(&window);

	PXWindowCreateA(&window, -1, -1, "[PX] OpenGL-Test", 1);

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

	MemoryScan(&memoryUsage);

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
		DirectXContext directXContext;		

		DirectXContextCreate(&directXContext);

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

		DirectXContextDestruct(&directXContext);
	}



	PXGraphicContext* graphicContext = &window.GraphicInstance;

	printf
	(
		"+---<OpenGL>----------------------------------------------+\n"
		"| Vendor   : %-44s |\n"
		"| Renderer : %-44s |\n"
		"| Version  : %-44s |\n"
		"| GLSL Ver.: %-44s |\n"
		"+----------+----------------------------------------------+\n",
		graphicContext->OpenGLInstance.Vendor,
		graphicContext->OpenGLInstance.Renderer,
		graphicContext->OpenGLInstance.VersionText,
		graphicContext->OpenGLInstance.GLSLVersionText
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

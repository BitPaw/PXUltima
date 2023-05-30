#include "TestOpenGL.h"


#include <Media/PXImage.h>
#include <OS/Memory/PXMemory.h>

#include <stdio.h>
#include <float.h>
#include <OS/Window/PXWindow.h>
#include <Media/PXFont.h>
#include <Math/PXMatrix.h>
#include <Graphic/SoftwareRender/PXSoftwareRender.h>

#include <Graphic/PXGraphic.h>

void TestPXOpenGLAll()
{
	printf("[PXOpenGL] Testing...\n");

	//TestPXOpenGLRenderToTexture();
	//TestPXOpenGLTextDraw();
	TestPXOpenGLVBO();

	printf("[PXOpenGL] Testing finished!\n");
}

void CreateRenderableTexture(PXOpenGLContext* openGLContext, unsigned int width, unsigned int height)
{
	PXOpenGLContextConstruct(openGLContext);
	PXOpenGLContextCreateWindowless(openGLContext, width, height);
	PXOpenGLContextSelect(openGLContext);
	//-------------------------------------------------------------------------

	// Init frame Buffer
	unsigned int frameBufferID = 0;
	PXOpenGLFrameBufferCreate(openGLContext, 1u, &frameBufferID);
	PXOpenGLFrameBufferBind(openGLContext, PXOpenGLFrameBufferModeDrawAndRead, frameBufferID);
	//-------------------------------------------------------------------------

	// Init image
	unsigned int textureID = 0;
	PXOpenGLTextureCreate(openGLContext, 1u, &textureID);
	PXOpenGLTextureBind(openGLContext, PXOpenGLTextureType2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	PXOpenGLTextureBind(openGLContext, PXOpenGLTextureType2D, 0);
	//-------------------------------------------------------------------------

	PXOpenGLFrameBufferLinkTexture2D(openGLContext, PXOpenGLRenderBufferAttachmentPointColor, PXOpenGLTextureType2D, textureID, 0);



	// init renderbuffer
	unsigned int rbo = 0;
	PXOpenGLRenderBufferCreate(openGLContext, 1u, &rbo);
	PXOpenGLRenderBufferBind(openGLContext, rbo);
	PXOpenGLRenderBufferStorage(openGLContext, PXOpenGLRenderBufferFormatDepth24Stencil8, width, height);
	PXOpenGLRenderBufferBind(openGLContext, 0);


	PXOpenGLFrameBufferLinkRenderBuffer(openGLContext, PXOpenGLRenderBufferAttachmentPointStencil, rbo);
	//-------------------------------------------------------------------------

	PXOpenGLClearColor(openGLContext, 0.2f, 0.2f, 0.2f, 1.0f);
	PXOpenGLClear(openGLContext, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PXOpenGLDrawScaleF(openGLContext, 1, -1, 1);

	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}


void DumpRenderedTexture(PXOpenGLContext* openGLContext, const char* filePath)
{
	PXWindow* pxWindow = (PXWindow*)openGLContext->AttachedWindow;

	PXImage image;

	PXImageConstruct(&image);

	PXImageResize(&image, PXColorFormatRGBI8, pxWindow->Width, pxWindow->Height);

	PXOpenGLPixelDataRead(openGLContext, 0, 0, pxWindow->Width, pxWindow->Height, PXOpenGLImageFormatRGB, PXOpenGLTypeByteUnsigned, image.PixelData);

	PXText pxText;

	PXTextMakeFixedA(&pxText, "_TEST_DATA_OUTPUT_/Buffertest.png");

	PXImageSave(&image, &pxText, FileFormatPNG, PXColorFormatRGBI8);

	PXImageDestruct(&image);
}

void TestPXOpenGLRenderToTexture()
{
	float scake = 1;
	unsigned int width = 800 * scake;
	unsigned int height = 600 * scake;

	PXOpenGLContext openGLContext;

	CreateRenderableTexture(&openGLContext, width, height);

	//------------------------------

	PXOpenGLDrawBegin(&openGLContext, PXOpenGLRenderPolygon);
	//glPixelZoom(1, -1);
	//glPerspective(-45.0, width / height, 1.0, 100.0);



	PXOpenGLDrawColorRGBF(&openGLContext, 0, 0, 1); PXOpenGLDrawVertexXYZF(&openGLContext, 0, 0.75, 0);
	PXOpenGLDrawColorRGBF(&openGLContext, 1, 0, 1); PXOpenGLDrawVertexXYZF(&openGLContext, 0.8, 0.8, 0);
	PXOpenGLDrawColorRGBF(&openGLContext, 0, 1, 0); PXOpenGLDrawVertexXYZF(&openGLContext, 0.6, -0.75, 0);
	PXOpenGLDrawColorRGBF(&openGLContext, 1, 0, 0); PXOpenGLDrawVertexXYZF(&openGLContext, -0.6, -0.75, 0.5);

	PXOpenGLDrawEnd(&openGLContext);

	//------------------------------

	DumpRenderedTexture(&openGLContext, "_TEST_DATA_OUTPUT_/Buffertest.png");

	PXOpenGLContextDeselect(&openGLContext);
}

void RenderRectangle(float postionX, float postionY, float textureScaleX, float textureScaleY)
{
	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glTexCoord2f(0* textureScaleX, 1 * textureScaleY);
	glVertex2f(-1 + postionX, -1 + postionY); // bottom-left

	glTexCoord2f(0 * textureScaleX, 0 * textureScaleY);
	glVertex2f(-1 + postionX, 1 + postionY); // top left

	glTexCoord2f(1 * textureScaleX, 0* textureScaleY);
	glVertex2f(1 + postionX, 1 + postionY); // top-right

	glTexCoord2f(1 * textureScaleX, 1 * textureScaleY);
	glVertex2f(1 + postionX, -1 + postionY); // top-left

	glEnd();
}

void DrawStructure(PXOpenGLContext* const openGLContext,float* modelMatrix,  float* vertexList, float* texturePositionList, float* colorList, unsigned int textureID)
{
	PXOpenGLTextureBind(openGLContext, PXOpenGLTextureType2D, textureID);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(modelMatrix);
	//glPushMatrix();

	glEnableClientState(GL_VERTEX_ARRAY);

	if (texturePositionList)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, 0, texturePositionList);
	}

	if (colorList)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(3, GL_FLOAT, 0, colorList);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexList);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	PXOpenGLTextureBind(openGLContext, PXOpenGLTextureType2D, 0);
}

void TestPXOpenGLTextDraw()
{
	float scale = 1;
	unsigned int width = 800 * scale;
	unsigned int height = 600 * scale;

	PXText pxText;

	PXTextMakeFixedA(&pxText, "_TEST_DATA_INPUT_/A.fnt");

	PXFont font;
	PXFontLoad(&font, &pxText);

	PXTexture textureText;
	PXTextureConstruct(&textureText);

	PXOpenGLContext openGLContext;
	CreateRenderableTexture(&openGLContext, width, height);

	//------------------------------




	//---<Draw text into texture>----------------------------------------------
	{
		PXSize textWidth = 1024u;
		PXSize textHeight = 1024;

		PXOpenGLTextureCreate(&openGLContext, 1u, &textureText.ID);
		PXOpenGLTextureBind(&openGLContext, PXOpenGLTextureType2D, textureText.ID);

		PXImageResize(&textureText.Image, PXColorFormatRGBI8, textWidth, textHeight); // Create memory
		PXImageFillColorRGBA8(&textureText.Image, 50, 50, 50, 150);
		
		PXSoftwareRenderDrawRectangle(&textureText.Image, 10, 10, 100, 100, 0xFF, 0, 0, 0xFF); // test
		PXImageDrawTextA(&textureText.Image, 0, 0, textWidth, textHeight, &font, "Sample Text"); // Draw text

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexEnvi(GL_TEXTURE_ENV, 0x8571, GL_MODULATE);

		PXOpenGLTextureData2D(&openGLContext, PXOpenGLTextureType2D, 0, PXOpenGLImageFormatRGBA, textWidth, textHeight, PXOpenGLImageFormatRGBA, PXOpenGLTypeByteUnsigned, textureText.Image.PixelData);
		PXOpenGLTextureBind(&openGLContext, PXOpenGLTextureType2D, 0);
	}



	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTexEnvi(GL_TEXTURE_ENV, 0x8571, GL_BLEND);// GL_COMBINE_RGB , 0x8570
	///glTexEnvi(GL_TEXTURE_ENV, 0x8571, GL_MODULATE);// GL_COMBINE_RGB , 0x8570
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MULT);



	//-------------------------------------------------------------------------

#if 1 // Mega Text
	float startPositionX = 0;
	float startPositionY = 0;
	float scaleTextureX = 0.5;
	float scaleTextureY = 0.05;

	PXOpenGLTextureBind(&openGLContext, PXOpenGLTextureType2D, textureText.ID);
	RenderRectangle(startPositionX, startPositionY, scaleTextureX, scaleTextureY);
	PXOpenGLTextureBind(&openGLContext, PXOpenGLTextureType2D, 0);
#endif

#if 1 // colored background
	PXOpenGLDrawBegin(&openGLContext, PXOpenGLRenderQuads);
	PXOpenGLDrawVertexXYZF(&openGLContext, -1, -1, 0);  glColor4d(1, 0, 1, 0.2f);
	PXOpenGLDrawVertexXYZF(&openGLContext, 1, -1, 0);  glColor4d(0, 1, 0, 0.2);
	PXOpenGLDrawVertexXYZF(&openGLContext, 1, 1, 0); glColor4d(0, 1, 1, 0.2);
	PXOpenGLDrawVertexXYZF(&openGLContext, -1, 1, 0);  glColor4d(0, 0.5, 0.2, 0.2f);
	PXOpenGLDrawEnd(&openGLContext);
#endif

	//PXOpenGLFlush(&openGLContext);


#if 0 // draw bigText

	glTexCoord2f(0, 0);	PXOpenGLDrawColorRGBF(&openGLContext, 0, 0, 1); PXOpenGLDrawVertexXYZF(&openGLContext, 0, 0.75, 0);
	glTexCoord2f(1, 0); PXOpenGLDrawColorRGBF(&openGLContext, 1, 0, 1); PXOpenGLDrawVertexXYZF(&openGLContext, 0.8, 0.8, 0);
	glTexCoord2f(1, 1); PXOpenGLDrawColorRGBF(&openGLContext, 0, 1, 0); PXOpenGLDrawVertexXYZF(&openGLContext, 0.6, -0.75, 0);
	glTexCoord2f(0, 1); PXOpenGLDrawColorRGBF(&openGLContext, 1, 0, 0); PXOpenGLDrawVertexXYZF(&openGLContext, -0.6, -0.75, 0.5);
#endif

#if 1 // Draw mini text

	{
		PXMatrix4x4F matrix4x4F;

		PXMatrix4x4FIdentity(&matrix4x4F);

		//PXMatrix4x4FRotate(&matrix4x4F, 0, 0, 1, &matrix4x4F);

		GLfloat vertices[] = {
			0, 0,
			0, 1,
			1, 1,
			1, 0,
		};

		GLfloat coords[] = {
			0, 1,
			0, 0,
			1, 0,
			1, 1
		};

		GLfloat colorList[] = {
		1, 1,1,
		1, 1,1,
		1, 1,1,
		1, 1,1
		};

		DrawStructure(&openGLContext, matrix4x4F.Data, vertices, coords, colorList, textureText.ID);
	}
#endif



	/*
	PXOpenGLDrawBegin(&openGLContext, PXOpenGLRenderLineLoop);
	PXOpenGLDrawVertexXYZF(&openGLContext, -.1, .1, 0);
	PXOpenGLDrawVertexXYZF(&openGLContext, .1, .1, 0);
	PXOpenGLDrawVertexXYZF(&openGLContext, .1, -.1, 0);
	PXOpenGLDrawVertexXYZF(&openGLContext, -.1, -.1, 0);
	PXOpenGLDrawEnd(&openGLContext);

	//glFinish();
	*/
	//PXOpenGLFlush(&openGLContext);
	PXOpenGLRenderBufferSwap(&openGLContext);

	// Simply sample the texture


	//PXOpenGLDrawScaleF(&openGLContext, 0.5f, -1, 1);
	//<--- Texture renderer

	//---

	// Rotate?


	//------------------------------

	DumpRenderedTexture(&openGLContext, "_TEST_DATA_OUTPUT_/Buffertest.png");

	PXOpenGLContextDeselect(&openGLContext);
}

void TestPXOpenGLVAO()
{
	PXWindow window;
	PXText pxText;
	PXTextMakeFixedA(&pxText, "RAW Test");
	PXWindowCreate(&window, 0, 0, &pxText, 1);


	PXRenderable renderable;
	PXModel model;

	//PXModelLoad(&model, "B:/Daten/Objects/Moze/Moze.obj");

	while (!window.IsRunning)
	{
		printf(".");
	}

	printf("Windows ok\n");


	PXOpenGLContextSelect(&window.GraphicInstance.OpenGLInstance);

	PXGraphicModelRegisterFromModel(&window.GraphicInstance, &renderable, &model);


	while (1)
	{
		//PXOpenGLVertexArrayBind(&window.GraphicInstance.OpenGLInstance, renderable.ID); ; // VAO
		//GraphicShaderUse(&_mainWindow.GraphicInstance, 1);
		//CameraDataGet(&_mainWindow, 1);
		//CameraDataUpdate(&_mainWindow, MainCamera);

		//GraphicShaderUpdateMatrix4x4F(&_mainWindow.GraphicInstance, _matrixModelID, renderable.MatrixModel.Data);



		PXOpenGLBufferBind(&window.GraphicInstance.OpenGLInstance, PXOpenGLBufferArray, renderable.VBO);

		glDrawBuffer(GL_POINTS);
		//glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);

		PXOpenGLBufferUnbind(&window.GraphicInstance.OpenGLInstance, PXOpenGLBufferArray);

		//PXOpenGLVertexArrayUnbind(&window.GraphicInstance.OpenGLInstance);
	}

}

void TestPXOpenGLVBO()
{
	PXWindow window;
	
	PXText pxText;
	PXTextMakeFixedA(&pxText, "RAW Test");

	PXWindowCreate(&window, -1, -1, &pxText, 1);

	int x = 0;

	while (!window.IsRunning)
	{
		x++;
		//printf(".");
	}

	printf("Windows ok\n");

	PXOpenGLContext* glContext = &window.GraphicInstance.OpenGLInstance;



	//This code will use an IndexBuffer to produce a triangle with 1 inner triangle that isn't drawn.

	PXText vertexShaderSource;
	PXTextMakeFixedNamedA(&vertexShaderSource, vertexShaderSourceBuffer, "#version 330 core \n"
		"layout (location = 0) in vec3 aPos; \n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}");

	PXText fragmentShaderSource;
	PXTextMakeFixedNamedA(&fragmentShaderSource, fragmentShaderSourceBuffer, "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"     FragColor = vec4(0.55f, 0.15f, 0.15f, 1.0f);\n"
		"}");


	PXOpenGLContextSelect(glContext);

	//glViewport(0, 0, 800, 800); //Determining the size of the Window.


	PXShaderProgram shaderProgram;
	shaderProgram.ID = -1;

	PXShader vertexShader;
	PXShader fragmentShader;

	PXShaderDataSet(&vertexShader, PXShaderTypeVertex, vertexShaderSource.TextA, vertexShaderSource.SizeUsed);
	PXShaderDataSet(&fragmentShader, PXShaderTypeFragment, fragmentShaderSource.TextA, fragmentShaderSource.SizeAllocated);
	
	PXGraphicShaderProgramLoadGLSL(&window.GraphicInstance, &shaderProgram, &vertexShader, &fragmentShader);

#if 0
	//Vertices coordinates
	GLfloat vertices[] =
	{
		-1.0f,		-1.0f * (1 / 3.0f),		0.0f, //Lower Left Corner
		1.0f,		-1.0f * (1 /  3.0f),	0.0f, //Lower Right Corner
		0.0f,		1.1f * (1 * 2 / 3.0f),	0.0f,
		-1.0f / 2,	1.0f * (1 / 6.0f),		0.0f, //Inner Left
		1.0f / 2,	1.0f * (1 / 6.0f),		0.0f,//Inner Right
		0.0f,		-1.0f * (1 / 3.0f),		0.0f// Inner Down

	};
	GLuint indices[] =
	{
		0, 3, 5, // Lower Left Triangle
		3, 2, 4, //Lower Right Triangle
		5, 4, 1 //Upper Triangle
	};
#else
	//Vertices coordinates
	GLfloat vertices[] =
	{
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f

	};
	GLuint indices[] =
	{
		0,1,2
	};
#endif

	//Create a reference container to the Vertex Array Object and Vertex Buffer Object
	GLuint VAO, VBO, EBO;
	//Generate the VAO and VBO with only 1 object each.

#define useVAO 1

#if useVAO
	PXOpenGLVertexArrayGenerate(glContext, 1, &VAO);
#endif



	PXOpenGLBufferGenerate(glContext, 1, &VBO);
	PXOpenGLBufferGenerate(glContext, 1, &EBO);

	//Make the VAO the current Vertex Array Object by binding it.
	//
#if useVAO
	PXOpenGLVertexArrayBind(glContext, VAO);
#endif


	//Bind the VBO specifying it's a Buffer Array.

	PXOpenGLBufferBind(glContext, PXOpenGLBufferElementArray, EBO);
	PXOpenGLBufferData(glContext, PXOpenGLBufferElementArray, sizeof(indices), indices, PXOpenGLStoreStaticDraw);
	PXOpenGLBufferUnbind(glContext, PXOpenGLBufferElementArray);

	PXOpenGLBufferBind(glContext, PXOpenGLBufferArray, VBO);
	PXOpenGLBufferData(glContext, PXOpenGLBufferArray, sizeof(vertices), vertices, PXOpenGLStoreStaticDraw);

	PXOpenGLVertexArrayAttributeDefine(glContext, 0, 3, PXOpenGLTypeFloat, GL_FALSE, 3 * sizeof(float), (void*)0);
	PXOpenGLVertexArrayEnable(glContext, 0);

	PXOpenGLBufferUnbind(glContext, PXOpenGLBufferArray);

#if useVAO
	PXOpenGLVertexArrayUnbind(glContext);
#endif





	PXOpenGLShaderProgramUse(glContext, shaderProgram.ID);


#if useVAO
	PXOpenGLVertexArrayBind(glContext, VAO);
#endif


	PXOpenGLBufferBind(glContext, PXOpenGLBufferArray, VBO);
	PXOpenGLBufferBind(glContext, PXOpenGLBufferElementArray, EBO);
	//glFrontFace(GL_CW);
	//glFrontFace(GL_CCW);

	//Declare how many Arrays we want GL to draw.
	glDisable(GL_CULL_FACE);

	glPointSize(5);

	while (window.IsRunning)
	{
		glClearColor(0.20f, 0.20f, 0.20f, 1.0f); //The 3 first values signify RGB numbers and the last digit stand for the Alpha
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glDrawBuffer(GL_POINTS);
		//glDrawArrays(GL_POINTS, 0, 3);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		PXOpenGLFlush(glContext);
		PXOpenGLRenderBufferSwap(glContext);

		//Take care of the glfwEvents
		//glfwPollEvents();
	}
	//glDeleteVertexArrays(1, &EBO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &VAO);
	//glDeleteProgram(shaderProgram);
	//glfwDestroyWindow(window);
	//glfwTerminate();

}

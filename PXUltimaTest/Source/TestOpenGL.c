#include "TestOpenGL.h"


#include <Format/Image.h>
#include <Memory/PXMemory.h>

#include <stdio.h>
#include <float.h>
#include <OS/PXWindow.h>
#include <Format/Font.h>
#include <Math/PXMatrix.h>

#include <Graphic/Graphic.h>
#include <Graphic/OpenGL/OpenGL.h>

void TestOpenGLAll()
{
	printf("[OpenGL] Testing...\n");

	//TestOpenGLRenderToTexture();
	//TestOpenGLTextDraw();
	TestOpenGLVBO();

	printf("[OpenGL] Testing finished!\n");
}

void CreateRenderableTexture(OpenGLContext* openGLContext, unsigned int width, unsigned int height)
{
	OpenGLContextConstruct(openGLContext);
	OpenGLContextCreateWindowless(openGLContext, width, height);
	OpenGLContextSelect(openGLContext);
	//-------------------------------------------------------------------------

	// Init frame Buffer
	unsigned int frameBufferID = 0;
	OpenGLFrameBufferCreate(openGLContext, 1u, &frameBufferID);
	OpenGLFrameBufferBind(openGLContext, OpenGLFrameBufferModeDrawAndRead, frameBufferID);
	//-------------------------------------------------------------------------

	// Init image
	unsigned int textureID = 0;
	OpenGLTextureCreate(openGLContext, 1u, &textureID);
	OpenGLTextureBind(openGLContext, OpenGLTextureType2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	OpenGLTextureBind(openGLContext, OpenGLTextureType2D, 0);
	//-------------------------------------------------------------------------

	OpenGLFrameBufferLinkTexture2D(openGLContext, OpenGLRenderBufferAttachmentPointColor, OpenGLTextureType2D, textureID, 0);



	// init renderbuffer
	unsigned int rbo = 0;
	OpenGLRenderBufferCreate(openGLContext, 1u, &rbo);
	OpenGLRenderBufferBind(openGLContext, rbo);
	OpenGLRenderBufferStorage(openGLContext, OpenGLRenderBufferFormatDepth24Stencil8, width, height);
	OpenGLRenderBufferBind(openGLContext, 0);


	OpenGLFrameBufferLinkRenderBuffer(openGLContext, OpenGLRenderBufferAttachmentPointStencil, rbo);
	//-------------------------------------------------------------------------

	OpenGLClearColor(openGLContext, 0.2f, 0.2f, 0.2f, 1.0f);
	OpenGLClear(openGLContext, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	OpenGLDrawScaleF(openGLContext, 1, -1, 1);

	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
}


void DumpRenderedTexture(OpenGLContext* openGLContext, const char* filePath)
{
	PXWindow* pxWindow = (PXWindow*)openGLContext->AttachedWindow;

	Image image;

	ImageConstruct(&image);

	ImageResize(&image, ImageDataFormatRGB8, pxWindow->Width, pxWindow->Height);

	OpenGLPixelDataRead(openGLContext, 0, 0, pxWindow->Width, pxWindow->Height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image.PixelData);

	ImageSaveA(&image, "_TEST_DATA_OUTPUT_/Buffertest.png", FileFormatPNG, ImageDataFormatRGB8);

	ImageDestruct(&image);
}

void TestOpenGLRenderToTexture()
{
	float scake = 1;
	unsigned int width = 800 * scake;
	unsigned int height = 600 * scake;

	OpenGLContext openGLContext;

	CreateRenderableTexture(&openGLContext, width, height);

	//------------------------------	

	OpenGLDrawBegin(&openGLContext, OpenGLRenderPolygon);
	//glPixelZoom(1, -1);
	//glPerspective(-45.0, width / height, 1.0, 100.0);

	

	OpenGLDrawColorRGBF(&openGLContext, 0, 0, 1); OpenGLDrawVertexXYZF(&openGLContext, 0, 0.75, 0);
	OpenGLDrawColorRGBF(&openGLContext, 1, 0, 1); OpenGLDrawVertexXYZF(&openGLContext, 0.8, 0.8, 0);
	OpenGLDrawColorRGBF(&openGLContext, 0, 1, 0); OpenGLDrawVertexXYZF(&openGLContext, 0.6, -0.75, 0);
	OpenGLDrawColorRGBF(&openGLContext, 1, 0, 0); OpenGLDrawVertexXYZF(&openGLContext, -0.6, -0.75, 0.5);

	OpenGLDrawEnd(&openGLContext);

	//------------------------------

	DumpRenderedTexture(&openGLContext, "_TEST_DATA_OUTPUT_/Buffertest.png");

	OpenGLContextDeselect(&openGLContext);
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

void DrawStructure(OpenGLContext* const openGLContext,float* modelMatrix,  float* vertexList, float* texturePositionList, float* colorList, unsigned int textureID)
{
	OpenGLTextureBind(openGLContext, OpenGLTextureType2D, textureID);
	
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

	OpenGLTextureBind(openGLContext, OpenGLTextureType2D, 0);
}

void TestOpenGLTextDraw()
{
	float scale = 1;
	unsigned int width = 800 * scale;
	unsigned int height = 600 * scale;

	PXFont font;
	FontLoadA(&font, "_TEST_DATA_INPUT_/A.fnt");

	PXTexture textureText;
	PXTextureConstruct(&textureText);

	OpenGLContext openGLContext;
	CreateRenderableTexture(&openGLContext, width, height);

	//------------------------------	


	

	//---<Draw text into texture>----------------------------------------------
	{
		size_t textWidth = 1024u;
		size_t textHeight = 1024;

		OpenGLTextureCreate(&openGLContext, 1u, &textureText.ID);
		OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, textureText.ID);

		ImageResize(&textureText.Image, ImageDataFormatRGBA8, textWidth, textHeight); // Create memory
		ImageFillColorRGBA8(&textureText.Image, 50, 50, 50, 150);

		ImageDrawRectangle(&textureText.Image, 10, 10, 100, 100, 0xFF, 0, 0, 0xFF); // test
		ImageDrawTextA(&textureText.Image, 0, 0, textWidth, textHeight, &font, "Sample Text"); // Draw text

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexEnvi(GL_TEXTURE_ENV, 0x8571, GL_MODULATE);

		OpenGLTextureData2D(&openGLContext, OpenGLTextureType2D, 0, OpenGLImageFormatRGBA, textWidth, textHeight, OpenGLImageFormatRGBA, OpenGLTypeByteUnsigned, textureText.Image.PixelData);
		OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, 0);
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

	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, textureText.ID);
	RenderRectangle(startPositionX, startPositionY, scaleTextureX, scaleTextureY);
	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, 0);
#endif

#if 1 // colored background
	OpenGLDrawBegin(&openGLContext, OpenGLRenderQuads);
	OpenGLDrawVertexXYZF(&openGLContext, -1, -1, 0);  glColor4d(1, 0, 1, 0.2f);
	OpenGLDrawVertexXYZF(&openGLContext, 1, -1, 0);  glColor4d(0, 1, 0, 0.2);
	OpenGLDrawVertexXYZF(&openGLContext, 1, 1, 0); glColor4d(0, 1, 1, 0.2);
	OpenGLDrawVertexXYZF(&openGLContext, -1, 1, 0);  glColor4d(0, 0.5, 0.2, 0.2f);
	OpenGLDrawEnd(&openGLContext);
#endif

	//OpenGLFlush(&openGLContext);


#if 0 // draw bigText

	glTexCoord2f(0, 0);	OpenGLDrawColorRGBF(&openGLContext, 0, 0, 1); OpenGLDrawVertexXYZF(&openGLContext, 0, 0.75, 0);
	glTexCoord2f(1, 0); OpenGLDrawColorRGBF(&openGLContext, 1, 0, 1); OpenGLDrawVertexXYZF(&openGLContext, 0.8, 0.8, 0);
	glTexCoord2f(1, 1); OpenGLDrawColorRGBF(&openGLContext, 0, 1, 0); OpenGLDrawVertexXYZF(&openGLContext, 0.6, -0.75, 0);
	glTexCoord2f(0, 1); OpenGLDrawColorRGBF(&openGLContext, 1, 0, 0); OpenGLDrawVertexXYZF(&openGLContext, -0.6, -0.75, 0.5);
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
	OpenGLDrawBegin(&openGLContext, OpenGLRenderLineLoop);
	OpenGLDrawVertexXYZF(&openGLContext, -.1, .1, 0);
	OpenGLDrawVertexXYZF(&openGLContext, .1, .1, 0);
	OpenGLDrawVertexXYZF(&openGLContext, .1, -.1, 0);
	OpenGLDrawVertexXYZF(&openGLContext, -.1, -.1, 0);
	OpenGLDrawEnd(&openGLContext);

	//glFinish();
	*/
	//OpenGLFlush(&openGLContext);
	OpenGLRenderBufferSwap(&openGLContext);

	// Simply sample the texture


	//OpenGLDrawScaleF(&openGLContext, 0.5f, -1, 1);
	//<--- Texture renderer

	//---

	// Rotate?


	//------------------------------

	DumpRenderedTexture(&openGLContext, "_TEST_DATA_OUTPUT_/Buffertest.png");

	OpenGLContextDeselect(&openGLContext);
}

void TestOpenGLVAO()
{
	PXWindow window;
	PXWindowConstruct(&window);
	PXWindowCreate(&window, 0, 0, "VAO Test", 1);


	PXRenderable renderable;
	Model model;

	ModelLoadA(&model, "B:/Daten/Objects/Moze/Moze.obj");

	while (!window.IsRunning)
	{
		printf(".");
	}

	printf("Windows ok\n");


	OpenGLContextSelect(&window.GraphicInstance.OpenGLInstance);

	GraphicModelRegisterFromModel(&window.GraphicInstance, &renderable, &model);


	while (1)
	{
		//OpenGLVertexArrayBind(&window.GraphicInstance.OpenGLInstance, renderable.ID); ; // VAO
		//GraphicShaderUse(&_mainWindow.GraphicInstance, 1);
		//CameraDataGet(&_mainWindow, 1);
		//CameraDataUpdate(&_mainWindow, MainCamera);

		//GraphicShaderUpdateMatrix4x4F(&_mainWindow.GraphicInstance, _matrixModelID, renderable.MatrixModel.Data);



		OpenGLBufferBind(&window.GraphicInstance.OpenGLInstance, OpenGLBufferArray, renderable.VBO);

		glDrawBuffer(GL_POINTS);
		//glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);

		OpenGLBufferUnbind(&window.GraphicInstance.OpenGLInstance, OpenGLBufferArray);

		//OpenGLVertexArrayUnbind(&window.GraphicInstance.OpenGLInstance);
	}

}

void TestOpenGLVBO()
{
	PXWindow window;
	PXWindowConstruct(&window);
	PXWindowCreate(&window, -1, -1, "RAW Test", 1);

	int x = 0;

	while (!window.IsRunning)
	{
		x++;
		//printf(".");
	}

	printf("Windows ok\n");

	OpenGLContext* glContext = &window.GraphicInstance.OpenGLInstance;



	//This code will use an IndexBuffer to produce a triangle with 1 inner triangle that isn't drawn.
//Vertex Shader Source Code
	const char VertexShaderSource[] = "#version 330 core \n"
		"layout (location = 0) in vec3 aPos; \n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}";
	//Fragment Shader Source Code
	const char fragmentShaderSource[] = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"     FragColor = vec4(0.55f, 0.15f, 0.15f, 1.0f);\n"
		"}";


	OpenGLContextSelect(glContext);

	//glViewport(0, 0, 800, 800); //Determining the size of the Window.


	ShaderProgram shaderProgram;
	shaderProgram.ID = -1;
	
	Shader vertexShader;
	Shader fragmentShader;

	ShaderDataSet(&vertexShader, ShaderTypeVertex, sizeof(VertexShaderSource), VertexShaderSource);
	ShaderDataSet(&fragmentShader, ShaderTypeFragment, sizeof(fragmentShaderSource), fragmentShaderSource);

	GraphicShaderProgramCreateVFData(&window.GraphicInstance, &shaderProgram, &vertexShader, &fragmentShader);

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
	OpenGLVertexArrayGenerate(glContext, 1, &VAO);
#endif



	OpenGLBufferGenerate(glContext, 1, &VBO);
	OpenGLBufferGenerate(glContext, 1, &EBO);

	//Make the VAO the current Vertex Array Object by binding it.
	// 
#if useVAO
	OpenGLVertexArrayBind(glContext, VAO);
#endif


	//Bind the VBO specifying it's a Buffer Array.

	OpenGLBufferBind(glContext, OpenGLBufferElementArray, EBO);
	OpenGLBufferData(glContext, OpenGLBufferElementArray, sizeof(indices), indices, OpenGLStoreStaticDraw);
	OpenGLBufferUnbind(glContext, OpenGLBufferElementArray, 0);

	OpenGLBufferBind(glContext, OpenGLBufferArray, VBO);
	OpenGLBufferData(glContext, OpenGLBufferArray, sizeof(vertices), vertices, OpenGLStoreStaticDraw);

	OpenGLVertexArrayAttributeDefine(glContext, 0, 3, OpenGLTypeFloat, GL_FALSE, 3 * sizeof(float), (void*)0);
	OpenGLVertexArrayEnable(glContext, 0);

	OpenGLBufferUnbind(glContext, OpenGLBufferArray, 0);

#if useVAO
	OpenGLVertexArrayUnbind(glContext);
#endif

	



	OpenGLShaderProgramUse(glContext, shaderProgram.ID);


#if useVAO
	OpenGLVertexArrayBind(glContext, VAO);
#endif


	OpenGLBufferBind(glContext, OpenGLBufferArray, VBO);
	OpenGLBufferBind(glContext, OpenGLBufferElementArray, EBO);
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

		OpenGLFlush(glContext);
		OpenGLRenderBufferSwap(glContext);

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
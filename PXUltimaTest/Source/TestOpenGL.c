#include "TestOpenGL.h"

#include <Graphic/OpenGL/OpenGL.h>
#include <Format/Image.h>
#include <Memory/Memory.h>

#include <stdio.h>
#include <float.h>
#include <OS/PXWindow.h>
#include <Format/Font.h>
#include <Math/Matrix.h>

void TestOpenGLAll()
{
	printf("[OpenGL] Testing...\n");

	//TestOpenGLRenderToTexture();
	TestOpenGLTextDraw();

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

	image.Height = pxWindow->Height;
	image.Width = pxWindow->Width;
	image.Format = ImageDataFormatRGB8;
	image.PixelDataSize = pxWindow->Width * pxWindow->Height * 3u;
	image.PixelData = MemoryAllocate(image.PixelDataSize);

	OpenGLPixelDataRead(openGLContext, 0, 0, pxWindow->Width, pxWindow->Height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image.PixelData);

	ImageSaveA(&image, "_TEST_DATA_OUTPUT_/Buffertest.png", ImageFileFormatPNG, ImageDataFormatRGB8);

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

		PXMatrix4x4FReset(&matrix4x4F);

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
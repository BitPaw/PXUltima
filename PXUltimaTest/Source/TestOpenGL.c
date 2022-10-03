#include "TestOpenGL.h"

#include <Graphic/OpenGL/OpenGL.h>
#include <Format/Image.h>
#include <Memory/Memory.h>

#include <stdio.h>
#include <float.h>
#include <OS/PXWindow.h>
#include <Format/Font.h>

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
}

void DumpRenderedTexture(OpenGLContext* openGLContext, const char* filePath)
{
	PXWindow* pxWindow = (PXWindow*)openGLContext->AttachedWindow;

	//glReadPixels(0,0, width, heigh, ;

	Image image;
	image.Height = pxWindow->Height;
	image.Width = pxWindow->Width;
	image.Format = ImageDataFormatRGB8;
	image.PixelDataSize = pxWindow->Width * pxWindow->Height * 3u;
	image.PixelData = MemoryAllocate(image.PixelDataSize);

	OpenGLPixelDataRead(&openGLContext, 0, 0, pxWindow->Width, pxWindow->Height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image.PixelData);

	ImageSaveA(&image, "_TEST_DATA_OUTPUT_/Buffertest.png", ImageFileFormatPNG, ImageDataFormatRGB8);
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

void TestOpenGLTextDraw()
{
	// Create OpenGL stuff

	// Draw text and rotate, size check

	// Save image




	float scake = 1;
	unsigned int width = 800 * scake;
	unsigned int height = 600 * scake;

	CFont font;
	FontConstruct(&font);

	FontLoadA(&font, "_TEST_DATA_INPUT_/A.fnt");

	OpenGLContext openGLContext;

	CreateRenderableTexture(&openGLContext, width, height);

	//------------------------------	


	//glPixelZoom(1, -1);
	//glPerspective(-45.0, width / height, 1.0, 100.0);

	// Render text to texture (generic, just text

	// Scale texture

	//glActiveTexture(0x84C0);


#if 1
// Put it where is should be set



//OpenGLDrawScaleF(&openGLContext, 1/5.0f, -1, 1);

// Will be rendered to RenderBuffer->Framebuffer->RenderTexture



	glDisable(GL_CULL_FACE); // render both sides


	PXTexture textureTecxt;

	PXTextureConstruct(&textureTecxt);

	glEnable(GL_TEXTURE_2D);

	//---<Draw text into texture>----------------------------------------------
	OpenGLTextureCreate(&openGLContext, 1u, &textureTecxt.ID);
	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, textureTecxt.ID);

	ImageResize(&textureTecxt.Image, ImageDataFormatRGBA8, 1024, 1024); // Create memory

	for (size_t i = 0; i < textureTecxt.Image.PixelDataSize; i += 4)
	{
		((unsigned char*)textureTecxt.Image.PixelData)[i + 0] = 0xFF;
		((unsigned char*)textureTecxt.Image.PixelData)[i + 1] = 0;
		((unsigned char*)textureTecxt.Image.PixelData)[i + 2] = 0;
		((unsigned char*)textureTecxt.Image.PixelData)[i + 3] = 0xFF;
	}

	ImageDrawRectangle(&textureTecxt.Image, 10, 10, 100, 100, 0xFF, 0, 0, 0xFF); // test
	ImageDrawTextA(&textureTecxt.Image, 0, 0, 1024, 1024, &font, "Sample Text"); // Draw text






	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);





	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glTexEnvi(GL_TEXTURE_ENV, 0x8571, GL_BLEND);// GL_COMBINE_RGB , 0x8570
	glTexEnvi(GL_TEXTURE_ENV, 0x8571, GL_ADD);// GL_COMBINE_RGB , 0x8570
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MULT);





	OpenGLTextureData2D(&openGLContext, OpenGLTextureType2D, 0, OpenGLImageFormatRGBA, 1024, 1024, OpenGLImageFormatRGBA, OpenGLTypeByteUnsigned, textureTecxt.Image.PixelData);
	//OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, 0);
	//-------------------------------------------------------------------------


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	float scaleTextureX = 0.5;
	float scaleTextureY = 0.5;

	glBegin(GL_QUADS);

	glColor3f(1, 1, 1);

	glTexCoord2f(0, 1);
	glVertex2f(-1* scaleTextureX, -1* scaleTextureY); // bottom-left

	glTexCoord2f(0, 0);
	glVertex2f(-1* scaleTextureX, 1* scaleTextureY); // top left

	glTexCoord2f(1, 0);
	glVertex2f(1* scaleTextureX, 1* scaleTextureY); // top-right

	glTexCoord2f(1, 1);
	glVertex2f(1* scaleTextureX, -1* scaleTextureY); // top-left

	glEnd();


	OpenGLDrawBegin(&openGLContext, OpenGLRenderQuads);
	OpenGLDrawVertexXYZF(&openGLContext, -1, -1, 0);  glColor4d(1, 0, 1, 0.2f);
	OpenGLDrawVertexXYZF(&openGLContext, 1, -1, 0);  glColor4d(0, 1, 0, 0.2);
	OpenGLDrawVertexXYZF(&openGLContext, 1, 1, 0); glColor4d(0, 1, 1, 0.2);
	OpenGLDrawVertexXYZF(&openGLContext, -1, 1, 0);  glColor4d(0, 0.5, 0.2, 0.2f);
	OpenGLDrawEnd(&openGLContext);

	//OpenGLFlush(&openGLContext);
#if 1


#if 0

	glTexCoord2f(0, 0);	OpenGLDrawColorRGBF(&openGLContext, 0, 0, 1); OpenGLDrawVertexXYZF(&openGLContext, 0, 0.75, 0);
	glTexCoord2f(1, 0); OpenGLDrawColorRGBF(&openGLContext, 1, 0, 1); OpenGLDrawVertexXYZF(&openGLContext, 0.8, 0.8, 0);
	glTexCoord2f(1, 1); OpenGLDrawColorRGBF(&openGLContext, 0, 1, 0); OpenGLDrawVertexXYZF(&openGLContext, 0.6, -0.75, 0);
	glTexCoord2f(0, 1); OpenGLDrawColorRGBF(&openGLContext, 1, 0, 0); OpenGLDrawVertexXYZF(&openGLContext, -0.6, -0.75, 0.5);
#elif 1

	/*

	GLfloat pixels[] = {
1, 0, 0,
0, 1, 0,
0, 0, 1,
1, 1, 1
	};

	GLfloat vertices[] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0,
	};

	GLfloat coords[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};

	glColorPointer(2, GL_FLOAT, 0, coords);
	glTexCoordPointer(2, GL_FLOAT, 0, coords);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	

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

#elif 1

	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, textureTecxt.ID);

	OpenGLDrawBegin(&openGLContext, OpenGLRenderQuads);

	OpenGLDrawVertexXYZF(&openGLContext, -1, -1, 0); glTexCoord2d(1, 1);  glColor4d(1, 0, 1, 0.2f);
	OpenGLDrawVertexXYZF(&openGLContext, 1, -1, 0); glTexCoord2d(0, 1); glColor4d(0, 1, 0, 0.2);
	OpenGLDrawVertexXYZF(&openGLContext, 1, 1, 0); glTexCoord2d(0, 0); glColor4d(0, 1, 1, 0.2);
	OpenGLDrawVertexXYZF(&openGLContext, -1, 1, 0); glTexCoord2d(1, 0); glColor4d(0, 0.5, 0.2, 0.2f);
	OpenGLDrawEnd(&openGLContext);

#else
	OpenGLDrawBegin(&openGLContext, OpenGLRenderQuads);
	OpenGLDrawVertexXYZF(&openGLContext, -1, 1, 0);
	OpenGLDrawVertexXYZF(&openGLContext, 1, 1, 0);
	OpenGLDrawVertexXYZF(&openGLContext, 1, -1, 0);
	OpenGLDrawVertexXYZF(&openGLContext, -1, -1, 0);
	OpenGLDrawEnd(&openGLContext);
#endif




#endif

	// Simply sample the texture


	//OpenGLDrawScaleF(&openGLContext, 0.5f, -1, 1);
	//<--- Texture renderer

	//---



#endif
	// Rotate?


	//------------------------------

	DumpRenderedTexture(&openGLContext, "_TEST_DATA_OUTPUT_/Buffertest.png");

	OpenGLContextDeselect(&openGLContext);
}
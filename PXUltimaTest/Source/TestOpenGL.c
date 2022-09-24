#include "TestOpenGL.h"

#include <Graphic/OpenGL/OpenGL.h>
#include <Format/Image.h>
#include <Memory/Memory.h>

#include <stdio.h>
#include <float.h>
#include <OS/PXWindow.h>

void TestOpenGLAll()
{
	printf("Testing OpenGL...\n");

	TestOpenGLRenderToTexture();
}

void TestOpenGLRenderToTexture()
{
	// Init OpenGL
	unsigned int width = 800;
	unsigned int height = 600;

	OpenGLContext openGLContext;

	OpenGLContextConstruct(&openGLContext);
	OpenGLContextCreate(&openGLContext);
	OpenGLContextSelect(&openGLContext);
	//-------------------------------------------------------------------------

	// Init frame Buffer
	unsigned int frameBufferID = 0;
	OpenGLFrameBufferCreate(&openGLContext, 1u, &frameBufferID);
	OpenGLFrameBufferBind(&openGLContext, OpenGLFrameBufferModeDrawAndRead, frameBufferID);
	//-------------------------------------------------------------------------

	// Init image
	unsigned int textureID = 0;
	OpenGLTextureCreate(&openGLContext, 1u, &textureID);
	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, 0);
	//-------------------------------------------------------------------------

	OpenGLFrameBufferLinkTexture2D(&openGLContext, OpenGLRenderBufferAttachmentPointColor, OpenGLTextureType2D, textureID, 0);



	// init renderbuffer
	unsigned int rbo= 0;
	OpenGLRenderBufferCreate(&openGLContext, 1u, &rbo);
	OpenGLRenderBufferBind(&openGLContext, rbo);
	OpenGLRenderBufferStorage(&openGLContext, OpenGLRenderBufferFormatDepth24Stencil8, width, height);
	OpenGLRenderBufferBind(&openGLContext, 0);


	OpenGLFrameBufferLinkRenderBuffer(&openGLContext, OpenGLRenderBufferAttachmentPointStencil,  rbo);
	//-------------------------------------------------------------------------


	glClearColor(0.2, 0.2, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glScalef(1, -1, 1);

	glBegin(GL_POLYGON);
	//glPixelZoom(1, -1);
	//glPerspective(-45.0, width / height, 1.0, 100.0);


	glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
	//glColor3f(1, 0, 1); glVertex3f(0.8, 0.8, 0);
	glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
	glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
	glEnd();

	glFlush();

	OpenGLRenderBufferSwap(&openGLContext);

	//glReadPixels(0,0, width, heigh, ;

	Image image;
	image.Height = height;
	image.Width = width;
	image.Format = ImageDataFormatRGB8;
	image.PixelDataSize = height * width * 3;
	image.PixelData = MemoryAllocate(image.PixelDataSize);

	OpenGLPixelDataRead(&openGLContext, 0,0,width, height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image.PixelData);

	ImageSaveA(&image, "_TEST_DATA_OUTPUT_/Buffertest.png", ImageFileFormatPNG, ImageDataFormatRGB8);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//OpenGLFrameBufferLinkTexture2D(&openGLContext, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);




}

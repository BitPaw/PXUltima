#include "TestOpenGL.h"

#include <Graphic/OpenGL/OpenGL.h>

#include <stdio.h>
#include <float.h>

void TestOpenGLAll()
{
	printf("Testing OpenGL...\n");

	TestOpenGLRenderToTexture();
}

void TestOpenGLRenderToTexture()
{
	// Init OpenGL
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	OpenGLTextureBind(&openGLContext, OpenGLTextureType2D, 0);
	//-------------------------------------------------------------------------

	OpenGLFrameBufferLinkTexture2D(&openGLContext, GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);


	
	// init renderbuffer
	unsigned int rbo= 0;
	OpenGLRenderBufferCreate(&openGLContext, 1u, &rbo);
	OpenGLRenderBufferBind(&openGLContext, GL_RENDERBUFFER, rbo);
	OpenGLRenderBufferStorage(&openGLContext, GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	OpenGLRenderBufferBind(&openGLContext, GL_RENDERBUFFER, 0);


	OpenGLFrameBufferLinkRenderBuffer(&openGLContext, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	//-------------------------------------------------------------------------



	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, 800, 600, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//OpenGLFrameBufferLinkTexture2D(&openGLContext, GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, textureID, 0);




}
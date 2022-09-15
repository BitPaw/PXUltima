#ifndef OpenGLINCLUDE
#define OpenGLINCLUDE

#include <OS/OSVersion.h>
#include <Format/Type.h>

#if defined(OSUnix)

#include <X11/X.h>
#include <X11/Xlib.h>

#define CWindowID XID // XID is CWindow
#define OpenGLConextID GLXContext

#elif defined(OSWindows)
#include <Windows.h>
#define CWindowID HWND
#define OpenGLConextID HGLRC
#endif

// OpenGL
#if defined(OSUnix)

#include <GL/gl.h>
#include <GL/glx.h>

#elif defined(OSWindows)

#endif


#include <GL/gl.h>

//-----------------



#ifdef __cplusplus
extern "C"
{
#endif
#pragma once

	typedef struct OpenGL_
	{
		OpenGLConextID OpenGLConext;

#if defined(OSUnix)

#elif defined(OSWindows)
		HDC WindowsDeviceContext;
#endif
	}
	OpenGL;

	CPublic void OpenGLContextConstruct(OpenGL* const openGL);
	CPublic void OpenGLContextDestruct(OpenGL* const openGL);


	CPrivate void OpenGLFunctionsFetchAll();
	CPrivate void* OpenGLFunctionFetch(const char* const fuctionName);

	CPublic void OpenGLInitialize(OpenGL* const openGL);

	CPublic void OpenGLContextCreate(OpenGL* const openGL);
	CPublic void OpenGLContextSelect(OpenGL* const openGL);
	CPublic void OpenGLContextDeselect();
	CPublic void OpenGLContextRelease(OpenGL* const openGL);

	CPublic void OpenGLContextFlush();

#ifdef __cplusplus
}
#endif

#endif
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

//---<OpenGL>----------------
#include <GL/gl.h>

#if defined(OSUnix)
#include <GL/glx.h>
#elif defined(OSWindows)

#endif
//---------------------------



#ifdef __cplusplus
extern "C"
{
#endif
#pragma once

#define OpenGLShaderProgramID unsigned int
#define OpenGLShaderID unsigned int

	typedef ptrdiff_t GLintptr;
	typedef ptrdiff_t GLsizeiptr;

	typedef enum OpenGLVersion_
	{
		OpenGLVersionInvalid,
		OpenGLVersionNone,
		OpenGLVersion1x0,
		OpenGLVersion1x1,   // 1995[citation needed] 	Texture objects, Vertex Arrays
		OpenGLVersion1x2,   // March 16, 1998 	3D textures, BGRAand packed pixel formats, [28] introduction of the imaging subset useful to image - processing applications
		OpenGLVersion1x2x1, // October 14, 1998 	A concept of ARB extensions
		OpenGLVersion1x3,   // August 14, 2001 	Multitexturing, multisampling, texture compression
		OpenGLVersion1x4, 	// July 24, 2002 	Depth textures, GLSlang[29]
		OpenGLVersion1x5, 	// July 29, 2003 	Vertex Buffer Object(VBO), Occlusion Queries[30]
		OpenGLVersion2x0, 	// September 7, 2004 	GLSL 1.1, MRT, Non Power of Two textures, Point Sprites, [31] Two - sided stencil[30]
		OpenGLVersion2x1, 	// July 2, 2006 	GLSL 1.2, Pixel Buffer Object(PBO), sRGB Textures[30]
		OpenGLVersion3x0, 	// August 11, 2008 	GLSL 1.3, Texture Arrays, Conditional rendering, Frame Buffer Object(FBO)[32]
		OpenGLVersion3x1, 	// March 24, 2009 	GLSL 1.4, Instancing, Texture Buffer Object, Uniform Buffer Object, Primitive restart[33]
		OpenGLVersion3x2, 	// August 3, 2009 	GLSL 1.5, Geometry Shader, Multi - sampled textures[34]
		OpenGLVersion3x3, 	// March 11, 2010 	GLSL 3.30, Backports as much function as possible from the OpenGL 4.0 specification
		OpenGLVersion4x0, 	// March 11, 2010 	GLSL 4.00, Tessellation on GPU, shaders with 64 - bit precision[35]
		OpenGLVersion4x1, 	// July 26, 2010 	GLSL 4.10, Developer - friendly debug outputs, compatibility with OpenGL ES 2.0[36]
		OpenGLVersion4x2, 	// August 8, 2011[37] 	GLSL 4.20, Shaders with atomic counters, draw transform feedback instanced, shader packing, performance improvements
		OpenGLVersion4x3, 	// August 6, 2012[38] 	GLSL 4.30, Compute shaders leveraging GPU parallelism, shader storage buffer objects, high - quality ETC2 / EAC texture compression, increased memory security, a multi - application robustness extension, compatibility with OpenGL ES 3.0[39]
		OpenGLVersion4x4, 	// July 22, 2013[40] 	GLSL 4.40, Buffer Placement Control, Efficient Asynchronous Queries, Shader Variable Layout, Efficient Multiple Object Binding, Streamlined Porting of Direct3D applications, Bindless Texture Extension, Sparse Texture Extension[40]
		OpenGLVersion4x5,	// August 11, 2014[12][41] 	GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness, OpenGL ES 3.1 API and shader compatibility, DX11 emulation features
		OpenGLVersion4x6	// July 31, 2017[7][42] 	GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering 
	}
	OpenGLVersion;

	//---<Added 2.0>-----------------------------------------------------------
	typedef unsigned int (*OpenGLShaderProgramCreateFunction)();
	typedef void (*OpenGLShaderProgramUseFunction)(unsigned int program);
	typedef void (*OpenGLShaderSourceFunction)(unsigned int shader, int count, const char** string, const int* length);
	typedef void (*OpenGLCompileShaderFunction)(GLuint shader);	
	typedef void (*OpenGLShaderGetivFunction)(GLuint shader, GLenum pname, GLint* params);
	typedef void (*OpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);
	typedef void (*OpenGLShaderDeleteFunction)(GLuint shader);
	typedef void (*OpenGLAttachShaderFunction)(GLuint program,	GLuint shader); // glAttachShader
	typedef void (*OpenGLLinkProgramFunction)(GLuint program); // glLinkProgram
	typedef void (*OpenGLValidateProgramFunction)(GLuint program); // glValidateProgram
	typedef void (*OpenGLActiveTextureFunction)(GLenum texture); // glActiveTexture
	typedef void (*OpenGLGenBuffersFunction)(GLsizei n, GLuint* buffers); // glGenBuffers
	typedef void (*OpenGLBindBufferFunction)(GLenum target, GLuint buffer);
	typedef void (*OpenGLBufferDataFunction)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (*OpenGLVertexAttribPointerFunction)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void (*OpenGLDisableVertexAttribArrayFunction)(GLuint index);
	typedef void (*OpenGLDisableVertexArrayAttribFunction)(GLuint vaobj, GLuint index);
	typedef GLint (*OpenGLGetUniformLocation)(GLuint program, const char* name);
	typedef void (*OpenGLUniform1fFunction)(GLint location, GLfloat v0);
	typedef void (*OpenGLUniform1fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (*OpenGLUniform1iFunction)(GLint location, GLint v0);
	typedef void (*OpenGLUniform1ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (*OpenGLUniform2fFunction)(GLint location, GLfloat v0, GLfloat v1);
	typedef void (*OpenGLUniform2fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (*OpenGLUniform2iFunction)(GLint location, GLint v0, GLint v1);
	typedef void (*OpenGLUniform2ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (*OpenGLUniform3fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (*OpenGLUniform3fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (*OpenGLUniform3iFunction)(GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (*OpenGLUniform3ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (*OpenGLUniform4fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (*OpenGLUniform4fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (*OpenGLUniform4iFunction)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (*OpenGLUniform4ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (*OpenGLUniformMatrix2fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (*OpenGLUniformMatrix3fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (*OpenGLUniformMatrix4fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	//-------------------------------------------------------------------------

	//---<Added 3.0>-----------------------------------------------------------
	typedef void (*OpenGLGenVertexArraysFunction)(GLsizei n, GLuint* arrays);
	typedef void (*OpenGLBindVertexArrayFunction)(GLuint arrayID);	
	typedef void (*OpenGLVertexAttribIPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	//-------------------------------------------------------------------------

	//---<Added 4.1>-----------------------------------------------------------	
	typedef void (*OpenGLVertexAttribLPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	//-------------------------------------------------------------------------

	//---<Added 4.3>-----------------------------------------------------------
	typedef void (APIENTRY* DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	typedef void (*OpenGLDebugMessageCallback)(DEBUGPROC callback, const void* userParam); // Set
	//-------------------------------------------------------------------------

	//---<Added 4.5>-----------------------------------------------------------
	typedef void (*OpenGLNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (*OpenGLEnableVertexAttribArray)(GLuint index);
	typedef void (*OpenGLEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
	//-------------------------------------------------------------------------

	typedef struct OpenGL_
	{
		OpenGLConextID OpenGLConext;

		char Vendor[64];
		char Renderer[64];
		char VersionText[64];
		OpenGLVersion Version;

#if defined(OSUnix)

#elif defined(OSWindows)
		HDC WindowsDeviceContext;
#endif

		OpenGLShaderProgramCreateFunction	OpenGLShaderProgramCreateCallBack;
		OpenGLShaderProgramUseFunction	OpenGLShaderProgramUseCallBack;
		OpenGLShaderSourceFunction	OpenGLShaderSourceCallBack;
		OpenGLCompileShaderFunction	OpenGLCompileShaderCallBack;
		OpenGLShaderGetivFunction	OpenGLShaderGetivCallBack;
		OpenGLShaderLogInfoGetFunction	OpenGLShaderLogInfoGetCallBack;
		OpenGLShaderDeleteFunction	OpenGLShaderDeleteCallBack;
		OpenGLAttachShaderFunction	OpenGLAttachShaderCallBack;
		OpenGLLinkProgramFunction	OpenGLLinkProgramCallBack;
		OpenGLValidateProgramFunction	OpenGLValidateProgramCallBack;
		OpenGLActiveTextureFunction	OpenGLActiveTextureCallBack;
		OpenGLGenBuffersFunction	OpenGLGenBuffersCallBack;
		OpenGLBindBufferFunction	OpenGLBindBufferCallBack;
		OpenGLBufferDataFunction	OpenGLBufferDataCallBack;
		OpenGLVertexAttribPointerFunction OpenGLVertexAttribPointerCallBack;
		OpenGLDisableVertexAttribArrayFunction	OpenGLDisableVertexAttribArrayCallBack;
		OpenGLDisableVertexArrayAttribFunction	OpenGLDisableVertexArrayAttribCallBack;
		OpenGLGetUniformLocation	OpenGLGetUniformLocation;
		OpenGLUniform1fFunction	OpenGLUniform1fCallBack;
		OpenGLUniform1fvFunction	OpenGLUniform1fvCallBack;
		OpenGLUniform1iFunction	OpenGLUniform1iCallBack;
		OpenGLUniform1ivFunction	OpenGLUniform1ivCallBack;
		OpenGLUniform2fFunction	OpenGLUniform2fCallBack;
		OpenGLUniform2fvFunction	OpenGLUniform2fvCallBack;
		OpenGLUniform2iFunction	OpenGLUniform2iCallBack;
		OpenGLUniform2ivFunction	OpenGLUniform2ivCallBack;
		OpenGLUniform3fFunction	OpenGLUniform3fCallBack;
		OpenGLUniform3fvFunction	OpenGLUniform3fvCallBack;
		OpenGLUniform3iFunction	OpenGLUniform3iCallBack;
		OpenGLUniform3ivFunction	OpenGLUniform3ivCallBack;
		OpenGLUniform4fFunction	OpenGLUniform4fCallBack;
		OpenGLUniform4fvFunction	OpenGLUniform4fvCallBack;
		OpenGLUniform4iFunction	OpenGLUniform4iCallBack;
		OpenGLUniform4ivFunction	OpenGLUniform4ivCallBack;
		OpenGLUniformMatrix2fvFunction	OpenGLUniformMatrix2fvCallBack;
		OpenGLUniformMatrix3fvFunction	OpenGLUniformMatrix3fvCallBack;
		OpenGLUniformMatrix4fvFunction OpenGLUniformMatrix4fvCallBack;

		OpenGLGenVertexArraysFunction OpenGLGenVertexArraysCallBack;
		OpenGLBindVertexArrayFunction OpenGLBindVertexArrayCallBack;
		OpenGLVertexAttribIPointerFunction OpenGLVertexAttribIPointerCallBack;	

		OpenGLVertexAttribLPointerFunction	OpenGLVertexAttribLPointerCallBack;
	}
	OpenGL;

	CPublic void OpenGLContextConstruct(OpenGL* const openGL);
	CPublic void OpenGLContextDestruct(OpenGL* const openGL);


	CPrivate void OpenGLFunctionsFetchAll(OpenGL* const openGL);
	CPrivate void* OpenGLFunctionFetch(const char* const fuctionName);

	CPublic void OpenGLInitialize(OpenGL* const openGL);

	CPublic void OpenGLContextCreate(OpenGL* const openGL);
	CPublic void OpenGLContextSelect(OpenGL* const openGL);
	CPublic void OpenGLContextDeselect();
	CPublic void OpenGLContextRelease(OpenGL* const openGL);

	CPublic void OpenGLContextFlush();

	// [Version 2] Extended functions
	CPublic OpenGLShaderProgramID OpenGLShaderProgramCreate(OpenGL* const openGL);
	CPublic void OpenGLShaderProgramUse(OpenGL* const openGL, const OpenGLShaderProgramID shaderProgramID);
	CPublic void OpenGLShaderSource(OpenGL* const openGL, unsigned int shader, int count, const char** string, const int* length);
	CPublic void OpenGLCompileShader(OpenGL* const openGL, const OpenGLShaderID shaderID);
	CPublic void OpenGLShaderGetiv(OpenGL* const openGL, const OpenGLShaderID shaderID, GLenum pname, GLint* params);
	CPublic void OpenGLShaderLogInfoGet(OpenGL* const openGL, const OpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog);
	CPublic void OpenGLShaderDelete(OpenGL* const openGL, const OpenGLShaderID shaderID);


#ifdef __cplusplus
}
#endif

#endif
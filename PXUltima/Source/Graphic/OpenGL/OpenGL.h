#ifndef OpenGLINCLUDE
#define OpenGLINCLUDE

#include <OS/OSVersion.h>

#if defined(OSUnix)

#include <X11/X.h>
#include <X11/Xlib.h>

#define PXWindowID XID // XID is PXWindow
#define OpenGLConextID GLXContext

#elif defined(OSWindows)
#include <Windows.h>
#define PXWindowID HWND
#define OpenGLConextID HGLRC
#endif

//---<OpenGL>----------------
#include <GL/gl.h>

#if defined(OSUnix)
#include <GL/glx.h>
#define OpenGLAPICallType
#elif defined(OSWindows)
#define OpenGLAPICallType APIENTRY
#endif
//---------------------------

#include <Format/Type.h>

#define OpenGLID unsigned int
#define OpenGLShaderProgramID unsigned int
#define OpenGLShaderID unsigned int

#ifdef __cplusplus
extern "C"
{
#endif

	//-------------------------------------------------------------------------
	typedef ptrdiff_t GLintptr;
	typedef ptrdiff_t GLsizeiptr;

	typedef enum OpenGLVersion_
	{
		OpenGLVersionInvalid,
		OpenGLVersionNone,
		OpenGLVersion1x0x0,
		OpenGLVersion1x1x0, // 1995[citation needed]. Texture objects, Vertex Arrays
		OpenGLVersion1x2x0, // March 16, 1998. 3D textures, BGRAand packed pixel formats, [28] introduction of the imaging subset useful to image - processing applications
		OpenGLVersion1x2x1, // October 14, 1998. A concept of ARB extensions
		OpenGLVersion1x3x0, // August 14, 2001. Multitexturing, multisampling, texture compression
		OpenGLVersion1x4x0, // July 24, 2002. Depth textures, GLSlang[29]
		OpenGLVersion1x5x0, // July 29, 2003. Vertex Buffer Object(VBO), Occlusion Queries[30]
		OpenGLVersion2x0x0, // September 7, 2004. GLSL 1.1, MRT, Non Power of Two textures, Point Sprites, [31] Two - sided stencil[30]
		OpenGLVersion2x1x0, // July 2, 2006. GLSL 1.2, Pixel Buffer Object(PBO), sRGB Textures[30]
		OpenGLVersion3x0x0, // August 11, 2008. GLSL 1.3, Texture Arrays, Conditional rendering, Frame Buffer Object(FBO)[32]
		OpenGLVersion3x1x0, // March 24, 2009. GLSL 1.4, Instancing, Texture Buffer Object, Uniform Buffer Object, Primitive restart[33]
		OpenGLVersion3x2x0, // August 3, 2009. GLSL 1.5, Geometry Shader, Multi - sampled textures[34]
		OpenGLVersion3x3x0, // March 11, 2010. GLSL 3.30, Backports as much function as possible from the OpenGL 4.0 specification
		OpenGLVersion4x0x0, // March 11, 2010. GLSL 4.00, Tessellation on GPU, shaders with 64 - bit precision[35]
		OpenGLVersion4x1x0, // July 26, 2010. GLSL 4.10, Developer - friendly debug outputs, compatibility with OpenGL ES 2.0[36]
		OpenGLVersion4x2x0, // August 8, 2011. GLSL 4.20, Shaders with atomic counters, draw transform feedback instanced, shader packing, performance improvements
		OpenGLVersion4x3x0, // August 6, 2012. GLSL 4.30, Compute shaders leveraging GPU parallelism, shader storage buffer objects, high - quality ETC2 / EAC texture compression, increased memory security, a multi - application robustness extension, compatibility with OpenGL ES 3.0[39]
		OpenGLVersion4x4x0, // July 22, 2013. GLSL 4.40, Buffer Placement Control, Efficient Asynchronous Queries, Shader Variable Layout, Efficient Multiple Object Binding, Streamlined Porting of Direct3D applications, Bindless Texture Extension, Sparse Texture Extension[40]
		OpenGLVersion4x5x0,	// August 11, 2014. GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness, OpenGL ES 3.1 API and shader compatibility, DX11 emulation features
		OpenGLVersion4x6x0	// July 31, 2017. GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering
	}
	OpenGLVersion;

	typedef enum OpenGLTextureType_
	{
		OpenGLTextureTypeInvalid,
		OpenGLTextureType1D,
		OpenGLTextureType2D,
		OpenGLTextureType3D,
		OpenGLTextureType1DArray,
		OpenGLTextureType2DArray,
		OpenGLTextureTypeRectangle,
		OpenGLTextureTypeCubeMap,
		OpenGLTextureTypeCubleMapArray,
		OpenGLTextureTypeBuffer,
		OpenGLTextureType2DMultiSample,
		OpenGLTextureType2DMultiSampleArray,
	}
	OpenGLTextureType;

	typedef enum OpenGLFrameBufferMode_
	{
		OpenGLFrameBufferModeInvalid,
		OpenGLFrameBufferModeDraw,
		OpenGLFrameBufferModeRead,
		OpenGLFrameBufferModeDrawAndRead
	}
	OpenGLFrameBufferMode;

	typedef enum OpenGLShaderType_
	{
		OpenGLShaderInvalid,
		OpenGLShaderVertex,
		OpenGLShaderFragment,
		OpenGLShaderTessellationControl,
		OpenGLShaderTessellationEvaluation,
		OpenGLShaderGeometry,
		OpenGLShaderCompute
	}
	OpenGLShaderType;

	typedef enum OpenGLRenderBufferAttachmentPoint_
	{
		OpenGLRenderBufferAttachmentPointInvalid,
		OpenGLRenderBufferAttachmentPointColor,
		OpenGLRenderBufferAttachmentPointDepth,
		OpenGLRenderBufferAttachmentPointStencil
	}
	OpenGLRenderBufferAttachmentPoint;

	CPrivate unsigned int OpenGLRenderBufferAttachmentPointToID(const OpenGLRenderBufferAttachmentPoint renderBufferAttachmentPoint);



	typedef enum OpenGLImageFormat_
	{
		OpenGLImageFormatInvalid,
		OpenGLImageFormatRGB,
		OpenGLImageFormatRGBA,
		OpenGLImageFormatBGR,
		OpenGLImageFormatBGRA,
		OpenGLImageFormatRed,
		OpenGLImageFormatGreen,
		OpenGLImageFormatBlue,
		OpenGLImageFormatAlpha,
		OpenGLImageFormatLuminance,
		OpenGLImageFormatLuminanceAlpha,
		OpenGLImageFormatColorIndex,
		OpenGLImageFormatStencilIndex,
		OpenGLImageFormatDepthComponent
	}
	OpenGLImageFormat;

	CPrivate int OpenGLImageFormatToID(const OpenGLImageFormat imageFormat);

	typedef enum OpenGLDataType_
	{
		OpenGLTypeInvalid,

		OpenGLTypeByteSigned,  // 1 Byte, signed
		OpenGLTypeByteUnsigned, // 1 Byte, unsigned

		OpenGLTypeShortSigned,  // 2 Byte, signed
		OpenGLTypeShortUnsigned,  // 2 Byte, unsigned

		OpenGLTypeIntegerSigned,  // 4 Byte, signed
		OpenGLTypeIntegerUnsigned,  // 4 Byte, unsigned

		OpenGLTypeFloat,  // 4 Byte
		OpenGLTypeDouble,  // 8 Byte
	}
	OpenGLDataType;

	CPrivate int OpenGLDataTypeToID(const OpenGLDataType dataType);

	typedef enum OpenGLRenderBufferFormat_
	{
		OpenGLRenderBufferFormatInvalid,
		OpenGLRenderBufferFormatDepth24Stencil8
	}
	OpenGLRenderBufferFormat;

	CPrivate int OpenGLRenderBufferFormatToID(const OpenGLRenderBufferFormat dataType);


	//-------------------------------------------------------------------------

	//---<OpenGL v.1.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.2.1>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.3.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.4.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.5.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.2.0.0>------------------------------------------------------
	CPrivate unsigned int OpenGLTextureTypeToID(const OpenGLTextureType openGLTextureType);

	typedef unsigned int (OpenGLAPICallType* OpenGLShaderProgramCreateFunction)();
	typedef void (OpenGLAPICallType* OpenGLShaderProgramUseFunction)(unsigned int program);
	typedef void (OpenGLAPICallType* OpenGLShaderProgramDeleteFunction)(GLuint program); // glDeleteProgram
	typedef void (OpenGLAPICallType* OpenGLShaderSourceFunction)(unsigned int shader, int count, const char** string, const int* length);
	typedef GLuint(OpenGLAPICallType* OpenGLShaderCreateFunction)(GLenum shaderType); // glCreateShader
	typedef void (OpenGLAPICallType* OpenGLShaderCompileFunction)(GLuint shader);
	typedef void (OpenGLAPICallType* OpenGLShaderGetivFunction)(GLuint shader, GLenum pname, GLint* params);
	typedef void (OpenGLAPICallType* OpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);
	typedef void (OpenGLAPICallType* OpenGLShaderDeleteFunction)(GLuint shader);
	typedef void (OpenGLAPICallType* OpenGLAttachShaderFunction)(GLuint program, GLuint shader); // glAttachShader
	typedef void (OpenGLAPICallType* OpenGLLinkProgramFunction)(GLuint program); // glLinkProgram
	typedef void (OpenGLAPICallType* OpenGLValidateProgramFunction)(GLuint program); // glValidateProgram
	typedef void (OpenGLAPICallType* OpenGLActiveTextureFunction)(GLenum texture); // glActiveTexture
	typedef void (OpenGLAPICallType* OpenGLGenBuffersFunction)(GLsizei n, GLuint* buffers); // glGenBuffers
	typedef void (OpenGLAPICallType* OpenGLBindBufferFunction)(GLenum target, GLuint buffer);
	typedef void (OpenGLAPICallType* OpenGLBufferDataFunction)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (OpenGLAPICallType* OpenGLVertexAttribPointerFunction)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void (OpenGLAPICallType* OpenGLDisableVertexAttribArrayFunction)(GLuint index);
	typedef void (OpenGLAPICallType* OpenGLDisableVertexArrayAttribFunction)(GLuint vaobj, GLuint index);
	typedef GLint(OpenGLAPICallType* OpenGLGetUniformLocation)(GLuint program, const char* name);
	typedef void (OpenGLAPICallType* OpenGLUniform1fFunction)(GLint location, GLfloat v0);
	typedef void (OpenGLAPICallType* OpenGLUniform1fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (OpenGLAPICallType* OpenGLUniform1iFunction)(GLint location, GLint v0);
	typedef void (OpenGLAPICallType* OpenGLUniform1ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (OpenGLAPICallType* OpenGLUniform2fFunction)(GLint location, GLfloat v0, GLfloat v1);
	typedef void (OpenGLAPICallType* OpenGLUniform2fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (OpenGLAPICallType* OpenGLUniform2iFunction)(GLint location, GLint v0, GLint v1);
	typedef void (OpenGLAPICallType* OpenGLUniform2ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (OpenGLAPICallType* OpenGLUniform3fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (OpenGLAPICallType* OpenGLUniform3fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (OpenGLAPICallType* OpenGLUniform3iFunction)(GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (OpenGLAPICallType* OpenGLUniform3ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (OpenGLAPICallType* OpenGLUniform4fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (OpenGLAPICallType* OpenGLUniform4fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (OpenGLAPICallType* OpenGLUniform4iFunction)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (OpenGLAPICallType* OpenGLUniform4ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (OpenGLAPICallType* OpenGLUniformMatrix2fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (OpenGLAPICallType* OpenGLUniformMatrix3fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (OpenGLAPICallType* OpenGLUniformMatrix4fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

	typedef void (OpenGLAPICallType* OpenGLTextureCreateFunction)(GLsizei n, GLuint* textures); // glGenTextures
	typedef void (OpenGLAPICallType* OpenGLTextureBindFunction)(GLenum target, GLuint texture); // glBindTexture
	typedef void (OpenGLAPICallType* OpenGLTextureDeleteFunction)(GLsizei n, const GLuint* textures); // glDeleteTextures
    //-------------------------------------------------------------------------

	//---<OpenGL v.2.1.0>------------------------------------------------------
    //-------------------------------------------------------------------------

    //---<OpenGL v.3.0.0>------------------------------------------------------
	typedef void (OpenGLAPICallType* OpenGLFrameBufferCreateFunction)(GLsizei n, GLuint* ids); // glGenFramebuffers
	typedef void (OpenGLAPICallType* OpenGLFrameBufferDeleteFunction)(GLsizei n, GLuint* framebuffers); // glDeleteFramebuffers
	typedef void (OpenGLAPICallType* OpenGLFrameBufferBindFunction)(GLenum target, GLuint framebuffer); // glBindFramebuffer

	typedef void (OpenGLAPICallType* OpenGLRenderBufferCreateFunction)(GLsizei n, GLuint* renderbuffers); // glGenRenderbuffers
	typedef void (OpenGLAPICallType* OpenGLRenderBufferBindFunction)(GLenum target, GLuint renderbuffer); // glBindRenderbuffer
	typedef void (OpenGLAPICallType* OpenGLRenderBufferDeleteFunction)(GLsizei n, GLuint* renderbuffers); // glDeleteRenderbuffers
	typedef void (OpenGLAPICallType* OpenGLRenderBufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height); // glRenderbufferStorage

	typedef void (OpenGLAPICallType* OpenGLFrameBufferLinkTexture2DFunction)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef GLuint (OpenGLAPICallType* OpenGLFrameBufferLinkRenderBufferFunction)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

	typedef void (OpenGLAPICallType* OpenGLGenVertexArraysFunction)(GLsizei n, GLuint* arrays);
	typedef void (OpenGLAPICallType* OpenGLBindVertexArrayFunction)(GLuint arrayID);
	typedef void (OpenGLAPICallType* OpenGLVertexAttribIPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	//-------------------------------------------------------------------------

	//---<OpenGL v.3.1.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.3.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------


	//---<OpenGL v.4.0.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.4.1.0>------------------------------------------------------
	typedef void (OpenGLAPICallType * OpenGLVertexAttribLPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	//-------------------------------------------------------------------------

	//---<OpenGL v.4.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.4.3.0>------------------------------------------------------
	typedef void (OpenGLAPICallType*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	typedef void (OpenGLAPICallType*OpenGLDebugMessageFunction)(DEBUGPROC callback, const void* userParam); // Set
	//-------------------------------------------------------------------------

	//---<OpenGL v.4.4.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.4.5.0>------------------------------------------------------
	typedef void (OpenGLAPICallType*OpenGLNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (OpenGLAPICallType*OpenGLEnableVertexAttribArray)(GLuint index);
	typedef void (OpenGLAPICallType*OpenGLEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
	//-------------------------------------------------------------------------

	//---<OpenGL v.4.6.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	typedef struct OpenGLContext_
	{
		OpenGLConextID OpenGLConext;

		char Vendor[64];
		char Renderer[64];
		char VersionText[64];
		OpenGLVersion Version;

        void* AttachedWindow;

		OpenGLShaderProgramCreateFunction	OpenGLShaderProgramCreateCallBack;
		OpenGLShaderProgramUseFunction	OpenGLShaderProgramUseCallBack;
		OpenGLShaderProgramDeleteFunction OpenGLShaderProgramDeleteCallBack;
		OpenGLShaderCreateFunction OpenGLShaderCreateCallBack;
		OpenGLShaderSourceFunction	OpenGLShaderSourceCallBack;
		OpenGLShaderCompileFunction	OpenGLShaderCompileCallBack;
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

		OpenGLTextureCreateFunction OpenGLTextureCreateCallBack;
		OpenGLTextureBindFunction OpenGLTextureBindCallBack;
		OpenGLTextureDeleteFunction OpenGLTextureDeleteCallBack;

		// 3
		OpenGLFrameBufferCreateFunction OpenGLFrameBufferCreateCallBack;
		OpenGLFrameBufferDeleteFunction OpenGLFrameBufferDeleteCallBack;
		OpenGLFrameBufferBindFunction OpenGLFrameBufferBindCallBack;

		OpenGLRenderBufferCreateFunction OpenGLRenderBufferCreateCallBack;
		OpenGLRenderBufferBindFunction  OpenGLRenderBufferBindCallBack;
		OpenGLRenderBufferDeleteFunction OpenGLRenderBufferDeleteCallBack;
		OpenGLRenderBufferStorageFunction OpenGLRenderBufferStorageCallBack;
		OpenGLFrameBufferLinkTexture2DFunction OpenGLFrameBufferLinkTexture2DCallBack;
		OpenGLFrameBufferLinkRenderBufferFunction OpenGLFrameBufferLinkRenderBufferCallBack;

		OpenGLGenVertexArraysFunction OpenGLGenVertexArraysCallBack;
		OpenGLBindVertexArrayFunction OpenGLBindVertexArrayCallBack;
		OpenGLVertexAttribIPointerFunction OpenGLVertexAttribIPointerCallBack;

		OpenGLVertexAttribLPointerFunction	OpenGLVertexAttribLPointerCallBack;

		OpenGLDebugMessageFunction OpenGLDebugMessageCallback;
	}
	OpenGLContext;

	CPrivate OpenGLVersion OpenGLVersionParse(const unsigned int versionID);
	CPrivate void OpenGLCacheFunction(void** loadList, size_t* currentSize, char* name, void* functionADress);
    CPrivate void* OpenGLFunctionAdressFetch(const char* const functionName);

	CPublic void OpenGLContextConstruct(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextDestruct(OpenGLContext* const openGLContext);

	CPublic void OpenGLContextCreate(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextSelect(OpenGLContext* const openGLContext);
	CPublic unsigned char  OpenGLContextDeselect(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextRelease(OpenGLContext* const openGLContext);

	CPublic void OpenGLRenderBufferSwap(OpenGLContext* const openGLContext);

	CPublic void OpenGLContextFlush();

	CPublic void OpenGLAPICallType OpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	// [Version 2] Extended functions
	CPublic OpenGLShaderProgramID OpenGLShaderProgramCreate(OpenGLContext* const openGLContext);
	CPublic void OpenGLShaderProgramUse(OpenGLContext* const openGLContext, const OpenGLShaderProgramID shaderProgramID);
	CPublic void OpenGLShaderProgramDelete(OpenGLContext* const openGLContext, const OpenGLShaderProgramID shaderProgramID);

	CPublic unsigned int OpenGLShaderTypeToID(const OpenGLShaderType openGLShaderType);
	CPublic OpenGLShaderID OpenGLShaderCreate(OpenGLContext* const openGLContext, const OpenGLShaderType openGLShaderType);
	CPublic void OpenGLShaderSource(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID, int count, const char** string, const int* length);
	CPublic unsigned char OpenGLShaderCompile(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID);
	CPublic void OpenGLShaderGetiv(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID, GLenum pname, GLint* params);
	CPublic void OpenGLShaderLogInfoGet(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog);
	CPublic void OpenGLShaderDelete(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID);


	CPublic void OpenGLShaderProgramAttach(OpenGLContext* const openGLContext, const OpenGLShaderProgramID shaderProgramID, const OpenGLShaderID shaderID);
	CPublic void OpenGLShaderProgramLink(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID);
	CPublic void OpenGLShaderProgramValidate(OpenGLContext* const openGLContext, const OpenGLShaderID shaderID);


	CPublic void OpenGLTextureCreate(OpenGLContext* const openGLContext, GLsizei n, GLuint* textures); // glGenTextures
	CPublic void OpenGLTextureBind(OpenGLContext* const openGLContext, const OpenGLTextureType textureType, GLuint texture); // glBindTexture
	CPublic void OpenGLTextureDelete(OpenGLContext* const openGLContext, GLsizei n, const GLuint* textures); // glDeleteTextures


	// [Version 3.0]

	CPublic void OpenGLFrameBufferCreate(OpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);
	CPublic void OpenGLRenderBufferStorage(OpenGLContext* const openGLContext, const OpenGLRenderBufferFormat internalformat, const int width, const int height);


	// If we bind to 0, we select the "main window"
	CPublic void OpenGLFrameBufferBind(OpenGLContext* const openGLContext, const OpenGLFrameBufferMode target, const unsigned int framebufferID);

	CPublic void OpenGLFrameBufferDestroy(OpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);

	CPublic void OpenGLRenderBufferCreate(OpenGLContext* const openGLContext, GLsizei n, GLuint* renderbuffers);


	CPublic void OpenGLRenderBufferBind(OpenGLContext* const openGLContext, const unsigned int renderbuffer);

	CPublic void OpenGLRenderBufferDelete(OpenGLContext* const openGLContext, GLsizei n, GLuint* renderbuffers);


	CPublic void OpenGLFrameBufferLinkTexture2D(OpenGLContext* const openGLContext, const OpenGLRenderBufferAttachmentPoint attachment, const OpenGLTextureType textarget, const unsigned int textureID, const int level);
	CPublic GLuint OpenGLFrameBufferLinkRenderBuffer(OpenGLContext* const openGLContext, const OpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer);



	CPublic GLint OpenGLShaderVariableIDGet(OpenGLContext* const openGLContext, GLuint program, const char* name);
	CPublic void OpenGLShaderVariableFx1(OpenGLContext* const openGLContext, GLint location, GLfloat v0);
	CPublic void OpenGLShaderVariableFx1xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	CPublic void OpenGLShaderVariableIx1(OpenGLContext* const openGLContext, GLint location, GLint v0);
	CPublic void OpenGLShaderVariableIx1xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	CPublic void OpenGLShaderVariableFx2(OpenGLContext* const openGLContext, GLint location, GLfloat v0, GLfloat v1);
	CPublic void OpenGLShaderVariableFx2xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	CPublic void OpenGLShaderVariableIx2(OpenGLContext* const openGLContext, GLint location, GLint v0, GLint v1);
	CPublic void OpenGLShaderVariableIx2xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	CPublic void OpenGLShaderVariableFx3(OpenGLContext* const openGLContext, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	CPublic void OpenGLShaderVariableFx3xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	CPublic void OpenGLShaderVariableIx3(OpenGLContext* const openGLContext, GLint location, GLint v0, GLint v1, GLint v2);
	CPublic void OpenGLShaderVariableIx3xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	CPublic void OpenGLShaderVariableFx4(OpenGLContext* const openGLContext, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	CPublic void OpenGLShaderVariableFx4xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	CPublic void OpenGLShaderVariableIx4(OpenGLContext* const openGLContext, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	CPublic void OpenGLShaderVariableIx4xN(OpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	CPublic void OpenGLShaderVariableMatrix2fv(OpenGLContext* const openGLContext, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	CPublic void OpenGLShaderVariableMatrix3fv(OpenGLContext* const openGLContext, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	CPublic void OpenGLShaderVariableMatrix4fv(OpenGLContext* const openGLContext, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);




	//---<OpenGL v.1.0.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.1.0>------------------------------------------------------
	CPublic void OpenGLPixelDataRead
	(
		OpenGLContext* const openGLContext,
		const int x,
		const int y,
		const int width,
		const int height,
		OpenGLImageFormat imageFormat,
		OpenGLDataType openGLDataType,
		void* const pixelData
	);
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<OpenGL v.1.2.1>------------------------------------------------------
    //-------------------------------------------------------------------------

	//---<OpenGL v.1.3.0>------------------------------------------------------
	//-------------------------------------------------------------------------

//---<OpenGL v.1.4.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.1.5.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.2.0.0>------------------------------------------------------

//-------------------------------------------------------------------------

//---<OpenGL v.2.1.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.3.0.0>------------------------------------------------------

//-------------------------------------------------------------------------

//---<OpenGL v.3.1.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.3.2.0>------------------------------------------------------
//-------------------------------------------------------------------------


//---<OpenGL v.4.0.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.4.1.0>------------------------------------------------------

//-------------------------------------------------------------------------

//---<OpenGL v.4.2.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.4.3.0>------------------------------------------------------

//-------------------------------------------------------------------------

//---<OpenGL v.4.4.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<OpenGL v.4.5.0>------------------------------------------------------

//-------------------------------------------------------------------------

//---<OpenGL v.4.6.0>------------------------------------------------------
//-------------------------------------------------------------------------






	/*
	CPublic OpenGLID OpenGLToRenderMode(const GraphicRenderMode renderMode);
	CPublic OpenGLID OpenGLToShaderType(const ShaderType shaderType);
	CPublic OpenGLID OpenGLToImageFormat(const ImageDataFormat imageFormat);
	CPublic OpenGLID OpenGLToImageType(const GraphicImageType imageType);
	CPublic OpenGLID OpenGLToImageWrap(const GraphicImageWrap imageWrap);
	CPublic OpenGLID OpenGLToImageLayout(const GraphicImageLayout layout);











	CPublic void OpenGLVertexAttributeArrayDefine(OpenGL* const openGL, const size_t sizeOfElement, const size_t listSize, const unsigned int* list);

	CPublic void VertexArrayDefine(unsigned int* vertexArrayID);
	CPublic void VertexDataDefine(unsigned int* vertexID, int size, void* data);
	CPublic void IndexDataDefine(unsigned int* indexID, int size, void* data);
	CPublic void VertexArrayUpdate(int vertexArrayID, int size, void* data);
	*/

#ifdef __cplusplus
}
#endif

#endif

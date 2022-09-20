#ifndef OpenGLINCLUDE
#define OpenGLINCLUDE

#include <OS/OSVersion.h>

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

#include <Format/Type.h>
#include <Graphics/Graphics.h>

#define OpenGLID unsigned int
#define OpenGLShaderProgramID unsigned int
#define OpenGLShaderID unsigned int

#ifdef __cplusplus
extern "C"
{
#endif

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

	// Version 1.3
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_SUBTRACT 0x84E7
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_MULTISAMPLE_BIT 0x20000000

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
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_DISPLAY_LIST 0x82E7
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0

	typedef void (* DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	typedef void (*OpenGLDebugMessageFunction)(DEBUGPROC callback, const void* userParam); // Set
	//-------------------------------------------------------------------------

	//---<Added 4.5>-----------------------------------------------------------
	typedef void (*OpenGLNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (*OpenGLEnableVertexAttribArray)(GLuint index);
	typedef void (*OpenGLEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
	//-------------------------------------------------------------------------

	typedef struct OpenGLContext_
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

		OpenGLDebugMessageFunction OpenGLDebugMessageCallback;
	}
	OpenGLContext;


	CPrivate OpenGLVersion OpenGLVersionParse(const unsigned int versionID);
	CPrivate void OpenGLCacheFunction(void** loadList, size_t* currentSize, char* name, void* functionADress);

	CPublic void OpenGLContextConstruct(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextDestruct(OpenGLContext* const openGLContext);

	CPublic void OpenGLContextCreate(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextSelect(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextDeselect(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextRelease(OpenGLContext* const openGLContext);

	CPublic void OpenGLContextFlush();


	CPublic void OpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	/*
	CPublic OpenGLID OpenGLToRenderMode(const GraphicRenderMode renderMode);
	CPublic OpenGLID OpenGLToShaderType(const ShaderType shaderType);
	CPublic OpenGLID OpenGLToImageFormat(const ImageDataFormat imageFormat);
	CPublic OpenGLID OpenGLToImageType(const GraphicImageType imageType);
	CPublic OpenGLID OpenGLToImageWrap(const GraphicImageWrap imageWrap);
	CPublic OpenGLID OpenGLToImageLayout(const GraphicImageLayout layout);
	

	




	// [Version 2] Extended functions
	CPublic OpenGLShaderProgramID OpenGLShaderProgramCreate(OpenGL* const openGL);
	CPublic void OpenGLShaderProgramUse(OpenGL* const openGL, const OpenGLShaderProgramID shaderProgramID);
	CPublic void OpenGLShaderSource(OpenGL* const openGL, unsigned int shader, int count, const char** string, const int* length);
	CPublic void OpenGLShaderCompile(OpenGL* const openGL, const OpenGLShaderID shaderID);
	CPublic void OpenGLShaderGetiv(OpenGL* const openGL, const OpenGLShaderID shaderID, GLenum pname, GLint* params);
	CPublic void OpenGLShaderLogInfoGet(OpenGL* const openGL, const OpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog);
	CPublic void OpenGLShaderDelete(OpenGL* const openGL, const OpenGLShaderID shaderID);



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
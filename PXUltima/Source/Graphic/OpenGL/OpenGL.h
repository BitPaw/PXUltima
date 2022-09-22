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
#elif defined(OSWindows)

#endif
//---------------------------

#include <Format/Type.h>

#define OpenGLID unsigned int
#define OpenGLShaderProgramID unsigned int
#define OpenGLShaderID unsigned int
#define GLAPIENTRY APIENTRY 

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

#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_POINT_SPRITE 0x8861
#define GL_COORD_REPLACE 0x8862
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5


// Texture, sparse
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_CUBE_MAP_ARRAY_OES 0x9009
#define GL_VIRTUAL_PAGE_SIZE_X_EXT 0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_EXT 0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_EXT 0x9197
#define GL_MAX_SPARSE_TEXTURE_SIZE_EXT 0x9198
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_EXT 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_EXT 0x919A
#define GL_TEXTURE_SPARSE_EXT 0x91A6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_EXT 0x91A7
#define GL_NUM_VIRTUAL_PAGE_SIZES_EXT 0x91A8
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_EXT 0x91A9
#define GL_NUM_SPARSE_LEVELS_EXT 0x91AA

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
	
CPrivate unsigned int OpenGLTextureTypeToID(const OpenGLTextureType openGLTextureType);


	typedef unsigned int (APIENTRY *OpenGLShaderProgramCreateFunction)();
	typedef void (APIENTRY * OpenGLShaderProgramUseFunction)(unsigned int program);
	typedef void (APIENTRY * OpenGLShaderProgramDeleteFunction)(GLuint program); // glDeleteProgram
	typedef void (APIENTRY * OpenGLShaderSourceFunction)(unsigned int shader, int count, const char** string, const int* length);
	typedef GLuint (APIENTRY * OpenGLShaderCreateFunction)(GLenum shaderType); // glCreateShader
	typedef void (APIENTRY * OpenGLShaderCompileFunction)(GLuint shader);	
	typedef void (APIENTRY * OpenGLShaderGetivFunction)(GLuint shader, GLenum pname, GLint* params);
	typedef void (APIENTRY * OpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);
	typedef void (APIENTRY * OpenGLShaderDeleteFunction)(GLuint shader);
	typedef void (APIENTRY * OpenGLAttachShaderFunction)(GLuint program,	GLuint shader); // glAttachShader
	typedef void (APIENTRY * OpenGLLinkProgramFunction)(GLuint program); // glLinkProgram
	typedef void (APIENTRY * OpenGLValidateProgramFunction)(GLuint program); // glValidateProgram
	typedef void (APIENTRY * OpenGLActiveTextureFunction)(GLenum texture); // glActiveTexture
	typedef void (APIENTRY * OpenGLGenBuffersFunction)(GLsizei n, GLuint* buffers); // glGenBuffers
	typedef void (APIENTRY * OpenGLBindBufferFunction)(GLenum target, GLuint buffer);
	typedef void (APIENTRY * OpenGLBufferDataFunction)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (APIENTRY * OpenGLVertexAttribPointerFunction)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void (APIENTRY * OpenGLDisableVertexAttribArrayFunction)(GLuint index);
	typedef void (APIENTRY * OpenGLDisableVertexArrayAttribFunction)(GLuint vaobj, GLuint index);
	typedef GLint (APIENTRY * OpenGLGetUniformLocation)(GLuint program, const char* name);
	typedef void (APIENTRY * OpenGLUniform1fFunction)(GLint location, GLfloat v0);
	typedef void (APIENTRY * OpenGLUniform1fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (APIENTRY * OpenGLUniform1iFunction)(GLint location, GLint v0);
	typedef void (APIENTRY * OpenGLUniform1ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (APIENTRY * OpenGLUniform2fFunction)(GLint location, GLfloat v0, GLfloat v1);
	typedef void (APIENTRY * OpenGLUniform2fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (APIENTRY * OpenGLUniform2iFunction)(GLint location, GLint v0, GLint v1);
	typedef void (APIENTRY * OpenGLUniform2ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (APIENTRY * OpenGLUniform3fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (APIENTRY * OpenGLUniform3fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (APIENTRY * OpenGLUniform3iFunction)(GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (APIENTRY * OpenGLUniform3ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (APIENTRY * OpenGLUniform4fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (APIENTRY * OpenGLUniform4fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (APIENTRY * OpenGLUniform4iFunction)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (APIENTRY * OpenGLUniform4ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (APIENTRY * OpenGLUniformMatrix2fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (APIENTRY * OpenGLUniformMatrix3fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (APIENTRY * OpenGLUniformMatrix4fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

	typedef void (APIENTRY * OpenGLTextureCreateFunction)(GLsizei n, GLuint* textures); // glGenTextures
	typedef void (APIENTRY * OpenGLTextureBindFunction)(GLenum target, GLuint texture); // glBindTexture
	typedef void (APIENTRY * OpenGLTextureDeleteFunction)(GLsizei n, const GLuint* textures); // glDeleteTextures
	//-------------------------------------------------------------------------

	//---<Added 3.0>-----------------------------------------------------------

#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_DEPTH_BUFFER 0x8223
#define GL_STENCIL_BUFFER 0x8224
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_ALPHA_INTEGER 0x8D97
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16



	typedef void (GLAPIENTRY* OpenGLFrameBufferCreateFunction)(GLsizei n, GLuint* ids); // glGenFramebuffers
	typedef void (GLAPIENTRY* OpenGLFrameBufferDeleteFunction)(GLsizei n, GLuint* framebuffers); // glDeleteFramebuffers
	typedef void (GLAPIENTRY* OpenGLFrameBufferBindFunction)(GLenum target, GLuint framebuffer); // glBindFramebuffer

	typedef void (GLAPIENTRY* OpenGLRenderBufferCreateFunction)(GLsizei n, GLuint* renderbuffers); // glGenRenderbuffers
	typedef void (GLAPIENTRY* OpenGLRenderBufferBindFunction)(GLenum target, GLuint renderbuffer); // glBindRenderbuffer
	typedef void (GLAPIENTRY* OpenGLRenderBufferDeleteFunction)(GLsizei n, GLuint* renderbuffers); // glDeleteRenderbuffers
	typedef void (GLAPIENTRY* OpenGLRenderBufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height); // glRenderbufferStorage

	typedef void (GLAPIENTRY* OpenGLFrameBufferLinkTexture2DFunction)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef GLuint (GLAPIENTRY* OpenGLFrameBufferLinkRenderBufferFunction)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);



	typedef void (GLAPIENTRY* OpenGLGenVertexArraysFunction)(GLsizei n, GLuint* arrays);
	typedef void (GLAPIENTRY* OpenGLBindVertexArrayFunction)(GLuint arrayID);
	typedef void (GLAPIENTRY* OpenGLVertexAttribIPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	//-------------------------------------------------------------------------

	// 3.2
#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126


	// 4.0
#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F


	//---<Added 4.1>-----------------------------------------------------------	
	typedef void (APIENTRY * OpenGLVertexAttribLPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
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

	typedef void (GLAPIENTRY *DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);

	typedef void (GLAPIENTRY *OpenGLDebugMessageFunction)(DEBUGPROC callback, const void* userParam); // Set
	//-------------------------------------------------------------------------

	//---<Added 4.5>-----------------------------------------------------------
	typedef void (GLAPIENTRY *OpenGLNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (GLAPIENTRY *OpenGLEnableVertexAttribArray)(GLuint index);
	typedef void (GLAPIENTRY *OpenGLEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
	//-------------------------------------------------------------------------




	// extension - Frabbuffer
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_INDEX 0x8222
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_SRGB 0x8C40
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57
	//------------------------















	typedef struct OpenGLContext_
	{
		OpenGLConextID OpenGLConext;

		char Vendor[64];
		char Renderer[64];
		char VersionText[64];
		OpenGLVersion Version;

#if defined(OSUnix)

#elif defined(OSWindows)
		void* AttachedWindow;
#endif

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


	CPrivate OpenGLVersion OpenGLVersionParse(const unsigned int versionID);
	CPrivate void OpenGLCacheFunction(void** loadList, size_t* currentSize, char* name, void* functionADress);

	CPublic void OpenGLContextConstruct(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextDestruct(OpenGLContext* const openGLContext);

	CPublic void OpenGLContextCreate(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextSelect(OpenGLContext* const openGLContext);
	CPublic unsigned char  OpenGLContextDeselect(OpenGLContext* const openGLContext);
	CPublic void OpenGLContextRelease(OpenGLContext* const openGLContext);

	CPublic void OpenGLContextFlush();


	CPublic void APIENTRY OpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);





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
	CPublic void OpenGLRenderBufferStorage(OpenGLContext* const openGLContext, GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	

	// If we bind to 0, we select the "main window" 
	CPublic void OpenGLFrameBufferBind(OpenGLContext* const openGLContext, const OpenGLFrameBufferMode target, const unsigned int framebufferID);

	CPublic void OpenGLFrameBufferDestroy(OpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);

	CPublic void OpenGLRenderBufferCreate(OpenGLContext* const openGLContext, GLsizei n, GLuint* renderbuffers);


	CPublic void OpenGLRenderBufferBind(OpenGLContext* const openGLContext, GLenum target, GLuint renderbuffer); 

	CPublic void OpenGLRenderBufferDelete(OpenGLContext* const openGLContext, GLsizei n, GLuint* renderbuffers);


	CPublic void OpenGLFrameBufferLinkTexture2D(OpenGLContext* const openGLContext, GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	CPublic GLuint OpenGLFrameBufferLinkRenderBuffer(OpenGLContext* const openGLContext, GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);



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